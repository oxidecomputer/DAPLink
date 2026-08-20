/**
 * @file    uart.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2020 Arm Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "string.h"
#include "fsl_device_registers.h"
#include "fsl_usart_cmsis.h"
#include "uart.h"
#include "util.h"
#include "cortex_m.h"
#include "circ_buf.h"
#include "settings.h" // for config_get_overflow_detect

#define USART_INSTANCE (Driver_USART0)
#define USART_IRQ      (FLEXCOMM0_IRQn)
// RX/TX completion is delivered through the DMA controller's shared IRQ,
// not FLEXCOMM0_IRQn, now that USART0 uses the DMA-backed CMSIS driver.
#define USART_DMA_IRQ  (DMA0_IRQn)

extern uint32_t SystemCoreClock;

static void clear_buffers(void);
static void uart_rx_start_chunk(void);
static void uart_rx_flush(void);

#define BUFFER_SIZE         (512)

// Size of each DMA RX transfer. The USART DMA driver only signals
// completion once this many bytes have been received, so uart_rx_flush()
// polls DMA progress within the in-flight chunk (via GetRxCount()) to
// forward bytes to read_buffer without waiting for the full chunk.
#define DMA_RX_CHUNK_SIZE   (256)

circ_buf_t write_buffer;
uint8_t write_buffer_data[BUFFER_SIZE];
circ_buf_t read_buffer;
uint8_t read_buffer_data[BUFFER_SIZE];

static uint8_t dma_rx_chunk[DMA_RX_CHUNK_SIZE];
// Number of bytes already copied out of dma_rx_chunk into read_buffer for
// the in-flight DMA transfer.
static uint32_t dma_rx_flushed;

struct {
    // Number of bytes pending to be transferred. This is 0 if there is no
    // ongoing transfer and the uart_handler processed the last transfer.
    volatile uint32_t tx_size;
} cb_buf;

void uart_handler(uint32_t event);

void clear_buffers(void)
{
    circ_buf_init(&write_buffer, write_buffer_data, sizeof(write_buffer_data));
    circ_buf_init(&read_buffer, read_buffer_data, sizeof(read_buffer_data));
    dma_rx_flushed = 0;
}

int32_t uart_initialize(void)
{
    clear_buffers();
    cb_buf.tx_size = 0;
    // The DMA-backed USART CMSIS driver (USART_DmaPowerControl) touches DMA0
    // registers directly but never brings up the DMA0 peripheral itself.
    // DMA_Init() ungates its clock, releases its reset, programs its SRAM
    // descriptor table base, and sets its master enable bit; without it,
    // accessing DMA0's registers below hangs the AHB bus since DMA0's clock
    // is gated off by default.
    DMA_Init(DMA0);
    USART_INSTANCE.Initialize(uart_handler);
    USART_INSTANCE.PowerControl(ARM_POWER_FULL);

    return 1;
}

int32_t uart_uninitialize(void)
{
    USART_INSTANCE.Control(ARM_USART_CONTROL_RX, 0);
    USART_INSTANCE.Control(ARM_USART_ABORT_RECEIVE, 0U);
    USART_INSTANCE.PowerControl(ARM_POWER_OFF);
    USART_INSTANCE.Uninitialize();
    clear_buffers();
    cb_buf.tx_size = 0;

    return 1;
}

int32_t uart_reset(void)
{
    // disable interrupt
    NVIC_DisableIRQ(USART_IRQ);
    NVIC_DisableIRQ(USART_DMA_IRQ);
    clear_buffers();
    if (cb_buf.tx_size != 0) {
        USART_INSTANCE.Control(ARM_USART_ABORT_SEND, 0U);
        cb_buf.tx_size = 0;
    }
    // Abort and re-arm the in-flight DMA receive too. Otherwise it keeps
    // running against the dma_rx_flushed watermark that clear_buffers() just
    // reset to 0, and the next flush re-copies the chunk's already-flushed
    // prefix into (now empty) read_buffer, duplicating already-sent bytes.
    USART_INSTANCE.Control(ARM_USART_ABORT_RECEIVE, 0U);
    uart_rx_start_chunk();
    // enable interrupt
    NVIC_EnableIRQ(USART_DMA_IRQ);
    NVIC_EnableIRQ(USART_IRQ);

    return 1;
}

int32_t uart_set_configuration(UART_Configuration *config)
{
    uint32_t control = ARM_USART_MODE_ASYNCHRONOUS;

    switch (config->DataBits) {
        case UART_DATA_BITS_5:
            control |= ARM_USART_DATA_BITS_5;
            break;

        case UART_DATA_BITS_6:
            control |= ARM_USART_DATA_BITS_6;
            break;

        case UART_DATA_BITS_7:
            control |= ARM_USART_DATA_BITS_7;
            break;

        case UART_DATA_BITS_8: /* fallthrough */
        default:
            control |= ARM_USART_DATA_BITS_8;
            break;
    }

    switch (config->Parity) {
        case UART_PARITY_EVEN:
            control |= ARM_USART_PARITY_EVEN;
            break;

        case UART_PARITY_ODD:
            control |= ARM_USART_PARITY_ODD;
            break;

        case UART_PARITY_NONE: /* fallthrough */
        default:
            control |= ARM_USART_PARITY_NONE;
            break;
    }

    switch (config->StopBits) {
        case UART_STOP_BITS_1: /* fallthrough */
        default:
            control |= ARM_USART_STOP_BITS_1;
            break;

        case UART_STOP_BITS_1_5:
            control |= ARM_USART_STOP_BITS_1_5;
            break;

        case UART_STOP_BITS_2:
            control |= ARM_USART_STOP_BITS_2;
            break;
    }

    switch (config->FlowControl) {
        case UART_FLOW_CONTROL_NONE: /* fallthrough */
        default:
            control |= ARM_USART_FLOW_CONTROL_NONE;
            break;

        case UART_FLOW_CONTROL_RTS_CTS:
            control |= ARM_USART_FLOW_CONTROL_RTS_CTS;
            break;
    }

    NVIC_DisableIRQ(USART_IRQ);
    NVIC_DisableIRQ(USART_DMA_IRQ);
    clear_buffers();
    if (cb_buf.tx_size != 0) {
        USART_INSTANCE.Control(ARM_USART_ABORT_SEND, 0U);
        cb_buf.tx_size = 0;
    }

    // If there was no Receive() call in progress aborting it is harmless.
    USART_INSTANCE.Control(ARM_USART_CONTROL_RX, 0U);
    USART_INSTANCE.Control(ARM_USART_ABORT_RECEIVE, 0U);

    uint32_t r = USART_INSTANCE.Control(control, config->Baudrate);
    if (r != ARM_DRIVER_OK) {
        return 0;
    }
    USART_INSTANCE.Control(ARM_USART_CONTROL_TX, 1);
    USART_INSTANCE.Control(ARM_USART_CONTROL_RX, 1);
    uart_rx_start_chunk();

    NVIC_ClearPendingIRQ(USART_IRQ);
    NVIC_EnableIRQ(USART_DMA_IRQ);
    NVIC_EnableIRQ(USART_IRQ);

    return 1;
}

int32_t uart_get_configuration(UART_Configuration *config)
{
    return 1;
}

// TODO implement
void uart_set_control_line_state(uint16_t ctrl_bmp)
{
}

// get the available space in the write buffer
int32_t uart_write_free(void)
{
    return circ_buf_count_free(&write_buffer);
}

// Start a new TX transfer if there are bytes pending to be transferred on the
// write_buffer buffer. The transferred bytes are not removed from the circular
// by this function, only the event handler will remove them once the transfer
// is done.
static void uart_start_tx_transfer() {
    uint32_t tx_size = 0;
    const uint8_t* buf = circ_buf_peek(&write_buffer, &tx_size);
    if (tx_size > BUFFER_SIZE / 4) {
        // The bytes being transferred remain on the circular buffer memory
        // until the transfer is done. Limiting the UART transfer size
        // allows the uart_handler to clear those bytes earlier.
        tx_size = BUFFER_SIZE / 4;
    }
    cb_buf.tx_size = tx_size;
    if (tx_size) {
        USART_INSTANCE.Send(buf, tx_size);
    }
}

int32_t uart_write_data(uint8_t *data, uint16_t size)
{
    if (size == 0) {
        return 0;
    }

    // TODO is the logic here correct?
    uint32_t cnt = circ_buf_write(&write_buffer, data, size);
    if (cb_buf.tx_size == 0) {
        // There's no pending transfer and the value of cb_buf.tx_size will not
        // change to non-zero by the event handler once it is zero. Note that it
        // is entirely possible that we transferred all the bytes we added to
        // the circular buffer in this function by the time we are in this
        // branch, in that case uart_start_tx_transfer() would not schedule any
        // transfer.
        uart_start_tx_transfer();
    }

    return cnt;
}

// (Re)arm a fresh DMA_RX_CHUNK_SIZE-byte receive into dma_rx_chunk. Callers
// are expected to already be holding the USART_IRQ/USART_DMA_IRQ critical
// section (either explicitly, or by virtue of running inside uart_handler()).
static void uart_rx_start_chunk(void)
{
    dma_rx_flushed = 0;
    USART_INSTANCE.Receive(dma_rx_chunk, sizeof(dma_rx_chunk));
}

// Copies any bytes that have arrived in the in-flight DMA RX chunk into
// read_buffer, and arms the next chunk once the current one is fully
// received and drained. This is what lets RX data reach read_buffer well
// before a full DMA_RX_CHUNK_SIZE chunk completes: it's polled from
// uart_read_data() (called frequently by the main loop) in addition to
// being called on ARM_USART_EVENT_RECEIVE_COMPLETE.
static void uart_rx_flush(void)
{
    NVIC_DisableIRQ(USART_IRQ);
    NVIC_DisableIRQ(USART_DMA_IRQ);

    uint32_t received = (uint32_t)USART_INSTANCE.GetRxCount();
    if (received > sizeof(dma_rx_chunk)) {
        received = sizeof(dma_rx_chunk);
    }

    if (received > dma_rx_flushed) {
        uint32_t new_bytes = received - dma_rx_flushed;
        uint32_t free = circ_buf_count_free(&read_buffer);
        if (new_bytes > free) {
            // read_buffer isn't draining fast enough; drop what doesn't fit
            // rather than overwrite or block. The un-flushed remainder is
            // retried on the next call once read_buffer has room again.
            new_bytes = free;
        }
        if (new_bytes) {
            circ_buf_write(&read_buffer, &dma_rx_chunk[dma_rx_flushed], new_bytes);
        }
        dma_rx_flushed += new_bytes;
    }

    // Only start the next chunk once this one has been fully drained above;
    // otherwise the still-unflushed tail would be overwritten in place.
    if (received == sizeof(dma_rx_chunk) && dma_rx_flushed == received) {
        uart_rx_start_chunk();
    }

    NVIC_EnableIRQ(USART_DMA_IRQ);
    NVIC_EnableIRQ(USART_IRQ);
}

int32_t uart_read_data(uint8_t *data, uint16_t size)
{
    uart_rx_flush();
    return circ_buf_read(&read_buffer, data, size);
}

void uart_handler(uint32_t event) {
   if (event & ARM_USART_EVENT_RECEIVE_COMPLETE) {
        // The in-flight DMA chunk finished; pull in whatever arrived and
        // arm the next chunk. uart_rx_flush() is also polled from
        // uart_read_data(), so data reaches read_buffer well before this
        // per-chunk completion event fires.
        uart_rx_flush();
    }

    if (event & ARM_USART_EVENT_SEND_COMPLETE) {
        circ_buf_pop_n(&write_buffer, cb_buf.tx_size);
        uart_start_tx_transfer();
    }
}

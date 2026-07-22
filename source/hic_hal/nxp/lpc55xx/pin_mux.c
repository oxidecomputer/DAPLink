/*
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

#include "fsl_clock.h"
#include "fsl_iocon.h"
#include "pin_mux.h"
#include "IO_Config.h"

uint32_t USART0_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(0U);
}

void USART0_InitPins(void)
{
    /* Enables the clock for the I/O controller.: Enable Clock. */
    CLOCK_EnableClock(kCLOCK_Iocon);

    const uint32_t cfg_rx = (/* Pin is configured as FC0_RXD_SDA_MOSI_DATA */
        IOCON_PIO_FUNC1 |
        /* No addition pin function */
        IOCON_PIO_MODE_INACT |
        /* Standard mode, output slew rate control is enabled */
        IOCON_PIO_SLEW_STANDARD |
        /* Input function is not inverted */
        IOCON_PIO_INV_DI |
        /* Enables digital function */
        IOCON_PIO_DIGITAL_EN |
        /* Open drain is disabled */
        IOCON_PIO_OPENDRAIN_DI);
    IOCON_PinMuxSet(IOCON, PIN_PIO_PORT, PIN_UART_RX, cfg_rx);

    const uint32_t cfg_tx = (/* Pin is configured as FC0_TXD_SCL_MISO_WS */
        IOCON_PIO_FUNC1 |
        /* No addition pin function */
        IOCON_PIO_MODE_INACT |
        /* Standard mode, output slew rate control is enabled */
        IOCON_PIO_SLEW_STANDARD |
        /* Input function is not inverted */
        IOCON_PIO_INV_DI |
        /* Enables digital function */
        IOCON_PIO_DIGITAL_EN |
        /* Open drain is disabled */
        IOCON_PIO_OPENDRAIN_DI);
    IOCON_PinMuxSet(IOCON, PIN_PIO_PORT, PIN_UART_TX, cfg_tx);

    const uint32_t cfg_cts = (/* Pin is configured as FC0_CTS_SDAX_SSEL0 */
        IOCON_PIO_FUNC1 |
        /* No addition pin function */
        IOCON_PIO_MODE_INACT |
        /* Standard mode, output slew rate control is enabled */
        IOCON_PIO_SLEW_STANDARD |
        /* Input function is not inverted */
        IOCON_PIO_INV_DI |
        /* Enables digital function */
        IOCON_PIO_DIGITAL_EN |
        /* Open drain is disabled */
        IOCON_PIO_OPENDRAIN_DI);
    IOCON_PinMuxSet(IOCON, PIN_UART_CTS_PORT, PIN_UART_CTS, cfg_cts);

    const uint32_t cfg_rts = (/* Pin is configured as FC0_RTS_SDLX_SSEL1 */
        IOCON_PIO_FUNC1 |
        /* No addition pin function */
        IOCON_PIO_MODE_INACT |
        /* Standard mode, output slew rate control is enabled */
        IOCON_PIO_SLEW_STANDARD |
        /* Input function is not inverted */
        IOCON_PIO_INV_DI |
        /* Enables digital function */
        IOCON_PIO_DIGITAL_EN |
        /* Open drain is disabled */
        IOCON_PIO_OPENDRAIN_DI);
    IOCON_PinMuxSet(IOCON, PIN_UART_RTS_PORT, PIN_UART_RTS, cfg_rts);
}

void USART0_DeinitPins(void)
{
    /* Enables the clock for the I/O controller.: Enable Clock. */
    CLOCK_EnableClock(kCLOCK_Iocon);

    const uint32_t cfg_rx = (/* Pin is configured as FC0_RXD_SDA_MOSI_DATA */
        IOCON_PIO_FUNC0 |
        /* No addition pin function */
        IOCON_PIO_MODE_INACT |
        /* Standard mode, output slew rate control is enabled */
        IOCON_PIO_SLEW_STANDARD |
        /* Input function is not inverted */
        IOCON_PIO_INV_DI |
        /* Enables digital function */
        IOCON_PIO_DIGITAL_EN |
        /* Open drain is disabled */
        IOCON_PIO_OPENDRAIN_DI);
    IOCON_PinMuxSet(IOCON, PIN_PIO_PORT, PIN_UART_RX, cfg_rx);

    const uint32_t cfg_tx = (/* Pin is configured as FC0_TXD_SCL_MISO_WS */
        IOCON_PIO_FUNC0 |
        /* No addition pin function */
        IOCON_PIO_MODE_INACT |
        /* Standard mode, output slew rate control is enabled */
        IOCON_PIO_SLEW_STANDARD |
        /* Input function is not inverted */
        IOCON_PIO_INV_DI |
        /* Enables digital function */
        IOCON_PIO_DIGITAL_EN |
        /* Open drain is disabled */
        IOCON_PIO_OPENDRAIN_DI);
    IOCON_PinMuxSet(IOCON, PIN_PIO_PORT, PIN_UART_TX, cfg_tx);

    const uint32_t cfg_cts = (/* Pin is configured as FC0_CTS_SDAX_SSEL0 */
        IOCON_PIO_FUNC0 |
        /* No addition pin function */
        IOCON_PIO_MODE_INACT |
        /* Standard mode, output slew rate control is enabled */
        IOCON_PIO_SLEW_STANDARD |
        /* Input function is not inverted */
        IOCON_PIO_INV_DI |
        /* Enables digital function */
        IOCON_PIO_DIGITAL_EN |
        /* Open drain is disabled */
        IOCON_PIO_OPENDRAIN_DI);
    IOCON_PinMuxSet(IOCON, PIN_UART_CTS_PORT, PIN_UART_CTS, cfg_cts);

    const uint32_t cfg_rts = (/* Pin is configured as FC0_RTS_SDLX_SSEL1 */
        IOCON_PIO_FUNC0 |
        /* No addition pin function */
        IOCON_PIO_MODE_INACT |
        /* Standard mode, output slew rate control is enabled */
        IOCON_PIO_SLEW_STANDARD |
        /* Input function is not inverted */
        IOCON_PIO_INV_DI |
        /* Enables digital function */
        IOCON_PIO_DIGITAL_EN |
        /* Open drain is disabled */
        IOCON_PIO_OPENDRAIN_DI);
    IOCON_PinMuxSet(IOCON, PIN_UART_RTS_PORT, PIN_UART_RTS, cfg_rts);
}

uint32_t USART3_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(3U);
}

void USART3_InitPins(void)
{
    /* Enables the clock for the I/O controller.: Enable Clock. */
    CLOCK_EnableClock(kCLOCK_Iocon);

    const uint32_t cfg_rx = (/* Pin is configured as FC3_RXD_SDA_MOSI_DATA */
        IOCON_PIO_FUNC1 |
        /* No addition pin function */
        IOCON_PIO_MODE_INACT |
        /* Standard mode, output slew rate control is enabled */
        IOCON_PIO_SLEW_STANDARD |
        /* Input function is not inverted */
        IOCON_PIO_INV_DI |
        /* Enables digital function */
        IOCON_PIO_DIGITAL_EN |
        /* Open drain is disabled */
        IOCON_PIO_OPENDRAIN_DI);
    IOCON_PinMuxSet(IOCON, PIN_PIO_PORT, PIN_TDO_SWO, cfg_rx);
}

void USART3_DeinitPins(void)
{
    /* Enables the clock for the I/O controller.: Enable Clock. */
    CLOCK_EnableClock(kCLOCK_Iocon);

    const uint32_t cfg_rx = (/* Pin is configured as FC3_RXD_SDA_MOSI_DATA */
        IOCON_PIO_FUNC0 |
        /* No addition pin function */
        IOCON_PIO_MODE_INACT |
        /* Standard mode, output slew rate control is enabled */
        IOCON_PIO_SLEW_STANDARD |
        /* Input function is not inverted */
        IOCON_PIO_INV_DI |
        /* Enables digital function */
        IOCON_PIO_DIGITAL_EN |
        /* Open drain is disabled */
        IOCON_PIO_OPENDRAIN_DI);
    IOCON_PinMuxSet(IOCON, PIN_PIO_PORT, PIN_TDO_SWO, cfg_rx);
}

void init_vbus_pin(void)
{
    const uint32_t port0_pin22_config = (/* Pin is configured as USB0_VBUS */
                                         IOCON_PIO_FUNC7 |
                                         /* No addition pin function */
                                         IOCON_PIO_MODE_INACT |
                                         /* Standard mode, output slew rate control is enabled */
                                         IOCON_PIO_SLEW_STANDARD |
                                         /* Input function is not inverted */
                                         IOCON_PIO_INV_DI |
                                         /* Enables digital function */
                                         IOCON_PIO_DIGITAL_EN |
                                         /* Open drain is disabled */
                                         IOCON_PIO_OPENDRAIN_DI);
    /* PORT0 PIN22 (coords: 78) is configured as USB0_VBUS */
    //IOCON_PinMuxSet(IOCON, 0U, 22U, port0_pin22_config);
}

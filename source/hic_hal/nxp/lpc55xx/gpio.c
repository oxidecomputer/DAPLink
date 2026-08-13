/**
 * @file    gpio.c
 * @brief   GPIO handling for LPC55xx
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * Copyright (c) 2016-2017 NXP
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

#include "gpio.h"
#include "DAP_config.h"
#include "daplink.h"
#include "fsl_clock.h"
#include "fsl_device_registers.h"
#include "fsl_iocon.h"
#include "fsl_lpadc.h"
#include "fsl_power.h"
#include "fsl_reset.h"
#include "hic_init.h"

#define VREF_TARGET_SPI_HALF_CHAN 0U
#define VREF_TARGET_SPI_HALF_CMD 1U
#define VREF_TARGET_UART_HALF_CHAN 2U
#define VREF_TARGET_UART_HALF_CMD 2U
// B side mux channel 0 is ADC channel 8, but the function is labeled as
// ADC0_8. The docs don't really explain this but you can find folks on the
// forums who have run into it. Effectively any B side mux channel is ADC
// channel - 8
#define VREF_TARGET_SWD_HALF_CHAN 0U
#define VREF_TARGET_SWD_HALF_CMD 3U

__WEAK void board_gpio_init(void) {
  // Nothing by default
}

void board_adc_init(void);

// Note: other gpio config happens in DAP_config
void gpio_init(void) {
  // Enable hardfault on unaligned access for the interface only.
  // If this is done in the bootloader than then it might (will) break
  // older application firmware or firmware from 3rd party vendors.
#if defined(DAPLINK_IF)
  SCB->CCR |= SCB_CCR_UNALIGN_TRP_Msk;
#endif

  // Ensure clocks are enabled.
  SYSCON->AHBCLKCTRLSET[0] =
      SYSCON_AHBCLKCTRL0_IOCON_MASK | SYSCON_AHBCLKCTRL0_GPIO0_MASK |
      SYSCON_AHBCLKCTRL0_GPIO1_MASK | SYSCON_AHBCLKCTRL0_ADC_MASK;
  SYSCON->AHBCLKCTRLSET[1] = SYSCON_AHBCLKCTRL1_FC0_MASK |
                             SYSCON_AHBCLKCTRL1_FC3_MASK |
                             SYSCON_AHBCLKCTRL1_FC7_MASK;

  // Reset peripherals.
  RESET_PeripheralReset(kIOCON_RST_SHIFT_RSTn);
  RESET_PeripheralReset(kGPIO0_RST_SHIFT_RSTn);
  RESET_PeripheralReset(kGPIO1_RST_SHIFT_RSTn);
  RESET_PeripheralReset(kFC0_RST_SHIFT_RSTn);
  RESET_PeripheralReset(kFC3_RST_SHIFT_RSTn);
  RESET_PeripheralReset(kFC7_RST_SHIFT_RSTn);
  RESET_PeripheralReset(kADC0_RST_SHIFT_RSTn);

  board_gpio_init();
  board_adc_init();
}

void gpio_set_board_power(bool powerEnabled) {
  // No target power control in this circuit.
}

// this is mostly cribbed from the polling example in the sdk
void board_adc_init() {
  lpadc_config_t adc_cfg;
  lpadc_conv_trigger_config_t adc_trig_cfg;
  lpadc_conv_command_config_t adc_cmd_cfg;

  CLOCK_SetClkDiv(kCLOCK_DivAdcAsyncClk, 4U, true);
  CLOCK_AttachClk(kMAIN_CLK_to_ADC_CLK);
  POWER_DisablePD(kPDRUNCFG_PD_LDOGPADC);

  LPADC_GetDefaultConfig(&adc_cfg);
  adc_cfg.enableAnalogPreliminary = true;
  adc_cfg.referenceVoltageSource = kLPADC_ReferenceVoltageAlt3;
  // for calibration not measurements!
  adc_cfg.conversionAverageMode = kLPADC_ConversionAverage8;
  LPADC_Init(ADC0, &adc_cfg);
  LPADC_DoAutoCalibration(ADC0);

  // setup a command for each input channel
  LPADC_GetDefaultConvCommandConfig(&adc_cmd_cfg);
  // these are tweaked based on testing
  adc_cmd_cfg.hardwareAverageMode = kLPADC_HardwareAverageCount8;
  adc_cmd_cfg.sampleTimeMode = kLPADC_SampleTimeADCK19;
  // per channel config
  adc_cmd_cfg.channelNumber = VREF_TARGET_SPI_HALF_CHAN;
  adc_cmd_cfg.chainedNextCommandNumber = VREF_TARGET_UART_HALF_CMD;
  LPADC_SetConvCommandConfig(ADC0, VREF_TARGET_SPI_HALF_CMD, &adc_cmd_cfg);

  adc_cmd_cfg.channelNumber = VREF_TARGET_UART_HALF_CHAN;
  adc_cmd_cfg.chainedNextCommandNumber = VREF_TARGET_SWD_HALF_CMD;
  LPADC_SetConvCommandConfig(ADC0, VREF_TARGET_UART_HALF_CMD, &adc_cmd_cfg);

  // this input is special, it goes to the b side mux
  adc_cmd_cfg.channelNumber = VREF_TARGET_SWD_HALF_CHAN;
  adc_cmd_cfg.sampleChannelMode = kLPADC_SampleChannelSingleEndSideB;
  adc_cmd_cfg.chainedNextCommandNumber = 0; // terminate chain
  LPADC_SetConvCommandConfig(ADC0, VREF_TARGET_SWD_HALF_CMD, &adc_cmd_cfg);

  // setup one trigger since commands chain
  LPADC_GetDefaultConvTriggerConfig(&adc_trig_cfg);
  adc_trig_cfg.enableHardwareTrigger = false;
  adc_trig_cfg.targetCommandId = VREF_TARGET_SPI_HALF_CMD;
  LPADC_SetConvTriggerConfig(ADC0, 0, &adc_trig_cfg); // using trigger 0
}

// get the hardware compatibility version of the board
uint8_t read_board_hcv() {
    uint8_t version = 0;
    version |= GPIO->B[PIN_HCV0_PORT][PIN_HCV0] << 0;
    version |= GPIO->B[PIN_HCV1_PORT][PIN_HCV1] << 1;
    version |= GPIO->B[PIN_HCV2_PORT][PIN_HCV2] << 2;
    return version;
}

// get probe id from the board. differentiates probes on the same board
// 0 = standalone
// 1 = Barback - RoT
// 2 = Barback - SP
uint8_t read_board_probe_id() {
    uint8_t probe_id = 0;
    probe_id = GPIO->B[PIN_PROBE_ID_PORT][PIN_PROBE_ID] + 1;
    return probe_id;
}

// the adc is in 12 bit mode so, per the datahseet, the format of a result is
//
// 0###_####_####_#000
//
// our board has 1/2 divider on these inputs. so we should
//
// 1. shift down 3 bits to get a 0-4095 ranged value
// 2. divide by 4095 to get a normalized value (0-1)
// 3. multiply by the reference voltage (~3.3V)
// 4. multiply by 2 (due to the input divider)
//
// We reorder these steps to do it with int math and avoid precision loss
uint16_t result_to_millivolts(uint16_t result) {
    const uint32_t REF_MV = 3300;
    const uint32_t value = result >> 3;
    return value * REF_MV / 2048;
}

// TODO we need a better output structure!
bool board_get_adc_values(uint16_t *values) {
    // must be same order as channel chain!
  uint32_t channels[] = {
    VREF_TARGET_SPI_HALF_CMD,
    VREF_TARGET_UART_HALF_CMD,
    VREF_TARGET_SWD_HALF_CMD,
  };
  uint32_t sample_num = 0;
  lpadc_conv_result_t result;
  ADC0->CTRL |= (1U << 8) | (1U << 9); // reset fifos, our sdk doesn't seem to
                                       // have this function
  LPADC_DoSoftwareTrigger(ADC0, 1U); // 1 is trigger 0's mask
  // TODO we need to check channel mapping and report timeout errors
  for (int sample_num = 0; sample_num < sizeof(channels)/sizeof(uint32_t); sample_num++) {
    // use a for loop here so we don't hang if conversion never happens
    // the delay number is a guess
    for (int i = 0; i < 12000; i++) {
      if (LPADC_GetConvResult(ADC0, &result, 0U))
        break;
    }
    // if there is a mismatch, bail
    if(result.commandIdSource != channels[sample_num]) return false;
    values[sample_num] = result_to_millivolts(result.convValue);
  }

  return true;
}

// TODO check the logic here, each field needs to be mapped to the correct LED
__WEAK void gpio_set_leds(uint32_t leds, gpio_led_state_t state) {
  // LED is active low, so set to inverse of the enum value.
  if (leds & (LED_T_CONNECTED | LED_T_RUNNING | LED_T_HID | LED_T_MSC)) {
    GPIO->B[PIN_SWD_STATUS_LED_PORT][PIN_SWD_STATUS_LED] =
        (uint8_t)((state == GPIO_LED_ON) ? 0 : 1);
  }

  if (leds & (LED_T_CDC)) {
    GPIO->B[PIN_UART_STATUS_LED_PORT][PIN_UART_STATUS_LED] =
        (uint8_t)((state == GPIO_LED_ON) ? 0 : 1);
  }
}

void gpio_set_hid_led(gpio_led_state_t state) {
  gpio_set_leds(LED_T_HID, state);
}

void gpio_set_cdc_led(gpio_led_state_t state) {
  gpio_set_leds(LED_T_CDC, state);
}

void gpio_set_msc_led(gpio_led_state_t state) {
  gpio_set_leds(LED_T_MSC, state);
}

__WEAK uint8_t gpio_get_reset_btn_no_fwrd(void) { return 0; }

__WEAK uint8_t gpio_get_reset_btn_fwrd(void) { return 0; }

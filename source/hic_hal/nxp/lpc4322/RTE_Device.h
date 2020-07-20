/* -------------------------------------------------------------------------- 
 * Copyright (c) 2013-2016 Arm Limited (or its affiliates). All 
 * rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * $Date:        25. April 2016
 * $Revision:    V2.2.1
 *
 * Project:      RTE Device Configuration for NXP LPC43xx
 * -------------------------------------------------------------------------- */

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

#ifndef __RTE_DEVICE_H
#define __RTE_DEVICE_H

// <e> UART1 (Universal asynchronous receiver transmitter) [Driver_USART1]
#define   RTE_UART1                     1

//   <h> Pin Configuration
//     <o> TX <0=>Not used <1=>P1_13 <2=>P3_4 <3=>P5_6 <4=>PC_13 <5=>PE_11
//     <i> UART0 Serial Output pin
#define   RTE_UART1_TX_ID               0
#if      (RTE_UART1_TX_ID == 0)
  #define RTE_UART1_TX_PIN_EN           0
#elif    (RTE_UART1_TX_ID == 1)
  #define RTE_UART1_TX_PORT             1
  #define RTE_UART1_TX_BIT              13
  #define RTE_UART1_TX_FUNC             1
#elif    (RTE_UART1_TX_ID == 2)
  #define RTE_UART1_TX_PORT             3
  #define RTE_UART1_TX_BIT              4
  #define RTE_UART1_TX_FUNC             4
#elif    (RTE_UART1_TX_ID == 3)
  #define RTE_UART1_TX_PORT             5
  #define RTE_UART1_TX_BIT              6
  #define RTE_UART1_TX_FUNC             4
#elif    (RTE_UART1_TX_ID == 4)
  #define RTE_UART1_TX_PORT             0xC
  #define RTE_UART1_TX_BIT              13
  #define RTE_UART1_TX_FUNC             2
#elif    (RTE_UART1_TX_ID == 5)
  #define RTE_UART1_TX_PORT             0xE
  #define RTE_UART1_TX_BIT              11
  #define RTE_UART1_TX_FUNC             2
#else
  #error "Invalid UART1_TX Pin Configuration!"
#endif
#ifndef   RTE_UART1_TX_PIN_EN
  #define RTE_UART1_TX_PIN_EN           1
#endif
//   <o> RX <0=>Not used <1=>P1_14 <2=>P3_5 <3=>P5_7 <4=>PC_14 <5=>PE_12
//   <i> UART1 Serial Input pin
#define   RTE_UART1_RX_ID               1
#if      (RTE_UART1_RX_ID == 0)
  #define RTE_UART1_RX_PIN_EN           0
#elif    (RTE_UART1_RX_ID == 1)
  #define RTE_UART1_RX_PORT             1
  #define RTE_UART1_RX_BIT              14
  #define RTE_UART1_RX_FUNC             1
#elif    (RTE_UART1_RX_ID == 2)
  #define RTE_UART1_RX_PORT             3
  #define RTE_UART1_RX_BIT              5
  #define RTE_UART1_RX_FUNC             4
#elif    (RTE_UART1_RX_ID == 3)
  #define RTE_UART1_RX_PORT             5
  #define RTE_UART1_RX_BIT              7
  #define RTE_UART1_RX_FUNC             4
#elif    (RTE_UART1_RX_ID == 4)
  #define RTE_UART1_RX_PORT             0xC
  #define RTE_UART1_RX_BIT              14
  #define RTE_UART1_RX_FUNC             2
#elif    (RTE_UART1_RX_ID == 5)
  #define RTE_UART1_RX_PORT             0xE
  #define RTE_UART1_RX_BIT              12
  #define RTE_UART1_RX_FUNC             2
#else
  #error "Invalid UART1_RX Pin Configuration!"
#endif
#ifndef   RTE_UART1_RX_PIN_EN
  #define RTE_UART1_RX_PIN_EN           1
#endif

//     <h> Modem Lines
//       <o> CTS <0=>Not used <1=>P1_11 <2=>P5_4 <3=>PC_2 <4=>PE_7
#define   RTE_UART1_CTS_ID              0
#if      (RTE_UART1_CTS_ID == 0)
  #define RTE_UART1_CTS_PIN_EN          0
#elif    (RTE_UART1_CTS_ID == 1)
  #define RTE_UART1_CTS_PORT            1
  #define RTE_UART1_CTS_BIT             11
  #define RTE_UART1_CTS_FUNC            1
#elif    (RTE_UART1_CTS_ID == 2)
  #define RTE_UART1_CTS_PORT            5
  #define RTE_UART1_CTS_BIT             4
  #define RTE_UART1_CTS_FUNC            4
#elif    (RTE_UART1_CTS_ID == 3)
  #define RTE_UART1_CTS_PORT            0xC
  #define RTE_UART1_CTS_BIT             2
  #define RTE_UART1_CTS_FUNC            2
#elif    (RTE_UART1_CTS_ID == 4)
  #define RTE_UART1_CTS_PORT            0xE
  #define RTE_UART1_CTS_BIT             7
  #define RTE_UART1_CTS_FUNC            2
#else
  #error "Invalid UART1_CTS Pin Configuration!"
#endif
#ifndef   RTE_UART1_CTS_PIN_EN
  #define RTE_UART1_CTS_PIN_EN          1
#endif
//       <o> RTS <0=>Not used <1=>P1_9  <2=>P5_2 <3=>PC_3 <4=>PE_5
#define   RTE_UART1_RTS_ID              0
#if      (RTE_UART1_RTS_ID == 0)
  #define RTE_UART1_RTS_PIN_EN          0
#elif    (RTE_UART1_RTS_ID == 1)
  #define RTE_UART1_RTS_PORT            1
  #define RTE_UART1_RTS_BIT             9
  #define RTE_UART1_RTS_FUNC            1
#elif    (RTE_UART1_RTS_ID == 2)
  #define RTE_UART1_RTS_PORT            5
  #define RTE_UART1_RTS_BIT             2
  #define RTE_UART1_RTS_FUNC            4
#elif    (RTE_UART1_RTS_ID == 3)
  #define RTE_UART1_RTS_PORT            0xC
  #define RTE_UART1_RTS_BIT             3
  #define RTE_UART1_RTS_FUNC            2
#elif    (RTE_UART1_RTS_ID == 4)
  #define RTE_UART1_RTS_PORT            0xE
  #define RTE_UART1_RTS_BIT             5
  #define RTE_UART1_RTS_FUNC            2
#else
  #error "Invalid UART1_RTS Pin Configuration!"
#endif
#ifndef   RTE_UART1_RTS_PIN_EN
  #define RTE_UART1_RTS_PIN_EN          1
#endif
//       <o> DCD <0=>Not used <1=>P1_12 <2=>P5_5 <3=>PC_11 <4=>PE_9
#define   RTE_UART1_DCD_ID              0
#if      (RTE_UART1_DCD_ID == 0)
  #define RTE_UART1_DCD_PIN_EN          0
#elif    (RTE_UART1_DCD_ID == 1)
  #define RTE_UART1_DCD_PORT            1
  #define RTE_UART1_DCD_BIT             12
  #define RTE_UART1_DCD_FUNC            1
#elif    (RTE_UART1_DCD_ID == 2)
  #define RTE_UART1_DCD_PORT            5
  #define RTE_UART1_DCD_BIT             5
  #define RTE_UART1_DCD_FUNC            4
#elif    (RTE_UART1_DCD_ID == 3)
  #define RTE_UART1_DCD_PORT            0xC
  #define RTE_UART1_DCD_BIT             11
  #define RTE_UART1_DCD_FUNC            2
#elif    (RTE_UART1_DCD_ID == 4)
  #define RTE_UART1_DCD_PORT            0xE
  #define RTE_UART1_DCD_BIT             9
  #define RTE_UART1_DCD_FUNC            2
#else
  #error "Invalid UART1_DCD Pin Configuration!"
#endif
#ifndef   RTE_UART1_DCD_PIN_EN
  #define RTE_UART1_DCD_PIN_EN          1
#endif
//       <o> DSR <0=>Not used <1=>P1_7 <2=>P5_0 <3=>PC_10 <4=>PE_8
#define   RTE_UART1_DSR_ID              0
#if      (RTE_UART1_DSR_ID == 0)
  #define RTE_UART1_DSR_PIN_EN          0
#elif    (RTE_UART1_DSR_ID == 1)
  #define RTE_UART1_DSR_PORT            1
  #define RTE_UART1_DSR_BIT             7
  #define RTE_UART1_DSR_FUNC            1
#elif    (RTE_UART1_DSR_ID == 2)
  #define RTE_UART1_DSR_PORT            5
  #define RTE_UART1_DSR_BIT             0
  #define RTE_UART1_DSR_FUNC            4
#elif    (RTE_UART1_DSR_ID == 3)
  #define RTE_UART1_DSR_PORT            0xC
  #define RTE_UART1_DSR_BIT             10
  #define RTE_UART1_DSR_FUNC            2
#elif    (RTE_UART1_DSR_ID == 4)
  #define RTE_UART1_DSR_PORT            0xE
  #define RTE_UART1_DSR_BIT             8
  #define RTE_UART1_DSR_FUNC            2
#else
  #error "Invalid UART1_DSR Pin Configuration!"
#endif
#ifndef   RTE_UART1_DSR_PIN_EN
  #define RTE_UART1_DSR_PIN_EN          1
#endif
//       <o> DTR <0=>Not used <1=>P1_8  <2=>P5_1 <3=>PC_12 <4=>PE_10
#define   RTE_UART1_DTR_ID              0
#if      (RTE_UART1_DTR_ID == 0)
  #define RTE_UART1_DTR_PIN_EN          0
#elif    (RTE_UART1_DTR_ID == 1)
  #define RTE_UART1_DTR_PORT            1
  #define RTE_UART1_DTR_BIT             8
  #define RTE_UART1_DTR_FUNC            1
#elif    (RTE_UART1_DTR_ID == 2)
  #define RTE_UART1_DTR_PORT            5
  #define RTE_UART1_DTR_BIT             1
  #define RTE_UART1_DTR_FUNC            4
#elif    (RTE_UART1_DTR_ID == 3)
  #define RTE_UART1_DTR_PORT            0xC
  #define RTE_UART1_DTR_BIT             12
  #define RTE_UART1_DTR_FUNC            2
#elif    (RTE_UART1_DTR_ID == 4)
  #define RTE_UART1_DTR_PORT            0xE
  #define RTE_UART1_DTR_BIT             10
  #define RTE_UART1_DTR_FUNC            2
#else
  #error "Invalid UART1_DTR Pin Configuration!"
#endif
#ifndef   RTE_UART1_DTR_PIN_EN
  #define RTE_UART1_DTR_PIN_EN          1
#endif
//       <o> RI <0=>Not used <1=>P1_10 <2=>P5_3 <3=>PC_1 <4=>PE_6
#define   RTE_UART1_RI_ID               0
#if      (RTE_UART1_RI_ID == 0)
  #define RTE_UART1_RI_PIN_EN           0
#elif    (RTE_UART1_RI_ID == 1)
  #define RTE_UART1_RI_PORT             1
  #define RTE_UART1_RI_BIT              10
  #define RTE_UART1_RI_FUNC             1
#elif    (RTE_UART1_RI_ID == 2)
  #define RTE_UART1_RI_PORT             5
  #define RTE_UART1_RI_BIT              3
  #define RTE_UART1_RI_FUNC             4
#elif    (RTE_UART1_RI_ID == 3)
  #define RTE_UART1_RI_PORT             0xC
  #define RTE_UART1_RI_BIT              1
  #define RTE_UART1_RI_FUNC             2
#elif    (RTE_UART1_RI_ID == 4)
  #define RTE_UART1_RI_PORT             0xE
  #define RTE_UART1_RI_BIT              6
  #define RTE_UART1_RI_FUNC             2
#else
  #error "Invalid UART1_RI Pin Configuration!"
#endif
#ifndef   RTE_UART1_RI_PIN_EN
  #define RTE_UART1_RI_PIN_EN           1
#endif
//     </h> Modem Lines
//   </h> Pin Configuration

//   <h> DMA
//     <e> Tx
//       <o1> Channel     <0=>0 <1=>1 <2=>2 <3=>3 <4=>4 <5=>5 <6=>6 <7=>7
//       <o2> Peripheral  <0=>3 (DMAMUXPER3)
//     </e>
#define   RTE_UART1_DMA_TX_EN           0
#define   RTE_UART1_DMA_TX_CH           0
#define   RTE_UART1_DMA_TX_PERI_ID      0
#if      (RTE_UART1_DMA_TX_PERI_ID == 0)
  #define RTE_UART1_DMA_TX_PERI         3
  #define RTE_UART1_DMA_TX_PERI_SEL     1
#endif
//     <e> Rx
//       <o1> Channel    <0=>0 <1=>1 <2=>2 <3=>3 <4=>4 <5=>5 <6=>6 <7=>7
//       <o2> Peripheral <0=>4 (DMAMUXPER4)
//     </e>
#define   RTE_UART1_DMA_RX_EN           0
#define   RTE_UART1_DMA_RX_CH           1
#define   RTE_UART1_DMA_RX_PERI_ID      0
#if      (RTE_UART1_DMA_RX_PERI_ID == 0)
  #define RTE_UART1_DMA_RX_PERI         4
  #define RTE_UART1_DMA_RX_PERI_SEL     1
#endif
//   </h> DMA
// </e> UART1 (Universal asynchronous receiver transmitter) [Driver_USART1]

#endif  /* __RTE_DEVICE_H */

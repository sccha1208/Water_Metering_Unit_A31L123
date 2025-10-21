/**
 *******************************************************************************
 * @file        A31L12x_hal_dmacn.h
 * @author      ABOV R&D Division
 * @brief       DMACn Header File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/

#ifndef _DMACn_H_
#define _DMACn_H_

#include "A31L12x.h"
#include "A31L12x_hal_aa_types.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

// Transfer Direction Control
#define DIR_MemToPeri         DMACn_CR_DIR_MemToPeri
#define DIR_PeriToMem         DMACn_CR_DIR_PeriToMem

// Transfer Size Selection Control
#define SIZE_8bit             DMACn_CR_SIZE_8bit
#define SIZE_16bit            DMACn_CR_SIZE_16bit
#define SIZE_32bit            DMACn_CR_SIZE_32bit

// Error Flag Stop Control
#define ERFGSTP_Disable       DMACn_CR_ERFGSTP_Disable
#define ERFGSTP_Enable        DMACn_CR_ERFGSTP_Enable

// Peripheral Selection Control
#define PERSEL_Idle           DMACn_CR_PERSEL_Idle
#define PERSEL_ADC            DMACn_CR_PERSEL_ADC
#define PERSEL_SPI0Rx         DMACn_CR_PERSEL_SPI0Rx
#define PERSEL_SPI0Tx         DMACn_CR_PERSEL_SPI0Tx
#define PERSEL_SPI1Rx         DMACn_CR_PERSEL_SPI1Rx
#define PERSEL_SPI1Tx         DMACn_CR_PERSEL_SPI1Tx
#define PERSEL_USART10Rx      DMACn_CR_PERSEL_USART10Rx
#define PERSEL_USART10Tx      DMACn_CR_PERSEL_USART10Tx
#define PERSEL_I2C0Rx         DMACn_CR_PERSEL_I2C0Rx
#define PERSEL_I2C0Tx         DMACn_CR_PERSEL_I2C0Tx
#define PERSEL_I2C1Rx         DMACn_CR_PERSEL_I2C1Rx
#define PERSEL_I2C1Tx         DMACn_CR_PERSEL_I2C1Tx
#define PERSEL_UART0Rx        DMACn_CR_PERSEL_UART0Rx
#define PERSEL_UART0Tx        DMACn_CR_PERSEL_UART0Tx
#define PERSEL_UART1Rx        DMACn_CR_PERSEL_UART1Rx
#define PERSEL_UART1Tx        DMACn_CR_PERSEL_UART1Tx
#define PERSEL_LPUARTRx       DMACn_CR_PERSEL_LPUARTRx
#define PERSEL_LPUARTTx       DMACn_CR_PERSEL_LPUARTTx
#define PERSEL_SC0Rx          DMACn_CR_PERSEL_SC0Rx
#define PERSEL_SC0Tx          DMACn_CR_PERSEL_SC0Tx
#define PERSEL_SC1Rx          DMACn_CR_PERSEL_SC1Rx
#define PERSEL_SC1Tx          DMACn_CR_PERSEL_SC1Tx

// Peripheral Address
#define PAR_ADC               0x300C
#define PAR_SPI0_Rx           0x5808
#define PAR_SPI0_Tx           0x580c
#define PAR_SPI1_Rx           0x5888
#define PAR_SPI1_Tx           0x588c
#define PAR_USART10_Rx        0x3814
#define PAR_USART10_Tx        0x3818
#define PAR_I2C0_Rx           0x4810
#define PAR_I2C0_Tx           0x4810
#define PAR_I2C1_Rx           0x4910
#define PAR_I2C1_Tx           0x4910
#define PAR_UART0_Rx          0x4000
#define PAR_UART0_Tx          0x4000
#define PAR_UART1_Rx          0x4100
#define PAR_UART1_Tx          0x4100
#define PAR_LPUART_Rx         0x5C18
#define PAR_LPUART_Tx         0x5C1C
#define PAR_SC0_Rx            0x5318
#define PAR_SC0_Tx            0x531C
#define PAR_SC1_Rx            0x5398
#define PAR_SC1_Tx            0x539c

//******************************************************************************
// Function
//******************************************************************************

HAL_Status_Type HAL_DMAC_Init( DMACn_Type* DMACx, uint8_t PERSEL, uint8_t DIR, uint8_t SIZE, uint8_t ERFGSTP );
HAL_Status_Type HAL_DMAC_DeInit( DMACn_Type* DMACx );

HAL_Status_Type HAL_DMAC_Setup( DMACn_Type* DMACx, uint32_t MAR, uint16_t TRANSCNT );

#ifdef __cplusplus
}
#endif

#endif   /* _DMACn_H_ */


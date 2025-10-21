/**
 *******************************************************************************
 * @file        A31L12x_hal_wdt.h
 * @author      ABOV R&D Division
 * @brief       Watchdog Timer Header File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#ifndef _WDT_H_
#define _WDT_H_

#include "A31L12x.h"
#include "A31L12x_hal_aa_types.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

// WDT_CR interrupt enable bit
/* Deprecated
#define WDT_CR_UNFIEN            ((uint32_t)(1 << 2))    // WDT Underflow Interrupt Enable bit
#define WDT_CR_WINMIEN           ((uint32_t)(1 << 3))    // WDT Window Match Interrupt Enable bit
#define WDT_INTERRUPT_BITMASK    0x000c
*/

/**   WDT_SR interrupt enable bit */
#define WDT_SR_UNFIFLAG          ((uint32_t)(1 << 0))    // WDT Underflow Interrupt Flag bit
#define WDT_SR_WINMIFLAG         ((uint32_t)(1 << 1))    // WDT Window Match Interrupt Flag bit

#define WDT_STATUS_BITMASK       0x0003

//******************************************************************************
// Type
//******************************************************************************

//==============================================================================
// Enumeration
//==============================================================================

enum
{
   WDT_DIV_4 = 0,
   WDT_DIV_16,
   WDT_DIV_64,
   WDT_DIV_256
};

typedef enum
{
   WDT_INTCFG_UNFIEN = 0,     /**<  UNFIEN Interrupt enable */
   WDT_INTCFG_WINMIEN,        /**<  WINMIEN Interrupt enable */
} WDT_INT_Type;

//==============================================================================
// Structure
//==============================================================================

typedef struct
{
   uint8_t     wdtResetEn;       /**<  if ENABLE -> the Reset bit is enabled */
   uint16_t    wdtClkDiv;        /**<  wdtClkDiv */
   uint32_t    wdtTmrConst;      /**<  Set Watch-Dog Timer Data Register */
   uint32_t    wdtWTmrConst;     /**<  Set Watch-Dog Timer Window Data Register */
} WDT_CFG_Type;

//******************************************************************************
// Function
//******************************************************************************

HAL_Status_Type HAL_WDT_Init( WDT_CFG_Type* WDT_Config );
HAL_Status_Type HAL_WDT_DeInit( void );

HAL_Status_Type HAL_WDT_ConfigInterrupt( WDT_INT_Type WDT_IntCfg, FunctionalState NewState );
HAL_Status_Type HAL_WDT_ReloadTimeCounter( void );
HAL_Status_Type HAL_WDT_Start( FunctionalState ctrl );
HAL_Status_Type HAL_WDT_ClearStatus( uint32_t clrbit );
uint32_t HAL_WDT_GetStatus( void );

uint32_t HAL_WDT_GetCurrentCount( void );

#ifdef __cplusplus
}
#endif

#endif   /* _WDT_H_ */


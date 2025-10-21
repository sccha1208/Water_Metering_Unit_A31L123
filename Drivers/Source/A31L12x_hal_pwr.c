/**
 *******************************************************************************
 * @file        A31L12x_hal_pwr.c
 * @author      ABOV R&D Division
 * @brief       Power sub system
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


/* Includes ----------------------------------------------------------------- */
//******************************************************************************
// Include
//******************************************************************************

#include "A31L12x_hal_pwr.h"

/* Public Functions --------------------------------------------------------- */
//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Enter Sleep mode with co-operated instruction by the Cortex-M0+.
 * @param         None
 * @return        None
 * @note          The 6 ��NOP Instruction�� should follow immediately after WFI or WFE Instruction��.
 *//*-------------------------------------------------------------------------*/
void HAL_PWR_EnterSleepMode( void )
{
   // Sleep Mode, clear SLEEPDEEP bit
   SCB->SCR = 0;

   // Sleep Mode
   WFI();
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();
}

/*-------------------------------------------------------------------------*//**
 * @brief         Enter Deep Sleep mode with co-operated instruction by the Cortex-M0+.
 * @param         None
 * @return        None
 * @note          The 6 ��NOP Instruction�� should follow immediately after WFI or WFE Instruction��.
 *//*-------------------------------------------------------------------------*/
void HAL_PWR_EnterPowerDownMode( void )
{
   // Deep-Sleep Mode, set SLEEPDEEP bit
   SCB->SCR = 0x4;

   // Deep Sleep Mode
   WFI();
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();
   NOP();
}


/**
 *******************************************************************************
 * @file        system_A31L12x.c
 * @author      ABOV R&D Division
 * @brief       CMSIS Cortex-M0+ Device Peripheral Access Layer for A31L12x
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/

#include "A31L12x.h"

//==============================================================================
// System Core Clock Variable
//==============================================================================
/* ToDo: initialize SystemCoreClock with the system core clock frequency value
         achieved after system intitialization.
         This means system core clock frequency after call to SystemInit() */
uint32_t    mclk;                /**<  Main Clock Frequency (MCLK) */
uint32_t    SystemCoreClock;     /**<  System Core Clock Frequency (Core Clock & HCLK) */
uint32_t    SystemPeriClock;     /**<  System Peripheral Clock Frequency (PCLK) */
uint32_t    ClkSrcTbl[] = { __HIRC, __XMOSC, __XSOSC, __WDTRC };

/*-------------------------------------------------------------------------*//**
 * @brief         Update SystemCoreClock variable.
 * @return        None
 * @details       Updates the SystemCoreClock with current core Clock retrieved from cpu registers.
 * @note          This function should be executed whenever the clock is changed.
 *//*-------------------------------------------------------------------------*/
void SystemCoreClockUpdate( void )      /* Get Core Clock Frequency */
{
   mclk = ClkSrcTbl[SCUCG->SCCR_b.MCLKSEL];
   if( SCUCG->SCCR_b.MCLKSEL == 0 ) mclk >>= SCUCG->CLKSRCR_b.HIRCSEL;  // MCLK

   if( SCUCG->SCDIVR1_b.HDIV > 4 ) SCUCG->SCDIVR1_b.HDIV = 4;
   SystemCoreClock = mclk >> (4 - SCUCG->SCDIVR1_b.HDIV);               // HCLK
   SystemPeriClock = SystemCoreClock >> SCUCG->SCDIVR2_b.PDIV;          // PCLK
}

/*-------------------------------------------------------------------------*//**
 * @brief         Setup the microcontroller system.
 * @return        None
 * @details       Initialize the System and update the SystemCoreClock variable.
 *//*-------------------------------------------------------------------------*/
void SystemInit( void )
{
   /* ToDo: add code to initialize the system
            do not use global variables because this function is called before
            reaching pre-main. RW section maybe overwritten afterwards. */

   while( SCUCC->SRSTVR_b.VALID != 0x55 ) {}

   // disable interrupt
   __disable_irq();

   // disable WDT
   WDT->CR  =  0
               | ( 0x5A69uL             << WDT_CR_WTIDKY_Pos )
               | ( WDT_CR_RSTEN_Disable << WDT_CR_RSTEN_Pos )     // added by lms
               | ( WDT_CR_CNTEN_Disable << WDT_CR_CNTEN_Pos )
               ;

#ifndef __ON_DEBUG__
   WDT->SR_b.DBGCNTEN = 0 << WDT_SR_DBGCNTEN_Pos;
#endif
}


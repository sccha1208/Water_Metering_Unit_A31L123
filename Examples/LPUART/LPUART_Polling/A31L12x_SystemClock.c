/**
 *******************************************************************************
 * @file        A31L12x_SystemClock.c
 * @author      ABOV R&D Division
 * @brief       System Clock Source File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/

/*******************************************************************************
 *                          A31L12x Device
 *------------------------------------------------------------------------
 *    System Clock source       | High Speend Internal oscillator
 *------------------------------------------------------------------------
 *    SYSCLK(MHz)               | 32MHz
 *------------------------------------------------------------------------
 *    HCLK(MHz) - Core Clock    | 32MHz
 *------------------------------------------------------------------------
 *    PCLK(MHz) - Peri Clock    | 32MHz
 *------------------------------------------------------------------------
 ******************************************************************************/

#include "main_conf.h"

/* Private typedef ---------------------------------------------------------- */
/* Private define ----------------------------------------------------------- */
/* Private macro ------------------------------------------------------------ */
/* Private variables -------------------------------------------------------- */


/* Private define ----------------------------------------------------------- */

// Main Clock Selection: Select only one of the following.
#define USED_HIRC       /*  HIRC  :  32000000uL */
//#define USED_WDTRC      /*  WDTRC :  40000uL */
//#define USED_XMOSC      /*  XMOSC :  16000000uL */
//#define USED_XSOSC      /*  XSOSC :  32768uL */

/* Clock Out Selection ( Monitoring ) */
/*#define USED_CLKO*/


/* Private function prototypes ---------------------------------------------- */

void SystemClock_Config( void );


/* Private variables -------------------------------------------------------- */


/*-------------------------------------------------------------------------*//**
 * @brief         Initialize default clock for A31L12x Board
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void SystemClock_Config( void )
{
   uint32_t    i;

   // enable clock source
   HAL_SCU_ClockSource_Enable( CLKSRCR_HIRCEN | CLKSRCR_XMOSCEN | CLKSRCR_XSOSCEN | CLKSRCR_WDTRCEN, HIRCSEL_HIRC1 );
   for( i = 0; i < 1000; i++ );  // Clock Stable Time

   // select system clock
#ifdef USED_WDTRC
   HAL_SCU_SystemClockDivider( WLDIV_MCLK64 | HDIV_MCLK1, SYSTDIV_HCLK1 | PDIV_HCLK1 );  // RTCC/LCD | HCLK, SysTick | PCLK
   HAL_SCU_SystemClockChange( MCLKSEL_WDTRC );

   SystemCoreClock = 40000uL;    // HCLK
   SystemPeriClock = 40000uL;    // PCLK
#endif

#ifdef USED_XSOSC
   HAL_SCU_SystemClockDivider( WLDIV_MCLK64 | HDIV_MCLK1, SYSTDIV_HCLK1 | PDIV_HCLK1 );  // RTCC/LCD | HCLK, SysTick | PCLK
   HAL_SCU_SystemClockChange( MCLKSEL_XSOSC );

   SystemCoreClock = 32768uL;    // HCLK
   SystemPeriClock = 32768uL;    // PCLK
#endif

#ifdef USED_XMOSC
   HAL_SCU_SystemClockChange( MCLKSEL_XMOSC );
   HAL_SCU_SystemClockDivider( WLDIV_MCLK64 | HDIV_MCLK1, SYSTDIV_HCLK1 | PDIV_HCLK1 );  // RTCC/LCD | HCLK, SysTick | PCLK

   SystemCoreClock = 16000000uL;    // HCLK
   SystemPeriClock = 16000000uL;    // PCLK
#endif

#ifdef USED_HIRC
   HAL_SCU_SystemClockChange( MCLKSEL_HIRC );
   HAL_SCU_SystemClockDivider( WLDIV_MCLK64 | HDIV_MCLK1, SYSTDIV_HCLK1 | PDIV_HCLK1 );  // RTCC/LCD | HCLK, SysTick | PCLK

   SystemCoreClock = 32000000uL;    // HCLK
   SystemPeriClock = 32000000uL;    // PCLK
#endif

   // disable unused clock source
   HAL_SCU_ClockSource_Disable( CLKSRCR_XMOSCEN | CLKSRCR_XSOSCEN );

   // enable clock monitoring
   HAL_SCU_ClockMonitoring( MACTS_SysClkChg, MONCS_MCLK );

   // enable clock output
#ifdef USED_CLKO
   HAL_SCU_CLKO_PinConfig( ( Pn_Type* )PC, 0 );
   HAL_SCU_ClockOutput( CLKOS_MCLK, POLSEL_Low, CLKODIV_SelectedClock1 );
#endif
}


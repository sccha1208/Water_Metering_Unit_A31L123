/**
 *******************************************************************************
 * @file        system_A31L12x.h
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

#ifndef SYSTEM_A31L12x_H
#define SYSTEM_A31L12x_H

#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================
// Define Debug mode or not
//==============================================================================
#define __ON_DEBUG__                 // This line should be defined on debug mode only.
 
//==============================================================================
// Define clocks
//==============================================================================
/* ToDo: add here your necessary defines for device initialization
         following is an example for different system frequencies */

#define __HIRC    (32000000uL)      /**<  Internal RC Oscillator Frequency */
#define __XMOSC   (16000000uL)      /**<  External Main Crystal Oscillator Frequency */
#define __XSOSC   (32768uL)         /**<  External Sub Crystal Oscillator Frequency */
#define __WDTRC   (40000uL)         /**<  Watch-Dog Timer RC Oscillator Frequency */

extern uint32_t mclk;               /**<  Main Clock Frequency (MCLK) */
extern uint32_t SystemCoreClock;    /**<  System Core Clock Frequency (Core Clock & HCLK) */
extern uint32_t SystemPeriClock;    /**<  System Peripheral Clock Frequency (PCLK) */

/*-------------------------------------------------------------------------*//**
 * @brief         Setup the microcontroller system.
 * @return        None
 * @details       Initialize the System and update the SystemCoreClock variable.
 *//*-------------------------------------------------------------------------*/
extern void SystemInit( void );

/*-------------------------------------------------------------------------*//**
 * @brief         Update SystemCoreClock variable.
 * @return        None
 * @details       Updates the SystemCoreClock with current core Clock retrieved from cpu registers.
 *//*-------------------------------------------------------------------------*/
extern void SystemCoreClockUpdate( void );

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_A31L12x_H */


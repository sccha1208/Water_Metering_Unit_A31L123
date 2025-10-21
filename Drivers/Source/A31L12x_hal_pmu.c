/**
 *******************************************************************************
 * @file        A31L12x_hal_pmu.c
 * @author      ABOV R&D Division
 * @brief       Power Management Unit
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

#include "A31L12x_hal_pmu.h"

/* Public Functions --------------------------------------------------------- */
//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Enable all power off function
 * @return        None
 * @details       This function powers off all system and peripheral except always-on region on deep sleep mode.
 *//*-------------------------------------------------------------------------*/
void HAL_PMU_Enable_AllPowerOff( void )
{
   PMU->PWRCR = PMU->PWRCR
                | ( PMU_PWRCR_ALLPWR_Off   << PMU_PWRCR_ALLPWR_Pos )
                | ( PMU_PWRCR_WTIDKY_Value << PMU_PWRCR_WTIDKY_Pos )
                ;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Enable flash power off
 * @return        None
 * @details       This function powers off the flash.
 *//*-------------------------------------------------------------------------*/
void HAL_PMU_Enable_FlashPowerOff( void )
{
   PMU->PWRCR = PMU->PWRCR
                | ( PMU_PWRCR_FLASHPWR_Off << PMU_PWRCR_FLASHPWR_Pos )
                | ( PMU_PWRCR_WTIDKY_Value << PMU_PWRCR_WTIDKY_Pos )
                ;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Disable flash power off
 * @return        None
 * @details       This function powers on the flash.
 *//*-------------------------------------------------------------------------*/
void HAL_PMU_Disable_FlashPowerOff( void )
{
   PMU->PWRCR = PMU->PWRCR
                & ~( PMU_PWRCR_FLASHPWR_Off << PMU_PWRCR_FLASHPWR_Pos )
                | ( PMU_PWRCR_WTIDKY_Value  << PMU_PWRCR_WTIDKY_Pos )
                ;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Enable the sram retention power function
 * @return        None
 * @details       This function selects the retention power for sram on sleep and deep sleep mode.
 *//*-------------------------------------------------------------------------*/
void HAL_PMU_Enable_SRAMRetentionPower( void )
{
   PMU->PWRCR = PMU->PWRCR
                | ( PMU_PWRCR_SRAMRTPWR_RetentionPower << PMU_PWRCR_SRAMRTPWR_Pos )
                | ( PMU_PWRCR_WTIDKY_Value             << PMU_PWRCR_WTIDKY_Pos )
                ;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Disable the sram retention power function
 * @return        None
 * @details       This function selects the normal power for sram on sleep and deep sleep mode.
 *//*-------------------------------------------------------------------------*/
void HAL_PMU_Disable_SRAMRetentionPower( void )
{
   PMU->PWRCR = PMU->PWRCR
                & ~( PMU_PWRCR_SRAMRTPWR_RetentionPower << PMU_PWRCR_SRAMRTPWR_Pos )
                | ( PMU_PWRCR_WTIDKY_Value              << PMU_PWRCR_WTIDKY_Pos )
                ;
}


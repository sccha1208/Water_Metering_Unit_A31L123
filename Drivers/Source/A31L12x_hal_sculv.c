/**
 *******************************************************************************
 * @file        A31L12x_hal_sculv.c
 * @author      ABOV R&D Division
 * @brief       LV sub system in System Control Unit
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

#include "A31L12x_hal_sculv.h"
#include "A31L12x_hal_scu.h"

/* Public Functions --------------------------------------------------------- */
//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Set LVI Block Enable/Disable and Voltage Level
 * @param[in]     u32LviEnBit
 *                   LVI Enable/Disable Control
 *                   -  LVIEN_Disable, LVIEN_Enable
 * @param[in]     u32LviIntEnBit
 *                   LVI Interrupt Enable/Disable Control
 *                   -  LVINTEN_Disable, LVINTEN_Enable
 * @param[in]     u32LviLevel
 *                   LVI Voltage Level
 *                   -  LVIVS_1p75V0   // 1.75V
 *                   -  LVIVS_1p75V1   // 1.75V
 *                   -  LVIVS_1p90V    // 1.90V
 *                   -  LVIVS_2p05V    // 2.05V
 *                   -  LVIVS_2p20V    // 2.20V
 *                   -  LVIVS_2p35V    // 2.35V
 *                   -  LVIVS_2p50V    // 2.50V
 *                   -  LVIVS_2p65V    // 2.65V
 * @return        None
 * @details       This function sets LVI voltage level.
 *//*-------------------------------------------------------------------------*/
void HAL_LVI_Init( uint32_t u32LviEnBit, uint32_t u32LviIntEnBit, uint32_t u32LviLevel )
{
   // enable peripheral clock
   HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_LVICLKE, PPxCLKE_Enable );

   SCULV->LVICR = ( u32LviEnBit | u32LviIntEnBit | u32LviLevel );
}


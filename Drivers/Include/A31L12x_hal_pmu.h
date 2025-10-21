/**
 *******************************************************************************
 * @file        A31L12x_hal_pmu.h
 * @author      ABOV R&D Division
 * @brief       PMU Header File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#ifndef _PMU_H_
#define _PMU_H_

#include "A31L12x.h"
#include "A31L12x_hal_aa_types.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Function
//******************************************************************************

void HAL_PMU_Enable_AllPowerOff( void );
void HAL_PMU_Enable_FlashPowerOff( void );
void HAL_PMU_Disable_FlashPowerOff( void );
void HAL_PMU_Enable_SRAMRetentionPower( void );
void HAL_PMU_Disable_SRAMRetentionPower( void );

#ifdef __cplusplus
}
#endif

#endif   /* _PMU_H_ */


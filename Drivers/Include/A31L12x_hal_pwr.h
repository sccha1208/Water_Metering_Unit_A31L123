/**
 *******************************************************************************
 * @file        A31L12x_hal_pwr.h
 * @author      ABOV R&D Division
 * @brief       Power Header File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#ifndef _PWR_H_
#define _PWR_H_

#include "A31L12x.h"
#include "A31L12x_hal_aa_types.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Function
//******************************************************************************

void HAL_PWR_EnterSleepMode( void );
void HAL_PWR_EnterPowerDownMode( void );

#ifdef __cplusplus
}
#endif

#endif   /*_PWR_H_ */


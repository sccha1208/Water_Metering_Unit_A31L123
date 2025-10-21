/**
 *******************************************************************************
 * @file        A31L12x_hal_wtidky.h
 * @author      ABOV R&D Division
 * @brief       WTIDKY Header File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#ifndef _WTIDKY_H_
#define _WTIDKY_H_

#include "A31L12x.h"
#include "A31L12x_hal_aa_types.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

// Write ID Key Definition
#define SCU_SCCRWTIDKY     0x570AuL       // 0     SCUCG_SCCR_WTIDKY_Value
#define SCU_CLKSRCRWTIDKY  0xA507uL       // 1     SCUCG_CLKSRCR_WTIDKY_Value
//#define Reserved          0xffffuL       // 2    Reserved
#define HIRCWTIDKY         0xA6B5uL       // 3     SCUCC_HIRCTRM_WTIDKY_Value
#define WDTRCWTIDKY        0x4C3DuL       // 4     SCUCC_WDTRCTRM_WTIDKY_Value
#define REMWTIDKY          0xE2F1uL       // 5     SCUCC_PMREMAP_WTIDKY_Value
#define WDTWTIDKY          0x5A69uL       // 6     WDT_CR_WTIDKY_Value
#define T30OUTWTIDKY       0xE06CuL       // 7
#define FMC_BCRWTIDKY      0xC1BEuL       // 8     FMC_BCR_WTIDKY_Value
#define FMC_CRWTIDKY       0x6C93uL       // 9     FMC_CR_WTIDKY_Value
#define SWRSTWTIDKY        0x9EB3uL       // 10    SCUCC_SWRSTR_WTIDKY_Value
#define VECTWTIDKY         0x05FAuL       // 11

#ifdef __cplusplus
}
#endif

#endif   /* _WTIDKY_H_ */


/**
 *******************************************************************************
 * @file        A31L12x_hal_sculv.h
 * @author      ABOV R&D Division
 * @brief       SCULV Header File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#ifndef _SCULV_H_
#define _SCULV_H_

#include "A31L12x.h"
#include "A31L12x_hal_aa_types.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

// LVI Enable/Disable Control
/*
#define LVI_DIS                  (0x00uL << 7)
#define LVI_EN                   (0x01uL << 7)
*/
#define LVIEN_Disable            (SCULV_LVICR_LVIEN_Disable << SCULV_LVICR_LVIEN_Pos)
#define LVIEN_Enable             (SCULV_LVICR_LVIEN_Enable  << SCULV_LVICR_LVIEN_Pos)

// LVI Interrupt Enable/Disable Control
/*
#define LVI_INT_DIS              (0x00uL << 5)
#define LVI_INT_EN               (0x01uL << 5)
*/
#define LVINTEN_Disable          (SCULV_LVICR_LVINTEN_Disable << SCULV_LVICR_LVINTEN_Pos)
#define LVINTEN_Enable           (SCULV_LVICR_LVINTEN_Enable  << SCULV_LVICR_LVINTEN_Pos)

// LVI Voltage Selection Control
/*
#define LV_1DOT88                (0x03uL)
#define LV_2DOT00                (0x04uL)
#define LV_2DOT13                (0x05uL)
#define LV_2DOT28                (0x06uL)
#define LV_2DOT46                (0x07uL)
#define LV_2DOT67                (0x08uL)
#define LV_3DOT04                (0x09uL)
#define LV_3DOT20                (0x0AuL)
#define LV_3DOT55                (0x0BuL)
#define LV_3DOT75                (0x0CuL)
#define LV_3DOT99                (0x0DuL)
#define LV_4DOT25                (0x0EuL)
#define LV_4DOT55                (0x0FuL)
*/
#define LVIVS_1p75V0             (SCULV_LVICR_LVIVS_1p75V0 << SCULV_LVICR_LVIVS_Pos)     // 1.75V                                                             */
#define LVIVS_1p75V1             (SCULV_LVICR_LVIVS_1p75V1 << SCULV_LVICR_LVIVS_Pos)     // 1.75V                                                             */
#define LVIVS_1p90V              (SCULV_LVICR_LVIVS_1p90V  << SCULV_LVICR_LVIVS_Pos)     // 1.90V                                                              */
#define LVIVS_2p05V              (SCULV_LVICR_LVIVS_2p05V  << SCULV_LVICR_LVIVS_Pos)     // 2.05V                                                              */
#define LVIVS_2p20V              (SCULV_LVICR_LVIVS_2p20V  << SCULV_LVICR_LVIVS_Pos)     // 2.20V                                                              */
#define LVIVS_2p35V              (SCULV_LVICR_LVIVS_2p35V  << SCULV_LVICR_LVIVS_Pos)     // 2.35V                                                              */
#define LVIVS_2p50V              (SCULV_LVICR_LVIVS_2p50V  << SCULV_LVICR_LVIVS_Pos)     // 2.50V                                                              */
#define LVIVS_2p65V              (SCULV_LVICR_LVIVS_2p65V  << SCULV_LVICR_LVIVS_Pos)     // 2.65V                                                              */

//******************************************************************************
// Macro
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Enable/Disable LVR
 * @details       This macro disables/enables LVR. If any value is written, the LVR register won't be changed until to POR.
 *//*-------------------------------------------------------------------------*/
#define SCULV_DisLVR()           (SCULV->LVRCR_b.LVREN = 0x55)
#define SCULV_EnLVR()            (SCULV->LVRCR_b.LVREN = 0x8A)

/*-------------------------------------------------------------------------*//**
 * @brief         Enable/Disable LVI Interrupt
 * @details       This macro sets the LVI interrupt enable bit.
 *//*-------------------------------------------------------------------------*/
#define SCULV_EnLviInt()         (SCULV->LVICR_b.LVINTEN = 1)
#define SCULV_DisLviInt()        (SCULV->LVICR_b.LVINTEN = 0)

/*-------------------------------------------------------------------------*//**
 * @brief         Get/Clear LVI Interrupt Flag
 * @details       This macro gets/clears the LVI interrupt flag.
 *//*-------------------------------------------------------------------------*/
#define SCULV_GetLviFlag()       (SCULV->LVICR_b.LVIFLAG)
#define SCULV_ClrLviFlag()       (SCULV->LVICR_b.LVIFLAG = 1)

//******************************************************************************
// Function
//******************************************************************************

void HAL_LVI_Init( uint32_t u32LviEnBit, uint32_t u32LviIntEnBit, uint32_t u32LviLevel );

#ifdef __cplusplus
}
#endif

#endif   /* _SCULV_H_ */


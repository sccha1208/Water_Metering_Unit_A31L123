/**
 *******************************************************************************
 * @file        A31L12x_hal_crc.h
 * @author      ABOV R&D Division
 * @brief       CRC Header File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#ifndef _CRC_H_
#define _CRC_H_

#include "A31L12x.h"
#include "A31L12x_hal_aa_types.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

// User/Auto Mode Selection Control
/*
#define CRC_USER_M               (0x0uL << 7)
#define CRC_AUTO_M               (0x1uL << 7)
*/
#define MODS_UserMode            (CRC_CR_MODS_UserMode << CRC_CR_MODS_Pos)
#define MODS_AutoMode            (CRC_CR_MODS_AutoMode << CRC_CR_MODS_Pos)

// CRC/Checksum Result Data Register (CRC_RLT) Initialization Control
/*
#define CRC_RLTCLR               (0x1uL << 6)
*/
#define RLTCLR_NoEffect          (CRC_CR_RLTCLR_NoEffect << CRC_CR_RLTCLR_Pos)
#define RLTCLR_Init              (CRC_CR_RLTCLR_Init     << CRC_CR_RLTCLR_Pos)

// CRC/Checksum Selection Control
/*
#define CRC_CRC                  (0x0uL << 5)
#define CRC_CHECKSUM             (0x1uL << 5)
*/
#define MDSEL_CRC                (CRC_CR_MDSEL_CRC      << CRC_CR_MDSEL_Pos)
#define MDSEL_Checksum           (CRC_CR_MDSEL_Checksum << CRC_CR_MDSEL_Pos)

// Polynomial Selection Control (CRC only)
/*
#define CRC_CCITT                (0x0uL << 4)
#define CRC_16                   (0x1uL << 4)
*/
#define POLYS_CRC16_CCITT        (CRC_CR_POLYS_CRC16_CCITT << CRC_CR_POLYS_Pos)
#define POLYS_CRC16              (CRC_CR_POLYS_CRC16       << CRC_CR_POLYS_Pos)
#define POLYS_CRC8               (CRC_CR_POLYS_CRC8        << CRC_CR_POLYS_Pos)
#define POLYS_CRC32              (CRC_CR_POLYS_CRC32       << CRC_CR_POLYS_Pos)

// CRC/Checksum Start Address Auto Increment Control (User mode only)
/*
#define CRC_NOINC                (0x0uL << 3)
#define SARINC_Inc               (0x1uL << 3)
*/
#define SARINC_Disable           (CRC_CR_SARINC_Disable << CRC_CR_SARINC_Pos)
#define SARINC_Enable            (CRC_CR_SARINC_Enable  << CRC_CR_SARINC_Pos)

// First Shifted-in Selection Control (CRC only)
/*
#define CRC_MSB                  (0x0uL << 1)
#define CRC_LSB                  (0x1uL << 1)
*/
#define FIRSTBS_msbFirst         (CRC_CR_FIRSTBS_msbFirst << CRC_CR_FIRSTBS_Pos)
#define FIRSTBS_lsbFirst         (CRC_CR_FIRSTBS_lsbFirst << CRC_CR_FIRSTBS_Pos)

// Input Data Size Selection Control
#define INSIZE_32Bit             (CRC_CR_INSIZE_32Bit << CRC_CR_INSIZE_Pos)
#define INSIZE_16Bit             (CRC_CR_INSIZE_16Bit << CRC_CR_INSIZE_Pos)
#define INSIZE_8Bit              (CRC_CR_INSIZE_8Bit  << CRC_CR_INSIZE_Pos)

// Input Data Complement Control
#define INCOMP_Disable           (CRC_CR_INCOMP_Disable << CRC_CR_INCOMP_Pos)
#define INCOMP_Enable            (CRC_CR_INCOMP_Enable  << CRC_CR_INCOMP_Pos)

//******************************************************************************
// Macro
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         CRC In Data
 * @param[in]     u32InData
 *                CRC/Checksum Input Data
 * @details       This macro puts input data for calculation
 *//*-------------------------------------------------------------------------*/
#define CRC_InData( u32InData )  (CRC->IN = u32InData)

/*-------------------------------------------------------------------------*//**
 * @brief         CRC/Checksum Run
 * @details       This macro starts CRC/Checksum calculation
 *//*-------------------------------------------------------------------------*/
#define CRCRun()                 { CRC->CR_b.CRCRUN = 1; NOP(); NOP(); NOP(); NOP(); NOP(); }

/*-------------------------------------------------------------------------*//**
 * @brief         CRC/Checksum Stop
 * @details       This macro stops forcingly CRC/Checksum calculation
 *//*-------------------------------------------------------------------------*/
#define CRCStop()                (CRC->CR_b.CRCRUN = 0)

/*-------------------------------------------------------------------------*//**
 * @brief         CRC/Checksum Finish Check
 * @details       This macro checks CRC/Checksum finish
 *//*-------------------------------------------------------------------------*/
#define ChkCRCFinish()           (CRC->CR_b.CRCRUN)

//******************************************************************************
// Function
//******************************************************************************

HAL_Status_Type HAL_CRC_Init( void );
HAL_Status_Type HAL_CRC_DeInit( void );

HAL_Status_Type HAL_CRC_SetAddress( uint32_t u32SAdr, uint32_t u32EAdr, uint32_t u32IniD );

uint32_t HAL_CRC_ConfigAutoMode( uint32_t MDSEL, uint32_t u32POLY, uint32_t u32FirstBit, uint32_t InputDataSize, uint32_t InputDataComplement );
HAL_Status_Type HAL_CRC_ConfigUserMode( uint32_t MDSEL, uint32_t u32POLY, uint32_t u32AdrInc, uint32_t u32FirstBit, uint32_t InputDataSize, uint32_t InputDataComplement );
uint32_t HAL_CRC_UserInput( uint32_t u32Input );

#ifdef __cplusplus
}
#endif

#endif   /* _CRC_H_ */


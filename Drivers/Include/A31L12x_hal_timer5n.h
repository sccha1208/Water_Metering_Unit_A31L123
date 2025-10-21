/**
 *******************************************************************************
 * @file        A31L12x_hal_timer5n.h
 * @author      ABOV R&D Division
 * @brief       Timer5n Header File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#ifndef _TIMER5n_H_
#define _TIMER5n_H_

#include "A31L12x.h"
#include "A31L12x_hal_aa_types.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

/**   TIMER5n Match Interrupt Flag */
#define TIMER5n_CR_MATCH_FLAG          (1 << TIMER5n_CR_T5nMIFLAG_Pos)
#define TIMER5n_CR_CAPTURE_FLAG        (1 << TIMER5n_CR_T5nCIFLAG_Pos)

//******************************************************************************
// Type
//******************************************************************************

//==============================================================================
// Enumeration
//==============================================================================

typedef enum
{
   TIMER5n_CR_MATCH_INTR = 0,    /**<  OVIE Interrupt enable*/
   TIMER5n_CR_CAPTURE_INTR,      /**<  MBIE Interrupt enable*/
} TIMER5n_INT_Type;

//==============================================================================
// Structure
//==============================================================================

typedef struct
{
   // TIMER5n.CR
   TIMER5n_CR_T5nMS_Enum         T5nMS;      // TIMER5n Operation Mode Selection
   TIMER5n_CR_T5nCLK_Enum        T5nCLK;     // TIMER5n Clock Selection
   TIMER5n_CR_T5nECE_Enum        T5nECE;     // TIMER5n External Clock Edge Selection
   TIMER5n_CR_T5nINSEL_Enum      T5nINSEL;   // TIMER5n Capture Signal Selection
   TIMER5n_CR_T5nINPOL_Enum      T5nINPOL;   // TIMER5n Input Caputure/'Counter Clear Input' Polarity Selection
   TIMER5n_CR_T5nOPOL_Enum       T5nOPOL;    // T5nOUT Polarity Selection

   // TIMER5n.ADR
   uint16_t    ADR;

   // TIMER5n.BDR
   uint16_t    BDR;

   // TIMER5n.PREDR
   uint16_t    Prescaler;
} TIMER5n_CFG_Type;

//******************************************************************************
// Macro
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         TIMER5n Counter and Prescaler clear
 * @param[in]     TIMER5x
 *                   -  TIMER50
 * @details       This macro clears Counter and Prescalrer of TIMER5n
 *//*-------------------------------------------------------------------------*/
#define TIMER5n_ClrCnt( TIMER5x )       (TIMER5x->CR_b.T4nCLR = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         Set TIMER5n A Data Register
 * @param[in]     TIMER5x
 *                   -  TIMER50
 * @param[in]     u32AData
 *                   A Data of TIMER5n
 * @details       This macro sets TIMER5n A Data Register
 *//*-------------------------------------------------------------------------*/
#define TIMER5n_SetAData( TIMER5x, u32AData )    (TIMER5x->ADR = u32AData)

/*-------------------------------------------------------------------------*//**
 * @brief         Set TIMER5n B Data Register
 * @param[in]     TIMER5x
 *                   -  TIMER50
 * @param[in]     u32BData
 *                   B Data of TIMER5n
 * @details       This macro sets TIMER5n B Data Register
 *//*-------------------------------------------------------------------------*/
#define TIMER5n_SetBData( TIMER5x, u32BData )    (TIMER5x->BDR = u32BData)

/**   TIMER5n Match Interrupt Enable bit */
#define TIMER5n_CR_MIEN_SET( n )       (n << TIMER5n_CR_T5nMIEN_Pos)

/**   TIMER5n Capture Interrupt Enable bit */
#define TIMER5n_CR_CIEN_SET( n )       (n << TIMER5n_CR_T5nCIEN_Pos)

/**   TIMER5n Counter Temporary Pause Control bit */
#define TIMER5n_CR_PAU_SET( n )        (n << TIMER5n_CR_T5nPAU_Pos)

#define TIMER5n_PREDR_SET( n )         (n << 0)

//******************************************************************************
// Function
//******************************************************************************

HAL_Status_Type HAL_TIMER5n_Init( TIMER5n_Type* TIMER5x, TIMER5n_CFG_Type* TIMER5n_Config );
HAL_Status_Type HAL_TIMER5n_DeInit( TIMER5n_Type* TIMER5x );

HAL_Status_Type HAL_TIMER5n_ConfigInterrupt( TIMER5n_Type* TIMER5x, TIMER5n_INT_Type TIMER5n_IntCfg, FunctionalState NewState );
HAL_Status_Type HAL_TIMER5n_Cmd( TIMER5n_Type* TIMER5x, FunctionalState NewState );
HAL_Status_Type HAL_TIMER5n_ClearCounter( TIMER5n_Type* TIMER5x );
HAL_Status_Type HAL_TIMER5n_UpdateCountValue( TIMER5n_Type* TIMER5x, uint8_t CountCh, uint16_t Value );
HAL_Status_Type HAL_TIMER5n_ClearStatus( TIMER5n_Type* TIMER5x, uint32_t value );
uint8_t HAL_TIMER5n_GetStatus( TIMER5n_Type* TIMER5x );

uint16_t HAL_TIMER5n_GetCaptureData( TIMER5n_Type* TIMER5x );

#ifdef __cplusplus
}
#endif

#endif   /* _TIMER5n_H_ */


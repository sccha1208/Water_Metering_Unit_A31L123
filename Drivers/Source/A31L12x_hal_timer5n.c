/**
 *******************************************************************************
 * @file        A31L12x_hal_timer5n.c
 * @author      ABOV R&D Division
 * @brief       5N typed Timer
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

#include "A31L12x_hal_timer5n.h"

/* Public Functions --------------------------------------------------------- */
//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Initialize the TIMER5n peripheral with the specified parameters.
 *                   -  Set Clock frequency for Timer
 *                   -  Set initial configuration for Timer
 * @param[in]     TIMER5x
 *                   Pointer to the target TIMER5
 *                   -  TIMER50
 * @param[in]     TIMER5n_Config
 *                   Pointer to the configuration information for the specified peripheral.
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER5n_Init( TIMER5n_Type* TIMER5x, TIMER5n_CFG_Type* TIMER5n_Config )
{
   /* Check TIMER5 handle */
   if( TIMER5x == NULL )
   {
      return HAL_ERROR;
   }

#if 1    // supported
   if( TIMER5x == ( TIMER5n_Type* )TIMER50 )
   {
      // enable peripheral clock
      SCUCG->PPCLKEN1_b.T50CLKE = 1;
   }
#endif

#if 0    // not supported
   if( TIMER5x == ( TIMER5n_Type* )TIMER51 )
   {
      // enable peripheral clock
      SCUCG->PPCLKEN1_b.T51CLKE = 1;
   }
#endif

   // CR
   TIMER5x->CR =  0
                  | ( TIMER5n_Config->T5nMS    << TIMER5n_CR_T5nMS_Pos )
                  | ( TIMER5n_Config->T5nCLK   << TIMER5n_CR_T5nCLK_Pos )
                  | ( TIMER5n_Config->T5nECE   << TIMER5n_CR_T5nECE_Pos )
                  | ( TIMER5n_Config->T5nINSEL << TIMER5n_CR_T5nINSEL_Pos )
                  | ( TIMER5n_Config->T5nINPOL << TIMER5n_CR_T5nINPOL_Pos )
                  | ( TIMER5n_Config->T5nOPOL  << TIMER5n_CR_T5nOPOL_Pos )
                  ;

   // ADR & BDR
   TIMER5x->ADR = TIMER5n_Config->ADR;
   TIMER5x->BDR = TIMER5n_Config->BDR;

   // PREDR
   TIMER5x->PREDR_b.PRED = TIMER5n_Config->Prescaler;

   // clear counter and prescaler
   HAL_TIMER5n_ClearCounter( TIMER5x );

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Close Timer/Counter device
 * @param[in]     TIMER5x
 *                   Pointer to the target TIMER5
 *                   -  TIMER50
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER5n_DeInit( TIMER5n_Type* TIMER5x )
{
   /* Check TIMER5 handle */
   if( TIMER5x == NULL )
   {
      return HAL_ERROR;
   }

   // Disable timer/counter
   TIMER5x->CR_b.T5nEN = 0;

#if 1    // supported
   if( TIMER5x == ( TIMER5n_Type* )TIMER50 )
   {
      // disable peripheral clock
      SCUCG->PPCLKEN1_b.T50CLKE = 0;
   }
#endif

#if 0    // not supported
   if( TIMER5x == ( TIMER5n_Type* )TIMER51 )
   {
      // disable peripheral clock
      SCUCG->PPCLKEN1_b.T51CLKE = 0;
   }
#endif

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure the peripheral interrupt.
 * @param[in]     TIMER5x
 *                   Pointer to the target TIMER5
 *                   -  TIMER50
 * @param[in]     TIMER5n_IntCfg
 *                   Specifies the interrupt flag
 *                   -  TIMER5n_CR_MATCH_INTR:     Match interrupt enable
 *                   -  TIMER5n_CR_CAPTURE_INTR:   Capture interrupt enable
 * @param[in]     NewState
 *                   Next State of Interrupt Operation
 *                   -  ENABLE, DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER5n_ConfigInterrupt( TIMER5n_Type* TIMER5x, TIMER5n_INT_Type TIMER5n_IntCfg, FunctionalState NewState )
{
   /* Check TIMER5 handle */
   if( TIMER5x == NULL )
   {
      return HAL_ERROR;
   }

   switch( TIMER5n_IntCfg )
   {
      case TIMER5n_CR_MATCH_INTR:
         if( NewState == ENABLE )
         {
            TIMER5x->CR_b.T5nMIEN = 1;
         }
         else if( NewState == DISABLE )
         {
            TIMER5x->CR_b.T5nMIEN = 0;
         }
         break;
      case TIMER5n_CR_CAPTURE_INTR:
         if( NewState == ENABLE )
         {
            TIMER5x->CR_b.T5nCIEN = 1;
         }
         else if( NewState == DISABLE )
         {
            TIMER5x->CR_b.T5nCIEN = 0;
         }
         break;
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Start/Stop Timer/Counter device
 * @param[in]     TIMER5x
 *                   Pointer to the target TIMER5
 *                   -  TIMER50
 * @param[in]     NewState
 *                   Next State of Functional Operation
 *                   -  ENABLE, DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER5n_Cmd( TIMER5n_Type* TIMER5x, FunctionalState NewState )
{
   /* Check TIMER5 handle */
   if( TIMER5x == NULL )
   {
      return HAL_ERROR;
   }

   if( NewState == ENABLE )
   {
      TIMER5x->CR_b.T5nEN = TIMER5n_CR_T5nEN_Enable;
   }
   else
   {
      TIMER5x->CR_b.T5nEN = TIMER5n_CR_T5nEN_Disable;
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Clear Counter and Prescaler
 * @param[in]     TIMER5x
 *                   Pointer to the target TIMER5
 *                   -  TIMER50
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER5n_ClearCounter( TIMER5n_Type* TIMER5x )
{
   /* Check TIMER5 handle */
   if( TIMER5x == NULL )
   {
      return HAL_ERROR;
   }

   TIMER5x->CR_b.T5nCLR = TIMER5n_CR_T5nCLR_Clear;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Update value
 * @param[in]     TIMER5x
 *                   Pointer to the target TIMER5
 *                   -  TIMER50
 * @param[in]     CountCh
 *                   -  0 = GRA
 *                   -  1 = GRB
 * @param[in]     Value
 *                   updated match value
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER5n_UpdateCountValue( TIMER5n_Type* TIMER5x, uint8_t CountCh, uint16_t Value )
{
   /* Check TIMER5 handle */
   if( TIMER5x == NULL )
   {
      return HAL_ERROR;
   }

   switch( CountCh )
   {
      case 0:
         TIMER5x->ADR = Value;
         break;
      case 1:
         TIMER5x->BDR = Value;
         break;
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Clear Timer Status
 * @param[in]     TIMER5x
 *                   Pointer to the target TIMER5
 *                   -  TIMER50
 * @param[in]     mask
 *                   clear value
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER5n_ClearStatus( TIMER5n_Type* TIMER5x, uint32_t mask )
{
   /* Check TIMER5 handle */
   if( TIMER5x == NULL )
   {
      return HAL_ERROR;
   }

   TIMER5x->CR =  TIMER5x->CR
                  &  ~TIMER5n_CR_T5nMIFLAG_Msk
                  &  ~TIMER5n_CR_T5nCIFLAG_Msk
                  &  ~TIMER5n_CR_T5nCLIFLAG_Msk
                  |  mask
                  ;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Get Timer Status
 * @param[in]     TIMER5x
 *                   Pointer to the target TIMER5
 *                   -  TIMER50
 * @return        Value of status register
 *//*-------------------------------------------------------------------------*/
uint8_t HAL_TIMER5n_GetStatus( TIMER5n_Type* TIMER5x )
{
   return TIMER5x->CR;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Read value of capture register in timer/counter device
 * @param[in]     TIMER5x
 *                   Pointer to the target TIMER5
 *                   -  TIMER50
 * @return        Value of count register
 *//*-------------------------------------------------------------------------*/
uint16_t HAL_TIMER5n_GetCaptureData( TIMER5n_Type* TIMER5x )
{
   return TIMER5x->CAPDR;
}


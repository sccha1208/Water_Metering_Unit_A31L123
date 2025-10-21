/**
 *******************************************************************************
 * @file        adc.c
 * @author      ABOV R&D Division
 * @brief       4N typed Timer
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

#include "A31L12x_hal_scu.h"
#include "A31L12x_hal_timer4n.h"

/* Public Functions --------------------------------------------------------- */
//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Initialize the TIMER4n peripheral with the specified parameters.
 *                   -  Set Clock frequency for Timer
 *                   -  Set initial configuration for Timer
 * @param[in]     TIMER4x
 *                   Pointer to the target TIMER4
 *                   -  TIMER40 ~ TIMER43
 * @param[in]     TimerCounterMode
 *                   Timer Counter Mode
 *                   -  TIMER4n_PERIODIC_MODE
 *                   -  TIMER4n_CAPTURE_MODE
 *                   -  TIMER4n_BACKTOBACK_MODE
 *                   -  TIMER4n_ONESHOT_MODE
 * @param[in]     TIMER4n_Config
 *                   Pointer to the configuration information for the specified peripheral.
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER4n_Init( TIMER4n_Type* TIMER4x, TIMER4n_MODE_OPT TimerCounterMode, void* TIMER4n_Config )
{
   TIMER4n_PERIODICCFG_Type*     pTimeCfg;
   TIMER4n_CAPTURECFG_Type*      pCaptureCfg;
   uint32_t                      reg_val32;

   /* Check TIMER4 handle */
   if( TIMER4x == NULL )
   {
      return HAL_ERROR;
   }

#if 1    // supported
   if( TIMER4x == ( TIMER4n_Type* )TIMER40 )
   {
      HAL_SCU_Peripheral_EnableClock1( PPCLKEN1_T40CLKE, PPxCLKE_Enable );
   }
#endif

#if 1    // supported
   if( TIMER4x == ( TIMER4n_Type* )TIMER41 )
   {
      HAL_SCU_Peripheral_EnableClock1( PPCLKEN1_T41CLKE, PPxCLKE_Enable );
   }
#endif

#if 1    // supported
   if( TIMER4x == ( TIMER4n_Type* )TIMER42 )
   {
      HAL_SCU_Peripheral_EnableClock1( PPCLKEN1_T42CLKE, PPxCLKE_Enable );
   }
#endif

#if 1    // supported
   if( TIMER4x == ( TIMER4n_Type* )TIMER43 )
   {
      HAL_SCU_Peripheral_EnableClock1( PPCLKEN1_T43CLKE, PPxCLKE_Enable );
   }
#endif

   if( ( TimerCounterMode == TIMER4n_PERIODIC_MODE ) || ( TimerCounterMode == TIMER4n_ONESHOT_MODE ) || ( TimerCounterMode == TIMER4n_BACKTOBACK_MODE ) )
   {
      pTimeCfg = ( TIMER4n_PERIODICCFG_Type* )TIMER4n_Config;

      reg_val32 = 0
                  | TIMER4n_CR_CKSEL_SET( pTimeCfg->CkSel )
                  | TIMER4n_CR_MODE_SET( TimerCounterMode )
                  ;
      if( pTimeCfg->CkSel == 1 )
      {
         reg_val32 = reg_val32 | TIMER4n_CR_ECE_SET( pTimeCfg->ECE );
      }
      TIMER4x->CR = reg_val32;

      reg_val32 = 0
                  | TIMER4n_OUTCR_POLB_SET( pTimeCfg->OutBStartLevel )
                  | TIMER4n_OUTCR_POLA_SET( pTimeCfg->OutAStartLevel )
                  | TIMER4n_OUTCR_BOE_SET( pTimeCfg->OutBEnable )
                  | TIMER4n_OUTCR_AOE_SET( pTimeCfg->OutAEnable )
                  ;
      TIMER4x->OUTCR = reg_val32;

      TIMER4x->PREDR = ( pTimeCfg->Prescaler & TIMER4n_PRS_MASK );
      TIMER4x->ADR = pTimeCfg->ADR;
      TIMER4x->BDR = pTimeCfg->BDR;
      TIMER4x->PDR = pTimeCfg->PDR;
   }
   else if( TimerCounterMode == TIMER4n_CAPTURE_MODE )
   {
      pCaptureCfg = ( TIMER4n_CAPTURECFG_Type* )TIMER4n_Config;

      reg_val32 = 0
                  | TIMER4n_CR_CKSEL_SET( pCaptureCfg->CkSel )
                  | TIMER4n_CR_MODE_SET( TimerCounterMode )
                  | TIMER4n_CR_INPOL_SET( pCaptureCfg->ClrMode )
                  ;
      if( pCaptureCfg->CkSel == 1 )
      {
         reg_val32 = reg_val32 | TIMER4n_CR_ECE_SET( pCaptureCfg->ECE );
      }
      TIMER4x->CR = reg_val32;

      TIMER4x->PREDR = ( pCaptureCfg->Prescaler & TIMER4n_PRS_MASK );
      TIMER4x->PDR = pCaptureCfg->PDR;
   }
   TIMER4n_ClrCnt( TIMER4x ); //  timer counter clear

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Close Timer/Counter device
 * @param[in]     TIMER4x
 *                   Pointer to the target TIMER4
 *                   -  TIMER40 ~ TIMER43
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER4n_DeInit( TIMER4n_Type* TIMER4x )
{
   /* Check TIMER4 handle */
   if( TIMER4x == NULL )
   {
      return HAL_ERROR;
   }

   // Disable timer/counter
   TIMER4x->CR = 0x00;

#if 1    // supported
   if( TIMER4x == ( TIMER4n_Type* )TIMER40 )
   {
      HAL_SCU_Peripheral_SetReset1( 1 << SCUCG_PPRST1_T40RST_Pos );
      HAL_SCU_Peripheral_EnableClock1( PPCLKEN1_T40CLKE, PPxCLKE_Disable );
   }
#endif

#if 1    // supported
   if( TIMER4x == ( TIMER4n_Type* )TIMER41 )
   {
      HAL_SCU_Peripheral_SetReset1( 1 << SCUCG_PPRST1_T41RST_Pos );
      HAL_SCU_Peripheral_EnableClock1( PPCLKEN1_T41CLKE, PPxCLKE_Disable );
   }
#endif

#if 1    // supported
   if( TIMER4x == ( TIMER4n_Type* )TIMER42 )
   {
      HAL_SCU_Peripheral_SetReset1( 1 << SCUCG_PPRST1_T42RST_Pos );
      HAL_SCU_Peripheral_EnableClock1( PPCLKEN1_T42CLKE, PPxCLKE_Disable );
   }
#endif

#if 1    // supported
   if( TIMER4x == ( TIMER4n_Type* )TIMER43 )
   {
      HAL_SCU_Peripheral_SetReset1( 1 << SCUCG_PPRST1_T43RST_Pos );
      HAL_SCU_Peripheral_EnableClock1( PPCLKEN1_T43CLKE, PPxCLKE_Disable );
   }
#endif

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure the peripheral interrupt.
 * @param[in]     TIMER4x
 *                   Pointer to the target TIMER4
 *                   -  TIMER40 ~ TIMER43
 * @param[in]     TIMER4n_IntCfg
 *                   Specifies the interrupt flag
 *                   -  TIMER4n_INTCFG_PMIE:  Period Match Interrupt enable
 *                   -  TIMER4n_INTCFG_CIE:   Capture  Interrupt enable
 * @param[in]     NewState
 *                   Next State of Interrupt Operation
 *                   -  ENABLE, DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER4n_ConfigInterrupt( TIMER4n_Type* TIMER4x, TIMER4n_INT_Type TIMER4n_IntCfg, FunctionalState NewState )
{
   uint16_t     tmp;

   /* Check TIMER4 handle */
   if( TIMER4x == NULL )
   {
      return HAL_ERROR;
   }

   switch( TIMER4n_IntCfg )
   {
      case TIMER4n_INTCFG_PMIE:
         tmp = TIMER4n_INT_PMIEN;
         break;
      case TIMER4n_INTCFG_CIE:
         tmp = TIMER4n_INT_CIEN;
         break;
   }

   if( NewState == ENABLE )
   {
      TIMER4x->INTCR |= tmp;
   }
   else
   {
      TIMER4x->INTCR &= ( ~tmp );
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Start/Stop Timer/Counter device
 * @param[in]     TIMER4x
 *                   Pointer to the target TIMER4
 *                   -  TIMER40 ~ TIMER43
 * @param[in]     NewState
 *                   Next State of Functional Operation
 *                   -  ENABLE, DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER4n_Cmd( TIMER4n_Type* TIMER4x, FunctionalState NewState )
{
   /* Check TIMER4 handle */
   if( TIMER4x == NULL )
   {
      return HAL_ERROR;
   }

   if( NewState == ENABLE )
   {
      TIMER4x->CR |=  TIMER4n_ENABLE;
   }
   else
   {
      TIMER4x->CR &= ~TIMER4n_ENABLE;
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set TIMER4n CR/PREDR Registers
 * @param[in]     TIMER4x
 *                   Pointer to the target TIMER4
 *                   -  TIMER40 ~ TIMER43
 * @param[in]     u32T4nSet
 *                   TIMER4n Control Register Setting Data
 * @param[in]     u32T4nClk
 *                   TIMER4n Prescaler Data
 * @return        @ref HAL_Status_Type
 * @details       This function sets the mode, external clock edge, Output pair,
 *                Delay Time Insertion and Capture Polarity.
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_TIMER4n_SetRegister( TIMER4n_Type* TIMER4x, uint32_t u32T4nSet, uint32_t u32T4nClk )
{
   /* Check TIMER4 handle */
   if( TIMER4x == NULL )
   {
      return HAL_ERROR;
   }

   TIMER4x->CR = u32T4nSet;      // Setting TIMER4n Control Register

   TIMER4x->PREDR = u32T4nClk;   // Setting TIMER4n Prescaler data

   return HAL_OK;
}


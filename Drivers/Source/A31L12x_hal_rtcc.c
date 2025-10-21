/**
 *******************************************************************************
 * @file        A31L12x_hal_rtcc.c
 * @author      ABOV R&D Division
 * @brief       Real-time Clock Control
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

#include "A31L12x_hal_rtcc.h"
#include "A31L12x_hal_scu.h"

/* Public Functions --------------------------------------------------------- */
//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Initialize the RTCC peripheral with the specified parameters.
 * @param[in]     RTCC_Config
 *                   Pointer to the configuration information for the specified peripheral.
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_RTCC_Init( void* RTCC_Config )
{
   RTCC_CFG_Type*    pRtccCfg;
   uint32_t          reg_val32;

   /* Check RTCC_Config */
   if( RTCC_Config == NULL )
   {
      return HAL_ERROR;
   }

   // enable peripheral clock: RTCC.
   HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_RTCCLKE, PPxCLKE_Enable );

   pRtccCfg = ( RTCC_CFG_Type* )RTCC_Config;

   reg_val32 = 0
               | RTCC_CR_RTIN_SET( pRtccCfg->rtccIntIn )
               | RTCC_CR_HS24_SET( pRtccCfg->rtccHS24 )
               | RTCC_CR_OUTSEL_SET( pRtccCfg->rtccOutsel )
               ;

   RTCC->CR = reg_val32;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Close RTCC
 * @return        None
 *//*-------------------------------------------------------------------------*/
void HAL_RTCC_DeInit( void )
{
   HAL_SCU_Peripheral_SetReset2( 1 << SCUCG_PPRST2_RTCRST_Pos );
   HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_RTCCLKE, PPxCLKE_Disable );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set RTC_CR Registers
 * @param[in]     u32rtccSet
 *                   RTCC Control Register Setting Data
 * @return        None
 * @details       This function sets the mode, Interrupt Interval, 12/24-hour System Selection,
 *                Alarm Match Operation and RTCOUT Selection.
 *//*-------------------------------------------------------------------------*/

void HAL_RTCC_SetRegister( uint32_t u32rtccSet )
{
   RTCC->CR = u32rtccSet;      // Setting RTCC Control Register
}

/*-------------------------------------------------------------------------*//**
 * @brief         Enable/Disable RTCC Operation
 * @param[in]     NewState
 *                   Next State of Functional Operation
 *                   -  ENABLE, DISABLE
 * @return        None
 *//*-------------------------------------------------------------------------*/
void HAL_RTCC_Cmd( FunctionalState NewState )
{
   if( NewState == ENABLE )
   {
      RTCC->CR |=  RTCC_RTEN;
   }
   else
   {
      RTCC->CR &= ~RTCC_RTEN;
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Rewrite to RTCC YEAR Register
 * @param[in]     u32rtccReWt
 *                   RTCC YEAR Counter Register data
 * @return        None
 *//*-------------------------------------------------------------------------*/
void HAL_RTCC_RewriteYEAR( uint32_t u32rtccReWt )
{
   RTCC->CR |= ( 1 << RTCC_CR_RTWAIT_Pos );            // Stop RTSEC to RTYEAR counters for read/write counter value.
   while( ( RTCC->CR & ( 1 << RTCC_CR_RTWST_Pos ) ) == 0 ) {}			// wait until RTWST = 1
   RTCC->YEAR = u32rtccReWt; 						              // Write to RTCC->YEAR register
   RTCC->CR &= ~RTCC_CR_RTWAIT_Msk;	                  // Set counter Operation
   while( ( RTCC->CR & ( 1 << RTCC_CR_RTWST_Pos ) ) == 1 ) {}		 // wait until RTWST = 0
}

/*-------------------------------------------------------------------------*//**
 * @brief         Rewrite to RTCC MONTH Register
 * @param[in]     u32rtccReWt
 *                   RTCC MONTH Counter Register data
 * @return        None
 *//*-------------------------------------------------------------------------*/
void HAL_RTCC_RewriteMONTH( uint32_t u32rtccReWt )
{
   RTCC->CR |= ( 1 << RTCC_CR_RTWAIT_Pos );            // Stop RTSEC to RTYEAR counters for read/write counter value.
   while( ( RTCC->CR & ( 1 << RTCC_CR_RTWST_Pos ) ) == 0 ) {}		// wait until RTWST = 1
   RTCC->MONTH = u32rtccReWt; 						              // Write to RTCC->MONTH register
   RTCC->CR &= ~RTCC_CR_RTWAIT_Msk;	                  // Set counter Operation
   while( ( RTCC->CR & ( 1 << RTCC_CR_RTWST_Pos ) ) == 1 ) {}		 // wait until RTWST = 0
}

/*-------------------------------------------------------------------------*//**
 * @brief         Rewrite to RTCC WEEK Register
 * @param[in]     u32rtccReWt
 *                   RTCC WEEK Counter Register data
 * @return        None
 *//*-------------------------------------------------------------------------*/
void HAL_RTCC_RewriteWEEK( uint32_t u32rtccReWt )
{
   RTCC->CR |= ( 1 << RTCC_CR_RTWAIT_Pos );            // Stop RTSEC to RTYEAR counters for read/write counter value.
   while( ( RTCC->CR & ( 1 << RTCC_CR_RTWST_Pos ) ) == 0 ) {}		// wait until RTWST = 1
   RTCC->WEEK = u32rtccReWt; 						              // Write to RTCC->WEEK register
   RTCC->CR &= ~RTCC_CR_RTWAIT_Msk;	                  // Set counter Operation
   while( ( RTCC->CR & ( 1 << RTCC_CR_RTWST_Pos ) ) == 1 ) {}		 // wait until RTWST = 0
}

/*-------------------------------------------------------------------------*//**
 * @brief         Rewrite to RTCC DAY Register
 * @param[in]     u32rtccReWt
 *                   RTCC DAY Counter Register data
 * @return        None
 *//*-------------------------------------------------------------------------*/
void HAL_RTCC_RewriteDAY( uint32_t u32rtccReWt )
{
   RTCC->CR |= ( 1 << RTCC_CR_RTWAIT_Pos );            // Stop RTSEC to RTYEAR counters for read/write counter value.
   while( ( RTCC->CR & ( 1 << RTCC_CR_RTWST_Pos ) ) == 0 ) {}		// wait until RTWST = 1
   RTCC->DAY = u32rtccReWt; 						              // Write to RTCC->DAY register
   RTCC->CR &= ~RTCC_CR_RTWAIT_Msk;	                  // Set counter Operation
   while( ( RTCC->CR & ( 1 << RTCC_CR_RTWST_Pos ) ) == 1 ) {}		 // wait until RTWST = 0
}

/*-------------------------------------------------------------------------*//**
 * @brief         Rewrite to RTCC HOUR Register
 * @param[in]     u32rtccReWt
 *                   RTCC HOUR Counter Register data
 * @return        None
 *//*-------------------------------------------------------------------------*/
void HAL_RTCC_RewriteHOUR( uint32_t u32rtccReWt )
{
   RTCC->CR |= ( 1 << RTCC_CR_RTWAIT_Pos );            // Stop RTSEC to RTYEAR counters for read/write counter value.
   while( ( RTCC->CR & ( 1 << RTCC_CR_RTWST_Pos ) ) == 0 ) {}		// wait until RTWST = 1
   RTCC->HOUR = u32rtccReWt; 						              // Write to RTCC->HOUR register
   RTCC->CR &= ~RTCC_CR_RTWAIT_Msk;	                  // Set counter Operation
   while( ( RTCC->CR & ( 1 << RTCC_CR_RTWST_Pos ) ) == 1 ) {}		 // wait until RTWST = 0
}

/*-------------------------------------------------------------------------*//**
 * @brief         Rewrite to RTCC MIN Register
 * @param[in]     u32rtccReWt
 *                   RTCC MIN Counter Register data
 * @return        None
 *//*-------------------------------------------------------------------------*/
void HAL_RTCC_RewriteMIN( uint32_t u32rtccReWt )
{
   RTCC->CR |= ( 1 << RTCC_CR_RTWAIT_Pos );            // Stop RTSEC to RTYEAR counters for read/write counter value.
   while( ( RTCC->CR & ( 1 << RTCC_CR_RTWST_Pos ) ) == 0 ) {}		// wait until RTWST = 1
   RTCC->MIN = u32rtccReWt; 						              // Write to RTCC->MIN register
   RTCC->CR &= ~RTCC_CR_RTWAIT_Msk;	                  // Set counter Operation
   while( ( RTCC->CR & ( 1 << RTCC_CR_RTWST_Pos ) ) == 1 ) {}		 // wait until RTWST = 0
}

/*-------------------------------------------------------------------------*//**
 * @brief         Rewrite to RTCC SEC Register
 * @param[in]     u32rtccReWt
 *                   RTCC SEC Counter Register data
 * @return        None
 *//*-------------------------------------------------------------------------*/
void HAL_RTCC_RewriteSEC( uint32_t u32rtccReWt )
{
   RTCC->CR |= ( 1 << RTCC_CR_RTWAIT_Pos );            // Stop RTSEC to RTYEAR counters for read/write counter value.
   while( ( RTCC->CR & ( 1 << RTCC_CR_RTWST_Pos ) ) == 0 ) {}		// wait until RTWST = 1
   RTCC->SEC = u32rtccReWt; 						              // Write to RTCC->SEC register
   RTCC->CR &= ~RTCC_CR_RTWAIT_Msk;	                  // Set counter Operation
   while( ( RTCC->CR & ( 1 << RTCC_CR_RTWST_Pos ) ) == 1 ) {}		 // wait until RTWST = 0
}


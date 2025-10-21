/**
 *******************************************************************************
 * @file        A31L12x_hal_wdt.c
 * @author      ABOV R&D Division
 * @brief       Watch Dog Timer
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

#include "A31L12x_hal_wdt.h"
#include "A31L12x_hal_scu.h"

/* Public Functions --------------------------------------------------------- */
//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Initialize the WDT peripheral with the specified parameters.
 * @param[in]     WDT_Config
 *                   Pointer to a WDT_CFG_Type structure
 *                   that contains the configuration information for the specified peripheral.
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_WDT_Init( WDT_CFG_Type* WDT_Config )
{
   uint32_t    reg_val = 0;

   /* Check WDT_Config */
   if( WDT_Config == NULL )
   {
      return HAL_ERROR;
   }

   // enable peripheral clock
   HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_WDTCLKE, PPxCLKE_Enable );

   WDT->DR = ( WDT_Config->wdtTmrConst & 0x00FFFFFF );
   WDT->WINDR = ( WDT_Config->wdtWTmrConst & 0x00FFFFFF );
   reg_val = WDT_Config->wdtClkDiv;
   if( WDT_Config->wdtResetEn == ENABLE )
   {
      reg_val &= ~( 0x3f << WDT_CR_RSTEN_Pos );
   }
   else
   {
      reg_val |= ( 0x25 << WDT_CR_RSTEN_Pos );
   }
   WDT->CR = ( 0x5A69 << WDT_CR_WTIDKY_Pos ) | ( 0x1a << WDT_CR_CNTEN_Pos ) | reg_val;    // /w Write Identification Key

   // return
   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Deinitialize WDT
 * @param         None
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_WDT_DeInit( void )
{
   WDT->CR = 0
             | ( 0x5A69 << WDT_CR_WTIDKY_Pos )  // Write Identification Key
             | ( 0x25  << WDT_CR_RSTEN_Pos )    // Disable watch-dog timer reset
             | ( 0x1A << WDT_CR_CNTEN_Pos )     // Disable watch-dog timer counter
             ;

   // return
   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure the peripheral interrupt.
 * @param[in]     WDT_IntCfg
 *                   Specifies the interrupt flag
 *                   -  WDT_INTCFG_UNFIEN:   UNFIEN Interrupt enable
 *                   -  WDT_INTCFG_WINMIEN:  WINMIEN Interrupt enable
 * @param[in]     NewState
 *                   Next State of Interrupt Operation
 *                   -  ENABLE, DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_WDT_ConfigInterrupt( WDT_INT_Type WDT_IntCfg, FunctionalState NewState )
{
#if 0    // before bug fix
   uint32_t    reg_val = 0;
   uint32_t    tmp = 0;

   reg_val = ( WDT->CR & 0xFFFF );

   switch( WDT_IntCfg )
   {
      case WDT_INTCFG_UNFIEN:
         tmp = WDT_CR_UNFIEN;
         break;
      case WDT_INTCFG_WINMIEN:
         tmp = WDT_CR_WINMIEN;
         break;
   }

   if( NewState == ENABLE )
   {
      reg_val |= ( tmp & WDT_INTERRUPT_BITMASK );
   }
   else
   {
      reg_val &= ( ( ~tmp ) & WDT_INTERRUPT_BITMASK );   // reg_val &= ~tmp;�� ���� ��...
   }

   WDT->CR = ( 0x5A69 << WDT_CR_WTIDKY_Pos ) | reg_val;  // Write Identification Key 0x5A69
#else    // after bug fix
   uint32_t    reg_val = 0;
   uint32_t    mask = 0;

   switch( WDT_IntCfg )
   {
      case WDT_INTCFG_UNFIEN:
         mask = WDT_CR_UNFIEN_Msk;
         break;
      case WDT_INTCFG_WINMIEN:
         mask = WDT_CR_WINMIEN_Msk;
         break;
   }

   reg_val =   WDT->CR
               & ~( WDT_CR_WTIDKY_Msk )
               | ( ( uint32_t )WDT_CR_WTIDKY_Value << WDT_CR_WTIDKY_Pos )
               ;
   if( NewState == ENABLE )
   {
      reg_val |= mask;
   }
   else
   {
      reg_val &= ~mask;
   }

   WDT->CR = reg_val;
#endif

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Reload WDT counter
 * @param         None
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_WDT_ReloadTimeCounter( void )
{
   WDT->CNTR = 0x6a;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Enable WDT activity
 * @param[in]     ctrl
 *                   -  DISABLE:  wdt enable
 *                   -  ENABLE:   wdt disable
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_WDT_Start( FunctionalState ctrl )
{
   uint32_t    tmp_reg;

   tmp_reg = WDT->CR & 0xFFFF;
   tmp_reg |= ( 0x1a << WDT_CR_CNTEN_Pos );  // Disable watch-dog timer counter

   if( ctrl == ENABLE )
   {
      tmp_reg &= ~( 0x3f << WDT_CR_CNTEN_Pos );    // Enable watch-dog timer counter,
   }

   WDT->CR = ( 0x5A69 << WDT_CR_WTIDKY_Pos ) | tmp_reg;  // Write Identification Key 0x5A69

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Clear the timer status register of WDT
 * @param[in]     clrbit
 *                   -  WDT_SR_UNFIFLAG:     UNFIFLAG Interrupt flag
 *                   -  WDT_SR_WINMIFLAG:    WINMIFLAG Interrupt flag
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_WDT_ClearStatus( uint32_t clrbit )
{
   WDT->SR = clrbit;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Get the timer status register of WDT
 * @param         None
 * @return        the status register of WDT
 *//*-------------------------------------------------------------------------*/
uint32_t HAL_WDT_GetStatus( void )
{
   return WDT->SR;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Get the current value of WDT
 * @param         None
 * @return        current value of WDT
 *//*-------------------------------------------------------------------------*/
uint32_t HAL_WDT_GetCurrentCount( void )
{
   return WDT->CNT;
}


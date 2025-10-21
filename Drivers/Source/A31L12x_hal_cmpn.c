/**
 *******************************************************************************
 * @file        A31L12x_hal_cmpn.c
 * @author      ABOV R&D Division
 * @brief       Comparator
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

#include "A31L12x_hal_cmpn.h"
#include "A31L12x_hal_scu.h"

/* Public Functions --------------------------------------------------------- */
//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Initialize the target peripheral.
 * @param[in]     CMPx
 *                   Pointer to the target CMP peripheral
 *                   -  CMP0 ~ CMP1
 * @return        @ref HAL_Status_Type
 * @details       This function enables the clock of target peripheral and resets it.
 * @code
 *                [Example]
 *
 *                HAL_COMP_Init( ( CMPn_Type* )CMP0 );
 * @endcode
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_COMP_Init( CMPn_Type* CMPx )
{
   /* Check CMP handle */
   if( CMPx == NULL )
   {
      return HAL_ERROR;
   }

#if 1    // supported
   if( CMPx == ( CMPn_Type* )CMP0 )
   {
      // enable peripheral clock and reset peripheral
      HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_CMP0CLKE, PPxCLKE_Enable );
      HAL_SCU_Peripheral_SetReset2( PPRST2_CMP0RST );
   }
#endif

#if 1    // supported
   if( CMPx == ( CMPn_Type* )CMP1 )
   {
      // enable peripheral clock and reset peripheral
      HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_CMP1CLKE, PPxCLKE_Enable );
      HAL_SCU_Peripheral_SetReset2( PPRST2_CMP1RST );
   }
#endif

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Deinitialize the target peripheral.
 * @param[in]     CMPx
 *                   Pointer to the target CMP peripheral
 *                   -  CMP0 ~ CMP1
 * @return        @ref HAL_Status_Type
 * @details       This function resets the target peripheral and disables it's clock.
 * @code
 *                [Example]
 *
 *                HAL_COMP_Deinit( ( CMPn_Type* )CMP0 );
 * @endcode
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_COMP_Deinit( CMPn_Type* CMPx )
{
   /* Check CMP handle */
   if( CMPx == NULL )
   {
      return HAL_ERROR;
   }

#if 1    // supported
   if( CMPx == ( CMPn_Type* )CMP0 )
   {
      // reset peripheral and disable peripheral clock
      HAL_SCU_Peripheral_SetReset2( PPRST2_CMP0RST );
      HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_CMP0CLKE, PPxCLKE_Disable );
   }
#endif

#if 1    // supported
   if( CMPx == ( CMPn_Type* )CMP1 )
   {
      // reset peripheral and disable peripheral clock
      HAL_SCU_Peripheral_SetReset2( PPRST2_CMP1RST );
      HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_CMP1CLKE, PPxCLKE_Disable );
   }
#endif

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Select the positive input pin of a comparator
 * @param[in]     CMPx
 *                   Pointer to the target CMP peripheral
 *                   -  CMP0 ~ CMP1
 * @param[in]     PositiveInputPin
 *                   Positive input pin
 *                   -  CMPnPOS_CPnP0
 *                   -  CMPnPOS_CPnP1
 *                   -  CMPnPOS_CPnP2
 *                   -  CMPnPOS_CPnP3
 *                   -  CMPnPOS_CPnP4
 *                   -  CMPnPOS_CPnP5
 * @return        @ref HAL_Status_Type
 * @code
 *                [Example]
 *
 *                HAL_COMP_ConfigPositiveInput( ( CMPn_Type* )CMP0, CMPnPOS_CPnP0 );
 * @endcode
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_COMP_ConfigPositiveInput( CMPn_Type* CMPx, CMPn_CR_CMPnPOS_Enum PositiveInputPin )
{
   /* Check CMP handle */
   if( CMPx == NULL )
   {
      return HAL_ERROR;
   }

   CMPx->CR_b.CMPnPOS = PositiveInputPin;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Select the negative input pin of a comparator.
 * @param[in]     CMPx
 *                   Pointer to the target CMP peripheral
 *                   -  CMP0 ~ CMP1
 * @param[in]     NegativeInputPin
 *                   Negative input pin
 *                   -  CMPnNEG_CPnN0
 *                   -  CMPnNEG_CPnN1
 *                   -  CMPnNEG_CPnN2
 *                   -  CMPnNEG_CPnN3
 *                   -  CMPnNEG_IntRef
 * @return        @ref HAL_Status_Type
 * @code
 *                [Example]
 *
 *                HAL_COMP_ConfigNegativeInput( ( CMPn_Type* )CMP0, CMPnNEG_IntRef );
 * @endcode
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_COMP_ConfigNegativeInput( CMPn_Type* CMPx, CMPn_CR_CMPnNEG_Enum NegativeInputPin )
{
   /* Check CMP handle */
   if( CMPx == NULL )
   {
      return HAL_ERROR;
   }

   CMPx->CR_b.CMPnNEG = NegativeInputPin;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Control the hysteresis function.
 * @param[in]     CMPx
 *                   Pointer to the target CMP peripheral
 *                   -  CMP0 ~ CMP1
 * @param[in]     ControlValue
 *                   -  HYSnEN_Disable
 *                   -  HYSnEN_Enable
 * @return        @ref HAL_Status_Type
 * @code
 *                [Example]
 *
 *                HAL_COMP_ConfigHysteresis( ( CMPn_Type* )CMP0, HYSnEN_Enable );
 * @endcode
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_COMP_ConfigHysteresis( CMPn_Type* CMPx, CMPn_CR_HYSnEN_Enum ControlValue )
{
   /* Check CMP handle */
   if( CMPx == NULL )
   {
      return HAL_ERROR;
   }

   CMPx->CR_b.HYSnEN = ControlValue;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Select the speed of a comparator.
 * @param[in]     CMPx
 *                   Pointer to the target CMP peripheral
 *                   -  CMP0 ~ CMP1
 * @param[in]     Speed
 *                   -  CMPnSPD_Slow
 *                   -  CMPnSPD_Fast
 * @return        @ref HAL_Status_Type
 * @code
 *                [Example]
 *
 *                HAL_COMP_ConfigSpeed( ( CMPn_Type* )CMP0, CMPnSPD_Slow );
 * @endcode
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_COMP_ConfigSpeed( CMPn_Type* CMPx, CMPn_CR_CMPnSPD_Enum Speed )
{
   /* Check CMP handle */
   if( CMPx == NULL )
   {
      return HAL_ERROR;
   }

   CMPx->CR_b.CMPnSPD = Speed;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Select the sampling clock of a noise filter.
 * @param[in]     CMPx
 *                   Pointer to the target CMP peripheral
 *                   -  CMP0 ~ CMP1
 * @param[in]     SamplingClock
 *                   -  NFCKn_PCLK1
 *                   -  NFCKn_PCLK2
 *                   -  NFCKn_PCLK4
 *                   -  NFCKn_PCLK8
 *                   -  NFCKn_PCLK16
 *                   -  NFCKn_PCLK32
 *                   -  NFCKn_PCLK64
 * @return        @ref HAL_Status_Type
 * @code
 *                [Example]
 *
 *                HAL_COMP_ConfigNoiseFilter( ( CMPn_Type* )CMP0, NFCKn_PCLK1 );
 * @endcode
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_COMP_ConfigNoiseFilter( CMPn_Type* CMPx, CMPn_CR_NFCKn_Enum SamplingClock )
{
   /* Check CMP handle */
   if( CMPx == NULL )
   {
      return HAL_ERROR;
   }

   CMPx->CR_b.NFCKn = SamplingClock;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Select the interrupt trigger polarity.
 * @param[in]     CMPx
 *                   Pointer to the target CMP peripheral
 *                   -  CMP0 ~ CMP1
 * @param[in]     Polarity
 *                   -  CMPnPOL_NoInterrupt
 *                   -  CMPnPOL_FallingEdge
 *                   -  CMPnPOL_RisingEdge
 *                   -  CMPnPOL_BothEdge
 * @return        @ref HAL_Status_Type
 * @code
 *                [Example]
 *
 *                HAL_COMP_ConfigInterruptPolarity( ( CMPn_Type* )CMP0, CMPnPOL_BothEdge );
 * @endcode
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_COMP_ConfigInterruptPolarity( CMPn_Type* CMPx, CMPn_CR_CMPnPOL_Enum Polarity )
{
   /* Check CMP handle */
   if( CMPx == NULL )
   {
      return HAL_ERROR;
   }

   CMPx->CR_b.CMPnPOL = Polarity;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Control the reference voltage generator and select the reference voltage.
 * @param[in]     CMPx
 *                   Pointer to the target CMP peripheral
 *                   -  CMP0 ~ CMP1
 * @param[in]     ControlValue
 *                   -  REFnEN_Disable
 *                   -  REFnEN_Enable
 * @param[in]     RefVoltage
 *                   -  CMPnREF_2over11VDD
 *                   -  CMPnREF_3over11VDD
 *                   -  CMPnREF_4over11VDD
 *                   -  CMPnREF_5over11VDD
 *                   -  CMPnREF_6over11VDD
 *                   -  CMPnREF_7over11VDD
 *                   -  CMPnREF_8over11VDD
 *                   -  CMPnREF_9over11VDD
 * @return        @ref HAL_Status_Type
 * @code
 *                [Example]
 *
 *                HAL_COMP_ConfigIntRef( ( CMPn_Type* )CMP0, REFnEN_Enable, CMPnREF_6over11VDD );
 * @endcode
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_COMP_ConfigIntRef( CMPn_Type* CMPx, CMPn_RCR_REFnEN_Enum ControlValue, CMPn_RCR_CMPnREF_Enum RefVoltage )
{
   /* Check CMP handle */
   if( CMPx == NULL )
   {
      return HAL_ERROR;
   }

   CMPx->RCR_b.REFnEN = ControlValue;
   CMPx->RCR_b.CMPnREF = RefVoltage;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Enable the comparator function
 * @param[in]     CMPx
 *                   Pointer to the target CMP peripheral
 *                   -  CMP0 ~ CMP1
 * @return        @ref HAL_Status_Type
 * @code
 *                [Example]
 *
 *                HAL_COMP_Enable( ( CMPn_Type* )CMP0 );
 * @endcode
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_COMP_Enable( CMPn_Type* CMPx )
{
   /* Check CMP handle */
   if( CMPx == NULL )
   {
      return HAL_ERROR;
   }

   CMPx->CR_b.CMPnEN = CMPn_CR_CMPnEN_Enable;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Disable the comparator function
 * @param[in]     CMPx
 *                   Pointer to the target CMP peripheral
 *                   -  CMP0 ~ CMP1
 * @return        @ref HAL_Status_Type
 * @code
 *                [Example]
 *
 *                HAL_COMP_Disable( ( CMPn_Type* )CMP0 );
 * @endcode
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_COMP_Disable( CMPn_Type* CMPx )
{
   /* Check CMP handle */
   if( CMPx == NULL )
   {
      return HAL_ERROR;
   }

   CMPx->CR_b.CMPnEN = CMPn_CR_CMPnEN_Disable;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Clear the Status Register
 * @param[in]     CMPx
 *                   Pointer to the target CMP peripheral
 *                   -  CMP0 ~ CMP1
 * @param[in]     mask
 *                   Mask indicating the target status flag.
 *                   -  CMPn_SR_CMPnIFLAG_Msk
 * @return        @ref HAL_Status_Type
 * @code
 *                [Example]
 *
 *                HAL_COMP_ClearStatus( ( CMPn_Type* )CMP0, CMPn_SR_CMPnIFLAG_Msk );
 * @endcode
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_COMP_ClearStatus( CMPn_Type* CMPx, uint32_t mask )
{
   /* Check CMP handle */
   if( CMPx == NULL )
   {
      return HAL_ERROR;
   }

   switch( mask )
   {
      case CMPn_SR_CMPnIFLAG_Msk:
         CMPx->SR =  0
                     | CMPn_SR_CMPnIFLAG_Msk
                     ;
         break;
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Get the Status Register
 * @param[in]     CMPx
 *                   Pointer to the target CMP peripheral
 *                   -  CMP0 ~ CMP1
 * @return        Status Register value
 * @code
 *                [Example]
 *
 *                status = HAL_COMP_GetStatus( ( CMPn_Type* )CMP0 );
 * @endcode
 *//*-------------------------------------------------------------------------*/
uint32_t HAL_COMP_GetStatus( CMPn_Type* CMPx )
{
   return CMPx->SR;
}


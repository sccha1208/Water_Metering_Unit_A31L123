/**
 *******************************************************************************
 * @file        A31L12x_hal_adc.c
 * @author      ABOV R&D Division
 * @brief       Analog-to-Digital Converter
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

#include "A31L12x_hal_adc.h"
#include "A31L12x_hal_scu.h"

/* Public Functions --------------------------------------------------------- */
//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Initialize the ADC peripheral with the specified parameters.
 * @param[in]     ADCx
 *                   Pointer to the target ADC
 *                   -  ADC
 * @param[in]     ADC_Config
 *                   Pointer to a ADC_CFG_Type structure
 *                   that contains the configuration information for the specified peripheral.
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_ADC_Init( ADC_Type* ADCx, ADC_CFG_Type* ADC_Config )
{
   uint32_t    tempreg;

   /* Check ADC handle */
   if( ADCx == NULL )
   {
      return HAL_ERROR;
   }

   HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_ADCLKE, PPxCLKE_Enable );

   tempreg = 0
             | ( ( ( ADC_Config->TrgSel ) & 7 ) << ADC_CR_TRIG_Pos )        // TRGSRC
             | ( ( ( ADC_Config->ExtTrgPolSel ) & 3 ) << ADC_CR_ETRGP_Pos ) // Ext Trigger Polarity
             | ( ( ( ADC_Config->ConvModSel ) & 3 ) << ADC_CR_MDSEL_Pos )   // Conversion Mode
             ;
   ADCx->CR = tempreg;

   ADCx->PREDR = ( ADC_Config->InClkDiv & ADC_PREDR_PRED_Msk );

   ADCx->SAMR = ( ADC_Config->SamplingClk & ADC_SAMR_SAMCK_Msk );            // Sampling Cycles: SamplingClk + 2

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Close ADC
 * @param[in]     ADCx
 *                   Pointer to the target ADC
 *                   -  ADC
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_ADC_DeInit( ADC_Type* ADCx )
{
   /* Check ADC handle */
   if( ADCx == NULL )
   {
      return HAL_ERROR;
   }

   ADCRDY_Stop();    // Stop subsequent steps
   ADCDIS();         // Disable ADC module
   HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_ADCLKE, PPxCLKE_Disable );

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         ADC interrupt configuration
 * @param[in]     ADCx
 *                   Pointer to the target ADC
 *                   -  ADC
 * @param[in]     ADCIntCfg
 *                   ADC_INT_Type value
 * @param[in]     NewState
 *                   Next State of Interrupt Operation
 *                   -  ENABLE, DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_ADC_ConfigInterrupt( ADC_Type* ADCx, ADC_INT_Type ADCIntCfg, FunctionalState NewState )
{
   uint32_t    tempreg;

   /* Check ADC handle */
   if( ADCx == NULL )
   {
      return HAL_ERROR;
   }

   // get mask
   switch( ADCIntCfg )
   {
      case ADC_INTCFG_EOS:
         tempreg = ADC_IESR_EOSIEN_Msk;
         break;
      case ADC_INTCFG_EOC:
         tempreg = ADC_IESR_EOCIEN_Msk;
         break;
      case ADC_INTCFG_OVRUN:
         tempreg = ADC_IESR_OVRUNIEN_Msk;
         break;
      case ADC_INTCFG_STB:
         tempreg = ADC_IESR_STBIEN_Msk;
         break;
      default:
         return HAL_ERROR;
   }

   // enable/disable
   if( NewState == ENABLE )
   {
      ADCx->IESR = ( ( ADCx->IESR & ADC_INT_STATUS_MSK ) | tempreg );
   }
   else
   {
      ADCx->IESR = ( ( ADCx->IESR & ADC_INT_STATUS_MSK ) & ( ~tempreg ) );
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         ADC oversampling configuration
 * @param[in]     ADCx
 *                   Pointer to the target ADC
 *                   -  ADC
 * @param[in]     OvSampRatio
 *                   OverSampling Ratio
 *                   -  0 ~ 7: 2**(in+1)
 * @param[in]     DataShiftRight
 *                   OverSampling Data Shift bits
 *                   -  0 ~ 8: result data are shifted right by in bits
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_ADC_OvSamplingConfig( ADC_Type* ADCx, ADC_OV_SAMPLING_RATIO OvSampRatio, ADC_OVSCR_OVSHT_Enum DataShiftRight )
{
   uint32_t    tempreg;

   /* Check ADC handle */
   if( ADCx == NULL )
   {
      return HAL_ERROR;
   }

   if( DataShiftRight > ADC_OVSCR_OVSHT_ShiftRight8Bit )
   {
      DataShiftRight = ADC_OVSCR_OVSHT_NoShift;
   }
   tempreg = 0
             | ( ( ( OvSampRatio ) & 7 ) << ADC_OVSCR_OVSMPR_Pos )      // Oversampling ratio
             | ( ( DataShiftRight ) << ADC_OVSCR_OVSHT_Pos )            // Shift right bits
             ;
   ADCx->OVSCR = tempreg;
   ADC_OVSMPEN();                                                       // ADC Oversampling Enable

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Select ADC Channels to conversion (Ex: ADC_AN0 | ADC_AN5 | ADC_AN14 | ADC_AVDD)
 * @param[in]     ADCx
 *                   Pointer to the target ADC
 *                   -  ADC
 * @param[in]     Channels
 *                   Channel Numbers
 *                   -  ADC_CHSELR_AN0_Msk | ... | ADC_CHSELR_AN16_Msk | ADC_CHSELR_AN18_Msk | ADC_CHSELR_AN19_Msk
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_ADC_ChannelSel( ADC_Type* ADCx, uint32_t Channels )
{
   /* Check ADC handle */
   if( ADCx == NULL )
   {
      return HAL_ERROR;
   }

   ADCx->CHSELR = Channels;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Start A/D conversion
 * @param[in]     ADCx
 *                   Pointer to the target ADC
 *                   -  ADC
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_ADC_Start( ADC_Type* ADCx )
{
   /* Check ADC handle */
   if( ADCx == NULL )
   {
      return HAL_ERROR;
   }

   ADCADST_Set();    // Start conversion by s/w

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Ready procedure for ADC conversion
 * @param[in]     ADCx
 *                   Pointer to the target ADC
 *                   -  ADC
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_ADC_RdyCmd( ADC_Type* ADCx )
{
   /* Check ADC handle */
   if( ADCx == NULL )
   {
      return HAL_ERROR;
   }

   ADCEN();                            // ADCEN;
   while( !ADCInt_GetStbFg() ) {}      // Check ADC stabiliaztion
   ADCInt_ClrStbFg();                  // Clear the stabiliaztion flag
   ADCRDY_Conv();                      // Ready for conversion

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Clear ADC interrupt status
 * @param[in]     ADCx
 *                   Pointer to the target ADC
 *                   -  ADC
 * @param[in]     ADCInt_status
 *                   Bit position to be cleared
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_ADC_ClearStatus( ADC_Type* ADCx, ADC_INT_STATUS_Type ADCInt_status )
{
   uint32_t    tempreg;

   /* Check ADC handle */
   if( ADCx == NULL )
   {
      return HAL_ERROR;
   }

   switch( ADCInt_status )
   {
      case ADC_INT_STATUS_EOS:
         tempreg = ADC_IESR_EOSIFLAG_Msk;
         break;
      case ADC_INT_STATUS_EOC:
         tempreg = ADC_IESR_EOCIFLAG_Msk;
         break;
      case ADC_INT_STATUS_OVRUN:
         tempreg = ADC_IESR_OVRUNIFLAG_Msk;
         break;
      case ADC_INT_STATUS_STB:
         tempreg = ADC_IESR_STBIFLAG_Msk;
         break;
      case ADC_INT_STATUS_ALL:
         tempreg = ADC_IESR_EOSIFLAG_Msk | ADC_IESR_EOCIFLAG_Msk | ADC_IESR_OVRUNIFLAG_Msk | ADC_IESR_STBIFLAG_Msk;
         break;
      default:
         return HAL_ERROR;
   }

   ADCx->IESR = ( ADCx->IESR & ADC_INT_STATUS_MSK ) | tempreg;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Get ADC interrupt status
 * @param[in]     ADCx
 *                   Pointer to the target ADC
 *                   -  ADC
 * @return        ADC interrupt status bits
 *//*-------------------------------------------------------------------------*/
uint8_t HAL_ADC_GetStatus( ADC_Type* ADCx )
{
   return ( uint8_t )( ( ADCx->IESR ) & ( ~ADC_INT_STATUS_MSK ) );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Get Result conversion from A/D data register
 * @param[in]     ADCx
 *                   Pointer to the target ADC
 *                   -  ADC
 * @return        Result of conversion
 *//*-------------------------------------------------------------------------*/
uint16_t HAL_ADC_GetData( ADC_Type* ADCx )
{
   uint16_t    adc_value;

   adc_value = ADCx->DR;
   return ADC_DR_RESULT( adc_value );
}


/**
 *******************************************************************************
 * @file        A31L12x_hal_adc.h
 * @author      ABOV R&D Division
 * @brief       ADC Header File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/

#ifndef _ADC_H_
#define _ADC_H_

#include "A31L12x.h"
#include "A31L12x_hal_aa_types.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

//----------   ADC Channel Selection Definition   ----------
#define ADC_AN0                     (0x0001uL << ADC_CHSELR_AN0_Pos)                      // AN0
#define ADC_AN1                     (0x0001uL << ADC_CHSELR_AN1_Pos)                      // AN1
#define ADC_AN2                     (0x0001uL << ADC_CHSELR_AN2_Pos)                      // AN2
#define ADC_AN3                     (0x0001uL << ADC_CHSELR_AN3_Pos)                      // AN3
#define ADC_AN4                     (0x0001uL << ADC_CHSELR_AN4_Pos)                      // AN4
#define ADC_AN5                     (0x0001uL << ADC_CHSELR_AN5_Pos)                      // AN5
#define ADC_AN6                     (0x0001uL << ADC_CHSELR_AN6_Pos)                      // AN6
#define ADC_AN7                     (0x0001uL << ADC_CHSELR_AN7_Pos)                      // AN7
#define ADC_AN8                     (0x0001uL << ADC_CHSELR_AN8_Pos)                      // AN8
#define ADC_AN9                     (0x0001uL << ADC_CHSELR_AN9_Pos)                      // AN9
#define ADC_AN10                    (0x0001uL << ADC_CHSELR_AN10_Pos)                     // AN10
#define ADC_AN11                    (0x0001uL << ADC_CHSELR_AN11_Pos)                     // AN11
#define ADC_AN12                    (0x0001uL << ADC_CHSELR_AN12_Pos)                     // AN12
#define ADC_AN13                    (0x0001uL << ADC_CHSELR_AN13_Pos)                     // AN13
#define ADC_AN14                    (0x0001uL << ADC_CHSELR_AN14_Pos)                     // AN14
#define ADC_AN15                    (0x0001uL << ADC_CHSELR_AN15_Pos)                     // AN15
#define ADC_AVSS                    (0x0001uL << ADC_CHSELR_AN16_Pos)                     // AVSS
#define ADC_AVDD                    (0x0001uL << ADC_CHSELR_AN18_Pos)                     // AVDD
#define ADC_BGR                     (0x0001uL << ADC_CHSELR_AN19_Pos)                     // BGR

//----------   ADC Mask Definition   ----------------
#define ADC_INT_STATUS_MSK          0xfffffff0uL

//******************************************************************************
// Type
//******************************************************************************

//==============================================================================
// Enumeration
//==============================================================================

typedef enum
{
   ADC_INT_STATUS_EOS = 0,
   ADC_INT_STATUS_EOC = 1,
   ADC_INT_STATUS_OVRUN = 2,
   ADC_INT_STATUS_STB = 3,
   ADC_INT_STATUS_ALL = 4
} ADC_INT_STATUS_Type;

typedef enum
{
   ADC_INTCFG_EOS = 0,
   ADC_INTCFG_EOC,
   ADC_INTCFG_OVRUN,
   ADC_INTCFG_STB
} ADC_INT_Type;

typedef enum
{
   ADC_2_OV_SAMPLING = 0,
   ADC_4_OV_SAMPLING,
   ADC_8_OV_SAMPLING,
   ADC_16_OV_SAMPLING,
   ADC_32_OV_SAMPLING,
   ADC_64_OV_SAMPLING,
   ADC_128_OV_SAMPLING,
   ADC_256_OV_SAMPLING
} ADC_OV_SAMPLING_RATIO;

//==============================================================================
// Structure
//==============================================================================

typedef struct
{
   /* ADC_CR */
   uint32_t    TrgSel;
   uint32_t    ExtTrgPolSel;
   uint32_t    ConvModSel;

   /* ADC_PREDR */
   uint32_t    InClkDiv;

   /* ADC_SAMR */
   uint32_t    SamplingClk;
} ADC_CFG_Type;

//******************************************************************************
// Macro
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         ADC Enable
 * @details       This macro Enable ADC Block
 *//*-------------------------------------------------------------------------*/
#define ADCEN()                     (ADC->CR_b.ADCEN = ADC_CR_ADCEN_Enable)

/*-------------------------------------------------------------------------*//**
 * @brief         ADC Disable
 * @details       This macro Disable ADC Block
 *//*-------------------------------------------------------------------------*/
#define ADCDIS()                    (ADC->CR_b.ADCEN = ADC_CR_ADCEN_Disable)

/*-------------------------------------------------------------------------*//**
 * @brief         ADC Conversion Ready Stop
 * @details       This macro stop subsequent steps of ADC
 *//*-------------------------------------------------------------------------*/
#define ADCRDY_Stop()               (ADC->CR_b.ADRDY = ADC_CR_ADRDY_Stop)

/*-------------------------------------------------------------------------*//**
 * @brief         ADC Conversion Ready to convert
 * @details       This macro make ready to convert
 *//*-------------------------------------------------------------------------*/
#define ADCRDY_Conv()               (ADC->CR_b.ADRDY = ADC_CR_ADRDY_Ready)

/*-------------------------------------------------------------------------*//**
 * @brief         ADC Start bit set
 * @details       This macro starts ADC conversion
 *//*-------------------------------------------------------------------------*/
#define ADCADST_Set()               (ADC->CR_b.ADST = ADC_CR_ADST_Start)

/*-------------------------------------------------------------------------*//**
 * @brief         ADC Oversampling Enable
 * @details       This macro enable oversampling
 *//*-------------------------------------------------------------------------*/
#define ADC_OVSMPEN()               (ADC->OVSCR_b.OVSMPEN = ADC_OVSCR_OVSMPEN_Enable)

/*-------------------------------------------------------------------------*//**
 * @brief         ADC Oversampling Disable
 * @details       This macro disable oversampling
 *//*-------------------------------------------------------------------------*/
#define ADC_OVSMPDIS()              (ADC->OVSCR_b.OVSMPEN = ADC_OVSCR_OVSMPEN_Disable)

/*-------------------------------------------------------------------------*//**
 * @brief         ADC Oversampling Ratio bits set
 * @details       This macro set ADC oversampling ratio
 *//*-------------------------------------------------------------------------*/
#define ADC_SetOvSmpR(adc_ovsmplr)  (ADC->OVSCR_b.OVSMPR = adc_ovsmplr)

/*-------------------------------------------------------------------------*//**
 * @brief         ADC Oversampling Data Shift bits set
 * @details       This macro set the number of ADC Data shift right
 *//*-------------------------------------------------------------------------*/
#define ADC_SetOvSht(adc_ovsht)     (ADC->OVSCR_b.OVSHT = adc_ovsht)

/*-------------------------------------------------------------------------*//**
 * @brief         Get ADC last conversion channel number
 * @details       This macro gets the latest conversion channel number
 *//*-------------------------------------------------------------------------*/
#define ADC_GetLastCh()             (ADC->IESR_b.LASTCH)

/*-------------------------------------------------------------------------*//**
 * @brief         Get flags of ADC Stabilization Interrupt
 * @details       This macro gets stabilization interrupt flag of ADC
 *//*-------------------------------------------------------------------------*/
#define ADCInt_GetStbFg()           (ADC->IESR_b.STBIFLAG)

/*-------------------------------------------------------------------------*//**
 * @brief         Clear flag of ADC Stabilization Interrupt
 * @details       This macro clear stabilization interrupt flag of ADC
 *//*-------------------------------------------------------------------------*/
#define ADCInt_ClrStbFg()           (ADC->IESR = ((ADC->IESR&0xfffffff0)|(0x01ul<<ADC_IESR_STBIFLAG_Pos)))

/*-------------------------------------------------------------------------*//**
 * @brief         Get flag of ADC Over Run Interrupt
 * @details       This macro gets Over Run interrupt flag of ADC
 *//*-------------------------------------------------------------------------*/
#define ADCInt_GetOvRunFg()         (ADC->IESR_b.OVRUNIFLAG)

/*-------------------------------------------------------------------------*//**
 * @brief         Clear flag of ADC Over Run Interrupt
 * @details       This macro clear over run interrupt flag of ADC
 *//*-------------------------------------------------------------------------*/
#define ADCInt_ClrOvRunFg()         (ADC->IESR = ((ADC->IESR&0xfffffff0)|(0x01ul<<ADC_IESR_OVRUNIFLAG_Pos)))

/*-------------------------------------------------------------------------*//**
 * @brief         Get flag of ADC EOC Interrupt
 * @details       This macro gets EOC interrupt flag of ADC
 *//*-------------------------------------------------------------------------*/
#define ADCInt_GetEOCFg()           (ADC->IESR_b.EOCIFLAG)

/*-------------------------------------------------------------------------*//**
 * @brief         Clear flag of ADC EOC Interrupt
 * @details       This macro clear EOC interrupt flag of ADC
 *//*-------------------------------------------------------------------------*/
#define ADCInt_ClrEOCFg()           (ADC->IESR = ((ADC->IESR&0xfffffff0)|(0x01ul<<ADC_IESR_EOCIFLAG_Pos)))

/*-------------------------------------------------------------------------*//**
 * @brief         Get flag of ADC EOS Interrupt
 * @details       This macro gets EOS interrupt flag of ADC
 *//*-------------------------------------------------------------------------*/
#define ADCInt_GetEOSFg()           (ADC->IESR_b.EOSIFLAG)

/*-------------------------------------------------------------------------*//**
 * @brief         Clear flag of ADC EOS Interrupt
 * @details       This macro clear EOS interrupt flag of ADC
 *//*-------------------------------------------------------------------------*/
#define ADCInt_ClrEOSFg()           (ADC->IESR = ((ADC->IESR&0xfffffff0)|(0x01ul<<ADC_IESR_EOSIFLAG_Pos)))

/*-------------------------------------------------------------------------*//**
 * @brief         Get data of ADC Conversion
 * @details       This macro gets data of ADC Conversion
 *//*-------------------------------------------------------------------------*/
#define ADCData_Get()               (ADC->DR)

/*-------------------------------------------------------------------------*//**
 * Macro defines for ADC Data register
 *//*-------------------------------------------------------------------------*/
/**   When DONE is 1, this field contains result value of ADC conversion */
#define ADC_DR_RESULT( n )          ((n) & ADC_DR_ADDATA_Msk)

/*-------------------------------------------------------------------------*//**
 * @brief         ADC Prescaler Data set
 * @details       This macro set the ADC conversion clock
 *//*-------------------------------------------------------------------------*/
#define ADC_SetPreData(adc_pred)    (ADC->PREDR = adc_pred)

/*-------------------------------------------------------------------------*//**
 * @brief         ADC Sampe/Hold Cycles set
 * @details       This macro set sampling cycles for sample/hold
 *//*-------------------------------------------------------------------------*/
#define ADC_SetSamR(adc_samck)      (ADC->SAMR = adc_samck)

//******************************************************************************
// Function
//******************************************************************************

HAL_Status_Type HAL_ADC_Init( ADC_Type* ADCx, ADC_CFG_Type* ADC_Config );
HAL_Status_Type HAL_ADC_DeInit( ADC_Type* ADCx );

HAL_Status_Type HAL_ADC_ConfigInterrupt( ADC_Type* ADCx, ADC_INT_Type ADCIntCfg, FunctionalState NewState );
HAL_Status_Type HAL_ADC_OvSamplingConfig( ADC_Type* ADCx, ADC_OV_SAMPLING_RATIO OvSampRatio, ADC_OVSCR_OVSHT_Enum DataShiftRight );
HAL_Status_Type HAL_ADC_ChannelSel( ADC_Type* ADCx, uint32_t Channels );
HAL_Status_Type HAL_ADC_Start( ADC_Type* ADCx );
HAL_Status_Type HAL_ADC_RdyCmd( ADC_Type* ADCx );
HAL_Status_Type HAL_ADC_ClearStatus( ADC_Type* ADCx, ADC_INT_STATUS_Type ADCInt_status );
uint8_t HAL_ADC_GetStatus( ADC_Type* ADCx );

uint16_t HAL_ADC_GetData( ADC_Type* ADCx );

#ifdef __cplusplus
}
#endif

#endif   /* _ADC_H_ */


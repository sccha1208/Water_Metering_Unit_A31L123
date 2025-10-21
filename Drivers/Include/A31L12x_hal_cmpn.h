/**
 *******************************************************************************
 * @file        A31L12x_hal_cmpn.h
 * @author      ABOV R&D Division
 * @brief       CMP Header File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#ifndef _CMPn_H_
#define _CMPn_H_

#include "A31L12x.h"
#include "A31L12x_hal_aa_types.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

// CMPn_CR_CMPnEN
#define CMPnEN_Disable        CMPn_CR_CMPnEN_Disable
#define CMPnEN_Enable         CMPn_CR_CMPnEN_Enable

// CMPn_CR_CMPnNEG
#define CMPnNEG_CPnN0         CMPn_CR_CMPnNEG_CPnN0
#define CMPnNEG_CPnN1         CMPn_CR_CMPnNEG_CPnN1
#define CMPnNEG_CPnN2         CMPn_CR_CMPnNEG_CPnN2
#define CMPnNEG_CPnN3         CMPn_CR_CMPnNEG_CPnN3
#define CMPnNEG_IntRef        CMPn_CR_CMPnNEG_IntRef

// CMPn_CR_CMPnPOS
#define CMPnPOS_CPnP0         CMPn_CR_CMPnPOS_CPnP0
#define CMPnPOS_CPnP1         CMPn_CR_CMPnPOS_CPnP1
#define CMPnPOS_CPnP2         CMPn_CR_CMPnPOS_CPnP2
#define CMPnPOS_CPnP3         CMPn_CR_CMPnPOS_CPnP3
#define CMPnPOS_CPnP4         CMPn_CR_CMPnPOS_CPnP4
#define CMPnPOS_CPnP5         CMPn_CR_CMPnPOS_CPnP5

// CMPn_CR_HYSnEN
#define HYSnEN_Disable        CMPn_CR_HYSnEN_Disable
#define HYSnEN_Enable         CMPn_CR_HYSnEN_Enable

// CMPn_CR_CMPnSPD
#define CMPnSPD_Slow          CMPn_CR_CMPnSPD_Slow
#define CMPnSPD_Fast          CMPn_CR_CMPnSPD_Fast

// CMPn_CR_CMPnPOL
#define CMPnPOL_NoInterrupt   CMPn_CR_CMPnPOL_NoInterrupt
#define CMPnPOL_FallingEdge   CMPn_CR_CMPnPOL_FallingEdge
#define CMPnPOL_RisingEdge    CMPn_CR_CMPnPOL_RisingEdge
#define CMPnPOL_BothEdge      CMPn_CR_CMPnPOL_BothEdge

// CMPn_CR_NFCKn
#define NFCKn_PCLK1           CMPn_CR_NFCKn_PCLK1
#define NFCKn_PCLK2           CMPn_CR_NFCKn_PCLK2
#define NFCKn_PCLK4           CMPn_CR_NFCKn_PCLK4
#define NFCKn_PCLK8           CMPn_CR_NFCKn_PCLK8
#define NFCKn_PCLK16          CMPn_CR_NFCKn_PCLK16
#define NFCKn_PCLK32          CMPn_CR_NFCKn_PCLK32
#define NFCKn_PCLK64          CMPn_CR_NFCKn_PCLK64

// CMPn_SR_CMPnIFLAG
#define CMPnIFLAG_NoRequest   CMPn_SR_CMPnIFLAG_NoRequest
#define CMPnIFLAG_Request     CMPn_SR_CMPnIFLAG_Request

// CMPn_SR_CMPnST
#define CMPnST_Low            CMPn_SR_CMPnST_Low
#define CMPnST_High           CMPn_SR_CMPnST_High

// CMPn_RCR_REFnEN
#define REFnEN_Disable        CMPn_RCR_REFnEN_Disable
#define REFnEN_Enable         CMPn_RCR_REFnEN_Enable

// CMPn_RCR_CMPnREF
#define CMPnREF_2over11VDD    CMPn_RCR_CMPnREF_2over11VDD
#define CMPnREF_3over11VDD    CMPn_RCR_CMPnREF_3over11VDD
#define CMPnREF_4over11VDD    CMPn_RCR_CMPnREF_4over11VDD
#define CMPnREF_5over11VDD    CMPn_RCR_CMPnREF_5over11VDD
#define CMPnREF_6over11VDD    CMPn_RCR_CMPnREF_6over11VDD
#define CMPnREF_7over11VDD    CMPn_RCR_CMPnREF_7over11VDD
#define CMPnREF_8over11VDD    CMPn_RCR_CMPnREF_8over11VDD
#define CMPnREF_9over11VDD    CMPn_RCR_CMPnREF_9over11VDD

//******************************************************************************
// Function
//******************************************************************************

HAL_Status_Type HAL_COMP_Init( CMPn_Type* CMPx );
HAL_Status_Type HAL_COMP_Deinit( CMPn_Type* CMPx );

HAL_Status_Type HAL_COMP_ConfigPositiveInput( CMPn_Type* CMPx, CMPn_CR_CMPnPOS_Enum PositiveInputPin );
HAL_Status_Type HAL_COMP_ConfigNegativeInput( CMPn_Type* CMPx, CMPn_CR_CMPnNEG_Enum NegativeInputPin );
HAL_Status_Type HAL_COMP_ConfigHysteresis( CMPn_Type* CMPx, CMPn_CR_HYSnEN_Enum ControlValue );
HAL_Status_Type HAL_COMP_ConfigSpeed( CMPn_Type* CMPx, CMPn_CR_CMPnSPD_Enum Speed );
HAL_Status_Type HAL_COMP_ConfigNoiseFilter( CMPn_Type* CMPx, CMPn_CR_NFCKn_Enum SamplingClock );
HAL_Status_Type HAL_COMP_ConfigInterruptPolarity( CMPn_Type* CMPx, CMPn_CR_CMPnPOL_Enum Polarity );
HAL_Status_Type HAL_COMP_ConfigIntRef( CMPn_Type* CMPx, CMPn_RCR_REFnEN_Enum ControlValue, CMPn_RCR_CMPnREF_Enum RefVoltage );
HAL_Status_Type HAL_COMP_Enable( CMPn_Type* CMPx );
HAL_Status_Type HAL_COMP_Disable( CMPn_Type* CMPx );
HAL_Status_Type HAL_COMP_ClearStatus( CMPn_Type* CMPx, uint32_t mask );
uint32_t HAL_COMP_GetStatus( CMPn_Type* CMPx );

#ifdef __cplusplus
}
#endif

#endif   /* _CMPn_H_ */


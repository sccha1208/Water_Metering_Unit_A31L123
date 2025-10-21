/**
 *******************************************************************************
 * @file        A31L12x_hal_lpuart.H
 * @author      ABOV R&D Division
 * @brief       LPUART Header File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#ifndef _LPUART_H_
#define _LPUART_H_

#include "A31L12x.h"
#include "A31L12x_hal_aa_types.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * LPUART time-out definitions in case of using Read() and Write function
 * with Blocking Flag mode
 *//*-------------------------------------------------------------------------*/
#define LPUART_BLOCKING_TIMEOUT    0xffffuL    // (0xFFFFFFFFuL)

//******************************************************************************
// Type
//******************************************************************************

//==============================================================================
// Enumeration
//==============================================================================

typedef enum
{
   LPUART_OVRS_16 = 0,
   LPUART_OVRS_8,
   LPUART_OVRS_1
} LPUART_OVER_SAMPLING_Type;

typedef enum
{
   LPUART_DATA_BIT_5 = 0,
   LPUART_DATA_BIT_6,
   LPUART_DATA_BIT_7,
   LPUART_DATA_BIT_8
} LPUART_DATA_BIT_Type;

typedef enum
{
   LPUART_PARITY_BIT_NONE = 0,   // [PEN:STKPEN:PSEL] = [0:0:0]
   LPUART_PARITY_BIT_ODD  = 4,   // [PEN:STKPEN:PSEL] = [1:0:0]
   LPUART_PARITY_BIT_EVEN,       // [PEN:STKPEN:PSEL] = [1:0:1]
   LPUART_PARITY_BIT_SP0 = 6,    // [PEN:STKPEN:PSEL] = [1:1:0]
   LPUART_PARITY_BIT_SP1         // [PEN:STKPEN:PSEL] = [1:1:1]
} LPUART_PARITY_BIT_Type;

typedef enum
{
   LPUART_STOP_BIT_1 = 0,
   LPUART_STOP_BIT_2
} LPUART_STOP_BIT_Type;

typedef enum
{
   // CR1
   LPUART_CONTROL_PEN = 0,
   LPUART_CONTROL_STKPEN,
   LPUART_CONTROL_PSEL,
   LPUART_CONTROL_STOPB,
   LPUART_CONTROL_HDCOM,
   LPUART_CONTROL_TXE,
   LPUART_CONTROL_RXE,
   LPUART_CONTROL_WAKEN,
   LPUART_CONTROL_LPUEN,

   // CR2
   LPUART_CONTROL_DEPOL,
   LPUART_CONTROL_DEPEN,
   LPUART_CONTROL_RCDEN,
   LPUART_CONTROL_RTOEN
} LPUART_CONTROL_Type;

typedef enum
{
   LPUART_INTCFG_RXCIEN = 0,
   LPUART_INTCFG_TXCIEN,
   LPUART_INTCFG_SBDIEN,
   LPUART_INTCFG_RTOIEN,
   LPUART_INTCFG_RCDIEN
} LPUART_INT_Type;

typedef enum
{
   LPUART_STATUS_RXCIFLAG = 0,
   LPUART_STATUS_TXCIFLAG,
   LPUART_STATUS_SBDIFLAG,
   LPUART_STATUS_RTOIFLAG,
   LPUART_STATUS_RCDIFLAG,
   LPUART_STATUS_RXBUSY,
   LPUART_STATUS_PE,
   LPUART_STATUS_FE,
   LPUART_STATUS_DOR
} LPUART_STATUS_Type;

//==============================================================================
// Structure
//==============================================================================

typedef struct
{
   uint32_t                   BaseClock;
   uint32_t                   Baudrate;

   LPUART_OVER_SAMPLING_Type  OverSampling;

   LPUART_DATA_BIT_Type       DataBit;
   LPUART_PARITY_BIT_Type     ParityBit;
   LPUART_STOP_BIT_Type       StopBit;
} LPUART_CFG_Type;

//******************************************************************************
// Function
//******************************************************************************

HAL_Status_Type HAL_LPUART_Init( LPUART_CFG_Type* LPUART_Config );
HAL_Status_Type HAL_LPUART_DeInit( void );

HAL_Status_Type HAL_LPUART_ConfigStructInit( LPUART_CFG_Type* LPUART_Config );
HAL_Status_Type HAL_LPUART_ConfigInterrupt( LPUART_INT_Type LPUART_IntCfg, FunctionalState NewState );
HAL_Status_Type HAL_LPUART_DataControlConfig( LPUART_CONTROL_Type Mode, FunctionalState NewState );
HAL_Status_Type HAL_LPUART_Enable( FunctionalState state );
HAL_Status_Type HAL_LPUART_ClearStatus( LPUART_STATUS_Type Status );
uint8_t HAL_LPUART_GetStatus( void );

HAL_Status_Type HAL_LPUART_TransmitByte( uint8_t Data );
uint8_t HAL_LPUART_ReceiveByte( void );
uint32_t HAL_LPUART_Transmit( uint8_t* txbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag );
uint32_t HAL_LPUART_Receive( uint8_t* rxbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag );

#ifdef __cplusplus
}
#endif

#endif   /* _LPUART_H_ */


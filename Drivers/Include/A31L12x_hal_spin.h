/**
 *******************************************************************************
 * @file        A31L12x_hal_spin.h
 * @author      ABOV R&D Division
 * @brief       SPIn Header File
 * 
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#ifndef _SPIn_H_
#define _SPIn_H_

#include "A31L12x.h"
#include "A31L12x_hal_aa_types.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * SPIn time-out definitions in case of using Read() and Write function
 * with Blocking Flag mode
 *//*-------------------------------------------------------------------------*/
#define SPIn_BLOCKING_TIMEOUT    0xffffuL    // (0xFFFFFFFFuL)

// SPIn Operation Control
#define SPInEN_Disable        (SPIn_CR_SPInEN_Disable << SPIn_CR_SPInEN_Pos)
#define SPInEN_Enable         (SPIn_CR_SPInEN_Enable  << SPIn_CR_SPInEN_Pos)

// Data Transmission sequence selection Control
#define FLSBn_msbFirst        (SPIn_CR_FLSBn_msbFirst << SPIn_CR_FLSBn_Pos)
#define FLSBn_lsbFirst        (SPIn_CR_FLSBn_lsbFirst << SPIn_CR_FLSBn_Pos)

// Master/Slave Selection Control
#define SPInMS_Slave          (SPIn_CR_SPInMS_Slave  << SPIn_CR_SPInMS_Pos)
#define SPInMS_Master         (SPIn_CR_SPInMS_Master << SPIn_CR_SPInMS_Pos)

// Clock Polarity Selection Control
#define CPOLn_IdleLow         (SPIn_CR_CPOLn_IdleLow  << SPIn_CR_CPOLn_Pos)
#define CPOLn_IdleHigh        (SPIn_CR_CPOLn_IdleHigh << SPIn_CR_CPOLn_Pos)

// Clock Phase Selection Control
#define CPHAn_StartIdle       (SPIn_CR_CPHAn_StartIdle     << SPIn_CR_CPHAn_Pos)
#define CPHAn_StartInverted   (SPIn_CR_CPHAn_StartInverted << SPIn_CR_CPHAn_Pos)

//******************************************************************************
// Type
//******************************************************************************

//==============================================================================
// Enumeration
//==============================================================================

typedef enum
{
   SPIn_MSB_FIRST = 0,              /**<  SPI MSB First */
   SPIn_LSB_FIRST,                  /**<  SPI LSB First */
} SPIn_SPI_ORDER_Type;

typedef enum
{
   SPIn_TX_RISING = 0,              /**<  Txd Change : Rising / Rxd Change : Falling */
   SPIn_TX_FALLING,                 /**<  Txd Change : Falling / Rxd Change : Rising */
} SPIn_ACK_Type;

typedef enum
{
   SPIn_TX_LEADEDGE_SAMPLE = 0,     /**<  Leading edge : Sample / Trailing edge : Setup */
   SPIn_TX_LEADEDGE_SETUP,          /**<  Leading edge : Setup / Trailing edge : Sample */
} SPIn_EDGE_Type;

typedef enum
{
   SPIn_CONTROL_SPInMS = 0,
   SPIn_CONTROL_FXCHn,
   SPIn_CONTROL_SSnEN,
} SPIn_CONTROL_Type;

typedef enum
{
   SPIn_STATUS_SPInIFLAG = 0,
} SPIn_STATUS_Type;

typedef enum
{
   SPIn_INTCFG_SPInIEN = 0,
} SPIn_INT_Type;

//==============================================================================
// Structure
//==============================================================================

typedef struct
{
   uint32_t                Baudrate;
   SPIn_SPI_ORDER_Type     Order;
   SPIn_ACK_Type           ACK;           // CPOLn
   SPIn_EDGE_Type          Edge;          // CPHAn
} SPIn_CFG_Type;

//******************************************************************************
// Function
//******************************************************************************

HAL_Status_Type HAL_SPIn_Init( SPIn_Type* SPIx, SPIn_CFG_Type* SPIn_Config );
HAL_Status_Type SPIn_DeInit( SPIn_Type* SPIx );

HAL_Status_Type SPIn_ConfigStructInit( SPIn_CFG_Type* SPIn_Config );
HAL_Status_Type SPIn_ConfigInterrupt( SPIn_Type* SPIx, SPIn_INT_Type SPIn_IntCfg, FunctionalState NewState );
HAL_Status_Type SPIn_DataControlConfig( SPIn_Type* SPIx, SPIn_CONTROL_Type Mode, FunctionalState NewState );
HAL_Status_Type SPIn_Enable( SPIn_Type* SPIx, FunctionalState state );
HAL_Status_Type SPIn_ClearStatus( SPIn_Type* SPIx, SPIn_STATUS_Type Status );
uint8_t SPIn_GetStatus( SPIn_Type* SPIx );

HAL_Status_Type SPIn_SendByte( SPIn_Type* SPIx, uint8_t Data );
uint8_t SPIn_ReceiveByte( SPIn_Type* SPIx );
uint32_t SPIn_Send( SPIn_Type* SPIx, uint8_t* txbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag );
uint32_t SPIn_Receive( SPIn_Type* SPIx, uint8_t* rxbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag );

#ifdef __cplusplus
}
#endif

#endif   /* _SPIn_H_ */


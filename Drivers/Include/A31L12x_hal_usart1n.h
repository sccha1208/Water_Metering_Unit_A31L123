/**
 *******************************************************************************
 * @file        A31L12x_hal_usart1n.h
 * @author      ABOV R&D Division
 * @brief       UART1n Header File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#ifndef _USART1n_H_
#define _USART1n_H_

#include "A31L12x.h"
#include "A31L12x_hal_aa_types.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * USART1n time-out definitions in case of using Read() and Write function
 * with Blocking Flag mode
 *//*-------------------------------------------------------------------------*/
#define USART1n_BLOCKING_TIMEOUT    0xffffuL    // (0xFFFFFFFFuL)

//--------------------------------------
// Macro defines for USART1n interrupt enable register
//--------------------------------------
/*
#define USART1n_IER_WAKEINT_EN   ((uint16_t)(1 << 2))    // WAKE Interrupt enable
#define USART1n_IER_RXCINT_EN    ((uint16_t)(1 << 3))    // RXC Interrupt enable
#define USART1n_IER_TXCINT_EN    ((uint16_t)(1 << 4))    // TXC interrupt enable
#define USART1n_IER_DR_EN        ((uint16_t)(1 << 5))    // DR interrupt enable
#define USART1n_IER_BITMASK      ((uint16_t)(0x3C))      // USART interrupt enable register bit mask
*/

//--------------------------------------
// Macro defines for USART1n interrupt status register
//--------------------------------------
#define USART1n_SR_DRE           ((uint16_t)(1 << 7))    /**<  Interrupt identification: Tx Buffer Busy */
#define USART1n_SR_TXC           ((uint16_t)(1 << 6))    /**<  Interrupt identification: Tx Complete */
#define USART1n_SR_RXC           ((uint16_t)(1 << 5))    /**<  Interrupt identification: Rx Complete */
#define USART1n_SR_WAKE          ((uint16_t)(1 << 4))    /**<  Interrupt identification: Wake */
#define USART1n_SR_DOR           ((uint16_t)(1 << 2))    /**<  Interrupt identification: Data OverRun */
#define USART1n_SR_FE            ((uint16_t)(1 << 1))    /**<  Interrupt identification: Frame Error */
#define USART1n_SR_PE            ((uint16_t)(1 << 0))    /**<  Interrupt identification: Parity Error */
#define USART1n_SR_BITMASK       ((uint16_t)(0xFF))      /**<  USART interrupt identification register bit mask */

#define USART1n_CR2_USTnRX8      ((uint16_t)(1 << 0))    /**<  */
#define USART1n_CR2_USTnTX8      ((uint16_t)(1 << 1))    /**<  */
#define USART1n_CR2_USTnSB       ((uint16_t)(1 << 2))    /**<  */
#define USART1n_CR2_FXCHn        ((uint16_t)(1 << 3))    /**<  */
#define USART1n_CR2_USTnSSEN     ((uint16_t)(1 << 4))    /**<  */
#define USART1n_CR2_DISSCKn      ((uint16_t)(1 << 5))    /**<  */
#define USART1n_CR2_LOOPSn       ((uint16_t)(1 << 6))    /**<  */
#define USART1n_CR2_MASTERn      ((uint16_t)(1 << 7))    /**<  */
#define USART1n_CR2_DBLSn        ((uint16_t)(1 << 8))    /**<  */
#define USART1n_CR2_USTnEN       ((uint16_t)(1 << 9))    /**<  */
#define USART1n_CR2_BITMASK      ((uint16_t)(0x3FF))     /**<  */

//******************************************************************************
// Type
//******************************************************************************

//==============================================================================
// Enumeration
//==============================================================================

typedef enum
{
   USART1n_UART_MODE = 0,     /**<  UART Mode */
   USART1n_USRT_MODE,         /**<  USRT Mode (Syncronous) */
   USART1n_SPI_MODE = 3       /**<  SPI Mode */
} USART1n_OPMODE_Type;

typedef enum
{
   USART1n_SPI_LSB = 0,          /**<  SPI LSB First */
   USART1n_SPI_MSB,              /**<  SPI MSB First */
} USART1n_SPI_ORDER_Type;

typedef enum
{
   USART1n_SPI_TX_RISING = 0,    /**<  Txd Change : Rising / Rxd Change : Falling */
   USART1n_SPI_TX_FALLING,       /**<  Txd Change : Falling / Rxd Change : Rising */
} USART1n_ACK_Type;

typedef enum
{
   USART1n_SPI_TX_LEADEDGE_SAMPLE = 0,    /**<  Leading edge : Sample / Trailing edge : Setup */
   USART1n_SPI_TX_LEADEDGE_SETUP,         /**<  Leading edge : Setup / Trailing edge : Sample */
} USART1n_EDGE_Type;

/**   USART1n Data Bit type definitions */
typedef enum
{
   USART1n_DATA_BIT_5 = 0,       /**<  5 Data Bits */
   USART1n_DATA_BIT_6,           /**<  6 Data Bits */
   USART1n_DATA_BIT_7,           /**<  7 Data Bits */
   USART1n_DATA_BIT_8,           /**<  8 Data Bits */
   USART1n_DATA_BIT_9 = 7        /**<  9 Data Bits */
} USART1n_DATA_BIT_Type;

/**   USART1n Stop Bit type definitions */
typedef enum
{
   USART1n_STOP_BIT_1 = 0,       /**<  1 Stop Bits */
   USART1n_STOP_BIT_2            /**<  2 Stop Bits */
} USART1n_STOP_BIT_Type;

/**   USART1n Parity Bit type definitions */
typedef enum
{
   USART1n_PARITY_BIT_NONE = 0,     /**<  No parity */
   USART1n_PARITY_BIT_EVEN = 2,     /**<  Even parity */
   USART1n_PARITY_BIT_ODD = 3       /**<  Odd parity */
} USART1n_PARITY_BIT_Type;

/**   USART Data Control type definition */
typedef enum
{
   USART1n_CONTROL_USTRX8 = 0,
   USART1n_CONTROL_USTTX8,
   USART1n_CONTROL_USTSB,
   USART1n_CONTROL_FXCH,
   USART1n_CONTROL_USTSSEN,
   USART1n_CONTROL_DISSCK,
   USART1n_CONTROL_LOOPS,
   USART1n_CONTROL_MASTER,
   USART1n_CONTROL_DBLS,
   USART1n_CONTROL_USTEN,

   USART1n_CONTROL_RCDEN,
   USART1n_CONTROL_RTOEN
} USART1n_CONTROL_Type;

typedef enum
{
   USART1n_STATUS_PE = 0,
   USART1n_STATUS_FE,
   USART1n_STATUS_DOR,
   USART1n_STATUS_WAKE,
   USART1n_STATUS_RXC,
   USART1n_STATUS_TXC,
   USART1n_STATUS_DRE,

   USART1n_STATUS_RCD,
   USART1n_STATUS_RTO
} USART1n_STATUS_Type;

typedef enum
{
   USART1n_INTCFG_WAKE = 0,
   USART1n_INTCFG_RXC,
   USART1n_INTCFG_TXC,
   USART1n_INTCFG_DR,

   USART1n_INTCFG_RCD,
   USART1n_INTCFG_RTO
} USART1n_INT_Type;

//==============================================================================
// Structure
//==============================================================================

typedef struct
{
   uint32_t                   Baudrate;
   USART1n_OPMODE_Type        Mode;
   USART1n_SPI_ORDER_Type     Order;
   USART1n_ACK_Type           ACK;
   USART1n_EDGE_Type          Edge;
   USART1n_DATA_BIT_Type      Databits;      /**<  Number of Data Bits, should be:
                                                   -  USART_DATABIT_5:  5 Data Bits
                                                   -  USART_DATABIT_6:  6 Data Bits
                                                   -  USART_DATABIT_7:  7 Data Bits
                                                   -  USART_DATABIT_8:  8 Data Bits
                                                   -  USART_DATABIT_9:  9 Data Bits
                                             */
   USART1n_PARITY_BIT_Type    Parity;        /**<  Number of Parity Bits, should be:
                                                   -  USART_PARITY_NONE:   No Parity
                                                   -  USART_PARITY_ODD:    Odd Parity
                                                   -  USART_PARITY_EVEN:   Even Parity
                                                   -  USART_PARITY_SP_1:   Forced "1" Stick Parity
                                                   -  USART_PARITY_SP_0:   Forced "0" Stick Parity
                                             */
   USART1n_STOP_BIT_Type      Stopbits;      /**<  Number of Stop Bits, should be:
                                                   -  USART_STOPBIT_1:  1 Stop Bits
                                                   -  USART_STOPBIT_2:  2 Stop Bits
                                             */
} USART1n_CFG_Type;

//******************************************************************************
// Function
//******************************************************************************

HAL_Status_Type HAL_USART_Init( USART1n_Type* USART1x, USART1n_CFG_Type* USART1n_Config );
HAL_Status_Type HAL_USART_DeInit( USART1n_Type* USART1x );

HAL_Status_Type HAL_USART_UART_Mode_Config( USART1n_CFG_Type* USART1n_Config );
HAL_Status_Type HAL_USART_USRT_Mode_Config( USART1n_CFG_Type* USART1n_Config );
HAL_Status_Type HAL_USART_SPI_Mode_Config( USART1n_CFG_Type* USART1n_Config );
HAL_Status_Type HAL_USART_ConfigInterrupt( USART1n_Type* USART1x, USART1n_INT_Type USART1n_IntCfg, FunctionalState NewState );
HAL_Status_Type HAL_USART_DataControlConfig( USART1n_Type* USART1x, USART1n_CONTROL_Type Mode, FunctionalState NewState );
HAL_Status_Type HAL_USART_Enable( USART1n_Type* USART1x, FunctionalState state );
HAL_Status_Type HAL_USART_ClearStatus( USART1n_Type* USART1x, USART1n_STATUS_Type Status );
uint8_t HAL_USART_GetStatus( USART1n_Type* USART1x );
uint8_t HAL_USART_GetStatusRTORCD( USART1n_Type* USART1x );
FlagStatus HAL_USART_CheckBusy( USART1n_Type* USART1x );

HAL_Status_Type HAL_USART_TransmitByte( USART1n_Type* USART1x, uint8_t Data );
uint8_t HAL_USART_ReceiveByte( USART1n_Type* USART1x );
uint32_t HAL_USART_Transmit( USART1n_Type* USART1x, uint8_t* txbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag );
uint32_t HAL_USART_Receive( USART1n_Type* USART1x, uint8_t* rxbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag );

#ifdef __cplusplus
}
#endif

#endif   /* _USART1n_H_ */


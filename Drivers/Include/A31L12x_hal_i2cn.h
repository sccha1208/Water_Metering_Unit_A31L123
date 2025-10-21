/**
 *******************************************************************************
 * @file        A31L12x_hal_i2cn.h
 * @author      ABOV R&D Division
 * @brief       I2Cn Header File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#ifndef _I2Cn_H_
#define _I2Cn_H_

#include "A31L12x.h"
#include "A31L12x_hal_aa_types.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

/**   I2C Slave Address registers bit mask */
#define I2Cn_SLA_BITMASK      (0xff)

// I2C state handle return values
#define RECEIVE_MODE          1
#define TRANS_MODE            2
#define RECEIVE_DATA          3
#define TRANS_DATA            4
#define LOST_BUS              5
#define STOP_DECT             6

//******************************************************************************
// Type
//******************************************************************************

//==============================================================================
// Enumeration
//==============================================================================

/**   Transfer option type definitions */
typedef enum
{
   I2Cn_TRANSFER_POLLING = 0,    /**<  Transfer in polling mode */
   I2Cn_TRANSFER_INTERRUPT       /**<  Transfer in interrupt mode */
} I2Cn_TRANSFER_OPT_Type;

//==============================================================================
// Structure
//==============================================================================

/**   Master transfer setup data structure definitions */
typedef struct
{
   uint32_t          sl_addr7bit;   /**<  Slave address in 7bit mode */
   uint8_t*          tx_data;       /**<  Pointer to Transmit data - NULL if data transmit is not used */
   uint32_t          tx_length;     /**<  Transmit data length - 0 if data transmit is not used */
   uint32_t          tx_count;      /**<  Current Transmit data counter */
   uint8_t*          rx_data;       /**<  Pointer to Receive data - NULL if data receive is not used */
   uint32_t          rx_length;     /**<  Receive data length - 0 if data receive is not used */
   uint32_t          rx_count;      /**<  Current Receive data counter */
} I2Cn_M_SETUP_Type;

/**   Slave transfer setup data structure definitions */
typedef struct
{
   uint8_t*          tx_data;       /**<  Pointer to transmit data - NULL if data transmit is not used */
   uint32_t          tx_length;     /**<  Transmit data length - 0 if data transmit is not used */
   uint32_t          tx_count;      /**<  Current transmit data counter */
   uint8_t*          rx_data;       /**<  Pointer to receive data - NULL if data received is not used */
   uint32_t          rx_length;     /**<  Receive data length - 0 if data receive is not used */
   uint32_t          rx_count;      /**<  Current receive data counter */
} I2Cn_S_SETUP_Type;

//******************************************************************************
// Function
//******************************************************************************

HAL_Status_Type HAL_I2C_Init( I2Cn_Type* I2Cx, uint32_t clockrate );
HAL_Status_Type HAL_I2C_DeInit( I2Cn_Type* I2Cx );

HAL_Status_Type HAL_I2C_ConfigInterrupt( I2Cn_Type* I2Cx, Bool NewState );
HAL_Status_Type HAL_I2C_Slave_SetAddress1( I2Cn_Type* I2Cx, uint8_t SlaveAddr_7bit, uint8_t GeneralCallState );
HAL_Status_Type HAL_I2C_Slave_SetAddress2( I2Cn_Type* I2Cx, uint8_t SlaveAddr_7bit, uint8_t GeneralCallState );
uint32_t HAL_I2C_Master_GetState( I2Cn_Type* I2Cx );
uint32_t HAL_I2C_Slave_GetState( I2Cn_Type* I2Cx );

HAL_Status_Type HAL_I2C_Master_IRQHandler_IT( I2Cn_Type*  I2Cx );
HAL_Status_Type HAL_I2C_Slave_IRQHandler_IT( I2Cn_Type*  I2Cx );

Status HAL_I2C_MasterTransferData( I2Cn_Type* I2Cx, I2Cn_M_SETUP_Type* TransferCfg, I2Cn_TRANSFER_OPT_Type Opt );
Status HAL_I2C_SlaveTransferData( I2Cn_Type* I2Cx, I2Cn_S_SETUP_Type* TransferCfg, I2Cn_TRANSFER_OPT_Type Opt );

Status HAL_I2C_Master_Transmit( I2Cn_Type* I2Cx, I2Cn_M_SETUP_Type* TransferCfg, I2Cn_TRANSFER_OPT_Type Opt );
Status HAL_I2C_Master_Receive( I2Cn_Type* I2Cx, I2Cn_M_SETUP_Type* TransferCfg, I2Cn_TRANSFER_OPT_Type Opt );
Status HAL_I2C_Slave_Receive( I2Cn_Type* I2Cx, I2Cn_S_SETUP_Type* TransferCfg, I2Cn_TRANSFER_OPT_Type Opt );

Status HAL_I2C_MasterTransferData_DMA( I2Cn_Type* I2Cx, I2Cn_M_SETUP_Type* TransferCfg, I2Cn_TRANSFER_OPT_Type Opt, DMACn_Type* TxDMACx, DMACn_Type* RxDMACx );
Status HAL_I2C_SlaveTransferData_DMA( I2Cn_Type* I2Cx, I2Cn_S_SETUP_Type* TransferCfg, I2Cn_TRANSFER_OPT_Type Opt, DMACn_Type* TxDMACx, DMACn_Type* RxDMACx );

Status HAL_I2C_MasterTransmit_DMA( I2Cn_Type* I2Cx, I2Cn_M_SETUP_Type* TransferCfg, I2Cn_TRANSFER_OPT_Type Opt, DMACn_Type* TxDMACx, DMACn_Type* RxDMACx );
Status HAL_I2C_SlaveReceive_DMA( I2Cn_Type* I2Cx, I2Cn_S_SETUP_Type* TransferCfg, I2Cn_TRANSFER_OPT_Type Opt, DMACn_Type* TxDMACx, DMACn_Type* RxDMACx );

#ifdef __cplusplus
}
#endif

#endif   /* _I2Cn_H_ */


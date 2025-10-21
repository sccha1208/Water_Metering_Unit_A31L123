/**
 *******************************************************************************
 * @file        A31L12x_hal_scn.h
 * @author      ABOV R&D Division
 * @brief       SCn Header File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/

#ifndef _SCn_H_
#define _SCn_H_

#include "A31L12x.h"
#include "A31L12x_hal_aa_types.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

//------------ Convention Type Data Definition -------------
#define DIRECT_CONV_DATA        0x3B
#define INVERSE_CONV_DATA       0x3F

//----------   SCn Mask Definition   ----------------
#define SCn_CR1_CONTROL_BITMASK 0x0000ffffuL
#define SCn_CR2_CONTROL_BITMASK 0x0000ffffuL
#define SCn_CR3_CONTROL_BITMASK 0x00ffffffuL

/*-------------------------------------------------------------------------*//**
 * SCn time-out definitions in case of using Read() and Write function
 * with Blocking Flag mode
 *//*-------------------------------------------------------------------------*/
#define SCn_BLOCKING_TIMEOUT    0xffffuL    // (0xFFFFFFFFuL)

//--------------------------------------
// Macro defines for SCn interrupt status register
//--------------------------------------

#define SCn_IFSR_RSTA          ((uint32_t)(1 << 23))         /**<  Interrupt identification: Reset Assertion */
#define SCn_IFSR_SER           ((uint32_t)(1 << 22))         /**<  Interrupt identification: Sequence Error */
#define SCn_IFSR_SED           ((uint32_t)(1 << 21))         /**<  Interrupt identification: Sequence End */
#define SCn_IFSR_CONER         ((uint32_t)(1 << 20))         /**<  Interrupt identification: Convention Detection Error */
#define SCn_IFSR_CONED         ((uint32_t)(1 << 19))         /**<  Interrupt identification: Convention Detection End */
#define SCn_IFSR_TRYER         ((uint32_t)(1 << 18))         /**<  Interrupt identification: Tx Retry Error */
#define SCn_IFSR_SCIN          ((uint32_t)(1 << 17))         /**<  Interrupt identification: SCnIN Pin Valid Edge */
#define SCn_IFSR_BLED          ((uint32_t)(1 << 16))         /**<  Interrupt identification: Block Length Count End */
#define SCn_IFSR_DOR           ((uint32_t)(1 << 15))         /**<  Interrupt identification: Data OverRun */
#define SCn_IFSR_FE            ((uint32_t)(1 << 14))         /**<  Interrupt identification: Frame Error */
#define SCn_IFSR_PE            ((uint32_t)(1 << 13))         /**<  Interrupt identification: Parity Error */
#define SCn_IFSR_RXBUSY        ((uint32_t)(1 << 12))         /**<  Interrupt identification: Rx Line Busy */
#define SCn_IFSR_RTO           ((uint32_t)(1 << 6))          /**<  Interrupt identification: Rx Time Out */
#define SCn_IFSR_TXC           ((uint32_t)(1 << 2))          /**<  Interrupt identification: Tx Complete */
#define SCn_IFSR_RXC           ((uint32_t)(1 << 0))          /**<  Interrupt identification: Rx Complete */
#define SCn_IFSR_BITMASK       ((uint32_t)(0x00FFFFFF))      /**<  SCn interrupt identification register bit mask */

//******************************************************************************
// Type
//******************************************************************************

//==============================================================================
// Enumeration
//==============================================================================

typedef enum
{
   SCn_SCI_MODE = 0,                                  /**<  Smart Card Interface Mode */
   SCn_UART_MODE                                      /**<  UART Mode */
} SCn_OPMODE_Type;

/**   SCn Parity Enable/Disable definition */
typedef enum
{
   SCn_PARITY_DISABLE = 0,                            /**<  Disable parity */
   SCn_PARITY_ENABLE                                  /**<  Enable parity */
} SCn_PARITY_EN_DISABLE;

/**   SCn Parity Bit type definition */
typedef enum
{
   SCn_PARITY_BIT_ODD = 0,                            /**<  Odd parity */
   SCn_PARITY_BIT_EVEN                                /**<  Even parity */
} SCn_PARITY_BIT_Type;

/**   SCn Data Bit type definitions */
typedef enum
{
   SCn_DATA_BIT_5 = 0,                                /**<  5 Data Bits */
   SCn_DATA_BIT_6,                                    /**<  6 Data Bits */
   SCn_DATA_BIT_7,                                    /**<  7 Data Bits */
   SCn_DATA_BIT_8                                     /**<  8 Data Bits */
} SCn_DATA_BIT_Type;

/**   SCn Stop Bit type definitions */
typedef enum
{
   SCn_STOP_BIT_1 = 0,                                /**<  1 Stop Bits */
   SCn_STOP_BIT_2                                     /**<  2 Stop Bits */
} SCn_STOP_BIT_Type;

/**   SCn Oversampling Type definition */
typedef enum
{
   SCn_OVR_SAMP_16 = 0,                               /**<  16 Oversampling */
   SCn_OVR_SAMP_8                                     /**<  8 Oversampling */
} SCn_OVR_SAMP_Type;

/**   SCn Control type definition */
typedef enum
{
   SCn_CR1_SCIEN = 0,
   SCn_CR1_RTOEN,
   SCn_CR1_RXE,
   SCn_CR1_TXE,
   SCn_CR1_PEN
} SCn_CR1_CONTROL_Type;

/**   SCn Smartcard Interface Mode Pin Level definition */
typedef enum
{
   SCn_CR2_CLKEN = 0,
   SCn_CR2_CLKLV,
   SCn_CR2_DATALV,
   SCn_CR2_RSTLV,
   SCn_CR2_PWRLV
} SCn_CR2_LEVEL_POS;

/**   SCn Convention Type definition */
typedef enum
{
   SCn_DIRECT_CONV = 0,                               /**<  Direct Convention */
   SCn_INVERSE_CONV                                   /**<  Inverse Convention */
} SCn_CONV_Type;

/**   SCn Retry Enable definition */
typedef enum
{
   SCn_RETRY_DISABLE = 0,                             /**<  Retry Disable */
   SCn_RETRY_ENABLE                                   /**<  Retry Enable */
} SCn_RETRY_EN_DISABLE;

/**   SCn Delay Time After Retry definition */
typedef enum
{
   SCn_DTIME_2J5ETU = 0,                              /**<  Delay Time: 2.5etu Before Retry */
   SCn_DTIME_2J5PEXT                                  /**<  Delay Time: 2.5etu + Extra Time Before Retry */
} SCn_DTIME_Type;

/**   SCn Delay Time After Retry definition */
typedef enum
{
   SCn_INPUT_POLARITY_FALLING = 0,                    /**<  Insert/Removal on falling edge */
   SCn_INPUT_POLARITY_RISING,                         /**<  Insert/Removal on rising edge */
   SCn_INPUT_POLARITY_BOTH                            /**<  Insert/Removal on both of falling and rising edge */
} SCn_INPUT_POLARITY_Type;

/**   SCn Received Byte Count Enable definition */
typedef enum
{
   SCn_RXCNT_DISABLE = 0,                             /**<  Received Byte Count Disable */
   SCn_RXCNT_ENABLE                                   /**<  Received Byte Count Enable */
} SCn_RXCNT_EN_DISABLE;

/**   SCn Status definition */
typedef enum
{
   SCn_STATUS_RXC = 0,
   SCn_STATUS_TXC = 2,
   SCn_STATUS_RTO = 6,
   SCn_STATUS_RXBUSY = 12,
   SCn_STATUS_PE = 13,
   SCn_STATUS_FE = 14,
   SCn_STATUS_DOR = 15,
   SCn_STATUS_BLED = 16,
   SCn_STATUS_SCIN = 17,
   SCn_STATUS_TRYER = 18,
   SCn_STATUS_CONED = 19,
   SCn_STATUS_CONER = 20,
   SCn_STATUS_SED = 21,
   SCn_STATUS_SER = 22,
   SCn_STATUS_RSTA = 23,
   SCn_STATUS_ALL_EXCEPT_TXC = ( 0x00ffffff & ~SCn_IFSR_TXCIFLAGn_Msk ), /**< except for TXC flag */
   SCn_STATUS_ALL = 0x00ffffff                          /**< All status */
} SCn_STATUS_Type;

/**   SCn Interrupt definition */
typedef enum
{
   SCn_INTCFG_RXC = 0,
   SCn_INTCFG_TXC = 2,
   SCn_INTCFG_RTO = 6,
   SCn_INTCFG_BLED = 16,
   SCn_INTCFG_SCIN = 17,
   SCn_INTCFG_TRYER = 18,
   SCn_INTCFG_CONED = 19,
   SCn_INTCFG_CONER = 20,
   SCn_INTCFG_SED = 21,
   SCn_INTCFG_SER = 22,
   SCn_INTCFG_RSTA = 23
} SCn_INT_Type;

//==============================================================================
// Structure
//==============================================================================

typedef struct
{
   uint32_t               Baudrate;                  /**<  Baud Rate */
   uint32_t               Baud_rate_Compensation;     /**<  Baud Rate Compensation */
   uint32_t               SCI_clock_gen;              /**<  Smartcard clock divider */
   uint32_t               Extra_guard_time;           /**<  Extra Guard Time to transmit */
   uint32_t               T3_dur_time;                /**<  T3 Duration Time between Rx enable and Reset Assertion */
   uint32_t               T4_dur_time;                /**<  T4 Duration Time between Reset Assertion and ATR receive */

   SCn_OPMODE_Type        Mode;
   SCn_PARITY_EN_DISABLE  ParityEnDis;                /**<  Parity Enable or Disable, should be:
                                                                -  SCn_PARITY_DISABLE:  Disable Parity
                                                                -  SCn_PARITY_ENABLE:   Enable Parity
                                                      */
   SCn_PARITY_BIT_Type    Parity;                     /**<  Number of Parity Bits, should be:
                                                                -  SCn_PARITY_BIT_ODD:   Odd Parity
                                                                -  SCn_PARITY_BIT_EVEN:  Even Parity
                                                      */
   SCn_DATA_BIT_Type      Databits;                   /**<  Number of Data Bits, should be:
                                                                -  SCn_DATA_BIT_5:  5 Data Bits
                                                                -  SCn_DATA_BIT_6:  6 Data Bits
                                                                -  SCn_DATA_BIT_7:  7 Data Bits
                                                                -  SCn_DATA_BIT_8:  8 Data Bits
                                                      */
   SCn_STOP_BIT_Type      Stopbits;                   /**<  Number of Stop Bits, should be:
                                                                -  SCn_STOP_BIT_1:  1 Stop Bits
                                                                -  SCn_STOP_BIT_2:  2 Stop Bits
                                                      */
   SCn_OVR_SAMP_Type      Oversampling;               /**< Oversampling bits, should be:
                                                                -  SCn_OVR_SAMP_16: 16 oversampling
                                                                -  SCn_OVR_SAMP_8:  8 oversampling
                                                      */
} SCn_CFG_Type;

//******************************************************************************
// Function
//******************************************************************************

HAL_Status_Type HAL_SC_Init( SCn_Type* SCx, SCn_CFG_Type* SCn_Config );
HAL_Status_Type HAL_SC_DeInit( SCn_Type* SCx );

HAL_Status_Type HAL_SC_ConfigStructInit( SCn_CFG_Type* SCn_Config, SCn_OPMODE_Type mode, uint32_t Baudrate );
HAL_Status_Type HAL_SC_ConfigInterrupt( SCn_Type* SCx, SCn_INT_Type SCn_IntCfg, FunctionalState NewState );
HAL_Status_Type HAL_SC_ControlConfig( SCn_Type* SCx, SCn_CR1_CONTROL_Type Mode, FunctionalState NewState );
HAL_Status_Type HAL_SC_Enable( SCn_Type* SCx, FunctionalState state );
HAL_Status_Type HAL_SC_AutoAct( SCn_Type* SCx );
HAL_Status_Type HAL_SC_AutoWarmRst( SCn_Type* SCx );
HAL_Status_Type HAL_SC_AutoDeAct( SCn_Type* SCx );
HAL_Status_Type HAL_SC_SetRxTimeOutData( SCn_Type* SCx, uint32_t RxTimeOutD );
HAL_Status_Type HAL_SC_SetExGuardTime( SCn_Type* SCx, uint32_t TxExGuardTime );
HAL_Status_Type HAL_SC_SetT34DR( SCn_Type* SCx, uint32_t SCnT3data, uint32_t SCnT4data );
HAL_Status_Type HAL_SC_SetLevel( SCn_Type* SCx, SCn_CR2_LEVEL_POS SelectPin, SetState SetLevel );
HAL_Status_Type HAL_SC_AutoConvDet( SCn_Type* SCx );
HAL_Status_Type HAL_SC_SetConv( SCn_Type* SCx, SCn_CONV_Type conv_sel );
HAL_Status_Type HAL_SC_ConfigRetry( SCn_Type* SCx, uint8_t retry_num, SCn_DTIME_Type retry_dly );
HAL_Status_Type HAL_SC_RetryEnable( SCn_Type* SCx, SCn_RETRY_EN_DISABLE retry_en );
HAL_Status_Type HAL_SC_SetInPol( SCn_Type* SCx, SCn_INPUT_POLARITY_Type In_pol );
HAL_Status_Type HAL_SC_RxCntEnable( SCn_Type* SCx, SCn_RXCNT_EN_DISABLE RxCnt_en );
HAL_Status_Type HAL_SC_SetRxBlkLen( SCn_Type* SCx, uint8_t blk_len );
HAL_Status_Type HAL_SC_ClearStatus( SCn_Type* SCx, SCn_STATUS_Type Status );
uint32_t HAL_SC_GetStatus( SCn_Type* SCx );
uint8_t HAL_SC_GetRxLineBusy( SCn_Type* SCx );
uint8_t HAL_SC_GetSCnINST( SCn_Type* SCx );
FlagStatus HAL_SC_CheckBusy( SCn_Type* SCx );

HAL_Status_Type HAL_SC_TransmitByte( SCn_Type* SCx, uint8_t Data );
uint8_t HAL_SC_ReceiveByte( SCn_Type* SCx );
uint32_t HAL_SC_Transmit( SCn_Type* SCx, uint8_t* txbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag );
uint32_t HAL_SC_Receive( SCn_Type* SCx, uint8_t* rxbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag );

#ifdef __cplusplus
}
#endif

#endif   /* _SCn_H_ */


/**
 *******************************************************************************
 * @file        A31L12x_hal_i2cn.c
 * @author      ABOV R&D Division
 * @brief       I2Cn communication
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

#include "A31L12x_hal_dmacn.h"
#include "A31L12x_hal_i2cn.h"
#include "A31L12x_hal_scu.h"

//******************************************************************************
// Constant
//******************************************************************************

#define I2Cn_BLOCKING_TIMEOUT    (0x000FFFFFUL)
#define I2Cn_MAX                 3

//******************************************************************************
// Type
//******************************************************************************

/**   I2Cn device configuration structure type */
typedef struct
{
   union
   {
      I2Cn_M_SETUP_Type  txrx_setup_master;      /**<  Transmission setup */
      I2Cn_S_SETUP_Type  txrx_setup_slave;       /**<  Transmission setup */
   };
   int32_t     dir;        /* Current direction phase, 0 - write, 1 - read */
} I2Cn_CFG_Type;

//******************************************************************************
// Variable
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         I2Cn driver data
 *//*-------------------------------------------------------------------------*/

static I2Cn_CFG_Type    i2cdat[I2Cn_MAX];
static Bool             I2Cn_MasterComplete[I2Cn_MAX];
static Bool             I2Cn_SlaveComplete[I2Cn_MAX];

/* Public Functions --------------------------------------------------------- */
//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Convert from I2C peripheral to number
 * @param[in]     I2Cx
 *                   Pointer to the target I2C
 *                   -  I2C0 ~ I2C1
 * @return        I2C number or error code, could be:
 *                   -  0:       I2C0
 *                   -  1:       I2C1
 *
 *                   -  (-1):    Error
 *//*-------------------------------------------------------------------------*/
int32_t I2Cn_getNum( I2Cn_Type* I2Cx )
{
#if 1    // supported
   if( I2Cx == ( I2Cn_Type* )I2C0 )
   {
      return 0;
   }
#endif

#if 1    // supported
   if( I2Cx == ( I2Cn_Type* )I2C1 )
   {
      return 1;
   }
#endif

#if 0    // not supported
   if( I2Cx == ( I2Cn_Type* )I2C2 )
   {
      return 2;
   }
#endif

   return -1;
}

/*-------------------------------------------------------------------------*//**
 * @brief         wait and return status in master mode
 * @param[in]     I2Cx
 *                   Pointer to the target I2C
 *                   -  I2C0 ~ I2C1
 * @return        Status
 *//*-------------------------------------------------------------------------*/
int32_t I2Cn_MWait( I2Cn_Type* I2Cx )
{
   uint32_t    tmp;
   int32_t     ret = 0;

   while( 1 )     // Interrupt Status Check
   {
      if( ( I2Cx->CR & I2Cn_CR_I2CnIFLAG_Msk ) != 0 )
      {
         break;
      }
   }

   tmp = I2Cx->ST;
   I2Cx->ST = 0xFF;

   switch( tmp )
   {
      // Transmitter mode
      case 0x87:
         ret = TRANS_MODE;
         break;

      // Receive mode
      case 0x85:
         ret = RECEIVE_MODE;
         break;

      // Transed Data
      case 0x47:
         ret = TRANS_DATA;
         break;

      // Received Data
      case 0x44:
      case 0x45:
         ret = RECEIVE_DATA;
         break;

      default:
         if( ( tmp ) & 0x08 )
         {
            ret = LOST_BUS;   // lost
         }
         else if( ( tmp ) & 0x20 )
         {
            ret = STOP_DECT;   // stop
         }
         else
         {
            ret = -1;
         }
         break;
   }

   return ret;
}

/*-------------------------------------------------------------------------*//**
 * @brief         wait and return status in slave mode
 * @param[in]     I2Cx
 *                   Pointer to the target I2C
 *                   -  I2C0 ~ I2C1
 * @return        Status
 *//*-------------------------------------------------------------------------*/
int32_t I2Cn_SWait( I2Cn_Type* I2Cx )
{
   uint32_t    tmp;
   int32_t     ret = 0;

   while( 1 )     // Interrupt Status Check
   {
      if( ( I2Cx->CR & I2Cn_CR_I2CnIFLAG_Msk ) != 0 )
      {
         break;
      }
   }

   tmp = I2Cx->ST;
   I2Cx->ST = 0xFF;

   switch( tmp )
   {
      // Receive mode
      case 0x15:
      case 0x95:
         ret = RECEIVE_MODE;
         break;

      // Transmitter mode
      case 0x17:
      case 0x97:
         ret = TRANS_MODE;
         break;

      // Received Data
      case 0x45:
         ret = RECEIVE_DATA;
         break;

      // Transed Data
      case 0x47:
         ret = TRANS_DATA;
         break;
      default:
         if( tmp & 0x08 )
         {
            ret = LOST_BUS;   // lost
         }
         else if( tmp & 0x20 )
         {
            ret = STOP_DECT;   // stop
         }
         else
         {
            ret = -1;
         }
         break;
   }

   return ret;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Initialize the I2Cn peripheral with the specified parameters.
 * @param[in]     I2Cx
 *                   Pointer to the target I2C
 *                   -  I2C0 ~ I2C1
 * @param[in]     clockrate
 *                   Target Clock Rate (Hz)
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_I2C_Init( I2Cn_Type* I2Cx, uint32_t clockrate )
{
   /* Check I2C handle */
   if( I2Cx == NULL )
   {
      return HAL_ERROR;
   }

#if 1    // supported
   if( I2Cx == ( I2Cn_Type* )I2C0 )
   {
      /* Set up clock for I2C0 module */
      SCUCG->PPCLKEN2_b.I2C0CLKE = 1;
      HAL_SCU_Peripheral_SetReset2( 1 << 6 );
   }
#endif

#if 1    // supported
   if( I2Cx == ( I2Cn_Type* )I2C1 )
   {
      /* Set up clock for I2C1 module */
      SCUCG->PPCLKEN2_b.I2C1CLKE = 1;
      HAL_SCU_Peripheral_SetReset2( 1 << 7 );
   }
#endif

#if 0    // not supported
   if( I2Cx == ( I2Cn_Type* )I2C2 )
   {
      /* Set up clock for I2C2 module */
      SCUCG->PPCLKEN2_b.I2C2CLKE = 1;
      HAL_SCU_Peripheral_SetReset2( 1 << 8 );
   }
#endif

   I2Cx->CR_b.I2CnEN = 1;     // I2C Block Active
   I2Cx->CR_b.I2CnIEN = 1;    // I2C Interrupt Enable

#if 0    // before bug fix
   I2Cx->SCLR = ( SystemPeriClock / clockrate - 4 ) / 8;    // freq = PCLK / ((4*SCLL+2) + (4*SCLH+2))
   I2Cx->SCHR = ( SystemPeriClock / clockrate - 4 ) / 8;    // ex) 100k = 10M / ((4*12+2) + (4*12+2)), if PCLK : 10MHz
#else    // after bug fix
   I2Cx->SCLR = ( SystemPeriClock / clockrate - 6 ) / 8;    // freq = PCLK / ((4*SCLL+3) + (4*SCLH+3))
   I2Cx->SCHR = ( SystemPeriClock / clockrate - 6 ) / 8;    // ex) 98.039k = 10M / ((4*12+3) + (4*12+3)), if PCLK : 10MHz
#endif
   I2Cx->SDHR = 1;            // default value 1

   I2Cx->CR_b.ACKnEN = 1;     // ACK Signal Enable

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Deinitialize the I2Cn peripheral registers to their default reset values.
 * @param[in]     I2Cx
 *                   Pointer to the target I2C
 *                   -  I2C0 ~ I2C1
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_I2C_DeInit( I2Cn_Type* I2Cx )
{
   /* Check I2C handle */
   if( I2Cx == NULL )
   {
      return HAL_ERROR;
   }

   /* Disable I2C control */
   I2Cx->CR = 0;     // I2C Block Disable

#if 1    // supported
   if( I2Cx == ( I2Cn_Type* )I2C0 )
   {
      /* Set up clock for I2C0 module */
      SCUCG->PPCLKEN2_b.I2C0CLKE = 0;
   }
#endif

#if 1    // supported
   if( I2Cx == ( I2Cn_Type* )I2C1 )
   {
      /* Set up clock for I2C1 module */
      SCUCG->PPCLKEN2_b.I2C1CLKE = 0;
   }
#endif

#if 0    // not supported
   if( I2Cx == ( I2Cn_Type* )I2C2 )
   {
      /* Set up clock for I2C2 module */
      SCUCG->PPCLKEN2_b.I2C2CLKE = 0;
   }
#endif

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Enable/Disable interrupt for I2C peripheral
 * @param[in]     I2Cx
 *                   Pointer to the target I2C
 *                   -  I2C0 ~ I2C1
 * @param[in]     NewState
 *                   Next State of Interrupt Operation
 *                   -  ENABLE, DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_I2C_ConfigInterrupt( I2Cn_Type* I2Cx, Bool NewState )
{
   /* Check I2C handle */
   if( I2Cx == NULL )
   {
      return HAL_ERROR;
   }

   if( NewState )
   {
#if 1    // supported
      if( I2Cx == ( I2Cn_Type* )I2C0 )
      {
         NVIC_ClearPendingIRQ( I2C0_IRQn );
         NVIC_EnableIRQ( I2C0_IRQn );
      }
#endif

#if 1    // supported
      if( I2Cx == ( I2Cn_Type* )I2C1 )
      {
         NVIC_ClearPendingIRQ( I2C1_IRQn );
         NVIC_EnableIRQ( I2C1_IRQn );
      }
#endif

#if 0    // not supported
      if( I2Cx == ( I2Cn_Type* )I2C2 )
      {
         NVIC_ClearPendingIRQ( I2C2_IRQn );
         NVIC_EnableIRQ( I2C2_IRQn );
      }
#endif
   }
   else
   {
#if 1    // supported
      if( I2Cx == ( I2Cn_Type* )I2C0 )
      {
         NVIC_DisableIRQ( I2C0_IRQn );
      }
#endif

#if 1    // supported
      if( I2Cx == ( I2Cn_Type* )I2C1 )
      {
         NVIC_DisableIRQ( I2C1_IRQn );
      }
#endif

#if 0    // not supported
      if( I2Cx == ( I2Cn_Type* )I2C2 )
      {
         NVIC_DisableIRQ( I2C2_IRQn );
      }
#endif
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set Own slave address in I2C peripheral
 *                corresponding to parameter specified in OwnSlaveAddrConfigStruct.
 * @param[in]     I2Cx
 *                   Pointer to the target I2C
 *                   -  I2C0 ~ I2C1
 * @param[in]     SlaveAddr_7bit
 *                   own slave address
 * @param[in]     GeneralCallState
 *                   -  ENABLE, DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_I2C_Slave_SetAddress1( I2Cn_Type* I2Cx, uint8_t SlaveAddr_7bit, uint8_t GeneralCallState )
{
   /* Check I2C handle */
   if( I2Cx == NULL )
   {
      return HAL_ERROR;
   }

   I2Cx->SAR1 = ( ( ( uint32_t )( SlaveAddr_7bit << 1 ) ) | ( ( GeneralCallState == ENABLE ) ? 0x01 : 0x00 ) ) & I2Cn_SLA_BITMASK;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set Own slave address in I2C peripheral
 *                corresponding to parameter specified in OwnSlaveAddrConfigStruct.
 * @param[in]     I2Cx
 *                   Pointer to the target I2C
 *                   -  I2C0 ~ I2C1
 * @param[in]     SlaveAddr_7bit
 *                   own slave address
 * @param[in]     GeneralCallState
 *                   -  ENABLE, DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_I2C_Slave_SetAddress2( I2Cn_Type* I2Cx, uint8_t SlaveAddr_7bit, uint8_t GeneralCallState )
{
   /* Check I2C handle */
   if( I2Cx == NULL )
   {
      return HAL_ERROR;
   }

   I2Cx->SAR2 = ( ( ( uint32_t )( SlaveAddr_7bit << 1 ) ) | ( ( GeneralCallState == ENABLE ) ? 0x01 : 0x00 ) ) & I2Cn_SLA_BITMASK;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Get Status of Master Transfer
 * @param[in]     I2Cx
 *                   Pointer to the target I2C
 *                   -  I2C0 ~ I2C1
 * @return        Status of Master Transfer
 *                   -  TRUE, FALSE
 *//*-------------------------------------------------------------------------*/
uint32_t HAL_I2C_Master_GetState( I2Cn_Type* I2Cx )
{
   uint32_t    retval, tmp;

   tmp = I2Cn_getNum( I2Cx );
   retval = I2Cn_MasterComplete[tmp];
   I2Cn_MasterComplete[tmp] = FALSE;

   return retval;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Get Status of Slave Transfer
 * @param[in]     I2Cx
 *                   Pointer to the target I2C
 *                   -  I2C0 ~ I2C1
 * @return        Status of Slave Transfer
 *                   -  TRUE, FALSE
 *//*-------------------------------------------------------------------------*/
uint32_t HAL_I2C_Slave_GetState( I2Cn_Type* I2Cx )
{
   uint32_t    retval, tmp;

   tmp = I2Cn_getNum( I2Cx );
   retval = I2Cn_SlaveComplete[tmp];
   I2Cn_SlaveComplete[tmp] = FALSE;

   return retval;
}

/*-------------------------------------------------------------------------*//**
 * @brief         General Master Interrupt handler for I2C peripheral
 * @param[in]     I2Cx
 *                   Pointer to the target I2C
 *                   -  I2C0 ~ I2C1
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_I2C_Master_IRQHandler_IT( I2Cn_Type* I2Cx )
{
   int32_t                 tmp;
   I2Cn_M_SETUP_Type*      txrx_setup;
   uint32_t                status;

   /* Check I2C handle */
   if( I2Cx == NULL )
   {
      return HAL_ERROR;
   }

   tmp = I2Cn_getNum( I2Cx );
   txrx_setup = ( I2Cn_M_SETUP_Type* )&i2cdat[tmp].txrx_setup_master;

   status = I2Cx->ST;

   switch( status )
   {
      case 0x87: // transmit mode - addr ACK
         if( txrx_setup->tx_count < txrx_setup->tx_length )
         {
            I2Cx->DR = txrx_setup->tx_data[txrx_setup->tx_count];
            txrx_setup->tx_count++;
         }
         else
         {
            I2Cx->CR = 0
                       | ( 1 << 7 )    // Enable I2C Block
                       | ( 1 << 5 )    // Interrupt Enable
                       | ( 1 << 1 );   // STOP
         }
         break;

      case 0x47: // transmit mode - data ACK
         if( txrx_setup->tx_count < txrx_setup->tx_length )
         {
            I2Cx->DR = txrx_setup->tx_data[txrx_setup->tx_count];
            txrx_setup->tx_count++;
         }
         else
         {
            if( txrx_setup->rx_count < txrx_setup->rx_length )
            {
               // load slave address and rw flag (SLA+RnW)
               I2Cx->DR = ( ( txrx_setup->sl_addr7bit << 1 ) | 0x01 );

               // generate start condition
               I2Cx->CR |= ( 1 << 0 );    // reSTART
            }
            else
            {
               I2Cx->CR = 0
                          | ( 1 << 7 )    // Enable I2C Block
                          | ( 1 << 5 )    // Interrupt Enable
                          | ( 1 << 1 );   // STOP
            }
         }
         break;

      case 0x85: // receive mode - addr ACK
         if( txrx_setup->rx_count < txrx_setup->rx_length )
         {
            if( ( txrx_setup->rx_length > 1 ) && ( txrx_setup->rx_count < ( txrx_setup->rx_length - 1 ) ) )
            {
               NOP();
            }
            else
            {
               I2Cx->CR_b.ACKnEN = 0;     // disable ACKEN
            }
         }
         else
         {
            I2Cx->CR = 0
                       | ( 1 << 7 )    // Enable I2C Block
                       | ( 1 << 5 )    // Interrupt Enable
                       | ( 1 << 1 );   // STOP
         }
         break;

      case 0x45: // receive mode - data ACK
         if( txrx_setup->rx_count < txrx_setup->rx_length )
         {
            txrx_setup->rx_data[txrx_setup->rx_count] = I2Cx->DR;
            txrx_setup->rx_count++;

            if( ( txrx_setup->rx_length > 1 ) && ( txrx_setup->rx_count < ( txrx_setup->rx_length - 1 ) ) )
            {
               NOP();
            }
            else
            {
               I2Cx->CR_b.ACKnEN = 0;     // disable ACKEN
            }
         }
         break;

      case 0x44: // receive mode - data NOACK
         if( txrx_setup->rx_count < txrx_setup->rx_length )
         {
            txrx_setup->rx_data[txrx_setup->rx_count] = I2Cx->DR;
            txrx_setup->rx_count++;

            I2Cx->CR = 0
                       | ( 1 << 7 )    // Enable I2C Block
                       | ( 1 << 5 )    // Interrupt Enable
                       | ( 1 << 1 );   // STOP
         }
         break;

      case 0x20: // receive mode
      case 0x22: // transmit mode - stop receive
         I2Cx->CR = 0
                    | ( 1 << 7 )    // Enable I2C Block
                    | ( 1 << 5 )    // Interrupt Enable
                    | ( 1 << 3 );   // ACK Signal Enable
         goto s_int_end;

      default:
         if( status & 0x08 )     // mastership lost
         {

         }
         break;
   }

   I2Cx->ST = 0xff;     // flag clear and SCL go to HIGH
   return HAL_OK;

s_int_end:

   I2Cx->ST = 0xff;     // flag clear and SCL go to HIGH

   // Disable interrupt
   HAL_I2C_ConfigInterrupt( I2Cx, FALSE );

   I2Cn_MasterComplete[tmp] = TRUE;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         General Slave Interrupt handler for I2C peripheral
 * @param[in]     I2Cx
 *                   Pointer to the target I2C
 *                   -  I2C0 ~ I2C1
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_I2C_Slave_IRQHandler_IT( I2Cn_Type* I2Cx )
{
   int32_t                 tmp;
   I2Cn_S_SETUP_Type*      txrx_setup;
   uint32_t                status;

   /* Check I2C handle */
   if( I2Cx == NULL )
   {
      return HAL_ERROR;
   }

   tmp = I2Cn_getNum( I2Cx );
   txrx_setup = ( I2Cn_S_SETUP_Type* )&i2cdat[tmp].txrx_setup_slave;

   status = I2Cx->ST;
   switch( status )
   {
      case 0x15: // receive mode - slave select + ACK
      case 0x45: // receive mode - data ACK
         if( ( txrx_setup->rx_count < txrx_setup->rx_length ) && ( txrx_setup->rx_data != NULL ) )
         {
            txrx_setup->rx_data[txrx_setup->rx_count] = I2Cx->DR;
            txrx_setup->rx_count++;
         }
         break;

      case 0x20: // receive mode
      case 0x22: // transmit mode - stop receive
         goto s_int_end;

      case 0x17: // transmit mode - slave select + ACK
      case 0x46: // transmit mode - data NOACK
      case 0x47: // transmit mode - data ACK
         if( ( txrx_setup->tx_count < txrx_setup->tx_length ) && ( txrx_setup->tx_data != NULL ) )
         {
            I2Cx->DR = txrx_setup->tx_data[txrx_setup->tx_count];
            txrx_setup->tx_count++;
         }
         break;
   }

   I2Cx->ST = 0xff;     // flag clear and SCL go to HIGH
   return HAL_OK;

s_int_end:

   I2Cx->ST = 0xff;     // flag clear and SCL go to HIGH

   // Disable interrupt
   HAL_I2C_ConfigInterrupt( I2Cx, FALSE );

   I2Cn_SlaveComplete[tmp] = TRUE;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Transmit and Receive data in master mode
 * @param[in]     I2Cx
 *                   Pointer to the target I2C
 *                   -  I2C0 ~ I2C1
 * @param[in]     TransferCfg
 *                   Pointer to a I2Cn_M_SETUP_Type structure
 *                   that contains specified information about the configuration for master transfer.
 * @param[in]     Opt
 *                   I2Cn_TRANSFER_OPT_Type type that selected for interrupt or polling mode.
 * @return        Result
 *                   -  SUCCESS, ERROR
 *//*-------------------------------------------------------------------------*/
Status HAL_I2C_MasterTransferData( I2Cn_Type* I2Cx, I2Cn_M_SETUP_Type* TransferCfg, I2Cn_TRANSFER_OPT_Type Opt )
{
   int32_t     tmp;
   uint32_t    exitflag;
   int32_t     Ret;

   // Reset I2C setup value to default state
   TransferCfg->tx_count = 0;
   TransferCfg->rx_count = 0;

   while( I2Cx->ST & 0x04 );     // busy check

   if( Opt == I2Cn_TRANSFER_POLLING )
   {
      // init count
      TransferCfg->tx_count = 0;
      TransferCfg->rx_count = 0;

      // tx transfer
      if( TransferCfg->tx_count < TransferCfg->tx_length )
      {
         // generate start condition
         I2Cx->DR = ( TransferCfg->sl_addr7bit << 1 );      // load slave address and write flag (SLA+RnW)
         I2Cx->CR |= ( 1 << 0 );    // START
         Ret = I2Cn_MWait( I2Cx );
         if( ( Ret != TRANS_MODE ) )
         {
            // generate stop condition
            I2Cx->CR |= ( 1 << 1 );    // STOP
            I2Cn_MWait( I2Cx );
            I2Cx->ST = 0xFF;
            I2Cx->CR = 0
                       | ( 1 << 7 )    // I2C Block Enable
                       | ( 1 << 5 )    // Interrupt Enable
                       | ( 1 << 3 );   // ACK Signal Enable

            // return
            return ERROR;
         }

         // tx data
         exitflag = 1;
         while( exitflag )
         {
            if( TransferCfg->tx_count < TransferCfg->tx_length )
            {
               // tx byte
               I2Cx->DR = TransferCfg->tx_data[TransferCfg->tx_count];
               TransferCfg->tx_count++;
               I2Cx->ST = 0xFF;
               Ret = I2Cn_MWait( I2Cx );
               if( ( Ret != TRANS_DATA ) )
               {
                  // generate stop condition
                  I2Cx->CR |= ( 1 << 1 );    // STOP
                  I2Cn_MWait( I2Cx );
                  I2Cx->ST = 0xFF;
                  I2Cx->CR = 0
                             | ( 1 << 7 )    // I2C Block Enable
                             | ( 1 << 5 )    // Interrupt Enable
                             | ( 1 << 3 );   // ACK Signal Enable

                  // return
                  return ERROR;
               }
            }
            else
            {
               if( TransferCfg->rx_count >= TransferCfg->rx_length )
               {
                  // generate stop condition
                  I2Cx->CR |= ( 1 << 1 );    // STOP
                  I2Cx->ST = 0xFF;
                  I2Cn_MWait( I2Cx );
                  I2Cx->ST = 0xFF;
                  I2Cx->CR = 0
                             | ( 1 << 7 )    // I2C Block Enable
                             | ( 1 << 5 )    // Interrupt Enable
                             | ( 1 << 3 );   // ACK Signal Enable

                  // return
                  return SUCCESS;
               }
               else
               {
                  exitflag = 0;
               }
            }
         }
      }

      // rx transfer
      if( TransferCfg->rx_count < TransferCfg->rx_length )
      {
         // generate start condition
         I2Cx->DR = ( ( TransferCfg->sl_addr7bit << 1 ) | 0x01 );    // load slave address and read flag (SLA+RnW)
         I2Cx->CR |= ( 1 << 0 );    // START
         I2Cx->ST = 0xFF;
         Ret = I2Cn_MWait( I2Cx );
         if( ( Ret != RECEIVE_MODE ) )
         {
            // generate stop condition
            I2Cx->CR |= ( 1 << 1 );    // STOP
            I2Cn_MWait( I2Cx );
            I2Cx->ST = 0xFF;
            I2Cx->CR = 0
                       | ( 1 << 7 )    // I2C Block Enable
                       | ( 1 << 5 )    // Interrupt Enable
                       | ( 1 << 3 );   // ACK Signal Enable

            // return
            return ERROR;
         }

         // rx data
         exitflag = 1;
         while( exitflag )
         {
            if( ( TransferCfg->rx_length > 1 ) && ( TransferCfg->rx_count < ( TransferCfg->rx_length - 1 ) ) )
            {
               // rx byte
               I2Cx->ST = 0xFF;
               Ret = I2Cn_MWait( I2Cx );
               if( ( Ret != RECEIVE_DATA ) )
               {
                  // generate stop condition
                  I2Cx->CR |= ( 1 << 1 );    // STOP
                  I2Cn_MWait( I2Cx );
                  I2Cx->ST = 0xFF;
                  I2Cx->CR = 0
                             | ( 1 << 7 )    // I2C Block Enable
                             | ( 1 << 5 )    // Interrupt Enable
                             | ( 1 << 3 );   // ACK Signal Enable

                  // return
                  return ERROR;
               }
            }
            else    // the next byte is the last byte, send NACK instead.
            {
               // generate nack
               I2Cx->CR &= ~( 1 << 3 );   // ACK Signal Disable

               // rx byte
               I2Cx->ST = 0xFF;
               Ret = I2Cn_MWait( I2Cx );
               if( ( Ret != RECEIVE_DATA ) )
               {
                  // generate stop condition
                  I2Cx->CR |= ( 1 << 1 );    // STOP
                  I2Cn_MWait( I2Cx );
                  I2Cx->CR = 0
                             | ( 1 << 7 )    // I2C Block Enable
                             | ( 1 << 5 )    // Interrupt Enable
                             | ( 1 << 3 );   // ACK Signal Enable

                  // return
                  return ERROR;
               }
            }
            TransferCfg->rx_data[TransferCfg->rx_count] = I2Cx->DR;
            TransferCfg->rx_count++;
            if( TransferCfg->rx_count == TransferCfg->rx_length )
            {
               exitflag = 0;
               // commented by kth  return SUCCESS;
            }
         }

         // generate stop condition
         I2Cx->CR |= ( 1 << 1 );    // STOP
         I2Cx->ST = 0xFF;
         I2Cn_MWait( I2Cx );
         I2Cx->ST = 0xFF;
         I2Cx->CR = 0
                    | ( 1 << 7 )    // I2C Block Enable
                    | ( 1 << 5 )    // Interrupt Enable
                    | ( 1 << 3 );   // ACK Signal Enable

         // return
         return SUCCESS;
      }
   }
   else if( Opt == I2Cn_TRANSFER_INTERRUPT )
   {
      // clear flag
      tmp = I2Cn_getNum( I2Cx );
      I2Cn_MasterComplete[tmp] = FALSE;

      // Setup tx_rx data, callback and interrupt handler
      i2cdat[tmp].txrx_setup_master = *TransferCfg;

      // Set direction phase, write first
      i2cdat[tmp].dir = 0;

      // enable interrupt
      HAL_I2C_ConfigInterrupt( I2Cx, TRUE );

      // generate start condition
      if( TransferCfg->tx_count < TransferCfg->tx_length )
      {
         I2Cx->DR = ( TransferCfg->sl_addr7bit << 1 );               // load slave address and write flag (SLA+RnW)
      }
      else if( TransferCfg->rx_count < TransferCfg->rx_length )
      {
         I2Cx->DR = ( ( TransferCfg->sl_addr7bit << 1 ) | 0x01 );    // load slave address and read flag (SLA+RnW)
      }
      I2Cx->CR |= ( 1 << 0 );    // START

      // return
      return SUCCESS;
   }

   // return
   return ERROR;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Receive and Transmit data in slave mode
 * @param[in]     I2Cx
 *                   Pointer to the target I2C
 *                   -  I2C0 ~ I2C1
 * @param[in]     TransferCfg
 *                   Pointer to a I2Cn_S_SETUP_Type structure
 *                   that contains specified information about the configuration for master transfer.
 * @param[in]     Opt
 *                   I2Cn_TRANSFER_OPT_Type type that selected for interrupt or polling mode.
 * @return        Result
 *                   -  SUCCESS, ERROR
 *//*-------------------------------------------------------------------------*/
Status HAL_I2C_SlaveTransferData( I2Cn_Type* I2Cx, I2Cn_S_SETUP_Type* TransferCfg, I2Cn_TRANSFER_OPT_Type Opt )
{
   int32_t     tmp;
   int32_t     Ret;

   // Reset I2C setup value to default state
   TransferCfg->tx_count = 0;
   TransferCfg->rx_count = 0;

   // Polling option
   if( Opt == I2Cn_TRANSFER_POLLING )
   {
      while( 1 )
      {
         Ret = I2Cn_SWait( I2Cx );      // Start
         switch( Ret )
         {
            case RECEIVE_MODE:
            case RECEIVE_DATA:
               if( ( TransferCfg->rx_count < TransferCfg->rx_length ) && ( TransferCfg->rx_data != NULL ) )
               {
                  TransferCfg->rx_data[TransferCfg->rx_count] = I2Cx->DR;
                  TransferCfg->rx_count++;
               }
               break;
            case TRANS_MODE:
            case TRANS_DATA:
               if( ( TransferCfg->tx_count < TransferCfg->tx_length ) && ( TransferCfg->tx_data != NULL ) )
               {
                  I2Cx->DR = TransferCfg->tx_data[TransferCfg->tx_count];
                  TransferCfg->tx_count++;
               }
               break;
            case STOP_DECT:
               goto s_end_stage;
            case 0:
               break;
            default:
               goto s_error;
         }
      }

s_end_stage:
      I2Cx->ST = 0xFF;
      return SUCCESS;

s_error:
      I2Cx->ST = 0xFF;
      return ERROR;
   }

   else if( Opt == I2Cn_TRANSFER_INTERRUPT )
   {
      tmp = I2Cn_getNum( I2Cx );
      I2Cn_SlaveComplete[tmp] = FALSE;

      // Setup tx_rx data, callback and interrupt handler
      i2cdat[tmp].txrx_setup_slave = *TransferCfg;

      // Set direction phase, read first
      i2cdat[tmp].dir = 1;

      HAL_I2C_ConfigInterrupt( I2Cx, TRUE );

      return SUCCESS;
   }

   // return
   return ERROR;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Transmit an array of bytes in Master mode
 * @param[in]     I2Cx
 *                   Pointer to the target I2C
 *                   -  I2C0 ~ I2C1
 * @param[in]     TransferCfg
 *                   Pointer to a I2Cn_M_SETUP_Type structure
 *                   that contains specified information about the configuration for master transfer.
 * @param[in]     Opt
 *                   I2Cn_TRANSFER_OPT_Type type that selected for interrupt or polling mode.
 * @return        Result
 *                   -  SUCCESS, ERROR
 *//*-------------------------------------------------------------------------*/
Status HAL_I2C_Master_Transmit( I2Cn_Type* I2Cx, I2Cn_M_SETUP_Type* TransferCfg, I2Cn_TRANSFER_OPT_Type Opt )
{
   TransferCfg->rx_data = NULL;
   TransferCfg->rx_length = 0;
   TransferCfg->tx_count = 0;
   TransferCfg->rx_count = 0;

   return HAL_I2C_MasterTransferData( I2Cx, TransferCfg, Opt );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Receive an array of bytes in Master mode
 * @param[in]     I2Cx
 *                   Pointer to the target I2C
 *                   -  I2C0 ~ I2C1
 * @param[in]     TransferCfg
 *                   Pointer to a I2Cn_M_SETUP_Type structure
 *                   that contains specified information about the configuration for master transfer.
 * @param[in]     Opt
 *                   I2Cn_TRANSFER_OPT_Type type that selected for interrupt or polling mode.
 * @return        Result
 *                   -  SUCCESS, ERROR
 *//*-------------------------------------------------------------------------*/
Status HAL_I2C_Master_Receive( I2Cn_Type* I2Cx, I2Cn_M_SETUP_Type* TransferCfg, I2Cn_TRANSFER_OPT_Type Opt )
{
   TransferCfg->tx_data = NULL;
   TransferCfg->tx_length = 0;
   TransferCfg->tx_count = 0;
   TransferCfg->rx_count = 0;

   return HAL_I2C_MasterTransferData( I2Cx, TransferCfg, Opt );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Receive an array of bytes in Slave mode
 * @param[in]     I2Cx
 *                   Pointer to the target I2C
 *                   -  I2C0 ~ I2C1
 * @param[in]     TransferCfg
 *                   Pointer to a I2Cn_S_SETUP_Type structure
 *                   that contains specified information about the configuration for slave transfer.
 * @param[in]     Opt
 *                   I2Cn_TRANSFER_OPT_Type type that selected for interrupt or polling mode.
 * @return        Result
 *                   -  SUCCESS, ERROR
 *//*-------------------------------------------------------------------------*/
Status HAL_I2C_Slave_Receive( I2Cn_Type* I2Cx, I2Cn_S_SETUP_Type* TransferCfg, I2Cn_TRANSFER_OPT_Type Opt )
{
   TransferCfg->tx_data = NULL;
   TransferCfg->tx_length = 0;
   TransferCfg->tx_count = 0;
   TransferCfg->rx_count = 0;

   return HAL_I2C_SlaveTransferData( I2Cx, TransferCfg, Opt );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Transmit and Receive data in master mode by DMA.
 * @param[in]     I2Cx
 *                   Pointer to the target I2C
 *                   -  I2C0 ~ I2C1
 * @param[in]     TransferCfg
 *                   Pointer to a I2Cn_M_SETUP_Type structure
 *                   that contains specified information about the configuration for master transfer.
 * @param[in]     Opt
 *                   I2Cn_TRANSFER_OPT_Type type that selected for interrupt or polling mode.
 * @param[in]     TxDMACx
 *                   Pointer to the target DMAC for Tx
 *                   -  DMAC0 ~ DMAC4
 * @param[in]     RxDMACx
 *                   Pointer to the target DMAC for Rx
 *                   -  DMAC0 ~ DMAC4
 * @return        Result
 *                   -  SUCCESS, ERROR
 * @details       On the master Tx: "SnDA" by S/W + Transmit(N) by DMA + "CSnSP" by S/W
 *                On the master Rx: Up to "rSnDA" by S/W + Receive (N-1) by DMA + Receive(1+NACK) and "CSnSP" by S/W
 *//*-------------------------------------------------------------------------*/
Status HAL_I2C_MasterTransferData_DMA( I2Cn_Type* I2Cx, I2Cn_M_SETUP_Type* TransferCfg, I2Cn_TRANSFER_OPT_Type Opt, DMACn_Type* TxDMACx, DMACn_Type* RxDMACx )
{
   int32_t     Ret;

   // Reset I2C setup value to default state
   TransferCfg->tx_count = 0;
   TransferCfg->rx_count = 0;

   while( I2Cx->ST & 0x04 );     // busy check

   if( Opt == I2Cn_TRANSFER_POLLING )
   {
      // init count
      TransferCfg->tx_count = 0;
      TransferCfg->rx_count = 0;

      // tx transfer
      if( TransferCfg->tx_count < TransferCfg->tx_length )
      {
         // arm tx transfer
         HAL_DMAC_Setup( TxDMACx, ( uint32_t )TransferCfg->tx_data, TransferCfg->tx_length );

         // generate start condition
         I2Cx->DR = ( TransferCfg->sl_addr7bit << 1 );   // load slave address and write flag (SLA+RnW)
         I2Cx->CR |= ( 1 << 0 );                         // START
         Ret = I2Cn_MWait( I2Cx );
         if( ( Ret != TRANS_MODE ) )
         {
            // generate stop condition
            I2Cx->CR |= ( 1 << 1 );    // STOP
            I2Cn_MWait( I2Cx );
            I2Cx->ST = 0xFF;
            I2Cx->CR = 0
                       | ( 1 << 7 )    // I2C Block Enable
                       | ( 1 << 5 )    // Interrupt Enable
                       | ( 1 << 3 );   // ACK Signal Enable

            // return
            return ERROR;
         }

         // tx data
         while( 1 )
         {
            // if dma transfer complete interrupt
            if( TxDMACx->IESR_b.TRCIFGn == 1 )      // TRCIFGn[00:00]   [0] in [0(no request) 1(request; reset by w1)]
            {
               // reset TRCIFGn
               TxDMACx->IESR = TxDMACx->IESR
                               &  ~DMACn_IESR_TRERIFGn_Msk       // TRERIFGn[01:01]   [0] in [0(no request) 1(request; reset by w1)]
                               |   DMACn_IESR_TRCIFGn_Msk        // TRCIFGn[00:00]   [0] in [0(no request) 1(request; reset by w1)]
                               ;

               // if don't need rx
               if( TransferCfg->rx_count >= TransferCfg->rx_length )
               {
                  // generate stop condition
                  I2Cx->CR |= ( 1 << 1 );    // STOP
                  I2Cx->ST = 0xFF;
                  I2Cn_MWait( I2Cx );
                  I2Cx->ST = 0xFF;
                  I2Cx->CR = 0
                             | ( 1 << 7 )    // I2C Block Enable
                             | ( 1 << 5 )    // Interrupt Enable
                             | ( 1 << 3 );   // ACK Signal Enable

                  // return
                  return SUCCESS;
               }

               // exit
               break;
            }

            // if dma transfer error interrupt
            if( TxDMACx->IESR_b.TRERIFGn == 1 )      // TRERIFGn[01:01]   [0] in [0(no request) 1(request; reset by w1)]
            {
               // reset TRERIFGn
               TxDMACx->IESR = TxDMACx->IESR
                               &  ~DMACn_IESR_TRCIFGn_Msk        // TRCIFGn[00:00]   [0] in [0(no request) 1(request; reset by w1)]
                               |   DMACn_IESR_TRERIFGn_Msk       // TRERIFGn[01:01]   [0] in [0(no request) 1(request; reset by w1)]
                               ;

               // generate stop condition
               I2Cx->CR |= ( 1 << 1 );    // STOP
               I2Cn_MWait( I2Cx );
               I2Cx->ST = 0xFF;
               I2Cx->CR = 0
                          | ( 1 << 7 )    // I2C Block Enable
                          | ( 1 << 5 )    // Interrupt Enable
                          | ( 1 << 3 );   // ACK Signal Enable

               // return
               return ERROR;
            }
         }
      }

      // rx transfer
      if( TransferCfg->rx_count < TransferCfg->rx_length )
      {
         // arm
         if( ( TransferCfg->rx_length > 1 ) && ( TransferCfg->rx_count < ( TransferCfg->rx_length - 1 ) ) )
         {
            // arm rx transfer
            HAL_DMAC_Setup( RxDMACx, ( uint32_t )TransferCfg->rx_data, TransferCfg->rx_length - 1 );
         }
         else
         {
            // generate nack
            I2Cx->CR &= ~( 1 << 3 );   // ACK Signal Disable

            // arm rx transfer
            HAL_DMAC_Setup( RxDMACx, ( uint32_t )TransferCfg->rx_data, 1 );
         }

         // generate start condition
         I2Cx->DR = ( ( TransferCfg->sl_addr7bit << 1 ) | 0x01 );    // load slave address and read flag (SLA+RnW)
         I2Cx->CR |= ( 1 << 0 );                                     // START
         I2Cx->ST = 0xFF;
         Ret = I2Cn_MWait( I2Cx );
         if( ( Ret != RECEIVE_MODE ) )
         {
            // generate stop condition
            I2Cx->CR |= ( 1 << 1 );    // STOP
            I2Cn_MWait( I2Cx );
            I2Cx->ST = 0xFF;
            I2Cx->CR = 0
                       | ( 1 << 7 )    // I2C Block Enable
                       | ( 1 << 5 )    // Interrupt Enable
                       | ( 1 << 3 );   // ACK Signal Enable

            // return
            return ERROR;
         }

         // rx data
         while( 1 )
         {
            // if dma transfer complete interrupt
            if( RxDMACx->IESR_b.TRCIFGn == 1 )      // TRCIFGn[00:00]   [0] in [0(no request) 1(request; reset by w1)]
            {
               // reset TRCIFGn
               RxDMACx->IESR = RxDMACx->IESR
                               &  ~DMACn_IESR_TRERIFGn_Msk       // TRERIFGn[01:01]   [0] in [0(no request) 1(request; reset by w1)]
                               |   DMACn_IESR_TRCIFGn_Msk        // TRCIFGn[00:00]   [0] in [0(no request) 1(request; reset by w1)]
                               ;

               // arm
               if( ( TransferCfg->rx_length > 1 ) && ( TransferCfg->rx_count < ( TransferCfg->rx_length - 1 ) ) )
               {
                  // generate nack
                  I2Cx->CR &= ~( 1 << 3 );   // ACK Signal Disable

                  // arm rx transfer
                  HAL_DMAC_Setup( RxDMACx, ( uint32_t )( TransferCfg->rx_data + TransferCfg->rx_length - 1 ), 1 );

                  // last byte rx data
                  while( 1 )
                  {
                     // if dma transfer complete interrupt
                     if( RxDMACx->IESR_b.TRCIFGn == 1 )      // TRCIFGn[00:00]   [0] in [0(no request) 1(request; reset by w1)]
                     {
                        // reset TRCIFGn
                        RxDMACx->IESR = RxDMACx->IESR
                                        &  ~DMACn_IESR_TRERIFGn_Msk       // TRERIFGn[01:01]   [0] in [0(no request) 1(request; reset by w1)]
                                        |   DMACn_IESR_TRCIFGn_Msk        // TRCIFGn[00:00]   [0] in [0(no request) 1(request; reset by w1)]
                                        ;

                        // exit
                        break;
                     }

                     // if dma transfer error interrupt
                     if( RxDMACx->IESR_b.TRERIFGn == 1 )      // TRERIFGn[01:01]   [0] in [0(no request) 1(request; reset by w1)]
                     {
                        // reset TRERIFGn
                        RxDMACx->IESR = RxDMACx->IESR
                                        &  ~DMACn_IESR_TRCIFGn_Msk        // TRCIFGn[00:00]   [0] in [0(no request) 1(request; reset by w1)]
                                        |   DMACn_IESR_TRERIFGn_Msk       // TRERIFGn[01:01]   [0] in [0(no request) 1(request; reset by w1)]
                                        ;

                        // generate stop condition
                        I2Cx->CR |= ( 1 << 1 );    // STOP
                        I2Cn_MWait( I2Cx );
                        I2Cx->ST = 0xFF;
                        I2Cx->CR = 0
                                   | ( 1 << 7 )    // I2C Block Enable
                                   | ( 1 << 5 )    // Interrupt Enable
                                   | ( 1 << 3 );   // ACK Signal Enable

                        // return
                        return ERROR;
                     }
                  }
               }

               // exit
               break;
            }

            // if dma transfer error interrupt
            if( RxDMACx->IESR_b.TRERIFGn == 1 )      // TRERIFGn[01:01]   [0] in [0(no request) 1(request; reset by w1)]
            {
               // reset TRERIFGn
               RxDMACx->IESR = RxDMACx->IESR
                               &  ~DMACn_IESR_TRCIFGn_Msk        // TRCIFGn[00:00]   [0] in [0(no request) 1(request; reset by w1)]
                               |   DMACn_IESR_TRERIFGn_Msk       // TRERIFGn[01:01]   [0] in [0(no request) 1(request; reset by w1)]
                               ;

               // generate stop condition
               I2Cx->CR |= ( 1 << 1 );    // STOP
               I2Cn_MWait( I2Cx );
               I2Cx->ST = 0xFF;
               I2Cx->CR = 0
                          | ( 1 << 7 )    // I2C Block Enable
                          | ( 1 << 5 )    // Interrupt Enable
                          | ( 1 << 3 );   // ACK Signal Enable

               // return
               return ERROR;
            }
         }

         // generate stop condition
         I2Cx->CR |= ( 1 << 1 );    // STOP
         I2Cx->ST = 0xFF;
         I2Cn_MWait( I2Cx );
         I2Cx->ST = 0xFF;
         I2Cx->CR = 0
                    | ( 1 << 7 )    // I2C Block Enable
                    | ( 1 << 5 )    // Interrupt Enable
                    | ( 1 << 3 );   // ACK Signal Enable

         // return
         return SUCCESS;
      }
   }

   // return
   return ERROR;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Receive and Transmit data in slave mode by DMA.
 * @param[in]     I2Cx
 *                   Pointer to the target I2C
 *                   -  I2C0 ~ I2C1
 * @param[in]     TransferCfg
 *                   Pointer to a I2Cn_S_SETUP_Type structure
 *                   that contains specified information about the configuration for master transfer.
 * @param[in]     Opt
 *                   I2Cn_TRANSFER_OPT_Type type that selected for interrupt or polling mode.
 * @param[in]     TxDMACx
 *                   Pointer to the target DMAC for Tx
 *                   -  DMAC0 ~ DMAC4
 * @param[in]     RxDMACx
 *                   Pointer to the target DMAC for Rx
 *                   -  DMAC0 ~ DMAC4
 * @return        Result
 *                   -  SUCCESS, ERROR
 * @details       On the slave Tx: Up to "rSnDA" by S/W + Transmit (N) by DMA + "CS" by S/W
 *                On the slave Rx: "SnDA" by S/W + Receive(N) by DMA + "CS" by S/W
 *//*-------------------------------------------------------------------------*/
Status HAL_I2C_SlaveTransferData_DMA( I2Cn_Type* I2Cx, I2Cn_S_SETUP_Type* TransferCfg, I2Cn_TRANSFER_OPT_Type Opt, DMACn_Type* TxDMACx, DMACn_Type* RxDMACx )
{
   int32_t     Ret;

   // Reset I2C setup value to default state
   TransferCfg->tx_count = 0;
   TransferCfg->rx_count = 0;

   // Polling option
   if( Opt == I2Cn_TRANSFER_POLLING )
   {
      if( ( TransferCfg->rx_count < TransferCfg->rx_length ) && ( TransferCfg->rx_data != NULL ) )
      {
         while( 1 )
         {
            Ret = I2Cn_SWait( I2Cx );
            if( ( Ret == RECEIVE_MODE ) || ( Ret == RECEIVE_DATA ) )
            {
               // arm rx transfer
               HAL_DMAC_Setup( RxDMACx, ( uint32_t )TransferCfg->rx_data, TransferCfg->rx_length );

               // exit
               break;
            }
         }
      }

      if( ( TransferCfg->tx_count < TransferCfg->tx_length ) && ( TransferCfg->tx_data != NULL ) )
      {
         while( 1 )
         {
            Ret = I2Cn_SWait( I2Cx );
            if( ( Ret == TRANS_MODE ) || ( Ret == TRANS_DATA ) )
            {
               // arm tx transfer
               HAL_DMAC_Setup( TxDMACx, ( uint32_t )TransferCfg->tx_data, TransferCfg->tx_length );

               // exit
               break;
            }
         }
      }

      while( 1 )
      {
         Ret = I2Cn_SWait( I2Cx );
         switch( Ret )
         {
            case STOP_DECT:
               goto s_end_stage;
            case 0:
               break;
            default:
               goto s_error;
         }
      }

s_end_stage:
      I2Cx->ST = 0xFF;
      return SUCCESS;

s_error:
      I2Cx->ST = 0xFF;
      return ERROR;
   }

   // return
   return ERROR;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Transmit an array of bytes in Master mode by DMA.
 * @param[in]     I2Cx
 *                   Pointer to the target I2C
 *                   -  I2C0 ~ I2C1
 * @param[in]     TransferCfg
 *                   Pointer to a I2Cn_M_SETUP_Type structure
 *                   that contains specified information about the configuration for master transfer.
 * @param[in]     Opt
 *                   I2Cn_TRANSFER_OPT_Type type that selected for interrupt or polling mode.
 * @param[in]     TxDMACx
 *                   Pointer to the target DMAC for Tx
 *                   -  DMAC0 ~ DMAC4
 * @param[in]     RxDMACx
 *                   Pointer to the target DMAC for Rx
 *                   -  DMAC0 ~ DMAC4
 * @return        Result
 *                   -  SUCCESS, ERROR
 *//*-------------------------------------------------------------------------*/
Status HAL_I2C_MasterTransmit_DMA( I2Cn_Type* I2Cx, I2Cn_M_SETUP_Type* TransferCfg, I2Cn_TRANSFER_OPT_Type Opt, DMACn_Type* TxDMACx, DMACn_Type* RxDMACx )
{
   TransferCfg->rx_data = NULL;
   TransferCfg->rx_length = 0;
   TransferCfg->tx_count = 0;
   TransferCfg->rx_count = 0;

   return HAL_I2C_MasterTransferData_DMA( I2Cx, TransferCfg, Opt, TxDMACx, RxDMACx );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Receive an array of bytes in Slave mode by DMA.
 * @param[in]     I2Cx
 *                   Pointer to the target I2C
 *                   -  I2C0 ~ I2C1
 * @param[in]     TransferCfg
 *                   Pointer to a I2Cn_S_SETUP_Type structure
 *                   that contains specified information about the configuration for slave transfer.
 * @param[in]     Opt
 *                   I2Cn_TRANSFER_OPT_Type type that selected for interrupt or polling mode.
 * @param[in]     TxDMACx
 *                   Pointer to the target DMAC for Tx
 *                   -  DMAC0 ~ DMAC4
 * @param[in]     RxDMACx
 *                   Pointer to the target DMAC for Rx
 *                   -  DMAC0 ~ DMAC4
 * @return        Result
 *                   -  SUCCESS, ERROR
 *//*-------------------------------------------------------------------------*/
Status HAL_I2C_SlaveReceive_DMA( I2Cn_Type* I2Cx, I2Cn_S_SETUP_Type* TransferCfg, I2Cn_TRANSFER_OPT_Type Opt, DMACn_Type* TxDMACx, DMACn_Type* RxDMACx )
{
   TransferCfg->tx_data = NULL;
   TransferCfg->tx_length = 0;
   TransferCfg->tx_count = 0;
   TransferCfg->rx_count = 0;

   return HAL_I2C_SlaveTransferData_DMA( I2Cx, TransferCfg, Opt, TxDMACx, RxDMACx );
}


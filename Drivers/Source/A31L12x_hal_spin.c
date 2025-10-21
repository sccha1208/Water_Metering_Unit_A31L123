/**
 *******************************************************************************
 * @file        A31L12x_hal_spin.c
 * @author      ABOV R&D Division
 * @brief       SPI communication
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

#include "A31L12x_hal_scu.h"
#include "A31L12x_hal_spin.h"

//******************************************************************************
// Variable
//******************************************************************************

uint32_t    SPIn_BaseClock;
Bool        SPIn_RXC = FALSE;    // rx complete flag

/* Public Functions --------------------------------------------------------- */
//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Determines best dividers to get a target clock rate
 * @param[in]     SPIx
 *                   Pointer to the target SPI
 *                   -  SPI0 ~ SPI1
 * @param[in]     baudrate
 *                   Desired baudrate
 * @return        None
 *//*-------------------------------------------------------------------------*/
static void spi_set_divisors( SPIn_Type* SPIx, uint32_t baudrate )
{
   uint32_t    numerator;
   uint32_t    denominator;
   uint32_t    n;
   uint32_t    bdr;

   // n
   {
      // baudrate = PCLK / (2 * (bdr + 1))
      // bdr = (PCLK / (2 * baudrate)) - 1
      n = 2;
   }

   //--------------------------------------
   // numerator & denominator
   //
   // bdr = SPIn_BaseClock / n / baudrate  - 1
   //--------------------------------------
   numerator = SPIn_BaseClock;
   denominator = baudrate;

   bdr = numerator / n / denominator - 1;

   SPIx->PREDR = ( uint16_t )( bdr & 0xffff );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Initialize the SPIn peripheral with the specified parameters.
 * @param[in]     SPIx
 *                   Pointer to the target SPI
 *                   -  SPI0 ~ SPI1
 * @param[in]     SPIn_Config
 *                   Pointer to a SPIn_CFG_Type structure
 *                   that contains the configuration information for the specified peripheral.
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_SPIn_Init( SPIn_Type* SPIx, SPIn_CFG_Type* SPIn_Config )
{
   /* Check SPI handle */
   if( SPIx == NULL )
   {
      return HAL_ERROR;
   }

#if 1    // supported
   if( SPIx == ( SPIn_Type* )SPI0 )
   {
      // enable peripheral clock and reset peripheral
      HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_SPI0CLKE, PPxCLKE_Enable );
      HAL_SCU_Peripheral_SetReset2( PPRST2_SPI0RST );
   }
#endif

#if 1    // supported
   if( SPIx == ( SPIn_Type* )SPI1 )
   {
      // enable peripheral clock and reset peripheral
      HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_SPI1CLKE, PPxCLKE_Enable );
      HAL_SCU_Peripheral_SetReset2( PPRST2_SPI1RST );
   }
#endif

   // select baudrate
   SPIn_BaseClock = SystemPeriClock;
   spi_set_divisors( SPIx, SPIn_Config->Baudrate );

   // select bit order / polarity(idle state) / phase(start state)
   SPIx->CR =     0
                  | ( SPIn_Config->Order << SPIn_CR_FLSBn_Pos )
                  | ( SPIn_Config->ACK   << SPIn_CR_CPOLn_Pos )
                  | ( SPIn_Config->Edge  << SPIn_CR_CPHAn_Pos )
                  ;

   // clear flag
   SPIx->SR =     SPIx->SR
                  |  SPIn_SR_SPInIFLAG_Msk      // SPInIFLAG[07:07]   [0] in [0(no request occurred) 1(request occurred; reset by w1)]
                  |  SPIn_SR_SSnHIGH_Msk      // SSnHIGH[04:04]   [0] in [0(no effect) 1(The SSn pin has gone from low level to high; reset by w1)]
                  ;

   // clear rxc flag
   SPIn_RXC = FALSE;

   // dummy read
   SPIn_ReceiveByte( SPIx );
   SPIn_ReceiveByte( SPIx );

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Deinitialize the SPIn peripheral registers to their default reset values.
 * @param[in]     SPIx
 *                   Pointer to the target SPI
 *                   -  SPI0 ~ SPI1
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type SPIn_DeInit( SPIn_Type* SPIx )
{
   /* Check SPI handle */
   if( SPIx == NULL )
   {
      return HAL_ERROR;
   }

#if 1    // supported
   if( SPIx == ( SPIn_Type* )SPI0 )
   {
      // reset peripheral and disable peripheral clock
      HAL_SCU_Peripheral_SetReset2( PPRST2_SPI0RST );
      HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_SPI0CLKE, PPxCLKE_Disable );
   }
#endif

#if 1    // supported
   if( SPIx == ( SPIn_Type* )SPI1 )
   {
      // reset peripheral and disable peripheral clock
      HAL_SCU_Peripheral_SetReset2( PPRST2_SPI1RST );
      HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_SPI1CLKE, PPxCLKE_Disable );
   }
#endif

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Fills each SPIn_CFG_Type structure member with its default value:
 *                   -  38400 bps
 *                   -  lsb first
 *                   -  CPOLn = 0
 *                   -  CPHAn = 0
 * @param[out]    SPIn_Config
 *                   Pointer to a SPIn_CFG_Type structure which will be initialized.
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type SPIn_ConfigStructInit( SPIn_CFG_Type* SPIn_Config )
{
   /* Check SPIn_Config */
   if( SPIn_Config == NULL )
   {
      return HAL_ERROR;
   }

   SPIn_Config->Baudrate = 38400;
   SPIn_Config->Order = SPIn_LSB_FIRST;

#if 1 // CPOLn : 0, CPHAn : 0 (X)
   SPIn_Config->ACK = SPIn_TX_RISING;
   SPIn_Config->Edge = SPIn_TX_LEADEDGE_SAMPLE;
#endif

#if 0 // CPOLn : 0, CPHAn : 1 (O)
   SPIn_Config->ACK = SPIn_TX_RISING;
   SPIn_Config->Edge = SPIn_TX_LEADEDGE_SETUP;
#endif

#if 0 // CPOLn : 1, CPHAn : 0 (X)
   SPIn_Config->ACK = SPIn_TX_FALLING;
   SPIn_Config->Edge = SPIn_TX_LEADEDGE_SAMPLE;
#endif

#if 0 // CPOLn : 1, CPHAn : 1 (O)
   SPIn_Config->ACK = SPIn_TX_FALLING;
   SPIn_Config->Edge = SPIn_TX_LEADEDGE_SETUP;
#endif

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure the interrupt source of selected SPIn peripheral
 * @param[in]     SPIx
 *                   Pointer to the target SPI
 *                   -  SPI0 ~ SPI1
 * @param[in]     SPIn_IntCfg
 *                   Specifies the interrupt source
 *                   -  SPIn_INTCFG_SPInIEN
 * @param[in]     NewState
 *                   Next State of Interrupt Operation
 *                   -  ENABLE
 *                   -  DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type SPIn_ConfigInterrupt( SPIn_Type* SPIx, SPIn_INT_Type SPIn_IntCfg, FunctionalState NewState )
{
   uint32_t    mask;

   /* Check SPI handle */
   if( SPIx == NULL )
   {
      return HAL_ERROR;
   }

   // get mask
   switch( SPIn_IntCfg )
   {
      case SPIn_INTCFG_SPInIEN:
         mask = SPIn_CR_SPInIEN_Msk;
         break;

      default:
         return HAL_ERROR;
   }

   // enable/disable
   if( NewState == ENABLE )
   {
      SPIx->CR |= mask;
   }
   else
   {
      SPIx->CR &= ~mask;
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure Data Control mode for SPIn peripheral
 * @param[in]     SPIx
 *                   Pointer to the target SPI
 *                   -  SPI0 ~ SPI1
 * @param[in]     Mode
 *                   Data Control Mode
 *                   -  SPIn_CONTROL_SPInMS
 *                   -  SPIn_CONTROL_FXCHn
 *                   -  SPIn_CONTROL_SSnEN
 * @param[in]     NewState
 *                   Next State of Functional Operation
 *                   -  ENABLE
 *                   -  DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type SPIn_DataControlConfig( SPIn_Type* SPIx, SPIn_CONTROL_Type Control, FunctionalState NewState )
{
   uint16_t    mask;

   /* Check SPI handle */
   if( SPIx == NULL )
   {
      return HAL_ERROR;
   }

   // if Control is for SPIx->CR
   if( Control <= SPIn_CONTROL_SPInMS )
   {
      switch( Control )
      {
         case SPIn_CONTROL_SPInMS:
            mask = SPIn_CR_SPInMS_Msk;
            break;
      }

      if( NewState == ENABLE )
      {
         SPIx->CR |= mask;
      }
      else
      {
         SPIx->CR &= ~mask;
      }
   }

   // if Control is for SPIx->SR
   if( Control >= SPIn_CONTROL_FXCHn )
   {
      switch( Control )
      {
         case SPIn_CONTROL_FXCHn:
            mask = SPIn_SR_FXCHn_Msk;
            break;
         case SPIn_CONTROL_SSnEN:
            mask = SPIn_SR_SSnEN_Msk;
            break;
      }

      if( NewState == ENABLE )
      {
         SPIx->SR |= mask;
      }
      else
      {
         SPIx->SR &= ~mask;
      }
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         SPIn enable control
 * @param[in]     SPIx
 *                   Pointer to the target SPI
 *                   -  SPI0 ~ SPI1
 * @param[in]     state
 *                   -  ENABLE
 *                   -  DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type SPIn_Enable( SPIn_Type* SPIx, FunctionalState state )
{
   /* Check SPI handle */
   if( SPIx == NULL )
   {
      return HAL_ERROR;
   }

   if( state == ENABLE )
   {
      SPIx->CR |= ( 1 << SPIn_CR_SPInEN_Pos );
   }
   else
   {
      SPIx->CR &= ~( 1 << SPIn_CR_SPInEN_Pos );
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         This function clears the flag bit of Status Register.
 * @param[in]     SPIx
 *                   Pointer to the target SPI
 *                   -  SPI0 ~ SPI1
 * @param[in]     Status
 *                   -  SPIn_STATUS_SPInIFLAG
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type SPIn_ClearStatus( SPIn_Type* SPIx, SPIn_STATUS_Type Status )
{
   uint32_t    reg;

   /* Check SPI handle */
   if( SPIx == NULL )
   {
      return HAL_ERROR;
   }

   reg  = SPIx->SR;
   reg &= ~SPIn_SR_SSnHIGH_Msk;

   switch( Status )
   {
      case SPIn_STATUS_SPInIFLAG:
         reg |= SPIn_SR_SPInIFLAG_Msk;
         break;

      default:
         return HAL_ERROR;
   }

   SPIx->SR = reg;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         This function returns the current value of Status Register.
 * @param[in]     SPIx
 *                   Pointer to the target SPI
 *                   -  SPI0 ~ SPI1
 * @return        Current value of Status register in SPIn peripheral.
 *//*-------------------------------------------------------------------------*/
uint8_t SPIn_GetStatus( SPIn_Type* SPIx )
{
   return ( SPIx->SR );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Transmit a single data through SPIn peripheral
 * @param[in]     SPIx
 *                   Pointer to the target SPI
 *                   -  SPI0 ~ SPI1
 * @param[in]     Data
 *                   Data to transmit (must be 8-bit long)
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type SPIn_SendByte( SPIn_Type* SPIx, uint8_t Data )
{
   /* Check SPI handle */
   if( SPIx == NULL )
   {
      return HAL_ERROR;
   }

   SPIx->TDR = Data;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Receive a single data from SPIn peripheral
 * @param[in]     SPIx
 *                   Pointer to the target SPI
 *                   -  SPI0 ~ SPI1
 * @return        Data received
 *//*-------------------------------------------------------------------------*/
uint8_t SPIn_ReceiveByte( SPIn_Type* SPIx )
{
   return SPIx->RDR;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Send a block of data via SPIn peripheral
 * @param[in]     SPIx
 *                   Pointer to the target SPI
 *                   -  SPI0 ~ SPI1
 * @param[in]     txbuf
 *                   Pointer to Transmit buffer
 * @param[in]     buflen
 *                   Length of Transmit buffer
 * @param[in]     flag
 *                   Flag used in  USART transfer
 *                   -  NONE_BLOCKING
 *                   -  BLOCKING
 * @return        Number of bytes sent.
 * @note          when using USART in BLOCKING mode,
 *                a time-out condition is used via defined symbol SPIn_BLOCKING_TIMEOUT.
 *//*-------------------------------------------------------------------------*/
uint32_t SPIn_Send( SPIn_Type* SPIx, uint8_t* txbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag )
{
   uint32_t    bToSend, bSent, timeOut;
   uint8_t*    pChar = txbuf;

   // init counter
   bToSend = buflen;
   bSent = 0;

   // Blocking Mode
   if( flag == BLOCKING )
   {
      while( bToSend )
      {
         // send byte
         SPIn_SendByte( SPIx, ( *pChar++ ) );

         // wait until tx data register is empty with timeout
         timeOut = SPIn_BLOCKING_TIMEOUT;
         while( !( SPIx->SR & SPIn_SR_SPInIFLAG_Msk ) )
         {
            if( timeOut == 0 )
            {
               break;
            }
            timeOut--;
         }

         // if timeout
         if( timeOut == 0 )
         {
            break;
         }

         // clear interrupt flag
         SPIn_ClearStatus( SPIx, SPIn_STATUS_SPInIFLAG );

         // update counter
         bToSend--;
         bSent++;

         // set rxc flag
         SPIn_RXC = TRUE;
      }
   }

   // Non-Blocking Mode
   else
   {
      while( bToSend )
      {
         // if tx data register is not empty
         if( SPIx->SR & SPIn_SR_SPInIFLAG_Msk )
         {
            break;
         }

         // send byte
         SPIn_SendByte( SPIx, ( *pChar++ ) );

         // update counter
         bToSend--;
         bSent++;
      }
   }

   // return
   return bSent;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Receive a block of data via SPIn peripheral
 * @param[in]     SPIx
 *                   Pointer to the target SPI
 *                   -  SPI0 ~ SPI1
 * @param[out]    rxbuf
 *                   Pointer to Received buffer
 * @param[in]     buflen
 *                   Length of Received buffer
 * @param[in]     flag
 *                   Flag mode
 *                   -  NONE_BLOCKING
 *                   -  BLOCKING
 * @return        Number of bytes received
 * @note          when using USART in BLOCKING mode,
 *                a time-out condition is used via defined symbol USART_BLOCKING_TIMEOUT.
 *//*-------------------------------------------------------------------------*/
uint32_t SPIn_Receive( SPIn_Type* SPIx, uint8_t* rxbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag )
{
   uint32_t    bToRecv, bRecv, timeOut;
   uint8_t*    pChar = rxbuf;

   // init counter
   bToRecv = buflen;
   bRecv = 0;

   // Blocking Mode
   if( flag == BLOCKING )
   {
      while( bToRecv )
      {
         // wait until data are received with timeout
         timeOut = SPIn_BLOCKING_TIMEOUT;
         while( SPIn_RXC == FALSE )
         {
            if( timeOut == 0 )
            {
               break;
            }
            timeOut--;
         }

         // if timeout
         if( timeOut == 0 )
         {
            break;
         }

         // receive byte
         ( *pChar++ ) = SPIn_ReceiveByte( SPIx );

         // update counter
         bToRecv--;
         bRecv++;

         // clear rxc flag
         SPIn_RXC = FALSE;
      }
   }

   // Non-Blocking Mode
   else
   {
      while( bToRecv )
      {
         // if no data were received
         if( !( SPIx->SR & SPIn_SR_SPInIFLAG_Msk ) )
         {
            break;
         }

         // receive byte
         ( *pChar++ ) = SPIn_ReceiveByte( SPIx );

         // update counter
         bRecv++;
         bToRecv--;
      }
   }

   // return
   return bRecv;
}


/**
 *******************************************************************************
 * @file        A31L12x_hal_lpuart.c
 * @author      ABOV R&D Division
 * @brief       LPUART communication
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

#include "A31L12x_hal_lpuart.h"
#include "A31L12x_hal_scu.h"

/* Public Functions --------------------------------------------------------- */
//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Determines best dividers to get a target clock rate
 * @param[in]     LPUART_Config
 *                   Pointer to a LPUART_CFG_Type structure
 * @return        None
 *//*-------------------------------------------------------------------------*/
static void lpuart_set_divisors( LPUART_CFG_Type* LPUART_Config )
{
   uint32_t    numerator;
   uint32_t    denominator;
   uint32_t    n;
   uint32_t    bdr;

   switch( LPUART_Config->OverSampling )
   {
      case LPUART_OVRS_16:
         n = 16;
         break;
      case LPUART_OVRS_8:
         n = 8;
         break;
      case LPUART_OVRS_1:
         n = 1;
         break;
   }

   //--------------------------------------
   // numerator & denominator
   //
   // bdr = LPUART_BaseClock / n / baudrate  - 1
   //--------------------------------------
   numerator = LPUART_Config->BaseClock;
   denominator = LPUART_Config->Baudrate;

   bdr = numerator / n / denominator - 1;

   LPUART->BDR = ( uint16_t )( bdr & 0xffff );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Initialize the LPUART peripheral with the specified parameters.
 * @param[in]     LPUART_Config
 *                   Pointer to a LPUART_CFG_Type structure
 *                   that contains the configuration information for the specified peripheral.
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_LPUART_Init( LPUART_CFG_Type* LPUART_Config )
{
   /* Check LPUART_Config */
   if( LPUART_Config == NULL )
   {
      return HAL_ERROR;
   }

   // enable peripheral clock and reset peripheral
   HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_LPUTCLKE, PPxCLKE_Enable );
   HAL_SCU_Peripheral_SetReset2( PPRST2_LPUTRST );

   lpuart_set_divisors( LPUART_Config );

   LPUART->CR1 = 0
                 | ( LPUART_Config->OverSampling << LPUART_CR1_OVRS_Pos )
                 | ( LPUART_Config->DataBit      << LPUART_CR1_DLEN_Pos )
                 | ( LPUART_Config->ParityBit    << LPUART_CR1_PSEL_Pos )
                 | ( LPUART_Config->StopBit      << LPUART_CR1_STOPB_Pos )
                 | ( 1                        << LPUART_CR1_TXE_Pos )
                 | ( 1                        << LPUART_CR1_RXE_Pos )
                 ;

   // dummy read
   HAL_LPUART_ReceiveByte();
   HAL_LPUART_ReceiveByte();

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Deinitialize the LPUART peripheral registers
 *             to their default reset values.
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_LPUART_DeInit( void )
{
   // reset peripheral and disable peripheral clock
   HAL_SCU_Peripheral_SetReset2( PPRST2_LPUTRST );
   HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_LPUTCLKE, PPxCLKE_Disable );

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Fills each LPUART_Config member with its default value:
 *                   -  38400 bps
 *                   -  8 Data Bit
 *                   -  No Parity Bit
 *                   -  1 Stop Bit
 * @param[out]    LPUART_Config
 *                   Pointer to a LPUART_CFG_Type structure which will be initialized.
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_LPUART_ConfigStructInit( LPUART_CFG_Type* LPUART_Config )
{
   /* Check LPUART_Config */
   if( LPUART_Config == NULL )
   {
      return HAL_ERROR;
   }

   LPUART_Config->BaseClock = SystemPeriClock;
   LPUART_Config->Baudrate = 38400;  // Digital Meter Protocol: 38400 bps

   LPUART_Config->OverSampling = LPUART_OVRS_16;

   LPUART_Config->DataBit = LPUART_DATA_BIT_8;
   LPUART_Config->ParityBit = LPUART_PARITY_BIT_NONE;
   LPUART_Config->StopBit = LPUART_STOP_BIT_1;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure the interrupt source of selected LPUART peripheral
 * @param[in]     LPUART_IntCfg
 *                   Specifies the interrupt source
 *                   -  LPUART_INTCFG_RXCIEN
 *                   -  LPUART_INTCFG_TXCIEN
 *                   -  LPUART_INTCFG_SBDIEN
 *                   -  LPUART_INTCFG_RTOIEN
 *                   -  LPUART_INTCFG_RCDIEN
 * @param[in]     NewState
 *                   Next State of Interrupt Operation
 *                   -  ENABLE, DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_LPUART_ConfigInterrupt( LPUART_INT_Type LPUART_IntCfg, FunctionalState NewState )
{
   uint32_t    mask;

   // get mask
   switch( LPUART_IntCfg )
   {
      case LPUART_INTCFG_RXCIEN:
         mask = LPUART_IER_RXCIEN_Msk;
         break;
      case LPUART_INTCFG_TXCIEN:
         mask = LPUART_IER_TXCIEN_Msk;
         break;
      case LPUART_INTCFG_SBDIEN:
         mask = LPUART_IER_SBDIEN_Msk;
         break;
      case LPUART_INTCFG_RTOIEN:
         mask = LPUART_IER_RTOIEN_Msk;
         break;
      case LPUART_INTCFG_RCDIEN:
         mask = LPUART_IER_RCDIEN_Msk;
         break;

      default:
         return HAL_ERROR;
   }

   // enable/disable
   if( NewState == ENABLE )
   {
      LPUART->IER |= mask;
   }
   else
   {
      LPUART->IER &= ~mask;
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure Data Control mode for LPUART peripheral
 * @param[in]     Mode
 *                   Data Control Mode
 *                   -  LPUART_CONTROL_PEN
 *                   -  LPUART_CONTROL_STKPEN
 *                   -  LPUART_CONTROL_PSEL
 *                   -  LPUART_CONTROL_STOPB
 *                   -  LPUART_CONTROL_HDCOM
 *                   -  LPUART_CONTROL_TXE
 *                   -  LPUART_CONTROL_RXE
 *                   -  LPUART_CONTROL_WAKEN
 *                   -  LPUART_CONTROL_LPUEN
 *                   -  LPUART_CONTROL_DEPOL
 *                   -  LPUART_CONTROL_DEPEN
 *                   -  LPUART_CONTROL_RCDEN
 *                   -  LPUART_CONTROL_RTOEN
 * @param[in]     NewState
 *                   Next State of Functional Operation
 *                   -  ENABLE, DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_LPUART_DataControlConfig( LPUART_CONTROL_Type Mode, FunctionalState NewState )
{
   uint32_t    mask;

   // CR1
   if( Mode <= LPUART_CONTROL_LPUEN )
   {
      // get mask
      switch( Mode )
      {
         case LPUART_CONTROL_PEN:
            mask = LPUART_CR1_PEN_Msk;
            break;
         case LPUART_CONTROL_STKPEN:
            mask = LPUART_CR1_STKPEN_Msk;
            break;
         case LPUART_CONTROL_PSEL:
            mask = LPUART_CR1_PSEL_Msk;
            break;
         case LPUART_CONTROL_STOPB:
            mask = LPUART_CR1_STOPB_Msk;
            break;
         case LPUART_CONTROL_HDCOM:
            mask = LPUART_CR1_HDCOM_Msk;
            break;
         case LPUART_CONTROL_TXE:
            mask = LPUART_CR1_TXE_Msk;
            break;
         case LPUART_CONTROL_RXE:
            mask = LPUART_CR1_RXE_Msk;
            break;
         case LPUART_CONTROL_WAKEN:
            mask = LPUART_CR1_WAKEN_Msk;
            break;
         case LPUART_CONTROL_LPUEN:
            mask = LPUART_CR1_LPUEN_Msk;
            break;

         default:
            return HAL_ERROR;
      }

      // enable/disable
      if( NewState == ENABLE )
      {
         LPUART->CR1 |= mask;
      }
      else
      {
         LPUART->CR1 &= ~mask;
      }
   }

   // CR2
   if( Mode >= LPUART_CONTROL_DEPOL )
   {
      // get mask
      switch( Mode )
      {
         case LPUART_CONTROL_DEPOL:
            mask = LPUART_CR2_DEPOL_Msk;
            break;
         case LPUART_CONTROL_DEPEN:
            mask = LPUART_CR2_DEPEN_Msk;
            break;
         case LPUART_CONTROL_RCDEN:
            mask = LPUART_CR2_RCDEN_Msk;
            break;
         case LPUART_CONTROL_RTOEN:
            mask = LPUART_CR2_RTOEN_Msk;
            break;

         default:
            return HAL_ERROR;
      }

      // enable/disable
      if( NewState == ENABLE )
      {
         LPUART->CR2 |= mask;
      }
      else
      {
         LPUART->CR2 &= ~mask;
      }
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         LPUART enable control
 * @param[in]     state
 *                   -  ENABLE, DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_LPUART_Enable( FunctionalState state )
{
   LPUART->CR1_b.LPUEN = state;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         This function clears Interrupt Flag and Status Register.
 * @param[in]     Status
 *                   -  LPUART_STATUS_RXCIFLAG
 *                   -  LPUART_STATUS_TXCIFLAG
 *                   -  LPUART_STATUS_SBDIFLAG
 *                   -  LPUART_STATUS_RTOIFLAG
 *                   -  LPUART_STATUS_RCDIFLAG
 *                   -  LPUART_STATUS_RXBUSY
 *                   -  LPUART_STATUS_PE
 *                   -  LPUART_STATUS_FE
 *                   -  LPUART_STATUS_DOR
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_LPUART_ClearStatus( LPUART_STATUS_Type Status )
{
   uint32_t    reg;

   reg = LPUART->IFSR;

   switch( Status )
   {
      case LPUART_STATUS_RXCIFLAG:
         reg |= LPUART_IFSR_RXCIFLAG_Msk;
         break;
      case LPUART_STATUS_TXCIFLAG:
         reg |= LPUART_IFSR_TXCIFLAG_Msk;
         break;
      case LPUART_STATUS_SBDIFLAG:
         reg |= LPUART_IFSR_SBDIFLAG_Msk;
         break;
      case LPUART_STATUS_RTOIFLAG:
         reg |= LPUART_IFSR_RTOIFLAG_Msk;
         break;
      case LPUART_STATUS_RCDIFLAG:
         reg |= LPUART_IFSR_RCDIFLAG_Msk;
         break;
      case LPUART_STATUS_RXBUSY:
         reg |= LPUART_IFSR_RXBUSY_Msk;
         break;
      case LPUART_STATUS_PE:
         reg |= LPUART_IFSR_PE_Msk;
         break;
      case LPUART_STATUS_FE:
         reg |= LPUART_IFSR_FE_Msk;
         break;
      case LPUART_STATUS_DOR:
         reg |= LPUART_IFSR_DOR_Msk;
         break;
      default:
         return HAL_ERROR;
   }

   LPUART->IFSR = reg;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         This function returns the current value of Interrupt Flag and Status Register.
 * @return        Current value of Status register in LPUART peripheral.
 *//*-------------------------------------------------------------------------*/
uint8_t HAL_LPUART_GetStatus( void )
{
   return LPUART->IFSR;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Transmit a single data through USART peripheral
 * @param[in]     Data
 *                   Data to transmit (must be 8-bit long)
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_LPUART_TransmitByte( uint8_t Data )
{
   LPUART->TDR = Data;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Receive a single data from USART peripheral
 * @return        Data     received
 *//*-------------------------------------------------------------------------*/
uint8_t HAL_LPUART_ReceiveByte( void )
{
   return LPUART->RDR;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Send a block of data via LPUART peripheral
 * @param[in]     txbuf
 *                   Pointer to Transmit buffer
 * @param[in]     buflen
 *                   Length of Transmit buffer
 * @param[in]     flag
 *                   Flag used in LPUART transfer
 *                   -  NONE_BLOCKING
 *                   -  BLOCKING
 * @return        Number of bytes sent.
 * @note          when using LPUART in BLOCKING mode,
 *                a time-out condition is used via defined symbol LPUART_BLOCKING_TIMEOUT.
 *//*-------------------------------------------------------------------------*/
uint32_t HAL_LPUART_Transmit( uint8_t* txbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag )
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
         HAL_LPUART_TransmitByte( *pChar++ );

         // wait until tx data register is empty with timeout
         timeOut = LPUART_BLOCKING_TIMEOUT;
         while( !( LPUART->IFSR & LPUART_IFSR_TXCIFLAG_Msk ) )
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

         // clear flag
         HAL_LPUART_ClearStatus( LPUART_STATUS_TXCIFLAG );

         // update counter
         bToSend--;
         bSent++;
      }
   }

   // Non-Blocking Mode
   else
   {
      while( bToSend )
      {
         // if tx data register is not empty
         if( !( LPUART->IFSR & LPUART_IFSR_TXCIFLAG_Msk ) )
         {
            break;
         }

         // send byte
         HAL_LPUART_TransmitByte( *pChar++ );

         // update counter
         bToSend--;
         bSent++;
      }
   }

   // return
   return bSent;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Receive a block of data via LPUART peripheral
 * @param[out]    rxbuf
 *                   Pointer to Received buffer
 * @param[in]     buflen
 *                   Length of Received buffer
 * @param[in]     flag
 *                   Flag mode
 *                   -  NONE_BLOCKING
 *                   -  BLOCKING
 * @return        Number of bytes received
 * @note          when using LPUART in BLOCKING mode,
 *                a time-out condition is used via defined symbol LPUART_BLOCKING_TIMEOUT.
 *//*-------------------------------------------------------------------------*/
uint32_t HAL_LPUART_Receive( uint8_t* rxbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag )
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
         timeOut = LPUART_BLOCKING_TIMEOUT;
         while( !( LPUART->IFSR & LPUART_IFSR_RXCIFLAG_Msk ) )
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
         ( *pChar++ ) = HAL_LPUART_ReceiveByte();

         // update counter
         bToRecv--;
         bRecv++;
      }
   }

   // Non-Blocking Mode
   else
   {
      while( bToRecv )
      {
         // if no data were received
         if( !( LPUART->IFSR & LPUART_IFSR_RXCIFLAG_Msk ) )
         {
            break;
         }

         // receive byte
         ( *pChar++ ) = HAL_LPUART_ReceiveByte();

         // update counter
         bRecv++;
         bToRecv--;
      }
   }

   // return
   return bRecv;
}


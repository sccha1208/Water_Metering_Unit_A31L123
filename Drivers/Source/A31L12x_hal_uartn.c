/**
 *******************************************************************************
 * @file        A31L12x_hal_uartn.c
 * @author      ABOV R&D Division
 * @brief       UART Serial Communication
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
#include "A31L12x_hal_uartn.h"

//******************************************************************************
// Variable
//******************************************************************************

static uint32_t      UARTn_BaseClock;

char                 InData[80];
int                  InFlag;
int                  InCount;

/* Public Functions --------------------------------------------------------- */
//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Determines best dividers to get a target clock rate
 * @param[in]     UARTx
 *                   Pointer to the target UART
 *                   -  UART0 ~ UART1
 * @param[in]     baudrate
 *                   Desired baudrate
 * @return        None
 *//*-------------------------------------------------------------------------*/
static void uart_set_divisors( UARTn_Type* UARTx, uint32_t baudrate )
{
   uint32_t    numerator;
   uint32_t    denominator;
   uint32_t    bdr, bfr;
   uint32_t    fd;

   //--------------------------------------
   // bdr = UARTn_BaseClock / (16 * baudrate) - 1
   //--------------------------------------
   numerator = UARTn_BaseClock;
   denominator = 16 * baudrate;
   bdr = numerator / denominator - 1;

   //--------------------------------------
   // fd = numerator - (bdr + 1) * denominator
   // bfr = (numerator / denominator - 1 - bdr) * 256
   //     = (numerator - (bdr + 1) * denominator) * 256 / denominator
   //     = fd * 256 / denominator
   //--------------------------------------
   fd = numerator - ( bdr + 1 ) * denominator;
   bfr = ( fd * 256 ) / denominator;
   UARTx->BDR = ( uint16_t )( bdr & 0xffff );
   UARTx->BFR = ( uint8_t )( bfr & 0xff );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Initialize the UARTn peripheral with the specified parameters.
 * @param[in]     UARTx
 *                   Pointer to the target UART
 *                   -  UART0 ~ UART1
 * @param[in]     UARTn_Config
 *                   Pointer to a UARTn_CFG_Type structure
 *                   that contains the configuration information for the specified UART peripheral.
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_UART_Init( UARTn_Type* UARTx, UARTn_CFG_Type* UARTn_Config )
{
   uint8_t     tmp;

   /* Check UART handle */
   if( UARTx == NULL )
   {
      return HAL_ERROR;
   }

#if 1    // supported
   if( UARTx == ( UARTn_Type* )UART0 )
   {
      /* Set up peripheral clock for UART0 module */
      HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_UT0CLKE, 1 );
   }
#endif

#if 1    // supported
   if( UARTx == ( UARTn_Type* )UART1 )
   {
      /* Set up peripheral clock for UART1 module */
      HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_UT1CLKE, PPxCLKE_Enable );
   }
#endif

   // Dummy reading
   while( UARTx->LSR & UARTn_LSR_RDR )
   {
      tmp = UARTx->RBR;
   }
   // Wait for current transmit complete
   while( !( UARTx->LSR & UARTn_LSR_THRE ) );

   // Disable interrupt
   UARTx->IER = 0;
   // Set LCR, DCR to default state
   UARTx->LCR = 0;
   UARTx->DCR = 0;
   // Dummy reading
   tmp = UARTx->LSR;
   tmp = UARTx->IIR;

   // uart clock set
   // SCU->MCCR7 &=0xffff0000;
   // SCU->MCCR7 |=((4<<8)|(2)); // MCLK
   UARTn_BaseClock = SystemPeriClock;

   // Set Line Control register ----------------------------
   uart_set_divisors( UARTx, ( UARTn_Config->Baudrate ) );

   tmp = ( UARTx->LCR & UARTn_LCR_BREAK_EN ) & UARTn_LCR_BITMASK;

   switch( UARTn_Config->Databits )
   {
      case UARTn_DATA_BIT_5:
         tmp |= UARTn_LCR_WLEN5;
         break;
      case UARTn_DATA_BIT_6:
         tmp |= UARTn_LCR_WLEN6;
         break;
      case UARTn_DATA_BIT_7:
         tmp |= UARTn_LCR_WLEN7;
         break;
      case UARTn_DATA_BIT_8:
      default:
         tmp |= UARTn_LCR_WLEN8;
         break;
   }

   if( UARTn_Config->Parity == UARTn_PARITY_BIT_NONE )
   {
      // Do nothing...
   }
   else
   {
      tmp |= UARTn_LCR_PARITY_EN;
      switch( UARTn_Config->Parity )
      {
         case UARTn_PARITY_BIT_ODD:
            tmp |= UARTn_LCR_PARITY_ODD;
            break;

         case UARTn_PARITY_BIT_EVEN:
            tmp |= UARTn_LCR_PARITY_EVEN;
            break;

         case UARTn_PARITY_BIT_SP_1:
            tmp |= UARTn_LCR_PARITY_F_1;
            break;

         case UARTn_PARITY_BIT_SP_0:
            tmp |= UARTn_LCR_PARITY_F_0;
            break;
         default:
            break;
      }
   }

   switch( UARTn_Config->Stopbits )
   {
      case UARTn_STOP_BIT_2:
         tmp |= UARTn_LCR_STOPBIT_SEL;
         break;
      case UARTn_STOP_BIT_1:
      default:
         // Do no thing
         break;
   }

   UARTx->LCR = ( uint8_t )( tmp & UARTn_LCR_BITMASK );

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Deinitialize the UARTn peripheral registers to their default reset values.
 * @param[in]     UARTx
 *                   Pointer to the target UART
 *                   -  UART0 ~ UART1
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_UART_DeInit( UARTn_Type* UARTx )
{
   /* Check UART handle */
   if( UARTx == NULL )
   {
      return HAL_ERROR;
   }

#if 1    // supported
   if( UARTx == ( UARTn_Type* )UART0 )
   {
      /* Set up peripheral clock for UART0 module */
      HAL_SCU_Peripheral_SetReset2( 1 << 2 );
      HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_UT0CLKE, PPxCLKE_Disable );
   }
#endif

#if 1    // supported
   if( UARTx == ( UARTn_Type* )UART1 )
   {
      /* Set up peripheral clock for UART1 module */
      HAL_SCU_Peripheral_SetReset2( 1 << 3 );
      HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_UT1CLKE, PPxCLKE_Disable );
   }
#endif

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Fills each UARTn_Config member with its default value:
 *                   -  38400 bps
 *                   -  8 Data Bit
 *                   -  No Parity Bit
 *                   -  1 Stop Bit
 * @param[out]    UARTn_Config
 *                   Pointer to a UARTn_CFG_Type structure which will be initialized.
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_UART_ConfigStructInit( UARTn_CFG_Type* UARTn_Config )
{
   /* Check UARTn_Config */
   if( UARTn_Config == NULL )
   {
      return HAL_ERROR;
   }

   UARTn_Config->Baudrate = 38400;
   UARTn_Config->Databits = UARTn_DATA_BIT_8;
   UARTn_Config->Parity = UARTn_PARITY_BIT_NONE;
   UARTn_Config->Stopbits = UARTn_STOP_BIT_1;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure the peripheral interrupt.
 * @param[in]     UARTx
 *                   Pointer to the target UART
 *                   -  UART0 ~ UART1
 * @param[in]     UARTn_IntCfg
 *                   Specifies the interrupt flag
 *                   -  UARTn_INTCFG_RBR:    RBR Interrupt enable
 *                   -  UARTn_INTCFG_THRE:   THR Interrupt enable
 *                   -  UARTn_INTCFG_RLS:    RX line status interrupt enable
 *                   -  UARTn_INTCFG_DRX:    THR Interrupt enable
 *                   -  UARTn_INTCFG_DTX:    RX line status interrupt enable
 * @param[in]     NewState
 *                   Next State of Interrupt Operation
 *                   -  ENABLE, DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_UART_ConfigInterrupt( UARTn_Type* UARTx, UARTn_INT_Type UARTn_IntCfg, FunctionalState NewState )
{
   uint32_t    tmp;

   /* Check UART handle */
   if( UARTx == NULL )
   {
      return HAL_ERROR;
   }

   switch( UARTn_IntCfg )
   {
      case UARTn_INTCFG_RBR:
         tmp = UARTn_IER_RBRINT_EN;
         break;
      case UARTn_INTCFG_THRE:
         tmp = UARTn_IER_THREINT_EN;
         break;
      case UARTn_INTCFG_RLS:
         tmp = UARTn_IER_RLSINT_EN;
         break;
      case UARTn_INTCFG_TXE:
         tmp = UARTn_IER_TXE_EN;
         break;
   }

   if( NewState == ENABLE )
   {
      UARTx->IER |= tmp;
   }
   else
   {
      UARTx->IER &= ( ~tmp ) & UARTn_IER_BITMASK;
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure Data Control mode for UART peripheral
 * @param[in]     UARTx
 *                   Pointer to the target UART
 * @param[in]     Mode
 *                   Data Control Mode
 *                   -  UARTn_DATA_CONTROL_LOOPBACK:  Loop back mode.
 *                   -  UARTn_DATA_CONTROL_RXINV:     RX Data inversion mode.
 *                   -  UARTn_DATA_CONTROL_TXINV:     TX Data inversion mode.
 *                   -  UARTn_DATA_CONTROL_RTXINV:    TX RX Data inversion mode.
 * @param[in]     NewState
 *                   Next State of Functional Operation
 *                   -  ENABLE, DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_UART_DataControlConfig( UARTn_Type* UARTx, UARTn_DATA_CONTROL_Type Mode, FunctionalState NewState )
{
   uint8_t     tmp;

   /* Check UART handle */
   if( UARTx == NULL )
   {
      return HAL_ERROR;
   }

   switch( Mode )
   {
      case UARTn_DATA_CONTROL_LOOPBACK:
         tmp = UARTn_DCR_LBON;
         break;
      case UARTn_DATA_CONTROL_RXINV:
         tmp = UARTn_DCR_RXINV;
         break;
      case UARTn_DATA_CONTROL_TXINV:
         tmp = UARTn_DCR_TXINV;
         break;
      case UARTn_DATA_CONTROL_RTXINV:
         tmp = UARTn_DCR_RXINV | UARTn_DCR_TXINV;
         break;
      default:
         break;
   }

   if( NewState == ENABLE )
   {
      UARTx->DCR |= tmp;
   }
   else
   {
      UARTx->DCR &= ( ~tmp ) & UARTn_DCR_BITMASK;
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure inter-frame delay time for UART peripheral
 * @param[in]     UARTx
 *                   Pointer to the target UART
 * @param[in]     waitval
 *                   inter-frame delay time: 1 bit time  unit (0~7)
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_UART_IFDelayConfig( UARTn_Type* UARTx, uint8_t waitval )
{
   /* Check UART handle */
   if( UARTx == NULL )
   {
      return HAL_ERROR;
   }

   if( waitval < 8 )
   {
      UARTx->IDTR = waitval;
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Force BREAK character on UART line, output pin UARTn TXD is
 *                   forced to logic 0.
 * @param[in]     UARTx
 *                   Pointer to the target UART
 *                   -  UART0 ~ UART1
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_UART_ForceBreak( UARTn_Type* UARTx )
{
   /* Check UART handle */
   if( UARTx == NULL )
   {
      return HAL_ERROR;
   }

   UARTx->LCR |= UARTn_LCR_BREAK_EN;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         This function returns the current value of Line Status Register.
 * @param[in]     UARTx
 *                   Pointer to the target UART
 *                   -  UART0 ~ UART1
 * @return        Current value of Line Status register in UART peripheral.
 * @note          The return value of this function must be ANDed with each member in
 *                UART_LS_Type enumeration to determine current flag status
 *                corresponding to each Line status type. Because some flags in
 *                Line Status register will be cleared after reading, the next reading
 *                Line Status register could not be correct. So this function used to
 *                read Line status register in one time only, then the return value
 *                used to check all flags.
 *//*-------------------------------------------------------------------------*/
uint8_t HAL_UART_GetLineStatus( UARTn_Type* UARTx )
{
   return ( ( UARTx->LSR ) & UARTn_LSR_BITMASK );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Check whether if UART is busy or not
 * @param[in]     UARTx
 *                   Pointer to the target UART
 *                   -  UART0 ~ UART1
 * @return        RESET if UART is not busy, otherwise return SET.
 *//*-------------------------------------------------------------------------*/
FlagStatus HAL_UART_CheckBusy( UARTn_Type* UARTx )
{
   if( UARTx->LSR & UARTn_LSR_TEMT )
   {
      return RESET;
   }
   else
   {
      return SET;
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Transmit a single data through UART peripheral
 * @param[in]     UARTx
 *                   Pointer to the target UART
 *                   -  UART0 ~ UART1
 * @param[in]     Data
 *                   Data to transmit (must be 8-bit long)
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_UART_TransmitByte( UARTn_Type* UARTx, uint8_t Data )
{
   /* Check UART handle */
   if( UARTx == NULL )
   {
      return HAL_ERROR;
   }

   UARTx->THR = Data;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Receive a single data from UART peripheral
 * @param[in]     UARTx
 *                   Pointer to the target UART
 *                   -  UART0 ~ UART1
 * @return        Data received
 *//*-------------------------------------------------------------------------*/
uint8_t HAL_UART_ReceiveByte( UARTn_Type* UARTx )
{
   return UARTx->RBR;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Send a block of data via UART peripheral
 * @param[in]     UARTx
 *                   Pointer to the target UART
 *                   -  UART0 ~ UART1
 * @param[in]     txbuf
 *                   Pointer to Transmit buffer
 * @param[in]     buflen
 *                   Length of Transmit buffer
 * @param[in]     flag
 *                   Flag used in  UART transfer
 *                   -  NONE_BLOCKING
 *                   -  BLOCKING
 * @return        Number of bytes sent.
 * @note          when using UART in BLOCKING mode,
 *                a time-out condition is used via defined symbol UARTn_BLOCKING_TIMEOUT.
 *//*-------------------------------------------------------------------------*/
uint32_t HAL_UART_Transmit( UARTn_Type* UARTx, uint8_t* txbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag )
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
         // wait until tx data register is empty with timeout
         timeOut = UARTn_BLOCKING_TIMEOUT;
         while( !( UARTx->LSR & UARTn_LSR_THRE ) )
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

         // send byte
         HAL_UART_TransmitByte( UARTx, ( *pChar++ ) );

         // update counter
         bToSend--;
         bSent++;
      }

      // wait until previous transmission is complete
      while( UARTx->LSR_b.TEMT == 0 );    // Polling Only
   }

   // Non-Blocking Mode
   else
   {
      while( bToSend )
      {
         // if tx data register is not empty
         if( !( UARTx->LSR & UARTn_LSR_THRE ) )
         {
            break;
         }

         // send byte
         HAL_UART_TransmitByte( UARTx, ( *pChar++ ) );

         // update counter
         bToSend--;
         bSent++;
      }
   }

   // return
   return bSent;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Receive a block of data via UART peripheral
 * @param[in]     UARTx
 *                   Pointer to the target UART
 *                   -  UART0 ~ UART1
 * @param[out]    rxbuf
 *                   Pointer to Received buffer
 * @param[in]     buflen
 *                   Length of Received buffer
 * @param[in]     flag
 *                   Flag mode
 *                   -  NONE_BLOCKING
 *                   -  BLOCKING
 * @return        Number of bytes received
 * @note          when using UART in BLOCKING mode,
 *                a time-out condition is used via defined symbol UARTn_BLOCKING_TIMEOUT.
 *//*-------------------------------------------------------------------------*/
uint32_t HAL_UART_Receive( UARTn_Type* UARTx, uint8_t* rxbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag )
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
         timeOut = UARTn_BLOCKING_TIMEOUT;
         while( !( UARTx->LSR & UARTn_LSR_RDR ) )
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
         ( *pChar++ ) = HAL_UART_ReceiveByte( UARTx );

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
         if( !( UARTx->LSR & UARTn_LSR_RDR ) )
         {
            break;
         }

         // receive byte
         ( *pChar++ ) = HAL_UART_ReceiveByte( UARTx );

         // update counter
         bToRecv--;
         bRecv++;
      }
   }

   // return
   return bRecv;
}


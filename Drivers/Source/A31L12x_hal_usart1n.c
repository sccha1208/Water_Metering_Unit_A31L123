/**
 *******************************************************************************
 * @file        A31L12x_hal_usart1n.c
 * @author      ABOV R&D Division
 * @brief       USART1 serial communication
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
#include "A31L12x_hal_usart1n.h"

//******************************************************************************
// Variable
//******************************************************************************

uint32_t    USART1n_BaseClock;

/* Public Functions --------------------------------------------------------- */
//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Determines best dividers to get a target clock rate
 * @param[in]     USART1x
 *                   Pointer to the target USART1
 *                   -  USART10
 * @param[in]     mode
 *                   -  USART1n_UART_MODE
 *                   -  USART1n_USRT_MODE
 *                   -  USART1n_SPI_MODE
 * @param[in]     baudrate
 *                   Desired baudrate
 * @return        None
 *//*-------------------------------------------------------------------------*/
static void usart_set_divisors( USART1n_Type* USART1x, uint32_t mode, uint32_t baudrate )
{
   uint32_t    numerator;
   uint32_t    denominator;
   uint32_t    n;
   uint32_t    bdr;

   if( mode == USART1n_UART_MODE )
   {
      // baudrate = PCLK / (16 * (bdr + 1))
      // bdr = (PCLK / (16 * baudrate)) - 1
      n = 16;
   }
   else
   {
      // baudrate = PCLK / (2 * (bdr + 1))
      // bdr = (PCLK / (2 * baudrate)) - 1
      n = 2;
   }

   //--------------------------------------
   // numerator & denominator
   //
   // bdr = USART1n_BaseClock / n / baudrate  - 1
   //--------------------------------------
   numerator = USART1n_BaseClock;
   denominator = baudrate;

   bdr = numerator / n / denominator - 1;

   USART1x->BDR = ( uint16_t )( bdr & 0xffff );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Initialize the USART1n peripheral with the specified parameters.
 * @param[in]     USART1x
 *                   Pointer to the target USART1
 *                   -  USART10
 * @param[in]     USART1n_Config
 *                   Pointer to a USART1n_CFG_Type structure
 *                   that contains the configuration information for the specified peripheral.
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_USART_Init( USART1n_Type* USART1x, USART1n_CFG_Type* USART1n_Config )
{
   uint32_t    tmp;

   /* Check USART handle */
   if( USART1x == NULL )
   {
      return HAL_ERROR;
   }

#if 1    // supported
   if( USART1x == ( USART1n_Type* )USART10 )
   {
      // Set up peripheral clock for USART10 module
      HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_UST10CLKE, PPxCLKE_Enable );
      HAL_SCU_Peripheral_SetReset2( 1 << 0 );
   }
#endif

#if 0    // not supported
   if( USART1x == ( USART1n_Type* )USART11 )
   {
      // Set up peripheral clock for USART11 module
      HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_UST11CLKE, PPxCLKE_Enable );
      HAL_SCU_Peripheral_SetReset2( 1 << 1 );
   }
#endif

#if 0    // not supported
   if( USART1x == ( USART1n_Type* )USART12 )
   {
      // Set up peripheral clock for USART12 module
      HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_UST12CLKE, PPxCLKE_Enable );
      HAL_SCU_Peripheral_SetReset2( 1 << 4 );
   }
#endif

#if 0    // not supported
   if( USART1x == ( USART1n_Type* )USART13 )
   {
      // Set up peripheral clock for USART13 module
      HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_UST13CLKE, PPxCLKE_Enable );
      HAL_SCU_Peripheral_SetReset2( 1 << 5 );
   }
#endif

   USART1n_BaseClock = SystemPeriClock;

   usart_set_divisors( USART1x, USART1n_Config->Mode, USART1n_Config->Baudrate );

   tmp = 0
         | ( ( USART1n_Config->Mode & 0x3 ) << USART1n_CR1_USTnMS_Pos )
         | ( ( USART1n_Config->Parity & 0x3 ) << USART1n_CR1_USTnP_Pos )
         | ( ( USART1n_Config->Databits & 0x7 ) << USART1n_CR1_USTnS_Pos )
         | ( ( USART1n_Config->Order & 0x1 ) << USART1n_CR1_ORDn_Pos )
         | ( ( USART1n_Config->ACK & 0x1 ) << USART1n_CR1_CPOLn_Pos )
         | ( ( USART1n_Config->Edge & 0x3 )  << USART1n_CR1_CPHAn_Pos )
         | ( 1 << USART1n_CR1_TXEn_Pos )  // Tx Enable
         | ( 1 << USART1n_CR1_RXEn_Pos )  // Rx Enable
         ;

   USART1x->CR1 = tmp;

   USART1x->CR2 &= ~( 1 << USART1n_CR2_USTnSB_Pos );     // USTnSB reset
   USART1x->CR2 |= ( ( USART1n_Config->Stopbits & 0x1 ) << USART1n_CR2_USTnSB_Pos );    // USTnSB
   USART1x->CR2 &= ~( 1 << USART1n_CR2_FXCHn_Pos );      // FXCHn reset
   //USART1x->CR2 |= (1<<USART1n_CR2_FXCHn_Pos);        // FXCHn

   HAL_USART_ClearStatus( USART1x, USART1n_STATUS_TXC );

   // dummy read
   HAL_USART_ReceiveByte( USART1x );
   HAL_USART_ReceiveByte( USART1x );

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Deinitialize the USART1n peripheral registers
 *                to their default reset values.
 * @param[in]     USART1x
 *                   Pointer to the target USART1
 *                   -  USART10
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_USART_DeInit( USART1n_Type* USART1x )
{
   /* Check USART handle */
   if( USART1x == NULL )
   {
      return HAL_ERROR;
   }

#if 1    // supported
   if( USART1x == ( USART1n_Type* )USART10 )
   {
      // Set up peripheral clock for USART10 module
      HAL_SCU_Peripheral_SetReset2( 1 << 0 );
      HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_UST10CLKE, PPxCLKE_Disable );
   }
#endif

#if 0    // not supported
   if( USART1x == ( USART1n_Type* )USART11 )
   {
      // Set up peripheral clock for USART11 module
      HAL_SCU_Peripheral_SetReset2( 1 << 1 );
      HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_UST11CLKE, PPxCLKE_Disable );
   }
#endif

#if 0    // not supported
   if( USART1x == ( USART1n_Type* )USART12 )
   {
      // Set up peripheral clock for USART12 module
      HAL_SCU_Peripheral_SetReset2( 1 << 4 );
      HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_UST12CLKE, PPxCLKE_Disable );
   }
#endif

#if 0    // not supported
   if( USART1x == ( USART1n_Type* )USART13 )
   {
      // Set up peripheral clock for USART13 module
      HAL_SCU_Peripheral_SetReset2( 1 << 5 );
      HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_UST13CLKE, PPxCLKE_Disable );
   }
#endif

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Fills each USART1n_Config member with its default value:
 *                   -  38400 bps
 *                   -  8 Data Bit
 *                   -  No Parity Bit
 *                   -  1 Stop Bit
 * @param[out]    USART1n_Config
 *                   Pointer to a USART1n_CFG_Type structure which will be initialized.
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_USART_UART_Mode_Config( USART1n_CFG_Type* USART1n_Config )
{
   /* Check USART1n_Config */
   if( USART1n_Config == NULL )
   {
      return HAL_ERROR;
   }

   USART1n_Config->Mode = USART1n_UART_MODE;
   USART1n_Config->Baudrate = 38400;
   USART1n_Config->Databits = USART1n_DATA_BIT_8;
   USART1n_Config->Parity = USART1n_PARITY_BIT_NONE;
   USART1n_Config->Stopbits = USART1n_STOP_BIT_1;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Fills each USART1n_Config member with its default value:
 *                   -  38400 bps
 *                   -  8 Data Bit
 *                   -  No Parity Bit
 *                   -  1 Stop Bit
 * @param[out]    USART1n_Config
 *                   Pointer to a USART1n_CFG_Type structure which will be initialized.
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_USART_USRT_Mode_Config( USART1n_CFG_Type* USART1n_Config )
{
   /* Check USART1n_Config */
   if( USART1n_Config == NULL )
   {
      return HAL_ERROR;
   }

   USART1n_Config->Mode = USART1n_USRT_MODE;
   USART1n_Config->Baudrate = 38400;
   USART1n_Config->Databits = USART1n_DATA_BIT_8;
   USART1n_Config->Parity = USART1n_PARITY_BIT_NONE;
   USART1n_Config->Stopbits = USART1n_STOP_BIT_1;

   // only SPI & Sync. Mode
   USART1n_Config->Order = USART1n_SPI_LSB;

#if 0 // CPOLn : 0, CPHAn : 0 (X)
   USART1n_Config->ACK = USART1n_SPI_TX_RISING;
   USART1n_Config->Edge = USART1n_SPI_TX_LEADEDGE_SAMPLE;
#endif

#if 1 // CPOLn : 0, CPHAn : 1 (O)
   USART1n_Config->ACK = USART1n_SPI_TX_RISING;
   USART1n_Config->Edge = USART1n_SPI_TX_LEADEDGE_SETUP;
#endif

#if 0 // CPOLn : 1, CPHAn : 0 (X)
   USART1n_Config->ACK = USART1n_SPI_TX_FALLING;
   USART1n_Config->Edge = USART1n_SPI_TX_LEADEDGE_SAMPLE;
#endif

#if 0 // CPOLn : 1, CPHAn : 1 (O)
   USART1n_Config->ACK = USART1n_SPI_TX_FALLING;
   USART1n_Config->Edge = USART1n_SPI_TX_LEADEDGE_SETUP;
#endif

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Fills each USART1n_Config member with its default value:
 *                   -  38400 bps
 *                   -  8 Data Bit
 *                   -  No Parity Bit
 *                   -  1 Stop Bit
 * @param[out]    USART1n_Config
 *                   Pointer to a USART1n_CFG_Type structure which will be initialized.
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_USART_SPI_Mode_Config( USART1n_CFG_Type* USART1n_Config )
{
   /* Check USART1n_Config */
   if( USART1n_Config == NULL )
   {
      return HAL_ERROR;
   }

   USART1n_Config->Mode = USART1n_SPI_MODE;
   USART1n_Config->Baudrate = 38400;
   USART1n_Config->Databits = USART1n_DATA_BIT_8;
   USART1n_Config->Parity = USART1n_PARITY_BIT_NONE;
   USART1n_Config->Stopbits = USART1n_STOP_BIT_1;

   // only SPI & Sync. Mode
   USART1n_Config->Order = USART1n_SPI_LSB;

#if 1 // CPOLn : 0, CPHAn : 0 (X)
   USART1n_Config->ACK = USART1n_SPI_TX_RISING;
   USART1n_Config->Edge = USART1n_SPI_TX_LEADEDGE_SAMPLE;
#endif

#if 0 // CPOLn : 0, CPHAn : 1 (O)
   USART1n_Config->ACK = USART1n_SPI_TX_RISING;
   USART1n_Config->Edge = USART1n_SPI_TX_LEADEDGE_SETUP;
#endif

#if 0 // CPOLn : 1, CPHAn : 0 (X)
   USART1n_Config->ACK = USART1n_SPI_TX_FALLING;
   USART1n_Config->Edge = USART1n_SPI_TX_LEADEDGE_SAMPLE;
#endif

#if 0 // CPOLn : 1, CPHAn : 1 (O)
   USART1n_Config->ACK = USART1n_SPI_TX_FALLING;
   USART1n_Config->Edge = USART1n_SPI_TX_LEADEDGE_SETUP;
#endif

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure the interrupt source of selected USART1n peripheral
 * @param[in]     USART1x
 *                   Pointer to the target USART1
 *                   -  USART10
 * @param[in]     USART1n_IntCfg
 *                   Specifies the interrupt source
 *                   -  USART1n_INTCFG_WAKE
 *                   -  USART1n_INTCFG_RXC
 *                   -  USART1n_INTCFG_TXC
 *                   -  USART1n_INTCFG_DR
 *                   -  USART1n_INTCFG_RCD
 *                   -  USART1n_INTCFG_RTO
 * @param[in]     NewState
 *                   Next State of Interrupt Operation
 *                   -  ENABLE, DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_USART_ConfigInterrupt( USART1n_Type* USART1x, USART1n_INT_Type USART1n_IntCfg, FunctionalState NewState )
{
   uint32_t    tmp = 0;

   /* Check USART handle */
   if( USART1x == NULL )
   {
      return HAL_ERROR;
   }

   if( USART1n_IntCfg <= USART1n_INTCFG_DR )
   {
      // get mask
      switch( USART1n_IntCfg )
      {
         case USART1n_INTCFG_WAKE:
            tmp = USART1n_CR1_WAKEIEn_Msk;
            break;
         case USART1n_INTCFG_RXC:
            tmp = USART1n_CR1_RXCIEn_Msk;
            break;
         case USART1n_INTCFG_TXC:
            tmp = USART1n_CR1_TXCIEn_Msk;
            break;
         case USART1n_INTCFG_DR:
            tmp = USART1n_CR1_DRIEn_Msk;
            break;
      }

      // enable/disable
      if( NewState == ENABLE )
      {
         USART1x->CR1 |= tmp;
      }
      else
      {
         USART1x->CR1 &= ~tmp;
      }
   }

   if( USART1n_IntCfg >= USART1n_INTCFG_RCD )
   {
      // get mask
      switch( USART1n_IntCfg )
      {
         case USART1n_INTCFG_RCD:
            tmp = USART1n_CR3_RCDIEn_Msk;
            break;
         case USART1n_INTCFG_RTO:
            tmp = USART1n_CR3_RTOIEn_Msk;
            break;
      }

      // enable/disable
      if( NewState == ENABLE )
      {
         USART1x->CR3 |= tmp;
      }
      else
      {
         USART1x->CR3 &= ~tmp;
      }
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure Data Control mode for USART peripheral
 * @param[in]     USART1x
 *                   Pointer to the target USART1
 *                   -  USART10
 * @param[in]     Mode
 *                   Data Control Mode
 *                   -  USART1n_CONTROL_USTEN:     Activate USARTn Block by supplying.
 *                   -  USART1n_CONTROL_DBLS:      Selects receiver sampling rate. (only UART mode)
 *                   -  USART1n_CONTROL_MASTER:    Selects master or slave in SPIn or Synchronous mode and controls the direction of SCKn pin.
 *                   -  USART1n_CONTROL_LOOPS:     Control the Loop Back mode of USARTn for test mode.
 *                   -  USART1n_CONTROL_DISSCK:    In synchronous mode operation, selects the waveform of SCKn output.
 *                   -  USART1n_CONTROL_USTSSEN:   This bit controls the SSn pin operation. (only SPI mode)
 *                   -  USART1n_CONTROL_FXCH:      SPIn port function exchange control bit. (only SPI mode)
 *                   -  USART1n_CONTROL_USTSB:     Selects the length of stop bit in Asynchronous or Synchronous mode.
 *                   -  USART1n_CONTROL_USTTX8:    The ninth bit of data frame in Asynchronous or Synchronous mode of operation. Write this bit first before loading the USARTn_DR register.
 *                   -  USART1n_CONTROL_USTRX8:    The ninth bit of data frame in Asynchronous or Synchronous mode of operation. Read this bit first before reading the receive buffer (only UART mode)
 *                   -  USART1n_CONTROL_RCDEN:     Receive Character Detection Function Enable
 *                   -  USART1n_CONTROL_RTOEN:     Receive Time Out Function Enable
 * @param[in]     NewState
 *                   Next State of Functional Operation
 *                   -  ENABLE, DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_USART_DataControlConfig( USART1n_Type* USART1x, USART1n_CONTROL_Type Mode, FunctionalState NewState )
{
   uint16_t    tmp = 0;

   /* Check USART handle */
   if( USART1x == NULL )
   {
      return HAL_ERROR;
   }

   if( Mode <= USART1n_CONTROL_USTEN )
   {
      switch( Mode )
      {
         case USART1n_CONTROL_USTRX8:
            tmp = USART1n_CR2_USTnRX8;
            break;
         case USART1n_CONTROL_USTTX8:
            tmp = USART1n_CR2_USTnTX8;
            break;
         case USART1n_CONTROL_USTSB:
            tmp = USART1n_CR2_USTnSB;
            break;
         case USART1n_CONTROL_FXCH:
            tmp = USART1n_CR2_FXCHn;
            break;
         case USART1n_CONTROL_USTSSEN:
            tmp = USART1n_CR2_USTnSSEN;
            break;
         case USART1n_CONTROL_DISSCK:
            tmp = USART1n_CR2_DISSCKn;
            break;
         case USART1n_CONTROL_LOOPS:
            tmp = USART1n_CR2_LOOPSn;
            break;
         case USART1n_CONTROL_MASTER:
            tmp = USART1n_CR2_MASTERn;
            break;
         case USART1n_CONTROL_DBLS:
            tmp = USART1n_CR2_DBLSn;
            break;
         case USART1n_CONTROL_USTEN:
            tmp = USART1n_CR2_USTnEN;
            break;
      }

      if( NewState == ENABLE )
      {
         USART1x->CR2 |= tmp;
      }
      else
      {
         USART1x->CR2 &= ~( tmp & USART1n_CR2_BITMASK );
      }
   }

   if( Mode >= USART1n_CONTROL_RCDEN )
   {
      switch( Mode )
      {
         case USART1n_CONTROL_RCDEN:
            tmp = USART1n_CR3_RCDIEn_Msk;
            break;
         case USART1n_CONTROL_RTOEN:
            tmp = USART1n_CR3_RTOENn_Msk;
            break;
      }

      if( NewState == ENABLE )
      {
         USART1x->CR3 |= tmp;
      }
      else
      {
         USART1x->CR3 &= ~tmp;
      }
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         USART1n enable control
 * @param[in]     USART1x
 *                   Pointer to the target USART1
 *                   -  USART10
 * @param[in]     state
 *                   -  ENABLE, DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_USART_Enable( USART1n_Type* USART1x, FunctionalState state )
{
   /* Check USART handle */
   if( USART1x == NULL )
   {
      return HAL_ERROR;
   }

   if( state == ENABLE )
   {
      USART1x->CR2 |= ( 1 << USART1n_CR2_USTnEN_Pos );    // USTnEN
   }
   else
   {
      USART1x->CR2 &= ~( 1 << USART1n_CR2_USTnEN_Pos );   // USTnEN
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Clear Status register in USART peripheral.
 * @param[in]     USART1x
 *                   Pointer to the target USART1
 *                   -  USART10
 * @param[in]     Status
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_USART_ClearStatus( USART1n_Type* USART1x, USART1n_STATUS_Type Status )
{
   uint32_t    tmp;

   /* Check USART handle */
   if( USART1x == NULL )
   {
      return HAL_ERROR;
   }

   if( Status <= USART1n_STATUS_DRE )
   {
      switch( Status )
      {
         case USART1n_STATUS_WAKE:
            tmp = USART1n_SR_WAKE;
            break;
         case USART1n_STATUS_RXC:
            tmp = USART1n_SR_RXC;
            break;
         case USART1n_STATUS_TXC:
            tmp = USART1n_SR_TXC;
            break;
         case USART1n_STATUS_DRE:
            tmp = USART1n_SR_DRE;
            break;
         default:
            return HAL_ERROR;
      }

      USART1x->ST = tmp;
   }

   if( Status >= USART1n_STATUS_RCD )
   {
      switch( Status )
      {
         case USART1n_STATUS_RCD:
            tmp = USART1n_CR3_RCDnIFLAG_Msk;
            break;
         case USART1n_STATUS_RTO:
            tmp = USART1n_CR3_RTOnIFLAG_Msk;
            break;
         default:
            return HAL_ERROR;
      }

      USART1x->CR3 = tmp;
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Get current value of Line Status register in USART peripheral.
 * @param[in]     USART1x
 *                   Pointer to the target USART1
 *                   -  USART10
 * @return        Current value of Status register in USART peripheral.
 *//*-------------------------------------------------------------------------*/
uint8_t HAL_USART_GetStatus( USART1n_Type* USART1x )
{
   return ( ( USART1x->ST ) & USART1n_SR_BITMASK );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Get the RTO/RCD interrupt flag of selected USART1n peripheral
 * @param[in]     USART1x
 *                   Pointer to the target USART1
 *                   -  USART10
 * @return        Current RTO/RCD interrupt flag
 *//*-------------------------------------------------------------------------*/
uint8_t HAL_USART_GetStatusRTORCD( USART1n_Type* USART1x )
{
   return ( USART1x->CR3 & ( USART1n_CR3_RCDnIFLAG_Msk | USART1n_CR3_RTOnIFLAG_Msk ) );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Check whether if USART is busy or not
 * @param[in]     USART1x
 *                   Pointer to the target USART1
 *                   -  USART10
 * @return        RESET if USART is not busy, otherwise return SET.
 *//*-------------------------------------------------------------------------*/
FlagStatus HAL_USART_CheckBusy( USART1n_Type* USART1x )
{
   if( USART1x->ST & USART1n_SR_DRE )
   {
      return RESET;
   }
   else
   {
      return SET;
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Transmit a single data through USART peripheral
 * @param[in]     USART1x
 *                   Pointer to the target USART1
 *                   -  USART10
 * @param[in]     Data
 *                   Data to transmit (must be 8-bit long)
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_USART_TransmitByte( USART1n_Type* USART1x, uint8_t Data )
{
   /* Check USART handle */
   if( USART1x == NULL )
   {
      return HAL_ERROR;
   }

   USART1x->TDR = Data;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Receive a single data from USART peripheral
 * @param[in]     USART1x
 *                   Pointer to the target USART1
 *                   -  USART10
 * @return        Data     received
 *//*-------------------------------------------------------------------------*/
uint8_t HAL_USART_ReceiveByte( USART1n_Type* USART1x )
{
   return USART1x->RDR;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Send a block of data via USART peripheral
 * @param[in]     USART1x
 *                   Pointer to the target USART1
 *                   -  USART10
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
 *                a time-out condition is used via defined symbol USART_BLOCKING_TIMEOUT.
 *//*-------------------------------------------------------------------------*/
uint32_t HAL_USART_Transmit( USART1n_Type* USART1x, uint8_t* txbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag )
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
         HAL_USART_TransmitByte( USART1x, ( *pChar++ ) );

         // wait until tx data register is empty with timeout
         timeOut = USART1n_BLOCKING_TIMEOUT;
         while( !( USART1x->ST & USART1n_SR_TXC ) )
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
         HAL_USART_ClearStatus( USART1x, USART1n_STATUS_TXC );

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
         if( !( USART1x->ST & USART1n_SR_DRE ) )
         {
            break;
         }

         // send byte
         HAL_USART_TransmitByte( USART1x, ( *pChar++ ) );

         // update counter
         bToSend--;
         bSent++;
      }
   }

   // return
   return bSent;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Receive a block of data via USART peripheral
 * @param[in]     USART1x
 *                   Pointer to the target USART1
 *                   -  USART10
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
uint32_t HAL_USART_Receive( USART1n_Type* USART1x, uint8_t* rxbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag )
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
         timeOut = USART1n_BLOCKING_TIMEOUT;
         while( !( USART1x->ST & USART1n_SR_RXC ) )
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
         ( *pChar++ ) = HAL_USART_ReceiveByte( USART1x );

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
         if( !( USART1x->ST & USART1n_SR_RXC ) )
         {
            break;
         }

         // receive byte
         ( *pChar++ ) = HAL_USART_ReceiveByte( USART1x );

         // update counter
         bRecv++;
         bToRecv--;
      }
   }

   // return
   return bRecv;
}


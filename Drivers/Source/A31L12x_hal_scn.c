/**
 *******************************************************************************
 * @file        A31L12x_hal_scn.c
 * @author      ABOV R&D Division
 * @brief       SC communication
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

#include "A31L12x_hal_scn.h"
#include "A31L12x_hal_scu.h"

//******************************************************************************
// Variable
//******************************************************************************

uint32_t    SCn_BaseClock;

/* Public Functions --------------------------------------------------------- */
//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Determines best dividers to get a target clock rate
 * @param[in]     SCx
 *                   Pointer to the target SC
 *                   -  SC0 ~ SC1
 * @param[in]     mode
 *                   -  SCn_SCI_MODE
 *                   -  SCn_UART_MODE
 * @param[in]     value
                     divider to generate smartcard clock
 * @param[in]     baudrate
 *                   Desired baudrate
 * @return        None
 *//*-------------------------------------------------------------------------*/
static void sc_set_divisors( SCn_Type* SCx, uint32_t mode, uint32_t value, uint32_t baudrate, uint32_t ovrsamp )
{
   uint32_t    numerator;
   uint32_t    denominator;
   uint32_t    n;
   uint32_t    bdr;

   if( ( mode == SCn_SCI_MODE ) && ( value != 0 ) )
   {
      n = value * 2 * 16;     // bdr = (PCLK / (value * 2 * 16 * baudrate)) - 1
   }
   else
   {
      n = 16;                 // bdr = (PCLK / (16 * baudrate)) - 1
   }

   if( ovrsamp == SCn_OVR_SAMP_8 )
   {
      n >>= 1;
   }

   //--------------------------------------
   // numerator & denominator
   //
   // bdr = SCn_BaseClock / n / baudrate  - 1
   //--------------------------------------
   numerator = SCn_BaseClock;
   denominator = baudrate;

   bdr = numerator / n / denominator - 1;

   SCx->BDR =  bdr & 0x0000ffff;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Initialize the SCn peripheral with the specified parameters.
 * @param[in]     SCx
 *                   Pointer to the target SC
 *                   -  SC0 ~ SC1
 * @param[in]     SCx_ConfigStruct
 *                   Pointer to a SCn_CFG_Type structure
 *                   that contains the configuration information for the specified peripheral.
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_SC_Init( SCn_Type* SCx, SCn_CFG_Type* SCx_ConfigStruct )
{
   uint32_t    tmp;

   /* Check SC handle */
   if( SCx == NULL )
   {
      return HAL_ERROR;
   }

#if 1    // supported
   if( SCx == ( SCn_Type* )SC0 )
   {
      // Set up peripheral clock for SC0 module
      HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_SC0CLKE, PPxCLKE_Enable );
      HAL_SCU_Peripheral_SetReset2( PPRST2_SC0RST );
   }
#endif

#if 1    // supported
   if( SCx == ( SCn_Type* )SC1 )
   {
      // Set up peripheral clock for SC1 module
      HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_SC1CLKE, PPxCLKE_Enable );
      HAL_SCU_Peripheral_SetReset2( PPRST2_SC1RST );
   }
#endif

   SCn_BaseClock = SystemPeriClock;

   sc_set_divisors( SCx, SCx_ConfigStruct->Mode, SCx_ConfigStruct->SCI_clock_gen, SCx_ConfigStruct->Baudrate, SCx_ConfigStruct->Oversampling );

   tmp = 0
         | ( ( SCx_ConfigStruct->Mode & 0x1 ) << SCn_CR1_SCnMD_Pos )
         | ( ( SCx_ConfigStruct->ParityEnDis & 0x1 ) << SCn_CR1_PENn_Pos )
         | ( ( SCx_ConfigStruct->Parity & 0x1 ) << SCn_CR1_PSELn_Pos )
         | ( ( SCx_ConfigStruct->Databits & 0x3 ) << SCn_CR1_DLENn_Pos )
         | ( ( SCx_ConfigStruct->Stopbits & 0x1 ) << SCn_CR1_STOPBn_Pos )
         | ( ( SCx_ConfigStruct->Oversampling & 0x1 ) << SCn_CR1_OVRSn_Pos )
         | ( 1 << SCn_CR1_TXEn_Pos )  // Tx Enable
         | ( 1 << SCn_CR1_RXEn_Pos )  // Rx Enable
         ;

   SCx->CR1 = tmp;
   SCx->CR2 = 0
              | ( ( SCx_ConfigStruct->SCI_clock_gen & SCn_CR2_SCnCLKG_Msk ) << SCn_CR2_SCnCLKG_Pos )
              ;
   SCx->BCMP = SCx_ConfigStruct->Baud_rate_Compensation;
   SCx->EGTR = SCx_ConfigStruct->Extra_guard_time;
   SCx->T3DR = SCx_ConfigStruct->T3_dur_time;
   SCx->T4DR = SCx_ConfigStruct->T4_dur_time;

   HAL_SC_ClearStatus( SCx, SCn_STATUS_ALL_EXCEPT_TXC );

   //Dummy Read
   tmp = HAL_SC_ReceiveByte( SCx );

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Deinitialize the SCn peripheral registers
 *                to their default reset values.
 * @param[in]     SCx
 *                   Pointer to the target SC
 *                   -  SC0 ~ SC1
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_SC_DeInit( SCn_Type* SCx )
{
   /* Check SC handle */
   if( SCx == NULL )
   {
      return HAL_ERROR;
   }

#if 1    // supported
   if( SCx == ( SCn_Type* )SC0 )
   {
      // Set up peripheral clock for SC0 module
      HAL_SCU_Peripheral_SetReset2( PPRST2_SC0RST );
      HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_SC0CLKE, PPxCLKE_Disable );
   }
#endif

#if 1    // supported
   if( SCx == ( SCn_Type* )SC1 )
   {
      // Set up peripheral clock for SC1 module
      HAL_SCU_Peripheral_SetReset2( PPRST2_SC1RST );
      HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_SC1CLKE, PPxCLKE_Disable );
   }
#endif

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Fills each SCn_Config member with its default value:
 *                   -  8 Data Bit
 *                   -  Even Parity Bit
 *                   -  1 Stop Bit
 *                   -  16 Oversampling
 *                   -  No guard time
 *                   -  fsc: PCLK/16 when SCI mode
 *                   -  T3DR: 10000 when SCI mode
 *                   -  T4DR: 40000 when SCI mode
 * @param[out]    SCn_Config
 *                   Pointer to a SCn_CFG_Type structure which will be initialized.
 * @param[in]     mode
 *                   SCn Operation Mode Type
 *                   -  SCn_SCI_MODE
 *                   -  SCn_UART_MODE
 * @param[in]     Baudrate
 *                   -  0 ~ 0xffff
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_SC_ConfigStructInit( SCn_CFG_Type* SCn_Config, SCn_OPMODE_Type mode, uint32_t Baudrate )
{
   /* Check SCn_Config */
   if( SCn_Config == NULL )
   {
      return HAL_ERROR;
   }

   SCn_Config->Mode = mode;
   if( mode == SCn_UART_MODE )
   {
      SCn_Config->ParityEnDis = SCn_PARITY_DISABLE;
   }
   else
   {
      SCn_Config->ParityEnDis = SCn_PARITY_ENABLE;
   }
   SCn_Config->Parity = SCn_PARITY_BIT_EVEN;
   SCn_Config->Databits = SCn_DATA_BIT_8;
   SCn_Config->Stopbits = SCn_STOP_BIT_1;
   SCn_Config->Oversampling = SCn_OVR_SAMP_16;
   SCn_Config->Baudrate = Baudrate;
   SCn_Config->SCI_clock_gen = 8;                    // SCI clock: To be 2MHz @System clock 32MHz
   SCn_Config->Baud_rate_Compensation = 0x00000010;  // Plus compensation @System clock 32MHz
   SCn_Config->Extra_guard_time = 0;                 // No extra guard time
   SCn_Config->T3_dur_time = 10000 - 1;              // Initial Reset Assertion Time when SCI mode
   SCn_Config->T4_dur_time = 40000 - 1;              // Initial ATR Receive Time when SCI mode

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure the interrupt source of selected SCn peripheral
 * @param[in]     SCx
 *                   Pointer to SCn peripheral
 *                   -  SC0 ~ SC1
 * @param[in]     SCn_IntCfg
 *                   Specifies the interrupt source
 *                   -  SCn_INTCFG_RXC
 *                   -  SCn_INTCFG_TXC
 *                   -  SCn_INTCFG_RTO
 *                   -  SCn_INTCFG_BLED
 *                   -  SCn_INTCFG_SCIN
 *                   -  SCn_INTCFG_TRYER
 *                   -  SCn_INTCFG_CONED
 *                   -  SCn_INTCFG_CONER
 *                   -  SCn_INTCFG_SED
 *                   -  SCn_INTCFG_SER
 *                   -  SCn_INTCFG_RSTA
 * @param[in]     NewState
 *                   Next State of Interrupt Operation
 *                   -  ENABLE, DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_SC_ConfigInterrupt( SCn_Type* SCx, SCn_INT_Type SCn_IntCfg, FunctionalState NewState )
{
   uint32_t    tmp = 0;

   /* Check SC handle */
   if( SCx == NULL )
   {
      return HAL_ERROR;
   }

   // get mask
   tmp = ( 1 << SCn_IntCfg );

   // enable/disable
   if( NewState == ENABLE )
   {
      SCx->IER |= tmp;
   }
   else
   {
      SCx->IER &= ~tmp;
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure Control mode for SCn peripheral
 * @param[in]     SCx
 *                   Pointer to the target SC
 *                   -  SC0 ~ SC1
 * @param[in]     Mode
 *                   Control Mode
 *                   -  SCn_CR1_SCIEN:     Activate SCn Block
 *                   -  SCn_CR1_RTOEN:     Receive Time Out Function Enable
 *                   -  SCn_CR1_RXE:       Enable/Disable Receiver unit
 *                   -  SCn_CR1_TXE:       Enable/Disable Transmit unit
 *                   -  SCn_CR1_PEN:       Enable/Disable Parity bit
 * @param[in]     NewState
 *                   Next State of Functional Operation
 *                   -  ENABLE, DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_SC_ControlConfig( SCn_Type* SCx, SCn_CR1_CONTROL_Type Mode, FunctionalState NewState )
{
   uint32_t    tmp;

   /* Check SC handle */
   if( SCx == NULL )
   {
      return HAL_ERROR;
   }

   switch( Mode )
   {
      case SCn_CR1_SCIEN:
         tmp = SCn_CR1_SCInEN_Msk;
         break;
      case SCn_CR1_RTOEN:
         tmp = SCn_CR1_RTOENn_Msk;
         break;
      case SCn_CR1_RXE:
         tmp = SCn_CR1_RXEn_Msk;
         break;
      case SCn_CR1_TXE:
         tmp = SCn_CR1_TXEn_Msk;
         break;
      case SCn_CR1_PEN:
         tmp = SCn_CR1_PENn_Msk;
         break;
   }

   if( NewState == ENABLE )
   {
      SCx->CR1 |= tmp;
   }
   else
   {
      SCx->CR1 &= ~( tmp & SCn_CR1_CONTROL_BITMASK );
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         SCn enable control
 * @param[in]     SCx
 *                   Pointer to the target SC
 *                   -  SC0 ~ SC1
 * @param[in]     state
 *                   -  ENABLE, DISABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_SC_Enable( SCn_Type* SCx, FunctionalState state )
{
   /* Check SC handle */
   if( SCx == NULL )
   {
      return HAL_ERROR;
   }

   if( state == ENABLE )
   {
      SCx->CR1 |= ( 1 << SCn_CR1_SCInEN_Pos );    // SCInEN
   }
   else
   {
      SCx->CR1 &= ~( 1 << SCn_CR1_SCInEN_Pos );   // SCInEN
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         SCn Auto Activation and Cold Reset
 * @param[in]     SCx
 *                   Pointer to the target SC
 *                   -  SC0 ~ SC1
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_SC_AutoAct( SCn_Type* SCx )
{
   /* Check SC handle */
   if( SCx == NULL )
   {
      return HAL_ERROR;
   }

   SCx->CR2_b.ACTENn = ENABLE;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         SCn Auto Warm Reset
 * @param[in]     SCx
 *                   Pointer to the target SC
 *                   -  SC0 ~ SC1
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_SC_AutoWarmRst( SCn_Type* SCx )
{
   /* Check SC handle */
   if( SCx == NULL )
   {
      return HAL_ERROR;
   }

   SCx->CR2_b.WRENn = ENABLE;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         SCn Auto Deactivation
 * @param[in]     SCx
 *                   Pointer to the target SC
 *                   -  SC0 ~ SC1
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_SC_AutoDeAct( SCn_Type* SCx )
{
   /* Check SC handle */
   if( SCx == NULL )
   {
      return HAL_ERROR;
   }

   SCx->CR2_b.DACTENn = ENABLE;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         SCn Receive Time Out Data Register Set
 * @param[in]     SCx
 *                   Pointer to the target SC
 *                   -  SC0 ~ SC1
 * @param[in]     RxTimeOutD
 *                   Receive Time Out Data with baud rate unit
 *                   -  0 ~ 0x00ffffff
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_SC_SetRxTimeOutData( SCn_Type* SCx, uint32_t RxTimeOutD )
{
   /* Check SC handle */
   if( SCx == NULL )
   {
      return HAL_ERROR;
   }

   SCx->RTODR = RxTimeOutD;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         SCn Transmit Extra Guard Time Register Set
 * @param[in]     SCx
 *                   Pointer to the target SC
 *                   -  SC0 ~ SC1
 * @param[in]     TxExGuardTime
 *                   Transmit Extra Guard Time between the last stop bit and the next start bit
 *                   -  0 ~ 0x000000ff
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_SC_SetExGuardTime( SCn_Type* SCx, uint32_t TxExGuardTime )
{
   /* Check SC handle */
   if( SCx == NULL )
   {
      return HAL_ERROR;
   }

   SCx->EGTR = TxExGuardTime;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         SCn T3DR and T4DR Registers Set
 * @param[in]     SCx
 *                   Pointer to the target SC
 *                   -  SC0 ~ SC1
 * @param[in]     SCnT3data
 *                   No. of fsc clock: Time duration for "Reset Assertion" of "Clock Disabe after Reset De-Assertion" of Smartcard Interface mode
 *                   -  0 ~ 0x0000ffff
 * @param[in]     SCnT4data
 *                   No. of fsc clock: Time duration for "ATR after Reset Assertion" of "from data low to Power down" of Smartcard Interface mode
 *                   -  0 ~ 0x0000ffff
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_SC_SetT34DR( SCn_Type* SCx, uint32_t SCnT3data, uint32_t SCnT4data )
{
   /* Check SC handle */
   if( SCx == NULL )
   {
      return HAL_ERROR;
   }

   SCx->T3DR = SCnT3data;
   SCx->T4DR = SCnT4data;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set Level for SCn peripheral when Smartcard Interface Mode
 * @param[in]     SCx
 *                   Pointer to the target SC
 *                   -  SC0 ~ SC1
 * @param[in]     Mode
 *                   Control Mode
 *                   -  SCn_CR2_CLKEN:     Enable/Disable Clock
 *                   -  SCn_CR2_CLKLV:     Clock Pin Level when disable
 *                   -  SCn_CR2_DATALV:    I/O Pin Level
 *                   -  SCn_CR2_RSTLV:     Reset Pin Level
 *                   -  SCn_CR2_PWRLV:     Power Pin Level
 * @param[in]     SetLevel
 *                   Next State of Functional Operation
 *                   -  SET
 *                   -  RESET
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_SC_SetLevel( SCn_Type* SCx, SCn_CR2_LEVEL_POS SelectPin, SetState SetLevel )
{
   uint32_t    tmp;

   /* Check SC handle */
   if( SCx == NULL )
   {
      return HAL_ERROR;
   }

   switch( SelectPin )
   {
      case SCn_CR2_CLKEN:
         tmp = SCn_CR2_SCnCLKEN_Msk;
         break;
      case SCn_CR2_CLKLV:
         tmp = SCn_CR2_SCnCLKLV_Msk;
         break;
      case SCn_CR2_DATALV:
         tmp = SCn_CR2_SCnDATALV_Msk;
         break;
      case SCn_CR2_RSTLV:
         tmp = SCn_CR2_SCnRSTLV_Msk;
         break;
      case SCn_CR2_PWRLV:
         tmp = SCn_CR2_SCnPWRLV_Msk;
         break;
   }

   if( SetLevel == SET )
   {
      SCx->CR2 |= tmp;
   }
   else
   {
      SCx->CR2 &= ~( tmp & SCn_CR2_CONTROL_BITMASK );
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         SCn Auto Convention Detection
 * @param[in]     SCx
 *                   Pointer to the target SC
 *                   -  SC0 ~ SC1
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_SC_AutoConvDet( SCn_Type* SCx )
{
   /* Check SC handle */
   if( SCx == NULL )
   {
      return HAL_ERROR;
   }

   SCx->CR3_b.ACONDETn = ENABLE;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         SCn Convention Selection
 * @param[in]     SCx
 *                   Pointer to the target SC
 *                   -  SC0 ~ SC1
 * @param[in]     conv_sel
 *                Convention Selection
 *                   -  SCn_DIRECT_CONV
 *                   -  SCn_INVERSE_CONV
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_SC_SetConv( SCn_Type* SCx, SCn_CONV_Type conv_sel )
{
   /* Check SC handle */
   if( SCx == NULL )
   {
      return HAL_ERROR;
   }

   SCx->CR3_b.CONSELn = conv_sel;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         SCn Retry Configuration with the specified parameters.
 * @param[in]     SCx
 *                   Pointer to the target SC
 *                   -  SC0 ~ SC1
 * @param[in]     retry_num
 *                   Specifies the retry number
 * @param[in]     retry_dly
 *                   Specifies the delay time before retry
 *                   -  SCn_DTIME_2J5ETU
 *                   -  SCn_DTIME_2J5PEXT
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_SC_ConfigRetry( SCn_Type* SCx, uint8_t retry_num, SCn_DTIME_Type retry_dly )
{
   /* Check SC handle */
   if( SCx == NULL )
   {
      return HAL_ERROR;
   }

   SCx->CR3_b.RETRYn = retry_num;
   SCx->CR3_b.DLYRETRYn = retry_dly;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         SCn Retry Enable.
 * @param[in]     SCx
 *                   Pointer to the target SC
 *                   -  SC0 ~ SC1
 * @param[in]     retry_en
 *                   Enable/Disable retry
 *                   -  SCn_RETRY_DISABLE
 *                   -  SCn_RETRY_ENABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_SC_RetryEnable( SCn_Type* SCx, SCn_RETRY_EN_DISABLE retry_en )
{
   /* Check SC handle */
   if( SCx == NULL )
   {
      return HAL_ERROR;
   }

   SCx->CR3_b.RETRYENn = retry_en;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         SCn Input Pin Polarity Set.
 * @param[in]     SCx
 *                   Pointer to the target SC
 *                   -  SC0 ~ SC1
 * @param[in]     In_pol
 *                   Input Polarity Setting
 *                   -  SCn_INPUT_POLARITY_FALLING
 *                   -  SCn_INPUT_POLARITY_RISING
 *                   -  SCn_INPUT_POLARITY_BOTH
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_SC_SetInPol( SCn_Type* SCx, SCn_INPUT_POLARITY_Type In_pol )
{
   /* Check SC handle */
   if( SCx == NULL )
   {
      return HAL_ERROR;
   }

   if( In_pol <= SCn_INPUT_POLARITY_BOTH )
   {
      SCx->CR3_b.SCnINPOL = In_pol;
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         SCn Receive Byte Count Enable.
 * @param[in]     SCx
 *                   Pointer to the target SC
 *                   -  SC0 ~ SC1
 * @param[in]     RxCnt_en
 *                   Enable/Disable Rx Byte Count
 *                   -  SCn_RXCNT_DISABLE
 *                   -  SCn_RXCNT_ENABLE
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_SC_RxCntEnable( SCn_Type* SCx, SCn_RXCNT_EN_DISABLE RxCnt_en )
{
   /* Check SC handle */
   if( SCx == NULL )
   {
      return HAL_ERROR;
   }

   SCx->CR3_b.RXCNTENn = RxCnt_en;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         SCn Rx block length set with the specified parameters.
 * @param[in]     SCx
 *                   Pointer to the target SC
 *                   -  SC0 ~ SC1
 * @param[in]     blk_len
 *                   Specifies the rx block length
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_SC_SetRxBlkLen( SCn_Type* SCx, uint8_t blk_len )
{
   /* Check SC handle */
   if( SCx == NULL )
   {
      return HAL_ERROR;
   }

   SCx->CR3_b.RXBLENn = blk_len;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Clear Status register in SCn peripheral.
 * @param[in]     SCx
 *                   Pointer to SCn peripheral
 *                   -  SC0 ~ SC1
 * @param[in]     Status
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_SC_ClearStatus( SCn_Type* SCx, SCn_STATUS_Type Status )
{
   /* Check SC handle */
   if( SCx == NULL )
   {
      return HAL_ERROR;
   }

   if( Status == SCn_STATUS_ALL_EXCEPT_TXC )
   {
      SCx->IFSR = SCn_STATUS_ALL_EXCEPT_TXC;
   }
   else
   {
      SCx->IFSR = ( 1 << Status );
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Get current value of Status register in SCn peripheral.
 * @param[in]     SCx
 *                   Pointer to SCn peripheral
 *                   -  SC0 ~ SC1
 * @return        Current value of Status register in SCn peripheral.
 *//*-------------------------------------------------------------------------*/
uint32_t HAL_SC_GetStatus( SCn_Type* SCx )
{
   return ( SCx->IFSR );
}

/*-------------------------------------------------------------------------*//**
 * @brief         Get the Rx Line Busy bit
 * @param[in]     SCx
 *                   Pointer to the target SC
 *                   -  SC0 ~ SC1
 * @return        Status of Rx Line
 * @note          Don't Tx during Rx Line Busy when Smartcard Interface Mode
 *//*-------------------------------------------------------------------------*/
uint8_t HAL_SC_GetRxLineBusy( SCn_Type* SCx )
{
   return SCx->IFSR_b.RXBUSYn;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Get the SCnIN Pin Status
 * @param[in]     SCx
 *                   Pointer to the target SC
 *                   -  SC0 ~ SC1
 * @return        Status of SCnIN Pin
 *//*-------------------------------------------------------------------------*/
uint8_t HAL_SC_GetSCnINST( SCn_Type* SCx )
{
   return SCx->CR2_b.SCnINST;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Check whether if SCn is busy or not
 * @param[in]     SCx
 *                   Pointer to the target SC
 *                   -  SC0 ~ SC1
 * @return        RESET if SCn is not busy, otherwise return SET.
 *//*-------------------------------------------------------------------------*/
FlagStatus HAL_SC_CheckBusy( SCn_Type* SCx )
{
   if( SCx->IFSR & SCn_IFSR_TXCIFLAGn_Msk )
   {
      return RESET;
   }
   else
   {
      return SET;
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Transmit a single data through SCn peripheral
 * @param[in]     SCx
 *                   Pointer to the target SC
 *                   -  SC0 ~ SC1
 * @param[in]     Data
 *                   Data to transmit (must be 8-bit long)
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_SC_TransmitByte( SCn_Type* SCx, uint8_t Data )
{
   /* Check SC handle */
   if( SCx == NULL )
   {
      return HAL_ERROR;
   }

   SCx->TDR = Data;

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Receive a single data from SCn peripheral
 * @param[in]     SCx
 *                   Pointer to the target SC
 *                   -  SC0 ~ SC1
 * @return        Data received
 *//*-------------------------------------------------------------------------*/
uint8_t HAL_SC_ReceiveByte( SCn_Type* SCx )
{
   return SCx->RDR;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Send a block of data via SCn peripheral
 * @param[in]     SCx
 *                   Pointer to the target SC
 *                   -  SC0 ~ SC1
 * @param[in]     txbuf
 *                   Pointer to Transmit buffer
 * @param[in]     buflen
 *                   Length of Transmit buffer
 * @param[in]     flag
 *                   Flag used in  SCn transfer
 *                   -  NONE_BLOCKING
 *                   -  BLOCKING
 * @return        Number of bytes sent.
 * @note          when using SCn in BLOCKING mode,
 *                a time-out condition is used via defined symbol SCn_BLOCKING_TIMEOUT.
 *//*-------------------------------------------------------------------------*/
uint32_t HAL_SC_Transmit( SCn_Type* SCx, uint8_t* txbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag )
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
         // clear flag
         HAL_SC_ClearStatus( SCx, SCn_STATUS_TXC );

         // send byte
         HAL_SC_TransmitByte( SCx, ( *pChar++ ) );

         // wait until tx is shifted out completely with timeout
         timeOut = SCn_BLOCKING_TIMEOUT;
         while( !( SCx->IFSR & SCn_IFSR_TXCIFLAGn_Msk ) )
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
         // if tx is not shifted out completely
         if( !( SCx->IFSR & SCn_IFSR_TXCIFLAGn_Msk ) )
         {
            break;
         }

         // send byte
         HAL_SC_TransmitByte( SCx, ( *pChar++ ) );
         HAL_SC_ClearStatus( SCx, SCn_STATUS_TXC );

         // update counter
         bToSend--;
         bSent++;
      }
   }

   // return
   return bSent;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Receive a block of data via SCn peripheral
 * @param[in]     SCx
 *                   Pointer to the target SC
 *                   -  SC0 ~ SC1
 * @param[out]    rxbuf
 *                   Pointer to Received buffer
 * @param[in]     buflen
 *                   Length of Received buffer
 * @param[in]     flag
 *                   Flag mode
 *                   -  NONE_BLOCKING
 *                   -  BLOCKING
 * @return        Number of bytes received
 * @note          when using SCn in BLOCKING mode,
 *                a time-out condition is used via defined symbol SCn_BLOCKING_TIMEOUT.
 *//*-------------------------------------------------------------------------*/
uint32_t HAL_SC_Receive( SCn_Type* SCx, uint8_t* rxbuf, uint32_t buflen, TRANSFER_BLOCK_Type flag )
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
         timeOut = SCn_BLOCKING_TIMEOUT;
         while( !( SCx->IFSR & SCn_IFSR_RXCIFLAGn_Msk ) )
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
         ( *pChar++ ) = HAL_SC_ReceiveByte( SCx );

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
         if( !( SCx->IFSR & SCn_IFSR_RXCIFLAGn_Msk ) )
         {
            break;
         }

         // receive byte
         ( *pChar++ ) = HAL_SC_ReceiveByte( SCx );

         // update counter
         bRecv++;
         bToRecv--;
      }
   }

   // return
   return bRecv;
}


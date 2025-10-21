/**
 *******************************************************************************
 * @file        A31L12x_hal_scu.c
 * @author      ABOV R&D Division
 * @brief       System Control Unit
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

/* Public Functions --------------------------------------------------------- */
//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Get Reset Source Status
 * @return        Reset Source
 * @details       This function gets reset source status and clear the register.
 *//*-------------------------------------------------------------------------*/
uint32_t HAL_SCU_ResetSourceStatus( void )
{
   uint32_t    tmp;

   tmp = SCUCC->RSTSSR;       // Get reset source status
#if 0    // before bug fix
   SCUCC->RSTSSR = 0x3FuL;    // Clear all reset source status
#else    // after bug fix
   SCUCC->RSTSSR = 0x7fuL;    // Clear all reset source status
#endif
   return tmp;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set Non-Maskable Interrupt(NMI) Source Selection Register
 * @param[in]     u32NmiCon
 *                   Values for NMISRCR register
 * @return        None
 * @details       This function sets NMISRCR register.
 *//*-------------------------------------------------------------------------*/
void HAL_SCU_SetNMI( uint32_t u32NmiCon )
{
   SCUCC->NMISRCR = u32NmiCon;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Check whether system reset ok or not. Generate s/w reset if a weak reset
 * @return        None
 * @details       This function checks system reset validation and Generate s/w reset if a weak reset.
 *//*-------------------------------------------------------------------------*/
void HAL_SCU_SoftwareReset_Config( void )
{
   if( SCUCC->SRSTVR_b.VALID != 0x55 )
   {
      SCUCC_GenSwRst();    // Generate S/W reset on invalid reset
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set Wake-Up Timer Data
 * @param[in]     u32Data
 * @return        None
 * @details       This function sets wake-up timer data to wait for release of deep sleep mode.
 * @note          1. When HIRC is system clock, its value should be set to be at least more than 4us.
                  2. When WDTRC is system clock, its value should be set to be at least more than 100us.
 *//*-------------------------------------------------------------------------*/
void HAL_SCU_SetWakupData( uint32_t u32Data )
{
   SCUCC->WUTDR = u32Data;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Change fine trim value of HIRC by one step
 * @param[in]     u32Ind
 *                   Indicator for +/- one step
 *                   -  HIRC_UP_ONESTEP, HIRC_DOWN_ONESTEP
 * @return        None
 * @details       This function changes fine trim value by one step.
 *                If the u32Ind is HIRC_UP_ONESTEP, HIRC frequency is changed up by about 140kHz.
 *                If the u32Ind is HIRC_DOWN_ONESTEP, HIRC frequency is changed down by about 140kHz.
 *//*-------------------------------------------------------------------------*/
void HAL_SCU_HIRCTRM_ClockConfig( uint32_t u32Ind )
{
   uint32_t    tmp, ntrim;

   tmp = ( SCUCC->HIRCTRM ) & 0x001fuL;   // Read current fine trim value of HIRC
   if( u32Ind == HIRC_UP_ONESTEP )        // Increment by one step(about 140kHz)
   {
      if( tmp != 0x0f )
      {
         tmp++;
      }
   }
   else     // Decrement by one step(about 140kHz)
   {
      if( tmp != 0x10 )
      {
         tmp--;
      }
   }
   tmp &= 0x1f;                                 // Fine trim value is only 5-bits
   tmp |= ( ( SCUCC->HIRCTRM ) & 0x00E0uL );    // Read Coarse trim value
   ntrim = ( tmp << 8 ) ^ 0x0000FF00;           // Make write complement key
   SCUCC->HIRCTRM = ( ( uint32_t )SCUCC_HIRCTRM_WTIDKY_Value << SCUCC_HIRCTRM_WTIDKY_Pos )   // Write new HIRC trim value with write ID and complement key
                    | ntrim
                    | tmp;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Change fine trim value of WDTRC by one step
 * @param[in]     u32Ind
 *                   Indicator for +/- one step
 *                   -  WDTRC_UP_ONESTEP, WDTRC_DOWN_ONESTEP
 * @return        None
 * @details       This function changes fine trim value by one step.
 *                If the u32Ind is WDTRC_UP_ONESTEP, WDTRC frequency is changed up by about 1.1kHz.
 *                If the u32Ind is WDTRC_DOWN_ONESTEP, WDTRC frequency is changed down by about 1.1kHz.
 *//*-------------------------------------------------------------------------*/
void HAL_SCU_WDTRCTRM_ClockConfig( uint32_t u32Ind )
{
   uint32_t    ctmp, ftmp, ntrim;

   ftmp = ( SCUCC->WDTRCTRM ) & 0x0007uL;    // Read current fine trim value of WDTRC
   ctmp = ( SCUCC->WDTRCTRM ) & 0x00F0uL;    // Read current coarse trim value of WDTRC
   if( u32Ind == WDTRC_UP_ONESTEP )          // Increment by one step(about 1.1kHz)
   {
      ftmp++;
      ftmp &= 0x07uL;                        // Fine trim value is only 3-bits
      if( ftmp == 0x04 )
      {
         if( ctmp != 0x70 )
         {
            ctmp += 0x10uL;
         }
         else
         {
            ftmp = 0x03uL;
         }
      }
   }
   else     // Decrement by one step(about 1.1kHz)
   {
      ftmp--;
      ftmp &= 0x07uL;      // Fine trim value is only 3-bits
      if( ftmp == 0x03 )
      {
         if( ctmp != 0x80 )
         {
            ctmp -= 0x10uL;
         }
         else
         {
            ftmp = 0x04uL;
         }
      }
   }
   ctmp &= 0x00f0uL;                      // Coarse trim value is only 4-bits
   ctmp |= ftmp;
   ntrim = ( ctmp << 8 ) ^ 0x0000FF00;    // Make write complement key
   SCUCC->WDTRCTRM   = ( SCUCC_WDTRCTRM_WTIDKY_Value << SCUCC_WDTRCTRM_WTIDKY_Pos )    // Write new WDTRC trim value with write ID and complement key
                       | ntrim
                       | ctmp;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure Clock Monitoring
 * @param[in]     u32Acts
 *                   Clock Monitoring Action Selection
 *                   -  MACTS_FlagChk, MACTS_RstGen, MACTS_SysClkChg
 * @param[in]     u32Target
 *                   Clock Monitoring Target Selection
 *                   -  MONCS_MCLK, MONCS_HIRC, MONCS_XMOSC, MONCS_XSOSC
 * @return        None
 * @details       This function checks whether the target clock oscillates.
 *//*-------------------------------------------------------------------------*/
void HAL_SCU_ClockMonitoring( uint32_t u32Acts, uint32_t u32Target )
{
   if( SCUCG->CLKSRCR_b.WDTRCEN == 0 )
   {
      SCUCG->CLKSRCR = SCUCG->CLKSRCR
                       | ( ( uint32_t )SCUCG_CLKSRCR_WTIDKY_Value << SCUCG_CLKSRCR_WTIDKY_Pos )
                       | CLKSRCR_WDTRCEN;                   // The WDTRC should be enabled to use clock monitoring
   }
   SCUCG->CMONCR = ( 0x3uL << 2 ) | u32Acts | u32Target;    // Clear MONFLAG and NMINTFG, Set Monitoring Target and Monitoring Action
   SCUCG->CMONCR_b.MONEN = 1;                               // Clock Monitoring Enable
}

/*-------------------------------------------------------------------------*//**
 * @brief         Disable Clock Monitoring
 * @return        None
 * @details       This function disables clock monitoring.
                  Before disabling the ��clock monitoring�� function, you need to take step to clear
                  the MACTS[1:0] bits of SCU_CMONCR register to ��00b��.
 *//*-------------------------------------------------------------------------*/
void HAL_SCU_ClockMonitoring_Disable( void )
{
   SCUCG->CMONCR_b.MACTS = 0;    // Clear MACTS bits first
   SCUCG->CMONCR_b.MONEN = 0;    // Disable clock monitoring function
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set Clock Source, HIRC Frequency, and type of XMOSC
 * @param[in]     u32FreIRC
 *                   HIRC Frequency Selection
 *                   -  HIRCSEL_HIRC1, HIRCSEL_HIRC2, HIRCSEL_HIRC4, HIRCSEL_HIRC8
 * @param[in]     u32TypeXM
 *                   Main Oscillator Type and Frequency Range Selection
 *                   -  XMFRNG_Xtal, XMFRNG_Clock
 * @param[in]     u32ClkSrc
 *                   Clock Source
 *                   -  CLKSRCR_WDTRCEN | CLKSRCR_HIRCEN | CLKSRCR_XMOSCEN | CLKSRCR_XSOSCEN
 * @return        None
 * @details       This function sets clock source, HIRC frequency, and x-tal type of XMOSC.
 *                If target clock source is one of XMOSC and XSOSC,
 *                the x-tal pins should be set as alternative before this function call.
 *                To set alternative for x-tal, Use HAL_SCU_MainXtal_PinConfig() and HAL_SCU_SubXtal_PinConfig() functions.
 *//*-------------------------------------------------------------------------*/
void HAL_SCU_ClockSource_Config( uint32_t u32FreIRC, uint32_t u32TypeXM, uint32_t u32ClkSrc )
{
   uint32_t    tmp;

   tmp = SCUCG->CLKSRCR & 0x0000000F;
   tmp |= ( ( ( uint32_t )SCUCG_CLKSRCR_WTIDKY_Value << SCUCG_CLKSRCR_WTIDKY_Pos )  // Write ID
            | u32FreIRC                                                             // HIRC Frequency
            | u32TypeXM                                                             // XMOSC type: x-tal or external clock
            | u32ClkSrc );
   SCUCG->CLKSRCR = tmp;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Enable Clock Source
 * @param[in]     u32ClkSrc
 *                   Clock Source
 *                   -  CLKSRCR_WDTRCEN | CLKSRCR_HIRCEN | CLKSRCR_XMOSCEN | CLKSRCR_XSOSCEN
 * @param[in]     u32HircDiv
 *                   HIRC Frequency Selection
 *                   -  HIRCSEL_HIRC1, HIRCSEL_HIRC2, HIRCSEL_HIRC4, HIRCSEL_HIRC8
 * @return        None
 * @details       This function is used to enable original source after system clock change.
 * @code
 *                [Example]
 *
 *                // eable HIRC, XMOSC, SXOSC, WDTRC and select HIRCSEL_HIRC1(32MHz HIRC)
 *                HAL_SCU_ClockSource_Enable( CLKSRCR_HIRCEN | CLKSRCR_XMOSCEN | CLKSRCR_XSOSCEN | CLKSRCR_WDTRCEN, HIRCSEL_HIRC1 );
 * @endcode
 *//*-------------------------------------------------------------------------*/
void HAL_SCU_ClockSource_Enable( uint32_t u32ClkSrc, uint32_t u32HircDiv )
{
#if 0    // before bug fix
   uint32_t    tmp;

   tmp = SCUCG->CLKSRCR & 0x0000ffff;     // 0x00000fff�� ���� ��...
   tmp |= ( ( uint32_t )SCUCG_CLKSRCR_WTIDKY_Value << SCUCG_CLKSRCR_WTIDKY_Pos );   // Write ID
   tmp |= u32ClkSrc;
   tmp |= u32HircDiv;
   SCUCG->CLKSRCR = tmp;
#else    // after bug fix
   SCUCG->CLKSRCR =  SCUCG->CLKSRCR
                     & ~( SCUCG_CLKSRCR_WTIDKY_Msk | SCUCG_CLKSRCR_HIRCSEL_Msk )
                     | ( ( uint32_t )SCUCG_CLKSRCR_WTIDKY_Value << SCUCG_CLKSRCR_WTIDKY_Pos )   // Write ID
                     | u32HircDiv
                     | u32ClkSrc
                     ;
#endif
}

/*-------------------------------------------------------------------------*//**
 * @brief         Disable Clock Source
 * @param[in]     u32ClkSrc
 *                   Clock Source
 *                   -  CLKSRCR_WDTRCEN | CLKSRCR_HIRCEN | CLKSRCR_XMOSCEN | CLKSRCR_XSOSCEN
 * @return        None
 * @details       This function is used to disable original source after system clock change.
 * @code
 *                [Example]
 *
 *                // disable XMOSC, SXOSC
 *                HAL_SCU_ClockSource_Disable( CLKSRCR_XMOSCEN | CLKSRCR_XSOSCEN );
 * @endcode
 *//*-------------------------------------------------------------------------*/
void HAL_SCU_ClockSource_Disable( uint32_t u32ClkSrc )
{
   uint32_t    tmp;

   tmp = SCUCG->CLKSRCR & 0x0000ffff;
   tmp |= ( ( uint32_t )SCUCG_CLKSRCR_WTIDKY_Value << SCUCG_CLKSRCR_WTIDKY_Pos );   // Write ID
   tmp &= ~u32ClkSrc;
   SCUCG->CLKSRCR = tmp;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set XMOSC Pins for x-tal
 * @return        None
 * @details       This function sets PE[3:2]'s alternative for x-tal of XMOSC.
 *//*-------------------------------------------------------------------------*/
void HAL_SCU_MainXtal_PinConfig( void )
{
   PE->AFSR1 &= 0xffff00ffuL;    // PE[3:2]: XOUT/XIN
   PE->PUPD  &= 0xffffff0fuL;    // PE[3:2]: Pull-up/down resistors Disable
   PE->MOD   &= 0xffffff0fuL;
   PE->MOD   |= 0x000000a0uL;    // PE[3:2]: Alternative Function
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set XSOSC Pins for x-tal
 * @return        None
 * @details       This function sets PE[1:0]'s alternative for x-tal of XSOSC.
 *//*-------------------------------------------------------------------------*/
void HAL_SCU_SubXtal_PinConfig( void )
{
   PE->AFSR1 &= 0xffffff00uL;    // PE[1:0]: SXOUT/SXIN
   PE->PUPD  &= 0xfffffff0uL;    // PE[1:0]: Pull-up/down resistors Disable
   PE->MOD   &= 0xfffffff0uL;
   PE->MOD   |= 0x0000000auL;    // PE[1:0]: Alternative Function
}

/*-------------------------------------------------------------------------*//**
 * @brief         Change System Clock
 * @param[in]     u32Target
 *                   Target Clock
 *                   -  MCLKSEL_HIRC, MCLKSEL_XMOSC, MCLKSEL_XSOSC, MCLKSEL_WDTRC
 * @return        None
 * @details       This function changes system clock to target source.
 *//*-------------------------------------------------------------------------*/
void HAL_SCU_SystemClockChange( uint32_t u32Target )
{
   SCUCG->SCCR = ( SCUCG_SCCR_WTIDKY_Value << SCUCG_SCCR_WTIDKY_Pos )   // Write ID
                 | u32Target;                                           // Target Clock Source
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set System Clock Dividers, SCDIVR1 for RTCC and LCD Driver in case of using MCLK, SCDIVR2 for SysTick Timer and PCLK
 * @param[in]     u32Div02
 *                   Values for Divider 0 and 2
 *                   -  Clock Divide for HCLK (Divider 0)
 *                      -  HDIV_MCLK16, HDIV_MCLK8, HDIV_MCLK4, HDIV_MCLK2, HDIV_MCLK1
 *                   -  Clock Divide for RTCC and LCD Driver (Divider 2)
 *                      -  WLDIV_MCLK64, WLDIV_MCLK128, WLDIV_MCLK256, WLDIV_MCLK512, WLDIV_MCLK1024
 * @param[in]     u32Div13
 *                   Values for Divider 1 and 3
 *                   -  Clock Divide for PCLK (Divider 1)
 *                      -  PDIV_HCLK1, PDIV_HCLK2, PDIV_HCLK4, PDIV_HCLK8
 *                   -  Clock Divide for SysTick Timer (Divider 3)
 *                      -  SYSTDIV_HCLK1, SYSTDIV_HCLK2, SYSTDIV_HCLK4, SYSTDIV_HCLK8
 * @return        None
 * @details       This function changes system clock to target source.
 *//*-------------------------------------------------------------------------*/
void HAL_SCU_SystemClockDivider( uint32_t u32Div02, uint32_t u32Div13 )
{
   // Divider 0 for HCLK:                 000/001/010/011/100: MCLK is divided by 16/8/4/2/1
   // Divider 2 for RTCC and LCD Driver:  000/001/010/011/100: MCLK is divided by 64/128/256/512/1024
   SCUCG->SCDIVR1 = u32Div02;

   // Divider 1 for PCLK:                 00/01/10/11: HCLK is divided by 1/2/4/8
   // Divider 3 for SysTick Timer:        00/01/10/11: HCLK is divided by 1/2/4/8
   SCUCG->SCDIVR2 = u32Div13;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set CLKO Pin for Clock Output
 * @param[in]     Px
 *                   Pointer to the target
 *                   -  PC only
 * @param[in]     pin_no
 *                   Pin Number
 *                   -  0 ~ 1
 * @return        None
 * @details       This function sets selected pin's alternative for CLKO.
 *//*-------------------------------------------------------------------------*/
void HAL_SCU_CLKO_PinConfig( Pn_Type* Px, uint8_t pin_no )
{
   bitsp( Px->MOD, pin_no * 2, 2, 2 );       // PC0   CLKO     0: Input Mode     1: Output Mode    2: Alternative Function Mode
   if( pin_no < 8 )
   {
      bitsp( Px->AFSR1, pin_no * 4, 4, 0 );  // PC0   CLKO     0: CLKO        1: ----        2: ----        3: ----        4: SC0IN       5: ----        6: ----        7: COM0
   }
   else
   {
      pin_no -= 8;
      bitsp( Px->AFSR2, pin_no * 4, 4, 0 );  // PC1   CLKO     0: CLKO        1: ----        2: TXD0        3: ----        4: SC0PWR      5: ----        6: ----        7: COM1
   }
   bitsp( Px->PUPD, pin_no * 2, 2, 0 );      // PC0   CLKO     0: Push-Pull Output     1: Open-Drain Output
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set Configuration for Clock Output
 * @param[in]     u32ClkSrc
 *                   Clock to output
 *                   -  CLKOS_MCLK, CLKOS_WDTRC, CLKOS_HIRC, CLKOS_HCLK, CLKOS_PCLK
 * @param[in]     u32Level
 *                   Clock Output Polarity when Disable
 *                   -  POLSEL_Low, POLSEL_High
 * @param[in]     u32Div
 *                   Output Clock Divide
 *                   -  CLKODIV_SelectedClock1, CLKODIV_SelectedClock2
 *                   -  CLKODIV_SelectedClock4, CLKODIV_SelectedClock8
 *                   -  CLKODIV_SelectedClock16, CLKODIV_SelectedClock32
 *                   -  CLKODIV_SelectedClock64, CLKODIV_SelectedClock128
 * @return        None
 * @details       This function sets clock output related configuration.
 *//*-------------------------------------------------------------------------*/
void HAL_SCU_ClockOutput( uint32_t u32ClkSrc, uint32_t u32Level, uint32_t u32Div )
{
   SCUCG->CLKOCR = 0
                   | ( 1 << SCUCG_CLKOCR_CLKOEN_Pos )    // CLKO Enable
                   | u32Level    // 0: Low Level   1: High Level
                   | u32Div      // 0: div_1       1: div_2       2: div_4    3: div_8    4: div_16   5: div_32   6: div_64   7: div_128
                   | u32ClkSrc   // 0: MCLK        1: WDTRC       2: HIRC     3: HCLK     4: PCLK
                   ;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set Peripheral Clock, The peripheral doesn't work if the corresponding bit is "0b".
 * @param[in]     u32PeriClk1
 *                   Values for TIMER50, TIMER40 ~ TIMER43, PA ~ PF
 * @param[in]     u32PeriClk2
 *                   Values for the Others Peripheral
 * @return        None
 * @details       This function sets the peripheral clock.
 *                A peripheral works properly during the corresponding bit is set to "1b".
 *//*-------------------------------------------------------------------------*/
void HAL_SCU_Peripheral_ClockConfig( uint32_t u32PeriClk1, uint32_t u32PeriClk2 )
{
   SCUCG->PPCLKEN1 = u32PeriClk1;   // Set peripheral clock of timers and ports
   SCUCG->PPCLKEN2 = u32PeriClk2;   // Set peripheral clock of the others
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set Each Peripheral Clock
 * @param[in]     u32PeriClk1
 *                   PeriClk1
 *                   -  PPCLKEN1_T50CLKE, PPCLKEN1_T40CLKE ~ PPCLKEN1_T43CLKE
 *                   -  PPCLKEN1_PACLKE ~ PPCLKEN1_PFCLKE
 * @param[in]     Ind
 *                   Enable/Disable Peripheral Clock.
 *                   -  PPxCLKE_Disable, PPxCLKE_Enable
 * @return        None
 * @details       This function sets each peripheral clock of timers and Ports.
 *//*-------------------------------------------------------------------------*/
void HAL_SCU_Peripheral_EnableClock1( uint32_t u32PeriClk1, uint32_t Ind )
{
   if( Ind )
   {
      SCUCG->PPCLKEN1 |= u32PeriClk1;     // Enable a peripheral clock of timers and ports
   }
   else
   {
      SCUCG->PPCLKEN1 &= ~u32PeriClk1;    // Disable a peripheral clock of timers and ports
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set Each Peripheral Clock
 * @param[in]     u32PeriClk2
 *                   PeriClk2
 *                   -  PPCLKEN2_DMACLKE, PPCLKEN2_CMP0CLKE ~ PPCLKEN2_CMP1CLKE
 *                   -  PPCLKEN2_SC0CLKE ~ PPCLKEN2_SC1CLKE, PPCLKEN2_SPI0CLKE ~ PPCLKEN2_SPI1CLKE
 *                   -  PPCLKEN2_FMCLKE, PPCLKEN2_LVICLKE, PPCLKEN2_WDTCLKE, PPCLKEN2_LPUTCLKE
 *                   -  PPCLKEN2_LCDCLKE, PPCLKEN2_CRCLKE, PPCLKEN2_RTCCLKE, PPCLKEN2_ADCLKE
 *                   -  PPCLKEN2_I2C0CLKE ~ PPCLKEN2_I2C1CLKE
 *                   -  PPCLKEN2_UT0CLKE ~ PPCLKEN2_UT1CLKE, PPCLKEN2_UST10CLKE
 * @param[in]     u32Ind
 *                   Enable/Disable Peripheral Clock.
 *                   -  PPxCLKE_Disable, PPxCLKE_Enable
 * @return        None
 * @details       This function sets each peripheral clock of the others.
 *//*-------------------------------------------------------------------------*/
void HAL_SCU_Peripheral_EnableClock2( uint32_t u32PeriClk2, uint32_t u32Ind )
{
   if( u32Ind )
   {
      SCUCG->PPCLKEN2 |= u32PeriClk2;     // Enable a peripheral clock of others
   }
   else
   {
      SCUCG->PPCLKEN2 &= ~u32PeriClk2;    // Disable a peripheral clock of others
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Reset Peripheral Block, The peripheral is reset if the corresponding bit is "1b".
 * @param[in]     u32PeriRst1
 *                   Values for TIMER50, TIMER40 ~ TIMER43, PA ~ PF
 * @param[in]     u32PeriRst2
 *                   Values for the Others Peripheral
 * @return        None
 * @details       This function reset peripheral block during the corresponding bit is set to "1b".
 *                After reset of a block, the corresponding bit should be cleared to "0b" for operation.
 *//*-------------------------------------------------------------------------*/
void HAL_SCU_Peripheral_ResetConfig( uint32_t u32PeriRst1, uint32_t u32PeriRst2 )
{
   uint32_t    i;

   SCUCG->PPRST1 = u32PeriRst1;  // Reset peripheral block of timers and ports if the corresponding bit is "1b"
   SCUCG->PPRST2 = u32PeriRst2;  // Reset peripheral block of the others
   for( i = 0 ; i < 10 ; i++ )
   {
      NOP();
   }
   SCUCG->PPRST1 = 0x0uL;        // Clear the peripheral reset bits
   SCUCG->PPRST2 = 0x0uL;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set/Reset Each Peripheral Block Reset of PPRST1 Register
 * @param[in]     u32EachPeri1
 *                   Peri1
 *                   -  PPRST1_T50RST, PPRST1_T40RST ~ PPRST1_T43RST
 *                   -  PPRST1_PARST ~ PPRST1_PFRST
 * @details       This function resets each peripheral block.
*//*-------------------------------------------------------------------------*/
void HAL_SCU_Peripheral_SetReset1( uint32_t u32EachPeri1 )
{
   uint32_t    i;

   SCUCG->PPRST1 = u32EachPeri1;    // Reset a peripheral block
   for( i = 0 ; i < 10 ; i++ )
   {
      NOP();
   }
   SCUCG->PPRST1 = 0;               // Clear the peripheral reset bit
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set/Reset Each Peripheral Block Reset of PPRST2 Register
 * @param[in]     u32EachPeri2
 *                   Peri2
 *                   -  PPRST2_DMARST, PPRST2_CMP0RST ~ PPRST2_CMP1RST, PPRST2_SC0RST ~ PPRST2_SC1RST
 *                   -  PPRST2_SPI0RST ~ PPRST2_SPI1RST, PPRST2_FMCRST, PPRST2_LVIRST, PPRST2_LPUTRST
 *                   -  PPRST2_LCDRST, PPRST2_CRRST, PPRST2_RTCRST, PPRST2_ADRST
 *                   -  PPRST2_I2C0RST ~ PPRST2_I2C1RST, PPRST2_UT0RST ~ PPRST2_UT1RST, PPRST2_UST10RST
 * @details       This function resets each peripheral block.
*//*-------------------------------------------------------------------------*/
void HAL_SCU_Peripheral_SetReset2( uint32_t u32EachPeri2 )
{
   uint32_t    i;

   SCUCG->PPRST2 = u32EachPeri2;    // Reset a peripheral block
   for( i = 0 ; i < 10 ; i++ )
   {
      NOP();
   }
   SCUCG->PPRST2 = 0;               // Clear the peripheral reset bit
}


/*-------------------------------------------------------------------------*//**
 * @brief         Peripheral Clock Selection of PPCLKSR Register
 * @param[in]     u32Peri
 *                   Peripheral Selection
 *                   -  PPCLKSR_T50CLK, PPCLKSR_LPUTCLK, PPCLKSR_RTCCLK, PPCLKSR_LCDCLK, PPCLKSR_WDTCLK
 * @param[in]     u32ClkSrc
 *                   Peripheral Clock Selection
 *                   -  PPCLKSR_T50CLK:   T50CLK_PCLK, T50CLK_WDTRC, T50CLK_HIRC, T50CLK_XSOSC
 *                   -  PPCLKSR_LPUTCLK:  LPUTCLK_PCLK, LPUTCLK_HIRC, LPUTCLK_XSOSC
 *                   -  PPCLKSR_RTCCLK:   RTCCLK_LowLevel, RTCCLK_XSOSC, RTCCLK_WDTRC, RTCCLK_DividedMCLK
 *                   -  PPCLKSR_LCDCLK:   LCDCLK_DividedMCLK, LCDCLK_XSOSC, LCDCLK_WDTRC
 *                   -  PPCLKSR_WDTCLK:   WDTCLK_WDTRC, WDTCLK_PCLK
 * @details       This function resets each peripheral block.
*//*-------------------------------------------------------------------------*/
void HAL_SCU_Peripheral_ClockSelection( uint32_t u32Peri, uint32_t u32ClkSrc )
{
   SCUCG->PPCLKSR &= ~u32Peri;
   SCUCG->PPCLKSR |= u32ClkSrc;
}


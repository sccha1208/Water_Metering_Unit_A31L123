/**
 *******************************************************************************
 * @file        A31L12x_hal_intc.c
 * @author      ABOV R&D Division
 * @brief       Interrupt Control
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

#include "A31L12x_hal_intc.h"

/* Public Functions --------------------------------------------------------- */
//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Configure External Interrupt Trigger
 * @param[in]     u32Px
 *                   Port Number
 *                   -  PORTA ~ PORTF
 * @param[in]     u32pin
 *                   Pin Number
 *                   -  0 ~ 12
 * @param[in]     u32Trig
 *                   Trigger Mode
 *                   -  ITRIGx_Edge, ITRIGx_Level
 * @param[in]     u32Con
 *                   Interrupt Mode
 *                   -  when Trigger Mode is ITRIGx_Edge
 *                      -  INTCTLx_Disable, INTCTLx_FallingEdge, INTCTLx_RisingEdge, INTCTLx_BothEdge
 *                   -  when Trigger Mode is ITRIGx_Level
 *                      -  INTCTLx_Disable, INTCTLx_LowLevel, INTCTLx_HighLevel
 * @return        None
 * @remark        Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 * @code
 *                [Example]
 *
 *                // configure PA0 as a Falling Edge Trigger
 *                HAL_INT_EIntPx_SetReg( PORTA, 0, ITRIGx_Edge, INTCTLx_FallingEdge );
 *
 *                // configure PA1 as a Falling Edge Trigger
 *                HAL_INT_EIntPx_SetReg( PORTA, 1, ITRIGx_Edge, INTCTLx_RisingEdge );
 *
 *                // configure PA2 as a Falling Edge Trigger
 *                HAL_INT_EIntPx_SetReg( PORTA, 2, ITRIGx_Edge, INTCTLx_BothEdge );
 *
 *                // configure PA3 as a Falling Edge Trigger
 *                HAL_INT_EIntPx_SetReg( PORTA, 3, ITRIGx_Level, INTCTLx_LowLevel );
 * @endcode
 *//*-------------------------------------------------------------------------*/
void HAL_INT_EIntPx_SetReg( uint32_t u32Px, uint32_t u32pin, uint32_t u32Trig, uint32_t u32Con )
{
   uint32_t    temp_reg;

#ifdef PORTA
   if( u32Px == PORTA )
   {
      temp_reg = INTC->PATRIG;
      temp_reg &= ~( 1 << u32pin );
      temp_reg |= ( u32Trig << u32pin );
      INTC->PATRIG = temp_reg;

      temp_reg = INTC->PACR;
      temp_reg &= ~( 3 << ( u32pin << 1 ) );
      temp_reg |= ( u32Con << ( u32pin << 1 ) );
      INTC->PACR = temp_reg;
   }
#endif

#ifdef PORTB
   if( u32Px == PORTB )
   {
      temp_reg = INTC->PBTRIG;
      temp_reg &= ~( 1 << u32pin );
      temp_reg |= ( u32Trig << u32pin );
      INTC->PBTRIG = temp_reg;

      temp_reg = INTC->PBCR;
      temp_reg &= ~( 3 << ( u32pin << 1 ) );
      temp_reg |= ( u32Con << ( u32pin << 1 ) );
      INTC->PBCR = temp_reg;
   }
#endif

#ifdef PORTC
   if( u32Px == PORTC )
   {
      temp_reg = INTC->PCTRIG;
      temp_reg &= ~( 1 << u32pin );
      temp_reg |= ( u32Trig << u32pin );
      INTC->PCTRIG = temp_reg;

      temp_reg = INTC->PCCR;
      temp_reg &= ~( 3 << ( u32pin << 1 ) );
      temp_reg |= ( u32Con << ( u32pin << 1 ) );
      INTC->PCCR = temp_reg;
   }
#endif

#ifdef PORTD
   if( u32Px == PORTD )
   {
      temp_reg = INTC->PDTRIG;
      temp_reg &= ~( 1 << u32pin );
      temp_reg |= ( u32Trig << u32pin );
      INTC->PDTRIG = temp_reg;

      temp_reg = INTC->PDCR;
      temp_reg &= ~( 3 << ( u32pin << 1 ) );
      temp_reg |= ( u32Con << ( u32pin << 1 ) );
      INTC->PDCR = temp_reg;
   }
#endif

#ifdef PORTE
   if( u32Px == PORTE )
   {
      temp_reg = INTC->PETRIG;
      temp_reg &= ~( 1 << u32pin );
      temp_reg |= ( u32Trig << u32pin );
      INTC->PETRIG = temp_reg;

      temp_reg = INTC->PECR;
      temp_reg &= ~( 3 << ( u32pin << 1 ) );
      temp_reg |= ( u32Con << ( u32pin << 1 ) );
      INTC->PECR = temp_reg;
   }
#endif

#ifdef PORTF
   if( u32Px == PORTF )
   {
      temp_reg = INTC->PFTRIG;
      temp_reg &= ~( 1 << u32pin );
      temp_reg |= ( u32Trig << u32pin );
      INTC->PFTRIG = temp_reg;

      temp_reg = INTC->PFCR;
      temp_reg &= ~( 3 << ( u32pin << 1 ) );
      temp_reg |= ( u32Con << ( u32pin << 1 ) );
      INTC->PFCR = temp_reg;
   }
#endif
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure External Interrupt Group
 * @param[in]     u32TarIntNum
 *                   External Interrupt Number
 *                   -  EINT0 ~ EINT3
 * @param[in]     u32SrcPort
 *                   Port Number
 *                   -  CONFx_PA ~ CONFx_PF
 * @param[in]     u32SrcPin
 *                   Pin Number
 *                   -  0 ~ 12
 * @return        None
 * @details       This function configures the external interrupt group 0 to 3
 *                   -  If EINT0CONF1 == 0x01214211, The group 0 interrupts are [None:PB6:None:PB4:PE3:PC2:PB1:PB0]
 * @remark        Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
void HAL_INT_EIntCfg( uint32_t u32TarIntNum, uint32_t u32SrcPort, uint32_t u32SrcPin )
{
   uint32_t    temp_reg;

   if( u32SrcPin < 8 )
   {
      if( u32TarIntNum == EINT0 )
      {
         temp_reg = INTC->EINT0CONF1;
         temp_reg &= ~( 0x0F << ( u32SrcPin * 4 ) );
         temp_reg |= ( u32SrcPort << ( u32SrcPin * 4 ) );
         INTC->EINT0CONF1 = temp_reg;
      }
      else if( u32TarIntNum == EINT1 )
      {
         temp_reg = INTC->EINT1CONF1;
         temp_reg &= ~( 0x0F << ( u32SrcPin * 4 ) );
         temp_reg |= ( u32SrcPort << ( u32SrcPin * 4 ) );
         INTC->EINT1CONF1 = temp_reg;
      }
      else if( u32TarIntNum == EINT2 )
      {
         temp_reg = INTC->EINT2CONF1;
         temp_reg &= ~( 0x0F << ( u32SrcPin * 4 ) );
         temp_reg |= ( u32SrcPort << ( u32SrcPin * 4 ) );
         INTC->EINT2CONF1 = temp_reg;
      }
      else if( u32TarIntNum == EINT3 )
      {
         temp_reg = INTC->EINT3CONF1;
         temp_reg &= ~( 0x0F << ( u32SrcPin * 4 ) );
         temp_reg |= ( u32SrcPort << ( u32SrcPin * 4 ) );
         INTC->EINT3CONF1 = temp_reg;
      }
   }
#if 1
   else
   {
      u32SrcPin -= 8;
      if( u32TarIntNum == EINT0 )
      {
         temp_reg = INTC->EINT0CONF2;
         temp_reg &= ~( 0x0F << ( u32SrcPin * 4 ) );
         temp_reg |= ( u32SrcPort << ( u32SrcPin * 4 ) );
         INTC->EINT0CONF2 = temp_reg;
      }
      else if( u32TarIntNum == EINT1 )
      {
         temp_reg = INTC->EINT1CONF2;
         temp_reg &= ~( 0x0F << ( u32SrcPin * 4 ) );
         temp_reg |= ( u32SrcPort << ( u32SrcPin * 4 ) );
         INTC->EINT1CONF2 = temp_reg;
      }
      else if( u32TarIntNum == EINT2 )
      {
         temp_reg = INTC->EINT2CONF2;
         temp_reg &= ~( 0x0F << ( u32SrcPin * 4 ) );
         temp_reg |= ( u32SrcPort << ( u32SrcPin * 4 ) );
         INTC->EINT2CONF2 = temp_reg;
      }
      else if( u32TarIntNum == EINT3 )
      {
         temp_reg = INTC->EINT3CONF2;
         temp_reg &= ~( 0x0F << ( u32SrcPin * 4 ) );
         temp_reg |= ( u32SrcPort << ( u32SrcPin * 4 ) );
         INTC->EINT3CONF2 = temp_reg;
      }
   }
#endif
}

/*-------------------------------------------------------------------------*//**
 * @brief         Enable Interrupt Source Mask
 * @param[in]     u32Src
 *                   Interrupt Source Mask
 *                   -  MSK_LVI | MSK_WUT | MSK_WDT | MSK_EINT0 | ...
 * @return        None
 * @code
 *                [Example]
 *
 *                // mask LVI, WUT, WDT interrupt
 *                HAL_INT_EInt_MaskEnable( MSK_LVI | MSK_WUT | MSK_WDT );
 * @endcode
 *//*-------------------------------------------------------------------------*/
void HAL_INT_EInt_MaskEnable( uint32_t u32Src )
{
   INTC->MSK &= ~u32Src;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Disable Interrupt Source Mask
 * @param[in]     u32Src
 *                   Interrupt Source Mask
 *                   -  MSK_LVI | MSK_WUT | MSK_WDT | MSK_EINT0 | ...
 * @return        None
 * @code
 *                [Example]
 *
 *                // unmask LVI, WUT, WDT interrupt
 *                HAL_INT_EInt_MaskDisable( MSK_LVI | MSK_WUT | MSK_WDT );
 * @endcode
 *//*-------------------------------------------------------------------------*/
void HAL_INT_EInt_MaskDisable( uint32_t u32Src )
{
   INTC->MSK |= u32Src;
}

#ifdef PORTA
/*-------------------------------------------------------------------------*//**
 * @brief         Clear PA Interrupt Flag
 * @param[in]     u32Value
 *                   Pn Interrupt Flag Mask
 *                   -  PnFLAG_FLAG0 ~ PnFLAG_FLAG9
 * @return        None
 * @remark        Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
void HAL_INT_EIntPA_ClearIntStatus( uint32_t u32Value )
{
   INTC->PAFLAG = u32Value;
}
#endif

#ifdef PORTB
/*-------------------------------------------------------------------------*//**
 * @brief         Clear PB Interrupt Flag
 * @param[in]     u32Value
 *                   Pn Interrupt Flag Mask
 *                   -  PnFLAG_FLAG0 ~ PnFLAG_FLAG12
 * @return        None
 * @remark        Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
void HAL_INT_EIntPB_ClearIntStatus( uint32_t u32Value )
{
   INTC->PBFLAG = u32Value;
}
#endif

#ifdef PORTC
/*-------------------------------------------------------------------------*//**
 * @brief         Clear PC Interrupt Flag
 * @param[in]     u32Value
 *                   Pn Interrupt Flag Mask
 *                   -  PnFLAG_FLAG0 ~ PnFLAG_FLAG11
 * @return        None
 * @remark        Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
void HAL_INT_EIntPC_ClearIntStatus( uint32_t u32Value )
{
   INTC->PCFLAG = u32Value;
}
#endif

#ifdef PORTD
/*-------------------------------------------------------------------------*//**
 * @brief         Clear PD Interrupt Flag
 * @param[in]     u32Value
 *                   Pn Interrupt Flag Mask
 *                   -  PnFLAG_FLAG0 ~ PnFLAG_FLAG7
 * @return        None
 * @remark        Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
void HAL_INT_EIntPD_ClearIntStatus( uint32_t u32Value )
{
   INTC->PDFLAG = u32Value;
}
#endif

#ifdef PORTE
/*-------------------------------------------------------------------------*//**
 * @brief         Clear PE Interrupt Flag
 * @param[in]     u32Value
 *                   Pn Interrupt Flag Mask
 *                   -  PnFLAG_FLAG0 ~ PnFLAG_FLAG4
 * @return        None
 * @remark        Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
void HAL_INT_EIntPE_ClearIntStatus( uint32_t u32Value )
{
   INTC->PEFLAG = u32Value;
}
#endif

#ifdef PORTF
/*-------------------------------------------------------------------------*//**
 * @brief         Clear PF Interrupt Flag
 * @param[in]     u32Value
 *                   Pn Interrupt Flag Mask
 *                   -  PnFLAG_FLAG0 ~ PnFLAG_FLAG3
 * @return        None
 * @remark        Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
void HAL_INT_EIntPF_ClearIntStatus( uint32_t u32Value )
{
   INTC->PFFLAG = u32Value;
}
#endif

#ifdef PORTA
/*-------------------------------------------------------------------------*//**
 * @brief         Get PA Interrupt Flag
 * @return        Pn Interrput Flag
 * @remark        Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
uint32_t HAL_INT_EIntPA_GetIntStatus( void )
{
   return INTC->PAFLAG;
}
#endif

#ifdef PORTB
/*-------------------------------------------------------------------------*//**
 * @brief         Get PB Interrupt Flag
 * @return        Pn Interrput Flag
 * @remark        Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
uint32_t HAL_INT_EIntPB_GetIntStatus( void )
{
   return INTC->PBFLAG;
}
#endif

#ifdef PORTC
/*-------------------------------------------------------------------------*//**
 * @brief         Get PC Interrupt Flag
 * @return        Pn Interrput Flag
 * @remark        Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
uint32_t HAL_INT_EIntPC_GetIntStatus( void )
{
   return INTC->PCFLAG;
}
#endif

#ifdef PORTD
/*-------------------------------------------------------------------------*//**
 * @brief         Get PD Interrupt Flag
 * @return        Pn Interrput Flag
 * @remark        Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
uint32_t HAL_INT_EIntPD_GetIntStatus( void )
{
   return INTC->PDFLAG;
}
#endif

#ifdef PORTE
/*-------------------------------------------------------------------------*//**
 * @brief         Get PE Interrupt Flag
 * @return        Pn Interrput Flag
 * @remark        Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
uint32_t HAL_INT_EIntPE_GetIntStatus( void )
{
   return INTC->PEFLAG;
}
#endif

#ifdef PORTF
/*-------------------------------------------------------------------------*//**
 * @brief         Get PF Interrupt Flag
 * @return        Pn Interrput Flag
 * @remark        Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
uint32_t HAL_INT_EIntPF_GetIntStatus( void )
{
   return INTC->PFFLAG;
}
#endif


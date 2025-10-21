/**
 *******************************************************************************
 * @file        A31L12x_hal_intc.h
 * @author      ABOV R&D Division
 * @brief       Interrupt Control Header File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#ifndef _INTC_H_
#define _INTC_H_

#include "A31L12x.h"
#include "A31L12x_hal_aa_types.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

// External Interrupt Port Number
#define PORTA                    (0x0uL)  /**< Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0] */
#define PORTB                    (0x1uL)  /**< Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0] */
#define PORTC                    (0x2uL)  /**< Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0] */
#define PORTD                    (0x3uL)  /**< Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0] */
#define PORTE                    (0x4uL)  /**< Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0] */
#define PORTF                    (0x5uL)  /**< Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0] */

// External Interrupt Number
/*
#define EINT0_SEL                (0x0uL)
#define EINT1_SEL                (0x1uL)
#define EINT2_SEL                (0x2uL)
#define EINT3_SEL                (0x3uL)
*/
#define EINT0                    (0x0uL)
#define EINT1                    (0x1uL)
#define EINT2                    (0x2uL)
#define EINT3                    (0x3uL)

// External Interrupt Trigger Selection Control
/*
#define TRG_EDGE                 (0x0uL)
#define TRG_LEVEL                (0x1uL)
*/
#define ITRIGx_Edge              INTC_PBTRIG_ITRIG12_Edge
#define ITRIGx_Level             INTC_PBTRIG_ITRIG12_Level

// External Interrupt Enable Control
/*
#define EInt_DISABLE             (0x0uL)
#define EInt_LOW_LEVEL_INT       (0x1uL)
#define EInt_HIGH_LEVEL_INT      (0x2uL)
#define EInt_FALLING_EDGE_INT    (0x1uL)
#define EInt_RISING_EDGE_INT     (0x2uL)
#define EInt_BOTH_EDGE_INT       (0x3uL)
*/
#define INTCTLx_Disable          INTC_PBCR_INTCTL12_Disable
#define INTCTLx_LowLevel         INTC_PBCR_INTCTL12_FallingEdgeLowLevel
#define INTCTLx_HighLevel        INTC_PBCR_INTCTL12_RisingEdgeHighLevel
#define INTCTLx_FallingEdge      INTC_PBCR_INTCTL12_FallingEdgeLowLevel
#define INTCTLx_RisingEdge       INTC_PBCR_INTCTL12_RisingEdgeHighLevel
#define INTCTLx_BothEdge         INTC_PBCR_INTCTL12_BothEdgeNoLevel

// External Interrupt Flag Clear Control
/*
#define EFLAG0                   (0x1uL << 0)
#define EFLAG1                   (0x1uL << 1)
#define EFLAG2                   (0x1uL << 2)
#define EFLAG3                   (0x1uL << 3)
#define EFLAG4                   (0x1uL << 4)
#define EFLAG5                   (0x1uL << 5)
#define EFLAG6                   (0x1uL << 6)
#define EFLAG7                   (0x1uL << 7)
*/
// #define PnFLAG_FLAG15         (0x1uL << INTC_PBFLAG_FLAG15_Pos)
// #define PnFLAG_FLAG14         (0x1uL << INTC_PBFLAG_FLAG14_Pos)
// #define PnFLAG_FLAG13         (0x1uL << INTC_PBFLAG_FLAG13_Pos)
#define PnFLAG_FLAG12            (0x1uL << INTC_PBFLAG_FLAG12_Pos)
#define PnFLAG_FLAG11            (0x1uL << INTC_PBFLAG_FLAG11_Pos)
#define PnFLAG_FLAG10            (0x1uL << INTC_PBFLAG_FLAG10_Pos)
#define PnFLAG_FLAG9             (0x1uL << INTC_PBFLAG_FLAG9_Pos)
#define PnFLAG_FLAG8             (0x1uL << INTC_PBFLAG_FLAG8_Pos)
#define PnFLAG_FLAG7             (0x1uL << INTC_PBFLAG_FLAG7_Pos)
#define PnFLAG_FLAG6             (0x1uL << INTC_PBFLAG_FLAG6_Pos)
#define PnFLAG_FLAG5             (0x1uL << INTC_PBFLAG_FLAG5_Pos)
#define PnFLAG_FLAG4             (0x1uL << INTC_PBFLAG_FLAG4_Pos)
#define PnFLAG_FLAG3             (0x1uL << INTC_PBFLAG_FLAG3_Pos)
#define PnFLAG_FLAG2             (0x1uL << INTC_PBFLAG_FLAG2_Pos)
#define PnFLAG_FLAG1             (0x1uL << INTC_PBFLAG_FLAG1_Pos)
#define PnFLAG_FLAG0             (0x1uL << INTC_PBFLAG_FLAG0_Pos)

// External Interrupt Configuration Selection Control
#ifdef PORTA
#define CONFx_PA                 INTC_EINT0CONF1_CONF0_PA   /**< Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0] */
#endif
#ifdef PORTB
#define CONFx_PB                 INTC_EINT0CONF1_CONF0_PB   /**< Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0] */
#endif
#ifdef PORTC
#define CONFx_PC                 INTC_EINT0CONF1_CONF0_PC   /**< Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0] */
#endif
#ifdef PORTD
#define CONFx_PD                 INTC_EINT0CONF1_CONF0_PD   /**< Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0] */
#endif
#ifdef PORTE
#define CONFx_PE                 INTC_EINT0CONF1_CONF0_PE   /**< Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0] */
#endif
#ifdef PORTF
#define CONFx_PF                 INTC_EINT0CONF1_CONF0_PF   /**< Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0] */
#endif

// Interrupt Source Mask Control
/*
#define LVI_MASK                 (0x1uL <<  0)
#define WUT_MASK                 (0x1uL <<  1)
#define WDT_MASK                 (0x1uL <<  2)
#define EINT0_MASK               (0x1uL <<  3)
#define EINT1_MASK               (0x1uL <<  4)
#define EINT2_MASK               (0x1uL <<  5)
#define EINT3_MASK               (0x1uL <<  6)
#define TIMER40_MASK             (0x1uL <<  7)
#define TIMER41_MASK             (0x1uL <<  8)
#define TIMER42_MASK             (0x1uL <<  9)
#define I2C0_MASK                (0x1uL << 10)
#define USART10_MASK             (0x1uL << 11)
#define SPI0_MASK                (0x1uL << 12)
#define SPI1_MASK                (0x1uL << 13)
#define I2C1_MASK                (0x1uL << 14)
#define TIMER50_MASK             (0x1uL << 15)
#define SC0_MASK                 (0x1uL << 16)
#define SC1_MASK                 (0x1uL << 17)
#define ADC_MASK                 (0x1uL << 18)
#define UART0_MASK               (0x1uL << 19)
#define UART1_MASK               (0x1uL << 20)
#define TIMER43_MASK             (0x1uL << 21)
#define CMP_MASK                 (0x1uL << 22)
#define DMAC0_MASK               (0x1uL << 23)
#define DMAC1_MASK               (0x1uL << 24)
#define LPUART_MASK              (0x1uL << 25)
#define RTCC_MASK                (0x1uL << 28)
#define DMAC2_MASK               (0x1uL << 29)
#define DMAC3_MASK               (0x1uL << 30)
#define DMAC4_MASK               (0x1uL << 31)
*/
#define MSK_DMAC4                (0x1uL << INTC_MSK_IMSK31_DMAC4_Pos)
#define MSK_DMAC3                (0x1uL << INTC_MSK_IMSK30_DMAC3_Pos)
#define MSK_DMAC2                (0x1uL << INTC_MSK_IMSK29_DMAC2_Pos)
#define MSK_RTCC                 (0x1uL << INTC_MSK_IMSK28_RTCC_Pos)
#define MSK_IMSK27               (0x1uL << INTC_MSK_IMSK27_NULL_Pos)
#define MSK_IMSK26               (0x1uL << INTC_MSK_IMSK26_NULL_Pos)
#define MSK_LPUART               (0x1uL << INTC_MSK_IMSK25_LPUART_Pos)
#define MSK_DMAC1                (0x1uL << INTC_MSK_IMSK24_DMAC1_Pos)
#define MSK_DMAC0                (0x1uL << INTC_MSK_IMSK23_DMAC0_Pos)
#define MSK_CMPn                 (0x1uL << INTC_MSK_IMSK22_CMPn_Pos)
#define MSK_TIMER43              (0x1uL << INTC_MSK_IMSK21_TIMER43_Pos)
#define MSK_UART1                (0x1uL << INTC_MSK_IMSK20_UART1_Pos)
#define MSK_UART0                (0x1uL << INTC_MSK_IMSK19_UART0_Pos)
#define MSK_ADC                  (0x1uL << INTC_MSK_IMSK18_ADC_Pos)
#define MSK_SC1                  (0x1uL << INTC_MSK_IMSK17_SC1_Pos)
#define MSK_SC0                  (0x1uL << INTC_MSK_IMSK16_SC0_Pos)
#define MSK_TIMER50              (0x1uL << INTC_MSK_IMSK15_TIMER50_Pos)
#define MSK_I2C1                 (0x1uL << INTC_MSK_IMSK14_I2C1_Pos)
#define MSK_SPI1                 (0x1uL << INTC_MSK_IMSK13_SPI1_Pos)
#define MSK_SPI0                 (0x1uL << INTC_MSK_IMSK12_SPI0_Pos)
#define MSK_USART10              (0x1uL << INTC_MSK_IMSK11_USART10_Pos)
#define MSK_I2C0                 (0x1uL << INTC_MSK_IMSK10_I2C0_Pos)
#define MSK_TIMER42              (0x1uL << INTC_MSK_IMSK9_TIMER42_Pos)
#define MSK_TIMER41              (0x1uL << INTC_MSK_IMSK8_TIMER41_Pos)
#define MSK_TIMER40              (0x1uL << INTC_MSK_IMSK7_TIMER40_Pos)
#define MSK_EINT3                (0x1uL << INTC_MSK_IMSK6_EINT3_Pos)
#define MSK_EINT2                (0x1uL << INTC_MSK_IMSK5_EINT2_Pos)
#define MSK_EINT1                (0x1uL << INTC_MSK_IMSK4_EINT1_Pos)
#define MSK_EINT0                (0x1uL << INTC_MSK_IMSK3_EINT0_Pos)
#define MSK_WDT                  (0x1uL << INTC_MSK_IMSK2_WDT_Pos)
#define MSK_WUT                  (0x1uL << INTC_MSK_IMSK1_WUT_Pos)
#define MSK_LVI                  (0x1uL << INTC_MSK_IMSK0_LVI_Pos)

//******************************************************************************
// Macro
//******************************************************************************

#ifdef PORTA
/*-------------------------------------------------------------------------*//**
 * @brief         Get PA Interrupt Flag
 * @return        Pn Interrput Flag
 * @remark        Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
#define EIntPA_GetFg()                 (INTC->PAFLAG)
#endif

#ifdef PORTB
/*-------------------------------------------------------------------------*//**
 * @brief         Get PB Interrupt Flag
 * @return        Pn Interrput Flag
 * @remark        Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
#define EIntPB_GetFg()                 (INTC->PBFLAG)
#endif

#ifdef PORTC
/*-------------------------------------------------------------------------*//**
 * @brief         Get PC Interrupt Flag
 * @return        Pn Interrput Flag
 * @remark        Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
#define EIntPC_GetFg()                 (INTC->PCFLAG)
#endif

#ifdef PORTD
/*-------------------------------------------------------------------------*//**
 * @brief         Get PD Interrupt Flag
 * @return        Pn Interrput Flag
 * @remark        Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
#define EIntPD_GetFg()                 (INTC->PDFLAG)
#endif

#ifdef PORTE
/*-------------------------------------------------------------------------*//**
 * @brief         Get PE Interrupt Flag
 * @return        Pn Interrput Flag
 * @remark        Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
#define EIntPE_GetFg()                 (INTC->PEFLAG)
#endif

#ifdef PORTF
/*-------------------------------------------------------------------------*//**
 * @brief         Get PF Interrupt Flag
 * @return        Pn Interrput Flag
 * @remark        Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
#define EIntPF_GetFg()                 (INTC->PFFLAG)
#endif

#ifdef PORTA
/*-------------------------------------------------------------------------*//**
 * @brief         Clear PA Interrupt Flag
 * @param[in]     u32Bit
 *                   Pn Interrupt Flag Mask
 *                   -  PnFLAG_FLAG0 ~ PnFLAG_FLAG9
 * @return        None
 * @remark        Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
#define EIntPA_ClrFgBits( u32Bit )     (INTC->PAFLAG = u32Bit)
#endif

#ifdef PORTB
/*-------------------------------------------------------------------------*//**
 * @brief         Clear PB Interrupt Flag
 * @param[in]     u32Bit
 *                   Pn Interrupt Flag Mask
 *                   -  PnFLAG_FLAG0 ~ PnFLAG_FLAG12
 * @return        None
 * @remark        Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
#define EIntPB_ClrFgBits( u32Bit )     (INTC->PBFLAG = u32Bit)
#endif

#ifdef PORTC
/*-------------------------------------------------------------------------*//**
 * @brief         Clear PC Interrupt Flag
 * @param[in]     u32Bit
 *                   Pn Interrupt Flag Mask
 *                   -  PnFLAG_FLAG0 ~ PnFLAG_FLAG11
 * @return        None
 * @remark        Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
#define EIntPC_ClrFgBits(u32Bit)       (INTC->PCFLAG = u32Bit)
#endif

#ifdef PORTD
/*-------------------------------------------------------------------------*//**
 * @brief         Clear PD Interrupt Flag
 * @param[in]     u32Bit
 *                   Pn Interrupt Flag Mask
 *                   -  PnFLAG_FLAG0 ~ PnFLAG_FLAG7
 * @return        None
 * @remark        Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
#define EIntPD_ClrFgBits( u32Bit )     (INTC->PDFLAG = u32Bit)
#endif

#ifdef PORTE
/*-------------------------------------------------------------------------*//**
 * @brief         Clear PE Interrupt Flag
 * @param[in]     u32Bit
 *                   Pn Interrupt Flag Mask
 *                   -  PnFLAG_FLAG0 ~ PnFLAG_FLAG4
 * @return        None
 * @remark        Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
#define EIntPE_ClrFgBits( u32Bit )     (INTC->PEFLAG = u32Bit)
#endif

#ifdef PORTF
/*-------------------------------------------------------------------------*//**
 * @brief         Clear PF Interrupt Flag
 * @param[in]     u32Bit
 *                   Pn Interrupt Flag Mask
 *                   -  PnFLAG_FLAG0 ~ PnFLAG_FLAG3
 * @return        None
 * @remark        Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
#define EIntPF_ClrFgBits( u32Bit )     (INTC->PFFLAG = u32Bit)
#endif

/*-------------------------------------------------------------------------*//**
 * @brief         Mask Interrupt Source
 * @param[in]     u32Msk
 *                   Interrupt Source Mask
 *                   -  MSK_LVI, MSK_WUT, MSK_WDT, MSK_EINT0, ...
 * @return        None
 * @details       This macro masks an interrupt vector
 *//*-------------------------------------------------------------------------*/
#define Int_Mask( u32Msk )             (INTC->MSK = (INTC->MSK) & ~u32Msk)

/*-------------------------------------------------------------------------*//**
 * @brief         Unmask Interrupt Source
 * @param[in]     u32UnMsk
 *                   Interrupt Source Mask
 *                   -  MSK_LVI, MSK_WUT, MSK_WDT, MSK_EINT0, ...
 * @return        None
 * @details       This macro unmasks an interrupt vector
 *//*-------------------------------------------------------------------------*/
#define Int_UnMask( u32UnMsk )         (INTC->MSK = (INTC->MSK) | u32UnMsk)

//******************************************************************************
// Function
//******************************************************************************

void HAL_INT_EIntPx_SetReg( uint32_t u32Px, uint32_t u32pin, uint32_t u32Trig, uint32_t u32Con );
void HAL_INT_EIntCfg( uint32_t u32TarIntNum, uint32_t u32SrcPort, uint32_t u32SrcPin );
void HAL_INT_EInt_MaskEnable( uint32_t u32Src );
void HAL_INT_EInt_MaskDisable( uint32_t u32Src );

#ifdef PORTA
void HAL_INT_EIntPA_ClearIntStatus( uint32_t u32Value );
#endif
#ifdef PORTB
void HAL_INT_EIntPB_ClearIntStatus( uint32_t u32Value );
#endif
#ifdef PORTC
void HAL_INT_EIntPC_ClearIntStatus( uint32_t u32Value );
#endif
#ifdef PORTD
void HAL_INT_EIntPD_ClearIntStatus( uint32_t u32Value );
#endif
#ifdef PORTE
void HAL_INT_EIntPE_ClearIntStatus( uint32_t u32Value );
#endif
#ifdef PORTF
void HAL_INT_EIntPF_ClearIntStatus( uint32_t u32Value );
#endif

#ifdef PORTA
uint32_t HAL_INT_EIntPA_GetIntStatus( void );
#endif
#ifdef PORTB
uint32_t HAL_INT_EIntPB_GetIntStatus( void );
#endif
#ifdef PORTC
uint32_t HAL_INT_EIntPC_GetIntStatus( void );
#endif
#ifdef PORTD
uint32_t HAL_INT_EIntPD_GetIntStatus( void );
#endif
#ifdef PORTE
uint32_t HAL_INT_EIntPE_GetIntStatus( void );
#endif
#ifdef PORTF
uint32_t HAL_INT_EIntPF_GetIntStatus( void );
#endif

#ifdef __cplusplus
}
#endif

#endif   /* _INTC_H_ */


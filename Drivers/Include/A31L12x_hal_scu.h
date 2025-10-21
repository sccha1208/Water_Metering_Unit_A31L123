/**
 *******************************************************************************
 * @file        A31L12x_hal_scu.h
 * @author      ABOV R&D Division
 * @brief       SCU Header File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#ifndef _SCU_H_
#define _SCU_H_

#include "A31L12x.h"
#include "A31L12x_hal_aa_types.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

// Boot Pin Function Selection Control
/*
#define B_INCLUDE_RSTB           2
#define B_POR_ONLY               3
*/
#define BFIND_PORorEXTR          SCUCC_BTPSCR_BFIND_PORorEXTR
#define BFIND_POR                SCUCC_BTPSCR_BFIND_POR

// HIRC Fine Trim One Step Change Control
#define HIRC_UP_ONESTEP          0
#define HIRC_DOWN_ONESTEP        1

// WDTRC Fine Trim One Step Change Control
#define WDTRC_UP_ONESTEP         0
#define WDTRC_DOWN_ONESTEP       1


// MCLK Selection Control
/*
#define SCU_HIRC                 (0x0uL << 0)
#define SCU_XMOSC                (0x1uL << 0)
#define SCU_XSOSC                (0x2uL << 0)
#define SCU_WDTRC                (0x3uL << 0)
*/
#define MCLKSEL_HIRC             (SCUCG_SCCR_MCLKSEL_HIRC  << SCUCG_SCCR_MCLKSEL_Pos)
#define MCLKSEL_XMOSC            (SCUCG_SCCR_MCLKSEL_XMOSC << SCUCG_SCCR_MCLKSEL_Pos)
#define MCLKSEL_XSOSC            (SCUCG_SCCR_MCLKSEL_XSOSC << SCUCG_SCCR_MCLKSEL_Pos)
#define MCLKSEL_WDTRC            (SCUCG_SCCR_MCLKSEL_WDTRC << SCUCG_SCCR_MCLKSEL_Pos)

// HIRC Selection Control
/*
#define HIRC_40M                 (0x0uL << 12)
#define HIRC_20M                 (0x1uL << 12)
#define HIRC_10M                 (0x2uL << 12)
#define HIRC_5M                  (0x3uL << 12)
*/
#define HIRCSEL_HIRC1            (SCUCG_CLKSRCR_HIRCSEL_HIRC1 << SCUCG_CLKSRCR_HIRCSEL_Pos)  // 32MHz HIRC
#define HIRCSEL_HIRC2            (SCUCG_CLKSRCR_HIRCSEL_HIRC2 << SCUCG_CLKSRCR_HIRCSEL_Pos)  // 16MHz HIRC
#define HIRCSEL_HIRC4            (SCUCG_CLKSRCR_HIRCSEL_HIRC4 << SCUCG_CLKSRCR_HIRCSEL_Pos)  //  8MHz HIRC
#define HIRCSEL_HIRC8            (SCUCG_CLKSRCR_HIRCSEL_HIRC8 << SCUCG_CLKSRCR_HIRCSEL_Pos)  //  4MHz HIRC

// XMFRNG Selection Control
/*
#define XTAL_XM                  (0x0uL << 8)
#define EXT_XM                   (0x1uL << 8)
*/
#define XMFRNG_Xtal              (SCUCG_CLKSRCR_XMFRNG_Xtal  << SCUCG_CLKSRCR_XMFRNG_Pos)
#define XMFRNG_Clock             (SCUCG_CLKSRCR_XMFRNG_Clock << SCUCG_CLKSRCR_XMFRNG_Pos)

// System Clock Source Enable Control
/*
#define EN_XSOSC                 (0x1uL << 0)
#define EN_XMOSC                 (0x1uL << 1)
#define EN_HIRC                  (0x1uL << 2)
#define EN_WDTRC                 (0x1uL << 3)
*/
#define CLKSRCR_WDTRCEN          (0x1uL << SCUCG_CLKSRCR_WDTRCEN_Pos)
#define CLKSRCR_HIRCEN           (0x1uL << SCUCG_CLKSRCR_HIRCEN_Pos)
#define CLKSRCR_XMOSCEN          (0x1uL << SCUCG_CLKSRCR_XMOSCEN_Pos)
#define CLKSRCR_XSOSCEN          (0x1uL << SCUCG_CLKSRCR_XSOSCEN_Pos)

// RTCC & LCD Clock Divider Selection Control (Divider 2)
/*
#define MCLK_64                  0
#define MCLK_128                 1
#define MCLK_256                 2
#define MCLK_512                 3
#define MCLK_1024                4
*/
#define WLDIV_MCLK64             (SCUCG_SCDIVR1_WLDIV_MCLK64   << SCUCG_SCDIVR1_WLDIV_Pos)   // MCLK/64    
#define WLDIV_MCLK128            (SCUCG_SCDIVR1_WLDIV_MCLK128  << SCUCG_SCDIVR1_WLDIV_Pos)   // MCLK/128  
#define WLDIV_MCLK256            (SCUCG_SCDIVR1_WLDIV_MCLK256  << SCUCG_SCDIVR1_WLDIV_Pos)   // MCLK/256  
#define WLDIV_MCLK512            (SCUCG_SCDIVR1_WLDIV_MCLK512  << SCUCG_SCDIVR1_WLDIV_Pos)   // MCLK/512  
#define WLDIV_MCLK1024           (SCUCG_SCDIVR1_WLDIV_MCLK1024 << SCUCG_SCDIVR1_WLDIV_Pos)   // MCLK/1024

// HCLK Divider Selection Control (Divider 0)
/*
#define MCLK_16                  0
#define MCLK_8                   1
#define MCLK_4                   2
#define MCLK_2                   3
#define MCLK_1                   4
*/
#define HDIV_MCLK16              (SCUCG_SCDIVR1_HDIV_MCLK16 << SCUCG_SCDIVR1_HDIV_Pos)    // MCLK/16
#define HDIV_MCLK8               (SCUCG_SCDIVR1_HDIV_MCLK8  << SCUCG_SCDIVR1_HDIV_Pos)    // MCLK/8  
#define HDIV_MCLK4               (SCUCG_SCDIVR1_HDIV_MCLK4  << SCUCG_SCDIVR1_HDIV_Pos)    // MCLK/4  
#define HDIV_MCLK2               (SCUCG_SCDIVR1_HDIV_MCLK2  << SCUCG_SCDIVR1_HDIV_Pos)    // MCLK/2  
#define HDIV_MCLK1               (SCUCG_SCDIVR1_HDIV_MCLK1  << SCUCG_SCDIVR1_HDIV_Pos)    // MCLK/1  

// SysTick Timer Clock Divider Selection Control (Divider 3)
/*
#define SCU_HCLK_1               0
#define SCU_HCLK_2               1
#define SCU_HCLK_4               2
#define SCU_HCLK_8               3
*/
#define SYSTDIV_HCLK1            (SCUCG_SCDIVR2_SYSTDIV_HCLK1 << SCUCG_SCDIVR2_SYSTDIV_Pos)  // HCLK/1
#define SYSTDIV_HCLK2            (SCUCG_SCDIVR2_SYSTDIV_HCLK2 << SCUCG_SCDIVR2_SYSTDIV_Pos)  // HCLK/2
#define SYSTDIV_HCLK4            (SCUCG_SCDIVR2_SYSTDIV_HCLK4 << SCUCG_SCDIVR2_SYSTDIV_Pos)  // HCLK/4
#define SYSTDIV_HCLK8            (SCUCG_SCDIVR2_SYSTDIV_HCLK8 << SCUCG_SCDIVR2_SYSTDIV_Pos)  // HCLK/8

// PCLK Divider Selection Control (Divider 1)
#define PDIV_HCLK1               (SCUCG_SCDIVR2_PDIV_HCLK1 << SCUCG_SCDIVR2_PDIV_Pos)  // HCLK/1
#define PDIV_HCLK2               (SCUCG_SCDIVR2_PDIV_HCLK2 << SCUCG_SCDIVR2_PDIV_Pos)  // HCLK/2
#define PDIV_HCLK4               (SCUCG_SCDIVR2_PDIV_HCLK4 << SCUCG_SCDIVR2_PDIV_Pos)  // HCLK/4
#define PDIV_HCLK8               (SCUCG_SCDIVR2_PDIV_HCLK8 << SCUCG_SCDIVR2_PDIV_Pos)  // HCLK/8

// Clock Output Enable/Disable Control
/*
#define DIS_CLKOUT               0
#define EN_CLKOUT                1
*/
#define CLKOEN_Disable           SCUCG_CLKOCR_CLKOEN_Disable
#define CLKOEN_Enable            SCUCG_CLKOCR_CLKOEN_Enable

// Clock Output Polarity Selection Control
/*
#define POL_L                    (0x0uL << 6)
#define POL_H                    (0x1uL << 6)
*/
#define POLSEL_Low               (SCUCG_CLKOCR_POLSEL_Low  << SCUCG_CLKOCR_POLSEL_Pos)    // Low level during disable  
#define POLSEL_High              (SCUCG_CLKOCR_POLSEL_High << SCUCG_CLKOCR_POLSEL_Pos)    // High level during disable

// Clock Output Divider Selection Control
/*
#define CLKODIV_1                (0x0uL << 3)
#define CLKODIV_2                (0x1uL << 3)
#define CLKODIV_4                (0x2uL << 3)
#define CLKODIV_8                (0x3uL << 3)
#define CLKODIV_16               (0x4uL << 3)
#define CLKODIV_32               (0x5uL << 3)
#define CLKODIV_64               (0x6uL << 3)
#define CLKODIV_128              (0x7uL << 3)
*/
#define CLKODIV_SelectedClock1   (SCUCG_CLKOCR_CLKODIV_SelectedClock1   << SCUCG_CLKOCR_CLKODIV_Pos)  // Selected Clock/1    
#define CLKODIV_SelectedClock2   (SCUCG_CLKOCR_CLKODIV_SelectedClock2   << SCUCG_CLKOCR_CLKODIV_Pos)  // Selected Clock/2    
#define CLKODIV_SelectedClock4   (SCUCG_CLKOCR_CLKODIV_SelectedClock4   << SCUCG_CLKOCR_CLKODIV_Pos)  // Selected Clock/4    
#define CLKODIV_SelectedClock8   (SCUCG_CLKOCR_CLKODIV_SelectedClock8   << SCUCG_CLKOCR_CLKODIV_Pos)  // Selected Clock/8    
#define CLKODIV_SelectedClock16  (SCUCG_CLKOCR_CLKODIV_SelectedClock16  << SCUCG_CLKOCR_CLKODIV_Pos)  // Selected Clock/16  
#define CLKODIV_SelectedClock32  (SCUCG_CLKOCR_CLKODIV_SelectedClock32  << SCUCG_CLKOCR_CLKODIV_Pos)  // Selected Clock/32  
#define CLKODIV_SelectedClock64  (SCUCG_CLKOCR_CLKODIV_SelectedClock64  << SCUCG_CLKOCR_CLKODIV_Pos)  // Selected Clock/64  
#define CLKODIV_SelectedClock128 (SCUCG_CLKOCR_CLKODIV_SelectedClock128 << SCUCG_CLKOCR_CLKODIV_Pos)  // Selected Clock/128

// Clock Output Target Selection Control
/*
#define MCLK_OUT                 (0x0uL << 0)
#define WDTRC_OUT                (0x1uL << 0)
#define HIRC_OUT                 (0x2uL << 0)
#define HCLK_OUT                 (0x3uL << 0)
#define PCLK_OUT                 (0x4uL << 0)
*/
#define CLKOS_MCLK               (SCUCG_CLKOCR_CLKOS_MCLK  << SCUCG_CLKOCR_CLKOS_Pos)
#define CLKOS_WDTRC              (SCUCG_CLKOCR_CLKOS_WDTRC << SCUCG_CLKOCR_CLKOS_Pos)
#define CLKOS_HIRC               (SCUCG_CLKOCR_CLKOS_HIRC  << SCUCG_CLKOCR_CLKOS_Pos)
#define CLKOS_HCLK               (SCUCG_CLKOCR_CLKOS_HCLK  << SCUCG_CLKOCR_CLKOS_Pos)
#define CLKOS_PCLK               (SCUCG_CLKOCR_CLKOS_PCLK  << SCUCG_CLKOCR_CLKOS_Pos)

// Clock Monitoring Action Selection Control
/*
#define FLAG_CHK_M               (0x0uL << 5)
#define RST_GEN_M                (0x1uL << 5)
#define SYS_CHG_M                (0x2uL << 5)
*/
#define MACTS_FlagChk            (SCUCG_CMONCR_MACTS_FlagChk   << SCUCG_CMONCR_MACTS_Pos)
#define MACTS_RstGen             (SCUCG_CMONCR_MACTS_RstGen    << SCUCG_CMONCR_MACTS_Pos)
#define MACTS_SysClkChg          (SCUCG_CMONCR_MACTS_SysClkChg << SCUCG_CMONCR_MACTS_Pos)

// Clock Monitoring Target Selection Control
/*
#define MCLK_MON                 (0x0uL << 0)
#define HIRC_MON                 (0x1uL << 0)
#define XMOSC_MON                (0x2uL << 0)
#define XSOSC_MON                (0x3uL << 0)
*/
#define MONCS_MCLK               (SCUCG_CMONCR_MONCS_MCLK  << SCUCG_CMONCR_MONCS_Pos)
#define MONCS_HIRC               (SCUCG_CMONCR_MONCS_HIRC  << SCUCG_CMONCR_MONCS_Pos)
#define MONCS_XMOSC              (SCUCG_CMONCR_MONCS_XMOSC << SCUCG_CMONCR_MONCS_Pos)
#define MONCS_XSOSC              (SCUCG_CMONCR_MONCS_XSOSC << SCUCG_CMONCR_MONCS_Pos)

// Peripheral Clock Enable Control 1
/*
#define PERI_PA                  (0x1uL << 0)
#define PERI_PB                  (0x1uL << 1)
#define PERI_PC                  (0x1uL << 2)
#define PERI_PD                  (0x1uL << 3)
#define PERI_PE                  (0x1uL << 4)
#define PERI_PF                  (0x1uL << 5)
#define PERI_T13                 (0x1uL << 8)
#define PERI_T14                 (0x1uL << 9)
#define PERI_T15                 (0x1uL << 10)
#define PERI_T16                 (0x1uL << 11)
#define PERI_T10                 (0x1uL << 16)
#define PERI_T11                 (0x1uL << 17)
#define PERI_T12                 (0x1uL << 18)
#define PERI_T30                 (0x1uL << 19)
#define PERI_T20                 (0x1uL << 20)
#define PERI_T21                 (0x1uL << 21)
*/
#define PPCLKEN1_T50CLKE         (0x1uL << SCUCG_PPCLKEN1_T50CLKE_Pos)
#define PPCLKEN1_T43CLKE         (0x1uL << SCUCG_PPCLKEN1_T43CLKE_Pos)
#define PPCLKEN1_T42CLKE         (0x1uL << SCUCG_PPCLKEN1_T42CLKE_Pos)
#define PPCLKEN1_T41CLKE         (0x1uL << SCUCG_PPCLKEN1_T41CLKE_Pos)
#define PPCLKEN1_T40CLKE         (0x1uL << SCUCG_PPCLKEN1_T40CLKE_Pos)
#define PPCLKEN1_PFCLKE          (0x1uL << SCUCG_PPCLKEN1_PFCLKE_Pos)
#define PPCLKEN1_PECLKE          (0x1uL << SCUCG_PPCLKEN1_PECLKE_Pos)
#define PPCLKEN1_PDCLKE          (0x1uL << SCUCG_PPCLKEN1_PDCLKE_Pos)
#define PPCLKEN1_PCCLKE          (0x1uL << SCUCG_PPCLKEN1_PCCLKE_Pos)
#define PPCLKEN1_PBCLKE          (0x1uL << SCUCG_PPCLKEN1_PBCLKE_Pos)
#define PPCLKEN1_PACLKE          (0x1uL << SCUCG_PPCLKEN1_PACLKE_Pos)

// Peripheral Clock Enable Control 2
/*
#define PERI_UST10               (0x1uL << 0)
#define PERI_UST11               (0x1uL << 1)
#define PERI_UT0                 (0x1uL << 2)
#define PERI_UT1                 (0x1uL << 3)
#define PERI_UST12               (0x1uL << 4)
#define PERI_UST13               (0x1uL << 5)
#define PERI_I2C0                (0x1uL << 6)
#define PERI_I2C1                (0x1uL << 7)
#define PERI_I2C2                (0x1uL << 8)
#define PERI_ADC                 (0x1uL << 10)
#define PERI_CRC                 (0x1uL << 12)
#define PERI_LCD                 (0x1uL << 13)
#define PERI_WT                  (0x1uL << 16)
#define PERI_WDT                 (0x1uL << 17)
#define PERI_LVI                 (0x1uL << 18)
#define PERI_FMC                 (0x1uL << 19)
*/
#define PPCLKEN2_DMACLKE         (0x1uL << SCUCG_PPCLKEN2_DMACLKE_Pos)
#define PPCLKEN2_CMP1CLKE        (0x1uL << SCUCG_PPCLKEN2_CMP1CLKE_Pos)
#define PPCLKEN2_CMP0CLKE        (0x1uL << SCUCG_PPCLKEN2_CMP0CLKE_Pos)
#define PPCLKEN2_SC1CLKE         (0x1uL << SCUCG_PPCLKEN2_SC1CLKE_Pos)
#define PPCLKEN2_SC0CLKE         (0x1uL << SCUCG_PPCLKEN2_SC0CLKE_Pos)
#define PPCLKEN2_SPI1CLKE        (0x1uL << SCUCG_PPCLKEN2_SPI1CLKE_Pos)
#define PPCLKEN2_SPI0CLKE        (0x1uL << SCUCG_PPCLKEN2_SPI0CLKE_Pos)
#define PPCLKEN2_FMCLKE          (0x1uL << SCUCG_PPCLKEN2_FMCLKE_Pos)
#define PPCLKEN2_LVICLKE         (0x1uL << SCUCG_PPCLKEN2_LVICLKE_Pos)
#define PPCLKEN2_WDTCLKE         (0x1uL << SCUCG_PPCLKEN2_WDTCLKE_Pos)
#define PPCLKEN2_LPUTCLKE        (0x1uL << SCUCG_PPCLKEN2_LPUTCLKE_Pos)
#define PPCLKEN2_LCDCLKE         (0x1uL << SCUCG_PPCLKEN2_LCDCLKE_Pos)
#define PPCLKEN2_CRCLKE          (0x1uL << SCUCG_PPCLKEN2_CRCLKE_Pos)
#define PPCLKEN2_RTCCLKE         (0x1uL << SCUCG_PPCLKEN2_RTCCLKE_Pos)
#define PPCLKEN2_ADCLKE          (0x1uL << SCUCG_PPCLKEN2_ADCLKE_Pos)
#define PPCLKEN2_I2C1CLKE        (0x1uL << SCUCG_PPCLKEN2_I2C1CLKE_Pos)
#define PPCLKEN2_I2C0CLKE        (0x1uL << SCUCG_PPCLKEN2_I2C0CLKE_Pos)
#define PPCLKEN2_UT1CLKE         (0x1uL << SCUCG_PPCLKEN2_UT1CLKE_Pos)
#define PPCLKEN2_UT0CLKE         (0x1uL << SCUCG_PPCLKEN2_UT0CLKE_Pos)
#define PPCLKEN2_UST10CLKE       (0x1uL << SCUCG_PPCLKEN2_UST10CLKE_Pos)

// Peripheral Clock Enable/Disable Control
/*
#define DIS_PERICLK              0
#define EN_PERICLK               1
*/
#define PPxCLKE_Disable          SCUCG_PPCLKEN1_PACLKE_Disable
#define PPxCLKE_Enable           SCUCG_PPCLKEN1_PACLKE_Enable

// Timer/Counter 50 Clock Selection Control
#define PPCLKSR_T50CLK           (SCUCG_PPCLKSR_T50CLK_Msk)
#define T50CLK_PCLK              (SCUCG_PPCLKSR_T50CLK_PCLK  << SCUCG_PPCLKSR_T50CLK_Pos)
#define T50CLK_WDTRC             (SCUCG_PPCLKSR_T50CLK_WDTRC << SCUCG_PPCLKSR_T50CLK_Pos)
#define T50CLK_HIRC              (SCUCG_PPCLKSR_T50CLK_HIRC  << SCUCG_PPCLKSR_T50CLK_Pos)
#define T50CLK_XSOSC             (SCUCG_PPCLKSR_T50CLK_XSOSC << SCUCG_PPCLKSR_T50CLK_Pos)

// Low Power UART Clock Selection Control
#define PPCLKSR_LPUTCLK          (SCUCG_PPCLKSR_LPUTCLK_Msk)
#define LPUTCLK_PCLK             (SCUCG_PPCLKSR_LPUTCLK_PCLK  << SCUCG_PPCLKSR_LPUTCLK_Pos)
#define LPUTCLK_HIRC             (SCUCG_PPCLKSR_LPUTCLK_HIRC  << SCUCG_PPCLKSR_LPUTCLK_Pos)
#define LPUTCLK_XSOSC            (SCUCG_PPCLKSR_LPUTCLK_XSOSC << SCUCG_PPCLKSR_LPUTCLK_Pos)

// Real Time Clock/Calendar Clock Selection Control
#define PPCLKSR_RTCCLK           (SCUCG_PPCLKSR_RTCCLK_Msk)
#define RTCCLK_LowLevel          (SCUCG_PPCLKSR_RTCCLK_LowLevel    << SCUCG_PPCLKSR_RTCCLK_Pos)
#define RTCCLK_XSOSC             (SCUCG_PPCLKSR_RTCCLK_XSOSC       << SCUCG_PPCLKSR_RTCCLK_Pos)
#define RTCCLK_WDTRC             (SCUCG_PPCLKSR_RTCCLK_WDTRC       << SCUCG_PPCLKSR_RTCCLK_Pos)
#define RTCCLK_DividedMCLK       (SCUCG_PPCLKSR_RTCCLK_DividedMCLK << SCUCG_PPCLKSR_RTCCLK_Pos)

// LCD Driver Clock Selection Control
#define PPCLKSR_LCDCLK           (SCUCG_PPCLKSR_LCDCLK_Msk)
#define LCDCLK_DividedMCLK       (SCUCG_PPCLKSR_LCDCLK_DividedMCLK << SCUCG_PPCLKSR_LCDCLK_Pos)
#define LCDCLK_XSOSC             (SCUCG_PPCLKSR_LCDCLK_XSOSC       << SCUCG_PPCLKSR_LCDCLK_Pos)
#define LCDCLK_WDTRC             (SCUCG_PPCLKSR_LCDCLK_WDTRC       << SCUCG_PPCLKSR_LCDCLK_Pos)

// Watch-Dog Timer Clock Selection Control
/*
#define SCUCG_WDTCLK             (SCUCG_PPCLKSR_WDTCLK_Msk)
#define SCUCG_WDTCLK_WDTRC       (0x0uL << SCUCG_PPCLKSR_WDTCLK_Pos)
#define SCUCG_WDTCLK_PCLK        (0x1uL << SCUCG_PPCLKSR_WDTCLK_Pos)
*/
#define PPCLKSR_WDTCLK           (SCUCG_PPCLKSR_WDTCLK_Msk)
#define WDTCLK_WDTRC             (SCUCG_PPCLKSR_WDTCLK_WDTRC << SCUCG_PPCLKSR_WDTCLK_Pos)
#define WDTCLK_PCLK              (SCUCG_PPCLKSR_WDTCLK_PCLK  << SCUCG_PPCLKSR_WDTCLK_Pos)

// Peripheral Reset Control 1
#define PPRST1_T50RST            (0x1uL << SCUCG_PPRST1_T50RST_Pos)
#define PPRST1_T43RST            (0x1uL << SCUCG_PPRST1_T43RST_Pos)
#define PPRST1_T42RST            (0x1uL << SCUCG_PPRST1_T42RST_Pos)
#define PPRST1_T41RST            (0x1uL << SCUCG_PPRST1_T41RST_Pos)
#define PPRST1_T40RST            (0x1uL << SCUCG_PPRST1_T40RST_Pos)
#define PPRST1_PFRST             (0x1uL << SCUCG_PPRST1_PFRST_Pos)
#define PPRST1_PERST             (0x1uL << SCUCG_PPRST1_PERST_Pos)
#define PPRST1_PDRST             (0x1uL << SCUCG_PPRST1_PDRST_Pos)
#define PPRST1_PCRST             (0x1uL << SCUCG_PPRST1_PCRST_Pos)
#define PPRST1_PBRST             (0x1uL << SCUCG_PPRST1_PBRST_Pos)
#define PPRST1_PARST             (0x1uL << SCUCG_PPRST1_PARST_Pos)

// Peripheral Reset Control 2
#define PPRST2_DMARST            (0x1uL << SCUCG_PPRST2_DMARST_Pos)
#define PPRST2_CMP1RST           (0x1uL << SCUCG_PPRST2_CMP1RST_Pos)
#define PPRST2_CMP0RST           (0x1uL << SCUCG_PPRST2_CMP0RST_Pos)
#define PPRST2_SC1RST            (0x1uL << SCUCG_PPRST2_SC1RST_Pos)
#define PPRST2_SC0RST            (0x1uL << SCUCG_PPRST2_SC0RST_Pos)
#define PPRST2_SPI1RST           (0x1uL << SCUCG_PPRST2_SPI1RST_Pos)
#define PPRST2_SPI0RST           (0x1uL << SCUCG_PPRST2_SPI0RST_Pos)
#define PPRST2_FMCRST            (0x1uL << SCUCG_PPRST2_FMCRST_Pos)
#define PPRST2_LVIRST            (0x1uL << SCUCG_PPRST2_LVIRST_Pos)
#define PPRST2_LPUTRST           (0x1uL << SCUCG_PPRST2_LPUTRST_Pos)
#define PPRST2_LCDRST            (0x1uL << SCUCG_PPRST2_LCDRST_Pos)
#define PPRST2_CRRST             (0x1uL << SCUCG_PPRST2_CRRST_Pos)
#define PPRST2_RTCRST            (0x1uL << SCUCG_PPRST2_RTCRST_Pos)
#define PPRST2_ADRST             (0x1uL << SCUCG_PPRST2_ADRST_Pos)
#define PPRST2_I2C1RST           (0x1uL << SCUCG_PPRST2_I2C1RST_Pos)
#define PPRST2_I2C0RST           (0x1uL << SCUCG_PPRST2_I2C0RST_Pos)
#define PPRST2_UT1RST            (0x1uL << SCUCG_PPRST2_UT1RST_Pos)
#define PPRST2_UT0RST            (0x1uL << SCUCG_PPRST2_UT0RST_Pos)
#define PPRST2_UST10RST          (0x1uL << SCUCG_PPRST2_UST10RST_Pos)

//******************************************************************************
// Macro
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Get "Vendor ID"/"Chip ID"/Revision Number"
 * @details       This macro gets vendor ID, chip ID, and revision number.
 *//*-------------------------------------------------------------------------*/
#define SCUCC_GetVendorID()         (SCUCC->VENDORID)
#define SCUCC_GetChipID()           (SCUCC->CHIPID)
#define SCUCC_GetRevNo()            (SCUCC->REVNR)

/*-------------------------------------------------------------------------*//**
 * @brief         Set Boot Pin Function
 * @param[in]     rst_src
 *                   Reset sources to check boot pin
 *                   -  BFIND_PORorEXTR, BFIND_POR
 * @details       This macro sets boot pin function to check when reset occurs.
 *//*-------------------------------------------------------------------------*/
#define SCUCC_SetBtFnc(rst_src)     (SCUCC->BTPSCR_b.BFIND = rst_src)

/*-------------------------------------------------------------------------*//**
 * @brief         Get Boot Pin Status
 * @details       This macro gets boot pin status.
 *//*-------------------------------------------------------------------------*/
#define SCUCC_GetBtPinSt()          (SCUCC->BTPSCR_b.BTPSTA)

/*-------------------------------------------------------------------------*//**
 * @brief         Enable/Disable NMI Interrupt
 * @details       This macro sets the NMI interrupt control bit.
 *//*-------------------------------------------------------------------------*/
#define SCUCC_EnNMI()               (SCUCC->NMISRCR_b.NMICON = 1)
#define SCUCC_DisNMI()              (SCUCC->NMISRCR_b.NMICON = 0)

/*-------------------------------------------------------------------------*//**
 * @brief         Generate Software Reset
 * @details       This macro generates software reset.
 *//*-------------------------------------------------------------------------*/
#define SCUCC_GenSwRst()            (SCUCC->SWRSTR = ((uint32_t)SCUCC_SWRSTR_WTIDKY_Value << SCUCC_SWRSTR_WTIDKY_Pos) | 0x2DuL)

/*-------------------------------------------------------------------------*//**
 * @brief         Enable/Disable Wake-Up Timer Interrupt
 * @details       This macro sets the wake-up timer interrupt enable bit.
 *//*-------------------------------------------------------------------------*/
#define SCUCC_EnWutInt()            (SCUCC->WUTCR_b.WUTIEN = 1)
#define SCUCC_DisWutInt()           (SCUCC->WUTCR_b.WUTIEN = 0)

/*-------------------------------------------------------------------------*//**
 * @brief         Get/Clear Wake-Up Timer Interrupt Flag
 * @details       This macro gets/clears the wake-up timer interrupt flag.
 *//*-------------------------------------------------------------------------*/
#define SCUCC_GetWutFlag()          (SCUCC->WUTCR_b.WUTIFLAG)
#define SCUCC_ClrWutFlag()          (SCUCC->WUTCR_b.WUTIFLAG = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         Reload Counter of Wake-Up Timer
 * @details       This macro reloads wake-up timer counter to re-count.
 *//*-------------------------------------------------------------------------*/
#define SCUCC_ReloadWut()           (SCUCC->WUTCR_b.CNTRLD = 1)


/*-------------------------------------------------------------------------*//**
 * @brief         Set Divider 0/1/2/3
 * @param[in]     scu_hdiv
 *                   -  scu_hdiv:      HDIV_MCLK16, HDIV_MCLK8, HDIV_MCLK4, HDIV_MCLK2, HDIV_MCLK1
 *                   -  scu_pdiv:      PDIV_HCLK1, PDIV_HCLK2, PDIV_HCLK4, PDIV_HCLK8
 *                   -  scu_wldiv:     WLDIV_MCLK64, WLDIV_MCLK128, WLDIV_MCLK256, WLDIV_MCLK512, WLDIV_MCLK1024
 *                   -  scu_systdiv:   SYSTDIV_HCLK1, SYSTDIV_HCLK2, SYSTDIV_HCLK4, SYSTDIV_HCLK8
 * @details       This macro sets dividers.
 *//*-------------------------------------------------------------------------*/
#define SCUCG_SetHCLK( scu_hdiv )            (SCUCG->SCDIVR1_b.HDIV = scu_hdiv)
#define SCUCG_SetPCLK( scu_pdiv )            (SCUCG->SCDIVR2_b.PDIV = scu_pdiv)
#define SCUCG_SetWtLcd( scu_wldiv )          (SCUCG->SCDIVR1_b.WLDIV = scu_wldiv)
#define SCUCG_SetSysTick( scu_systdiv )      (SCUCG->SCDIVR2_b.SYSTDIV = scu_systdiv)

/*-------------------------------------------------------------------------*//**
 * @brief         Set Clock Output Control Register (CLKOCR)
 * @param[in]     u32Clko
 *                   Values for CLKO frequency, Polarity, and Divider
 * @details       This macro sets clock output control register for CLKO.
 *//*-------------------------------------------------------------------------*/
#define SCUCG_SetClkOutReg( u32Clko )        (SCUCG->CLKOCR = u32Clko)

/*-------------------------------------------------------------------------*//**
 * @brief         Get MONFLAG of Clock Monitoring
 * @details       This macro reads MONFLAG of Clock Monitoring.
 *//*-------------------------------------------------------------------------*/
#define SCUCG_GetMonFlag()                   (SCUCG->CMONCR_b.MONFLAG)

/*-------------------------------------------------------------------------*//**
 * @brief         Set Timer/Counter 50 Clock
 * @param[in]     clk
 *                   -  T50CLK_PCLK, T50CLK_WDTRC, T50CLK_HIRC, T50CLK_XSOSC
 * @details       This macro sets Timer/Counter 50 clock.
 *//*-------------------------------------------------------------------------*/
#define SCUCG_SetT50Clk( clk )               (SCUCG->PPCLKSR_b.T50CLK = clk)

/*-------------------------------------------------------------------------*//**
 * @brief         Set Low Power UART Clock
 * @param[in]     clk
 *                   -  LPUTCLK_PCLK, LPUTCLK_HIRC, LPUTCLK_XSOSC
 * @details       This macro sets Low Power UART clock.
 *//*-------------------------------------------------------------------------*/
#define SCUCG_SetLPUTClk( clk )              (SCUCG->PPCLKSR_b.LPUTCLK = clk)

/*-------------------------------------------------------------------------*//**
 * @brief         Set Real Time Clock/Calendar Clock
 * @param[in]     clk
 *                   -  RTCCLK_LowLevel, RTCCLK_XSOSC, RTCCLK_WDTRC, RTCCLK_DividedMCLK
 * @details       This macro sets Real Time Clock/Calendar clock.
 *//*-------------------------------------------------------------------------*/
#define SCUCG_SetRtccClk( clk )              (SCUCG->PPCLKSR_b.RTCCLK = clk)

/*-------------------------------------------------------------------------*//**
 * @brief         Set LCD Driver Clock
 * @param[in]     clk
 *                   -  LCDCLK_DividedMCLK, LCDCLK_XSOSC, LCDCLK_WDTRC
 * @details       This macro sets LCD Driver clock.
 *//*-------------------------------------------------------------------------*/
#define SCUCG_SetLcdClk( clk )               (SCUCG->PPCLKSR_b.LCDCLK = clk)

/*-------------------------------------------------------------------------*//**
 * @brief         Set Watch-Dog Timer Clock
 * @param[in]     clk
 *                   -  WDTCLK_WDTRC, WDTCLK_PCLK
 * @details       This macro sets Watch-Dog Timer clock.
 *//*-------------------------------------------------------------------------*/
#define SCUCG_SetWdtClk( clk )               (SCUCG->PPCLKSR_b.WDTCLK = clk)

//******************************************************************************
// Function
//******************************************************************************

uint32_t HAL_SCU_ResetSourceStatus( void );
void HAL_SCU_SetNMI( uint32_t u32NmiCon );
void HAL_SCU_SoftwareReset_Config( void );
void HAL_SCU_SetWakupData( uint32_t u32Data );
void HAL_SCU_HIRCTRM_ClockConfig( uint32_t u32Ind );
void HAL_SCU_WDTRCTRM_ClockConfig( uint32_t u32Ind );


void HAL_SCU_ClockMonitoring( uint32_t u32Acts, uint32_t u32Target );
void HAL_SCU_ClockMonitoring_Disable( void );
void HAL_SCU_ClockSource_Config( uint32_t u32FreIRC, uint32_t u32TypeXM, uint32_t u32ClkSrc );
void HAL_SCU_ClockSource_Enable( uint32_t u32ClkSrc, uint32_t u32HircDiv );
void HAL_SCU_ClockSource_Disable( uint32_t u32ClkSrc );
void HAL_SCU_SystemClockChange( uint32_t u32Target );
void HAL_SCU_MainXtal_PinConfig( void );
void HAL_SCU_SubXtal_PinConfig( void );
void HAL_SCU_SystemClockDivider( uint32_t u32Div02, uint32_t u32Div13 );
void HAL_SCU_CLKO_PinConfig( Pn_Type* Px, uint8_t pin_no );
void HAL_SCU_ClockOutput( uint32_t u32ClkSrc, uint32_t u32Level, uint32_t u32Div );
void HAL_SCU_Peripheral_ClockConfig( uint32_t u32PeriClk1, uint32_t u32PeriClk2 );
void HAL_SCU_Peripheral_EnableClock1( uint32_t u32PeriClk1, uint32_t Ind );
void HAL_SCU_Peripheral_EnableClock2( uint32_t u32PeriClk2, uint32_t u32Ind );
void HAL_SCU_Peripheral_ResetConfig( uint32_t u32PeriRst1, uint32_t u32PeriRst2 );
void HAL_SCU_Peripheral_SetReset1( uint32_t u32EachPeri1 );
void HAL_SCU_Peripheral_SetReset2( uint32_t u32EachPeri2 );
void HAL_SCU_Peripheral_ClockSelection( uint32_t u32Peri, uint32_t u32ClkSrc );

#ifdef __cplusplus
}
#endif

#endif   /* _SCU_H_ */


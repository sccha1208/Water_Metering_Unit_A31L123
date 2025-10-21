/**
 *******************************************************************************
 * @file        A31L12x_PortInit.c
 * @author      ABOV R&D Division
 * @brief       Port Initialization Source File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#include "main_conf.h"

/* Private typedef ---------------------------------------------------------- */
/* Private define ----------------------------------------------------------- */
/* Private macro ------------------------------------------------------------ */
/* Private variables -------------------------------------------------------- */
/* Private define ----------------------------------------------------------- */


/* Private function prototypes ---------------------------------------------- */

void Port_Init( void );


/* Private variables -------------------------------------------------------- */


/*-------------------------------------------------------------------------*//**
 * @brief         This function initializes all ports.
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void Port_Init( void )
{
   // enable peripheral clock
   HAL_SCU_Peripheral_ClockConfig( 0x3f, 0x0 );   // enable all ports,

   // Port A
   PA->MOD     =  0
                  | ( 1 << PA_PA_MOD_MODE9_Pos )         // PA9            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PA_PA_MOD_MODE8_Pos )         // PA8            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PA_PA_MOD_MODE7_Pos )         // PA7            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PA_PA_MOD_MODE6_Pos )         // PA6            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PA_PA_MOD_MODE5_Pos )         // PA5            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PA_PA_MOD_MODE4_Pos )         // PA4            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PA_PA_MOD_MODE3_Pos )         // PA3            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PA_PA_MOD_MODE2_Pos )         // PA2            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PA_PA_MOD_MODE1_Pos )         // PA1            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PA_PA_MOD_MODE0_Pos )         // PA0            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  ;

   PA->TYP     =  0
                  | ( 0 << PA_PA_TYP_TYP9_Pos )          // PA9            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PA_PA_TYP_TYP8_Pos )          // PA8            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PA_PA_TYP_TYP7_Pos )          // PA7            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PA_PA_TYP_TYP6_Pos )          // PA6            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PA_PA_TYP_TYP5_Pos )          // PA5            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PA_PA_TYP_TYP4_Pos )          // PA4            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PA_PA_TYP_TYP3_Pos )          // PA3            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PA_PA_TYP_TYP2_Pos )          // PA2            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PA_PA_TYP_TYP1_Pos )          // PA1            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PA_PA_TYP_TYP0_Pos )          // PA0            0: Push-Pull Output     1: Open-Drain Output
                  ;

   PA->AFSR2   =  0
                  | ( 0 << PA_PA_AFSR2_AFSR9_Pos )       // PA9            0: ----        1: ----        2: LPRXD       3: ----        4: AN9         5: ----        6: ----        7: SEG13
                  | ( 0 << PA_PA_AFSR2_AFSR8_Pos )       // PA8            0: ----        1: ----        2: LPTXD       3: ----        4: AN8         5: ----        6: ----        7: SEG14
                  ;

   PA->AFSR1   =  0
                  | ( 0 << PA_PA_AFSR1_AFSR7_Pos )       // PA7            0: T43OUTB     1: EC43        2: ----        3: MOSI1       4: AN7         5: CP1P1       6: CP1OUT      7: SEG15
                  | ( 0 << PA_PA_AFSR1_AFSR6_Pos )       // PA6            0: T43OUTA     1: T43INP      2: ----        3: MISO1       4: AN6         5: ----        6: CP0OUT      7: SEG16
                  | ( 0 << PA_PA_AFSR1_AFSR5_Pos )       // PA5            0: T40OUTA     1: T40INP      2: ----        3: SCK1        4: AN5         5: CP0N2       6: CP1N2       7: ----
                  | ( 0 << PA_PA_AFSR1_AFSR4_Pos )       // PA4            0: ----        1: T43INP      2: SS1         3: SCK10       4: AN4         5: CP0N1       6: CP1N1       7: ----
                  | ( 0 << PA_PA_AFSR1_AFSR3_Pos )       // PA3            0: T41OUTB     1: EC41        2: RXD10       3: MISO10      4: AN3         5: CP1P0       6: ADTRG       7: SEG17
                  | ( 0 << PA_PA_AFSR1_AFSR2_Pos )       // PA2            0: T41OUTA     1: T41INP      2: TXD10       3: MOSI10      4: AN2         5: CP1N0       6: CP1OUT      7: SEG18
                  | ( 0 << PA_PA_AFSR1_AFSR1_Pos )       // PA1            0: T40OUTB     1: EC40        2: T42INP      3: SS10        4: AN1         5: CP0P0       6: ----        7: SEG19
                  | ( 0 << PA_PA_AFSR1_AFSR0_Pos )       // PA0            0: T40OUTA     1: T40INP      2: ----        3: ----        4: AN0         5: CP0N0       6: CP0OUT      7: ----
                  ;

   PA->PUPD    =  0
                  | ( 0 << PA_PA_PUPD_PUPD9_Pos )        // PA9            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PA_PA_PUPD_PUPD8_Pos )        // PA8            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PA_PA_PUPD_PUPD7_Pos )        // PA7            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PA_PA_PUPD_PUPD6_Pos )        // PA6            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PA_PA_PUPD_PUPD5_Pos )        // PA5            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PA_PA_PUPD_PUPD4_Pos )        // PA4            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PA_PA_PUPD_PUPD3_Pos )        // PA3            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PA_PA_PUPD_PUPD2_Pos )        // PA2            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PA_PA_PUPD_PUPD1_Pos )        // PA1            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PA_PA_PUPD_PUPD0_Pos )        // PA0            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  ;

   PA->OUTDR   =  0
                  | ( 0 << PA_PA_OUTDR_OUTDR9_Pos )      // PA9            0: Output Low     1: Output High
                  | ( 0 << PA_PA_OUTDR_OUTDR8_Pos )      // PA8            0: Output Low     1: Output High
                  | ( 0 << PA_PA_OUTDR_OUTDR7_Pos )      // PA7            0: Output Low     1: Output High
                  | ( 0 << PA_PA_OUTDR_OUTDR6_Pos )      // PA6            0: Output Low     1: Output High
                  | ( 0 << PA_PA_OUTDR_OUTDR5_Pos )      // PA5            0: Output Low     1: Output High
                  | ( 0 << PA_PA_OUTDR_OUTDR4_Pos )      // PA4            0: Output Low     1: Output High
                  | ( 0 << PA_PA_OUTDR_OUTDR3_Pos )      // PA3            0: Output Low     1: Output High
                  | ( 0 << PA_PA_OUTDR_OUTDR2_Pos )      // PA2            0: Output Low     1: Output High
                  | ( 0 << PA_PA_OUTDR_OUTDR1_Pos )      // PA1            0: Output Low     1: Output High
                  | ( 0 << PA_PA_OUTDR_OUTDR0_Pos )      // PA0            0: Output Low     1: Output High
                  ;

   // Port B
   PB->MOD     =  0
                  | ( 1 << PB_PB_MOD_MODE12_Pos )        // PB12           0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PB_PB_MOD_MODE11_Pos )        // PB11           0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PB_PB_MOD_MODE10_Pos )        // PB10           0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PB_PB_MOD_MODE9_Pos )         // PB9            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PB_PB_MOD_MODE8_Pos )         // PB8            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PB_PB_MOD_MODE7_Pos )         // PB7            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PB_PB_MOD_MODE6_Pos )         // PB6            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PB_PB_MOD_MODE5_Pos )         // PB5            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PB_PB_MOD_MODE4_Pos )         // PB4            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PB_PB_MOD_MODE3_Pos )         // PB3            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PB_PB_MOD_MODE2_Pos )         // PB2            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PB_PB_MOD_MODE1_Pos )         // PB1            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PB_PB_MOD_MODE0_Pos )         // PB0            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  ;

   PB->TYP     =  0
                  | ( 0 << PB_PB_TYP_TYP12_Pos )         // PB12           0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PB_PB_TYP_TYP11_Pos )         // PB11           0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PB_PB_TYP_TYP10_Pos )         // PB10           0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PB_PB_TYP_TYP9_Pos )          // PB9            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PB_PB_TYP_TYP8_Pos )          // PB8            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PB_PB_TYP_TYP7_Pos )          // PB7            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PB_PB_TYP_TYP6_Pos )          // PB6            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PB_PB_TYP_TYP5_Pos )          // PB5            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PB_PB_TYP_TYP4_Pos )          // PB4            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PB_PB_TYP_TYP3_Pos )          // PB3            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PB_PB_TYP_TYP2_Pos )          // PB2            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PB_PB_TYP_TYP1_Pos )          // PB1            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PB_PB_TYP_TYP0_Pos )          // PB0            0: Push-Pull Output     1: Open-Drain Output
                  ;

   PB->AFSR2   =  0
                  | ( 0 << PB_PB_AFSR2_AFSR12_Pos )      // PB12           0: T42INP      1: ----        2: ----        3: ----        4: ----        5: ----        6: ----        7: SEG0
                  | ( 0 << PB_PB_AFSR2_AFSR11_Pos )      // PB11           0: T43INP      1: ----        2: ----        3: ----        4: ----        5: ----        6: ----        7: SEG1
                  | ( 0 << PB_PB_AFSR2_AFSR10_Pos )      // PB10           0: T43OUTB     1: EC43        2: ----        3: ----        4: ----        5: ----        6: ----        7: SEG2
                  | ( 0 << PB_PB_AFSR2_AFSR9_Pos )       // PB9            0: T43OUTA     1: T43INP      2: ----        3: ----        4: ----        5: ----        6: ----        7: SEG3
                  | ( 0 << PB_PB_AFSR2_AFSR8_Pos )       // PB8            0: ----        1: ----        2: ----        3: MOSI0       4: ----        5: ----        6: ----        7: SEG4
                  ;

   PB->AFSR1   =  0
                  | ( 0 << PB_PB_AFSR1_AFSR7_Pos )       // PB7            0: T42OUTB     1: EC42        2: RTCOUT      3: MISO0       4: SDA1        5: LPDE        6: ADTRG       7: SEG5
                  | ( 0 << PB_PB_AFSR1_AFSR6_Pos )       // PB6            0: T42OUTA     1: T42INP      2: ----        3: SCK0        4: SCL1        5: ----        6: ----        7: SEG6
                  | ( 0 << PB_PB_AFSR1_AFSR5_Pos )       // PB5            0: ----        1: ----        2: ----        3: SS0         4: ----        5: LPDE        6: ----        7: SEG7
                  | ( 0 << PB_PB_AFSR1_AFSR4_Pos )       // PB4            0: T41OUTB     1: EC41        2: LPRXD       3: ----        4: SDA1        5: ----        6: ADTRG       7: SEG8
                  | ( 0 << PB_PB_AFSR1_AFSR3_Pos )       // PB3            0: T41OUTA     1: T41INP      2: LPTXD       3: SCK0        4: SCL1        5: ----        6: ADTRG       7: SEG9
                  | ( 0 << PB_PB_AFSR1_AFSR2_Pos )       // PB2            0: T50OUT      1: ----        2: ----        3: ----        4: ----        5: ----        6: ----        7: SEG10
                  | ( 0 << PB_PB_AFSR1_AFSR1_Pos )       // PB1            0: ----        1: ----        2: RXD1        3: ----        4: AN11        5: LPDE        6: ----        7: SEG11
                  | ( 0 << PB_PB_AFSR1_AFSR0_Pos )       // PB0            0: ----        1: ----        2: TXD1        3: ----        4: AN10        5: ----        6: ----        7: SEG12
                  ;

   PB->PUPD    =  0
                  | ( 0 << PB_PB_PUPD_PUPD12_Pos )       // PB12           0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PB_PB_PUPD_PUPD11_Pos )       // PB11           0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PB_PB_PUPD_PUPD10_Pos )       // PB10           0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PB_PB_PUPD_PUPD9_Pos )        // PB9            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PB_PB_PUPD_PUPD8_Pos )        // PB8            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PB_PB_PUPD_PUPD7_Pos )        // PB7            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PB_PB_PUPD_PUPD6_Pos )        // PB6            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PB_PB_PUPD_PUPD5_Pos )        // PB5            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PB_PB_PUPD_PUPD4_Pos )        // PB4            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PB_PB_PUPD_PUPD3_Pos )        // PB3            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PB_PB_PUPD_PUPD2_Pos )        // PB2            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PB_PB_PUPD_PUPD1_Pos )        // PB1            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PB_PB_PUPD_PUPD0_Pos )        // PB0            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  ;

   PB->OUTDR   =  0
                  | ( 0 << PB_PB_OUTDR_OUTDR12_Pos )     // PB12           0: Output Low     1: Output High
                  | ( 0 << PB_PB_OUTDR_OUTDR11_Pos )     // PB11           0: Output Low     1: Output High
                  | ( 0 << PB_PB_OUTDR_OUTDR10_Pos )     // PB10           0: Output Low     1: Output High
                  | ( 0 << PB_PB_OUTDR_OUTDR9_Pos )      // PB9            0: Output Low     1: Output High
                  | ( 0 << PB_PB_OUTDR_OUTDR8_Pos )      // PB8            0: Output Low     1: Output High
                  | ( 0 << PB_PB_OUTDR_OUTDR7_Pos )      // PB7            0: Output Low     1: Output High
                  | ( 0 << PB_PB_OUTDR_OUTDR6_Pos )      // PB6            0: Output Low     1: Output High
                  | ( 0 << PB_PB_OUTDR_OUTDR5_Pos )      // PB5            0: Output Low     1: Output High
                  | ( 0 << PB_PB_OUTDR_OUTDR4_Pos )      // PB4            0: Output Low     1: Output High
                  | ( 0 << PB_PB_OUTDR_OUTDR3_Pos )      // PB3            0: Output Low     1: Output High
                  | ( 0 << PB_PB_OUTDR_OUTDR2_Pos )      // PB2            0: Output Low     1: Output High
                  | ( 0 << PB_PB_OUTDR_OUTDR1_Pos )      // PB1            0: Output Low     1: Output High
                  | ( 0 << PB_PB_OUTDR_OUTDR0_Pos )      // PB0            0: Output Low     1: Output High
                  ;

   // Port C
   PC->MOD     =  0
                  | ( 1 << PC_PC_MOD_MODE11_Pos )        // PC11           0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PC_PC_MOD_MODE10_Pos )        // PC10           0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PC_PC_MOD_MODE9_Pos )         // PC9            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PC_PC_MOD_MODE8_Pos )         // PC8            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PC_PC_MOD_MODE7_Pos )         // PC7            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 2 << PC_PC_MOD_MODE6_Pos )         // PC6   SWCLK    0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 2 << PC_PC_MOD_MODE5_Pos )         // PC5   SWDIO    0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PC_PC_MOD_MODE4_Pos )         // PC4            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PC_PC_MOD_MODE3_Pos )         // PC3            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PC_PC_MOD_MODE2_Pos )         // PC2            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PC_PC_MOD_MODE1_Pos )         // PC1   CLKO     0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PC_PC_MOD_MODE0_Pos )         // PC0   CLKO     0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  ;

   PC->TYP     =  0
                  | ( 0 << PC_PC_TYP_TYP11_Pos )         // PC11           0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PC_PC_TYP_TYP10_Pos )         // PC10           0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PC_PC_TYP_TYP9_Pos )          // PC9            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PC_PC_TYP_TYP8_Pos )          // PC8            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PC_PC_TYP_TYP7_Pos )          // PC7            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PC_PC_TYP_TYP6_Pos )          // PC6   SWCLK    0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PC_PC_TYP_TYP5_Pos )          // PC5   SWDIO    0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PC_PC_TYP_TYP4_Pos )          // PC4            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PC_PC_TYP_TYP3_Pos )          // PC3            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PC_PC_TYP_TYP2_Pos )          // PC2            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PC_PC_TYP_TYP1_Pos )          // PC1   CLKO     0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PC_PC_TYP_TYP0_Pos )          // PC0   CLKO     0: Push-Pull Output     1: Open-Drain Output
                  ;

   PC->AFSR2   =  0
                  | ( 0 << PC_PC_AFSR2_AFSR11_Pos )      // PC11           0: ----        1: ----        2: RXD1        3: ----        4: ----        5: LPDE        6: ----        7: COM7/SEG28
                  | ( 0 << PC_PC_AFSR2_AFSR10_Pos )      // PC10           0: ----        1: ----        2: TXD1        3: ----        4: SC0IN       5: ----        6: ----        7: COM6/SEG27
                  | ( 0 << PC_PC_AFSR2_AFSR9_Pos )       // PC9            0: ----        1: ----        2: LPRXD       3: SS10        4: SC0PWR      5: ----        6: ----        7: COM5/SEG26
                  | ( 0 << PC_PC_AFSR2_AFSR8_Pos )       // PC8            0: ----        1: ----        2: LPTXD       3: SCK10       4: SC0CLK      5: ----        6: ----        7: COM4/SEG25
                  ;

   PC->AFSR1   =  0
                  | ( 0 << PC_PC_AFSR1_AFSR7_Pos )       // PC7            0: T40OUTA     1: T40INP      2: RXD10       3: MISO10      4: SC0RST      5: SC0RXD      6: SS1         7: SEG24
                  | ( 0 << PC_PC_AFSR1_AFSR6_Pos )       // PC6   SWCLK    0: SWCLK       1: ----        2: TXD10       3: MOSI10      4: SC0DATA     5: SC0TXD      6: ----        7: ----
                  | ( 0 << PC_PC_AFSR1_AFSR5_Pos )       // PC5   SWDIO    0: SWDIO       1: ----        2: ----        3: ----        4: ----        5: ----        6: ----        7: ----
                  | ( 0 << PC_PC_AFSR1_AFSR4_Pos )       // PC4            0: ----        1: ----        2: ----        3: MOSI1       4: SC0DATA     5: SC0TXD      6: CP1OUT      7: ----
                  | ( 0 << PC_PC_AFSR1_AFSR3_Pos )       // PC3            0: ----        1: ----        2: ----        3: MISO1       4: SC0RST      5: SC0RXD      6: CP0OUT      7: ----
                  | ( 0 << PC_PC_AFSR1_AFSR2_Pos )       // PC2            0: ----        1: ----        2: RXD0        3: ----        4: SC0CLK      5: ----        6: ----        7: COM2
                  | ( 0 << PC_PC_AFSR1_AFSR1_Pos )       // PC1   CLKO     0: CLKO        1: ----        2: TXD0        3: ----        4: SC0PWR      5: ----        6: ----        7: COM1
                  | ( 0 << PC_PC_AFSR1_AFSR0_Pos )       // PC0   CLKO     0: CLKO        1: ----        2: ----        3: ----        4: SC0IN       5: ----        6: ----        7: COM0
                  ;

   PC->PUPD    =  0
                  | ( 0 << PC_PC_PUPD_PUPD11_Pos )       // PC11           0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PC_PC_PUPD_PUPD10_Pos )       // PC10           0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PC_PC_PUPD_PUPD9_Pos )        // PC9            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PC_PC_PUPD_PUPD8_Pos )        // PC8            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PC_PC_PUPD_PUPD7_Pos )        // PC7            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 2 << PC_PC_PUPD_PUPD6_Pos )        // PC6   SWCLK    0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 1 << PC_PC_PUPD_PUPD5_Pos )        // PC5   SWDIO    0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PC_PC_PUPD_PUPD4_Pos )        // PC4            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PC_PC_PUPD_PUPD3_Pos )        // PC3            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PC_PC_PUPD_PUPD2_Pos )        // PC2            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PC_PC_PUPD_PUPD1_Pos )        // PC1   CLKO     0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PC_PC_PUPD_PUPD0_Pos )        // PC0   CLKO     0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  ;

   PC->OUTDR   =  0
                  | ( 0 << PC_PC_OUTDR_OUTDR11_Pos )     // PC11           0: Output Low     1: Output High
                  | ( 0 << PC_PC_OUTDR_OUTDR10_Pos )     // PC10           0: Output Low     1: Output High
                  | ( 0 << PC_PC_OUTDR_OUTDR9_Pos )      // PC9            0: Output Low     1: Output High
                  | ( 0 << PC_PC_OUTDR_OUTDR8_Pos )      // PC8            0: Output Low     1: Output High
                  | ( 0 << PC_PC_OUTDR_OUTDR7_Pos )      // PC7            0: Output Low     1: Output High
                  | ( 0 << PC_PC_OUTDR_OUTDR6_Pos )      // PC6   SWCLK    0: Output Low     1: Output High
                  | ( 0 << PC_PC_OUTDR_OUTDR5_Pos )      // PC5   SWDIO    0: Output Low     1: Output High
                  | ( 0 << PC_PC_OUTDR_OUTDR4_Pos )      // PC4            0: Output Low     1: Output High
                  | ( 0 << PC_PC_OUTDR_OUTDR3_Pos )      // PC3            0: Output Low     1: Output High
                  | ( 0 << PC_PC_OUTDR_OUTDR2_Pos )      // PC2            0: Output Low     1: Output High
                  | ( 0 << PC_PC_OUTDR_OUTDR1_Pos )      // PC1   CLKO     0: Output Low     1: Output High
                  | ( 0 << PC_PC_OUTDR_OUTDR0_Pos )      // PC0   CLKO     0: Output Low     1: Output High
                  ;

   // Port D
   PD->MOD     =  0
                  | ( 1 << PD_PD_MOD_MODE7_Pos )         // PD7            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PD_PD_MOD_MODE6_Pos )         // PD6            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 2 << PD_PD_MOD_MODE5_Pos )         // PD5   BOOT     0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PD_PD_MOD_MODE4_Pos )         // PD4            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PD_PD_MOD_MODE3_Pos )         // PD3            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PD_PD_MOD_MODE2_Pos )         // PD2            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PD_PD_MOD_MODE1_Pos )         // PD1            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PD_PD_MOD_MODE0_Pos )         // PD0            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  ;

   PD->TYP     =  0
                  | ( 0 << PD_PD_TYP_TYP7_Pos )          // PD7            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PD_PD_TYP_TYP6_Pos )          // PD6            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PD_PD_TYP_TYP5_Pos )          // PD5   BOOT     0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PD_PD_TYP_TYP4_Pos )          // PD4            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PD_PD_TYP_TYP3_Pos )          // PD3            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PD_PD_TYP_TYP2_Pos )          // PD2            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PD_PD_TYP_TYP1_Pos )          // PD1            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PD_PD_TYP_TYP0_Pos )          // PD0            0: Push-Pull Output     1: Open-Drain Output
                  ;

   // PD->AFSR2   =  0
   ;

   PD->AFSR1   =  0
                  | ( 0 << PD_PD_AFSR1_AFSR7_Pos )       // PD7            0: ----        1: ----        2: ----        3: SS0         4: SDA0        5: ----        6: ----        7: COM3
                  | ( 0 << PD_PD_AFSR1_AFSR6_Pos )       // PD6            0: ----        1: ----        2: ----        3: ----        4: SCL0        5: ----        6: ----        7: SEG32
                  | ( 0 << PD_PD_AFSR1_AFSR5_Pos )       // PD5   BOOT     0: BOOT        1: ----        2: ----        3: ----        4: ----        5: ----        6: ----        7: ----
                  | ( 0 << PD_PD_AFSR1_AFSR4_Pos )       // PD4            0: ----        1: T50INP      2: RXD0        3: ----        4: SC1PWR      5: SDA0        6: CP1P5       7: ----
                  | ( 0 << PD_PD_AFSR1_AFSR3_Pos )       // PD3            0: ----        1: ----        2: TXD0        3: ----        4: SC1IN       5: SCL0        6: CP1P4       7: VLC0
                  | ( 0 << PD_PD_AFSR1_AFSR2_Pos )       // PD2            0: T43OUTB     1: EC43        2: EC50        3: MOSI1       4: SC1DATA     5: SC1TXD      6: CP1P3       7: SEG31
                  | ( 0 << PD_PD_AFSR1_AFSR1_Pos )       // PD1            0: T43OUTA     1: T43INP      2: ----        3: MISO1       4: SC1RST      5: SC1RXD      6: CP1P2       7: SEG30
                  | ( 0 << PD_PD_AFSR1_AFSR0_Pos )       // PD0            0: T40OUTB     1: EC40        2: ----        3: SCK1        4: SC1CLK      5: CP1N3       6: ----        7: SEG29
                  ;

   PD->PUPD    =  0
                  | ( 0 << PD_PD_PUPD_PUPD7_Pos )        // PD7            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PD_PD_PUPD_PUPD6_Pos )        // PD6            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 1 << PD_PD_PUPD_PUPD5_Pos )        // PD5   BOOT     0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PD_PD_PUPD_PUPD4_Pos )        // PD4            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PD_PD_PUPD_PUPD3_Pos )        // PD3            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PD_PD_PUPD_PUPD2_Pos )        // PD2            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PD_PD_PUPD_PUPD1_Pos )        // PD1            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PD_PD_PUPD_PUPD0_Pos )        // PD0            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  ;

   PD->OUTDR   =  0
                  | ( 0 << PD_PD_OUTDR_OUTDR7_Pos )      // PD7            0: Output Low     1: Output High
                  | ( 0 << PD_PD_OUTDR_OUTDR6_Pos )      // PD6            0: Output Low     1: Output High
                  | ( 0 << PD_PD_OUTDR_OUTDR5_Pos )      // PD5   BOOT     0: Output Low     1: Output High
                  | ( 0 << PD_PD_OUTDR_OUTDR4_Pos )      // PD4            0: Output Low     1: Output High
                  | ( 0 << PD_PD_OUTDR_OUTDR3_Pos )      // PD3            0: Output Low     1: Output High
                  | ( 0 << PD_PD_OUTDR_OUTDR2_Pos )      // PD2            0: Output Low     1: Output High
                  | ( 0 << PD_PD_OUTDR_OUTDR1_Pos )      // PD1            0: Output Low     1: Output High
                  | ( 0 << PD_PD_OUTDR_OUTDR0_Pos )      // PD0            0: Output Low     1: Output High
                  ;

   // Port E
   PE->MOD     =  0
                  | ( 1 << PE_PE_MOD_MODE4_Pos )         // PE4            0: Input Mode     1: Output Mode    2: Alternative Function Mode
#ifdef USED_XMOSC
                  | ( 2 << PE_PE_MOD_MODE3_Pos )         // PE3   XOUT     0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 2 << PE_PE_MOD_MODE2_Pos )         // PE2   XIN      0: Input Mode     1: Output Mode    2: Alternative Function Mode
#else
                  | ( 1 << PE_PE_MOD_MODE3_Pos )         // PE3   XOUT     0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PE_PE_MOD_MODE2_Pos )         // PE2   XIN      0: Input Mode     1: Output Mode    2: Alternative Function Mode
#endif
#ifdef USED_XSOSC
                  | ( 2 << PE_PE_MOD_MODE1_Pos )         // PE1   SXOUT    0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 2 << PE_PE_MOD_MODE0_Pos )         // PE0   SXIN     0: Input Mode     1: Output Mode    2: Alternative Function Mode
#else
                  | ( 1 << PE_PE_MOD_MODE1_Pos )         // PE1   SXOUT    0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PE_PE_MOD_MODE0_Pos )         // PE0   SXIN     0: Input Mode     1: Output Mode    2: Alternative Function Mode
#endif
                  ;

   PE->TYP     =  0
                  | ( 0 << PE_PE_TYP_TYP4_Pos )          // PE4            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PE_PE_TYP_TYP3_Pos )          // PE3   XOUT     0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PE_PE_TYP_TYP2_Pos )          // PE2   XIN      0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PE_PE_TYP_TYP1_Pos )          // PE1   SXOUT    0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PE_PE_TYP_TYP0_Pos )          // PE0   SXIN     0: Push-Pull Output     1: Open-Drain Output
                  ;

   PE->AFSR1   =  0
                  | ( 0 << PE_PE_AFSR1_AFSR4_Pos )       // PE4            0: RTCOUT      1: ----        2: ----        3: ----        4: ----        5: ----        6: ----        7: ----
                  | ( 0 << PE_PE_AFSR1_AFSR3_Pos )       // PE3   XOUT     0: XOUT        1: ----        2: ----        3: ----        4: ----        5: ----        6: ----        7: ----
                  | ( 0 << PE_PE_AFSR1_AFSR2_Pos )       // PE2   XIN      0: XIN         1: ----        2: ----        3: ----        4: ----        5: ----        6: ----        7: ----
                  | ( 0 << PE_PE_AFSR1_AFSR1_Pos )       // PE1   SXOUT    0: SXOUT       1: ----        2: ----        3: ----        4: ----        5: ----        6: ----        7: ----
                  | ( 0 << PE_PE_AFSR1_AFSR0_Pos )       // PE0   SXIN     0: SXIN        1: ----        2: ----        3: ----        4: ----        5: ----        6: ----        7: ----
                  ;

   // PE->AFSR2   =  0
   ;

   PE->PUPD    =  0
                  | ( 0 << PE_PE_PUPD_PUPD4_Pos )        // PE4            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PE_PE_PUPD_PUPD3_Pos )        // PE3   XOUT     0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PE_PE_PUPD_PUPD2_Pos )        // PE2   XIN      0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PE_PE_PUPD_PUPD1_Pos )        // PE1   SXOUT    0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PE_PE_PUPD_PUPD0_Pos )        // PE0   SXIN     0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  ;

   PE->OUTDR   =  0
                  | ( 0 << PE_PE_OUTDR_OUTDR4_Pos )      // PE4            0: Output Low     1: Output High
                  | ( 0 << PE_PE_OUTDR_OUTDR3_Pos )      // PE3   XOUT     0: Output Low     1: Output High
                  | ( 0 << PE_PE_OUTDR_OUTDR2_Pos )      // PE2   XIN      0: Output Low     1: Output High
                  | ( 0 << PE_PE_OUTDR_OUTDR1_Pos )      // PE1   SXOUT    0: Output Low     1: Output High
                  | ( 0 << PE_PE_OUTDR_OUTDR0_Pos )      // PE0   SXIN     0: Output Low     1: Output High
                  ;

   // Port F
   PF->MOD     =  0
                  | ( 1 << PF_PF_MOD_MODE3_Pos )         // PF3            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PF_PF_MOD_MODE2_Pos )         // PF2            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PF_PF_MOD_MODE1_Pos )         // PF1            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  | ( 1 << PF_PF_MOD_MODE0_Pos )         // PF0            0: Input Mode     1: Output Mode    2: Alternative Function Mode
                  ;

   PF->TYP     =  0
                  | ( 0 << PF_PF_TYP_TYP3_Pos )          // PF3            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PF_PF_TYP_TYP2_Pos )          // PF2            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PF_PF_TYP_TYP1_Pos )          // PF1            0: Push-Pull Output     1: Open-Drain Output
                  | ( 0 << PF_PF_TYP_TYP0_Pos )          // PF0            0: Push-Pull Output     1: Open-Drain Output
                  ;

   // PF->AFSR2   =  0
   ;

   PF->AFSR1   =  0
                  | ( 0 << PF_PF_AFSR1_AFSR3_Pos )       // PF3            0: ----        1: ----        2: ----        3: MOSI0       4: AN15        5: ----        6: ----        7: SEG20
                  | ( 0 << PF_PF_AFSR1_AFSR2_Pos )       // PF2            0: ----        1: T50INP      2: ----        3: MISO0       4: AN14        5: ----        6: ----        7: SEG21
                  | ( 0 << PF_PF_AFSR1_AFSR1_Pos )       // PF1            0: T50OUT      1: ----        2: ----        3: ----        4: AN13        5: ----        6: ----        7: SEG22
                  | ( 0 << PF_PF_AFSR1_AFSR0_Pos )       // PF0            0: ----        1: EC50        2: ----        3: ----        4: AN12        5: ----        6: ----        7: SEG23
                  ;

   PF->PUPD    =  0
                  | ( 0 << PF_PF_PUPD_PUPD3_Pos )        // PF3            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PF_PF_PUPD_PUPD2_Pos )        // PF2            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PF_PF_PUPD_PUPD1_Pos )        // PF1            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  | ( 0 << PF_PF_PUPD_PUPD0_Pos )        // PF0            0: Disable Pull-Up/Down    1: Enable Pull-Up    2: Enable Pull-Down
                  ;

   PF->OUTDR   =  0
                  | ( 0 << PF_PF_OUTDR_OUTDR3_Pos )      // PF3            0: Output Low     1: Output High
                  | ( 0 << PF_PF_OUTDR_OUTDR2_Pos )      // PF2            0: Output Low     1: Output High
                  | ( 0 << PF_PF_OUTDR_OUTDR1_Pos )      // PF1            0: Output Low     1: Output High
                  | ( 0 << PF_PF_OUTDR_OUTDR0_Pos )      // PF0            0: Output Low     1: Output High
                  ;
}


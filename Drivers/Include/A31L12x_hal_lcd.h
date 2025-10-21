/**
 *******************************************************************************
 * @file        A31L12x_hal_lcd.h
 * @author      ABOV R&D Division
 * @brief       LCD Header File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#ifndef _LCD_H_
#define _LCD_H_

#include "A31L12x.h"
#include "A31L12x_hal_aa_types.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

// Internal LCD Bias Dividing Resistor Selection Control
/*
#define LCD_RLCD3       (0x0uL << 6)
#define LCD_RLCD1       (0x1uL << 6)
#define LCD_RLCD2       (0x2uL << 6)
#define LCD_RLCD4       (0x3uL << 6)
*/
#define IRSEL_RLCD3     (LCD_CR_IRSEL_RLCD3 << LCD_CR_IRSEL_Pos)  // RLCD3 : 105/105/80[kohm] @(1/2)/(1/3)/(1/4) bias 
#define IRSEL_RLCD1     (LCD_CR_IRSEL_RLCD1 << LCD_CR_IRSEL_Pos)  // RLCD1 : 10/10/10[kohm] @(1/2)/(1/3)/(1/4) bias   
#define IRSEL_RLCD2     (LCD_CR_IRSEL_RLCD2 << LCD_CR_IRSEL_Pos)  // RLCD2 : 66/66/50[kohm] @(1/2)/(1/3)/(1/4) bias   
#define IRSEL_RLCD4     (LCD_CR_IRSEL_RLCD4 << LCD_CR_IRSEL_Pos)  // RLCD4 : 320/320/240[kohm] @(1/2)/(1/3)/(1/4) bias

// LCD Duty and Bias Selection Control
/*
#define LCD_8D4B        (0x0uL << 3)
#define LCD_6D4B        (0x1uL << 3)
#define LCD_5D3B        (0x2uL << 3)
#define LCD_4D3B        (0x3uL << 3)
#define LCD_3D3B        (0x4uL << 3)
#define LCD_3D2B        (0x5uL << 3)
*/
#define DBS_8D4B        (LCD_CR_DBS_8D4B << LCD_CR_DBS_Pos)    // 8D4B : 1/8 duty, 1/4 bias
#define DBS_6D4B        (LCD_CR_DBS_6D4B << LCD_CR_DBS_Pos)    // 6D4B : 1/6 duty, 1/4 bias
#define DBS_5D3B        (LCD_CR_DBS_5D3B << LCD_CR_DBS_Pos)    // 5D3B : 1/5 duty, 1/3 bias
#define DBS_4D3B        (LCD_CR_DBS_4D3B << LCD_CR_DBS_Pos)    // 4D3B : 1/4 duty, 1/3 bias
#define DBS_3D3B        (LCD_CR_DBS_3D3B << LCD_CR_DBS_Pos)    // 3D3B : 1/3 duty, 1/3 bias
#define DBS_3D2B        (LCD_CR_DBS_3D2B << LCD_CR_DBS_Pos)    // 3D2B : 1/3 duty, 1/2 bias

// LCD Clock Selection Control
/*
#define LCD_CLK128      (0x0uL << 1)
#define LCD_CLK256      (0x1uL << 1)
#define LCD_CLK512      (0x2uL << 1)
#define LCD_CLK1024     (0x3uL << 1)
*/
#define LCLK_128Hz      (LCD_CR_LCLK_128Hz  << LCD_CR_LCLK_Pos)   // 128Hz : 32.768kHz(fLCD) / 256
#define LCLK_256Hz      (LCD_CR_LCLK_256Hz  << LCD_CR_LCLK_Pos)   // 256Hz : 32.768kHz(fLCD) / 128
#define LCLK_512Hz      (LCD_CR_LCLK_512Hz  << LCD_CR_LCLK_Pos)   // 512Hz : 32.768kHz(fLCD) / 64 
#define LCLK_1024Hz     (LCD_CR_LCLK_1024Hz << LCD_CR_LCLK_Pos)   // 1024Hz : 32.768kHz(fLCD) / 32

// LCD Automatic Bias Control
/*
#define LCD_ABCDIS      (0x0uL << 12)
#define LCD_ABCEN       (0x1uL << 12)
*/
#define LCDABC_Off      (LCD_BCCR_LCDABC_Off << LCD_BCCR_LCDABC_Pos)
#define LCDABC_On       (LCD_BCCR_LCDABC_On  << LCD_BCCR_LCDABC_Pos)

// 'Bias Mode A' Time Selection Control
/*
#define LCD_BIA1CLK     (0x0uL << 8)
#define LCD_BIA2CLK     (0x1uL << 8)
#define LCD_BIA3CLK     (0x2uL << 8)
#define LCD_BIA4CLK     (0x3uL << 8)
#define LCD_BIA5CLK     (0x4uL << 8)
#define LCD_BIA6CLK     (0x5uL << 8)
#define LCD_BIA7CLK     (0x6uL << 8)
#define LCD_BIA8CLK     (0x7uL << 8)
*/
#define BMSEL_BMA1Clk   (LCD_BCCR_BMSEL_BMA1Clk << LCD_BCCR_BMSEL_Pos)  // BMA1Clk : 'Bias Mode A' for 1-clock of fLCD
#define BMSEL_BMA2Clk   (LCD_BCCR_BMSEL_BMA2Clk << LCD_BCCR_BMSEL_Pos)  // BMA2Clk : 'Bias Mode A' for 2-clock of fLCD
#define BMSEL_BMA3Clk   (LCD_BCCR_BMSEL_BMA3Clk << LCD_BCCR_BMSEL_Pos)  // BMA3Clk : 'Bias Mode A' for 3-clock of fLCD
#define BMSEL_BMA4Clk   (LCD_BCCR_BMSEL_BMA4Clk << LCD_BCCR_BMSEL_Pos)  // BMA4Clk : 'Bias Mode A' for 4-clock of fLCD
#define BMSEL_BMA5Clk   (LCD_BCCR_BMSEL_BMA5Clk << LCD_BCCR_BMSEL_Pos)  // BMA5Clk : 'Bias Mode A' for 5-clock of fLCD
#define BMSEL_BMA6Clk   (LCD_BCCR_BMSEL_BMA6Clk << LCD_BCCR_BMSEL_Pos)  // BMA6Clk : 'Bias Mode A' for 6-clock of fLCD
#define BMSEL_BMA7Clk   (LCD_BCCR_BMSEL_BMA7Clk << LCD_BCCR_BMSEL_Pos)  // BMA7Clk : 'Bias Mode A' for 7-clock of fLCD
#define BMSEL_BMA8Clk   (LCD_BCCR_BMSEL_BMA8Clk << LCD_BCCR_BMSEL_Pos)  // BMA8Clk : 'Bias Mode A' for 8-clock of fLCD

// LCD Driver Contrast Control
/*
#define LCD_CONTDIS     (0x0uL << 5)
#define LCD_CONTEN      (0x1uL << 5)
*/
#define LCTEN_Disable   (LCD_BCCR_LCTEN_Disable << LCD_BCCR_LCTEN_Pos)
#define LCTEN_Enable    (LCD_BCCR_LCTEN_Enable  << LCD_BCCR_LCTEN_Pos)

// VLC0 Voltage Control
/*
#define LCD_VLC31       (0x0uL << 0)
#define LCD_VLC30       (0x1uL << 0)
#define LCD_VLC29       (0x2uL << 0)
#define LCD_VLC28       (0x3uL << 0)
#define LCD_VLC27       (0x4uL << 0)
#define LCD_VLC26       (0x5uL << 0)
#define LCD_VLC25       (0x6uL << 0)
#define LCD_VLC24       (0x7uL << 0)
#define LCD_VLC23       (0x8uL << 0)
#define LCD_VLC22       (0x9uL << 0)
#define LCD_VLC21       (0xAuL << 0)
#define LCD_VLC20       (0xBuL << 0)
#define LCD_VLC19       (0xCuL << 0)
#define LCD_VLC18       (0xDuL << 0)
#define LCD_VLC17       (0xEuL << 0)
#define LCD_VLC16       (0xFuL << 0)
*/
#define VLCD_Step0      (LCD_BCCR_VLCD_Step0  << LCD_BCCR_VLCD_Pos)  // Step0 : VDD x 32/47 Step 
#define VLCD_Step1      (LCD_BCCR_VLCD_Step1  << LCD_BCCR_VLCD_Pos)  // Step1 : VDD x 32/46 Step 
#define VLCD_Step2      (LCD_BCCR_VLCD_Step2  << LCD_BCCR_VLCD_Pos)  // Step2 : VDD x 32/45 Step 
#define VLCD_Step3      (LCD_BCCR_VLCD_Step3  << LCD_BCCR_VLCD_Pos)  // Step3 : VDD x 32/44 Step 
#define VLCD_Step4      (LCD_BCCR_VLCD_Step4  << LCD_BCCR_VLCD_Pos)  // Step4 : VDD x 32/43 Step 
#define VLCD_Step5      (LCD_BCCR_VLCD_Step5  << LCD_BCCR_VLCD_Pos)  // Step5 : VDD x 32/42 Step 
#define VLCD_Step6      (LCD_BCCR_VLCD_Step6  << LCD_BCCR_VLCD_Pos)  // Step6 : VDD x 32/41 Step 
#define VLCD_Step7      (LCD_BCCR_VLCD_Step7  << LCD_BCCR_VLCD_Pos)  // Step7 : VDD x 32/40 Step 
#define VLCD_Step8      (LCD_BCCR_VLCD_Step8  << LCD_BCCR_VLCD_Pos)  // Step8 : VDD x 32/39 Step 
#define VLCD_Step9      (LCD_BCCR_VLCD_Step9  << LCD_BCCR_VLCD_Pos)  // Step9 : VDD x 32/38 Step 
#define VLCD_Step10     (LCD_BCCR_VLCD_Step10 << LCD_BCCR_VLCD_Pos)  // Step10 : VDD x 32/37 Step
#define VLCD_Step11     (LCD_BCCR_VLCD_Step11 << LCD_BCCR_VLCD_Pos)  // Step11 : VDD x 32/36 Step
#define VLCD_Step12     (LCD_BCCR_VLCD_Step12 << LCD_BCCR_VLCD_Pos)  // Step12 : VDD x 32/35 Step
#define VLCD_Step13     (LCD_BCCR_VLCD_Step13 << LCD_BCCR_VLCD_Pos)  // Step13 : VDD x 32/34 Step
#define VLCD_Step14     (LCD_BCCR_VLCD_Step14 << LCD_BCCR_VLCD_Pos)  // Step14 : VDD x 32/33 Step
#define VLCD_Step15     (LCD_BCCR_VLCD_Step15 << LCD_BCCR_VLCD_Pos)  // Step15 : VDD x 32/32 Step

// Display Data RAM Size
#define LCDBufSize      33

//******************************************************************************
// Type
//******************************************************************************

//==============================================================================
// Structure
//==============================================================================

typedef struct
{
   // LCD_CR
   uint32_t    Bias;          // IRSEL
   uint32_t    Duty;          // DBS
   uint32_t    Clk;           // LCLK

   // LCD_BCCR
   uint32_t    AutoBiasEn;    // LCDABC
   uint32_t    BiasTime;      // BMSEL
   uint32_t    Contrast;      // LCTEN
   uint32_t    ContrastStep;  // VLCD
} LCD_CFG_Type;

//******************************************************************************
// Macro
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         LCD Enable
 * @details       This macro Enable LCD Block
 *//*-------------------------------------------------------------------------*/
#define LCDON()         (LCD->CR_b.DISP = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         LCD Disable
 * @details       This macro Disable LCD Block
 *//*-------------------------------------------------------------------------*/
#define LCDOFF()        (LCD->CR_b.DISP = 0)

//******************************************************************************
// Function
//******************************************************************************

HAL_Status_Type HAL_LCD_Init( LCD_CFG_Type* LCD_Config );

HAL_Status_Type HAL_LCD_SetRegister( uint32_t u32LCD_CR, uint32_t u32LCD_BCCR );
HAL_Status_Type HAL_LCD_ClearDspRam( void );
HAL_Status_Type HAL_LCD_WriteDspRam( uint8_t* write_buf, uint32_t u32Index, uint32_t size );

#ifdef __cplusplus
}
#endif

#endif   /* _LCD_H_ */


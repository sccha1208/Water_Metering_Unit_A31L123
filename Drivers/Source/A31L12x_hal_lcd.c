/**
 *******************************************************************************
 * @file        A31L12x_hal_lcd.c
 * @author      ABOV R&D Division
 * @brief       LCD Driver
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

#include "A31L12x_hal_lcd.h"
#include "A31L12x_hal_scu.h"

/* Public Functions --------------------------------------------------------- */
//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Initialize the LCD peripheral with the specified parameters.
 * @param[in]     LCD_Config
 *                   Pointer to LCD_CFG_Type
 *                   that contains the configuration information for the specified peripheral.
 * @return        @ref HAL_Status_Type
 * @details       This function sets the LCD Bias Resistor, LCD Duty,
 *                LCD Clock, Automatic Bias Control, and Contrast Control Data.
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_LCD_Init( LCD_CFG_Type* LCD_Config )
{
   /* Check LCD_Config */
   if( LCD_Config == NULL )
   {
      return HAL_ERROR;
   }

   // enable peripheral clock
   HAL_SCU_Peripheral_EnableClock2( PPCLKEN2_LCDCLKE, PPxCLKE_Enable );

   LCD->CR = 0
             | LCD_Config->Bias
             | LCD_Config->Duty
             | LCD_Config->Clk
             ;

   LCD->BCCR = 0
               | LCD_Config->AutoBiasEn
               | LCD_Config->BiasTime
               | LCD_Config->Contrast
               | LCD_Config->ContrastStep
               ;

   // return
   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set LCD LCD_CR/LCD_BCCR Registers
 * @param[in]     u32LCD_CR
 *                   LCD Driver Control Register Setting Data
 * @param[in]     u32LCD_BCCR
 *                   LCD Automatic bias and Contrast Control Register Setting Data
 * @return        @ref HAL_Status_Type
 * @details       This function sets the LCD Bias Resistor, LCD Duty,
 *                LCD Clock, Automatic Bias Control, and Contrast Control Data.
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_LCD_SetRegister( uint32_t u32LCD_CR, uint32_t u32LCD_BCCR )
{
   LCD->CR = u32LCD_CR;       // Set LCD Driver Control Register
   LCD->BCCR = u32LCD_BCCR;   // Set LCD Driver Automatic Bias and Contrast Control Register

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Clear LCD Buffer
 * @return        @ref HAL_Status_Type
 * @details       This function clear Display Data RAM.
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_LCD_ClearDspRam( void )
{
   uint8_t              u32Index;
   volatile uint8_t*    lcd_dsp_ram;

   lcd_dsp_ram = &LCD->DR0;
   for( u32Index = 0; u32Index < LCDBufSize; u32Index ++ )
   {
      lcd_dsp_ram[u32Index] = 0;
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Write LCD Buffer
 * @param[in]     write_buf
 *                   Font Data Buffer Address
 * @param[in]     u32Index
 *                   Font Data Buffer Index
 * @param[in]     size
 *                   Font Data Buffer Size
 * @return        @ref HAL_Status_Type
 * @details       This function writes display data to Display Data RAM.
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_LCD_WriteDspRam( uint8_t* write_buf, uint32_t u32Index, uint32_t size )
{
   volatile uint8_t*    lcd_dsp_ram;
   uint32_t             i;

   if( ( u32Index + size ) > LCDBufSize )    // Check Buffer size
   {
      size = size - ( ( u32Index + size ) - LCDBufSize );
   }

   lcd_dsp_ram = &LCD->DR0;
   for( i = 0; i < size; i ++ )
   {
      lcd_dsp_ram[u32Index + i] = write_buf[i];
   }

   return HAL_OK;
}


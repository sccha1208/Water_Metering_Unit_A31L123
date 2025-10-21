/**
 *******************************************************************************
 * @file        A31L12x_hal_pcu.c
 * @author      ABOV R&D Division
 * @brief       Port Control Unit
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

#include "A31L12x_hal_pcu.h"

/* Public Functions --------------------------------------------------------- */
//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Set PCU Pn_MOD/Pn_TYP/Pn_AFSR1/Pn_PUPD Registers
 * @param[in]     Px
 *                   Pointer to the target PORT
 *                   -  PA ~ PF
 * @param[in]     u32Mode
 *                   Values for In/Out/Alternative mode
 * @param[in]     u32Type
 *                   Values for Push-pull and Open-drain
 * @param[in]     u32Afsr1
 *                   Values for Alternative Function
 * @param[in]     u32Afsr2
 *                   Values for Alternative Function
 * @param[in]     u32PuPd
 *                   Values for Pull-up/down resistor
 * @return        None
 * @details       This function sets the mode, type, alternative function, and pull-up/down resistor of port.
 * @remark        Available GPIO Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
void HAL_GPIO_Init( Pn_Type* Px, uint32_t u32Mode, uint32_t u32Type, uint32_t u32Afsr1, uint32_t u32Afsr2, uint32_t u32PuPd )
{
   Px->MOD   = u32Mode;    // 00/01/10/11: Input/Output/"Alternative Function"/Reserved Mode
   Px->TYP   = u32Type;    // 0/1:         Push-pull/Open-drain Output
   Px->AFSR1 = u32Afsr1;   // 0 to 4:      Alternative Function 0 to 4
   Px->AFSR2 = u32Afsr2;   // 0 to 4:      Alternative Function 0 to 4
   Px->PUPD  = u32PuPd;    // 00/01/10/11: "Disable Pull-up/down"/"Enable Pull-up"/"Enable Pull-down"/Reserved Resistor
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure pin function
 * @param[in]     Px
 *                   Pointer to the target PORT
 *                   -  PA ~ PF
 * @param[in]     pin_no
 *                   Pin Number
 *                   -  0 ~ 12
 * @param[in]     func
 *                   Alternative Function Number
 *                   -  AFSRx_AF0 ~ AFSRx_AF7
 * @return        None
 * @remark        Available GPIO Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
void HAL_GPIO_ConfigFunction( Pn_Type* Px, uint8_t pin_no, uint32_t func )
{
   uint8_t     pin_offset;
   uint32_t    reg_val;

   if( pin_no < 8 ) // 0~7
   {
      //--------------------------------------
      // pin_offset = pin_no * 4
      //--------------------------------------
      pin_offset = ( pin_no * 4 );

      //--------------------------------------
      // MR
      //--------------------------------------
      reg_val = Px->AFSR1;
      reg_val &= ~( AFSRx_Msk << pin_offset );
      reg_val |= ( func << pin_offset );

      Px->AFSR1 = reg_val;
   }
   else
   {
      pin_no -= 8;
      //--------------------------------------
      // pin_offset = pin_no * 4
      //--------------------------------------
      pin_offset = ( pin_no * 4 );

      //--------------------------------------
      // MR
      //--------------------------------------
      reg_val = Px->AFSR2;
      reg_val &= ~( AFSRx_Msk << pin_offset );
      reg_val |= ( func << pin_offset );

      Px->AFSR2 = reg_val;
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure pin mode
 * @param[in]     Px
 *                   Pointer to the target PORT
 *                   -  PA ~ PF
 * @param[in]     pin_no
 *                   Pin Number
 *                   -  0 ~ 12
 * @param[in]     dir_type
 *                   Pin Mode
 *                   -  INPUT:               0
 *                   -  PUSH_PULL_OUTPUT:    1
 *                   -  ALTERN_FUNC:         2
 *                   -  OPEN_DRAIN_OUTPUT:   3
 * @return        None
 * @remark        Available GPIO Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
void HAL_GPIO_ConfigOutput( Pn_Type* Px, uint8_t pin_no, PCU_PORT_MODE dir_type )
{
   uint8_t     pin_offset;
   uint32_t    reg_val;
   uint32_t    dir_type_temp;

   dir_type_temp = dir_type;
   if( dir_type_temp == OPEN_DRAIN_OUTPUT )
   {
      dir_type = PUSH_PULL_OUTPUT;
   }
   //--------------------------------------
   // pin_offset = pin_no * 2
   //--------------------------------------
   pin_offset = ( pin_no << 1 );

   //--------------------------------------
   // Pn_MOD
   //--------------------------------------
   reg_val = Px->MOD;
   reg_val &= ~( MODEx_Msk << pin_offset );
   reg_val |= ( dir_type << pin_offset );
   Px->MOD = reg_val;

   //--------------------------------------
   // Pn_TYP
   //--------------------------------------
   if( ( dir_type_temp == PUSH_PULL_OUTPUT ) || ( dir_type_temp == OPEN_DRAIN_OUTPUT ) )
   {
      reg_val = Px->TYP;
      reg_val &= ~( 1 << pin_no );
      if( dir_type_temp == OPEN_DRAIN_OUTPUT )
      {
         reg_val |= ( 1 << pin_no );
      }
      Px->TYP = reg_val;
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure out data Mask
 * @param[in]     Px
 *                   Pointer to the target PORT
 *                   -  PA ~ PF
 * @param[in]     pin_no
 *                   Pin Number
 *                   -  0 ~ 12
 * @param[in]     maskctrl
 *                   state
 *                   -  OUTDMSKx_Unmask  // The corresponding OUTDRx bit can be changed.
 *                   -  OUTDMSKx_Mask    // The corresponding OUTDRx bit is protected.
 * @return        None
 * @remark        Available GPIO Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
void HAL_GPIO_ConfigOutDataMask( Pn_Type* Px, uint8_t pin_no, FunctionalState maskctrl )
{
   uint32_t    reg_val;

   reg_val = Px->OUTDMSK;
   reg_val &= ~( 1 << pin_no );
   reg_val |= ( maskctrl << pin_no );
   Px->OUTDMSK = reg_val;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Configure Pin Pull-Up & Pull-Down
 * @param[in]     Px
 *                   Pointer to the target PORT
 *                   -  PA ~ PF
 * @param[in]     pin_no
 *                   Pin Number
 *                   -  0 ~ 12
 * @param[in]     pullupdown
 *                   Target Configuration
 *                   -  PUPDx_Disable
 *                   -  PUPDx_EnablePU
 *                   -  PUPDx_EnablePD
 * @return        None
 * @remark        Available GPIO Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
void HAL_GPIO_ConfigPullup( Pn_Type* Px, uint8_t pin_no, uint8_t pullupdown )
{
   uint32_t    reg_val;
   uint8_t     pin_offset;

   pin_offset = ( pin_no << 1 );

   reg_val = Px->PUPD;
   reg_val &= ~( 3 << pin_offset );
   reg_val |= ( pullupdown << pin_offset );
   Px->PUPD = reg_val;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set PCU Debounce
 * @param[in]     Px
 *                   Pointer to the target PORT
 *                   -  PA ~ PF
 * @param[in]     u32Pins
 *                   Pin Number
 *                   -  0 ~ 12
 * @param[in]     u32Debnc
 *                   Sampling Clock Selection
 *                   -  DBCLK_HCLK1, DBCLK_HCLK4, DBCLK_HCLK16, DBCLK_HCLK64, DBCLK_HCLK256, DBCLK_HCLK1024
 * @return        None
 * @details       This function enables the debounce filter for the pin specified by u32Pins
 *                and selects the debounce filter sampling clock.
 * @remark        Available EINT Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 * @code
 *                [Example]
 *
 *                HAL_GPIO_SetDebouncePin( ( Pn_Type* )PA, 0, DBCLK_HCLK1024 );
 *                HAL_GPIO_SetDebouncePin( ( Pn_Type* )PA, 1, DBCLK_HCLK1024 );
 * @endcode
 *//*-------------------------------------------------------------------------*/
void HAL_GPIO_SetDebouncePin( Pn_Type* Px, uint32_t u32Pins, uint32_t u32Debnc )
{
#if 0    // before bug fix
   uint32_t    reg_val;

   reg_val = ( 0x07ff & Px->DBCR );
   reg_val |= ( 0x01 << u32Pins );
   reg_val |= u32Debnc;
   Px->DBCR = reg_val;
#else    // after bug fix
   Px->DBCR = Px->DBCR
              & ~Pn_DBCR_DBCLK_Msk
              | ( 1 << u32Pins )
              | u32Debnc
              ;
#endif
}

/*-------------------------------------------------------------------------*//**
 * @brief         Set Value for bits that have output direction on GPIO port.
 * @param[in]     Px
 *                   Pointer to the target PORT
 *                   -  PA ~ PF
 * @param[in]     bitValue
 *                   Value that contains all bits on GPIO to set.
 *                   -  0x0000 ~ 0xffff
 *                   -  Example: Use the value of 0x0003 to set bit 0 and bit 1.
 * @return        None
 * @remark        Available GPIO Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
void HAL_GPIO_SetPin( Pn_Type* Px, uint16_t bitValue )
{
   Px->BSR = bitValue;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Clear Value for bits that have output direction on GPIO port.
 * @param[in]     Px
 *                   Pointer to the target PORT
 *                   -  PA ~ PF
 * @param[in]     bitValue
 *                   Value that contains all bits on GPIO to clear.
 *                   -  0x0000 ~ 0xffff
 *                   -  Example: Use the value of 0x0003 to clear bit 0 and bit 1.
 * @return        None
 * @remark        Available GPIO Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
void HAL_GPIO_ClearPin( Pn_Type* Px, uint16_t bitValue )
{
   Px->BCR = bitValue;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Write Value on port that have output direction of GPIO
 * @param[in]     Px
 *                   Pointer to the target PORT
 *                   -  PA ~ PF
 * @param[in]     Value
 *                   Value to write
 * @return        None
 * @remark        Available GPIO Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 *//*-------------------------------------------------------------------------*/
void HAL_GPIO_WritePin( Pn_Type* Px, uint16_t Value )
{
   Px->OUTDR = Value;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Read Current state on port pin that have input direction of GPIO
 * @param[in]     Px
 *                   Pointer to the target PORT
 *                   -  PA ~ PF
 * @return        Current value of GPIO port.
 * @remark        Available GPIO Pin: PA[9:0], PB[12:0], PC[11:0], PD[7:0], PE[4:0], PF[3:0]
 * @note          Return value contain state of each port pin (bit) on that GPIO regardless
 *                its direction is input or output.
 *//*-------------------------------------------------------------------------*/
uint16_t HAL_GPIO_ReadPin( Pn_Type* Px )
{
   return Px->INDR;
}


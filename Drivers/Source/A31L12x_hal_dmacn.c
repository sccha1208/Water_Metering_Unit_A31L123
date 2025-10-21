/**
 *******************************************************************************
 * @file        A31L12x_hal_dmacn.c
 * @author      ABOV R&D Division
 * @brief       Direct Memory Access
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

#include "A31L12x_hal_dmacn.h"

//******************************************************************************
// Variable
//******************************************************************************

uint16_t    PeripheralAddress[] =
{
   0x0,
   PAR_ADC,
   PAR_SPI0_Rx,
   PAR_SPI0_Tx,
   PAR_SPI1_Rx,
   PAR_SPI1_Tx,
   PAR_USART10_Rx,
   PAR_USART10_Tx,
   PAR_I2C0_Rx,
   PAR_I2C0_Tx,
   PAR_I2C1_Rx,
   PAR_I2C1_Tx,
   PAR_UART0_Rx,
   PAR_UART0_Tx,
   PAR_UART1_Rx,
   PAR_UART1_Tx,
   PAR_LPUART_Rx,
   PAR_LPUART_Tx,
   PAR_SC0_Rx,
   PAR_SC0_Tx,
   PAR_SC1_Rx,
   PAR_SC1_Tx,
};

/* Public Functions --------------------------------------------------------- */
//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         Init the DMAC configuration with the specified values.
 * @param[in]     DMACx
 *                   Pointer to the target DMAC
 *                   -  DMAC0 ~ DMAC4
 * @param[in]     PERSEL
 *                   Peripheral Selection
 *                   -  PERSEL_Idle
 *                   -  PERSEL_ADC
 *                   -  PERSEL_SPI0Rx
 *                   -  PERSEL_SPI0Tx
 *                   -  PERSEL_SPI1Rx
 *                   -  PERSEL_SPI1Tx
 *                   -  PERSEL_USART10Rx
 *                   -  PERSEL_USART10Tx
 *                   -  PERSEL_I2C0Rx
 *                   -  PERSEL_I2C0Tx
 *                   -  PERSEL_I2C1Rx
 *                   -  PERSEL_I2C1Tx
 *                   -  PERSEL_UART0Rx
 *                   -  PERSEL_UART0Tx
 *                   -  PERSEL_UART1Rx
 *                   -  PERSEL_UART1Tx
 *                   -  PERSEL_LPUARTRx
 *                   -  PERSEL_LPUARTTx
 *                   -  PERSEL_SC0Rx
 *                   -  PERSEL_SC0Tx
 *                   -  PERSEL_SC1Rx
 *                   -  PERSEL_SC1Tx
 * @param[in]     DIR
 *                   Transfer Direction
 *                   -  DIR_MemToPeri
 *                   -  DIR_PeriToMem
 * @param[in]     SIZE
 *                   Transfer Size Selection
 *                   -  SIZE_8bit
 *                   -  SIZE_16bit
 *                   -  SIZE_32bit
 * @param[in]     ERFGSTP
 *                   Error Flag Stop
 *                   -  ERFGSTP_Disable
 *                   -  ERFGSTP_Enable
 * @return        @ref HAL_Status_Type
 * @details       This fuunction configures the DMA Controller.
 * @code
 *                [Example]
 *
 *                HAL_DMAC_Init( ( DMACn_Type* )DMAC0, PERSEL_USART10Tx, DIR_MemToPeri, SIZE_8bit, ERFGSTP_Disable );
 * @endcode
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_DMAC_Init( DMACn_Type* DMACx, uint8_t PERSEL, uint8_t DIR, uint8_t SIZE, uint8_t ERFGSTP )
{
   /* Check DMAC handle */
   if( DMACx == NULL )
   {
      return HAL_ERROR;
   }

   // enable peripheral clock and reset peripheral
   if( SCUCG->PPCLKEN2_b.DMACLKE == 0 )
   {
      // enable peripheral clock
      SCUCG->PPCLKEN2_b.DMACLKE = 1;      // DMACLKE[31:31]   [0] in [0(disable clock) 1(enable)]

      // reset peripheral
      SCUCG->PPRST2_b.DMARST = 1;      // DMARST[31:31]   [0] in [0(no effect) 1(reset; reset by w0)]
      SCUCG->PPRST2_b.DMARST = 0;      // DMARST[31:31]   [0] in [0(no effect) 1(reset; reset by w0)]
   }

   // select peripheral address: PAR
   DMACx->PAR = PeripheralAddress[PERSEL];

   // select peripheral: PERSEL
   DMACx->CR_b.PERSEL = PERSEL;

   // select direction: DIR
   DMACx->CR_b.DIR = DIR;

   // select size: SIZE
   DMACx->CR_b.SIZE = SIZE;

   // select ERFGSTP: ERFGSTP
   DMACx->CR_b.ERFGSTP = ERFGSTP;      // ERFGSTP[15:15]   [0] in [0(disable DMA Stop Function by an error of peripheral) 1(enable)]

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         DeInitialize DMAC peripheral
 * @param[in]     DMACx
 *                   Pointer to the target DMAC
 *                   -  DMAC0 ~ DMAC4
 * @return        @ref HAL_Status_Type
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_DMAC_DeInit( DMACn_Type* DMACx )
{
   /* Check DMAC handle */
   if( DMACx == NULL )
   {
      return HAL_ERROR;
   }

   return HAL_OK;
}

/*-------------------------------------------------------------------------*//**
 * @brief         Enable the transfer
 * @param[in]     DMACx
 *                   Pointer to the target DMAC
 *                   -  DMAC0 ~ DMAC4
 * @param[in]     MAR
 *                   Memory Address
 * @param[in]     TRANSCNT
 *                   The number of times to transfer
 * @return        @ref HAL_Status_Type
 * @details       This fuunction enables the DMA transfer.
 * @code
 *                [Example]
 *
 *                HAL_DMAC_Setup( ( DMACn_Type* )DMAC0, ( uint32_t )buffer, len );
 * @endcode
 *//*-------------------------------------------------------------------------*/
HAL_Status_Type HAL_DMAC_Setup( DMACn_Type* DMACx, uint32_t MAR, uint16_t TRANSCNT )
{
   /* Check DMAC handle */
   if( DMACx == NULL )
   {
      return HAL_ERROR;
   }

   // select memory address: MAR
   DMACx->MAR = MAR;

   // select count: TRANSCNT
   DMACx->CR_b.TRANSCNT = TRANSCNT;

   // enable channel
   DMACx->CR_b.CHnEN = 1;      // CHnEN[00:00]   [0] in [0(disable channel n) 1(enable; reset by transfer complete or error)]

   return HAL_OK;
}


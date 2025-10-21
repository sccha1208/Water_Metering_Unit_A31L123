/**
 *******************************************************************************
 * @file        A31L12x_it.h
 * @author      ABOV R&D Division
 * @brief       Interrupt Header File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __A31L12x_IT_H
#define __A31L12x_IT_H

/* Includes ----------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ----------------------------------------------------------- */
/* Exported constants ------------------------------------------------------- */
/* Exported macro ----------------------------------------------------------- */


/* Exported functions ------------------------------------------------------- */

void NMI_Handler( void );
void HardFault_Handler( void );
void MemManage_Handler( void );
void BusFault_Handler( void );
void UsageFault_Handler( void );
void SVC_Handler( void );
void DebugMon_Handler( void );
void PendSV_Handler( void );
void SysTick_Handler( void );

void LPUART_Handler( void );

#ifdef __cplusplus
}
#endif

#endif   /* __A31L12x_IT_H */


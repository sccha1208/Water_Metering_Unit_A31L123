/**
 *******************************************************************************
 * @file        A31L12x_it.c
 * @author      ABOV R&D Division
 * @brief       Interrupt Source File
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
#include "meter_protocol.h"

/* Private typedef ---------------------------------------------------------- */
/* Private define ----------------------------------------------------------- */
/* Private macro ------------------------------------------------------------ */
/* Private define ----------------------------------------------------------- */
/* Private function prototypes ---------------------------------------------- */
/* Private variables -------------------------------------------------------- */


/******************************************************************************/
/*            Cortex M0+ Processor Exceptions Handlers                        */
/******************************************************************************/

/*-------------------------------------------------------------------------*//**
 * @brief         This function handles NMI exception.
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void NMI_Handler( void )
{
}

/*-------------------------------------------------------------------------*//**
 * @brief         This function handles Hard Fault exception.
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void HardFault_Handler( void )
{
   /* Go to infinite loop when Hard Fault exception occurs */
   while( 1 )
   {
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         This function handles SVCall exception
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void SVC_Handler( void )
{
}

/*-------------------------------------------------------------------------*//**
 * @brief         This function handles PendSVC exception
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void PendSV_Handler( void )
{
}

/*-------------------------------------------------------------------------*//**
 * @brief         This function handles SysTick Handler.
 * @param         None
 * @return        None
 * @details       1ms마다 호출되어 시스템 틱 카운터 증가
 *//*-------------------------------------------------------------------------*/
void SysTick_Handler( void )
{
    // Meter 프로토콜 타임아웃을 위한 밀리초 카운터 증가
    Meter_SysTick_Increment();
}

/******************************************************************************/
/*                 A31L12x Peripherals Interrupt Handlers                     */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_A31L12x.s).                                                 */
/******************************************************************************/

/*-------------------------------------------------------------------------*//**
 * @brief         This function handles LPUART Handler.
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void LPUART_Handler( void )
{
   LPUART_IRQHandler_IT();
}


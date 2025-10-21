;/**************************************************************************//**
; * @file     startup_A31L12x_iar.s
; * @brief    CMSIS Core Device Startup File for
; *           A31L12x Device Series
; * @version  V5.00
; * @date     08. March 2016
; ******************************************************************************/
;/*
; * Copyright (c) 2009-2016 ARM Limited. All rights reserved.
; *
; * SPDX-License-Identifier: Apache-2.0
; *
; * Licensed under the Apache License, Version 2.0 (the License); you may
; * not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; * www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing, software
; * distributed under the License is distributed on an AS IS BASIS, WITHOUT
; * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.
; */

;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table


        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA

__vector_table
        DCD     sfe(CSTACK)               ; Top of Stack
        DCD     Reset_Handler             ; Reset Handler

        DCD     NMI_Handler               ; NMI Handler
        DCD     HardFault_Handler         ; Hard Fault Handler
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved

        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     SVC_Handler               ; SVCall Handler
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     PendSV_Handler            ; PendSV Handler
        DCD     SysTick_Handler           ; SysTick Handler

        ; External Interrupts
        DCD     LVI_Handler               ; IRQ 0
        DCD     WUT_Handler               ; IRQ 1
        DCD     WDT_Handler               ; IRQ 2
        DCD     EINT0_Handler             ; IRQ 3
        DCD     EINT1_Handler             ; IRQ 4
        DCD     EINT2_Handler             ; IRQ 5
        DCD     EINT3_Handler             ; IRQ 6
        DCD     TIMER40_Handler           ; IRQ 7
        DCD     TIMER41_Handler           ; IRQ 8
        DCD     TIMER42_Handler           ; IRQ 9
        DCD     I2C0_Handler              ; IRQ 10
        DCD     USART10_Handler           ; IRQ 11
        DCD     SPI0_Handler              ; IRQ 12
        DCD     SPI1_Handler              ; IRQ 13
        DCD     I2C1_Handler              ; IRQ 14
        DCD     TIMER50_Handler           ; IRQ 15
        DCD     SC0_Handler               ; IRQ 16
        DCD     SC1_Handler               ; IRQ 17
        DCD     ADC_Handler               ; IRQ 18
        DCD     UART0_Handler             ; IRQ 19
        DCD     UART1_Handler             ; IRQ 20
        DCD     TIMER43_Handler           ; IRQ 21
        DCD     CMPn_Handler              ; IRQ 22
        DCD     DMAC0_Handler             ; IRQ 23
        DCD     DMAC1_Handler             ; IRQ 24
        DCD     LPUART_Handler            ; IRQ 25
        DCD     0                         ; IRQ 26
        DCD     0                         ; IRQ 27
        DCD     RTCC_Handler              ; IRQ 28
        DCD     DMAC2_Handler             ; IRQ 29
        DCD     DMAC3_Handler             ; IRQ 30
        DCD     DMAC4_Handler             ; IRQ 31
__Vectors_End

__Vectors       EQU   __vector_table
__Vectors_Size  EQU   __Vectors_End - __Vectors


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
        B HardFault_Handler
        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B SVC_Handler
        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B SysTick_Handler

        PUBWEAK LVI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
LVI_Handler
        B LVI_Handler

        PUBWEAK WUT_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
WUT_Handler
        B WUT_Handler

        PUBWEAK WDT_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
WDT_Handler
        B WDT_Handler

        PUBWEAK EINT0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
EINT0_Handler
        B EINT0_Handler

        PUBWEAK EINT1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
EINT1_Handler
        B EINT1_Handler

        PUBWEAK EINT2_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
EINT2_Handler
        B EINT2_Handler

        PUBWEAK EINT3_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
EINT3_Handler
        B EINT3_Handler

        PUBWEAK TIMER40_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER40_Handler
        B TIMER40_Handler

        PUBWEAK TIMER41_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER41_Handler
        B TIMER41_Handler

        PUBWEAK TIMER42_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER42_Handler
        B TIMER42_Handler

        PUBWEAK I2C0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C0_Handler
        B I2C0_Handler

        PUBWEAK USART10_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
USART10_Handler
        B USART10_Handler

        PUBWEAK SPI0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPI0_Handler
        B SPI0_Handler

        PUBWEAK SPI1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPI1_Handler
        B SPI1_Handler

        PUBWEAK I2C1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C1_Handler
        B I2C1_Handler

        PUBWEAK TIMER50_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER50_Handler
        B TIMER50_Handler

        PUBWEAK SC0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SC0_Handler
        B SC0_Handler

        PUBWEAK SC1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SC1_Handler
        B SC1_Handler

        PUBWEAK ADC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
ADC_Handler
        B ADC_Handler

        PUBWEAK UART0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART0_Handler
        B UART0_Handler

        PUBWEAK UART1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART1_Handler
        B UART1_Handler

        PUBWEAK TIMER43_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER43_Handler
        B TIMER43_Handler

        PUBWEAK CMPn_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
CMPn_Handler
        B CMPn_Handler

        PUBWEAK DMAC0_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMAC0_Handler
        B DMAC0_Handler

        PUBWEAK DMAC1_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMAC1_Handler
        B DMAC1_Handler

        PUBWEAK LPUART_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
LPUART_Handler
        B LPUART_Handler

        PUBWEAK RTCC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
RTCC_Handler
        B RTCC_Handler

        PUBWEAK DMAC2_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMAC2_Handler
        B DMAC2_Handler        

        PUBWEAK DMAC3_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMAC3_Handler
        B DMAC3_Handler        

        PUBWEAK DMAC4_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMAC4_Handler
        B DMAC4_Handler        

        END


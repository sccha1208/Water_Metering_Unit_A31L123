;/**************************************************************************//**
; * @file     startup_A31L12x.s
; * @brief    CMSIS Core Device Startup File for
; *           A31L12x Device Series
; * @version  V5.00
; * @date     02. March 2016
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

;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000200

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000100

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
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

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

DEVICE_Handler  PROC

                EXPORT  LVI_Handler               [WEAK]
                EXPORT  WUT_Handler               [WEAK]
                EXPORT  WDT_Handler               [WEAK]
                EXPORT  EINT0_Handler             [WEAK]
                EXPORT  EINT1_Handler             [WEAK]
                EXPORT  EINT2_Handler             [WEAK]
                EXPORT  EINT3_Handler             [WEAK]
                EXPORT  TIMER40_Handler           [WEAK]
                EXPORT  TIMER41_Handler           [WEAK]
                EXPORT  TIMER42_Handler           [WEAK]
                EXPORT  I2C0_Handler              [WEAK]
                EXPORT  USART10_Handler           [WEAK]
                EXPORT  SPI0_Handler              [WEAK]
                EXPORT  SPI1_Handler              [WEAK]
                EXPORT  I2C1_Handler              [WEAK]
                EXPORT  TIMER50_Handler           [WEAK]
                EXPORT  SC0_Handler               [WEAK]
                EXPORT  SC1_Handler               [WEAK]
                EXPORT  ADC_Handler               [WEAK]
                EXPORT  UART0_Handler             [WEAK]
                EXPORT  UART1_Handler             [WEAK]
                EXPORT  TIMER43_Handler           [WEAK]
                EXPORT  CMPn_Handler              [WEAK]
                EXPORT  DMAC0_Handler             [WEAK]
                EXPORT  DMAC1_Handler             [WEAK]
                EXPORT  LPUART_Handler            [WEAK]
                EXPORT  RTCC_Handler              [WEAK]
                EXPORT  DMAC2_Handler             [WEAK]
                EXPORT  DMAC3_Handler             [WEAK]
                EXPORT  DMAC4_Handler             [WEAK]

LVI_Handler
WUT_Handler
WDT_Handler
EINT0_Handler
EINT1_Handler
EINT2_Handler
EINT3_Handler
TIMER40_Handler
TIMER41_Handler
TIMER42_Handler
I2C0_Handler
USART10_Handler
SPI0_Handler
SPI1_Handler
I2C1_Handler
TIMER50_Handler
SC0_Handler
SC1_Handler
ADC_Handler
UART0_Handler
UART1_Handler
TIMER43_Handler
CMPn_Handler
DMAC0_Handler
DMAC1_Handler
LPUART_Handler
RTCC_Handler
DMAC2_Handler
DMAC3_Handler
DMAC4_Handler
                B       .

                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap

__user_initial_stackheap PROC
                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR
                ENDP

                ALIGN

                ENDIF


                END


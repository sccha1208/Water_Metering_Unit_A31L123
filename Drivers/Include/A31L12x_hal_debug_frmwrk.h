/**
 *******************************************************************************
 * @file        A31L12x_hal_debug_frmwrk.h
 * @author      ABOV R&D Division
 * @brief       Debug Framework Header File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#ifndef _DEBUG_FRMWRK_H_
#define _DEBUG_FRMWRK_H_

#include "A31L12x_hal_uartn.h"
#include "A31L12x_hal_libcfg.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _DEBUG_MSG

//******************************************************************************
// Constant
//******************************************************************************

#define USED_UART_DEBUG_PORT     1

#if (USED_UART_DEBUG_PORT == 0)
#define DEBUG_UART_PORT          UART0
#elif (USED_UART_DEBUG_PORT == 1)
#define DEBUG_UART_PORT          UART1
#endif

//******************************************************************************
// Macro
//******************************************************************************

#define _DBG( x )       _db_msg( (UARTn_Type*)DEBUG_UART_PORT, x )
#define _DBG_( x )      _db_msg_( (UARTn_Type*)DEBUG_UART_PORT, x )
#define _DBC( x )       _db_char( (UARTn_Type*)DEBUG_UART_PORT, x )
#define _DBD( x )       _db_dec( (UARTn_Type*)DEBUG_UART_PORT, x )
#define _DBD16( x )     _db_dec_16( (UARTn_Type*)DEBUG_UART_PORT, x )
#define _DBD32( x )     _db_dec_32( (UARTn_Type*)DEBUG_UART_PORT, x )
#define _DBH( x )       _db_hex( (UARTn_Type*)DEBUG_UART_PORT, x )
#define _DBH16( x )     _db_hex_16( (UARTn_Type*)DEBUG_UART_PORT, x )
#define _DBH32( x )     _db_hex_32( (UARTn_Type*)DEBUG_UART_PORT, x )
#define _DG()           _db_get_char( (UARTn_Type*)DEBUG_UART_PORT )
#define _DG_( x )       _db_get_ch( (UARTn_Type*)DEBUG_UART_PORT, x )

//******************************************************************************
// Variable
//******************************************************************************

extern void ( *_db_msg )( UARTn_Type* UARTx, const void* s );
extern void ( *_db_msg_ )( UARTn_Type* UARTx, const void* s );
extern void ( *_db_char )( UARTn_Type* UARTx, uint8_t ch );
extern void ( *_db_dec )( UARTn_Type* UARTx, uint8_t decn );
extern void ( *_db_dec_16 )( UARTn_Type* UARTx, uint16_t decn );
extern void ( *_db_dec_32 )( UARTn_Type* UARTx, uint32_t decn );
extern void ( *_db_hex )( UARTn_Type* UARTx, uint8_t hexn );
extern void ( *_db_hex_16 )( UARTn_Type* UARTx, uint16_t hexn );
extern void ( *_db_hex_32 )( UARTn_Type* UARTx, uint32_t hexn );
extern uint8_t ( *_db_get_char )( UARTn_Type* UARTx );
extern uint8_t ( *_db_get_ch )( UARTn_Type* UARTx, uint8_t* ch );

//******************************************************************************
// Function
//******************************************************************************

void UARTPutChar( UARTn_Type* UARTx, uint8_t ch );
void UARTPuts( UARTn_Type* UARTx, const void* str );
void UARTPuts_( UARTn_Type* UARTx, const void* str );
void UARTPutDec( UARTn_Type* UARTx, uint8_t decnum );
void UARTPutDec16( UARTn_Type* UARTx, uint16_t decnum );
void UARTPutDec32( UARTn_Type* UARTx, uint32_t decnum );
void UARTPutHex( UARTn_Type* UARTx, uint8_t hexnum );
void UARTPutHex16( UARTn_Type* UARTx, uint16_t hexnum );
void UARTPutHex32( UARTn_Type* UARTx, uint32_t hexnum );
uint8_t UARTGetChar( UARTn_Type* UARTx );
uint8_t UARTGetCh( UARTn_Type* UARTx, uint8_t* ch );
void cprintf( const char* format, ... );
void debug_frmwrk_init( void );
uint8_t getstring( void );

#endif   /* _DEBUG_MSG */

#ifdef __cplusplus
}
#endif

#endif   /* _DEBUG_FRMWRK_H_ */


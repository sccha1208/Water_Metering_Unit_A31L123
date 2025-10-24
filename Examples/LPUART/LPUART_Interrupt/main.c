/**
 *******************************************************************************
 * @file        main.c
 * @author      ABOV R&D Division
 * @brief       Main Example Code
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


//******************************************************************************
// Include
//******************************************************************************

#include "main_conf.h"
#include <string.h>
#include "meter_protocol.h"


/* Private typedef ---------------------------------------------------------- */
/* Private define ----------------------------------------------------------- */
/* Private macro ------------------------------------------------------------ */
/* Private variables -------------------------------------------------------- */
/* Private define ----------------------------------------------------------- */
/* Private function prototypes ---------------------------------------------- */

void IntTransmit( void );
void IntReceive( void );

uint32_t rbSend( uint8_t* txbuf, uint8_t buflen );
uint32_t rbReceive( uint8_t* rxbuf, uint8_t buflen );
void LPUART_IRQHandler_IT( void );
void DEBUG_Init( void );
void DEBUG_MenuPrint( void );
void LPUART_Configure( void );
void LPUART_InterruptRun( void );
void mainloop( void );
int main( void );
void Error_Handler( void );

// Meter protocol callback function prototypes
void OnMeterResponseReceived( uint8_t* data, uint16_t length );
void OnMeterError( METER_ERROR_Type error );
void Test_Protocol_Parser( void );

//******************************************************************************
// Constant
//******************************************************************************

// ring buffer size
#define RING_BUF_SIZE      32

//******************************************************************************
// Type
//******************************************************************************

typedef struct
{
   uint32_t    tx_head;                /**<  Tx ring buffer head index */
   uint32_t    tx_tail;                /**<  Tx ring buffer tail index */
   uint32_t    rx_head;                /**<  Rx ring buffer head index */
   uint32_t    rx_tail;                /**<  Rx ring buffer tail index */
   uint8_t     tx[RING_BUF_SIZE];      /**<  Tx data ring buffer */
   uint8_t     rx[RING_BUF_SIZE];      /**<  Rx data ring buffer */
} RING_BUF;

//******************************************************************************
// Macro
//******************************************************************************

// Buf mask
#define __BUF_MASK                        ( RING_BUF_SIZE - 1 )

// Check buf is full or not
#define __BUF_IS_FULL( head, tail )       ( (tail & __BUF_MASK) == ((head+1) & __BUF_MASK) )

// Check buf will be full in next receiving or not
#define __BUF_WILL_FULL( head, tail )     ( (tail & __BUF_MASK) == ((head+2) & __BUF_MASK) )

// Check buf is empty
#define __BUF_IS_EMPTY( head, tail )      ( (head & __BUF_MASK) == (tail & __BUF_MASK) )

// Reset buf
#define __BUF_INCR( bufidx )              ( bufidx = (bufidx+1) & __BUF_MASK )

/* Private variables -------------------------------------------------------- */
//******************************************************************************
// Variable
//******************************************************************************

const uint8_t menu[] =  "************************************************\n\r"
                        "Seoul Digital Water Meter Protocol V1.1~V1.4 \n\r"
                        "  - MCU:        A31L123 \n\r"
                        "  - Core:       Cortex-M0+ \n\r"
                        "  - Protocol:   1200bps, 8-N-1 \n\r"
                        "Target Peripheral: LPUART \n\r"
                        "Counterpart:       Digital Water Meter \n\r"
                        "Protocol Versions: V1.1, V1.2, V1.3, V1.4 \n\r"
                        "Auto Detection:    Enabled \n\r"
                        "UART TXD Pin:      PB3(LPTXD) \n\r"
                        "UART RXD Pin:      PB4(LPRXD) \n\r"
                        "************************************************\n\r"
                        "Press 't' to run Protocol Parser Test\n\r"
                        "************************************************\n\r\n\r";

// ring buffer
RING_BUF                rb;

// Current Tx Interrupt enable state
volatile FlagStatus     TxIntStat;

//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         UART transmit function (ring buffer used)
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void IntTransmit( void )
{
   // Disable THRE interrupt
   HAL_LPUART_ConfigInterrupt( LPUART_INTCFG_TXCIEN, DISABLE );




   while( !__BUF_IS_EMPTY( rb.tx_head, rb.tx_tail ) )
   {
      /* Move a piece of data into the transmit FIFO */
      if( HAL_LPUART_Transmit( ( uint8_t* )&rb.tx[rb.tx_tail], 1, NONE_BLOCKING ) )
      {
         /* Update transmit ring FIFO tail pointer */
         __BUF_INCR( rb.tx_tail );
      }
      else
      {
         break;
      }
   }

   /* If there is no more data to send, disable the transmit
      interrupt - else enable it or keep it enabled */
   if( __BUF_IS_EMPTY( rb.tx_head, rb.tx_tail ) )
   {
      HAL_LPUART_ConfigInterrupt( LPUART_INTCFG_TXCIEN, DISABLE );
      // Reset Tx Interrupt state
      TxIntStat = RESET;
   }
   else
   {
      // Set Tx Interrupt state
      TxIntStat = SET;
      HAL_LPUART_ConfigInterrupt( LPUART_INTCFG_TXCIEN, ENABLE );
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         UART receive function (ring buffer used)
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void IntReceive( void )
{
   uint8_t     tmpc;
   uint32_t    rLen;

   while( 1 )
   {
      // Call UART receive function in UART driver
      rLen = HAL_LPUART_Receive( &tmpc, 1, NONE_BLOCKING );

      // If data received
      if( rLen )
      {
         /* Check if buffer is more space
          * If no more space, remaining character will be trimmed out
          */
         if( !__BUF_IS_FULL( rb.rx_head, rb.rx_tail ) )
         {
            rb.rx[rb.rx_head] = tmpc;
            __BUF_INCR( rb.rx_head );
         }
      }
      // no more data
      else
      {
         break;
      }
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         LPUART transmit function for interrupt mode (using ring buffers)
 * @param[in]     txbuf
 *                   Pointer to Transmit buffer
 * @param[in]     buflen
 *                   Length of Transmit buffer
 * @return        Number of bytes actually sent to the ring buffer
 *//*-------------------------------------------------------------------------*/
uint32_t rbSend( uint8_t* txbuf, uint8_t buflen )
{
   uint8_t*    data = ( uint8_t* ) txbuf;
   uint32_t    bytes = 0;

   /* Temporarily lock out UART transmit interrupts during this
      read so the UART transmit interrupt won't cause problems
      with the index values */
   HAL_LPUART_ConfigInterrupt( LPUART_INTCFG_TXCIEN, DISABLE );

   /* Loop until transmit run buffer is full or until n_bytes expires */
   while( ( buflen > 0 ) && ( !__BUF_IS_FULL( rb.tx_head, rb.tx_tail ) ) )
   {
      /* Write data from buffer into ring buffer */
      rb.tx[rb.tx_head] = *data;
      data++;

      /* Increment head pointer */
      __BUF_INCR( rb.tx_head );

      /* Increment data count and decrement buffer size count */
      bytes++;
      buflen--;
   }

   /*
    * Check if current Tx interrupt enable is reset,
    * that means the Tx interrupt must be re-enabled
    * due to call IntTransmit() function to trigger
    * this interrupt type
    */
   if( TxIntStat == RESET )
   {
      IntTransmit();
   }
   /*
    * Otherwise, re-enables Tx Interrupt
    */
   else
   {
      HAL_LPUART_ConfigInterrupt( LPUART_INTCFG_TXCIEN, ENABLE );
   }

   return bytes;
}

/*-------------------------------------------------------------------------*//**
 * @brief         LPUART read function for interrupt mode (using ring buffers)
 * @param[out]    rxbuf
 *                   Pointer to Received buffer
 * @param[in]     buflen
 *                   Length of Received buffer
 * @return        Number of bytes actually read from the ring buffer
 *//*-------------------------------------------------------------------------*/
uint32_t rbReceive( uint8_t* rxbuf, uint8_t buflen )
{
   uint8_t*    data = ( uint8_t* ) rxbuf;
   uint32_t    bytes = 0;

   /* Temporarily lock out UART receive interrupts during this
      read so the UART receive interrupt won't cause problems
      with the index values */
   HAL_LPUART_ConfigInterrupt( LPUART_INTCFG_RXCIEN, DISABLE );

   /* Loop until receive buffer ring is empty or until max_bytes expires */
   while( ( buflen > 0 ) && ( !( __BUF_IS_EMPTY( rb.rx_head, rb.rx_tail ) ) ) )
   {
      /* Read data from ring buffer into user buffer */
      *data = rb.rx[rb.rx_tail];
      data++;

      /* Update tail pointer */
      __BUF_INCR( rb.rx_tail );

      /* Increment data count and decrement buffer size count */
      bytes++;
      buflen--;
   }

   /* Re-enable UART interrupts */
   HAL_LPUART_ConfigInterrupt( LPUART_INTCFG_RXCIEN, ENABLE );

   return bytes;
}

/*-------------------------------------------------------------------------*//**
 * @brief         LPUART_IRQHandler_IT
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void LPUART_IRQHandler_IT( void )
{
   uint32_t    intsrc;

   // get interrupt flag
   intsrc = HAL_LPUART_GetStatus();

   // if Receive Complete Interrupt
   if( ( intsrc & LPUART_IFSR_RXCIFLAG_Msk ) == LPUART_IFSR_RXCIFLAG_Msk )
   {
      IntReceive();
      // Clear RX interrupt flag
      HAL_LPUART_ClearStatus( LPUART_STATUS_RXCIFLAG );
   }

   // if Transmit Complete Interrupt
   if( ( intsrc & LPUART_IFSR_TXCIFLAG_Msk ) == LPUART_IFSR_TXCIFLAG_Msk )
   {
      IntTransmit();
      // Clear TX interrupt flag
      HAL_LPUART_ClearStatus( LPUART_STATUS_TXCIFLAG );
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         DEBUG_Init
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void DEBUG_Init( void )
{
#ifdef _DEBUG_MSG
   debug_frmwrk_init();

   // Test: Send a simple message directly to verify UART1 is working
   {
      volatile uint32_t delay;
      uint8_t test_msg[] = "UART1 DEBUG TEST\n\r";
      uint8_t i;

      for(delay = 0; delay < 100000; delay++);  // Small delay after init

      for(i = 0; test_msg[i] != '\0'; i++)
      {
         UARTPutChar((UARTn_Type*)UART1, test_msg[i]);
      }
   }
#endif
}

/*-------------------------------------------------------------------------*//**
 * @brief         DEBUG_MenuPrint
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void DEBUG_MenuPrint( void )
{
#ifdef _DEBUG_MSG
   _DBG( menu );
#endif
}

/*-------------------------------------------------------------------------*//**
 * @brief         LPUART_Configure
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void LPUART_Configure( void )
{
   LPUART_CFG_Type      LPUART_Config;

   // init flag
   TxIntStat = RESET;

   // Reset ring buf head and tail idx
   rb.rx_head = 0;
   rb.rx_tail = 0;
   rb.tx_head = 0;
   rb.tx_tail = 0;

   // LPUART configuration
   {
      // uart TXD/RXD pin configuration
      {
         _DBG( "\n\r========================================\n\r" );
         _DBG( "LPUART Pin Configuration\n\r" );
         _DBG( "========================================\n\r" );

         // configure PB3 as a LPTXD                              // PA8            0: ----        1: ----        2: LPTXD       3: ----        4: AN8         5: ----        6: ----        7: SEG14
         HAL_GPIO_ConfigOutput( ( Pn_Type* )PB, 3, ALTERN_FUNC );      // PB3            0: T41OUTA     1: T41INP      2: LPTXD       3: SCK0        4: SCL1        5: ----        6: ADTRG       7: SEG9
         HAL_GPIO_ConfigFunction( ( Pn_Type* )PB, 3, AFSRx_AF2 );   // PC8            0: ----        1: ----        2: LPTXD       3: SCK10       4: SC0CLK      5: ----        6: ----        7: COM4/SEG25
         _DBG( "PB3 configured as LPTXD (AF2)\n\r" );

         // configure PB4 as a LPRXD                              // PA9            0: ----        1: ----        2: LPRXD       3: ----        4: AN9         5: ----        6: ----        7: SEG13
         HAL_GPIO_ConfigOutput( ( Pn_Type* )PB, 4, ALTERN_FUNC );      // PB4            0: T41OUTB     1: EC41        2: LPRXD       3: ----        4: SDA1        5: ----        6: ADTRG       7: SEG8
         HAL_GPIO_ConfigFunction( ( Pn_Type* )PB, 4, AFSRx_AF2 );   // PC7            0: T40OUTA     1: T40INP      2: RXD10       3: MISO10      4: SC0RST      5: SC0RXD      6: SS1         7: SEG24
         _DBG( "PB4 configured as LPRXD (AF2)\n\r" );

         // 레지스터 상태 출력
         _DBG( "PB->MOD (PB3/PB4): 0x" );
         _DBH( (uint8_t)((PB->MOD >> 6) & 0xFF) );
         _DBG( "\n\r" );

         _DBG( "PB->AFSR1 (PB3/PB4): 0x" );
         _DBH( (uint8_t)((PB->AFSR1 >> 12) & 0xFF) );
         _DBG( "\n\r" );
         _DBG( "========================================\n\r\n\r" );
      }

      // uart configuration
      {
         _DBG( "========================================\n\r" );
         _DBG( "LPUART Configuration\n\r" );
         _DBG( "========================================\n\r" );

         // Seoul Digital Water Meter Protocol: 1200bps, 8-N-1
         HAL_LPUART_ConfigStructInit( &LPUART_Config );

         // Baud rate setting: 1200 bps
         LPUART_Config.Baudrate = 1200;
         _DBG( "Baudrate: 1200 bps\n\r" );

         // select peripheral clock: PCLK
         HAL_SCU_Peripheral_ClockSelection( PPCLKSR_LPUTCLK, LPUTCLK_PCLK );
         LPUART_Config.BaseClock = SystemPeriClock;
         _DBG( "Base Clock: " );
         _DBD32( SystemPeriClock );
         _DBG( " Hz\n\r" );

         // init LPUART
         HAL_LPUART_Init( &LPUART_Config );
         _DBG( "LPUART Initialized\n\r" );

         // enable interrupt
         HAL_LPUART_ConfigInterrupt( LPUART_INTCFG_RXCIEN, ENABLE );
         HAL_LPUART_ConfigInterrupt( LPUART_INTCFG_TXCIEN, ENABLE );
         _DBG( "Interrupts Enabled (RX/TX)\n\r" );

         // enable LPUART
         HAL_LPUART_Enable( ENABLE );
         _DBG( "LPUART Enabled\n\r" );

         // LPUART 레지스터 상태 출력
         _DBG( "\n\rLPUART Register Status:\n\r" );
         _DBG( "  CR1: 0x" );
         _DBH( (uint8_t)(LPUART->CR1 & 0xFF) );
         _DBG( "\n\r" );

         _DBG( "  BDR: 0x" );
         _DBH( (uint8_t)((LPUART->BDR >> 8) & 0xFF) );
         _DBH( (uint8_t)(LPUART->BDR & 0xFF) );
         _DBG( "\n\r" );

         _DBG( "========================================\n\r\n\r" );
      }

      // unmask interrupt
      NVIC_SetPriority( LPUART_IRQn, 3 );
      NVIC_EnableIRQ( LPUART_IRQn );
      HAL_INT_EInt_MaskDisable( MSK_LPUART );
   }

   // LPUART 하드웨어 안정화를 위한 딜레이
   {
      volatile uint32_t delay;
      for( delay = 0; delay < 100000; delay++ );
   }

   // LPUART 전송 테스트 (간단한 바이트 전송)
   {
      _DBG( "\n\rLPUART Test: Sending 0xAA...\n\r" );

      uint8_t test_byte = 0xAA;
      HAL_LPUART_Transmit( &test_byte, 1, BLOCKING );

      volatile uint32_t delay;
      for( delay = 0; delay < 100000; delay++ );

      _DBG( "LPUART Test: 0xAA sent\n\r" );
   }

   // 첫 전송 시 0x10 누락 방지: 더미 바이트 전송
   {
      uint8_t dummy = 0xFF;
      HAL_LPUART_Transmit( &dummy, 1, BLOCKING );

      // 더미 바이트 전송 후 대기
      for( volatile uint32_t delay = 0; delay < 10000; delay++ );

      // LPUART 비활성화/재활성화로 FIFO 완전 클리어
      HAL_LPUART_Enable( DISABLE );
      for( volatile uint32_t delay = 0; delay < 1000; delay++ );
      HAL_LPUART_Enable( ENABLE );

      // 재안정화
      for( volatile uint32_t delay = 0; delay < 10000; delay++ );
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Meter response received callback
 * @param         data - Received data
 * @param         length - Data length
 * @return        None
 *//*-------------------------------------------------------------------------*/
// Global buffer to store received meter data
static uint8_t g_meter_rx_data[256];
static uint16_t g_meter_rx_length = 0;  // Length of last received frame

void OnMeterResponseReceived( uint8_t* data, uint16_t length )
{
   // Store data to global buffer for later use
   if( length <= sizeof( g_meter_rx_data ) )
   {
      memcpy( g_meter_rx_data, data, length );
      g_meter_rx_length = length;

      // Prevent unused variable warning
      (void)g_meter_rx_length;
   }

   // 범용 파서 사용: 자동 버전 감지 및 파싱
   MeterData_t parsed_data;

   _DBG( "\n\r" );
   _DBG( "====================================\n\r" );
   _DBG( "  Meter Response Received\n\r" );
   _DBG( "====================================\n\r" );

   // 프레임 파싱 시도
   if( Meter_ParseFrame( data, length, &parsed_data ) )
   {
      // 파싱 성공: 구조화된 데이터 출력
      Meter_PrintParsedData( &parsed_data );
   }
   else
   {
      // 파싱 실패: 원본 HEX 데이터만 출력
      _DBG( "[ERROR] Failed to parse frame\n\r" );
      _DBG( "Raw frame (" );
      _DBD16( length );
      _DBG( " bytes):\n\r" );

      uint16_t i;
      for( i = 0; i < length; i++ )
      {
         _DBH( data[i] );
         _DBG( " " );

         // New line every 16 bytes
         if( ( i + 1 ) % 16 == 0 && i < length - 1 )
         {
            _DBG( "\n\r" );
         }
      }
      _DBG( "\n\r====================================\n\r\n\r" );
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Meter error callback
 * @param         error - Error code
 * @return        None
 *//*-------------------------------------------------------------------------*/
void OnMeterError( METER_ERROR_Type error )
{
   _DBG( "Meter Error: " );

   switch( error )
   {
      case METER_ERR_TIMEOUT:
         _DBG( "Timeout\n\r" );
         break;
      case METER_ERR_CHECKSUM:
         _DBG( "Checksum Error\n\r" );
         break;
      case METER_ERR_INVALID_FRAME:
         _DBG( "Invalid Frame\n\r" );
         break;
      case METER_ERR_NAK_RECEIVED:
         _DBG( "NAK Received\n\r" );
         break;
      case METER_ERR_BUFFER_OVERFLOW:
         _DBG( "Buffer Overflow\n\r" );
         break;
      default:
         _DBG( "Unknown Error\n\r" );
         break;
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         LPUART_InterruptRun
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void LPUART_InterruptRun( void )
{
   // Actual TX frame: 10-5B-01-5C-16
   uint8_t     test_data[] = { 0x5C };  // Data 1 byte
   uint8_t     meter_cmd = 0x5B;        // Command
   uint8_t     meter_addr = 0x01;       // Address (not used)
   volatile uint32_t delay;

   // Initialize meter protocol
   Meter_Init();

   // Register callback functions
   Meter_SetResponseCallback( OnMeterResponseReceived );
   Meter_SetErrorCallback( OnMeterError );

   _DBG( "\n\rSeoul Digital Water Meter Protocol Initialized\n\r" );
   _DBG( "Baudrate: 1200 bps, Format: 8-N-1\n\r" );
   _DBG( "Auto Version Detection: V1.1, V1.2, V1.3, V1.4\n\r\n\r" );

   /* Infinite loop */
   while( 1 )
   {
      // Execute meter protocol task (RX processing and timeout check)
      Meter_Task();

      // Check for debug UART input (UART1)
      // Press 't' to run Protocol Parser Test
      if( HAL_UART_GetLineStatus( (UARTn_Type*)UART1 ) & UARTn_LSR_RDR )
      {
         uint8_t ch = HAL_UART_ReceiveByte( (UARTn_Type*)UART1 );

         if( ch == 't' || ch == 'T' )
         {
            _DBG( "\n\rRunning Protocol Parser Test...\n\r" );
            Test_Protocol_Parser();
            _DBG( "Test complete. Press 't' again to re-run.\n\r\n\r" );
         }
      }

      // Test: Send command every 5 seconds
      static uint32_t tick_counter = 0;

      tick_counter++;

      // Send every ~5 seconds (adjust based on loop speed)
      if( tick_counter >= 500000 )
      {
         tick_counter = 0;

         _DBG( "\n\rSending command to meter (Frame: 10-5B-01-5C-16)...\n\r" );

         // Send command to meter
         // TX Frame: [0x10] [0x5B] [0x01] [0x5C] [0x16]
         //            HEADER  CMD    LEN   DATA   CHECKSUM
         METER_ERROR_Type err = Meter_SendCommand(
            meter_addr,                    // Meter address (not used)
            meter_cmd,                     // Command: 0x5B
            test_data,                     // Data: 0x5C
            sizeof( test_data )            // Data length: 1
         );

         if( err != METER_ERR_NONE )
         {
            _DBG( "Failed to send command\n\r" );
         }
      }

      // 작은 지연
      for( delay = 0; delay < 100; delay++ )
      {
         __NOP();
      }
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         Main loop
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void mainloop( void )
{
   volatile uint32_t delay;

   /* Configure menu prinf */
   DEBUG_MenuPrint();

   /* LPUART_Configure */
   LPUART_Configure();

   /* Enable IRQ Interrupts */
   __enable_irq();

   /* LPUART hardware stabilization delay */
   for( delay = 0; delay < 500000; delay++ );

   /* LPUART_InterruptRun */
   LPUART_InterruptRun();
}

/*-------------------------------------------------------------------------*//**
 * @brief         Main program
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
int main( void )
{
   /* Initialize all port */
   Port_Init();

   /* Configure the system clock to HSI 32MHz */
   SystemClock_Config();

   /* Initialize Debug frame work through initializing USART port  */
   DEBUG_Init();

   /* Configure SysTick for 1ms tick (Meter protocol timeout support) */
   // SystemCoreClock = 32MHz
   // SysTick_Config(32000) = 1ms 인터럽트
   SysTick_Config( SystemCoreClock / 1000 );

   /* Infinite loop */
   mainloop();

   return 0;
}

/*-------------------------------------------------------------------------*//**
 * @brief         프로토콜 파서 테스트 함수 (V1.1~V1.4 샘플 데이터)
 * @param         None
 * @return        None
 * @note          4개 버전의 실제 프레임 샘플로 파서 테스트
 *//*-------------------------------------------------------------------------*/
void Test_Protocol_Parser( void )
{
   _DBG("\r\n");
   _DBG("==================================================\r\n");
   _DBG("  Seoul Water Meter Protocol Parser Test\r\n");
   _DBG("  Testing V1.1, V1.2, V1.3, V1.4\r\n");
   _DBG("==================================================\r\n");
   _DBG("\r\n");

   MeterData_t parsed_data;

   //==========================================================================
   // TEST 1: V1.1 샘플 프레임 (UDF 없음, L=12)
   //==========================================================================
   // 68 0C 0C 68 08 01 78 0F 12 34 56 78 84 1C 13 00 00 12 34 D7 16
   // UserData 길이 = 12 (0x0C - 3 = 9가 아니라 L=12)
   // MDH(0F) + ID(12345678) + Status(84) + DIF(1C) + VIF(13) + Data(00001234)
   _DBG("==================================================\r\n");
   _DBG("TEST 1: Protocol V1.1 (2013.06)\r\n");
   _DBG("==================================================\r\n");

   uint8_t frame_v11[] = {
      0x68, 0x0F, 0x0F, 0x68,  // Start + L + L + Start (L = C + A + CI + UserData = 3 + 12 = 15)
      0x08, 0x01, 0x78,        // C + A + CI
      0x0F,                    // MDH
      0x12, 0x34, 0x56, 0x78,  // ID (BCD): 78563412
      0x84,                    // Status: 10000100 (Q4초과, Batt Low)
      0x1C,                    // DIF: 구경 정보
      0x13,                    // VIF: 소수점 3
      0x00, 0x00, 0x12, 0x34,  // Data (BCD): 34120000
      0xC8,                    // Checksum (08+01+78+0F+12+34+56+78+84+1C+13+00+00+12+34=1C8, LSB=C8)
      0x16                     // End
   };

   if (Meter_ParseFrame(frame_v11, sizeof(frame_v11), &parsed_data))
   {
      Meter_PrintParsedData(&parsed_data);
   }
   else
   {
      _DBG("[ERROR] V1.1 Parsing Failed\r\n");
   }

   //==========================================================================
   // TEST 2: V1.2 샘플 프레임 (UDF 있음, Protocol Ver=0x12)
   //==========================================================================
   // 68 10 10 68 08 01 78 0F 12 34 56 78 A4 1C 13 00 00 56 78 12 05 00 41 E8 16
   // UserData 길이 = 16 (0x10 - 3 = 13이 아니라 L=16)
   // UDF: [12] [05] [00 41]
   _DBG("==================================================\r\n");
   _DBG("TEST 2: Protocol V1.2 (2021.05)\r\n");
   _DBG("==================================================\r\n");

   uint8_t frame_v12[] = {
      0x68, 0x13, 0x13, 0x68,  // Start + L + L + Start (L = C + A + CI + UserData(12) + UDF(4) = 3 + 16 = 19)
      0x08, 0x01, 0x78,        // C + A + CI
      0x0F,                    // MDH
      0x12, 0x34, 0x56, 0x78,  // ID (BCD): 78563412
      0xA4,                    // Status: 10100100 (Q3초과, 옥내누수, Batt Low)
      0x1C,                    // DIF
      0x13,                    // VIF: 소수점 3
      0x00, 0x00, 0x56, 0x78,  // Data (BCD): 78560000
      0x12,                    // UDF: Protocol Ver (V1.2)
      0x05,                    // UDF: Verification Month (5월)
      0x00, 0x41,              // UDF: Manufacturer Code (0x0041 = 'A')
      0x50,                    // Checksum (08+01+78+0F+12+34+56+78+A4+1C+13+00+00+56+78+12+05+00+41=350, LSB=50)
      0x16                     // End
   };

   if (Meter_ParseFrame(frame_v12, sizeof(frame_v12), &parsed_data))
   {
      Meter_PrintParsedData(&parsed_data);
   }
   else
   {
      _DBG("[ERROR] V1.2 Parsing Failed\r\n");
   }

   //==========================================================================
   // TEST 3: V1.3 샘플 프레임 (배터리 전압 5비트, Protocol Ver=0x13)
   //==========================================================================
   // 68 10 10 68 08 01 78 0F 12 34 56 78 BF 1C 13 00 90 12 34 13 06 00 41 xx 16
   // Status: 10111111 (Q3초과, 역류, 옥내누수, Batt=31)
   _DBG("==================================================\r\n");
   _DBG("TEST 3: Protocol V1.3 (2023.01)\r\n");
   _DBG("==================================================\r\n");

   uint8_t frame_v13[] = {
      0x68, 0x13, 0x13, 0x68,  // Start + L + L + Start (L = C + A + CI + UserData(12) + UDF(4) = 3 + 16 = 19)
      0x08, 0x01, 0x78,        // C + A + CI
      0x0F,                    // MDH
      0x12, 0x34, 0x56, 0x78,  // ID (BCD): 78563412
      0xBF,                    // Status: 10111111 (Q3, 역류, 누수, Batt=31)
      0x1C,                    // DIF
      0x13,                    // VIF: 소수점 3
      0x00, 0x90, 0x12, 0x34,  // Data (BCD): 34129000
      0x13,                    // UDF: Protocol Ver (V1.3)
      0x06,                    // UDF: Verification Month (6월)
      0x00, 0x41,              // UDF: Manufacturer Code
      0x86,                    // Checksum (08+01+78+0F+12+34+56+78+BF+1C+13+00+90+12+34+13+06+00+41=386, LSB=86)
      0x16                     // End
   };

   if (Meter_ParseFrame(frame_v13, sizeof(frame_v13), &parsed_data))
   {
      Meter_PrintParsedData(&parsed_data);
   }
   else
   {
      _DBG("[ERROR] V1.3 Parsing Failed\r\n");
   }

   //==========================================================================
   // TEST 4: V1.4 샘플 프레임 (Status2 포함, Protocol Ver=0x14)
   //==========================================================================
   // 68 10 10 68 08 01 78 0F 12 34 56 78 E5 1C D3 00 11 22 33 14 07 00 41 xx 16
   // Status1: 11100101 (Q3, 역류, 누수, Batt=5)
   // S2&VIF: 11010011 (자석감지=1, 동파경보=1, VIF=0x13)
   _DBG("==================================================\r\n");
   _DBG("TEST 4: Protocol V1.4 (2024.07)\r\n");
   _DBG("==================================================\r\n");

   uint8_t frame_v14[] = {
      0x68, 0x13, 0x13, 0x68,  // Start + L + L + Start (L = C + A + CI + UserData(12) + UDF(4) = 3 + 16 = 19)
      0x08, 0x01, 0x78,        // C + A + CI
      0x0F,                    // MDH
      0x12, 0x34, 0x56, 0x78,  // ID (BCD): 78563412
      0xE5,                    // Status1: 11100101 (Q3, 역류, 누수, Batt=5)
      0x1C,                    // DIF
      0xD3,                    // S2&VIF: 11010011 (자석감지, 동파경보, VIF=13)
      0x00, 0x11, 0x22, 0x33,  // Data (BCD): 33221100
      0x14,                    // UDF: Protocol Ver (V1.4)
      0x07,                    // UDF: Verification Month (7월)
      0x00, 0x41,              // UDF: Manufacturer Code
      0xAB,                    // Checksum (08+01+78+0F+12+34+56+78+E5+1C+D3+00+11+22+33+14+07+00+41=3AB, LSB=AB)
      0x16                     // End
   };

   if (Meter_ParseFrame(frame_v14, sizeof(frame_v14), &parsed_data))
   {
      Meter_PrintParsedData(&parsed_data);
   }
   else
   {
      _DBG("[ERROR] V1.4 Parsing Failed\r\n");
   }

   _DBG("==================================================\r\n");
   _DBG("  Protocol Parser Test Complete\r\n");
   _DBG("==================================================\r\n");
   _DBG("\r\n");
}

/*-------------------------------------------------------------------------*//**
 * @brief         Driver error handler
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void Error_Handler( void )
{
   while( 1 )
   {
   }
}

#ifdef  USE_FULL_ASSERT
/*-------------------------------------------------------------------------*//**
 * @brief         Reports the name of the source file and the source line number
 *                where the assert_param error has occurred.
 * @param         file: pointer to the source file name
 * @param         line: assert_param error line source number
 * @return        None
 *//*-------------------------------------------------------------------------*/
void check_failed( uint8_t* file, uint32_t line )
{
   /* User can add his own implementation to report the file name and line number,
      ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

   /* Infinite loop */
   while( 1 )
   {
   }
}
#endif


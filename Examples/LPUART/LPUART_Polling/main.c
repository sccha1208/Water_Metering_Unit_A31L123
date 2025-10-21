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

/* Private typedef ---------------------------------------------------------- */
/* Private define ----------------------------------------------------------- */
/* Private macro ------------------------------------------------------------ */
/* Private variables -------------------------------------------------------- */
/* Private define ----------------------------------------------------------- */
/* Private function prototypes ---------------------------------------------- */

void DEBUG_Init( void );
void DEBUG_MenuPrint( void );
void LPUART_Configure( void );
void LPUART_PollingRun( void );
void mainloop( void );
int main( void );
void Error_Handler( void );

/* Private variables -------------------------------------------------------- */

/* Private variables -------------------------------------------------------- */
//******************************************************************************
// Variable
//******************************************************************************

const uint8_t menu[] =  "************************************************\n\r"
                        "LPUART_Polling Example \n\r"
                        "  - MCU:        A31L123 \n\r"
                        "  - Core:       Cortex-M0+ \n\r"
                        "  - Monitoring: UART1 - 38400-8-N-1 \n\r"
                        "Target Peripheral: LPUART \n\r"
                        "Counterpart:       A external device like a PC \n\r"
                        "Tx Data:           Any character \n\r"
                        "UART TXD Pin:      PB3(LPTXD) \n\r"
                        "UART RXD Pin:      PB4(LPRXD) \n\r"
                        "************************************************\n\r\n\r";

//******************************************************************************
// Function
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         DEBUG_Init
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void DEBUG_Init( void )
{
#ifdef _DEBUG_MSG
   debug_frmwrk_init();
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
   LPUART_CFG_Type   LPUART_Config;

   // LPUART configuration
   {
      // uart TXD/RXD pin configuration
      {
         // print
         _DBG( "PB3 was configured as a LPTXD. \n\r" );
         _DBG( "PB4 was configured as a LPRXD. \n\r\n\r" );

         // configure PA8 as a LPTXD                              // PA8            0: ----        1: ----        2: LPTXD       3: ----        4: AN8         5: ----        6: ----        7: SEG14
         HAL_GPIO_ConfigOutput( ( Pn_Type* )PB, 3, ALTERN_FUNC );      // PB3            0: T41OUTA     1: T41INP      2: LPTXD       3: SCK0        4: SCL1        5: ----        6: ADTRG       7: SEG9
         HAL_GPIO_ConfigFunction( ( Pn_Type* )PB, 3, AFSRx_AF2 );   // PC8            0: ----        1: ----        2: LPTXD       3: SCK10       4: SC0CLK      5: ----        6: ----        7: COM4/SEG25

         // configure PA9 as a LPRXD                              // PA9            0: ----        1: ----        2: LPRXD       3: ----        4: AN9         5: ----        6: ----        7: SEG13
         HAL_GPIO_ConfigOutput( ( Pn_Type* )PB, 4, ALTERN_FUNC );      // PB4            0: T41OUTB     1: EC41        2: LPRXD       3: ----        4: SDA1        5: ----        6: ADTRG       7: SEG8
         HAL_GPIO_ConfigFunction( ( Pn_Type* )PB, 4, AFSRx_AF2 );   // PC7            0: T40OUTA     1: T40INP      2: RXD10       3: MISO10      4: SC0RST      5: SC0RXD      6: SS1         7: SEG24
      }

      // uart configuration
      {
         // get default configuration for uart: 38400-8-N-1
         HAL_LPUART_ConfigStructInit( &LPUART_Config );

         // select peripheral clock: PCLK
         HAL_SCU_Peripheral_ClockSelection( PPCLKSR_LPUTCLK, LPUTCLK_PCLK );
         LPUART_Config.BaseClock = SystemPeriClock;

         // init LPUART
         HAL_LPUART_Init( &LPUART_Config );

         // enable LPUART
         HAL_LPUART_Enable( ENABLE );
      }
   }
}

/*-------------------------------------------------------------------------*//**
 * @brief         LPUART_PollingRun
 * @param         None
 * @return        None
 *//*-------------------------------------------------------------------------*/
void LPUART_PollingRun( void )
{
   uint8_t     buf[100];   // tx/rx buffer
   uint32_t    len;

   /* Infinite loop */
   while( 1 )
   {
      // receive data from PC
      len = HAL_LPUART_Receive( buf, sizeof( buf ), BLOCKING );

      // send the received data to PC
      if( len > 0 )
      {
         HAL_LPUART_Transmit( buf, len, BLOCKING );
         if( buf[len - 1] == '\r' )
         {
            buf[0] = '\n';
            HAL_LPUART_Transmit( buf, 1, BLOCKING );
         }
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
   /* Configure menu prinf */
   DEBUG_MenuPrint();

   /* LPUART_Configure */
   LPUART_Configure();

   /* LPUART_PollingRun */
   LPUART_PollingRun();
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

   /* Infinite loop */
   mainloop();

   return 0;
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


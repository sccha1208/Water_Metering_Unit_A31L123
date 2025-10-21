/**
 *******************************************************************************
 * @file        main_conf.h
 * @author      ABOV R&D Division
 * @brief       Main Configuration Header File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#ifndef __A31L12x_CONF_H
#define __A31L12x_CONF_H

/* Includes ----------------------------------------------------------------- */
/* Uncomment the line below to enable peripheral header file inclusion */

#include "A31L12x.h"
#include "A31L12x_hal_aa_types.h"
#include "A31L12x_hal_debug_frmwrk.h"
#include "A31L12x_hal_libcfg.h"
#include "A31L12x_hal_wtidky.h"

#include "A31L12x_hal_adc.h"
#include "A31L12x_hal_cmpn.h"
#include "A31L12x_hal_crc.h"
#include "A31L12x_hal_dmacn.h"
#include "A31L12x_hal_fmc.h"
#include "A31L12x_hal_i2cn.h"
#include "A31L12x_hal_intc.h"
#include "A31L12x_hal_lcd.h"
#include "A31L12x_hal_lpuart.h"
#include "A31L12x_hal_pcu.h"
#include "A31L12x_hal_pmu.h"
#include "A31L12x_hal_pwr.h"
#include "A31L12x_hal_rtcc.h"
#include "A31L12x_hal_scn.h"
#include "A31L12x_hal_scu.h"
#include "A31L12x_hal_sculv.h"
#include "A31L12x_hal_spin.h"
#include "A31L12x_hal_timer4n.h"
#include "A31L12x_hal_timer5n.h"
#include "A31L12x_hal_uartn.h"
#include "A31L12x_hal_usart1n.h"
#include "A31L12x_hal_wdt.h"

#include "A31L12x_it.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* Private typedef ---------------------------------------------------------- */
/* Private define ----------------------------------------------------------- */
/* Private macro ------------------------------------------------------------ */
/* Private variables -------------------------------------------------------- */
/* Private define ----------------------------------------------------------- */


/* Private function prototypes ---------------------------------------------- */

/* Initialize all port */
void Port_Init( void );

/* Configure the system clock to 32MHz */
void SystemClock_Config( void );

#ifdef __cplusplus
}
#endif

#endif   /* __A31L12x_CONF_H */


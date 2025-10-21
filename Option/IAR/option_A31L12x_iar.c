/**
 *******************************************************************************
 * @file        option_A31L12x_iar.c
 * @author      ABOV R&D Division
 * @brief       IAR Option Source File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/
#include <stdint.h>

//---------------------- Configure Option Page 1 --------------------
// Read Protection
#define ReadProtLevel      Level_0                 // Read Protection, Select one of Level_0, Level_1, and Level_2
#define Level_0            0x00000003              // Level 0
#define Level_1            0x00000002              // Level 1
#define Level_2            0x00000000              // Level 2
#define ReadProtection     0x69C8A270 | ReadProtLevel

// Watch-Dog Timer Configuration
// WDTRC Enable
#define WdtrcEnSel         EnBySoftWare            // Select one of EnBySoftWare, EnButDisDpSp, and WdtAlwEnable
#define EnBySoftWare       0x000096D0              // By software
#define EnButDisDpSp       0x00002A70              // Enable but disabled at deep sleep mode
#define WdtAlwEnable       0x0000FFF0              // Always enable
// Watch-Dog Timer Clock
#define WdtClkSel          ClkBySoftware           // Select one of ClkBySoftware and WDgTimerClock
#define ClkBySoftware      0x00000000              // By software
#define WDgTimerClock      0x00000004              // WDTRC
// Watch-Dog Timer Reset
#define WdtRstSel          WRstBySoftware          // Select one of WdtRstMasterEn and WRstBySoftware
#define WdtRstMasterEn     0x00000000              // Enable watch-dog timer reset
#define WRstBySoftware     0x00000002              // By software
// Watch-Dog Timer Counter
#define WdtCntSel          WdtCntSoftware          // Select one of WdtCntMasterEn and WdtCntSoftware
#define WdtCntMasterEn     0x00000000              // Enable watch-dog timer counter
#define WdtCntSoftware     0x00000001              // By software
//
#define WdtConfig          WdtrcEnSel | WdtClkSel | WdtRstSel | WdtCntSel

// Low Voltage Reset Configuration
// LVR Operation Control
#define LvrOprConSel       LvrEnBySoftWare         // Select one of LvrEnBySoftWare and LvrAlwaysEnable
#define LvrEnBySoftWare    0x0000AA00              // By software
#define LvrAlwaysEnable    0x0000FF00              // Always enable
// LVR Voltage Level
#define LvrLevel           Lvr_1dot50              // Select one of Lvr_1dot50 to Lvr_2dot65
#define Lvr_1dot50         0x00000007              // 1.50V
#define Lvr_1dot75         0x00000006              // 1.75V
#define Lvr_1dot90         0x00000005              // 1.90V
#define Lvr_2dot05         0x00000004              // 2.05V
#define Lvr_2dot20         0x00000003              // 2.20V
#define Lvr_2dot35         0x00000002              // 2.35V
#define Lvr_2dot50         0x00000001              // 2.50V
#define Lvr_2dot65         0x00000000              // 2.65V
//
#define LvrConfig          LvrOprConSel | LvrLevel

// Configure Option Page Erase/Write Protection
// [2:0] --> Page3:Page2:Page1. If 0/1, Protection/None
#define OptPgConfig        0xffffffff

// Flash Memory Erase/Write Protection
// [31:0] --> Sector31:...:Sector0. If 0/1, Protection/None
#define FlashProConfig     0xffffffff

#pragma location= ".CONFIGURE_OPTION_1"
const uint32_t CONFIGURE_OPTION_1[32] =
         {
            // 0x0             0x4          0x8         0xC
            ReadProtection, 0xffffffff,  0xffffffff, WdtConfig,   // 0x0
            LvrConfig,      OptPgConfig, 0xffffffff, 0xffffffff,  // 0x1
            0xffffffff,     0xffffffff,  0xffffffff, 0xffffffff,  // 0x2
            0xffffffff,     0xffffffff,  0xffffffff, 0xffffffff,  // 0x3
            FlashProConfig, 0xffffffff,  0xffffffff, 0xffffffff,  // 0x4
            0xffffffff,     0xffffffff,  0xffffffff, 0xffffffff,  // 0x5
            0xffffffff,     0xffffffff,  0xffffffff, 0xffffffff,  // 0x6
            0xffffffff,     0xffffffff,  0xffffffff, 0xffffffff,  // 0x7
         };

#pragma location= ".CONFIGURE_OPTION_2"
const uint32_t CONFIGURE_OPTION_2[32] =
         {
            0xa2a2a2a2, 0xffffffff, 0xffffffff, 0xffffffff,       // 0x0
            0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,       // 0x1
            0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,       // 0x2
            0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,       // 0x3
            0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,       // 0x4
            0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,       // 0x5
            0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,       // 0x6
            0xffffffff, 0xffffffff, 0xffffffff, 0xa2a2a2a2,       // 0x7
         };

#pragma location= ".CONFIGURE_OPTION_3"
const uint32_t CONFIGURE_OPTION_3[32] =
         {            
            0xa3a3a3a3, 0xffffffff, 0xffffffff, 0xffffffff,       // 0x0
            0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,       // 0x1
            0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,       // 0x2
            0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,       // 0x3
            0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,       // 0x4
            0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,       // 0x5
            0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,       // 0x6
            0xffffffff, 0xffffffff, 0xffffffff, 0xa3a3a3a3        // 0x7
         };


;Configure option

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

                           ASEGN  ConfigPage1:CONST, 0x1ffff200
                           //     0x0             0x4          0x8         0xC
                           DCD    ReadProtection, 0xffffffff,  0xffffffff, WdtConfig   // 0x0
                           DCD    LvrConfig,      OptPgConfig, 0xffffffff, 0xffffffff  // 0x1
                           DCD    0xffffffff,     0xffffffff,  0xffffffff, 0xffffffff  // 0x2
                           DCD    0xffffffff,     0xffffffff,  0xffffffff, 0xffffffff  // 0x3
                           DCD    FlashProConfig, 0xffffffff,  0xffffffff, 0xffffffff  // 0x4
                           DCD    0xffffffff,     0xffffffff,  0xffffffff, 0xffffffff  // 0x5
                           DCD    0xffffffff,     0xffffffff,  0xffffffff, 0xffffffff  // 0x6
                           DCD    0xffffffff,     0xffffffff,  0xffffffff, 0xffffffff  // 0x7
                        
                           ASEGN  ConfigPage2:CONST, 0x1ffff400
                           // This is configure option page 2. You can modify the below data
                           //     0x0             0x4          0x8         0xC
                           DCD    0xa2a2a2a2, 0xffffffff, 0xffffffff, 0xffffffff       // 0x0
                           DCD    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff       // 0x1
                           DCD    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff       // 0x2
                           DCD    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff       // 0x3
                           DCD    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff       // 0x4
                           DCD    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff       // 0x5
                           DCD    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff       // 0x6
                           DCD    0xffffffff, 0xffffffff, 0xffffffff, 0xa2a2a2a2       // 0x7
        
                           ASEGN  ConfigPage3:CONST, 0x1ffff600
                           // This is configure option page 3. You can modify the below data
                           //     0x0             0x4          0x8         0xC
                           DCD    0xa3a3a3a3, 0xffffffff, 0xffffffff, 0xffffffff       // 0x0
                           DCD    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff       // 0x1
                           DCD    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff       // 0x2
                           DCD    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff       // 0x3
                           DCD    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff       // 0x4
                           DCD    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff       // 0x5
                           DCD    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff       // 0x6
                           DCD    0xffffffff, 0xffffffff, 0xffffffff, 0xa3a3a3a3       // 0x7
        
                           END


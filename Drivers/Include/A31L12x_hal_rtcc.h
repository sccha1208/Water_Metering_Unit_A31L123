/**
 *******************************************************************************
 * @file        A31L12x_hal_rtcc.h
 * @author      ABOV R&D Division
 * @brief       RTCC Header File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#ifndef _RTCC_H_
#define _RTCC_H_

#include "A31L12x.h"
#include "A31L12x_hal_aa_types.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

//==========   RTCC_CR   ========================================

//----------   RTCC En/Disable Definition   ----------
#define RTCC_RTDIS           (0x0uL << RTCC_CR_RTEN_Pos)
#define RTCC_RTEN            (0x1uL << RTCC_CR_RTEN_Pos)

//----------   RTCC Interrupt Interval Selection Definition   ----------
#define RTCC_RTIN_DIS        (0x0uL << RTCC_CR_RTIN_Pos)
#define RTCC_RTIN_500ms      (0x1uL << RTCC_CR_RTIN_Pos)
#define RTCC_RTIN_1sec       (0x2uL << RTCC_CR_RTIN_Pos)
#define RTCC_RTIN_1min       (0x3uL << RTCC_CR_RTIN_Pos)
#define RTCC_RTIN_1hour      (0x4uL << RTCC_CR_RTIN_Pos)
#define RTCC_RTIN_1day       (0x5uL << RTCC_CR_RTIN_Pos)
#define RTCC_RTIN_1month     (0x6uL << RTCC_CR_RTIN_Pos)

//----------   12/24-hour System Selection Definition   ----------
#define RTCC_HS_12hour       (0x0uL << RTCC_CR_HS24_Pos)
#define RTCC_HS_24hour       (0x1uL << RTCC_CR_HS24_Pos)

//----------   RTCOUT Selection Definition   ----------
#define RTCC_RTO_1Hz         (0x0uL << RTCC_CR_OUTSEL_Pos)
#define RTCC_RTO_32kHz       (0x1uL << RTCC_CR_OUTSEL_Pos)

//----------   RTCC Alarm Match Operation En/Disable Definition   ----------
#define RTCC_ALDIS           (0x0uL << RTCC_CR_ALEN_Pos)
#define RTCC_ALEN            (0x1uL << RTCC_CR_ALEN_Pos)

//----------   RTCC Alarm Match Interrupt En/Disable Definition   ----------
#define RTCC_ALIDIS           (0x0uL << RTCC_CR_ALIEN_Pos)
#define RTCC_ALIEN            (0x1uL << RTCC_CR_ALIEN_Pos)

//******************************************************************************
// Type
//******************************************************************************

//==============================================================================
// Enumeration
//==============================================================================

typedef enum
{
   RTCC_RTIN_Disable = 0,             /**<  Disable RTCC Interval Interrupt */
   RTCC_RTIN_OncePer500ms,            /**<  Once per 0.5 sec */
   RTCC_RTIN_OncePer1sec,             /**<  Once per 1 sec */
   RTCC_RTIN_OncePer1min,             /**<  Once per 1 min */
   RTCC_RTIN_OncePer1hour,            /**<  Once per 1 hour */
   RTCC_RTIN_OncePer1day,             /**<  Once per 1 day */
   RTCC_RTIN_OncePer1month            /**<  Once per 1 month */
} RTCC_INT_INTERVAL_OPT;

typedef enum
{
   RTCC_12HS = 0,    /**<  12-hour system */
   RTCC_24HS         /**<  24-hour system */
} RTCC_HS24_OPT;

typedef enum
{
   RTCC_RTO_1 = 0,    /**<  RTCOUT : 1Hz */
   RTCC_RTO_32K       /**<  RTCOUT : 32kHz */
} RTCC_RTCOUT_OPT;

typedef enum
{
   RTCC_Sunday = 0,             /**<  Day of the week : Sunday */
   RTCC_Monday,                 /**<  Day of the week : Monday */
   RTCC_Tuesday,                /**<  Day of the week : Tuesday */
   RTCC_Wednesday,              /**<  Day of the week : Wednesday */
   RTCC_Thursday,               /**<  Day of the week : Thursday */
   RTCC_Friday,                 /**<  Day of the week : Friday */
   RTCC_Saturday                /**<  Day of the week : Saturday */
} RTCC_WEEK_OPT;

//==============================================================================
// Structure
//==============================================================================

/**   Configuration structure in RTCC */
typedef struct
{
   uint8_t     rtccIntIn;    /**<  set initial Interrupt Interval value
                                    -  RTCC_RTIN_Disable,
                                       RTCC_RTIN_OncePer500ms,
                                       RTCC_RTIN_OncePer1sec,
                                       RTCC_RTIN_OncePer1min,
                                       RTCC_RTIN_OncePer1hour,
                                       RTCC_RTIN_OncePer1day,
                                       RTCC_RTIN_OncePer1month		  */

   uint8_t	   rtccHS24;	 /**<  set Hour System value
								                  -  RTCC_12HS, RTCC_24HS		   */

   uint8_t     rtccOutsel;    /**<  set RTCOUT Selection value
                                  -  RTCC_RTO_1, RTCC_RTO_32K */

} RTCC_CFG_Type;

//******************************************************************************
// Macro
//******************************************************************************
/*-------------------------------------------------------------------------*//**
 * @brief         Set RTCC Second Counter Register
 * @param[in]     u32SecData
 *                   Second Counter of RTCC
 * @details       This macro sets RTCC Second Counter Register
 *//*-------------------------------------------------------------------------*/
#define RTCC_SetSEC( u32SecData )      (RTCC->SEC = u32SecData)

/*-------------------------------------------------------------------------*//**
 * @brief         Set RTCC Minute Counter Register
 * @param[in]     u32MinData
 *                   Minute Counter of RTCC
 * @details       This macro sets RTCC Minute Counter Register
 *//*-------------------------------------------------------------------------*/
#define RTCC_SetMIN( u32MinData )      (RTCC->MIN = u32MinData)

/*-------------------------------------------------------------------------*//**
 * @brief         Set RTCC Hour Counter Register
 * @param[in]     u32HourData
 *                   Hour Counter of RTCC
 * @details       This macro sets RTCC Hour Counter Register
 *//*-------------------------------------------------------------------------*/
#define RTCC_SetHOUR( u32HourData )    (RTCC->HOUR = u32HourData)

/*-------------------------------------------------------------------------*//**
 * @brief         Set RTCC Day Counter Register
 * @param[in]     u32DayData
 *                   Day Counter of RTCC
 * @details       This macro sets RTCC Day Counter Register
 *//*-------------------------------------------------------------------------*/
#define RTCC_SetDAY( u32DayData )      (RTCC->DAY = u32DayData)

/*-------------------------------------------------------------------------*//**
 * @brief         Set RTCC Week Counter Register
 * @param[in]     u32WeekData
 *                   Week Counter of RTCC
 * @details       This macro sets RTCC Week Counter Register
 *//*-------------------------------------------------------------------------*/
#define RTCC_SetWEEK( u32WeekData )    (RTCC->WEEK = u32WeekData)

/*-------------------------------------------------------------------------*//**
 * @brief         Set RTCC Month Counter Register
 * @param[in]     u32MonthData
 *                   Month Counter of RTCC
 * @details       This macro sets RTCC Month Counter Register
 *//*-------------------------------------------------------------------------*/
#define RTCC_SetMONTH( u32MonthData )  (RTCC->MONTH = u32MonthData)

/*-------------------------------------------------------------------------*//**
 * @brief         Set RTCC Year Counter Register
 * @param[in]     u32YearData
 *                   Year Counter of RTCC
 * @details       This macro sets RTCC Year Counter Register
 *//*-------------------------------------------------------------------------*/
#define RTCC_SetYEAR( u32YearData )    (RTCC->YEAR = u32YearData)

/*-------------------------------------------------------------------------*//**
 * @brief         Get flags of RTCC Interval Interrupt
 * @details       This macro gets interrupt flag of RTCC Interval RTCC Interval Interrupt
 *//*-------------------------------------------------------------------------*/
#define RTCC_InInt_GetFg()             (RTCC->CR_b.RTIFLAG)

/*-------------------------------------------------------------------------*//**
 * @brief         Clear flags of RTCC Interval Interrupt
 * @details       This macro clears interrupt flag of RTCC Interval Interrupt
 *//*-------------------------------------------------------------------------*/
#define RTCC_InInt_ClrFg()             (RTCC->CR_b.RTIFLAG = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         RTCC device enumeration
 *//*-------------------------------------------------------------------------*/
/* RTCC Control register  */

#define RTCC_CR_RTIN_MASK              (RTCC_CR_RTIN_Msk)
#define RTCC_CR_RTIN_SET( n )          (n << RTCC_CR_RTIN_Pos)

#define RTCC_CR_HS24_MASK              (RTCC_CR_HS24_Msk)
#define RTCC_CR_HS24_SET( n )          (n << RTCC_CR_HS24_Pos)

#define RTCC_CR_OUTSEL_MASK            (RTCC_CR_OUTSEL_Msk)
#define RTCC_CR_OUTSEL_SET( n )        (n << RTCC_CR_OUTSEL_Pos)

//******************************************************************************
// Function
//******************************************************************************

HAL_Status_Type HAL_RTCC_Init( void* RTCC_Config );
void HAL_RTCC_DeInit( void );

void HAL_RTCC_SetRegister( uint32_t );
void HAL_RTCC_Cmd( FunctionalState NewState );
void HAL_RTCC_RewriteYEAR( uint32_t );
void HAL_RTCC_RewriteMONTH( uint32_t );
void HAL_RTCC_RewriteWEEK( uint32_t );
void HAL_RTCC_RewriteDAY( uint32_t );
void HAL_RTCC_RewriteHOUR( uint32_t );
void HAL_RTCC_RewriteMIN( uint32_t );
void HAL_RTCC_RewriteSEC( uint32_t );

#ifdef __cplusplus
}
#endif

#endif   /* _RTCC_H_ */

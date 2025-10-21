/**
 *******************************************************************************
 * @file        A31L12x_hal_timer4n.h
 * @author      ABOV R&D Division
 * @brief       Timer4n Header File
 *
 * Copyright 2020 ABOV Semiconductor Co.,Ltd. All rights reserved.
 *
 * This file is licensed under terms that are found in the LICENSE file
 * located at Document directory.
 * If this file is delivered or shared without applicable license terms,
 * the terms of the BSD-3-Clause license shall be applied.
 * Reference: https://opensource.org/licenses/BSD-3-Clause
 ******************************************************************************/


#ifndef _TIMER4n_H_
#define _TIMER4n_H_

#include "A31L12x.h"
#include "A31L12x_hal_aa_types.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constant
//******************************************************************************

//==========   TIMER4n_CR   ========================================

//----------   TIMER4n Output Force Level En/Disable Definition   ----------
#define TIMER4n_FRCDIS           (0x0uL << TIMER4n_CR_T4nFRCEN_Pos)
#define TIMER4n_FRCEN            (0x1uL << TIMER4n_CR_T4nFRCEN_Pos)

//----------   TIMER4n Force Input Selection Definition   ----------
#define TIMER4n_FRCS_T40         (0x0uL << TIMER4n_CR_T4nFRCS_Pos)
#define TIMER4n_FRCS_T41         (0x1uL << TIMER4n_CR_T4nFRCS_Pos)
#define TIMER4n_FRCS_T42         (0x2uL << TIMER4n_CR_T4nFRCS_Pos)
#define TIMER4n_FRCS_T43         (0x3uL << TIMER4n_CR_T4nFRCS_Pos)

//----------   TIMER4n Counter Sharing En/Disable Definition   ----------
#define TIMER4n_CNTSHDIS         (0x0uL << TIMER4n_CR_CNTSHEN_Pos)
#define TIMER4n_CNTSHEN          (0x1uL << TIMER4n_CR_CNTSHEN_Pos)

//----------   TIMER4n Counter Sharing Selection Definition   ----------
#define TIMER4n_CNTSH_T40        (0x0uL << TIMER4n_CR_CNTSH_Pos)
#define TIMER4n_CNTSH_T41        (0x1uL << TIMER4n_CR_CNTSH_Pos)
#define TIMER4n_CNTSH_T42        (0x2uL << TIMER4n_CR_CNTSH_Pos)
#define TIMER4n_CNTSH_T43        (0x3uL << TIMER4n_CR_CNTSH_Pos)

//----------   TIMER4n Enable/Disable Definition   ----------
#define TIMER4n_DISABLE          (0x0uL << TIMER4n_CR_T4nEN_Pos)
#define TIMER4n_ENABLE           (0x1uL << TIMER4n_CR_T4nEN_Pos)

//----------   TIMER4n Clock Selection Definition   ----------
#define TIMER4n_CLKINT           (0x0uL << TIMER4n_CR_T4nCLK_Pos)
#define TIMER4n_CLKEXT           (0x1uL << TIMER4n_CR_T4nCLK_Pos)

//----------   TIMER4n Mode Selection Definition   ----------
#define TIMER4n_INVM             (0x0uL << TIMER4n_CR_T4nMS_Pos)
#define TIMER4n_CAPM             (0x1uL << TIMER4n_CR_T4nMS_Pos)
#define TIMER4n_BTOB             (0x2uL << TIMER4n_CR_T4nMS_Pos)
#define TIMER4n_OSINVM           (0x3uL << TIMER4n_CR_T4nMS_Pos)

//----------   TIMER4n External Clock Edge Selection Definition   ----------
#define TIMER4n_FEDGE            (0x0uL << TIMER4n_CR_T4nECE_Pos)
#define TIMER4n_REDGE            (0x1uL << TIMER4n_CR_T4nECE_Pos)

//----------   TIMER4n Output Pair Selection Definition   ----------
#define TIMER4n_NO_OPAIR         (0x0uL << TIMER4n_CR_T4nOPAIR_Pos)
#define TIMER4n_OPAIR            (0x1uL << TIMER4n_CR_T4nOPAIR_Pos)

//----------   TIMER4n Delay Time Insert En/Disable Definition   ----------
#define TIMER4n_DLYDIS           (0x0uL << TIMER4n_CR_DLYEN_Pos)
#define TIMER4n_DLYEN            (0x1uL << TIMER4n_CR_DLYEN_Pos)

//----------   TIMER4n Delay Timer Insertion Position Definition   ----------
#define TIMER4n_INSFRONT         (0x0uL << TIMER4n_CR_DLYPOS_Pos)
#define TIMER4n_INSBACK          (0x1uL << TIMER4n_CR_DLYPOS_Pos)

//----------   TIMER4n Data Reload Time Selection Definition   ----------
#define TIMER4n_UPWRITE          (0x0uL << TIMER4n_CR_UPDT_Pos)
#define TIMER4n_UPMATCH          (0x1uL << TIMER4n_CR_UPDT_Pos)
#define TIMER4n_UPBOTTOM         (0x2uL << TIMER4n_CR_UPDT_Pos)

//----------   TIMER4n Input Capture/��Force level�� Polarity Selection Definition   ----------
#define TIMER4n_INPOL_FALL       (0x0uL << TIMER4n_CR_T4nINPOL_Pos)
#define TIMER4n_INPOL_RISE       (0x1uL << TIMER4n_CR_T4nINPOL_Pos)
#define TIMER4n_INPOL_BOTH       (0x2uL << TIMER4n_CR_T4nINPOL_Pos)

//==========   TIMER4n_OUTCR   ========================================

//----------   TIMER4n T4nOUTB Output Polarity Selection Definition   ----------
#define TIMER4n_OUT_POLBLOW      (0x0uL << TIMER4n_OUTCR_POLB_Pos)
#define TIMER4n_OUT_POLBHIGH     (0x1uL << TIMER4n_OUTCR_POLB_Pos)

//----------   TIMER4n T4nOUTA Output Polarity Selection Definition   ----------
#define TIMER4n_OUT_POLALOW      (0x0uL << TIMER4n_OUTCR_POLA_Pos)
#define TIMER4n_OUT_POLAHIGH     (0x1uL << TIMER4n_OUTCR_POLA_Pos)

//----------   TIMER4n T4nOUTB Output En/Disable Definition   ----------
#define TIMER4n_OUT_BOEDIS       (0x0uL << TIMER4n_OUTCR_T4nBOE_Pos)
#define TIMER4n_OUT_BOEEN        (0x1uL << TIMER4n_OUTCR_T4nBOE_Pos)

//----------   TIMER4n T4nOUTA Output En/Disable Definition   ----------
#define TIMER4n_OUT_AOEDIS       (0x0uL << TIMER4n_OUTCR_T4nAOE_Pos)
#define TIMER4n_OUT_AOEEN        (0x1uL << TIMER4n_OUTCR_T4nAOE_Pos)

//----------   TIMER4n T4nOUTB Output When Disable   ----------
#define TIMER4n_OUT_LVLBLOW      (0x0uL << TIMER4n_OUTCR_LVLB_Pos)
#define TIMER4n_OUT_LVLBHIGH     (0x1uL << TIMER4n_OUTCR_LVLB_Pos)

//----------   TIMER4n T4nOUTA Output When Disable   ----------
#define TIMER4n_OUT_LVLALOW      (0x0uL << TIMER4n_OUTCR_LVLA_Pos)
#define TIMER4n_OUT_LVLAHIGH     (0x1uL << TIMER4n_OUTCR_LVLA_Pos)

//==========   TIMER4n_INTCR   ========================================

//----------   TIMER4n Output Force Level Interrupt EN/Disable Definition   ----------
#define TIMER4n_INT_FRCIDIS      (0x0uL << TIMER4n_INTCR_T4nFRCIEN_Pos)
#define TIMER4n_INT_FRCIEN       (0x1uL << TIMER4n_INTCR_T4nFRCIEN_Pos)

//----------   TIMER4n Capture Interrupt EN/Disable Definition   ----------
#define TIMER4n_INT_CIDIS        (0x0uL << TIMER4n_INTCR_T4nCIEN_Pos)
#define TIMER4n_INT_CIEN         (0x1uL << TIMER4n_INTCR_T4nCIEN_Pos)

//----------   TIMER4n Bottom Interrupt EN/Disable Definition   ----------
#define TIMER4n_INT_BTIDIS       (0x0uL << TIMER4n_INTCR_T4nBTIEN_Pos)
#define TIMER4n_INT_BTIEN        (0x1uL << TIMER4n_INTCR_T4nBTIEN_Pos)

//----------   TIMER4n Period Match Interrupt EN/Disable Definition   ----------
#define TIMER4n_INT_PMIDIS       (0x0uL << TIMER4n_INTCR_T4nPMIEN_Pos)
#define TIMER4n_INT_PMIEN        (0x1uL << TIMER4n_INTCR_T4nPMIEN_Pos)

//----------   TIMER4n B Match Interrupt EN/Disable Definition   ----------
#define TIMER4n_INT_BMIDIS       (0x0uL << TIMER4n_INTCR_T4nBMIEN_Pos)
#define TIMER4n_INT_BMIENUP      (0x1uL << TIMER4n_INTCR_T4nBMIEN_Pos)
#define TIMER4n_INT_BMIENDOWN    (0x2uL << TIMER4n_INTCR_T4nBMIEN_Pos)
#define TIMER4n_INT_BMIENBOTH    (0x3uL << TIMER4n_INTCR_T4nBMIEN_Pos)

//----------   TIMER4n A Match Interrupt EN/Disable Definition   ----------
#define TIMER4n_INT_AMIDIS       (0x0uL << TIMER4n_INTCR_T4nAMIEN_Pos)
#define TIMER4n_INT_AMIENUP      (0x1uL << TIMER4n_INTCR_T4nAMIEN_Pos)
#define TIMER4n_INT_AMIENDOWN    (0x2uL << TIMER4n_INTCR_T4nAMIEN_Pos)
#define TIMER4n_INT_AMIENBOTH    (0x3uL << TIMER4n_INTCR_T4nAMIEN_Pos)

//==========   TIMER4n_ADTCR   ========================================

//----------   TIMER4n Bottom for A/DC Trigger Signal Generator EN/Disable Definition   ----------
#define TIMER4n_ADT_BTTGDIS      (0x0uL << TIMER4n_ADTCR_T4nBTTG_Pos)
#define TIMER4n_ADT_BTTGEN       (0x1uL << TIMER4n_ADTCR_T4nBTTG_Pos)

//----------   TIMER4n Period Match for A/DC Trigger Signal Generator EN/Disable Definition   ----------
#define TIMER4n_ADT_PMTGDIS      (0x0uL << TIMER4n_ADTCR_T4nPMTG_Pos)
#define TIMER4n_ADT_PMTGEN       (0x1uL << TIMER4n_ADTCR_T4nPMTG_Pos)

//----------   TIMER4n B-ch Match for A/DC Trigger Signal Generator EN/Disable Definition   ----------
#define TIMER4n_ADT_BMTGDIS      (0x0uL << TIMER4n_ADTCR_T4nBMTG_Pos)
#define TIMER4n_ADT_BMTGENUP     (0x1uL << TIMER4n_ADTCR_T4nBMTG_Pos)
#define TIMER4n_ADT_BMTGENDOWN   (0x2uL << TIMER4n_ADTCR_T4nBMTG_Pos)
#define TIMER4n_ADT_BMTGENBOTH   (0x3uL << TIMER4n_ADTCR_T4nBMTG_Pos)

//----------   TIMER4n A-ch Match for A/DC Trigger Signal Generator EN/Disable Definition   ----------
#define TIMER4n_ADT_AMTGDIS      (0x0uL << TIMER4n_ADTCR_T4nAMTG_Pos)
#define TIMER4n_ADT_AMTGENUP     (0x1uL << TIMER4n_ADTCR_T4nAMTG_Pos)
#define TIMER4n_ADT_AMTGENDOWN   (0x2uL << TIMER4n_ADTCR_T4nAMTG_Pos)
#define TIMER4n_ADT_AMTGENBOTH   (0x3uL << TIMER4n_ADTCR_T4nAMTG_Pos)

#define TIMER4n_PRS_MASK         0x0FFF

//******************************************************************************
// Type
//******************************************************************************

//==============================================================================
// Enumeration
//==============================================================================

/**   counter clock source select */

typedef enum
{
   TIMER4n_PCLK = 0,             /**<  clock source from pclk */
   TIMER4n_ECn = 1               /**<  clock source from ECn pin input. before setting, have to set ECn pin mode */
} TIMER4n_CKSEL_MODE_OPT;

typedef enum
{
   TIMER4n_PERIODIC_MODE = 0,    /**<  PERIODIC mode */
   TIMER4n_CAPTURE_MODE,         /**<  CAPTURE mode */
   TIMER4n_BACKTOBACK_MODE,         /**<  BACK TO BACK mode */
   TIMER4n_ONESHOT_MODE              /**<  ONE SHOT mode */
} TIMER4n_MODE_OPT;

typedef enum
{
   TIMER4n_FALLING_EGDE = 0,     /**<  falling edge clear mode */
   TIMER4n_RISING_EGDE,          /**<  rising edge clear mode */
   TIMER4n_BOTH_EGDE,            /**<  both edge clear  mode */
   TIMER4n_NONE                  /**<  none clear mode */
} TIMER4n_CLR_MODE_OPT;

/**   start default level select: initial output value. */
typedef enum
{
   TIMER4n_OUTB_START_LOW = 0,
   TIMER4n_OUTB_START_HIGH
} TIMER4n_OUTB_STARTLVL_OPT;

typedef enum
{
   TIMER4n_OUTA_START_LOW = 0,
   TIMER4n_OUTA_START_HIGH
} TIMER4n_OUTA_STARTLVL_OPT;

typedef enum
{
   TIMER4n_OUTB_DSIABLE = 0,
   TIMER4n_OUTB_ENABLE
} TIMER4n_OUTB_ENABLE_OPT;

typedef enum
{
   TIMER4n_OUTA_DSIABLE = 0,
   TIMER4n_OUTA_ENABLE
} TIMER4n_OUTA_ENABLE_OPT;

/**   TIMER Interrupt Type definitions */
typedef enum
{
   TIMER4n_INTCFG_PMIE = 0,	/**<  Period Match Interrupt enable*/
   TIMER4n_INTCFG_CIE,         /**<  Capture Interrupt enable*/
} TIMER4n_INT_Type;

//==============================================================================
// Structure
//==============================================================================

/**   Configuration structure in TIMER mode */
typedef struct
{
   uint16_t	   PDR;
   uint16_t    ADR;
   uint16_t    BDR;
   uint16_t    Prescaler;
   uint8_t     OutBStartLevel;    /**<  set initial outputB value
                                    -  TIMER4n_OUTB_START_LOW, TIMER4n_OUTB_START_HIGH		  */

   uint8_t	   OutAStartLevel;	 /**<  set initial outputA value
								    -  TIMER4n_OUTA_START_LOW, TIMER4n_OUTA_START_HIGH		   */

   uint8_t     OutBEnable;    /**<  set initial outputB value */
   uint8_t     OutAEnable;    /**<  set initial outputA value */

   uint8_t     CkSel;         /**<  Counter clock source select
                                    -  PCLK, ECn
                              */
   uint8_t     ECE;
} TIMER4n_PERIODICCFG_Type, TIMER4n_ONESHOTCFG_Type;

/**   Capture Input configuration structure */
typedef struct
{
   uint16_t    PDR;
   uint16_t    Prescaler;
   uint8_t     ClrMode;
   uint8_t     CkSel;
   uint8_t     ECE;
} TIMER4n_CAPTURECFG_Type;

//******************************************************************************
// Macro
//******************************************************************************

/*-------------------------------------------------------------------------*//**
 * @brief         TIMER4n Enable/Disable
 * @param[in]     TIMER4x
 *                   -  TIMER40 ~ TIMER43
 * @details       This macro Enable TIMER4n Block
 *//*-------------------------------------------------------------------------*/
#define TIMER4n_EN( TIMER4x )        (TIMER4x->CR_b.T4nEN = 1)
#define TIMER4n_DIS( TIMER4x )       (TIMER4x->CR_b.T4nEN = 0)

/*-------------------------------------------------------------------------*//**
 * @brief         TIMER4n Continue Counting/Temporary Pause
 * @param[in]     TIMER4x
 *                   -  TIMER40 ~ TIMER43
 * @details       This macro select TIMER4n Continue Counting/Temporary Pause
 *//*-------------------------------------------------------------------------*/
#define TIMER4n_ConCnt( TIMER4x )       (TIMER4x->CR_b.T4nPAU = 0)
#define TIMER4n_TempPau( TIMER4x )      (TIMER4x->CR_b.T4nPAU = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         TIMER4n Counter and Prescaler clear
 * @param[in]     TIMER4x
 *                   -  TIMER40 ~ TIMER43
 * @details       This macro clears Counter and Prescalrer of TIMER4n
 *//*-------------------------------------------------------------------------*/
#define TIMER4n_ClrCnt( TIMER4x )       (TIMER4x->CR_b.T4nCLR = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         Get TIMER4n Counter Register
 * @param[in]     TIMER4x
 *                   -  TIMER40 ~ TIMER43
 * @details       This macro gets TIMER4n Counter Register
 *//*-------------------------------------------------------------------------*/
#define TIMER4n_GetCnt( TIMER4x )       (TIMER4x->CNT)

/*-------------------------------------------------------------------------*//**
 * @brief         Set TIMER4n Period Data Register
 * @param[in]     TIMER4x
 *                   -  TIMER40 ~ TIMER43
 * @param[in]     u32PData
 *                   Period Data of TIMER4n
 * @details       This macro sets TIMER4n Period Data Register
 *//*-------------------------------------------------------------------------*/
#define TIMER4n_SetPData( TIMER4x, u32PData )    (TIMER4x->PDR = u32PData)

/*-------------------------------------------------------------------------*//**
 * @brief         Set TIMER4n A Data Register
 * @param[in]     TIMER4x
 *                   -  TIMER40 ~ TIMER43
 * @param[in]     u32AData
 *                   A Data of TIMER4n
 * @details       This macro sets TIMER4n A Data Register
 *//*-------------------------------------------------------------------------*/
#define TIMER4n_SetAData( TIMER4x, u32AData )    (TIMER4x->ADR = u32AData)

/*-------------------------------------------------------------------------*//**
 * @brief         Set TIMER4n B Data Register
 * @param[in]     TIMER4x
 *                   -  TIMER40 ~ TIMER43
 * @param[in]     u32BData
 *                   B Data of TIMER4n
 * @details       This macro sets TIMER4n B Data Register
 *//*-------------------------------------------------------------------------*/
#define TIMER4n_SetBData( TIMER4x, u32BData )    (TIMER4x->BDR = u32BData)

/*-------------------------------------------------------------------------*//**
 * @brief         Get TIMER4n Capture Data Register
 * @param[in]     TIMER4x
 *                   -  TIMER40 ~ TIMER43
 * @details       This macro gets TIMER4n Capture Data Register
 *//*-------------------------------------------------------------------------*/
#define TIMER4n_GetCapData( TIMER4x )      (TIMER4x->CAPDR)

/*-------------------------------------------------------------------------*//**
 * @brief         Set TIMER4n Prescaler Data Register
 * @param[in]     TIMER4x
 *                   -  TIMER40 ~ TIMER43
 * @param[in]     u32PresData
 *                   Prescaler Data of TIMER4n
 * @details       This macro sets TIMER4n Prescaler Data Register
 *//*-------------------------------------------------------------------------*/
#define TIMER4n_SetPresData( TIMER4x, u32PresData )    (TIMER4x->PREDR = u32PresData)

/*-------------------------------------------------------------------------*//**
 * @brief         Set TIMER4n Output Delay Data Register
 * @param[in]     TIMER4x
 *                   -  TIMER40 ~ TIMER43
 * @param[in]     u32DelayData
 *                   Output Delay Data of TIMER4n
 * @details       This macro sets TIMER4n Output Delay Data Register
 *//*-------------------------------------------------------------------------*/
#define TIMER4n_SetDelayData( TIMER4x, u32DelayData )    (TIMER4x->DLY = u32DelayData)

/*-------------------------------------------------------------------------*//**
 * @brief         Get flags of TIMER4n Output Force Level Interrupt
 * @param[in]     TIMER4x
 *                   -  TIMER40 ~ TIMER43
 * @details       This macro gets interrupt flag of TIMER4n Output Force Level Interrupt
 *//*-------------------------------------------------------------------------*/
#define T4nFRCInt_GetFg( TIMER4x )          (TIMER4x->INTFLAG_b.T4nFRCIFLAG)

/*-------------------------------------------------------------------------*//**
 * @brief         Clear flags of TIMER4n Output Force Level Interrupt
 * @param[in]     TIMER4x
 *                   -  TIMER40 ~ TIMER43
 * @details       This macro clears interrupt flag of TIMER4n Output Force Level Interrupt
 *//*-------------------------------------------------------------------------*/
#define T4nFRCInt_ClrFg( TIMER4x )          (TIMER4x->INTFLAG_b.T4nFRCIFLAG = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         Get flags of TIMER4n Capture Interrupt
 * @param[in]     TIMER4x
 *                   -  TIMER40 ~ TIMER43
 * @details       This macro gets interrupt flag of TIMER4n Capture Interrupt
 *//*-------------------------------------------------------------------------*/
#define T4nCapInt_GetFg( TIMER4x )         (TIMER4x->INTFLAG_b.T4nCIFLAG)

/*-------------------------------------------------------------------------*//**
 * @brief         Clear flags of TIMER4n Capture Interrupt
 * @param[in]     TIMER4x
 *                   -  TIMER40 ~ TIMER43
 * @details       This macro clears interrupt flag of TIMER4n Capture Interrupt
 *//*-------------------------------------------------------------------------*/
#define T4nCapInt_ClrFg( TIMER4x )         (TIMER4x->INTFLAG_b.T4nCIFLAG = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         Get flags of TIMER4n Bottom Interrupt
 * @param[in]     TIMER4x
 *                   -  TIMER40 ~ TIMER43
 * @details       This macro gets interrupt flag of TIMER4n Bottom Interrupt
 *//*-------------------------------------------------------------------------*/
#define T4nBTInt_GetFg( TIMER4x )          (TIMER4x->INTFLAG_b.T4nBTIFLAG)

/*-------------------------------------------------------------------------*//**
 * @brief         Clear flags of TIMER4n Bottom Interrupt
 * @param[in]     TIMER4x
 *                   -  TIMER40 ~ TIMER43
 * @details       This macro clears interrupt flag of TIMER4n Bottom Interrupt
 *//*-------------------------------------------------------------------------*/
#define T4nBTInt_ClrFg( TIMER4x )          (TIMER4x->INTFLAG_b.T4nBTIFLAG = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         Get flags of TIMER4n Period Match Interrupt
 * @param[in]     TIMER4x
 *                   -  TIMER40 ~ TIMER43
 * @details       This macro gets interrupt flag of TIMER4n Period Match Interrupt
 *//*-------------------------------------------------------------------------*/
#define T4nPMInt_GetFg( TIMER4x )          (TIMER4x->INTFLAG_b.T4nPMIFLAG)

/*-------------------------------------------------------------------------*//**
 * @brief         Clear flags of TIMER4n Period Match Interrupt
 * @param[in]     TIMER4x
 *                   -  TIMER40 ~ TIMER43
 * @details       This macro clears interrupt flag of TIMER4n Period Match Interrupt
 *//*-------------------------------------------------------------------------*/
#define T4nPMInt_ClrFg( TIMER4x )          (TIMER4x->INTFLAG_b.T4nPMIFLAG = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         Get flags of TIMER4n B Match Interrupt
 * @param[in]     TIMER4x
 *                   -  TIMER40 ~ TIMER43
 * @details       This macro gets interrupt flag of TIMER4n B Match Interrupt
 *//*-------------------------------------------------------------------------*/
#define T4nBMInt_GetFg( TIMER4x )          (TIMER4x->INTFLAG_b.T4nBMIFLAG)

/*-------------------------------------------------------------------------*//**
 * @brief         Clear flags of TIMER4n B Match Interrupt
 * @param[in]     TIMER4x
 *                   -  TIMER40 ~ TIMER43
 * @details       This macro clears interrupt flag of TIMER4n B Match Interrupt
 *//*-------------------------------------------------------------------------*/
#define T4nBMInt_ClrFg( TIMER4x )          (TIMER4x->INTFLAG_b.T4nBMIFLAG = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         Get flags of TIMER4n A Match Interrupt
 * @param[in]     TIMER4x
 *                   -  TIMER40 ~ TIMER43
 * @details       This macro gets interrupt flag of TIMER4n A Match Interrupt
 *//*-------------------------------------------------------------------------*/
#define T4nAMInt_GetFg( TIMER4x )          (TIMER4x->INTFLAG_b.T4nAMIFLAG)

/*-------------------------------------------------------------------------*//**
 * @brief         Clear flags of TIMER4n A Match Interrupt
 * @param[in]     TIMER4x
 *                   -  TIMER40 ~ TIMER43
 * @details       This macro clears interrupt flag of TIMER4n A Match Interrupt
 *//*-------------------------------------------------------------------------*/
#define T4nAMInt_ClrFg( TIMER4x )          (TIMER4x->INTFLAG_b.T4nAMIFLAG = 1)

/*-------------------------------------------------------------------------*//**
 * @brief         Timer device enumeration
 *//*-------------------------------------------------------------------------*/

#define TIMER4n_CR_CKSEL_MASK          (TIMER4n_CR_T4nCLK_Msk)
#define TIMER4n_CR_CKSEL_SET( n )      (n << TIMER4n_CR_T4nCLK_Pos)

#define TIMER4n_CR_MODE_MASK           (TIMER4n_CR_T4nMS_Msk)
#define TIMER4n_CR_MODE_SET( n )       (n << TIMER4n_CR_T4nMS_Pos)

#define TIMER4n_CR_INPOL_MASK          (TIMER4n_CR_T4nINPOL_Msk)
#define TIMER4n_CR_INPOL_SET( n )      (n << TIMER4n_CR_T4nINPOL_Pos)

#define TIMER4n_CR_ECE_MASK            (TIMER4n_CR_T4nECE_Msk)
#define TIMER4n_CR_ECE_SET( n )        (n << TIMER4n_CR_T4nECE_Pos)

#define TIMER4n_OUTCR_POLB_MASK        (TIMER4n_OUTCR_POLB_Msk)
#define TIMER4n_OUTCR_POLB_SET( n )    (n << TIMER4n_OUTCR_POLB_Pos)

#define TIMER4n_OUTCR_POLA_MASK        (TIMER4n_OUTCR_POLA_Msk)
#define TIMER4n_OUTCR_POLA_SET( n )    (n << TIMER4n_OUTCR_POLA_Pos)

#define TIMER4n_OUTCR_BOE_MASK         (TIMER4n_OUTCR_T4nBOE_Msk)
#define TIMER4n_OUTCR_BOE_SET( n )     (n << TIMER4n_OUTCR_T4nBOE_Pos)

#define TIMER4n_OUTCR_AOE_MASK         (TIMER4n_OUTCR_T4nAOE_Msk)
#define TIMER4n_OUTCR_AOE_SET( n )     (n << TIMER4n_OUTCR_T4nAOE_Pos)

#define TIMER4n_OUTCR_LVLB_MASK        (TIMER4n_OUTCR_LVLB_Msk)
#define TIMER4n_OUTCR_LVLB_SET( n )    (n << TIMER4n_OUTCR_LVLB_Pos)

#define TIMER4n_OUTCR_LVLA_MASK        (TIMER4n_OUTCR_LVLA_Msk)
#define TIMER4n_OUTCR_LVLA_SET( n )    (n << TIMER4n_OUTCR_LVLA_Pos)

//******************************************************************************
// Function
//******************************************************************************

HAL_Status_Type HAL_TIMER4n_Init( TIMER4n_Type* TIMER4x, TIMER4n_MODE_OPT TimerCounterMode, void* TIMER4n_Config );
HAL_Status_Type HAL_TIMER4n_DeInit( TIMER4n_Type* TIMER4x );

HAL_Status_Type HAL_TIMER4n_ConfigInterrupt( TIMER4n_Type* TIMER4x, TIMER4n_INT_Type TIMER4n_IntCfg, FunctionalState NewState );
HAL_Status_Type HAL_TIMER4n_Cmd( TIMER4n_Type* TIMER4x, FunctionalState NewState );
HAL_Status_Type HAL_TIMER4n_SetRegister( TIMER4n_Type* TIMER4x, uint32_t u32T4nSet, uint32_t u32T4nClk );

#ifdef __cplusplus
}
#endif

#endif   /* _TIMER4n_H_ */


/*
****************************************************************************
PROJECT : BSP - GFX - CSI display init
FILE    : $Id: r_bsp_gfx_disp_init.c 7327 2016-01-22 10:29:57Z matthias.nippert $
============================================================================
DESCRIPTION
BSP Implementation D1X MANGO Board
Init NEC display NL2432HC22-41K via CSI sequence.
============================================================================
                            C O P Y R I G H T
============================================================================
                           Copyright (c) 2014
                                  by
                    Renesas Electronics (Europe) GmbH. 
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: only for testing

DISCLAIMER                                                                   
This software is supplied by Renesas Electronics Corporation and is only     
intended for use with Renesas products. No other uses are authorized. This   
software is owned by Renesas Electronics Corporation and is protected under  
all applicable laws, including copyright laws.                               
THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING  
THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT      
LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE   
AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.          
TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS       
ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE  
FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR   
ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE  
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.                             
Renesas reserves the right, without notice, to make changes to this software 
and to discontinue the availability of this software. By using this software,
you agree to the additional terms and conditions found by accessing the      
following link:                                                              
http://www.renesas.com/disclaimer *                                          
Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.     

****************************************************************************
*/

#if 1    /* use CSISW (not CSIH) */
/*******************************************************************************
  Section: Includes
*/
#include "r_typedefs.h"     /* Renesas basic types, e.g. uint32_t */
#include "r_dev_api.h"
#include "r_gpio_api.h"
#include "r_tick_api.h"
#include "r_bsp_gfx_disp_init.h"    /* CSI init for NEC display */

#include "r_csisw_api.h"


/*******************************************************************************
  Section: Local Defines
*/

/*  CSI pins used for the displays
    display NL2432HC22-41K (order code: Y-EB-EXT-QVGA-PT)
    
    CSI display connector on D1X MANGO (CN48)
    VOnSCK      - P1_2   CN48_3  -> Software SPI (CSISW driver)
    VOnSO       - P1_0   CN48_1  -> Software SPI (CSISW driver)
    VOnCS       - P1_4   CN48_7 (emulated by GPIO)
    VOnRst      - P1_5   CN48_9 (emulated by GPIO)
    VOnLighEnab - P1_3   CN48_6  (emulated by GPIO, hold HI-Z means enabled)
    
    Connect the CSI connector (CN48) to the display NL2432HC22-41K
    (Y-EB-EXT-QVGA-PT QVGA display board).
*/

#define CSISW_UNIT                 0

#define CSISW_GPIO_RESETZ_PORT      1
#define CSISW_GPIO_RESETZ_PIN       5

#define CSISW_GPIO_CHIPSELZ_PORT    1
#define CSISW_GPIO_CHIPSELZ_PIN     3

#define CSISW_GPIO_LIGHTEN_PORT     42
#define CSISW_GPIO_LIGHTEN_PIN      7

/*******************************************************************************
  Section: Local Constants
*/

static const r_dev_PinConfig_t loc_CSISWPinConfig[] = 
/*  Port Pin Func      Dir      Feedback     Pull        OpenDrain   HiCurr   HiSpeed  InputType */
{
    /* GPIO reset, chip select, light enable (1st connector) */
    {CSISW_GPIO_RESETZ_PORT,   CSISW_GPIO_RESETZ_PIN,   0u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},  
    {CSISW_GPIO_CHIPSELZ_PORT, CSISW_GPIO_CHIPSELZ_PIN, 0u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},  
    {CSISW_GPIO_LIGHTEN_PORT,  CSISW_GPIO_LIGHTEN_PIN,  0u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},
	{CSISW_GPIO_RESETZ_PORT,   0,                       0u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},  
    {CSISW_GPIO_CHIPSELZ_PORT, 1,                       0u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},  
    {CSISW_GPIO_LIGHTEN_PORT,  2,                       0u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, 
    
    /* delimiter - do not remove */
    {0u,  R_DEV_PIN_LAST,  0u,  R_DEV_PIN_OUT,  0u,  R_DEV_PIN_PULLNO,  0u,  0u,  0u,  R_DEV_PIN_CMOS1}  
};


/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_BSP_GFX_CSI_NecDisplay_Init

  Init NEC display NL2432HC22-41K via CSI.

  Parameters:
  void

  Returns:
  void
*/
void R_BSP_GFX_CSI_NecDisplay_Init(void) {
    /* Initialization of the NEC display NL2432HC22-41K via CSI needs to be done. */
    
    /* Assign Pins */
    /* SHARED FOR CSI CONNECTOR 1 + 2 */
    /* CSISWSC (clock) VOnSCK on port CSISW_CLOCK_SC_PORT.CSISW_CLOCK_SC_PIN */
    /* CSISWSO (data out)   VOnSO on port CSISW_DOUT_SO_PORT.CSISW_DOUT_SO_PIN */
    /* FOR CSI CONNECTOR 1 */
    /* set port pin for reset for CSISW via GPIO
       first make sure the pin is set to inactive-level
       VOnRst on port CSISW_GPIO_RESETZ_PORT.CSISW_GPIO_RESETZ_PIN */
    /* set port pin for chip select for CSISW via GPIO
       first make sure the pin is set to inactive-level
       VOnCS on port pin CSISW_GPIO_CHIPSELZ_PORT.CSISW_GPIO_CHIPSELZ_PIN */
    R_DEV_PinInit (loc_CSISWPinConfig);
    R_GPIO_WritePin(CSISW_GPIO_RESETZ_PORT, CSISW_GPIO_RESETZ_PIN, 1);
	// R_GPIO_WritePin(CSISW_GPIO_CHIPSELZ_PORT, CSISW_GPIO_CHIPSELZ_PIN, 1);
	// R_GPIO_WritePin(1, 0, 0);
    // R_GPIO_WritePin(1, 1, 1);
	// R_GPIO_WritePin(1, 2, 0);
    R_GPIO_WritePin(42, 7, 1);
    
    /* Init CSISW */
    /*  Set master config */
	// R_CSISW_Init(CSISW_UNIT);

    R_CSISW_Enable(CSISW_UNIT, R_CSISW_TX_ONLY);       /* only TX direction */

    /* reset display
        assert reset, wait 2ms
        reset, wait 2ms after reset deassertion
    */
    R_GPIO_WritePin(CSISW_GPIO_RESETZ_PORT, CSISW_GPIO_RESETZ_PIN, 0);
    R_TICK_WaitMS(0u, 2u);
    R_GPIO_WritePin(CSISW_GPIO_RESETZ_PORT, CSISW_GPIO_RESETZ_PIN, 1);
    R_TICK_WaitMS(0u, 2u);
    
    /* activate (low active) chip select */
    R_GPIO_WritePin(CSISW_GPIO_CHIPSELZ_PORT, CSISW_GPIO_CHIPSELZ_PIN, 0);
    R_TICK_WaitMS(0u, 2u);
    
    /* transmit display de-init sequence via CSI */
    /* Note: the de-init is important, because without deinit first,
       the display stays off after Reset without power off */
#if 0
	/* Initialization of the NEC display
       NL2432HC22-41K via CSI needs to be done. */
    uint32_t            tmp;
    
    /* power on sequence for NL2432HC22-41K display */
        /* write reg, register number, write data, data value */
    static const uint8_t SendDispOn[] = {
               0x00u,   3u, 0x01u, 0x01u,
               0x00u,   1u, 0x01u, 0x00u,
               0x00u, 100u, 0x01u, 0x0Fu,   /* example: write data 0x0F to register R100 */
               0x00u, 101u, 0x01u, 0x3Fu,
               0x00u, 102u, 0x01u, 0x3Fu,
               0x00u, 103u, 0x01u, 0x00u,
               0x00u, 104u, 0x01u, 0x00u,
               0x00u, 105u, 0x01u, 0x30u,
               0x00u, 106u, 0x01u, 0x04u,
               0x00u, 107u, 0x01u, 0x37u,
               0x00u, 108u, 0x01u, 0x17u,
               0x00u, 109u, 0x01u, 0x00u,
               0x00u, 110u, 0x01u, 0x40u,
               0x00u, 111u, 0x01u, 0x30u,
               0x00u, 112u, 0x01u, 0x04u,
               0x00u, 113u, 0x01u, 0x37u,
               0x00u, 114u, 0x01u, 0x17u,
               0x00u, 115u, 0x01u, 0x00u,
               0x00u, 116u, 0x01u, 0x40u,
               0x00u,   2u, 0x01u, 0x40u,
               0x00u,  75u, 0x01u, 0x04u,
               0x00u,  76u, 0x01u, 0x01u,
               0x00u,  77u, 0x01u, 0x01u,
               0x00u,  80u, 0x01u, 0x00u,
               0x00u,  81u, 0x01u, 0x00u,
               0x00u,  82u, 0x01u, 0x2Eu,
               0x00u,  83u, 0x01u, 0xC4u,
               0x00u,  86u, 0x01u, 0x15u,
               0x00u,  87u, 0x01u, 0xEDu,
               0x00u,  95u, 0x01u, 0x3Fu,
               0x00u,  96u, 0x01u, 0x22u,
               0x00u,  25u, 0x01u, 0x76u,
               0x00u,  26u, 0x01u, 0x54u,
               0x00u,  27u, 0x01u, 0x67u,
               0x00u,  28u, 0x01u, 0x60u,
               0x00u,  29u, 0x01u, 0x04u,
               0x00u,  30u, 0x01u, 0x1Cu,
               0x00u,  31u, 0x01u, 0xA9u,
               0x00u,  32u, 0x01u, 0x00u,
               0x00u,  33u, 0x01u, 0x20u,
               0x00u,  24u, 0x01u, 0x77u,
               0x00u,  59u, 0x01u, 0x01u,
               0x00u,   0u, 0x01u, 0x00u};
    
    /* power off sequence for NL2432HC22-41K display */
              /* write, register number, write, data value */
    static const uint8_t SendDispOff[] = {
               0x00u,   0u, 0x01u, 0x08u,
               0x00u,  24u, 0x01u, 0x00u,
               0x00u,   1u, 0x01u, 0x08u};
	
    for (tmp = 0u; tmp < sizeof(SendDispOff); tmp++)
	{
        if ((tmp % 4) == 0u) 
        {
            R_TICK_WaitMS(0u, 25u);
        }
        R_CSISW_Write(CSISW_UNIT, (void*)(&SendDispOff[tmp]), 1u);
    }
    
    /* transmit display init sequence via CSI */

    for (tmp = 0u; tmp < sizeof(SendDispOn); tmp++)
	{
        if ((tmp == (sizeof(SendDispOn)-4)) || (tmp == (sizeof(SendDispOn)-8)))
		{
            R_TICK_WaitMS(0u, 20u);
        }
        R_CSISW_Write(CSISW_UNIT, (void*)(&SendDispOn[tmp]), 1u);
    }
#endif
    /* de-activate (low active) chip select */
    R_TICK_WaitMS(0u, 2u);
	// R_GPIO_WritePin(CSISW_GPIO_CHIPSELZ_PORT, CSISW_GPIO_CHIPSELZ_PIN, 1);
    
    R_CSISW_DeInit(CSISW_UNIT);
    
    /* enable the backlight */
    //R_GPIO_WritePin(CSISW_GPIO_LIGHTEN_PORT, CSISW_GPIO_LIGHTEN_PIN, 1);
}

#else   /* use CSIH (not CSISW) */

/*******************************************************************************
  Section: Includes
*/
#include "r_typedefs.h"     /* Renesas basic types, e.g. uint32_t */
#include "r_dev_api.h"
#include "r_gpio_api.h"
#include "r_tick_api.h"
#include "r_csih_api.h"
#include "r_bsp_gfx_disp_init.h"    /* CSI init for NEC display */


/*******************************************************************************
  Section: Local Defines
*/

/*  CSI pins used for the displays
    display NL2432HC22-41K (order code: Y-EB-EXT-QVGA-PT)
    
    CSI display connector on D1X MANGO (CN48)
    VOnSCK      - P1_2   CN48_3
    VOnSO       - P1_0   CN48_1
    VOnCS       - P1_4   CN48_7 (emulated by GPIO)
    VOnRst      - P1_5   CN48_9 (emulated by GPIO)
    VOnLighEnab - P1_3   CN48_6  (emulated by GPIO, hold HI-Z means enabled)
    
    Connect the CSI connector (CN48) to the display NL2432HC22-41K
    (Y-EB-EXT-QVGA-PT QVGA display board).
*/

#define CSIH_DOUT_SO_PORT          1
#define CSIH_DOUT_SO_PIN           0

#define CSIH_CLOCK_SC_PORT         1
#define CSIH_CLOCK_SC_PIN          2

#define CSIH_UNIT                  1
#define CSIH_CLOCK_DOMAIN          (R_DEV_CKS_CSIH)

#define CSIH_GPIO_CHIPSELZ_PORT    1
#define CSIH_GPIO_CHIPSELZ_PIN     4

#define CSIH_GPIO_RESETZ_PORT      1
#define CSIH_GPIO_RESETZ_PIN       5

#define CSIH_GPIO_LIGHTEN_PORT     1
#define CSIH_GPIO_LIGHTEN_PIN      3

/*******************************************************************************
  Section: Local Constants
*/

static const r_dev_PinConfig_t loc_DISPPinConfig[] = 
/*  Port Pin Func      Dir      Feedback     Pull        OpenDrain   HiCurr   HiSpeed  InputType */
{
    /* CSIH unit 1; clock and data pin (shared) */
    {CSIH_CLOCK_SC_PORT, CSIH_CLOCK_SC_PIN, 2u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},  
    {CSIH_DOUT_SO_PORT,  CSIH_DOUT_SO_PIN,  2u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},  
    
    /* GPIO for CSI; reset, chip select, light enable (1st connector) */
    {CSIH_GPIO_RESETZ_PORT,     CSIH_GPIO_RESETZ_PIN,  0u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},  
    {CSIH_GPIO_CHIPSELZ_PORT,   CSIH_GPIO_CHIPSELZ_PIN,  0u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},  
    {CSIH_GPIO_LIGHTEN_PORT,   CSIH_GPIO_LIGHTEN_PIN,  0u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},  
    
    /* delimiter - do not remove */
    {0u, R_DEV_PIN_LAST,0u,R_DEV_PIN_OUT,0u,R_DEV_PIN_PULLNO,     0u,      0u,         0u,    R_DEV_PIN_CMOS1}  
};

/*******************************************************************************
  Section: Local Functions
*/
void loc_CsiHConfig ( r_csih_InitCfg_t*    Cfg,
                      r_csih_ExtFunc_t*    Extf,
                      r_csih_ChipSelCfg_t* CsCfg,
                      uint32_t Bdr);


void loc_CsiHConfig ( r_csih_InitCfg_t*    Cfg,
                      r_csih_ExtFunc_t*    Extf,
                      r_csih_ChipSelCfg_t* CsCfg,
                      uint32_t Bdr)
{
    uint8_t i;
    r_csih_ChipSelCfg_t* cfg;

    Cfg->Mode        = R_CSIH_MASTER;	/* master mode */
    Cfg->MemCfg      = R_CSIH_MODE_DUAL_BUF;
    Cfg->NbCs        = 1;
    Cfg->ChipSelCfg  = CsCfg;

    Extf->JobMode     = 0;
    Extf->ExtDatLengE = 0;
    Extf->DataConsCheckE = 0;
    Extf->CsRetInact  = 1;
    Extf->TicNormal   = 0;
    Extf->IntDelayedE = 0;
    Extf->HandShakeE  = 0;
    Extf->SlaveSelE   = 0;
    Extf->TimeOut     = 0;

    for(i=0; i<8; i++) {
        cfg = &CsCfg[i];

        cfg->Chipselect    = i;
        cfg->DataLength    = 8;	/* 1byte = 8bit */
        cfg->Parity        = (r_csih_Parity_t)(i&0x03u);
        cfg->Baudrate      = Bdr;
        cfg->RecessiveCfg  = 0;
        cfg->Direction     = R_CSIH_MSB;	/* MSB first */
        cfg->ClockPhase    = R_CSIH_CLK_ACTIVE_HIGH;
        cfg->DataPhase     = R_CSIH_DATA_PHASE_RISE;
        cfg->ForceIdle     = 0;
        cfg->IdleTime      = 200;
        cfg->HoldTime      = 200;
        cfg->SetupTime     = 200;
        cfg->InterDataTime = 500;
    }
}

/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_BSP_GFX_CSI_NecDisplay_Init

  Init NEC display NL2432HC22-41K via CSI.

  Parameters:
  void

  Returns:
  void
*/
void R_BSP_GFX_CSI_NecDisplay_Init(void) {
    /* Initialization of the NEC display
       NL2432HC22-41K via CSI needs to be done. */
    
    r_csih_FrameS_t     frameT;
    r_csih_InitCfg_t    mcfg;
    r_csih_ExtFunc_t    extf;
    r_csih_ChipSelCfg_t cscfg[8];
    
    uint32_t            tmp;
    
    /* power on sequence for NL2432HC22-41K display */
        /* write reg, register number, write data, data value */
    static uint16_t SendDispOn[] = {
               0x00u,   3u, 0x01u, 0x01u,
               0x00u,   1u, 0x01u, 0x00u,
               0x00u, 100u, 0x01u, 0x0Fu,   /* example: write data 0x0F to register R100 */
               0x00u, 101u, 0x01u, 0x3Fu,
               0x00u, 102u, 0x01u, 0x3Fu,
               0x00u, 103u, 0x01u, 0x00u,
               0x00u, 104u, 0x01u, 0x00u,
               0x00u, 105u, 0x01u, 0x30u,
               0x00u, 106u, 0x01u, 0x04u,
               0x00u, 107u, 0x01u, 0x37u,
               0x00u, 108u, 0x01u, 0x17u,
               0x00u, 109u, 0x01u, 0x00u,
               0x00u, 110u, 0x01u, 0x40u,
               0x00u, 111u, 0x01u, 0x30u,
               0x00u, 112u, 0x01u, 0x04u,
               0x00u, 113u, 0x01u, 0x37u,
               0x00u, 114u, 0x01u, 0x17u,
               0x00u, 115u, 0x01u, 0x00u,
               0x00u, 116u, 0x01u, 0x40u,
               0x00u,   2u, 0x01u, 0x40u,
               0x00u,  75u, 0x01u, 0x04u,
               0x00u,  76u, 0x01u, 0x01u,
               0x00u,  77u, 0x01u, 0x01u,
               0x00u,  80u, 0x01u, 0x00u,
               0x00u,  81u, 0x01u, 0x00u,
               0x00u,  82u, 0x01u, 0x2Eu,
               0x00u,  83u, 0x01u, 0xC4u,
               0x00u,  86u, 0x01u, 0x15u,
               0x00u,  87u, 0x01u, 0xEDu,
               0x00u,  95u, 0x01u, 0x3Fu,
               0x00u,  96u, 0x01u, 0x22u,
               0x00u,  25u, 0x01u, 0x76u,
               0x00u,  26u, 0x01u, 0x54u,
               0x00u,  27u, 0x01u, 0x67u,
               0x00u,  28u, 0x01u, 0x60u,
               0x00u,  29u, 0x01u, 0x04u,
               0x00u,  30u, 0x01u, 0x1Cu,
               0x00u,  31u, 0x01u, 0xA9u,
               0x00u,  32u, 0x01u, 0x00u,
               0x00u,  33u, 0x01u, 0x20u,
               0x00u,  24u, 0x01u, 0x77u,
               0x00u,  59u, 0x01u, 0x01u,
               0x00u,   0u, 0x01u, 0x00u};
    
    /* power off sequence for NL2432HC22-41K display */
              /* write, register number, write, data value */
    static uint16_t SendDispOff[] = {
               0x00u,   0u, 0x01u, 0x08u,
               0x00u,  24u, 0x01u, 0x00u,
               0x00u,   1u, 0x01u, 0x08u};
    
    /* Assign Pins */
    /* SHARED FOR CSI CONNECTOR 1 + 2 */
    /*  CSIHSC (clock) VOnSCK on port CSIH_CLOCK_SC_PORT.CSIH_CLOCK_SC_PIN */
    /*  CSIHSO (data out)   VOnSO on port CSIH_DOUT_SO_PORT.CSIH_DOUT_SO_PIN */
    /* FOR CSI CONNECTOR 1 */
    /*  set port pin for reset for CSIH via GPIO
        first make sure the pin is set to inactive-level
        VOnRst on port CSIH_GPIO_RESETZ_PORT.CSIH_GPIO_RESETZ_PIN */
    /*  set port pin for chip select for CSIH via GPIO
        first make sure the pin is set to inactive-level
        VOnCS on port pin CSIH_GPIO_CHIPSELZ_PORT.CSIH_GPIO_CHIPSELZ_PIN */
    R_DEV_PinInit (loc_DISPPinConfig);
    R_GPIO_WritePin(CSIH_GPIO_RESETZ_PORT, CSIH_GPIO_RESETZ_PIN, 1);
    R_GPIO_WritePin(CSIH_GPIO_CHIPSELZ_PORT, CSIH_GPIO_CHIPSELZ_PIN, 1);
    
    /* Init CSIH */
    /*  Set master config and baudrate (clock divider 16 = 500k baud) */
	loc_CsiHConfig(&mcfg, &extf, cscfg, (R_DEV_ClkFrequencyHz(CSIH_CLOCK_DOMAIN) / 16));
	/* printf("\nBaud rate is: %d \n", mcfg.Baudrate); */
    
   
    R_CSIH_Init(CSIH_UNIT, &mcfg, &extf);
    #if 0
        R_CSIH_SetIsrCallback(CSIH_UNIT, R_CSIH_INT_TIRE, _unusedCsiHIsr);
        R_CSIH_SetIsrCallback(CSIH_UNIT, R_CSIH_INT_TIR,  _unusedCsiHIsr);
        R_CSIH_SetIsrCallback(CSIH_UNIT, R_CSIH_INT_TIC,  _unusedCsiHIsr);
        R_CSIH_SetIsrCallback(CSIH_UNIT, R_CSIH_INT_TIJC, _unusedCsiHIsr);
    #endif 
    R_CSIH_Enable(CSIH_UNIT, R_CSIH_TX_ONLY);          /* only TX direction */
    
    /* reset display
        assert reset, wait 2ms
        reset, wait 2ms after reset deassertion
    */
    R_GPIO_WritePin(CSIH_GPIO_RESETZ_PORT, CSIH_GPIO_RESETZ_PIN, 0);
    R_TICK_WaitMS(0u, 2u);
    R_GPIO_WritePin(CSIH_GPIO_RESETZ_PORT, CSIH_GPIO_RESETZ_PIN, 1);
    R_TICK_WaitMS(0u, 2u);
    
    /* activate (low active) chip select */
    R_GPIO_WritePin(CSIH_GPIO_CHIPSELZ_PORT, CSIH_GPIO_CHIPSELZ_PIN, 0);
    R_TICK_WaitMS(0u, 2u);
    
    /* transmit display de-init sequence via CSI */
    /* Note: the de-init is important, because without deinit first,
       the display stays off after Reset without power off */
    for (tmp = 0u; tmp < sizeof(SendDispOff); tmp++) {
        if ((tmp % 4) == 0u) 
        {
            R_TICK_WaitMS(0u, 25u);
        }
        frameT.IntReq     = 0;
        frameT.Eoj        = 0;
        frameT.NbData     = 1;
        frameT.CsSel.Byte = 1;
        frameT.Databitsize = 8;
        frameT.Data       = &SendDispOff[tmp];
        R_CSIH_DaSwitch(CSIH_UNIT, 1);
        R_CSIH_DaWrite(CSIH_UNIT, &frameT);
        /* wait until end of the transfer*/
        while(R_CSIH_StatusGet(CSIH_UNIT, R_CSIH_STATUS_TSF) == 1)
        {}
    }
    
    /* transmit display init sequence via CSI */
    for (tmp = 0u; tmp < sizeof(SendDispOn); tmp++) 
    {
        if ((tmp == (sizeof(SendDispOn)-4)) || (tmp == (sizeof(SendDispOn)-8))) 
        {
            R_TICK_WaitMS(0u, 20u);
        }
        frameT.IntReq     = 0;
        frameT.Eoj        = 0;
        frameT.NbData     = 1;
        frameT.CsSel.Byte = 1;
        frameT.Databitsize = 8;
        frameT.Data       = &SendDispOn[tmp];
        R_CSIH_DaSwitch(CSIH_UNIT, 1);
        R_CSIH_DaWrite(CSIH_UNIT, &frameT);
        /* wait until end of the transfer*/
        while(R_CSIH_StatusGet(CSIH_UNIT, R_CSIH_STATUS_TSF) == 1)
        {}
    }
    
    /* de-activate (low active) chip select */
    R_TICK_WaitMS(0u, 2u);
    R_GPIO_WritePin(CSIH_GPIO_CHIPSELZ_PORT, CSIH_GPIO_CHIPSELZ_PIN, 1);
    
    R_CSIH_DeInit(CSIH_UNIT);
    
    /* enable the backlight */
    R_GPIO_WritePin(CSIH_GPIO_LIGHTEN_PORT, CSIH_GPIO_LIGHTEN_PIN, 1);
}

#endif  /* use CSISW or CSIH */


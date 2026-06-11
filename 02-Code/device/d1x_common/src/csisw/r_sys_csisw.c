/*
****************************************************************************
PROJECT : VLIB macro device level driver
FILE    : $Id: r_sys_csisw.c 7180 2016-01-07 17:45:54Z tobyas.hennig $
============================================================================ 
DESCRIPTION
CSISW system functions for D1x
============================================================================
                            C O P Y R I G H T                            
============================================================================
                           Copyright (c) 2015
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


/*******************************************************************************
  Title: dev_csisw CSISW device functions

  Implementation of the CSISW support functions for the 
  device dev_csisw.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_dev_api.h"
#include "r_gpio_api.h"
#include "r_tick_api.h"
#include "r_csisw_api.h"
#include "r_csisw_sys.h"
#include "r_config_csisw.h"

#if defined (R_DBG_PRINT_DEV_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#endif



/*******************************************************************************
  Section: Local Constants
*/

/*  CSI pins used for the displays
    display NL2432HC22-41K (order code: Y-EB-EXT-QVGA-PT)
    
    CSI display connector on D1X MANGO (CN48)
    VOnSCK      - P1_2   CN48_3  -> Software SPI (CSISW driver)
    VOnSO       - P1_0   CN48_1  -> Software SPI (CSISW driver)
    VOnCS       - P1_4   CN48_7  no handled by application software (not by Software SPI CSISW driver)
    VOnRst      - P1_5   CN48_9 (emulated by GPIO)
    VOnLighEnab - P1_3   CN48_6  (emulated by GPIO, hold HI-Z means enabled)
    
    Connect the CSI connector (CN48) to the display NL2432HC22-41K
    (Y-EB-EXT-QVGA-PT QVGA display board).
*/

#define CSISW_DOUT_SO_PORT          1
#define CSISW_DOUT_SO_PIN           0

#define CSISW_DOUT_SI_PORT          1
#define CSISW_DOUT_SI_PIN           1

#define CSISW_CLOCK_SC_PORT         1
#define CSISW_CLOCK_SC_PIN          2

#define CSISW_GPIO_SS0_PORT         1
#define CSISW_GPIO_SS0_PIN          4


/*******************************************************************************
  Section: Local Constants
*/

static const r_dev_PinConfig_t loc_CsiswPinConfig[] = 
/*  Port Pin Func      Dir      Feedback     Pull        OpenDrain   HiCurr   HiSpeed  InputType */
{
    /* CSISW unit 0; clock, data tx, data rx and slave select pin */
    {CSISW_CLOCK_SC_PORT, CSISW_CLOCK_SC_PIN, 0u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, 
    {CSISW_DOUT_SO_PORT,  CSISW_DOUT_SO_PIN,  0u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, 
    {CSISW_DOUT_SI_PORT,  CSISW_DOUT_SI_PIN,  0u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, 
    {CSISW_GPIO_SS0_PORT, CSISW_GPIO_SS0_PIN, 0u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, 
    
    /* delimiter - do not remove */
    {0u, R_DEV_PIN_LAST,0u,R_DEV_PIN_OUT,0u,R_DEV_PIN_PULLNO,     0u,      0u,         0u,    R_DEV_PIN_CMOS1}  
};

/*******************************************************************************
  Section: Global Functions

*/


/*******************************************************************************
  Function: R_CSISW_SYS_HardwareInit
  
  See: r_csisw_sys.h for details
*/

r_csisw_Error_t R_CSISW_SYS_HardwareInit(uint32_t Unit) 
{
    /* make sure the TICK is already initialized, but no re-init here ;) */
    while (R_TICK_GetTimeUS(0) == 0)
    {
        /* No tick, no CSISW*/
    }
    R_DEV_PinInit(loc_CsiswPinConfig);
    return R_CSISW_ERR_OK;
}

/*******************************************************************************
  Function: R_CSISW_SYS_HardwareDeInit
  
  See: r_csisw_sys.h for details
*/

r_csisw_Error_t R_CSISW_SYS_HardwareDeInit(uint32_t Unit) 
{
    return R_CSISW_ERR_OK;
}

/*******************************************************************************
  Function: R_CSISW_SYS_SetPin

  Set decidated pin to certain level.
  
  Parameters:
  Unit       - Instance number
  PinName    - Name of port pin to set. see: <r_csisw_Pin_t>
  Level      - Level to set the pin to. see: <r_csisw_Level_t>
  
  Returns:
  see: <r_csisw_Error_t>
*/

r_csisw_Error_t R_CSISW_SYS_SetPin(uint32_t Unit,
                                    r_csisw_Pin_t PinName,
                                    r_csisw_Level_t Level)
{
    uint8_t pin_lvl = 0;
    if (Unit == 0)
    {
        if (PinName == R_CSISW_CLK_PIN)
        {   
            pin_lvl = (uint8_t)Level;   /* high active */
            R_GPIO_WritePin(CSISW_CLOCK_SC_PORT, CSISW_CLOCK_SC_PIN, pin_lvl);
        }
        if (PinName == R_CSISW_TXD_PIN)
        {
            pin_lvl = (uint8_t)Level;   /* high active */
            R_GPIO_WritePin(CSISW_DOUT_SO_PORT, CSISW_DOUT_SO_PIN, pin_lvl);
        }
        if (PinName == R_CSISW_SS0_PIN)
        {
#if 0   /* handle SS0 (chip select) by application software */
            if (Level == R_CSISW_LEVEL_ACTIVE)
            {
                pin_lvl = 0;   /* low active */
            } else {
                pin_lvl = 1;
            }
            R_GPIO_WritePin(CSISW_GPIO_SS0_PORT, CSISW_GPIO_SS0_PIN, pin_lvl);
#endif
        }
    }
    return R_CSISW_ERR_OK;
}

/*******************************************************************************
  Function: R_CSISW_SYS_GetPin

  Get level of decidated (input) pin.
  
  Parameters:
  Unit       - Instance number
  PinName    - Name of port pin to set. see: <r_csisw_Pin_t>
  
  
  Returns:
  Level      - Level to set the pin to. see: <r_csisw_Level_t>
*/

r_csisw_Level_t R_CSISW_SYS_GetPin(uint32_t Unit, r_csisw_Pin_t PinName)
{
    uint8_t pin_lvl = 0;
    if (Unit == 0)
    {
        if (PinName == R_CSISW_RXD_PIN)
        {
            pin_lvl = R_GPIO_PinRead(CSISW_DOUT_SI_PORT, CSISW_DOUT_SI_PIN);
        }
    }
    
    if (pin_lvl == 0)
    {
        return R_CSISW_LEVEL_INACTIVE;
    } else {
        return R_CSISW_LEVEL_ACTIVE;
    }
}


/*******************************************************************************
  Function: R_CSISW_SYS_Delay

  wait a short time
  
  Parameters:
  Unit       - Instance number
  TimeUs     - time to wait in us
  
  
  Returns:
  see: <r_csisw_Error_t>
*/

r_csisw_Error_t R_CSISW_SYS_Delay(uint32_t Unit, uint32_t TimeUs)
{
    R_TICK_WaitUS(0u, TimeUs);
    return R_CSISW_ERR_OK;
}



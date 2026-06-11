/*
****************************************************************************
PROJECT : VLIB Macro Driver  
FILE    : $Id: r_csisw_main.c 7181 2016-01-07 18:23:41Z tobyas.hennig $
============================================================================ 
DESCRIPTION
Driver for the CSISW macro
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

/*******************************************************************************
  Title: CSISW Main Module

  Implementation of the API and other global functions.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_config_csisw.h"
#include "r_csisw_api.h"
#define CSISW_MAIN_GLOBALS_
#include "r_csisw_main.h"
#include "r_csisw_sys.h"


/*******************************************************************************
  Section: Local Constants
*/

/*******************************************************************************
  Constant: loc_CsiswVersion

  A string containig the macro driver version information  
*/



#define LOC_CSISW_VERSIONPREFIX    "@(#)CSISW Version "
#define LOC_CSISW_VERSIONPOSTFIX   " - "


#define LOC_CSISW_VERSION      { LOC_CSISW_VERSIONPREFIX  R_XSTR (R_CSISW_VERSION_HI) "." \
                                R_XSTR(R_CSISW_VERSION_LO) LOC_CSISW_VERSIONPOSTFIX \
                                __DATE__ R_SPACE __TIME__ }

/*******************************************************************************
  Section: Local Variables
*/

/*******************************************************************************
  Variable: loc_ErrorHandler

  A function pointer to a user callback (Applications error handler)  
  
  The value is assigned by the application using the driver 
*/

static void (*loc_ErrorHandler )(uint32_t Unit, uint32_t Error) = 0u;


/*******************************************************************************
  Section: Local Functions
*/
static uint8_t loc_Csisw_MasterTransferByte(uint32_t Unit, uint8_t byte_out);
void R_CSISW_SetErrorCallback(void(*ErrorCallback )(uint32_t Unit, uint32_t Error)); 

/*******************************************************************************
  Function: loc_Csisw_MasterTransferByte
  
  Simultaneously transmit and receive a byte on the SPI.
  
  Polarity and phase are assumed to be both 0:
   o input data is captured on rising edge of SCLK
   o output data is propagated on falling edge of SCLK
   
  Parameters:
  Unit          - Macro instance number
  byte_out      - byte to be written
 
  Returns:
  byte_in       - received byte.
*/
static uint8_t loc_Csisw_MasterTransferByte(uint32_t Unit, uint8_t byte_out)
{
    uint8_t byte_in = 0;
    uint8_t bit;
    
    for (bit = 0; bit < 8; bit++)
    {
        /* Shift-out a bit to the MOSI line. */
        if (0 != (byte_out & 0x80u))
        {
            R_CSISW_SYS_SetPin(Unit, R_CSISW_TXD_PIN, R_CSISW_LEVEL_ACTIVE);
        } else {
            R_CSISW_SYS_SetPin(Unit, R_CSISW_TXD_PIN, R_CSISW_LEVEL_INACTIVE);
        }
        byte_out <<= 1;
        
        /* Delay for at least the peer's setup time. */
        /*  -> CLK low time */
        R_CSISW_SYS_Delay(Unit, ((1000000 / r_csisw_Dev[Unit].Baudrate)/2));
        
        /* Pull the clock line high. */
        R_CSISW_SYS_SetPin(Unit, R_CSISW_CLK_PIN, R_CSISW_LEVEL_ACTIVE);
 
        /* Shift-in a bit from the MISO line. */
        byte_in <<= 1;
        if (R_CSISW_SYS_GetPin(Unit, R_CSISW_RXD_PIN) == R_CSISW_LEVEL_ACTIVE)
        {
            byte_in |= 1u;
        }
        /* Delay for at least the peer's hold time. */
        /*  -> CLK high time */
        R_CSISW_SYS_Delay(Unit, ((1000000 / r_csisw_Dev[Unit].Baudrate)/2));
        
        /* Pull the clock line low. */
        R_CSISW_SYS_SetPin(Unit, R_CSISW_CLK_PIN, R_CSISW_LEVEL_INACTIVE);
    }
 
    return byte_in;
}


/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_CSISW_CheckMacroParams
  
  See: <csisw_main.h>
*/
r_csisw_Error_t R_CSISW_CheckMacroParams(uint32_t Unit)
{
    /* Feel free to add any more usefull parameter checks */
    if (R_CSISW_MACRO_NUM <= Unit)
    {
        return  R_CSISW_ERR_RANGE; 
    }
    return R_CSISW_ERR_OK;  
}

/*******************************************************************************
  Function: R_CSISW_ErrorHandler
  
  See: <csisw_main.h>
*/

void R_CSISW_ErrorHandler(uint32_t Unit, r_csisw_Error_t Error) 
{
    if (0 != loc_ErrorHandler) 
    {      /* we've got a registered error handler here ...*/
        loc_ErrorHandler(Unit, (uint32_t) Error);      /* ... and we gonna use it!*/
    } 
    else 
    {
        for(;;) 
        {        /* we don't have a handler */
        }        /* so, let's wait here */
    }
}

/*******************************************************************************
  Section: Global API Functions
  
  See: <csisw_api.h>
*/

/*******************************************************************************
  Function: R_CSISW_GetVersionStr

  See: <csisw_api.h> 
*/
const int8_t * R_CSISW_GetVersionStr(void)
{
    static const int8_t version[] = LOC_CSISW_VERSION;
    return version;
}

/*******************************************************************************
  Function: R_CSISW_GetMajorVersion

  See: <csisw_api.h> 
*/
const uint16_t R_CSISW_GetMajorVersion(void) 
{
    return (uint16_t)R_CSISW_VERSION_HI; 
}

/*******************************************************************************
  Function: R_CSISW_GetMinorVersion

  See: <csisw_api.h> 
*/
const uint16_t R_CSISW_GetMinorVersion(void) 
{
    return (uint16_t)R_CSISW_VERSION_LO; 
}



/*******************************************************************************
  Function: R_CSISW_Init

  See: <csisw_api.h> 
*/

r_csisw_Error_t R_CSISW_Init(uint32_t Unit) 
{
    r_csisw_Error_t err;
    err = R_CSISW_CheckMacroParams(Unit); 
    if (R_CSISW_ERR_OK != err) 
    {
        return err; 
    }
    if (0 == r_csisw_Dev[Unit].BaseAddr)
    {
        
        r_csisw_Dev[Unit].BaseAddr = 1;
        err = R_CSISW_SYS_HardwareInit(Unit);
        r_csisw_Dev[Unit].Baudrate = 100000;    /* fix to 100kHz */
        /* set initial (default) pin state */
        R_CSISW_SYS_SetPin(Unit, R_CSISW_CLK_PIN, R_CSISW_LEVEL_INACTIVE);
        R_CSISW_SYS_SetPin(Unit, R_CSISW_SS0_PIN, R_CSISW_LEVEL_INACTIVE);
    }
    else 
    {   
        /* please, no double-init*/
        err = R_CSISW_ERR_LOCKED; 
    }
    return err;
}


/*******************************************************************************
  Function: R_CSISW_Enable

  See: <csisw_api.h> 
*/

r_csisw_Error_t R_CSISW_Enable(uint32_t Unit, r_csisw_TxRxMode_t TxRxMode)
{
    r_csisw_Error_t err;
    err = R_CSISW_CheckMacroParams(Unit); 
    if (R_CSISW_ERR_OK != err) 
    {
        return err; 
    }
    
    /* do nothing */
    
    return err;
}


/*******************************************************************************
  Function: R_CSISW_Read

  See: <csisw_api.h> 
*/

r_csisw_Error_t R_CSISW_Read(uint32_t Unit, void *Buffer, uint32_t MaxNum)
{
    uint32_t i;
    r_csisw_Error_t err;
    err = R_CSISW_CheckMacroParams(Unit); 
    if (R_CSISW_ERR_OK != err) 
    {
        return err; 
    }
    
    for (i=0; i<MaxNum; i++)
    {
        R_CSISW_SYS_SetPin(Unit, R_CSISW_SS0_PIN, R_CSISW_LEVEL_ACTIVE);
        ((uint8_t*)(Buffer))[i] = loc_Csisw_MasterTransferByte(Unit, 0);  /* Tx value "0" is just a dummy */
        R_CSISW_SYS_SetPin(Unit, R_CSISW_SS0_PIN, R_CSISW_LEVEL_INACTIVE);
    }
    
    return err;
}


/*******************************************************************************
  Function: R_CSISW_Write

  See: <csisw_api.h> 
*/

r_csisw_Error_t R_CSISW_Write(uint32_t Unit, void *Buffer, uint32_t Num)
{
    uint32_t i;
    r_csisw_Error_t err;
    err = R_CSISW_CheckMacroParams(Unit); 
    if (R_CSISW_ERR_OK != err) 
    {
        return err; 
    }
    
    for (i=0; i<Num; i++)
    {
        R_CSISW_SYS_SetPin(Unit, R_CSISW_SS0_PIN, R_CSISW_LEVEL_ACTIVE);
        loc_Csisw_MasterTransferByte(Unit, ((uint8_t*)(Buffer))[i]);
        R_CSISW_SYS_SetPin(Unit, R_CSISW_SS0_PIN, R_CSISW_LEVEL_INACTIVE);
    }
    
    return err;
}


/*******************************************************************************
  Function: R_CSISW_DeInit

  See: <csisw_api.h> 
*/

r_csisw_Error_t R_CSISW_DeInit(uint32_t Unit)
 {
    r_csisw_Error_t err;

    err = R_CSISW_CheckMacroParams(Unit); 

    if (R_CSISW_ERR_OK  != err) 
    {
        return err; 
    }
    if (0 != r_csisw_Dev[Unit].BaseAddr)
    {
        
        r_csisw_Dev[Unit].BaseAddr = 0;
        err = R_CSISW_SYS_HardwareDeInit(Unit);
    }
    else 
    {   
        /* nothing to do at this point of time  */
        err = R_CSISW_ERR_NOTLOCKED; 
    }
    return err;
}


/*******************************************************************************
  Function: R_CSISW_SetErrorCallback
*/

void R_CSISW_SetErrorCallback(void(*ErrorCallback )(uint32_t Unit, uint32_t Error)) 
{
    loc_ErrorHandler = ErrorCallback;
}



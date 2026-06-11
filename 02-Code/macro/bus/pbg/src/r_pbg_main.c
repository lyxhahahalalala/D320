/*
****************************************************************************
PROJECT : VLIB Macro Driver  
FILE    : $Id: r_pbg_main.c 7171 2016-01-06 17:52:36Z tobyas.hennig $
============================================================================ 
DESCRIPTION
Driver for the XC GUARD macro
============================================================================
                            C O P Y R I G H T                            
============================================================================
                           Copyright (c) 2012
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
  Title: PBG Main Module

  Implementation of the API and other global functions.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_dev_api.h"

#include "r_config_pbg.h"
#include "r_pbg_api.h"
#define PBG_MAIN_GLOBALS_
#include "r_pbg_main.h"
#include "r_pbg_sys.h"


/*******************************************************************************
  Section: Local Constants
*/

/*******************************************************************************
  Constant: loc_PBGVersion

  A string containig the macro driver version information  
*/

static const int8_t  loc_pbgDrvVer[] = R_VERSION(PBG, R_PBG_VERSION_HI, R_PBG_VERSION_LO);



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
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_PBG_CheckMacroParams
  
  See: <PBG_main.h>
*/
r_pbg_Error_t R_PBG_CheckMacroParams(uint32_t Unit);
r_pbg_Error_t R_PBG_CheckMacroParams(uint32_t Unit)
{
    /* Feel free to add any more usefull parameter checks */
    if (R_PBG_MACRO_NUM <= Unit)
    {
        return  R_PBG_ERR_RANGE; 
    }
    return R_PBG_ERR_OK;  
}

/*******************************************************************************
  Function: R_PBG_ErrorHandler
  
  See: <PBG_main.h>
*/
void R_PBG_ErrorHandler(uint32_t Unit, r_pbg_Error_t Error) 
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
  
  See: <r_pbg_api.h>
*/

/*******************************************************************************
  Function: R_PBG_GetVersionStr

  See: <r_pbg_api.h> 
*/
const int8_t * R_PBG_GetVersionStr(void) 
{
    return (const int8_t *) loc_pbgDrvVer;
}

/*******************************************************************************
  Function: R_PBG_GetMajorVersion

  See: <r_pbg_api.h> 
*/
const uint16_t R_PBG_GetMajorVersion(void) 
{
    return (uint16_t)R_PBG_VERSION_HI; 
}

/*******************************************************************************
  Function: R_PBG_GetMinorVersion

  See: <r_pbg_api.h> 
*/
const uint16_t R_PBG_GetMinorVersion(void) 
{
    return (uint16_t)R_PBG_VERSION_LO; 
}



/*******************************************************************************
  Function: R_PBG_Init

  See: <r_pbg_api.h> 
*/
r_pbg_Error_t R_PBG_Init(void) 
{
    r_dev_PbgInstance_t Instance;
    
    for (Instance = (r_dev_PbgInstance_t)0; Instance < R_DEV_PBG_NUM; Instance++)
    {
        R_PBG_InitInstance(Instance);
    }
    
    return R_PBG_ERR_OK;
}

/*******************************************************************************
  Function: R_PBG_InitInstance

  See: <r_pbg_api.h> 
*/
r_pbg_Error_t R_PBG_InitInstance(r_dev_PbgInstance_t Instance) 
{
    r_pbg_Id_t      Id;
    r_pbg_Error_t   Err = R_PBG_ERR_NG;
    r_pbg_Error_t   LoopErr;
    
    Id.Instance = Instance;
    
    for (Id.Channel = 0; Id.Channel < 16; Id.Channel++)
    {
        /* Try to initialialize a certain channel, risking that it might not exist */
        LoopErr = R_PBG_InitChannel(Id);
        
        /* If at least one channel was set up correctly, return OK. */
        if (R_PBG_ERR_OK == LoopErr)
        {
            Err = R_PBG_ERR_OK;
        }
    }
    
    return Err;
}

/*******************************************************************************
  Function: R_PBG_InitChannel

  See: <r_pbg_api.h> 
*/
r_pbg_Error_t R_PBG_InitChannel(r_pbg_Id_t Id) 
{
    r_pbg_t Instance;
    r_pbg_Error_t err = R_PBG_SYS_GetInstance(Id, &Instance);
    
    /* Initialize channel if it exists */
    if (R_PBG_ERR_OK == err)
    {        
        R_PBG_WRITE32(Instance.Base0 + (4 * Id.Channel), R_PBG_DEFAULT_SETTING);
    }
    return err;
}


/*******************************************************************************
  Function: R_PBG_ConfigureGuard

  See: <r_pbg_api.h> 
*/
r_pbg_Error_t R_PBG_ConfigureGuard(r_pbg_Id_t Id, r_pbg_Config_t Config)
{
    r_pbg_t Instance;
    r_pbg_Error_t err = R_PBG_SYS_GetInstance(Id, &Instance);
    
    uint32_t prot_reg_value = 0;
    uint32_t prot_reg_address = 0;

    if (R_PBG_ERR_OK != err)
    {
        /* ERROR - Guard does not exist */
    }
    else
    {
        prot_reg_value |= (Config.PROTLOCK  << PROTLOCK_BIT);
        prot_reg_value |= (Config.PROTVM    << PROTVM_BIT);
        prot_reg_value |= (Config.PROTUM    << PROTUM_BIT);
        prot_reg_value |= (Config.PROTPEID  << PROTPEID_BIT);
        prot_reg_value |= (Config.VCID      << VCID_BIT);
        prot_reg_value |= (Config.PROTSPID  << PROTSPID_BIT);
        prot_reg_value |= (Config.PROTDEB   << PROTDEB_BIT);
        prot_reg_value |= (Config.PROTRDPDEF << PROTRDPDEF_BIT);
        prot_reg_value |= (Config.PROTWRPDEF << PROTWRPDEF_BIT);
        prot_reg_value |= (Config.PROTRD    << PROTRD_BIT);
        prot_reg_value |= (Config.PROTWR    << PROTWR_BIT);
        
        prot_reg_address = Instance.Base0 + (4 * Id.Channel);
        
        R_PBG_WRITE32(prot_reg_address, prot_reg_value);
    }

    return err;
}

/*******************************************************************************
  Function: R_PBG_ConfigureGuardSysCall

  See: <r_pbg_api.h> 
*/
r_pbg_Error_t R_PBG_ConfigureGuardSysCall(r_pbg_Id_t Id, r_pbg_Config_t Config)
{
    return R_PBG_ConfigureGuardSysCall_Internal(Id, Config);
}

/*******************************************************************************
  Function: R_PBG_ClearErrorStatus

  See: <r_pbg_api.h> 
*/
void R_PBG_ClearErrorStatus(r_pbg_Id_t Id)
{
    r_pbg_t Instance;
    r_pbg_Error_t err = R_PBG_SYS_GetInstance(Id, &Instance);
    
    uint32_t ctl_reg_address = 0;

    if (R_PBG_ERR_OK != err)
    {
        /* ERROR - Guard does not exist */
    }
    else
    {
        ctl_reg_address = Instance.Base1 + R_PBG_REG_CTL;
        
        R_PBG_WRITE32(ctl_reg_address, 0x03);
    }
}

/*******************************************************************************
  Function: R_PBG_GetErrorStatus

  See: <r_pbg_api.h> 
*/
uint32_t R_PBG_GetErrorStatus(r_pbg_Id_t Id)
{
    r_pbg_t Instance;
    r_pbg_Error_t err = R_PBG_SYS_GetInstance(Id, &Instance);
    
    uint32_t stat_reg_address = 0;

    if (R_PBG_ERR_OK != err)
    {
        /* ERROR - Guard does not exist */
        return 0;
    }
    else
    {
        stat_reg_address = Instance.Base1 + R_PBG_REG_STAT;
        
        return R_PBG_READ32(stat_reg_address);
    }
}

/*******************************************************************************
  Function: R_PBG_GetErrorDetails

  See: <r_pbg_api.h> 
*/
uint32_t R_PBG_GetErrorDetails(r_pbg_Id_t Id, uint32_t *Addr, uint32_t *Type)
{
    r_pbg_t Instance;
    r_pbg_Error_t err = R_PBG_SYS_GetInstance(Id, &Instance);
    
    uint32_t addr_reg_address = 0;
    uint32_t type_reg_address = 0;

    if (R_PBG_ERR_OK != err)
    {
        /* ERROR - Guard does not exist */
        return 0;
    }
    else
    {
        addr_reg_address = Instance.Base1 + R_PBG_REG_ADDR;
        type_reg_address = Instance.Base1 + R_PBG_REG_TYPE;
        
        if (0 != Addr)
        {
            *Addr = R_PBG_READ32(addr_reg_address);
        }

        if (0 != Type)
        {
            *Type = R_PBG_READ32(type_reg_address);
        }
        
        return 0;
    }
}


#if 0
/*******************************************************************************
  Function: R_PBG_DeInit

  See: <r_pbg_api.h> 
*/
r_pbg_Error_t R_PBG_DeInit(uint32_t Unit)
{
    r_pbg_Error_t       err;
    r_pbg_Interrupt_t   interrupt;

    err = R_PBG_CheckMacroParams(Unit); 

    if (R_PBG_ERR_OK  != err) 
    {
        return err; 
    }
    if (0 != r_pbg_Dev[Unit].BaseAddr)
    {
        for (interrupt = (r_pbg_Interrupt_t)0; interrupt < R_PBG_INT_NUM; interrupt++)
        {
            R_PBG_DisableInt(Unit, interrupt);
            R_PBG_SetIsrCallback(Unit, interrupt, 0);
        }
        r_pbg_Dev[Unit].BaseAddr = 0;
        err = R_PBG_SYS_HardwareDeInit(Unit);
    }
    else 
    {   
        /* nothing to do at this point of time  */
        err = R_PBG_ERR_NOTLOCKED; 
    }
    return err;
}
#endif

#if 0 /* Unused */
/*******************************************************************************
  Function: R_PBG_SetErrorCallback
*/
void R_PBG_SetErrorCallback(void(*ErrorCallback )(uint32_t Unit, uint32_t Error)) 
{
    loc_ErrorHandler = ErrorCallback;
}
#endif



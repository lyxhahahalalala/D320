/*
****************************************************************************
PROJECT : OSTM driver
AUTHOR  : Michael Golczewski
FILE    : $Id: r_ostm_main.c 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Driver for OSTM macro

============================================================================
                            C O P Y R I G H T
============================================================================
                       Copyright (c) 2013 - 2014
                                  by 
                       Renesas Electronics (Europe) GmbH. 
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: only for testing, not for mass production

DISCLAIMER

LICENSEE has read, understood and accepted the terms and conditions defined in
the license agreement, especially the usage rights. In any case, it is
LICENSEE's responsibility to make sure that any user of the software complies
with the terms and conditions of the signed license agreement.

SAMPLE CODE is not part of the licensed software, as such it must not be used in
mass-production applications. It can only be used for evaluation and
demonstration purposes at customer's premises listed in the signed license
agreement.

****************************************************************************

****************************************************************************
*/


/*******************************************************************************
  Title: Main Module

  Implementation of the API and other global functions.
*/


/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_ostm_api.h"
#include "r_ostm_sys.h"
#include "r_config_ostm.h"
#include "r_ostm_regs.h"
#define OSTM_MAIN_GLOBALS_
#include "r_ostm_main.h"


/*******************************************************************************
  Section: Local Variables
*/

/*******************************************************************************
  Variable: loc_ErrorHandler

  A function pointer to a user callback (Applications error handler)  
  
  The value is assigned by the application using the driver 
*/

static void ( *loc_ErrorHandler )(uint32_t Unit, uint32_t Error) = 0u;


/*******************************************************************************
  Section: Global Functions
*/


/*******************************************************************************
  Function: R_OSTM_ErrorHandler

  For API documentation see file ostm_main.h
*/

void R_OSTM_ErrorHandler(uint32_t Unit, r_ostm_Error_t Error) 
{
    
    if (loc_ErrorHandler != 0u) 
    {   /* we've got a registered error handler here ...*/
        loc_ErrorHandler(Unit, (uint32_t)Error);   /* ... and we gonna use it!*/
    }
    else 
    {                                /* we don't have a handler */
        for (;;)                     /* so let's wait here ... */
        {
        }
    }
}


/*******************************************************************************
  Section: Global API Functions

  For API documentation see file <ostm_api.h>
*/

/*******************************************************************************
  Function: R_OSTM_Init

  For API documentation see file <ostm_api.h>
*/

r_ostm_Error_t R_OSTM_Init(uint32_t Unit, r_ostm_Parameter_t *Config) 
{
    uint32_t        base;
    uint32_t        cmpval;
    R_OSTM_RegCtl_t regval;
    
    if (Unit >= R_OSTM_MACRO_NUM) 
    {
        R_OSTM_ErrorHandler(Unit, R_OSTM_ERR_RANGE);
    }
    else
    {

        R_OSTM_Dev[Unit].BaseAddr = R_SYS_OSTM_GetBaseAddr(Unit);
        base = R_OSTM_Dev[Unit].BaseAddr;
        cmpval = Config->CmpVal;
        R_OSTM_WRITE_REG(32, base + R_OSTM_CMP, cmpval);

        regval.Bit.OstmMd1 = Config->Mode;
        regval.Bit.OstmMd0 = Config->IntCtl;
        
        R_OSTM_WRITE_REG(8, base + R_OSTM_CTL, regval.Byte);
    } 

    return R_OSTM_ERR_OK;
}


/*******************************************************************************
  Function: R_OSTM_DeInit

  For API documentation see file <ostm_api.h>
*/

r_ostm_Error_t R_OSTM_DeInit(uint32_t Unit)
{
    uint32_t base;
    if (Unit >= R_OSTM_MACRO_NUM)
    {
        R_OSTM_ErrorHandler(Unit, R_OSTM_ERR_RANGE);
    }
    else
    {
        R_OSTM_Dev[Unit].BaseAddr = R_SYS_OSTM_GetBaseAddr(Unit);
        base = R_OSTM_Dev[Unit].BaseAddr;

        R_OSTM_WRITE_REG(8, base + R_OSTM_TT, 0x01);         
        R_OSTM_WRITE_REG(8, base + R_OSTM_CTL, 0); 
        R_OSTM_WRITE_REG(8, base + R_OSTM_TO , 0);
        R_OSTM_WRITE_REG(8, base + R_OSTM_TOE, 0);
        R_OSTM_WRITE_REG(32, base + R_OSTM_EMU, 0);  
    }   
    R_SYS_OSTM_DisableInt(Unit, R_OSTM_INT_TINT);    
    return R_OSTM_ERR_OK;
}


/*******************************************************************************
  Function: R_OSTM_GetStatus

  For API documentation see file <ostm_api.h>
*/

uint8_t R_OSTM_GetStatus(uint32_t Unit)
{
    uint32_t base;
    uint8_t  regval;
    
    regval = 0u;
    
    if (Unit >= R_OSTM_MACRO_NUM)
    {
        R_OSTM_ErrorHandler(Unit, R_OSTM_ERR_RANGE);
    }
    else
    {
        base   = R_OSTM_Dev[Unit].BaseAddr;
        regval = R_OSTM_READ_REG(8, base + R_OSTM_TE);
    }
    return regval;
}


/*******************************************************************************
  Function: R_OSTM_GetCounterValue

  For API documentation see file <ostm_api.h>
*/

uint32_t R_OSTM_GetCounterValue(uint32_t Unit)
{
    uint32_t base;
    uint32_t  regval;
    
    if (Unit >= R_OSTM_MACRO_NUM)
    {
        R_OSTM_ErrorHandler(Unit, R_OSTM_ERR_RANGE);
    }
    else
    {
        base   = R_OSTM_Dev[Unit].BaseAddr;
        regval = R_OSTM_READ_REG(32, base + R_OSTM_CNT);
    }
    return regval;
}

/*******************************************************************************
  Function: R_OSTM_GetCompareRegValue

  For API documentation see file <ostm_api.h>
*/

uint32_t R_OSTM_GetCompareRegValue(uint32_t Unit)
{
    uint32_t base;
    uint32_t  regval;
    
    if (Unit >= R_OSTM_MACRO_NUM)
    {
        R_OSTM_ErrorHandler(Unit, R_OSTM_ERR_RANGE);
    }
    else
    {
        base   = R_OSTM_Dev[Unit].BaseAddr;
        regval = R_OSTM_READ_REG(32, base + R_OSTM_CMP);
    }

    return regval;
}

/*******************************************************************************
  Function: R_OSTM_GetOutputState

  For API documentation see file <ostm_api.h>
*/

uint8_t R_OSTM_GetOutputState(uint32_t Unit)
{
    uint8_t  regval;
    uint32_t base;
    
    if (Unit >= R_OSTM_MACRO_NUM)
    {
        R_OSTM_ErrorHandler(Unit, R_OSTM_ERR_RANGE);
    }
    else
    {
        base   = R_OSTM_Dev[Unit].BaseAddr;
        regval = R_OSTM_READ_REG(8, base + R_OSTM_TO);
    }
    return regval;
}


/*******************************************************************************
  Function: R_OSTM_Start

  For API documentation see file <ostm_api.h>
*/

r_ostm_Error_t R_OSTM_Start(uint32_t Unit)
{
    uint32_t base;

    if (Unit >= R_OSTM_MACRO_NUM)
    {
        R_OSTM_ErrorHandler(Unit, R_OSTM_ERR_RANGE);
    }
    else
    {
        base = R_OSTM_Dev[Unit].BaseAddr;
        R_OSTM_WRITE_REG(8, base + R_OSTM_TS, 0x01);
    }
    return R_OSTM_ERR_OK;

}


/*******************************************************************************
  Function: R_OSTM_Stop

  For API documentation see file <ostm_api.h>
*/

r_ostm_Error_t R_OSTM_Stop(uint32_t Unit)
{
    uint32_t base;
    
    if (Unit >= R_OSTM_MACRO_NUM)
    {
        R_OSTM_ErrorHandler(Unit, R_OSTM_ERR_RANGE);
    }
    else
    {
        base = R_OSTM_Dev[Unit].BaseAddr;
        R_OSTM_WRITE_REG(8, base + R_OSTM_TT, 0x01);
    }
    return R_OSTM_ERR_OK;
}


/*******************************************************************************
  Function: R_OSTM_OutputCtl

  For API documentation see file <ostm_api.h>
*/

r_ostm_Error_t R_OSTM_OutputCtl(uint32_t Unit, r_ostm_OutputParams_t *Params)
{
    uint32_t base;
        
    if (Unit >= R_OSTM_MACRO_NUM)
    {
        R_OSTM_ErrorHandler(Unit, R_OSTM_ERR_RANGE);
    }
    else
    {
        base = R_OSTM_Dev[Unit].BaseAddr;
        if (Params->Mode == R_OSTM_OUTPUT_CTL)
        {
            R_OSTM_WRITE_REG(8, base + R_OSTM_TOE, 0x00);
            if (Params->Level == R_OSTM_HIGH)
            {
                R_OSTM_WRITE_REG(8, base + R_OSTM_TO, 0x01);
            }
            else
            {

                R_OSTM_WRITE_REG(8, base + R_OSTM_TO, 0x00);
            }
        }
        else
        {

            R_OSTM_WRITE_REG(8, base + R_OSTM_TOE, 0x01);
        }
    }
    return R_OSTM_ERR_OK;
}


/*******************************************************************************
  Function: R_OSTM_SetOutputMode

  For API documentation see file <ostm_api.h>
*/

r_ostm_Error_t R_OSTM_SetOutputMode(uint32_t Unit, r_ostm_OutputParams_t *Mode)
{
    uint32_t base;
        
    if (Unit >= R_OSTM_MACRO_NUM)
    {
        R_OSTM_ErrorHandler(Unit, R_OSTM_ERR_RANGE);
    }
    else
    {
        base = R_OSTM_Dev[Unit].BaseAddr;
        if (Mode->Mode == R_OSTM_OUTPUT_CTL)
        {
            R_OSTM_WRITE_REG(8, base + R_OSTM_TOE, 0x00);
        
        }
        else
        {

            R_OSTM_WRITE_REG(8, base + R_OSTM_TOE, 0x01);
        }
    }
    return R_OSTM_ERR_OK;
}


/*******************************************************************************
  Function: R_OSTM_SetSvstop

  For API documentation see file <ostm_api.h>
*/

r_ostm_Error_t R_OSTM_SetSvstop(uint32_t Unit, r_ostm_SvStop_t Svstop)
{
    uint32_t base;
    
    if (Unit >= R_OSTM_MACRO_NUM)
    {
        R_OSTM_ErrorHandler(Unit, R_OSTM_ERR_RANGE);
    }
    else
    {

        R_OSTM_Dev[Unit].BaseAddr = R_SYS_OSTM_GetBaseAddr(Unit);
        base = R_OSTM_Dev[Unit].BaseAddr;
        R_OSTM_WRITE_REG(8, base + R_OSTM_EMU, (uint8_t)Svstop);
    }
    return R_OSTM_ERR_OK;
}


/*******************************************************************************
  Function: R_OSTM_SetCmp

  For API documentation see file <ostm_api.h>
*/

r_ostm_Error_t R_OSTM_SetCmp(uint32_t Unit, uint32_t CmpVal)
{
    uint32_t base;

    if (Unit >= R_OSTM_MACRO_NUM)
    {
        R_OSTM_ErrorHandler(Unit, R_OSTM_ERR_RANGE);
    }
    else
    {
        base = R_OSTM_Dev[Unit].BaseAddr;
        R_OSTM_WRITE_REG(32, base + R_OSTM_CMP, CmpVal);
    }
    return R_OSTM_ERR_OK;
}


/*******************************************************************************
  Function: R_OSTM_EnableInt

  For API documentation see file <ostm_api.h>
*/

r_ostm_Error_t R_OSTM_EnableInt(uint32_t Unit, r_ostm_Interrupt_t Interrupt)
{
    /* Check range */
    if ((Unit >= R_OSTM_MACRO_NUM) || (Interrupt >= R_OSTM_INT_NUM))
    {
        return R_OSTM_ERR_RANGE; 
    }
    R_SYS_OSTM_EnableInt(Unit, Interrupt); 
    return R_OSTM_ERR_OK; 
}


/*******************************************************************************
  Function: R_OSTM_DisableInt

  For API documentation see file <ostm_api.h>
*/

r_ostm_Error_t R_OSTM_DisableInt(uint32_t Unit, r_ostm_Interrupt_t Interrupt)
{
    
    /* Check range */
    if ((Unit >= R_OSTM_MACRO_NUM) || (Interrupt >= R_OSTM_INT_NUM))
    {
        return R_OSTM_ERR_RANGE; 
    }

    R_SYS_OSTM_DisableInt(Unit, Interrupt); 
    return R_OSTM_ERR_OK; 
}


/*******************************************************************************
  Function: R_OSTM_SetErrorCallback

  For API documentation see file <ostm_api.h>
*/

void R_OSTM_SetErrorCallback(void (*ErrorCallback)(uint32_t Unit, uint32_t Error))
{
    loc_ErrorHandler = ErrorCallback; 
}


/*******************************************************************************
  Function: R_OSTM_GetPClock

  For API documentation see file <ostm_api.h>
*/

uint32_t R_OSTM_GetClock(uint32_t Unit)
{
    return R_SYS_OSTM_GetClock(Unit);
}



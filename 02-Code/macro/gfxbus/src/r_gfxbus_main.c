/*
****************************************************************************
PROJECT : VLIB Macro Driver  
FILE    : $Id: r_gfxbus_main.c 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Driver for the GFXBUS macro
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
*/

/*******************************************************************************
  Title: GFXBUS Main Module

  Implementation of the API and other global functions.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_gfxbus_api.h"
#include "r_gfxbus_sys.h"
#include "r_gfxbus_main.h"


/*******************************************************************************
  Section: Local Constants
*/

/*******************************************************************************
  Constant: loc_GfxBusVersion

  A string containig the macro driver version information  
*/

#define LOC_GFXBUS_VERSIONPREFIX    "@(#)GFXBUS Version "
#define LOC_GFXBUS_VERSIONPOSTFIX   " - "


#define LOC_GFXBUS_VERSION      { LOC_GFXBUS_VERSIONPREFIX  R_XSTR (R_GFXBUS_VERSION_HI) "." \
                                R_XSTR(R_GFXBUS_VERSION_LO) LOC_GFXBUS_VERSIONPOSTFIX \
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
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_GFXBUS_ErrorHandler
  
  See: <gfxbus_main.h>
*/

void R_GFXBUS_ErrorHandler(uint32_t Unit, r_gfxbus_Error_t Error) 
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
  
  See: <gfxbus_api.h>
*/

/*******************************************************************************
  Function: R_GFXBUS_GetVersionStr

  See: <gfxbus_api.h> 
*/
const int8_t * R_GFXBUS_GetVersionStr(void)
{
    static const int8_t version[] = LOC_GFXBUS_VERSION;
    return version;
}

/*******************************************************************************
  Function: R_GFXBUS_GetMajorVersion

  See: <gfxbus_api.h> 
*/
const uint16_t R_GFXBUS_GetMajorVersion(void) 
{
    return (uint16_t)R_GFXBUS_VERSION_HI; 
}

/*******************************************************************************
  Function: R_GFXBUS_GetMinorVersion

  See: <gfxbus_api.h> 
*/
const uint16_t R_GFXBUS_GetMinorVersion(void) 
{
    return (uint16_t)R_GFXBUS_VERSION_LO; 
}



/*******************************************************************************
  Function: R_GFXBUS_Init

  See: <gfxbus_api.h> 
*/

 int32_t R_GFXBUS_Init(uint32_t Unit) 
 {
    int32_t err;
    err = R_GFXBUS_SYS_Init(Unit);
    return err;
}

/*******************************************************************************
  Function: R_GFXBUS_Config

  See: <gfxbus_api.h> 
*/

 r_gfxbus_Error_t R_GFXBUS_Config(uint32_t Unit, r_gfxbus_Cfg_t * Cfg) 
 {
    r_gfxbus_Error_t err;
    err = R_GFXBUS_SYS_Config(Unit, Cfg);
    return err;
}

/*******************************************************************************
  Function: R_GFXBUS_SetMode

  See: <gfxbus_api.h> 
*/

r_gfxbus_Error_t R_GFXBUS_SetMode(uint32_t Unit, uint8_t Area, r_gfxbus_AreaMode_t Mode)
{
    r_gfxbus_Error_t err;
    err = R_GFXBUS_SYS_SetMode(Unit, Area, Mode);
    return err;
}

/*******************************************************************************
  Function: R_GFXBUS_Flush

  See: <gfxbus_api.h> 
*/

void R_GFXBUS_Flush(uint32_t Unit, uint32_t Flush, uint32_t Size) 
{
  
    R_GFXBUS_SYS_Flush(Unit, Flush, Size);
}

/*******************************************************************************
  Function: R_GFXBUS_DeInit

  See: <gfxbus_api.h> 
*/

 int32_t R_GFXBUS_DeInit(uint32_t Unit)
 {
    int32_t err;
    err = R_GFXBUS_SYS_DeInit(Unit);
    return err;
}


/*******************************************************************************
  Function: R_GFXBUS_SetErrorCallback
*/

void R_GFXBUS_SetErrorCallback(void(*ErrorCallback )(uint32_t Unit, uint32_t Error)) 
{
    loc_ErrorHandler = ErrorCallback;
}



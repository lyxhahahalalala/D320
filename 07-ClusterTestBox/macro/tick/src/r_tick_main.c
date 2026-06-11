/*
****************************************************************************
PROJECT : VLIB
FILE    : $Id: r_tick_main.c 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Driver for TICK macro
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
*/

/*******************************************************************************
  Title: Main Module

  Implementation of the API and other global functions.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_config_tick.h"
#include "r_tick_api.h"
#define TICK_MAIN_GLOBALS_
#include "r_tick_main.h"
#include "r_tick_sys.h"


/*******************************************************************************
  Section: Global API Functions
  
  Comments see: <tick_api.h>
*/



/*******************************************************************************
  Function: R_TICK_Init
*/

 r_tick_Error_t R_TICK_Init(uint32_t Unit) 
 {
    r_tick_Error_t err;

    r_tick_Dev[Unit].Isr    = (void*)0;
    r_tick_Dev[Unit].TimeMS = 0u;
    err = R_SYS_TICK_HardwareInit(Unit);
    return err;
}


/*******************************************************************************
  Function: R_TICK_DeInit
*/

 r_tick_Error_t R_TICK_DeInit(uint32_t Unit) 
 {
    r_tick_Error_t err;

    err = R_SYS_TICK_HardwareDeInit(Unit);
    return err;
}

/*******************************************************************************
  Function: R_TICK_GetTimeMS
*/

 uint32_t R_TICK_GetTimeMS(uint32_t Unit) 
 {
    return r_tick_Dev[Unit].TimeMS;
}


/*******************************************************************************
  Function: R_TICK_GetTimeUS
*/

 uint32_t R_TICK_GetTimeUS(uint32_t Unit) 
 {
    uint32_t tick_us;
    uint32_t timeMS;
    
    do 
    {
        timeMS  = r_tick_Dev[Unit].TimeMS;
        tick_us = R_SYS_TICK_GetTimeUS(Unit, timeMS);
    } while (timeMS != r_tick_Dev[Unit].TimeMS);
    return tick_us;
}


/*******************************************************************************
  Function: R_TICK_WaitMS
*/

void R_TICK_WaitMS(uint32_t Unit, uint32_t TickMS) 
{
    int32_t tnow;
    int32_t tend;
    
    /* Wait for <TickMS> ms. */
    tend = R_TICK_GetTimeMS(Unit) + TickMS;
    do 
    {
        tnow = tend - R_TICK_GetTimeMS(Unit);
    } while (tnow > 0);
}


/*******************************************************************************
  Function: R_TICK_WaitUS
*/

void R_TICK_WaitUS(uint32_t Unit, uint32_t TickUS) 
{
    int32_t tnow;
    int32_t tend;
    
    /* Wait for <TickUS> us. */
    tend = R_TICK_GetTimeUS(Unit) + TickUS;
    do 
    {
        tnow = tend - R_TICK_GetTimeUS(Unit);
    } while (tnow > 0);
}

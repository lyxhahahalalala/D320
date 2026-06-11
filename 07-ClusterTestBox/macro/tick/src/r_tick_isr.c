/*
****************************************************************************
PROJECT : VLIB
FILE    : $Id: r_tick_isr.c 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Generic TICK driver
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
  Title: UARTA Interrupt Functions

  Handle the UARTA interrupts.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_config_tick.h"
#include "r_tick_api.h"
#include "r_tick_sys.h"
#include "r_tick_main.h"


/*******************************************************************************
  Section: Global API Functions
  
  For comments see: <tick_api.h>
*/

/*******************************************************************************
  Function: R_TICK_Isr
*/

void R_TICK_Isr(uint32_t Unit) {
    r_tick_Dev[Unit].TimeMS++;
    if (0 != r_tick_Dev[Unit].Isr) 
    {
        r_tick_Dev[Unit].Isr();
    }
}


/*******************************************************************************
  Function: R_TICK_EnableInt
*/

void R_TICK_EnableInt(uint32_t Unit) 
{
    R_SYS_TICK_EnableInt(Unit); 
}


/*******************************************************************************
  Function: R_TICK_DisableInt
*/

void R_TICK_DisableInt(uint32_t Unit) 
{
    R_SYS_TICK_DisableInt(Unit); 
 
}

/*******************************************************************************
  Function: R_TICK_SetIsrCallback
*/

r_tick_Error_t R_TICK_SetIsrCallback(uint32_t Unit, void (*Isr)(void)) 
{
    r_tick_Dev[Unit].Isr = Isr;
    return R_TICK_ERR_OK;
}


/*
****************************************************************************
PROJECT : TAUJ driver
FILE    : $Id: r_tauj_isr.c 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Driver for TAUJ macro
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



/***********************************************************
  Title: TAUJ Interrupt Functions

  Handle the TAUJ interrupts.
*/

/***********************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_tauj_api.h"

#include "r_tauj_sys.h"
#include "r_config_tauj.h"
#include "r_tauj_regs.h"
#include "r_tauj_main.h"


/***********************************************************
  Section: Global Functions
*/

/***********************************************************
  Section: Global API Functions

  For comments see file tauj_api.h
*/

/***********************************************************
  Function: R_TAUJ_SetIsrCallback
*/
r_tauj_Error_t R_TAUJ_SetIsrCallback(uint32_t Unit, r_tauj_Interrupt_t IntType,
                                   void (*Isr)(void)) 
{
    if (Unit >= R_TAUJ_MACRO_NUM) {
        return R_TAUJ_ERR_RANGE;
    } else {
        R_TAUJ_Dev[Unit].IsrCallback[IntType] = Isr;
    }
    return R_TAUJ_ERR_OK;
}


/***********************************************************
  Function: R_TAUJ_EnableInt

*/
r_tauj_Error_t R_TAUJ_EnableInt(uint32_t Unit, r_tauj_Interrupt_t Interrupt) 
{
    /* Check range */
    if ((Unit >= R_TAUJ_MACRO_NUM) || (Interrupt >= R_TAUJ_INT_NUM))
    {
        return R_TAUJ_ERR_RANGE;
    }
    R_TAUJ_Sys_EnableInt(Unit, Interrupt);

    return R_TAUJ_ERR_OK;
}


/***********************************************************
  Function: R_TAUJ_DisableInt

*/
r_tauj_Error_t R_TAUJ_DisableInt(uint32_t Unit, r_tauj_Interrupt_t Interrupt) 
{
    /* Check range */
    if ((Unit >= R_TAUJ_MACRO_NUM) || (Interrupt >= R_TAUJ_INT_NUM))
    {
        return R_TAUJ_ERR_RANGE;
    }
    R_TAUJ_Sys_DisableInt(Unit, Interrupt);

    return R_TAUJ_ERR_OK;
}


/***********************************************************
  Function: R_TAUJ_Isr
*/

void R_TAUJ_Isr(uint32_t Unit, r_tauj_Interrupt_t Interrupt) 
{
    if (R_TAUJ_Dev[Unit].IsrCallback[Interrupt] != R_NULL) 
    {
        R_TAUJ_Dev[Unit].IsrCallback[Interrupt]();
    }
}

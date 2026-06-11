/*
****************************************************************************
PROJECT : OSTM driver
FILE    : $Id: r_ostm_isr.c 7402 2016-01-27 15:43:39Z florian.zimmermann $
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
*/


/*******************************************************************************
  Title: OSTM Interrupt Functions

  Handle the OSTM interrupts.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_ostm_api.h"

#include "r_ostm_sys.h"
#include "r_config_ostm.h"
#include "r_ostm_regs.h"
#include "r_ostm_main.h"

/*******************************************************************************
  Section: Global Functions
*/
  

/*******************************************************************************
  Section: Global API Functions
  
  For API documentation see file ostm_api.h
*/

/*******************************************************************************
  Function: R_OSTM_SetIsrCallback
*/

 r_ostm_Error_t R_OSTM_SetIsrCallback(uint32_t Unit, r_ostm_Interrupt_t IntType, 
                                      void (*Isr)(void)) 
{
                                    
    if (Unit >= R_OSTM_MACRO_NUM) {
        return R_OSTM_ERR_RANGE; 
    } 

    R_OSTM_Dev[Unit].IsrCallback[IntType] = Isr;
    return R_OSTM_ERR_OK;
}


/*******************************************************************************
  Function: R_OSTM_TintIsr
*/

void R_OSTM_TintIsr(uint32_t Unit) 
{
    /* Applications interrupt handling */
    if (R_OSTM_Dev[Unit].IsrCallback[R_OSTM_INT_TINT] != 0u) {
        R_OSTM_Dev[Unit].IsrCallback[R_OSTM_INT_TINT](); 
    }
}


/*
****************************************************************************
PROJECT : TAUJ driver
FILE    : $Id: r_sys_tauj.c 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Driver for TAUJ macro
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



/***********************************************************
  Title: TAUJ Support Functions
*/

/***********************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_tauj_api.h"
#include "r_tauj_sys.h"
#include "r_config_tauj.h"
#include "r_dev_api.h"

/***********************************************************
  Section: Local Defines
*/

 /* TAUJ Filters  */

#define FILTER_BYPASS (0x80u)
 #define TAUJ0_FCLACTL_BASE (0xFF414080u)
 #define TAUJ1_FCLACTL_BASE (0xFF414090u)
 #define TAUJ2_FCLACTL_BASE (0xFF416180u)
 
/***********************************************************
  Variable: loc_eeTauJ_IntAddress

  Stores the Addresses of the Interrupt control registers for each unit
  The order of the adresses for each interrupt type schould be the same
  as defined in tauj_api.h See <eeTauJ_Interrupt_t>
*/

/* Interrupt controller base address */
#define TAUJ0_EIC_BASE     (R_INTC_TAUJ0_FIRST) 
                             
#define TAUJ_X_INT_Y(unit,num)  (R_DEV_INT_TAUJ##unit##I##num)

static const r_dev_IntSel_t loc_eeTauJ_IntAddress[R_TAUJ_MACRO_NUM][R_TAUJ_INT_NUM] = 
{
/* unit 0 (TAUJ 0) */  /* 0 */         /* 1 */             /* 2 */             /* 3 */
                     {TAUJ_X_INT_Y(0,0), TAUJ_X_INT_Y(0,1), TAUJ_X_INT_Y(0,2), TAUJ_X_INT_Y(0,3)}

};

static const uint32_t loc_eeTauJ_BaseAddress[R_TAUJ_MACRO_NUM] = 
{
/* unit 0 (TAUJ) */    /* 0 */            
                       R_TAUJ_0_BASE_ADDR
};



/***********************************************************
  Section: Global Functions

  Comments see: <tauj_sys.h>
*/



/***********************************************************
  Function: R_TAUJ_Sys_GetBaseAddr

*/

uint32_t R_TAUJ_Sys_GetBaseAddr(uint32_t Unit)
{
    return (uint32_t) loc_eeTauJ_BaseAddress[Unit];
}


/***********************************************************
  Function: R_TAUJ_Sys_GetClock
*/
uint32_t R_TAUJ_Sys_GetClock(uint32_t Unit) 
{ 
    uint32_t val;                                 
                                                  
    if(Unit == 0)
    {
         val = R_DEV_ClkFrequencyHz(R_DEV_CKS_TAUJ);
    }
    else 
    {
        val = R_TAUJ_ERR_INSTANCE;
    }
    return val;                                   
}                

/***********************************************************
  Function: R_TAUJ_Sys_IntCInit

*/

void R_TAUJ_Sys_IntCInit(uint32_t Unit)
{
    uint8_t i = 0;
    for(i = 0; i < R_TAUJ_INT_NUM; i++)
    {
        /* Enable the interrupts */
        R_DEV_IntEnable(loc_eeTauJ_IntAddress[Unit][i], 0);
    }
}


/***********************************************************
  Function: R_TAUJ_Sys_EnableInt

*/

void R_TAUJ_Sys_EnableInt(uint32_t Unit, r_tauj_Interrupt_t Interrupt)
{
    R_DEV_IntEnable(loc_eeTauJ_IntAddress[Unit][(uint8_t)Interrupt], 1);
}


/***********************************************************
  Function: R_TAUJ_Sys_DisableInt

*/

void R_TAUJ_Sys_DisableInt(uint32_t Unit, r_tauj_Interrupt_t Interrupt)
{
    R_DEV_IntEnable(loc_eeTauJ_IntAddress[Unit][(uint8_t)Interrupt], 0);
}

/***********************************************************
  Function: R_TAUJ_Sys_SetFilterBypass

*/

uint32_t R_TAUJ_Sys_SetFilterBypass(uint32_t Unit)  
{
    uint32_t base_address = 0;
    uint32_t i = 0;
    uint32_t val = 0;

    if (Unit >= R_TAUJ_MACRO_NUM)
    {
        val = 1;
    }
    else
    {
        if(Unit == 0)
        {
           base_address = TAUJ0_FCLACTL_BASE;
        }
        else 
        {
            val = 1;
        }
    }
    if(val == 0)
    {
        /* Set digital filter to Bypass mode */
        for(i = 0; i < R_TAUJ_NUM_CHANNELS; i++)
        {
            R_TAUJ_WRITE8(base_address + (4 * i), FILTER_BYPASS);
        }
    }
    return val;
}



/*
****************************************************************************
PROJECT : VLIB macro device level driver
FILE    : $Id: r_sys_spea.c 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
SPEA system functions for d1x
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
  Title: dev_spea SPEA device functions

  Implementation of the SPEA support functions for the 
  device dev_spea.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_dev_api.h"
#include "r_spea_api.h"
#include "r_spea_sys.h"
#include "r_config_spea.h"

#if defined (R_DBG_PRINT_DEV_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#endif



/*******************************************************************************
  Section: Local Constants
*/

/* Sprite Engine update timing control register */
#define LOC_SPEAUPDEN   0xFFC06048u

/*******************************************************************************
  Section: Global Functions

*/

/*******************************************************************************
  Function: R_SPEA_SPEA_BaseAddr
  
  See: r_spea_sys.h for details
*/

uint32_t R_SPEA_SYS_BaseAddr(uint32_t Unit) 
{
    if (Unit >= R_SPEA_MACRO_NUM) 
    {
        return R_SPEA_ERR_NG; 
    }
    else 
    {
        return R_SPEA_BASE;
    } 
}

/*******************************************************************************
  Function: R_SPEA_SYS_HardwareInit
  
  See: r_spea_sys.h for details
*/

r_spea_Error_t R_SPEA_SYS_HardwareInit(uint32_t Unit) 
{
    if (Unit >= R_SPEA_MACRO_NUM) 
    {
        return R_SPEA_ERR_NG; 
    }
    else 
    {
        R_DEV_WRITE_REG(32, LOC_SPEAUPDEN, 0x0F0Fu);
        return R_SPEA_ERR_OK;
    } 
}

/*******************************************************************************
  Function: R_SPEA_SYS_ErrorHandler
  See: r_spea_sys.h for details
*/

void R_SPEA_SYS_ErrorHandler(uint32_t Unit, r_spea_Error_t Error) 
{
    R_DEV_ERROR(Unit, (uint32_t)Error, "SPEA: Error occured");
}


/*******************************************************************************
  Function: R_SPEA_SYS_HardwareDeInit
  
  See: r_spea_sys.h for details
*/

r_spea_Error_t R_SPEA_SYS_HardwareDeInit(uint32_t Unit) 
{
    if (Unit >= R_SPEA_MACRO_NUM) 
    {
        return R_SPEA_ERR_RANGE; 
    }
    else 
    {
        return R_SPEA_ERR_OK;
    } 
}




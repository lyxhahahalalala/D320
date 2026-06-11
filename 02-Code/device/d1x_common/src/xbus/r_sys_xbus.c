/*
****************************************************************************
PROJECT : VLIB macro device level driver
FILE    : $Id: r_sys_xbus.c 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
XBUS system functions for d1x
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
  Title: dev_xbus XBUS device functions

  Implementation of the XBUS support functions for the 
  device dev_xbus.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_dev_api.h"
#include "r_xbus_api.h"
#include "r_xbus_sys.h"
#include "r_config_xbus.h"

#if defined (R_DBG_PRINT_DEV_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#endif



/*******************************************************************************
  Section: Local Constants
*/

/*******************************************************************************
  Section: Global Functions

*/


/*******************************************************************************
  Function: R_XBUS_SYS_BaseAddr
  
  See: r_xbus_sys.h for details
*/

uint32_t R_XBUS_SYS_BaseAddr(uint32_t Unit) 
{
    return R_XBUS_BASE_ADDR; 
}

/*******************************************************************************
  Function: R_XBUS_SYS_GetMaxIdx
  See: r_xbus_sys.h for details
*/

uint32_t R_XBUS_SYS_GetMaxIdx(uint32_t Unit) 
{
    r_dev_Device_t  device  =  R_DEV_GetDev(); 
    uint32_t        ret_val = 0; 
    if (device < R_DEV_R7F701402)
    {
        ret_val = 0; 
    }
    else if (device < R_DEV_R7F701408)
    {
        ret_val = 128u; 
    }
    else if (device < R_DEV_LAST_DX_DEV)
    {
        ret_val = 256u; 
    }
    else 
    {
       /* no cache available */
       ret_val = 0;  
    }
    return ret_val; 
}

/*******************************************************************************
  Function: R_SYS_XBUS_ErrorHandler
  See: r_xbus_sys.h for details
*/

void R_SYS_XBUS_ErrorHandler(uint32_t Unit, r_xbus_Error_t Error) 
{
    R_DEV_ERROR(Unit, (uint32_t)Error, "XBUS: Error occured");
}

/*******************************************************************************
  Function: R_XBUS_SYS_HardwareInit
  
  See: r_xbus_sys.h for details
*/

r_xbus_Error_t R_XBUS_SYS_HardwareInit(uint32_t Unit) 
{
    return R_XBUS_ERR_OK; 
    
}
/*******************************************************************************
  Function: R_XBUS_SYS_HardwareDeInit
  
  See: r_xbus_sys.h for details
*/

r_xbus_Error_t R_XBUS_SYS_HardwareDeInit(uint32_t Unit) 
{
    return R_XBUS_ERR_OK; 
}

/*******************************************************************************
  Function: R_XBUS_SYS_BusBusy
  
  See: r_xbus_sys.h for details
*/

r_Error_t R_XBUS_SYS_BusBusy(uint32_t Unit) 
{
    return R_ERR_OK; 
}


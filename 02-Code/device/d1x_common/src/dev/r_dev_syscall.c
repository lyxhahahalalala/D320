/*
****************************************************************************
PROJECT : device driver for V-Lib
FILE    : $Id: r_dev_syscall.c 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================
DESCRIPTION
r7f701412 syscall routines
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
  Title: device general (error handling, init/prepare) functions

  
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_dev_api.h"

#ifdef R_DBG_PRINT_MSG
   #include <stdio.h> 
   #include "r_dbg_api.h"
#endif

#ifdef USE_PBG
   #include "r_pbg_api.h"
#endif

/*******************************************************************************
  Section: Local Types 
*/

/*******************************************************************************
  Section: Local Constants 
*/

/*******************************************************************************
  Section: Local Variables
*/

/*******************************************************************************
  Section: Local Functions
*/

/*******************************************************************************
  Section: Headers for Assember Functions

  The following two functions are defined in <r_dev_asm.s>
*/

/***********************************************************
  Function: R_DEV_EnterUserModeElevated

  Sets the UserMode bit of the PSW.
  Calling this function reduces the privileges of the application.
  To be called from <R_DEV_SysCallElevated>.

  Parameters:
  void
  
  Returns:
  void   
  
*/

void R_DEV_EnterUserModeElevated(void);
        

/***********************************************************
  Function: R_DEV_EnterSupervisorModeElevated

  Clears the UserMode bit of the FEPSW.
  Calling this function elevates the privileges of the application.
  To be called from <R_DEV_SysCallElevated>.

  Parameters:
  void
  
  Returns:
  void   
  
*/

void R_DEV_EnterSupervisorModeElevated(void);


/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_DEV_SysCallElevated

  see <r_dev_api.h>

*/
uint32_t R_DEV_SysCallElevated(r_dev_SysCall_t Cmd, void *Par1, void *Par2, void *Par3)
{
    uint32_t retval;
    
    switch(Cmd)
    {
    case R_DEV_SYSCALL_SUPERVISOR_MODE:
        R_DEV_EnterSupervisorModeElevated();
        retval = 0;
        break;
    case R_DEV_SYSCALL_USER_MODE:
        R_DEV_EnterUserModeElevated();
        retval = 0;
        break;
    case R_DEV_SYSCALL_CONFIGURE_PBG:
      #ifdef USE_PBG
        retval = (uint32_t)R_PBG_ConfigureGuard( 
                                *(r_pbg_Id_t*)Par1, *(r_pbg_Config_t*)Par2 );
      #else
        retval = 0;
      #endif
        break;
    default:
        retval = 1;
        break;        
    }
    
    return retval;
}





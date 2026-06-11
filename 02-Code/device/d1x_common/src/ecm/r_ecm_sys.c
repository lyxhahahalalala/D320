/*
****************************************************************************
PROJECT : VLIB macro device level driver
FILE    : $Id: r_ecm_sys.c 3952 2014-09-24 09:52:47Z golczewskim $
============================================================================ 
DESCRIPTION
ECM system functions for D1M2(H) device
============================================================================
                            C O P Y R I G H T                            
============================================================================
                           Copyright (c) 2014
                                  by 
                       Renesas Electronics (Europe) GmbH. 
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: only for testing

DISCLAIMER                                                                   
This software is supplied by Renesas Electronics Corporation and is only     
intended for use with Renesas products. No other uses are authorized. This   
software is owned by Renesas Electronics Corporation and is protected under  
all applicable laws, including copyright laws.                               
THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING  
THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT      
LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE   
AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.          
TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS       
ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE  
FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR   
ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE  
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.                             
Renesas reserves the right, without notice, to make changes to this software 
and to discontinue the availability of this software. By using this software,
you agree to the additional terms and conditions found by accessing the      
following link:                                                              
http://www.renesas.com/disclaimer *                                          
Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.     

****************************************************************************
*/


/*******************************************************************************
  Title: dev_ecm ECM device functions

  Implementation of the ECM support functions for the 
  device dev_ecm.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_dev_api.h"
#include "r_dev_err_types.h"

#include "r_ecm_api.h"
#include "r_ecm_sys.h"
#include "r_config_ecm.h"

#if defined (R_DBG_PRINT_DEV_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#else
    #define R_DBG_PRINT(lvl, txt) 
#endif



/*******************************************************************************
  Section: Local Constants
*/

/*******************************************************************************
  Section: Global Functions

*/

/*******************************************************************************
  Function: R_ECM_CheckMacroParams
  
  See: <ecm_main.h>
*/
r_ecm_Error_t R_ECM_CheckMacroParams(uint32_t Unit);

/*******************************************************************************
  Function: R_ECM_SYS_BaseAddr
  
  See: r_ecm_sys.h for details
*/

uint32_t R_ECM_SYS_BaseAddr(uint32_t Unit) 
{
    R_ECM_CheckMacroParams(Unit);
    
    return R_ECM_BASE;
}

/*******************************************************************************
  Function: R_ECM_SYS_HardwareInit
  
  See: r_ecm_sys.h for details
*/

r_ecm_Error_t R_ECM_SYS_HardwareInit(uint32_t Unit) 
{
    R_ECM_CheckMacroParams(Unit);
#if 0
    R_DEV_IntEnable(r_dev_IntSel_t Int, 1);
#endif
    return R_ECM_ERR_OK;
}
/*******************************************************************************
  Function: R_ECM_SYS_HardwareDeInit
  
  See: r_ecm_sys.h for details
*/

r_ecm_Error_t R_ECM_SYS_HardwareDeInit(uint32_t Unit) 
{
    R_ECM_CheckMacroParams(Unit);
#if 0
    R_DEV_IntEnable(r_dev_IntSel_t Int, 0);
#endif
    return R_ECM_ERR_OK;
}

/*******************************************************************************
  Function: R_ECM_SYS_EnableInt
*/
#if 0
void R_ECM_SYS_EnableInt(uint32_t Unit, r_ecm_Interrupt_t Interrupt) 
{
   /* Example (You need to update the device interrupt table r_dev_IntSel_t first): 
    R_DEV_IntEnable((r_dev_IntSel_t)(R_DEV_INT_ECMint + Interrupt) ,1); 
   */
}
#endif

/*******************************************************************************
  Function: R_ECM_SYS_DisableInt
*/
#if 0
void R_ECM_SYS_DisableInt(uint32_t Unit, r_ecm_Interrupt_t Interrupt) 
{
    /* Example : 
    R_DEV_IntEnable((r_dev_IntSel_t)(R_DEV_INT_ECMint + Interrupt) ,0); 
    */
}
#endif



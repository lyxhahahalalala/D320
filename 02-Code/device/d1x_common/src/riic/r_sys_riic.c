/*
****************************************************************************
PROJECT : RIIC driver
FILE    : $Id: r_sys_riic.c 3952 2014-09-24 09:52:47Z golczewskim $
============================================================================ 
DESCRIPTION
RIIC system functions for r7f701412
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
  Title: upd703532 RIIC Support Functions

  Implementation of the RIIC support functions for the 
  upd703532 device.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_riic_api.h"
#include "r_dev_api.h"
#include "r_riic_sys.h"
#include "r_config_riic.h"

#if defined (R_DBG_PRINT_DEV_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#else
    #define R_DBG_PRINT(lvl, txt) 
#endif




/*******************************************************************************
  Section: Local Defines
*/

/*******************************************************************************
  Section: Local Variables
*/

/*******************************************************************************
  Constant: loc_BaseAddress

  Base address table of RIIC macros
*/
static const uint32_t loc_BaseAddress[R_RIIC_MACRO_NUM] = 
{
/*riic0*/     0xFFDB0000u,
/*riic1*/     0xFFDB1000u,
};

/*******************************************************************************
  Constant: loc_ClkSelId

  Clock selector ID for each RIIC unit  
*/
static const r_dev_ClkSel_t loc_ClkSelId[R_RIIC_MACRO_NUM] = 
{
/*riic0*/    R_DEV_CKS_RIIC,
/*riic1*/    R_DEV_CKS_RIIC
};


/*******************************************************************************
  Section: RIIC Macro Driver Support Functions
  
  Comments see: <riic_sys.h>
*/


/*******************************************************************************
  Function: R_SYS_RIIC_SysGetClockHz

  See: r_sys_riic.h for details    

*/

uint32_t R_SYS_RIIC_SysGetClockHz(uint32_t Unit) 
{
    uint32_t val;
    
    val = R_DEV_ClkFrequencyHz(loc_ClkSelId[Unit]); 
    return val;
}


/*******************************************************************************
  Function: R_SYS_RIIC_BaseAddr

  See: r_uerte_sys.h for details    

*/
uint32_t R_SYS_RIIC_BaseAddr(uint32_t Unit) 
{
    uint32_t addr;
    if (Unit >= R_RIIC_MACRO_NUM) 
    {
        return R_BAD_ADDRESS; 
    }
    addr = loc_BaseAddress[Unit];
    return addr;
}

/*******************************************************************************
  Function: R_SYS_RIIC_Init
  
  See: r_uerte_sys.h for details    
*/

void R_SYS_RIIC_Init(uint32_t Unit)
{
}
    

/*******************************************************************************
  Function: R_SYS_RIIC_DeInit
  
  See: r_uerte_sys.h for details    
*/

void R_SYS_RIIC_DeInit(uint32_t Unit)
{
}


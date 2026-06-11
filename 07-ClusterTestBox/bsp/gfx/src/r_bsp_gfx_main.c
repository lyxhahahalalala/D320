/*
****************************************************************************
PROJECT : BSP - GFX
FILE    : $Id: r_bsp_gfx_main.c 2708 2014-05-16 08:17:53Z michael.golczewski $
============================================================================
DESCRIPTION
BSP extension for Graphics (GFX)
============================================================================
                            C O P Y R I G H T
============================================================================
                           Copyright (c) 2012
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

/*
  Section: BSP GFX Description 
  
  This is the GFX for the BSP. All function for user interaction are implemented here
  The Board specific part (if required) is found in 
 'vlib/bsp/board/<any board>/src/gfx/r_bsp_sys_gfx.c'
  
*/


/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"     /* Renesas basic types, e.g. uint32_t */

#include "r_bsp_gfx_api.h"
#include "r_bsp_gfx_sys.h"
#include "r_bsp_board_config.h"


/*******************************************************************************
  Section: Local Defines
*/
 
/*******************************************************************************
  Section: Local Functions
*/

/*******************************************************************************
  Section: Global BSP GFX Functions

*/

/*******************************************************************************
  Function: R_BSP_GFX_Init
  
  See: r_bsp_gfx_api.h for details    
*/
void R_BSP_GFX_LowInit(void) 
{
    /* configure board GFX */
    R_BSP_SYS_GFX_LowInit(); 
}

/*******************************************************************************
  Function: R_BSP_GFX_Init
  
  See: r_bsp_gfx_api.h for details    
*/
void R_BSP_GFX_Init(void) 
{
    /* configure board GFX */
    R_BSP_SYS_GFX_Init(); 
}


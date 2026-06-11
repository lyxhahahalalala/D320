/*
****************************************************************************
PROJECT : BSP - GFX
FILE    : $Id: r_bsp_gfx_sys.h 2708 2014-05-16 08:17:53Z michael.golczewski $
============================================================================ 
Description
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

#ifndef BSP_GFX_SYS_H_
#define BSP_GFX_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Section: board specific GFX service functions
 
  These functions must be implemented specific in
  'vlib/bsb/board/<any board>/src/gfx/r_bsp_sys_gfx.c'
*/

/*******************************************************************************
  Function: R_BSP_SYS_GFX_LowInit

  Initialise the device & board dependant GFX on a lower (befire main() ) level 

  Parameters:
    void
  Returns:
    void     
*/

void R_BSP_SYS_GFX_LowInit(void); 


/*******************************************************************************
  Function: R_BSP_SYS_GFX_Init

  Initialise the device & board dependant GFX  

  Parameters:
    void
  Returns:
    void     

*/

void R_BSP_SYS_GFX_Init(void); 


#ifdef __cplusplus
}
#endif

#endif /* BSP_GFX_SYS_H_ */


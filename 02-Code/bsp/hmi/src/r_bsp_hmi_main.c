/*
****************************************************************************
PROJECT : BSP - HMI 
FILE    : $Id: r_bsp_hmi_main.c 7262 2016-01-15 13:34:37Z michael.golczewski $
============================================================================
DESCRIPTION
BSP extension for Human Machine Interface (HMI)
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
  Section: BSP HMI Description 
  
  This is the HMI for the BSP. All function for user interaction are implemented here
  The Board specific part (if required) is found in 
 'vlib/bsb/board/<any board>/src/hmi/r_bsp_sys_hmi.c'
  
        
*/


/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"     /* Renesas basic types, e.g. uint32_t */

#include "r_tick_api.h"

#include "r_bsp_hmi_api.h"
#include "r_bsp_hmi_sys.h"
#include "r_bsp_board_config.h"


/*******************************************************************************
  Section: Local Defines
*/

/*******************************************************************************
  Section: Local Variables
*/

/*******************************************************************************
  Variable: loc_ButtonCallback

  A function pointer array to a user callback for button/knob action  
  
  The value is assigned by the application using the driver 
*/
static  void (*loc_ButtonCallback[BSP_BTN_MAX])(void);

 
/*******************************************************************************
  Section: Local Functions
*/



/*******************************************************************************
  Section: Global BSP HMI Functions

*/

/*******************************************************************************
  Function: R_BSP_HMI_Init
  
  See: r_bsp_hmi_api.h for details    
*/
void R_BSP_HMI_Init(void) 
{
    /* configure board HMI */
    R_BSP_SYS_HMI_Init(); 
}

/*******************************************************************************
  Function: R_BSP_SetButtonCallback

  See: r_bsp_hmi_api.h for details
*/
r_Error_t R_BSP_SetButtonCallback(r_bsp_hmi_ButtonAaction_t BtnAction, 
                                          void (*Callback)(void))
{
    if (BtnAction < BSP_BTN_MAX)
    {
        loc_ButtonCallback[BtnAction] = Callback;
        return R_ERR_OK;
    }
    else
    {
        return R_PARAMETER_INCORRECT;
    }
}

/*******************************************************************************
  Function: R_BSP_BtnAction

  See: r_bsp_hmi_api.h for details
*/

void R_BSP_BtnAction(r_bsp_hmi_ButtonAaction_t BtnAction)
{
    if ((BtnAction < BSP_BTN_MAX) && (loc_ButtonCallback[BtnAction] != 0u))
    {
        loc_ButtonCallback[BtnAction]();
    }
}

/*******************************************************************************
  Function: R_BSP_HMI_SetLed
  
  See: r_bsp_hmi_api.h for details    
*/
r_Error_t R_BSP_HMI_SetLed(uint8_t LedNumber, uint32_t Brightness)
{
    return R_BSP_SYS_HMI_SetLed(LedNumber, Brightness);
}

/*******************************************************************************
  Function: R_BSP_HMI_GetLed
  
  See: r_bsp_hmi_api.h for details    
*/
int8_t R_BSP_HMI_GetLed(uint8_t LedNumber) 
{
    return R_BSP_SYS_HMI_GetLed(LedNumber);
}






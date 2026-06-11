/*
****************************************************************************
PROJECT : BSP - HMI (API)
FILE    : $Id: r_bsp_hmi_api.h 2211 2014-03-26 16:00:15Z tobyas.hennig $
============================================================================ 
Description
API definition for the application test framework
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
#ifndef BSP_HMI_API_H_
#define BSP_HMI_API_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Title: BSP HMI API

  Any application using HMI  
  should include this header file. 
  
*/
/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Enum: r_bsp_hmi_ButtonAaction_t

  BSP Buttons actions
  You can assign your own function to each of this "actions". 
  
  Values:
    BSP_BTN_UP_PRESS             - Button "up" pressed
    BSP_BTN_UP_RELEASE,          - Button "up" released
    BSP_BTN_DOWN_PRESS,          - Button "down" pressed  
    BSP_BTN_DOWN_RELEASE,        - Button "downm" released 
    BSP_BTN_LEFT_PRESS,          - Button "left" pressed  
    BSP_BTN_LEFT_RELEASE,        - Button "left" released 
    BSP_BTN_RIGHT_PRESS          - Button "right" pressed
    BSP_BTN_RIGHT_RELEASE        - Button "right" released 
    BSP_BTN_CENTER_PRESS,        - Button "center" pressed  
    BSP_BTN_CENTER_RELEASE,      - Button "center" released 
    BSP_BTN_LEFT_TURN,           - Rotary rotate left  
    BSP_BTN_RIGHT_TURN,          - Rotary rotate right
    BSP_BTN_OK_PRESS,            - Rotary center pressed  
    BSP_BTN_OK_RELEASE,          - Rotate center released 
    BSP_BTN_MAX                  - Delimiter

*/

typedef enum
{
    BSP_BTN_UP_PRESS = 1u,
    BSP_BTN_UP_RELEASE,
    BSP_BTN_DOWN_PRESS,
    BSP_BTN_DOWN_RELEASE,
    BSP_BTN_LEFT_PRESS,
    BSP_BTN_LEFT_RELEASE,
    BSP_BTN_RIGHT_PRESS,
    BSP_BTN_RIGHT_RELEASE,
    BSP_BTN_CENTER_PRESS,
    BSP_BTN_CENTER_RELEASE,
    BSP_BTN_LEFT_TURN,
    BSP_BTN_RIGHT_TURN,
    BSP_BTN_OK_PRESS,
    BSP_BTN_OK_RELEASE,
    BSP_BTN_MAX
} r_bsp_hmi_ButtonAaction_t;


/*******************************************************************************
  Enum: r_bsp_hmi_MouseAaction_t

  BSP Mouse actions

  Values:
    BSP_MOUSE_LEFT_PRESS       - Mouse Button left press (button state changed to pressed)
    BSP_MOUSE_LEFT_PRESSED     - Mouse Button left pressed (hold pressed)
    BSP_MOUSE_LEFT_RELEASE     - Mouse Button left release (button state changed to released)
    BSP_MOUSE_RIGHT_PRESS      - Mouse Button right press (button state changed to pressed)
    BSP_MOUSE_RIGHT_PRESSED    - Mouse Button right pressed (hold pressed)
    BSP_MOUSE_RIGHT_RELEASE    - Mouse Button right release (button state changed to released)
*/

typedef enum
{
    BSP_MOUSE_LEFT_PRESS     = 0x01u,
    BSP_MOUSE_LEFT_PRESSED   = 0x02u,
    BSP_MOUSE_LEFT_RELEASE   = 0x04u,
    BSP_MOUSE_RIGHT_PRESS    = 0x08u,
    BSP_MOUSE_RIGHT_PRESSED  = 0x10u,
    BSP_MOUSE_RIGHT_RELEASE  = 0x20u,
    BSP_MOUSE_ACTION_INVALID = 0x00
} r_bsp_hmi_MouseAaction_t;

/*******************************************************************************
  Section: Global API Functions
*/

/*******************************************************************************
  Function: R_BSP_HMI_Init

  Initialise the HMI 

  Parameters:
    void
  Returns:
    void     
*/

void R_BSP_HMI_Init(void); 


/*******************************************************************************
  Function: R_BSP_SetButtonCallback

  Assign an Action (callback) to a button event.  

  Parameters:
  BtnAction    - Button action (press, release, turn) See: <r_bsp_hmi_ButtonAaction_t>
  Callback     - Function poiter to the applications reaction  

  Returns:
  == E_OK    - init successfully
  != E_OK    - init failed
  
*/

r_Error_t R_BSP_SetButtonCallback(r_bsp_hmi_ButtonAaction_t BtnAction, 
                                          void (*Callback)(void));  


/*******************************************************************************
  Function: R_BSP_BtnAction

  Perform the Action (callback) assigned to a button event  
  The function is normally called by a System level handler, but 
  it can also be used to "simulate" a specific action.
    
  Parameters:
  BtnAction    - Button action (press, release, turn) See: <r_bsp_hmi_ButtonAaction_t>

  Returns:
  void
  
*/

void R_BSP_BtnAction(r_bsp_hmi_ButtonAaction_t BtnAction); 

/*******************************************************************************
  Function: R_BSP_HMI_GetLed

  Get the state (on or off) of a LED. 

  Parameters:
  LedNumber    - LED Id (numeric)

  Returns:
  == 1     - LED is on
  == 0     - LED is off 
  != (1|0) - wrong LED Id
  
*/

 int8_t R_BSP_HMI_GetLed(uint8_t LedNumber); 


/*******************************************************************************
  Function: R_BSP_HMI_SetLed

  Set the brightness (from 0: off to 100: on) of a LED. 
  
  The diming itself might not be present on the given board, 
  in this casse any value for Brightness other than 0 turns the LED on 

  Parameters:
  LedNumber    - LED Id (numeric)
  Brightness   - From 0 (off) to 100 (on)

  Returns:
  == E_OK    - success
  != E_OK    - wrong LED number 
  
*/

r_Error_t R_BSP_HMI_SetLed(uint8_t LedNumber, uint32_t Brightness); 

/*******************************************************************************
  Function: R_BSP_GetMouse

  Get position and button information from the mouse pointing device (e.g. touchscreen).

  Parameters:
  MouseDeviceId     - instance of the mouse pointing device
  cursorX           - X coordinate (return by parameter)
  cursorY           - Y coordinate (return by parameter)

  Returns:
  r_bsp_hmi_MouseAaction_t   - state of mouse buttons, see <r_bsp_hmi_MouseAaction_t>
*/

 r_bsp_hmi_MouseAaction_t R_BSP_GetMouse(uint32_t MouseDeviceId,
                                          uint32_t *cursorX, 
                                          uint32_t *cursorY);




#ifdef __cplusplus                                   
}                                                    
#endif

#endif /* BSP_HMI_API_H_ */

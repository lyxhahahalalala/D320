/*
****************************************************************************
PROJECT : BSP HMI Knob/Button
FILE    : $Id: r_bsp_hmi_knob.h 7640 2016-02-12 13:14:23Z florian.zimmermann $
============================================================================ 
DESCRIPTION
BSP Implementation D1M2 MANGO Board support sys functions - knob and button
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

#ifndef R_BSP_HMI_KNOB_H_
#define R_BSP_HMI_KNOB_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Defines for port assignment of Rotary Knob

    The Rotary Knob is connected to the following
    external interrupt port pins on MANGO board.
    
     Knob  direction A SW13-A   -> INTP0 on P0_1 (AF1 IN)
     Knob  direction B SW13-B   -> INTP1 on P0_2 (AF1 IN)
     Knob  center SW13-D        -> INTP5 on P0_0 (AF4 IN)
     
     This requires the DIP Switch settings on the MANGO board
         DSW29-6 on - Connect SW13-A (dir A)    to P0_1 (INTP0)
         DSW29-7 on - Connect SW13-B (dir B)    to P0_2 (INTP1)
         DSW29-8 on - Connect SW13-D (center)   to P0_0 (INTP5)
*/

#define BSP_KNOB_DIR_A_INTP    0
#define BSP_KNOB_DIR_A_PORT    0
#define BSP_KNOB_DIR_A_PIN     1
        
#define BSP_KNOB_DIR_B_INTP    1
#define BSP_KNOB_DIR_B_PORT    0
#define BSP_KNOB_DIR_B_PIN     2

#define BSP_KNOB_PRESS_INTP    5
#define BSP_KNOB_PRESS_PORT    0
#define BSP_KNOB_PRESS_PIN     0

/*******************************************************************************
  Defines for port assignment of Buttons

    All buttons are connected to external interrupts
     SW12-2 (center) -> INTP8 on P0_7 (AF1 IN)
     SW12-1 (up)     -> INTP2 on P0_3 (AF1 IN)
//   SW12-6 (down)   -> INTP5 on P0_4 (AF1 IN)    - don't use; use FOUT (CSCXFOUT on P0_4 AF3) instead
     SW12-3 (left)   -> INTP6 on P0_5 (AF4 IN)
     SW12-4 (right)  -> INTP7 on P0_6 (AF1 IN)
     
     This requires the DIP Switch settings on the MANGO board
         DSW29-1 on - Connect SW12-2 (center) to P0_7 (INTP8)
         DSW29-2 on - Connect SW12-1 (up)     to P0_3 (INTP2)
//       DSW29-3 on - Connect SW12-6 (down)   to P0_4 (INTP5) - Set DSW29-3 off -> use FOUT (CSCXFOUT on P0_4 AF3) instead
         DSW29-4 on - Connect SW12-3 (left)   to P0_5 (INTP6) - Remove JP30 2-3 (disable CAN RX)
         DSW29-5 on - Connect SW12-4 (right)  to P0_6 (INTP7)

    In fact the Navi button SW12 is mounted with 225 degree offset rotation by mistake.
    That means the up/down/left/right is rotated (CW) by 225 degree as following:
         up     -> bottom left
         down   ->    top right
         left   -> bottom right
         right  ->    top left
*/

/*
    USE_90DEG_ROTATED_BUTTONS
    This setting will reassign the physical up/down/left/right assignments
    by artificial exchaininging the directions in the way:
    90degCWrot by sw - Schematic name - actual hw reaction
             left	->  former up     -> bottom left
             right	->  former down   ->    top right
             down	->  former left   -> bottom right
             up     ->  former right  ->    top left
    This results in an effective rotation of SW12 by 45 degree. (CW 45 degree)
*/
/*  $$$: TODO this rotation option does not work for unknown reason, 
    do NOT set to 1 */
#define USE_90DEG_ROTATED_BUTTONS  1

/* disable Button down / right to allow usage as FOUT */
#if USE_90DEG_ROTATED_BUTTONS
    #define USE_FOUT_INSTEAD_BSP_BTN_RIGHT   1u
    #define USE_FOUT_INSTEAD_BSP_BTN_DOWN    0
#else
    #define USE_FOUT_INSTEAD_BSP_BTN_RIGHT   0
    #define USE_FOUT_INSTEAD_BSP_BTN_DOWN   1u
#endif  /* USE_90DEG_ROTATED_BUTTONS */


#if USE_90DEG_ROTATED_BUTTONS
    /* 90 degree CCW rotated Button assignment (225 CW - 90 degree; sw corrected) */
    #define BSP_BTN_CENTER_INTP 8u
    #define BSP_BTN_CENTER_PORT 0u
    #define BSP_BTN_CENTER_PIN  7u

    #define BSP_BTN_LEFT_INTP     2u    /* former up */
    #define BSP_BTN_LEFT_PORT     0u
    #define BSP_BTN_LEFT_PIN      3u

 #if USE_FOUT_INSTEAD_BSP_BTN_RIGHT != 1 
    #define BSP_BTN_RIGHT_INTP   5u     /* former down */
    #define BSP_BTN_RIGHT_PORT   0u
    #define BSP_BTN_RIGHT_PIN    4u
 #endif /* USE_FOUT_INSTEAD_BSP_BTN_RIGHT */

    #define BSP_BTN_DOWN_INTP   6u      /* former left */
    #define BSP_BTN_DOWN_PORT   0u
    #define BSP_BTN_DOWN_PIN    5u
    
    #define BSP_BTN_UP_INTP     7u      /* former right */
    #define BSP_BTN_UP_PORT     0u
    #define BSP_BTN_UP_PIN      6u
#else
    /* 225 degree CW rotated Button assignment (hw like) */
    #define BSP_BTN_CENTER_INTP 8u
    #define BSP_BTN_CENTER_PORT 0u
    #define BSP_BTN_CENTER_PIN  7u
    
    #define BSP_BTN_UP_INTP     2u
    #define BSP_BTN_UP_PORT     0u
    #define BSP_BTN_UP_PIN      3u
    
 #if USE_FOUT_INSTEAD_BSP_BTN_DOWN != 1
    #define BSP_BTN_DOWN_INTP   5u
    #define BSP_BTN_DOWN_PORT   0u
    #define BSP_BTN_DOWN_PIN    4u
 #endif /* USE_FOUT_INSTEAD_BSP_BTN_DOWN */
    
    #define BSP_BTN_LEFT_INTP   6u
    #define BSP_BTN_LEFT_PORT   0u
    #define BSP_BTN_LEFT_PIN    5u
    
    #define BSP_BTN_RIGHT_INTP  7u
    #define BSP_BTN_RIGHT_PORT  0u
    #define BSP_BTN_RIGHT_PIN   6u
#endif  /* USE_90DEG_ROTATED_BUTTONS */

/*******************************************************************************
  Section: Global functions
*/


/*******************************************************************************
  Function: R_BSP_HMI_InitRotaryKnob

    Init Rotary Knob

    Parameters:
    void

    Returns:
    void

*/
void R_BSP_HMI_InitRotaryKnob(void);


/*******************************************************************************
  Function: R_BSP_HMI_InitButton

    This function will init Buttons

    Parameters:
    void

    Returns:
    void
*/
void R_BSP_HMI_InitButton(void);

extern uint32_t _100ms;
extern uint32_t _10ms;

extern uint8_t main_init;



#ifdef __cplusplus
}
#endif

#endif /* R_BSP_HMI_KNOB_H_ */

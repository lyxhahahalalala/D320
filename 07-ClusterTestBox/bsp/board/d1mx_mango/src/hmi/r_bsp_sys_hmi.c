/*
****************************************************************************
PROJECT : BSP - HMI (SYS)
FILE    : $Id: r_bsp_sys_hmi.c 7262 2016-01-15 13:34:37Z michael.golczewski $
============================================================================
DESCRIPTION
BSP Implementation D1M2 MANGO Board
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

/*
  Section: BSP SYS HMI description 
  
  This is the board specific HMI implementation for the BSP. 
        
*/


/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"     /* Renesas basic types, e.g. uint32_t */

#include "r_tick_api.h"
#include "r_dev_api.h"
#include "r_gpio_api.h"

#include "r_bsp_hmi_api.h"
#include "r_bsp_hmi_sys.h"
#include "r_bsp_hmi_knob.h" /* BSP HMI Knob and Button */
#include "r_bsp_board_config.h"


/*******************************************************************************
  Section: Local Defines
*/

/*******************************************************************************
  Defines for port assignment of LEDs
  
  The 16 LEDs are connected to the following
  output port pins on MANGO board.
    LED0 port   P16_0
    LED1 port   P16_1
    [...]
    LED11 port  P16_11
    LED12 port  P17_0
    [...]
    LED15 port  P17_3

  This requires the Jumper settings (CN19 + CN20) on the MANGO board  
    Set Jumper CN19 1-2     to connect LED0  to P16_0
    Set Jumper CN19 3-4     to connect LED1  to P16_1
    [...]
    Set Jumper CN20 1-2     to connect LED8  to P16_8
    [...]
    Set Jumper CN20 15-16   to connect LED15 to P17_3
    
*/
#define LOC_LED0_PORT    16u
#define LOC_LED0_PIN      0u
#define LOC_LED1_PORT    16u
#define LOC_LED1_PIN      1u
#define LOC_LED2_PORT    16u
#define LOC_LED2_PIN      2u
#define LOC_LED3_PORT    16u
#define LOC_LED3_PIN      3u
#define LOC_LED4_PORT    16u
#define LOC_LED4_PIN      4u
#define LOC_LED5_PORT    16u
#define LOC_LED5_PIN      5u
#define LOC_LED6_PORT    16u
#define LOC_LED6_PIN      6u
#define LOC_LED7_PORT    16u
#define LOC_LED7_PIN      7u
#define LOC_LED8_PORT    16u
#define LOC_LED8_PIN      8u
#define LOC_LED9_PORT    16u
#define LOC_LED9_PIN      9u
#define LOC_LED10_PORT   16u
#define LOC_LED10_PIN    10u
#define LOC_LED11_PORT   16u
#define LOC_LED11_PIN    11u
#define LOC_LED12_PORT   17u
#define LOC_LED12_PIN     0u
#define LOC_LED13_PORT   17u
#define LOC_LED13_PIN     1u
#define LOC_LED14_PORT   17u
#define LOC_LED14_PIN     2u
#define LOC_LED15_PORT   17u
#define LOC_LED15_PIN     3u

/*******************************************************************************
  Section: Local Constants
*/

static const r_dev_PinConfig_t loc_PinConfig[] = 
/*      Port                Pin                Func      Dir      Feedback     Pull        OpenDrain   HiCurr   HiSpeed  InputType */
{
    /* LED */
    {LOC_LED0_PORT,       LOC_LED0_PIN,        0u, R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
    {LOC_LED1_PORT,       LOC_LED1_PIN,        0u, R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
    {LOC_LED2_PORT,       LOC_LED2_PIN,        0u, R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
    {LOC_LED3_PORT,       LOC_LED3_PIN,        0u, R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
    {LOC_LED4_PORT,       LOC_LED4_PIN,        0u, R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
    {LOC_LED5_PORT,       LOC_LED5_PIN,        0u, R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
    {LOC_LED6_PORT,       LOC_LED6_PIN,        0u, R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
    {LOC_LED7_PORT,       LOC_LED7_PIN,        0u, R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
    {LOC_LED8_PORT,       LOC_LED8_PIN,        0u, R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
    {LOC_LED9_PORT,       LOC_LED9_PIN,        0u, R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
    {LOC_LED10_PORT,      LOC_LED10_PIN,       0u, R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
    {LOC_LED11_PORT,      LOC_LED11_PIN,       0u, R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
    {LOC_LED12_PORT,      LOC_LED12_PIN,       0u, R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
    {LOC_LED13_PORT,      LOC_LED13_PIN,       0u, R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
    {LOC_LED14_PORT,      LOC_LED14_PIN,       0u, R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
    {LOC_LED15_PORT,      LOC_LED15_PIN,       0u, R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1}, 
    /* Button */
    {BSP_BTN_LEFT_PORT,   BSP_BTN_LEFT_PIN,    4u, R_DEV_PIN_IN,     0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
    {BSP_BTN_UP_PORT,     BSP_BTN_UP_PIN,      1u, R_DEV_PIN_IN,     0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
#if USE_FOUT_INSTEAD_BSP_BTN_DOWN != 1
    {BSP_BTN_DOWN_PORT,   BSP_BTN_DOWN_PIN,    1u, R_DEV_PIN_IN,     0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
#endif  /* USE_FOUT_INSTEAD_BSP_BTN_DOWN */
#if USE_FOUT_INSTEAD_BSP_BTN_RIGHT != 1
    {BSP_BTN_RIGHT_PORT,  BSP_BTN_RIGHT_PIN,   1u, R_DEV_PIN_IN,     0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
#endif  /* USE_FOUT_INSTEAD_BSP_BTN_RIGHT */
    {BSP_BTN_CENTER_PORT, BSP_BTN_CENTER_PIN,  1u, R_DEV_PIN_IN,     0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
    /* Knob */
    {BSP_KNOB_PRESS_PORT, BSP_KNOB_PRESS_PIN,  4u, R_DEV_PIN_IN,     0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
    {BSP_KNOB_DIR_A_PORT, BSP_KNOB_DIR_A_PIN,  1u, R_DEV_PIN_IN,     0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
    {BSP_KNOB_DIR_B_PORT, BSP_KNOB_DIR_B_PIN,  1u, R_DEV_PIN_IN,     0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
    /* delimiter - do not remove */
    {0u, R_DEV_PIN_LAST,0u,R_DEV_PIN_OUT,0u,R_DEV_PIN_PULLNO,     0u,      0u,         0u,    R_DEV_PIN_CMOS1}  
};

/*******************************************************************************
  Section: Local Variables 
*/

static uint8_t loc_Brightness[BSP_MAX_LED];

/*******************************************************************************
  Section: Local Functions
*/



/*******************************************************************************
  Section: Global BSP HMI SYS Functions

*/

/*******************************************************************************
  Function: R_BSP_HMI_Init
  
  See: r_bsp_hmi_sys.h for details    
*/
void R_BSP_SYS_HMI_Init(void) 
{
    //uint8_t lednr;
    //R_DEV_PinInit (loc_PinConfig); 
    
    /* LED 0..15 set default state (on)*/
    //for (lednr=0; lednr<16; lednr++)
    {
        //R_BSP_SYS_HMI_SetLed(lednr, 100u);
    }
    
    /* Init Rotary - Knob */
    R_BSP_HMI_InitRotaryKnob();
    
    /* Init Buttons */
    //R_BSP_HMI_InitButton();
}

/*******************************************************************************
  Function: R_BSP_HMI_SetLed
  
  See: r_bsp_hmi_sys.h for details    
*/
r_Error_t R_BSP_SYS_HMI_SetLed(uint8_t LedNumber, uint32_t Brightness)
{
    uint32_t port;
    uint32_t pin;
    if (LedNumber <= 11u)
    {
        /* LED is on P16 */
        port = 16;
        pin = (LedNumber - 0u);
        if (Brightness == 0u) 
        {
            /* switch LOC_LED[11..0] off */
            R_GPIO_WritePin(port, pin, 1);
        } 
        else
        {
            /* switch LOC_LED[11..0] on */
            /* dimming not (yet) implemented, the first 
            one in need it the first one to complete this function */
            R_GPIO_WritePin(port, pin, 0);
        }
        loc_Brightness[LedNumber] = Brightness; 
        return R_ERR_OK;
    }
    
    if ((LedNumber >= 12u) && (LedNumber < BSP_MAX_LED))
    {
        /* LED is on P17 */
        port = 17;
        pin = (LedNumber - 12u);
        if (Brightness == 0u) 
        {
            /* switch LOC_LED[15..12] off */
            R_GPIO_WritePin(port, pin, 1);
        } 
        else
        {
            /* switch LOC_LED[15..12] on */
            /* dimming not (yet) implemented, the first 
            one in need it the first one to complete this function */
            R_GPIO_WritePin(port, pin, 0);
        }
        loc_Brightness[LedNumber] = Brightness; 
        return R_ERR_OK;
    }
    return R_NG;
}

/*******************************************************************************
  Function: R_BSP_HMI_GetLed
  
  See: r_bsp_hmi_sys.h for details    
*/
int8_t R_BSP_SYS_HMI_GetLed(uint8_t LedNumber) 
{
    if (LedNumber < BSP_MAX_LED) 
    {
        return loc_Brightness[LedNumber];
    }
    else
    {
        return (-1);
    }
}


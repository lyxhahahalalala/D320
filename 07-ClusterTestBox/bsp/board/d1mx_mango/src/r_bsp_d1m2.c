/*
****************************************************************************
PROJECT : VLIB - BSP
FILE    : $Id: r_bsp_d1m2.c 7182 2016-01-08 08:49:00Z tobyas.hennig $
============================================================================
DESCRIPTION
BSP Implementation D1M2 / D1M2H MANGO Board
============================================================================
                            C O P Y R I G H T
============================================================================
                           Copyright (c) 2015
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
  Section: BSP Description

    The "D1M2-Mango Board" Support Package is intended for the
    following board combination:
    
    BSP Board Name: "D1M2-MANGO Board"
        + D1M2 or D1M2H MANGO Adapter board
        + D1x MANGO Main board
*/


/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"     /* Renesas basic types, e.g. uint32_t */

#include "r_gpio_api.h"
#include "r_dev_api.h"
#include "r_tick_api.h"
#include "r_bsp_api.h"


#ifdef USE_BSP_STDIO
#include "r_bsp_stdio_api.h"
#endif

#ifdef USE_BSP_HMI
#include "r_bsp_hmi_api.h"
#endif

#ifdef USE_BSP_GFX
#include "r_bsp_gfx_api.h"
#include "r_bsp_gfx_disp_init.h"    /* CSI init for NEC display */
#include "r_bsp_gfx_vi_saa71_init.h"    /* SAA71xx video input converter init */
#include "r_bsp_gfx_hdmi_out.h"     /* init HDMI output converters */
#include "r_bsp_gfx_iis_audio_dac.h"    /* IIS Audio Digital Analog converter init */
#endif /* USE_BSP_GFX */


/*******************************************************************************
  Section: Local Defines
*/

/* Module reset control register */
#define LOC_MRSTC           0xFFF8F400u
#define LOC_PROTSMRST		0xFFF8F484u
#define LOC_PROTCMDMRST     0xFFF8F480u

/* Module reset control register bits */
#define MRSTC_XC0RES		((uint32_t)(1ul<<13u))
#define MRSTC_SDRB0RES		((uint32_t)(1ul<<12u))
#define MRSTC_MLBB0RES      ((uint32_t)(1ul<<11u))
#define MRSTC_LCBI0RES      ((uint32_t)(1ul<<10u))
#define MRSTC_SG4RES        ((uint32_t)(1ul<<9u)) 
#define MRSTC_SG3RES        ((uint32_t)(1ul<<8u)) 
#define MRSTC_SG2RES        ((uint32_t)(1ul<<7u)) 
#define MRSTC_SG1RES        ((uint32_t)(1ul<<6u)) 
#define MRSTC_SG0RES        ((uint32_t)(1ul<<5u)) 
#define MRSTC_ADCE0RES      ((uint32_t)(1ul<<4u)) 
#define MRSTC_ISM0RES       ((uint32_t)(1ul<<3u)) 
#define MRSTC_ETNB0RES      ((uint32_t)(1ul<<2u)) 
#define MRSTC_VDCE1RES      ((uint32_t)(1ul<<1u)) 
#define MRSTC_VDCE0RES      ((uint32_t)(1ul<<0u)) 


/*******************************************************************************
  Section: Local Constants 
*/


/*******************************************************************************
  Array loc_ClkSelectionCfg 
  
  contains the clock selectors configuration 
*/
static const r_dev_ClkSelConfig_t loc_ClkSelectionCfgD1M2[] = 
/*   Cks                   SrcId                Div  StpReqMsk      */
{
    {R_DEV_CKS_OSTM ,    R_DEV_CKS_SRC_MOSC,   2,   0u},
    {R_DEV_CKS_CLKJIT ,  R_DEV_CKS_SRC_DIV,    6,   0u},
    {R_DEV_CKS_PLLFIX ,  R_DEV_CKS_SRC_PLL1,   4,   0u},
    {R_DEV_CKS_CLKFIX ,  R_DEV_CKS_SRC_DIV,   12,   0u},
    {R_DEV_CKS_CPU    ,  R_DEV_CKS_SRC_PLL1,   2,   0u},
    {R_DEV_CKS_PLL2IN ,  R_DEV_CKS_SRC_MOSC,   1,   0u},
    {R_DEV_CKS_SFMA ,    R_DEV_CKS_SRC_PLL1,   3,   0u},
    

    /* delimiter - do not remove */
    {R_DEV_CKS_LAST  ,     R_DEV_CKS_SRC_MOSC,  2,   0u}
}; 

/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Section: Local Functions
*/

/*******************************************************************************
  Function: loc_InitClocksD1M2

  This function just contains the starting (global) clock config for the BSP. 
  Its purpose is to write the given setting in the structure passed as parameter.  
  
  As the clock config is done in low init, the globally stored data is cleared 
  afterwards. So, this function has to be called during and after the low init 
  in order to have the right settings afterwards. And becasue we want to keep data 
  consistant and do not have two configs, this funbction is used.  
  
  Parameters:
  ClkGenCfg - pointer to the Clock generator configuration, 
              see <r_dev_ClkGenConfig_t>
  
  Returns:
  none

*/
const r_dev_ClkSelConfig_t* loc_InitClocksD1M2(r_dev_ClkGenConfig_t * Config);
const r_dev_ClkSelConfig_t* loc_InitClocksD1M2(r_dev_ClkGenConfig_t * Config)
{
    /* Init Device Clocks  */
    Config->MainOsc.FrequencyHz          = R_DEV_MOSC_FRQ_HZ;
    Config->MainOsc.StabiTimeNs          = 400000;
    Config->MainOsc.Gain                 = 3;
    Config->MainOsc.Running              = 1;
    Config->MainOsc.StpReqMsk            = 1;
    Config->SubOsc.FrequencyHz           = 32768;
    Config->SubOsc.Running               = 1;
    Config->RoscStpReqMsk                = 1;                   
                  
    Config->Pll[0].Mode                  = R_DEV_PLL_SSCG;
    Config->Pll[0].DithMode              = R_DEV_PLL_DITH_CENTER;
    Config->Pll[0].DithRange             = 3;
    Config->Pll[0].ModFreq               = 50;
    Config->Pll[0].FrequencyHz           = 480 * 1000 * 1000;
    Config->Pll[0].StabiTimeNs           = 40000;
    Config->Pll[0].StpReqMsk             = 0;
                                            
    Config->Pll[1].Mode                  = R_DEV_PLL_ON;
    Config->Pll[1].DithMode              = R_DEV_PLL_DITH_FIXED;
    Config->Pll[1].DithRange             = 0;
    Config->Pll[1].ModFreq               = 0;
    Config->Pll[1].FrequencyHz           = 960 * 1000 * 1000;
    Config->Pll[1].StabiTimeNs           = 40000;
    Config->Pll[1].StpReqMsk             = 0;
                                            
    Config->Pll[2].Mode                  = R_DEV_PLL_ON;
    Config->Pll[2].DithMode              = R_DEV_PLL_DITH_FIXED;
    Config->Pll[2].DithRange             = 0;
    Config->Pll[2].ModFreq               = 0;
    Config->Pll[2].FrequencyHz           = 480 * 1000 * 1000;
    Config->Pll[2].StabiTimeNs           = 40000;
    Config->Pll[2].StpReqMsk             = 0;
    
    Config->BusEnable[R_DEV_CLK_XCC]     = 1;
    Config->BusEnable[R_DEV_CLK_PLL0]    = 1;
    Config->BusEnable[R_DEV_CLK_PLL1]    = 1;
    Config->BusEnable[R_DEV_CLK_PLL2]    = 1;
    Config->BusEnable[R_DEV_CLK_PLL0PIX] = 1;
    Config->BusEnable[R_DEV_CLK_SDRB]    = 1;
    Config->BusEnable[R_DEV_CLK_ETNBP]   = 1;
    Config->BusEnable[R_DEV_CLK_ETNBXCC] = 1;
    Config->BusEnable[R_DEV_CLK_MLBP]    = 0;
    Config->BusEnable[R_DEV_CLK_MLBXCC]  = 0;
    Config->BusEnable[R_DEV_CLK_RSCANP]  = 0;
    
    return (loc_ClkSelectionCfgD1M2);    
}


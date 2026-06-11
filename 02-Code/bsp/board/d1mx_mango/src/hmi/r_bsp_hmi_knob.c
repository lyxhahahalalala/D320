/*
****************************************************************************
PROJECT : BSP HMI Knob/Button
FILE    : $Id: r_bsp_hmi_knob.c 4988 2015-03-30 13:06:17Z golczewskim $
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

/*******************************************************************************
  Section: Includes
*/

#include "iodefine.h"
#include "r_typedefs.h"
#include "r_tauj_api.h"
#include "r_bsp_hmi_knob.h"
#include "network_layer.h"




/*******************************************************************************
  Section: Local variable 
*/


/*******************************************************************************
  Section: Local Functions
*/
static void locTauj0ch0Isr(void);
static void locTauj0ch1Isr(void);
static void locTauj0ch2Isr(void);
static void locTauj0ch3Isr(void);


uint32_t _16us=0, _1ms=0, _10ms=0, _100ms=0;

extern void ptmr_IncTick( void );
extern void Interrupt16us(void);
extern void Interrupt1ms(void);
extern void Interrupt10ms(void);
extern void Interrupt100ms(void);


/*
  Configure TAUJ to generate interrupt every 500uSec
*/
static void locConfigTauj(void)
{
    r_tauj_Error_t error = R_TAUJ_ERR_OK;
    uint8_t Inst = 0;
	
    error = R_TAUJ_Init(0);
	
    /* set the clock period for CK0 (TPS) */
    error += R_TAUJ_SetClockPeriod(Inst, R_TAUJ_CK_0,  R_TAUJ_TIME_INTERVAL_0);
	
    /* select the clock source for the timer channel CK_0 - 3 in the CMOR */
    error += R_TAUJ_SetClkSource(Inst, (r_tauj_Channel_t)0, R_TAUJ_CK_0);
	error += R_TAUJ_SetClkSource(Inst, (r_tauj_Channel_t)1, R_TAUJ_CK_0);
    error += R_TAUJ_SetClkSource(Inst, (r_tauj_Channel_t)2, R_TAUJ_CK_0);
    error += R_TAUJ_SetClkSource(Inst, (r_tauj_Channel_t)3, R_TAUJ_CK_0);
	
    /* select the mode for the timer channel (CMOR) */
    error += R_TAUJ_SetMode(Inst, (r_tauj_Channel_t)0, R_TAUJ_INTERVAL_TIMER_MODE);
    error += R_TAUJ_SetMode(Inst, (r_tauj_Channel_t)1, R_TAUJ_INTERVAL_TIMER_MODE);
    error += R_TAUJ_SetMode(Inst, (r_tauj_Channel_t)2, R_TAUJ_INTERVAL_TIMER_MODE);
    error += R_TAUJ_SetMode(Inst, (r_tauj_Channel_t)3, R_TAUJ_INTERVAL_TIMER_MODE);
	
	// R_TAUJ_SetTinEdge(Inst, (r_tauj_Channel_t)0, R_TAUJ_BOTH_HIGH_WIDTH);
	// R_TAUJ_SetTinEdge(Inst, (r_tauj_Channel_t)1, R_TAUJ_BOTH_HIGH_WIDTH);
	// R_TAUJ_SetTinEdge(Inst, (r_tauj_Channel_t)2, R_TAUJ_BOTH_HIGH_WIDTH);
	// R_TAUJ_SetTinEdge(Inst, (r_tauj_Channel_t)3, R_TAUJ_BOTH_HIGH_WIDTH);
	
    /* Write the count value in the CDR */
	error += R_TAUJ_SetPeriod(Inst, (r_tauj_Channel_t)0, 160 * 0.667);        //16us
    error += R_TAUJ_SetPeriod(Inst, (r_tauj_Channel_t)1, 10000 * 0.6667);     //1ms
    error += R_TAUJ_SetPeriod(Inst, (r_tauj_Channel_t)2, 100000 * 0.66667);	  //10ms
    error += R_TAUJ_SetPeriod(Inst, (r_tauj_Channel_t)3, 1000000 * 0.666667); //100ms
	
    /* Disable the output (TOE) */
    error += R_TAUJ_OutputDisable(Inst, R_TAUJ_CHANNEL_0);
	error += R_TAUJ_OutputDisable(Inst, R_TAUJ_CHANNEL_1);
    error += R_TAUJ_OutputDisable(Inst, R_TAUJ_CHANNEL_2);
    error += R_TAUJ_OutputDisable(Inst, R_TAUJ_CHANNEL_3);
	
    /* Enable interrupt */
    R_TAUJ_EnableInt(0, R_TAUJ_INT_CH0);
	R_TAUJ_EnableInt(0, R_TAUJ_INT_CH1);
    R_TAUJ_EnableInt(0, R_TAUJ_INT_CH2);
    R_TAUJ_EnableInt(0, R_TAUJ_INT_CH3);
	
    /* Set ISR routine */
    R_TAUJ_SetIsrCallback(0, R_TAUJ_INT_CH0, &locTauj0ch0Isr);
	R_TAUJ_SetIsrCallback(0, R_TAUJ_INT_CH1, &locTauj0ch1Isr);
    R_TAUJ_SetIsrCallback(0, R_TAUJ_INT_CH2, &locTauj0ch2Isr);
    R_TAUJ_SetIsrCallback(0, R_TAUJ_INT_CH3, &locTauj0ch3Isr);
	
	R_TAUJ_Stop(0, R_TAUJ_CHANNEL_0);
	// R_TAUJ_Start(0, R_TAUJ_CHANNEL_0);
	R_TAUJ_Start(0, R_TAUJ_CHANNEL_1);
	R_TAUJ_Start(0, R_TAUJ_CHANNEL_2);
	R_TAUJ_Start(0, R_TAUJ_CHANNEL_3);
	
}

/*******************************************************************************
  Function: locTauj0ch0Isr

   Every edge on inputA re-starts the corresponding timer channel. 
   This is the tiner ISR. 
   Input Debouncing: 
   When this ISR is called that means the input was stable for the defined timer
   And can be processed - Button action call  

    Parameters:
    void

    Returns:
    void
 */

static void locTauj0ch0Isr (void)
{
	Interrupt16us();
	_16us++;
}
static void locTauj0ch1Isr (void)
{
	ptmr_IncTick();
	network_main();
	Interrupt1ms();
	_1ms++;
}
static void locTauj0ch2Isr (void)
{
 	Interrupt10ms();
	_10ms++;
}
static void locTauj0ch3Isr (void)
{
	Interrupt100ms();
	_100ms++;

}

void R_BSP_HMI_InitRotaryKnob(void)
{
    locConfigTauj(); 
}




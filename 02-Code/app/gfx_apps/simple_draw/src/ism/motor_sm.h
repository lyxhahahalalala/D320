/*
****************************************************************************
PROJECT : ISM Test APP
FILE    : $Id: motor_sm.h 4975 2015-03-27 16:22:34Z tobyas.hennig $
============================================================================ 
DESCRIPTION
Stepper Motor App Functions
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

/*******************************************************************************
  Title: motor_sm.h

  Definition for the stepper motor app functions.
*/

#ifndef _MOTOR_SM_H_
#define _MOTOR_SM_H_

#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************
  Section: Global Functions
*/


/***********************************************************
  Function: StepperMotorInit

  Setup the stepper motor system.

  Parameters:
  uint8_t EnableZPD R_FALSE disable, R_TRUE enable
  uint8_t EnabledChannels 0..5

  Returns:
  void
*/
void StepperMotorInit(uint8_t EnableZPD, uint8_t EnabledChannels);


/***********************************************************
  Function: StepperMotorDeInit

  Deinitialize the stepper motor system.

  Parameters:
  void

  Returns:
  void
*/
void StepperMotorDeInit(void);


/***********************************************************
  Function: StepperMotorUpdate
  
  StepperMotor update
  
  updates the stepper motor positions
  by sending control commands to the stepper motor driver.
  
  Parameters:
  none   - Not used.
  
  Returns:
  void
*/
void StepperMotorUpdate(void);

void StepperMotorZero(void);
void Stepper_Motor_Init(void);
void StepperMotorback(void);
void StepperMotormiddle(void);
void needleTesting(void);

extern void ClrWdt(void);


void SetMotSm_SpdTgtPos(float target_value);
void SetMotSm_CurrTgtPos(float target_value);


#ifdef __cplusplus
}
#endif

#endif /* _MOTOR_SM_H_ */

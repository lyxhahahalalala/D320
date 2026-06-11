/*
****************************************************************************
PROJECT : ISM Test APP
FILE    : $Id: motor_sm.c 4975 2015-03-27 16:22:34Z tobyas.hennig $
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
  Title: motor_sm.c

  Implementation of stepper motor app functions.
*/


/***********************************************************
  Title: Stepper Motor Application Module
*/

/***********************************************************
  Section: Includes
*/

#include "r_typedefs.h"     /* Renesas basic types, e.g. uint32_t */
#include "r_config_ism.h"
#include "r_ism_api.h"
// #include "carmodel.h"
#include "r_tick_api.h"
#include "r_bsp_stdio_api.h" 
#include "motor_sm.h"

/***********************************************************
  Section: Local Defines
*/

/* default is to run two motors (0 & 1),
   o it can be limited to run only one motor (0)
   o or extended to all six motors (0 to 6) */
#define APP_CLUSTER_ONLY_RUN_SM0        1
#define DEG       242
#define DEGSPEED  242

/* if not defined, will assume JCI Sonceboz motor */
#define APP_CLUSTER_USE_CONTI2000_SM    1


/***********************************************************
  Section: Local Variables
*/
static unsigned char ISMTest_EnabledChannels;
static uint32_t      TargetPositionRange_u32[ R_ISM_CHANNELS ];


/***********************************************************
  Section: Local Functions
*/


/***********************************************************
  Function: locUpdateStepperMotorTarget

  Takes the new angel and commands the stepper motor
  
  Parameters:
  void
  Returns:
  void
*/
static void locUpdateStepperMotorTarget (uint32_t CurTargetPosition_u32,
                                  uint8_t CurrentChannel_u08,
                                  uint8_t ChannelEnableMask_u08)
{
    uint8_t UnitNumber_u08;
    uint8_t ISM_Status_bit = R_ISM_ERR_OK;

    for( UnitNumber_u08=0; UnitNumber_u08<R_ISM_MACRO_NUM; UnitNumber_u08++ )
    {
        if( ( 1 << CurrentChannel_u08 ) & ChannelEnableMask_u08 )
        {
			/* Disable REACHED interrupts for the currently signalled channel */
			ISM_Status_bit &= R_ISM_SetInterrupt( UnitNumber_u08,
                                                  ( 1 << CurrentChannel_u08 ),
                                                  0,   /* DONE  Interrupt disable */
                                                  0,   /* ZPDAD Interrupt disable */
                                                  0 ); /* Enable Flag */

		#if 0
			/* compensate for the permanent deviation of the algorithm */
			ISM_Status_bit &= R_ISM_CompensateDeviation( &CurTargetPosition_u32[ CurrentChannel_u08 ] );
		#endif
			/* Set the calculated position */
			ISM_Status_bit &= R_ISM_SetCMTargetPos( UnitNumber_u08,
                                                    CurrentChannel_u08,
                                                    CurTargetPosition_u32);
			
			/* Enable REACHED interrupts for the currently moved channel */
			ISM_Status_bit &= R_ISM_SetInterrupt( UnitNumber_u08,
                                                  ( 1 << CurrentChannel_u08 ),
                                                  0,   /* DONE  Interrupt disable */
                                                  0,   /* ZPDAD Interrupt disable */
                                                  1 ); /* Enable Flag */
        }
    }
}


/***********************************************************
  Section: Global Functions

  See: <motor_sm.h>
*/


/***********************************************************
  Function: StepperMotorInit
*/

void StepperMotorInit(uint8_t EnableZPD, uint8_t EnabledChannels)
{
    unsigned char status;
    unsigned char count;
	
    status = R_ISM_ERR_OK;
	
    /* setting the channels to run */
    ISMTest_EnabledChannels = EnabledChannels;
	
    /* Init the ISM (stepper motor) controler */
    if (R_ISM_ERR_OK != R_ISM_Init(R_ISM_UNIT))
    {
        while(1)
        {
            ; /* ISM init error */
        }
    }
    
    for( count=0; count<R_ISM_MACRO_NUM; count++ )
    {
		/* Set to motor used in demo.  */
	#ifdef APP_CLUSTER_USE_CONTI2000_SM
		// status = R_ISM_SetMotor_JCISONCEBOZ6403( count );
		status = R_ISM_SetMotor_CONTI2000( count );
	#else
		status = R_ISM_SetMotor_JCISONCEBOZ6403( count );
		status = R_ISM_SetMotor_CHINASTANDARD(count);
	#endif
    }
	
    /* ZPD */
    /* the problem with the current code is it hangs on until ZPD is done */
    for( count=0; count<R_ISM_MACRO_NUM; count++ )
    {
        status &= R_ISM_Reset( count );
        status &= R_ISM_DownloadCMParamset( count,
                                            ISMTest_EnabledChannels );
        status &= R_ISM_SetupCMChannel( count,
                                        ISMTest_EnabledChannels, /* all configured channels */
                                        1 );                     /* clear target position */
		
        if(R_FALSE != EnableZPD)
        {
            /* the problem with the current code is it hangs on until ZPD is done */
            status &= R_ISM_ZPDCMChannel( count,
                                        ISMTest_EnabledChannels ); /* all configured channels */
        } 
    }
	
    for( count = R_ISM_CHANNEL0; count < R_ISM_CHANNELS; count++ )
    {
		/* All interrupts aren't acknowledged */
		switch( R_ISM_A_CURPARAMSET[ count ]->DEFAULTPRECISION )
        {
        case R_ISM_PRECISION_STD:
			TargetPositionRange_u32[ count ] = 
			R_ISM_A_CURPARAMSET[ count ]->TARGETRANGE_LOW;
			break;
        case R_ISM_PRECISION_HI:
			TargetPositionRange_u32[ count ] = 
			R_ISM_A_CURPARAMSET[ count ]->TARGETRANGE_HIGH;
			break;
        default:
			status &=  R_ISM_ERR_NG ;
			break;
        }
    }
}


/***********************************************************
  Function: StepperMotorDeInit
*/

void StepperMotorDeInit(void)
{
    /* DeInit the ISM (stepper motor) controler */
    R_ISM_DeInit(R_ISM_UNIT);
}
void needleTesting(void)
{
	StepperMotorZero();
	ClrWdt();
	R_TICK_WaitMS(0, 500);
	ClrWdt();
	R_TICK_WaitMS(0, 500);
	ClrWdt();
	StepperMotorback();
	R_TICK_WaitMS(0, 500);
	ClrWdt();
	ClrWdt();
	R_TICK_WaitMS(0, 500);
	
	#if 0
	StepperMotorZero();
	ClrWdt();
	R_TICK_WaitMS(0, 500);
	R_TICK_WaitMS(0, 500);
	ClrWdt();
	StepperMotorback();
	R_TICK_WaitMS(0, 500);
	ClrWdt();
	ClrWdt();
	R_TICK_WaitMS(0, 500);
	#endif
	
}

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
static float motSm_Speed;
static float motSm_Current;
void StepperMotorUpdate(void)
{
    static double rpm_target_position = 1000;
    static double oil_target_position = 1000;
    double new_rpm_target_position;
    double new_oil_target_position;
    
	/* scale to motor steps per revolution and only 270 degree movement */
	
    new_rpm_target_position = 120 - motSm_Speed;
    new_rpm_target_position = TargetPositionRange_u32[R_ISM_CHANNEL1] * new_rpm_target_position / 120 * DEGSPEED / 90;
	// if ((uint32_t)new_rpm_target_position != (uint32_t)rpm_target_position)
    {
		StepperMotorSetSpeed(600.0);
        rpm_target_position = new_rpm_target_position;
        locUpdateStepperMotorTarget((uint32_t)rpm_target_position, R_ISM_CHANNEL1, ISMTest_EnabledChannels );
    }
	
	new_oil_target_position = motSm_Current;
    new_oil_target_position = TargetPositionRange_u32[R_ISM_CHANNEL5] * new_oil_target_position / 1200 * DEG / 90;
    // if ((uint32_t)new_oil_target_position != (uint32_t)oil_target_position)
    {
		StepperMotorSetSpeed(600.0);
        oil_target_position = new_oil_target_position;
        locUpdateStepperMotorTarget((uint32_t)oil_target_position, R_ISM_CHANNEL5, ISMTest_EnabledChannels );
		
    }

}
void StepperMotorZero(void)
{
    double new_speed_target_position;
    double new_rpm_target_position;
	
	/* scale to motor steps per revolution and only 270 degree movement */
	
    StepperMotorSetSpeed(2500.0);
	
    new_rpm_target_position = 120 - 0;
    new_rpm_target_position = TargetPositionRange_u32[R_ISM_CHANNEL1] * new_rpm_target_position / 120 * DEGSPEED / 90;
    locUpdateStepperMotorTarget((uint32_t)new_rpm_target_position, R_ISM_CHANNEL1, ISMTest_EnabledChannels );
	
    new_speed_target_position = 1200;
    new_speed_target_position = TargetPositionRange_u32[R_ISM_CHANNEL5] * new_speed_target_position / 1200 * DEG / 90;
    locUpdateStepperMotorTarget((uint32_t)new_speed_target_position, R_ISM_CHANNEL5, ISMTest_EnabledChannels );
	
}
void StepperMotorback(void)
{
    double new_speed_target_position;
    double new_rpm_target_position;
	
	/* scale to motor steps per revolution and only 270 degree movement */
	
    StepperMotorSetSpeed(2500.0);
	
    new_rpm_target_position = 0;
    new_rpm_target_position = TargetPositionRange_u32[R_ISM_CHANNEL1] * new_rpm_target_position / 120 * DEGSPEED / 90;
    locUpdateStepperMotorTarget((uint32_t)new_rpm_target_position, R_ISM_CHANNEL1, ISMTest_EnabledChannels );
    
    new_speed_target_position = 0;
    new_speed_target_position = TargetPositionRange_u32[R_ISM_CHANNEL5] * new_speed_target_position / 1200 * DEG / 90;
    locUpdateStepperMotorTarget((uint32_t)new_speed_target_position, R_ISM_CHANNEL5, ISMTest_EnabledChannels );
	
}
void StepperMotormiddle(void)
{
    double new_speed_target_position;
    double new_rpm_target_position;
	
	/* scale to motor steps per revolution and only 270 degree movement */
	
    StepperMotorSetSpeed(2500.0);
    
    new_rpm_target_position = 120;
    new_rpm_target_position = TargetPositionRange_u32[R_ISM_CHANNEL1] * new_rpm_target_position / 120 * DEGSPEED / 90;
    locUpdateStepperMotorTarget((uint32_t)new_rpm_target_position, R_ISM_CHANNEL1, ISMTest_EnabledChannels );
    
    new_speed_target_position = 600;
    new_speed_target_position = TargetPositionRange_u32[R_ISM_CHANNEL5] * new_speed_target_position / 1200 * DEG / 90;
    locUpdateStepperMotorTarget((uint32_t)new_speed_target_position, R_ISM_CHANNEL5, ISMTest_EnabledChannels );
	
}

void Stepper_Motor_Init(void)
{
	// CarModel_Init();
	// StepperMotorInit(R_FALSE, R_ISM_CHANNEL_EN0 | R_ISM_CHANNEL_EN1 | R_ISM_CHANNEL_EN2 | R_ISM_CHANNEL_EN3  | R_ISM_CHANNEL_EN4  | R_ISM_CHANNEL_EN5);
	StepperMotorInit(R_FALSE, R_ISM_CHANNEL_EN1 | R_ISM_CHANNEL_EN4 | R_ISM_CHANNEL_EN5);
	
	ClrWdt();
	
}

void SetMotSm_SpdTgtPos(float target_value)
{
	motSm_Speed = target_value;
}

void SetMotSm_CurrTgtPos(float target_value)
{
	motSm_Current = target_value;
}




/*
****************************************************************************
PROJECT : ism driver
FILE    : $Id: r_ism_api.h 4975 2015-03-27 16:22:34Z tobyas.hennig $
============================================================================ 
DESCRIPTION
Driver for ISM macro
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

#ifndef ISM_API_H_
#define ISM_API_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: ISM API

  An application using ISM should include this header file.
*/


/*******************************************************************************
  Section: Global Defines
*/

#define R_ISM_UNIT     0             /* ISM macro selection */

/* ISM Globals */

#define R_ISM_CHANNELS       6

/* Settings definitions */

#define R_ISM_PRECISION_STD ( 0 )
#define R_ISM_PRECISION_HI  ( 1 )

/* ISM Channel control values */

#define R_ISM_CHANNEL0      ( 0 )   
#define R_ISM_CHANNEL1      ( 1 )   
#define R_ISM_CHANNEL2      ( 2 )   
#define R_ISM_CHANNEL3      ( 3 )   
#define R_ISM_CHANNEL4      ( 4 )   
#define R_ISM_CHANNEL5      ( 5 ) 

#define R_ISM_CHANNEL_EN0   ( 0x01 )
#define R_ISM_CHANNEL_EN1   ( 0x02 )
#define R_ISM_CHANNEL_EN2   ( 0x04 )
#define R_ISM_CHANNEL_EN3   ( 0x08 )
#define R_ISM_CHANNEL_EN4   ( 0x10 )
#define R_ISM_CHANNEL_EN5   ( 0x20 )
#define R_ISM_CHANNEL_ENALL ( 0x3F )
#define R_ISM_CHANNELS_OFF  ( 0x00 )
#define R_ISM_GLOBAL_EN     ( 0xFF )



/*******************************************************************************
  Section: Global Types
*/


/*******************************************************************************
  Enum: r_ism_Error_t

  ISM macro driver error code.
  
  If an error occures these enums give information about the
  reason.

  Values:
  R_ISM_ERR_OK           - No error
  R_ISM_ERR_NG           - Unspecified error
  R_ISM_ERR_RANGE        - Unit do not exist
  R_ISM_ERR_PORTINIT     - ISM port init failed
  R_ISM_ERR_LAST         - Delimiter 
*/

typedef enum
{
    R_ISM_ERR_OK         = R_TRUE,
    R_ISM_ERR_NG         = R_FALSE,
    R_ISM_ERR_RANGE      = 0x02u,
    R_ISM_ERR_PORTINIT,
    R_ISM_ERR_LAST
} r_ism_Error_t;


/*******************************************************************************
  Enum: r_ism_Interrupt_t

  ISM macro interrupt type. List of all available interrupts 
  for this macro

  Values:
  R_ISM_INT_ISM0REACHED   - reached Interrupt
  R_ISM_INT_ISM0DONE      - done    Interrupt
  R_ISM_INT_ISM0ZPDAD     - zpdad   Interrupt
  R_ISM_INT_ISM0ZPD       - zpd     Interrupt
  R_ISM_INT_NUM        - Delimiter 
*/

typedef enum
{
    R_ISM_INT_ISM0REACHED = 0x01u,
    R_ISM_INT_ISM0DONE    = 0x02u,
    R_ISM_INT_ISM0ZPDAD   = 0x04u,
    R_ISM_INT_ISM0ZPD     = 0x08u,
    R_ISM_INT_ISM_ALL     = (R_ISM_INT_ISM0REACHED | R_ISM_INT_ISM0DONE
                             | R_ISM_INT_ISM0ZPDAD | R_ISM_INT_ISM0ZPD),
    R_ISM_INT_NUM
} r_ism_Interrupt_t;



/*............... The ISM motor characteristics ............................*/

typedef struct r_ism_motor_t {
    
    uint32_t PDF;   /* Calculator Parameters */
    uint32_t PAL;
    uint32_t PDL;
    uint32_t PMS;
    uint32_t PHC;
    uint32_t PS1;
    uint32_t PS2;
    uint32_t PS3;
    uint32_t PS4;

    uint8_t CHANNELTYPE;    /* Determines equality of channels */

    uint32_t TARGETRANGE_LOW;     /* Target Position Ranges */
    uint32_t TARGETRANGE_HIGH;

    const struct rism_pwmstd_t  *PWMSTANDARDTABLE;  /* RAM Tables */
    const struct rism_pwmhi_t   *PWMHIRESTABLE;
    const struct rism_zpdtcfg_t *ZPDSTANDARDTABLE;

    float PWMSPEED;       /* Frequencies of Operation */
    float CMSPEED;
    float ZPDPERIOD;

    uint8_t RECIRCULATIONSIDE;   /* PWM Operation Defaults */
    uint8_t DEFAULTPRECISION;

    float ZPDBLANKINGTIME;    /* ZPD Operation Defaults */
    uint8_t ZPDVIBRATIONDAMPING;
    uint8_t ZPDSUSPENDCLOCKS;
    uint8_t ZPDDIGFILTERLEVEL;
    uint8_t ZPDDIGFILTERDEPTH;
    uint8_t ZPDANALOGUELEVEL;
    uint8_t ZPDANALOGUESELECT;
    
    float ZPDSYNCTIME;
    float ZPDSTABTIME;
    float ACCELTIME;
    uint16_t ACCELRESOLUTION;
    uint32_t ZPDTARGETCOMPENSATION;

} r_ism_motor_t;


/*******************************************************************************
  Section: Global Functions
*/


r_ism_Error_t R_ISM_Init(uint32_t Unit);
r_ism_Error_t R_ISM_DeInit(uint32_t Unit);


#if 1
/* ISM Application level Functions of driver
   (former header file "qlapbismv1_a.h" */

 extern uint8_t R_ISM_PWMMoveTest( uint8_t UnitNumber,
                                          float   PWMFrequency,
                                          float   UpdateFrequency,
                                          uint8_t ChannelEnableMask,
                                          uint8_t RecirculationSide );
 extern uint8_t R_ISM_GetCMCharacteristic( uint8_t  UnitNumber,
                                                  float    PWMFrequency,
                                                  float    UpdateFrequency,
                                                  uint8_t  ChannelEnableMask,
                                                  uint32_t* BusyTimeCount );
 extern  uint8_t R_ISM_SetupCMChannel( uint8_t UnitNumber,
                                              uint8_t ChannelEnableMask,
                                              uint8_t ClearTargetPosFlag );
 extern  uint8_t R_ISM_MoveCMChannel( uint8_t  UnitNumber,
                                             uint8_t   ChannelEnableMask,
                                             uint32_t  TargetPosition );
 extern  uint8_t R_ISM_AutoMoveCMChannel( uint8_t  UnitNumber,
                                                 uint8_t  ChannelEnableMask,
                                                 uint8_t  RandomMovement );
 extern  uint8_t R_ISM_SelectCMParamset( uint8_t  ChannelNumber,
                                                uint8_t  SetNumber );
 extern uint8_t R_ISM_ZPDCMChannel( uint8_t  UnitNumber,
                                           uint8_t  ChannelEnableMask );
 extern uint8_t R_ISM_MonitorZPDCMChannels( uint8_t  UnitNumber,
                                                   uint8_t  ChannelEnableMask,
                                                   uint8_t  InterruptClearFlag,
                                                   uint8_t* ZPDActive,
                                                   uint8_t* ZPDDelayState,
                                                   uint8_t* ZPDTableIndex,
                                                   uint8_t* ZPDMeasureActive,
                                                   uint8_t* ZPDCurrentResult,
                                                   uint8_t* ZPDAccumulatedResult );
 extern uint8_t R_ISM_InitializeMovements( uint8_t  UnitNumber,
                                                  uint8_t  ParameterSet );
 extern uint8_t R_ISM_DownloadCMParamset( uint8_t  UnitNumber,
                                                 uint8_t  ChannelEnableMask );
#endif




#if 1
/* ISM Application Level Parameter Sets Include
   (former header file "qlapbismv1_s.h") */
#define R_ISM_A_USE_CONTI2000        ( 0 )
#define R_ISM_A_USE_JCISONCEBOZ6403  ( 1 )
#define R_ISM_A_USE_CHINASTANDARD    ( 3 )
#define R_ISM_A_USE_MARELLIPM20T     ( 4 )

  extern struct r_ism_motor_t R_ISM_A_CONTI2000;
  extern struct r_ism_motor_t R_ISM_A_JCISONCEBOZ6403;
  extern struct r_ism_motor_t R_ISM_A_CHINASTANDARD;
  extern struct r_ism_motor_t R_ISM_A_MARELLIPM20T;

  extern struct r_ism_motor_t *R_ISM_A_CURPARAMSET[];

  extern uint8_t R_ISM_SetMotor_CONTI2000( uint8_t ChannelNumber );
  extern uint8_t R_ISM_SetMotor_JCISONCEBOZ6403( uint8_t ChannelNumber );
  extern uint8_t R_ISM_SetMotor_CHINASTANDARD( uint8_t ChannelNumber );
  extern uint8_t R_ISM_SetMotor_MARELLIPM20T( uint8_t ChannelNumber );
  extern void StepperMotorUpdateSpeed(float speed);
  extern void StepperMotorSetSpeed(float speed);
#endif


#if 1  // $$$: TODO check this
/* functions from lower level ISM driver, but needed on application level
   from header file "qlapbismv1_p.h" */
    uint32_t R_ISM_Reset( uint8_t UnitNumber );
    uint32_t R_ISM_SetCMTargetPos( uint8_t UnitNumber,
                                             uint8_t  ChannelNumber,
                                             uint32_t TargetPosition );
    uint32_t R_ISM_SetInterrupt( uint8_t UnitNumber,
                                           uint8_t ChannelMask,
                                           uint8_t GlobalMask,
                                           uint8_t ZPDADMask,
                                           uint8_t EnableFlag );
#endif


/* Interrupt routines and flags of driver */
void R_ISM_IsrReached(uint32_t Unit);
void R_ISM_IsrDone(uint32_t Unit);
void R_ISM_IsrZpdad(uint32_t Unit);
void R_ISM_IsrZpd(uint32_t Unit);

void loc_Ism_TC0002(void);
void loc_Ism_TC0003(void);
void loc_Ism_TC0004(void);
 void loc_Ism_TcCarModelMove(void);
void  loc_Ism_Dx3_demo(void);

#endif  /* ISM_API_H_ */


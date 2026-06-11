/************************************************************************************************
PROJECT : ISM Test APP
FILE    : $Id: r_test_ism.c 4975 2015-03-27 16:22:34Z tobyas.hennig $
============================================================================ 
DESCRIPTION
Main funcitons of the Inteligent Stepper Motor (ISM) Test Application
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
  Title: ISM Test Application - Main Module

  Application is a test of the
  Inteligent Stepper Motor (ISM) macro
  that was made for the D1x MCU series.

  Pinout connection for stepper motors

  Mango main board 

       Stepper motor                                    WVGA board

Normal:
        Coil 1  COSP    +  Blue    P16_2   CN19 - 5     3   7
                COSM    -  Red     P16_3   CN19 - 7     4   8
        Coil 2  SINP    +  Green   P16_0   CN19 - 1     6   10
                SINM    -  Black   P16_1   CN19 - 3     5   9

Reverse:
        Coil 1  SINP    +  Green   P16_0   CN19 - 1     3   7
                SINM    -  Black   P16_1   CN19 - 3     4   8
        Coil 2  COSP    +  Blue    P16_2   CN19 - 5     6   10
                COSM    -  Red     P16_3   CN19 - 7     5   9


  Mango adaptor board CN9/CN11                   DX3 dash board

        ISM channel 1                            24 pin header
          Coil 1  COSP    P16_0   CN9 - 1        1
                  COSM    P16_1   CN9 - 2        2
          Coil 2  SINP    P16_2   CN9 - 3        3
                  SINM    P16_3   CN9 - 4        4

        ISM channel 2
          Coil 1  COSP    P16_4   CN9 - 5        5
                  COSM    P16_5   CN9 - 6        6
          Coil 2  SINP    P16_6   CN9 - 7        7
                  SINM    P16_7   CN9 - 8        8

        ISM channel 5
          Coil 1  COSP    P16_8   CN9 - 9        9
                  COSM    P16_9   CN9 - 10       10
          Coil 2  SINP    P16_10  CN9 - 11       11
                  SINM    P16_11  CN9 - 12       12

        ISM channel 6
          Coil 1  COSP    P17_0   CN11 - 1       13
                  COSM    P17_1   CN11 - 2       14
          Coil 2  SINP    P17_2   CN11 - 3       15
                  SINM    P17_3   CN11 - 4       16

        ISM channel 3
          Coil 1  COSP    P17_4   CN11 - 5       17
                  COSM    P17_5   CN11 - 6       18
          Coil 2  SINP    P17_6   CN11 - 7       19
                  SINM    P17_7   CN11 - 8       20

        ISM channel 4
          Coil 1  COSP    P17_8   CN11 - 9       21
                  COSM    P17_9   CN11 - 10      22
          Coil 2  SINP    P17_10  CN11 - 11      23
                  SINM    P17_11  CN11 - 12      24

    Clocks 

        M_CLK = PCLK / ISMnGGZF[3..0] bits 6..3 
 
        80MHz / 1024 = 78.125 KHz (12.8us)

*/

/*******************************************************************************
  Section: Includes
*/
#include "r_typedefs.h"     /* Renesas basic types, e.g. uint32_t */

#include "r_atf_api.h"
#include "r_bsp_stdio_api.h"
#include "r_dev_api.h"

#include "r_config_ism.h"
#include "r_ism_api.h"     /* ISM stepper motor */

#include "carmodel.h"     /* car model */
#include "motor_sm.h"     /* motor control app functions */

#include "r_tick_api.h"
#include "r_cdi_api.h"      /* R_CDI_HeapAlloc, R_CDI_HeapFree */

#include "config.h"
#include "r_gpio_api.h"
#include "r_ecm_api.h"

/* ISM links for the ECM IRQ */
#include "qlapbismv1_p.h"
#include "qlapbismv1.h"


/*******************************************************************************
  Section: Local Defines
*/

/* only one ISM unit */
#define ISM_UNIT0 0

/* set 1 to display speed/rpm on stdout */
#define SHOW_SPEED_DURING_MOVEMENT 0

/* ZDP configuration for the tests */
#define ISM_MIN_ZPD_TIME 200
#define ISM_MAX_ZPD_TIME 2000
#define ISM_ZDP_LOOPS_MAX 6


/*******************************************************************************
  Section: Local types
*/

/* Dx3 demo board motor config */
enum{
    MOT1_TEMPERATURE,
    MOT2_RPM,
    MOT3_UNUSED1,
    MOT4_UNUSED2,
    MOT5_SPEED,
    MOT6_FUEL
};


/*******************************************************************************
  Section: Local Constants
*/

/* zdp typical time limits mS */
static const uint32_t zdp_typ_time_limit[ISM_ZDP_LOOPS_MAX] = { 
200, 400, 600, 1000, 1200, 1500
};

/* set up some test pins to monitor */
static const r_dev_PinConfig_t loc_TestPinConfig[] = 
/*      Port                Pin                Func      Dir      Feedback     Pull        OpenDrain   HiCurr   HiSpeed  InputType */
{
    /* LED */
    {TEST_PORT,           TEST_PIN0,           0u, R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
    {TEST_PORT,           TEST_PIN1,           0u, R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
    {TEST_PORT,           TEST_PIN2,           0u, R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
    {TEST_PORT,           TEST_PIN3,           0u, R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
    {TEST_PORT,           TEST_PIN4,           0u, R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
    {TEST_PORT,           TEST_PIN5,           0u, R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
    {TEST_PORT,           TEST_PIN6,           0u, R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  
    {TEST_PORT,           TEST_PIN7,           0u, R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  

    /* delimiter - do not remove */
    {0u, R_DEV_PIN_LAST,0u,R_DEV_PIN_OUT,0u,R_DEV_PIN_PULLNO,     0u,      0u,         0u,    R_DEV_PIN_CMOS1}  
};


/*******************************************************************************
  Section: Local Variables
*/
static uint32_t loc_EcmZpdErrCount;


/*******************************************************************************
  Section: Local Functions
*/

/***********************************************************
  Function: loc_TestPinSetup

  Use some spare pins for debug use
*/
static void loc_TestPinSetup(void)
{
    /* set up some test pins to monitor */
    R_DEV_PinInit (loc_TestPinConfig);

}


/***********************************************************
  Function: loc_IsrEcmZpdErr

  Interrupt IRQ_ZDP ZPD has not occured
*/
static void loc_IsrEcmZpdErr(void)
{
    loc_EcmZpdErrCount++; 
}


/***********************************************************
  Function: loc_IsrEcmZpdEn

  Interrupt test IRQ_ZDP
*/
static void loc_IsrEcmZpdEn(void)
{
    r_ecm_ErrSrcConfig_t ErrSourceConfig;

    /* IRQ_ZPD is generated by the ECM */
    ErrSourceConfig.FEINT       = 1;
    ErrSourceConfig.FENMI       = 0;
    ErrSourceConfig.FEINTTimer  = 0;
    ErrSourceConfig.FENMITimer  = 0;
    ErrSourceConfig.Reset       = 0;
    ErrSourceConfig.Mask        = 1;

    /* do a de init first as we trasition in and out of here */
    R_ECM_DeInit(0);
    R_ECM_Init(0);

    R_ECM_ClearErrorSourceStatus(0, R_DEV_ECMERR_ISM0ZPD);
    ErrSourceConfig.ErrSource = R_DEV_ECMERR_ISM0ZPD; 
    R_ECM_SetErrorSourceConfig(0, &ErrSourceConfig);
    R_ECM_SetIsrCallback(0, R_ECM_INT_FEINT, R_DEV_ECMERR_ISM0ZPD, &loc_IsrEcmZpdErr);

    /* use the ISM driver to enable the interrupt */
    R_ISM_CreateInterrupt(ISM_UNIT0, R_ISM_INT_ZPD, 0, R_NULL);
}


/***********************************************************
  Function: loc_Ism_Move_Motors

  Moves the ISM in a random way for the DX3 board
*/
static void loc_Ism_Move_Motors(uint8_t channel, uint32_t angle)
{
uint8_t CurrentChannel_u08;
uint32_t TargetPositionRange_u32[R_ISM_CHANNELS];
uint32_t CurTargetPosition_u32[R_ISM_CHANNELS];


    /* get the range of the stepper motors */
    CurrentChannel_u08 = channel;
    {
        /* All interrupts aren't acknowledged */
        switch( R_ISM_A_CURPARAMSET[ CurrentChannel_u08 ]->DEFAULTPRECISION )
        {
            case R_ISM_PRECISION_STD:
            {
                TargetPositionRange_u32[ CurrentChannel_u08 ] = 
                R_ISM_A_CURPARAMSET[ CurrentChannel_u08 ]->TARGETRANGE_LOW;
                break;
            }

            case R_ISM_PRECISION_HI:
            {
                TargetPositionRange_u32[ CurrentChannel_u08 ] = 
                R_ISM_A_CURPARAMSET[ CurrentChannel_u08 ]->TARGETRANGE_HIGH;
                break;
            }

            default:
            {
                /* error */
                while(1);
            }
        }

        /* the TargetPositionRange_u32 is used to position the motor
            so for CONTI motors this is:
                0x0174000L Target Position Range Low Resolution
                0x05D0000L; Target Position Range High Resolution
    
                we will use an angle of 270 deg max
    
                so position_max = TargetPositionRange_u32 * 270 / 360
                   position_min = 0
        */
    
        /* set to 135 deg */
        CurTargetPosition_u32[CurrentChannel_u08] = TargetPositionRange_u32[CurrentChannel_u08] * (angle) / 360;

        /* Disable REACHED interrupts for the currently signalled channel */
        R_ISM_SetInterrupt(R_ISM_UNIT,
                                           ( 1 << CurrentChannel_u08 ),
                                             0,   /* DONE  Interrupt disable */
                                             0,   /* ZPDAD Interrupt disable */
                                             0 ); /* Enable Flag */
    
        /* compensate for the permanent deviation of the algorithm */
        //R_ISM_CompensateDeviation(&CurTargetPosition_u32[ CurrentChannel_u08 ]);
        
        /* Set the calculated position */
        R_ISM_SetCMTargetPos(R_ISM_UNIT,
                                             CurrentChannel_u08,
                                             CurTargetPosition_u32[CurrentChannel_u08]);
    
        /* Enable REACHED interrupts for the currently moved channel */
        R_ISM_SetInterrupt(R_ISM_UNIT,
                                            (1 << CurrentChannel_u08 ),
                                             0,   /* DONE  Interrupt disable */
                                             0,   /* ZPDAD Interrupt disable */
                                             1 ); /* Enable Flag */
    }
}


/*******************************************************************************
  Section: Main Functions
*/


/***********************************************************
  Function: loc_Ism_TC0001

  Manual ZPD on all channels
*/
void loc_Ism_TC0001(void)
{
uint8_t ISMTest_EnabledChannels;
uint8_t CurrentChannel_u08;


    /* init the ISM */
    R_ISM_Init(R_ISM_UNIT);//clock,port,zpd,interrupt

    //R_ATF_Prologue("Manual ZPD on all channels\n");

    /* just one channel for now */
    ISMTest_EnabledChannels = R_ISM_CHANNEL_EN0 | R_ISM_CHANNEL_EN1 |
                              R_ISM_CHANNEL_EN2 | R_ISM_CHANNEL_EN3 |
                              R_ISM_CHANNEL_EN4 | R_ISM_CHANNEL_EN5;


    for(CurrentChannel_u08 = R_ISM_CHANNEL0; CurrentChannel_u08 < R_ISM_CHANNELS; CurrentChannel_u08++)
    {
        /* use the CONTI motor profile */
        R_ISM_SetMotor_CONTI2000(CurrentChannel_u08);
    }

    /* init and download pram set */
    R_ISM_Reset(R_ISM_UNIT);
    R_ISM_DownloadCMParamset(R_ISM_UNIT, ISMTest_EnabledChannels);
    R_ISM_SetupCMChannel(R_ISM_UNIT,
                                        ISMTest_EnabledChannels, /* all configured channels */
                                        1 );                     /* clear target position */
    /* reset motor back to ZPD */
    R_ISM_ZPDCMChannel(R_ISM_UNIT, ISMTest_EnabledChannels ); /* all configured channels */


    /* give them some time to reach the target position */
    R_TICK_WaitMS(0, 2000);

    /* DeInit the ISM (stepper motor) controller */
    R_ISM_DeInit(R_ISM_UNIT);
}


/***********************************************************
  Function: loc_Ism_TC0002

  ZPD sequence one motor after the other
*/
void loc_Ism_TC0002(void)
{
uint8_t ISMTest_EnabledChannels;
uint8_t CurrentChannel_u08;

/* each motor given a different position */
const uint32_t angle[R_ISM_CHANNELS] = { 45, 90, 135, 180, 225, 270};

   // R_ATF_Prologue("ZPD sequence one motor after the other\n");

    /* init the ISM */
    R_ISM_Init(R_ISM_UNIT);

    /* all channels */
    ISMTest_EnabledChannels = R_ISM_CHANNEL_EN0 | R_ISM_CHANNEL_EN1 |
                              R_ISM_CHANNEL_EN2 | R_ISM_CHANNEL_EN3 |
                              R_ISM_CHANNEL_EN4 | R_ISM_CHANNEL_EN5;


    for(CurrentChannel_u08 = R_ISM_CHANNEL0; CurrentChannel_u08 < R_ISM_CHANNELS; CurrentChannel_u08++)
    {
        /* use the CONTI motor profile */
        R_ISM_SetMotor_CONTI2000(CurrentChannel_u08);
    }

    /* init and download pram set */
    R_ISM_Reset(R_ISM_UNIT);
    R_ISM_DownloadCMParamset(R_ISM_UNIT, ISMTest_EnabledChannels);
    R_ISM_SetupCMChannel(R_ISM_UNIT,
                                        ISMTest_EnabledChannels, /* all configured channels */
                                        1 );                     /* clear target position */
    /* reset motor back to ZPD */
    R_ISM_ZPDCMChannel(R_ISM_UNIT, ISMTest_EnabledChannels ); /* all configured channels */

    /* move the motors */
    loc_Ism_Move_Motors(R_ISM_CHANNEL0, angle[0]);
    loc_Ism_Move_Motors(R_ISM_CHANNEL1, angle[1]);
    loc_Ism_Move_Motors(R_ISM_CHANNEL2, angle[2]);
    loc_Ism_Move_Motors(R_ISM_CHANNEL3, angle[3]);
    loc_Ism_Move_Motors(R_ISM_CHANNEL4, angle[4]);
    loc_Ism_Move_Motors(R_ISM_CHANNEL5, angle[5]);

    /* give them some time to reach the target position */
    R_TICK_WaitMS(0, 1000);

    /* now run through each motor in turn to ZPD */
    for(CurrentChannel_u08 = R_ISM_CHANNEL0; CurrentChannel_u08 < R_ISM_CHANNELS; CurrentChannel_u08++)
    {
       // R_BSP_STDIO_Printf("Running ZPD for ISM channel %d... ", CurrentChannel_u08);

        /* reset motor back to ZPD */
        R_ISM_ZPDCMChannel(R_ISM_UNIT, 1 << CurrentChannel_u08 ); /* all configured channels */

       // R_BSP_STDIO_Printf("done \n");
    }

    /* DeInit the ISM (stepper motor) controller */
    R_ISM_DeInit(R_ISM_UNIT);
}


/***********************************************************
  Function: loc_Ism_TC0003

  ZPD all motors together 
*/
void loc_Ism_TC0003(void)
{
uint32_t end_time;
uint32_t start_time;
uint32_t delta_time;
uint8_t ISMTest_EnabledChannels;
uint8_t CurrentChannel_u08;
uint32_t channel, c, i;

/* each motor given a different position */
const uint32_t angle[R_ISM_CHANNELS] = { 5, 7, 11, 15, 20, 27};

   // R_ATF_Prologue("ISM ZDP all motors together\n");

    /* init the ISM */
    R_ISM_Init(R_ISM_UNIT);

    ISMTest_EnabledChannels = 0;

  //  R_BSP_STDIO_Printf("Select channel range to test 0..5, enter highest channel (i.e 0, 1 and 2 select 2)\n");
  //  channel = R_BSP_STDIO_ReadValue();

    /* for the stepper each channel uses a bit
       chan 0 = 0x01
       chan 1 = 0x02
       ... 
    */
   // for(c = 0; c <= channel; c++)
    //{
     //   ISMTest_EnabledChannels |= 1 << c;
     //   R_BSP_STDIO_Printf(" %d,", c);
   // }

   // R_BSP_STDIO_Printf(" channels to test\n");
    /* enable the port buffer for ZDP analog input, now done in the driver */

    /* clear the ZPD error count */
    loc_EcmZpdErrCount = 0;
    loc_IsrEcmZpdEn();

    for(CurrentChannel_u08 = R_ISM_CHANNEL0; CurrentChannel_u08 < R_ISM_CHANNELS; CurrentChannel_u08++)
    {
        /* use the CONTI motor profile */
        R_ISM_SetMotor_CONTI2000(CurrentChannel_u08);
    }

    /* init and download pram set */
    R_ISM_Reset(R_ISM_UNIT);
    R_ISM_DownloadCMParamset(R_ISM_UNIT, ISMTest_EnabledChannels);
    R_ISM_SetupCMChannel(R_ISM_UNIT,
                                            ISMTest_EnabledChannels, /* all configured channels */
                                            1 );                     /* clear target position */
    /* reset motor back to ZPD */
    R_ISM_ZPDCMChannel(R_ISM_UNIT, ISMTest_EnabledChannels ); /* all configured channels */

    /* do a number of ZDP tests */
    for(i = 0; i < ISM_ZDP_LOOPS_MAX; i++)
    {
        /* move the motors */
        loc_Ism_Move_Motors(R_ISM_CHANNEL0, angle[0] * i);
        loc_Ism_Move_Motors(R_ISM_CHANNEL1, angle[1] * i);
        loc_Ism_Move_Motors(R_ISM_CHANNEL2, angle[2] * i);
        loc_Ism_Move_Motors(R_ISM_CHANNEL3, angle[3] * i);
        loc_Ism_Move_Motors(R_ISM_CHANNEL4, angle[4] * i);
        loc_Ism_Move_Motors(R_ISM_CHANNEL5, angle[5] * i);
    
        /* give them some time to reach the target position */
        R_TICK_WaitMS(0, 1000);
    
        /* now do ZPD, start and measure the time */
        start_time = R_TICK_GetTimeMS(0);

        /* reset motor back to ZPD */
        R_ISM_ZPDCMChannel(R_ISM_UNIT, ISMTest_EnabledChannels ); /* all configured channels */

        end_time = R_TICK_GetTimeMS(0);

        delta_time = end_time - start_time;

        /* if it takes too long or too quick then the ZPD is not setup correctly 
           limits are tight here maybe widen them... 
        */
        if( (delta_time > (zdp_typ_time_limit[i] + 200) ) || (delta_time < (zdp_typ_time_limit[i] - 200) ) )
        {
            R_BSP_STDIO_Printf(" - Fail: ZDP took %d mS\n", delta_time);
        }
        else
        {
             R_BSP_STDIO_Printf(" - Pass: ZDP took %d mS\n", delta_time);
        }

        R_BSP_STDIO_Printf(" - ECM ZDP not reached error count %d\n", loc_EcmZpdErrCount);

        R_TICK_WaitMS(0, 1000);
    }

    /* DeInit the ISM (stepper motor) controller */
    R_ISM_DeInit(R_ISM_UNIT);
}


/***********************************************************
  Function: loc_Ism_TC0004

  Motors at 270 degrees
*/
void loc_Ism_TC0004(void)
{
uint8_t ISMTest_EnabledChannels;
uint8_t CurrentChannel_u08;
uint32_t channel, c;

/* each motor given a different position */
const uint32_t angle[R_ISM_CHANNELS] = { 270, 270, 270, 270, 270, 270};

  //  R_ATF_Prologue("Motors at 270 degrees\n");

    /* init the ISM */
    //R_ISM_Init(R_ISM_UNIT);

    ISMTest_EnabledChannels = 0;

   // R_BSP_STDIO_Printf("Select channel range to test 0..5, enter highest channel (i.e 0, 1 and 2 select 2)\n");
    //channel = R_BSP_STDIO_ReadValue();
//
    /* for the stepper each channel uses a bit
       chan 0 = 0x01
       chan 1 = 0x02
       ... 
    */
    for(c = 0; c <= channel; c++)
    {
       // ISMTest_EnabledChannels |= 1 << c;
     //   R_BSP_STDIO_Printf(" %d,", c);
    }

  //  R_BSP_STDIO_Printf(" channels to test\n");
    /* enable the port buffer for ZDP analog input, now done in the driver */

    /* clear the ZPD error count */
    loc_EcmZpdErrCount = 0;
    loc_IsrEcmZpdEn();

    for(CurrentChannel_u08 = R_ISM_CHANNEL0; CurrentChannel_u08 < R_ISM_CHANNELS; CurrentChannel_u08++)
    {
        /* use the CONTI motor profile */
        //R_ISM_SetMotor_CONTI2000(CurrentChannel_u08);
	R_ISM_SetMotor_CONTI2000(CurrentChannel_u08);
    }

    /* init and download pram set */
    R_ISM_Reset(R_ISM_UNIT);
    R_ISM_DownloadCMParamset(R_ISM_UNIT, 4);
    R_ISM_SetupCMChannel(R_ISM_UNIT,
                                        4, /* all configured channels */
                                        1 );                     /* clear target position */

    /* move the motors */
    loc_Ism_Move_Motors(R_ISM_CHANNEL0, 30);
    loc_Ism_Move_Motors(R_ISM_CHANNEL1, 60);
    loc_Ism_Move_Motors(R_ISM_CHANNEL2,90);
    loc_Ism_Move_Motors(R_ISM_CHANNEL3, 120);
    loc_Ism_Move_Motors(R_ISM_CHANNEL4, 180);
    loc_Ism_Move_Motors(R_ISM_CHANNEL5, 45);
    
    /* give them some time to reach the target position */
    R_TICK_WaitMS(0, 1000);
    
    /* DeInit the ISM (stepper motor) controller */
    R_ISM_DeInit(R_ISM_UNIT);
}




/***********************************************************
  Function: loc_Ism_Dx3_demo

  Moves the ISM in a random way for the DX3 board
*/
void loc_Ism_Dx3_demo()
{
uint8_t ISMTest_EnabledChannels;
uint8_t CurrentChannel_u08;
uint32_t i;

/* max position for the motors */
uint32_t angle[R_ISM_CHANNELS] = {  100, 295, 270, 270, 295, 100 };

/* lookup table for the motor position */
uint32_t sequence[] [R_ISM_CHANNELS + 1] = {
 
    /*  MOT1_TEMPERATURE
        MOT2_RPM
        MOT3_UNUSED1
        MOT4_UNUSED2
        MOT5_SPEED
        MOT6_Fuel

   M1    M2            M3            M4       M5   M6    time mS */
{   0,   10,     135 - 25,     135 + 25,      30,  30,      500},
{   5,   20,     135 + 25,     135 - 25,      40,  95,      500},
{  10,   30,          150,          150,      50,  90,      500},
{  15,   40,          150,          150,      60,  85,      500},
{  20,   50,          150,          150,      80,  80,      500},
{  25,   60,          150,          150,      90,  75,      500},
{  30,   70,          150,          150,     100,  70,      500},
{  35,   80,          150,          150,     110,  70,      500},
{  40,   90,          150,          150,     120,  70,      500},
{  45,   60,     135 - 25,     135 + 25,     130,  70,      500},
{  50,   70,     135 + 25,     135 - 25,     140,  70,      500},
{  60,   60,          150,          150,     130,  70,      500},
{  60,   90,          150,          150,     220,  70,      500},
{  60,   80,          150,          150,     210,  70,      500},
{  60,   80,          100,          150,     200,  70,      500},
{  60,   50,          100,          100,      90,  70,      500},
{  60,   40,           50,          100,      80,  70,      500},
{  60,   30,           50,           50,      70,  70,      500},
{  60,   20,           40,           40,      60,  60,      500},
{  50,   10,           30,           30,      50,  50,      500},
{  40,   10,           30,           30,      40,  40,      500},
{  30,   10,           30,           30,      30,  30,      500},
{  20,   10,           20,           20,      20,  20,      500},
{   0,    0,            0,            0,       0,   0,      500}
};

    //R_ATF_Prologue("Dx3 board 6 motor demo\n");
    R_BSP_STDIO_Printf(" - Connect the demo board 24 way pin header to the D1M2H manago adaptor CN9/11 \n");
    R_BSP_STDIO_Printf(" - (align CN9 - 1 to Dx3 24 way pin header pin 1\n");

    /* init the ISM */
    R_ISM_Init(R_ISM_UNIT);

    /* just one channel for now */
    ISMTest_EnabledChannels = R_ISM_CHANNEL_EN0 | R_ISM_CHANNEL_EN1 |
                              R_ISM_CHANNEL_EN2 | R_ISM_CHANNEL_EN3 |
                              R_ISM_CHANNEL_EN4 | R_ISM_CHANNEL_EN5;


    for(CurrentChannel_u08 = R_ISM_CHANNEL0; CurrentChannel_u08 < R_ISM_CHANNELS; CurrentChannel_u08++)
    {
        /* use the CONTI motor profile */
        R_ISM_SetMotor_CONTI2000(CurrentChannel_u08);
    }

    /* init and download pram set */
    R_ISM_Reset(R_ISM_UNIT);
    R_ISM_DownloadCMParamset(R_ISM_UNIT, ISMTest_EnabledChannels);
    R_ISM_SetupCMChannel(R_ISM_UNIT,
                                        ISMTest_EnabledChannels, /* all configured channels */
                                        1 );                     /* clear target position */

    R_BSP_STDIO_Printf("Assert ZPD\n");

    /* reset motor back to ZPD */
    R_ISM_ZPDCMChannel(R_ISM_UNIT, ISMTest_EnabledChannels ); /* all configured channels */

    /* give them some */
    R_TICK_WaitMS(0, 2000);

    R_BSP_STDIO_Printf("Motors at Quater position\n");

    /* Position at a quarter */
    for(CurrentChannel_u08 = R_ISM_CHANNEL0; CurrentChannel_u08 < R_ISM_CHANNELS; CurrentChannel_u08++)
    {
        loc_Ism_Move_Motors(CurrentChannel_u08, angle[CurrentChannel_u08] / 4);
    }

    /* give them some time to reach the target position */
    R_TICK_WaitMS(0, 2000);
 
    R_BSP_STDIO_Printf("Running ZPD sequence\n");

    /* now run through each motor in turn to ZPD */
    for(CurrentChannel_u08 = R_ISM_CHANNEL0; CurrentChannel_u08 < R_ISM_CHANNELS; CurrentChannel_u08++)
    {
        R_BSP_STDIO_Printf(" - ZPD for ISM channel %d... ", CurrentChannel_u08);

        /* reset motor back to ZPD */
        R_ISM_ZPDCMChannel(R_ISM_UNIT, 1 << CurrentChannel_u08 ); /* all configured channels */

        R_BSP_STDIO_Printf("done \n");
    }

    /* Position at MAX */
    for(CurrentChannel_u08 = R_ISM_CHANNEL0; CurrentChannel_u08 < R_ISM_CHANNELS; CurrentChannel_u08++)
    {
        loc_Ism_Move_Motors(CurrentChannel_u08, angle[CurrentChannel_u08]);
    }

    R_TICK_WaitMS(0, 2000);

    /* Position at MIN */
    for(CurrentChannel_u08 = R_ISM_CHANNEL0; CurrentChannel_u08 < R_ISM_CHANNELS; CurrentChannel_u08++)
    {
        loc_Ism_Move_Motors(CurrentChannel_u08, 0);
    }

    R_TICK_WaitMS(0, 2000);

    /* now do a demo sequence */
    for(i = 0; i < sizeof(sequence) / sizeof(sequence[0]); i++)
    {
        loc_Ism_Move_Motors(MOT1_TEMPERATURE, sequence[i] [MOT1_TEMPERATURE]);
        loc_Ism_Move_Motors(MOT2_RPM, sequence[i] [MOT2_RPM]);
        loc_Ism_Move_Motors(MOT3_UNUSED1, sequence[i] [MOT3_UNUSED1]);
        loc_Ism_Move_Motors(MOT4_UNUSED2, sequence[i] [MOT4_UNUSED2]);
        loc_Ism_Move_Motors(MOT5_SPEED, sequence[i] [MOT5_SPEED]);
        loc_Ism_Move_Motors(MOT6_FUEL, sequence[i] [MOT6_FUEL]);

        R_TICK_WaitMS(0, sequence[i] [6]);
    }

    /* DeInit the ISM (stepper motor) controller */
    R_ISM_DeInit(R_ISM_UNIT);
}


/***********************************************************
  Function: loc_Ism_TcCarModelMove

  Moves the ISM in a random way
*/
int speed = 30;
void loc_Ism_TcCarModelMove(void)
{

#if SHOW_SPEED_DURING_MOVEMENT    
    double oldspeed;
    double rpm;
    double speed;
#endif
    static uint8_t cnt;
    uint32_t endTime;
    
  //  R_ATF_Prologue("ISM car model movement (simple Example)");
    
    CarModel_Init();
    StepperMotorInit(R_FALSE, R_ISM_CHANNEL_EN0 | R_ISM_CHANNEL_EN1 | R_ISM_CHANNEL_EN2 | R_ISM_CHANNEL_EN3 | R_ISM_CHANNEL_EN4 | R_ISM_CHANNEL_EN5);

    /* For 20 seconds move needles */
    //R_BSP_STDIO_Printf("Move needles for 20 seconds now.\n");
    endTime = (R_TICK_GetTimeMS(0) + (20 * 1000));
    while (endTime > R_TICK_GetTimeMS(0))
    {
        R_TICK_WaitMS(0, LOC_CM_UPDATE_TICK);
      
        CarModel_Update();

        /* less updates for the motors (compared to the car model) */
        cnt = ((cnt + 1) % 4);
        if (cnt == 0)
        {
            StepperMotorUpdate();
#if SHOW_SPEED_DURING_MOVEMENT
        /* test car model speed */
        speed = CarModelSpeedGet();
        /* test car model RPM */
        rpm = CarModelRpmGet();
        if (oldspeed != speed)
        {
            oldspeed = speed;
           // R_BSP_STDIO_Printf("speed: %f km/h, rpm: %f 1/min\n", speed, rpm);
        }
#endif
        }
    }
    
    /* move needles back to zero */
  //  R_BSP_STDIO_Printf("Move needles back to zero position (no ZPD).\n");
  if(speed<140)speed++;
  	else speed = 0;
    CarModelSpeedSet(speed);
    CarModelRpmSet(0);
    StepperMotorUpdate();
    R_TICK_WaitMS(0, 1500);


	
    /* De-Init */
   // CarModel_DeInit();
   // StepperMotorDeInit();
    
    //R_BSP_STDIO_Printf("End of test case\n\n");
}


/*******************************************************************************
  Constant: R_ATF_CasesTree

  Testcase listing with multilevel menu support
  ID: numbers 1...9 and lower and upper case letters
  Level: Menu level, 0 is root, ...
         Level and order of items desribe the menu tree
  Item Description: textual test description (less 70 characters)
  loc_Itemfunction: function pointer of the related action,

*/
const r_atf_Menu_t loc_TtestCases[]=
{
/*   loc_Itemid, menu_id,    child_id,    loc_Itemdesc,     loc_Itemfunction */
    /* This is the main menu */

    {(int8_t)'1'      , 0,        0,    "ISM_TC_0001: Manual ZPD on all channels",                  loc_Ism_TC0001   },
    {(int8_t)'2'      , 0,        0,    "ISM_TC_0002: ZPD sequence one motor after the other",      loc_Ism_TC0002   },
    {(int8_t)'3'      , 0,        0,    "ISM_TC_0003: ZPD all motors together",                     loc_Ism_TC0003   },
    {(int8_t)'4'      , 0,        0,    "ISM_TC_0004: Motors at 270 degrees",                       loc_Ism_TC0004   },
  
    {(int8_t)'a'      , 0,        0,    "ISM car model movement (DX3 board)",                       loc_Ism_Dx3_demo},
    {(int8_t)'b'      , 0,        0,    "ISM car model movement (simple Example)",                  loc_Ism_TcCarModelMove },

    /* Delimiter, don't change*/
    {(int8_t)'0'      , 0,        0,    0,               0   }
};


/***********************************************************
  Function: R_ATF_TestInit

  Run all tests without user interaction
*/
void R_ATF_TestInit(void)
{



    loc_TestPinSetup();



}


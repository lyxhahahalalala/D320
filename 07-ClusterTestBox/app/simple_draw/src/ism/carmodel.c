/*
****************************************************************************
PROJECT : ISM Test APP
FILE    : $Id: carmodel.c 4594 2015-01-08 17:13:58Z tobyas.hennig $
============================================================================ 
DESCRIPTION
Car Model
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
  Title: carmodel.c

  Implementation of car model functions.
*/


/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"     /* Renesas basic types, e.g. uint32_t */
#include "carmodel.h"


/***********************************************************
  Section: Local Defines
*/


/***********************************************************
  Define: LOC_ACCELERATION_FACTOR

  Speed difference that is caused by one speed update tick. (in km/h)
*/

#define LOC_ACCELERATION_FACTOR    0.95


/***********************************************************
  Define: LOC_CM_SPEED_MAX

  Maximum speed.
*/

#define LOC_CM_SPEED_MAX           240


/***********************************************************
  Define: LOC_CM_SPEED_MIN

  Minimum speed.
*/

#define LOC_CM_SPEED_MIN             10


/***********************************************************
  Define: LOC_CM_SPEED_MID

  Medium speed.
*/

#define LOC_CM_SPEED_MID             50


/***********************************************************
  Define: LOC_CM_RPM_MAX

  Maximum rpm.
*/

#define LOC_CM_RPM_MAX             8000


/***********************************************************
  Define: LOC_CM_RPM_MIN

  Minimum rpm.
*/

#define LOC_CM_RPM_MIN             100


/***********************************************************
  Define: LOC_CM_GEARDOWN_RPM

  Shift down RPM.
*/

#define LOC_CM_GEARDOWN_RPM        2700

/***********************************************************
  Define: LOC_CM_GEARUP_RPM

  Shift down RPM.
*/

#define LOC_CM_GEARUP_RPM          5500


/***********************************************************
  Define: LOC_CM_GEAR_MAX

  Max gear.
*/

#define LOC_CM_GEAR_MAX            5


/***********************************************************
  Define: LOC_CM_GEAR_MIN

  Min gear.
*/

#define LOC_CM_GEAR_MIN            1


/***********************************************************
  Section: Local Variables
*/

/***********************************************************
  Variable: CarData
*/
CarData_t CarData;


/***********************************************************
  Variable: locGearTrans

  Translation factor speed to rpm.
*/

// static const uint16_t locGearTrans[] = {
    // 200, 200, 150, 100, 50, 25 
// };


/***********************************************************
  Section: Local Functions
*/


/***********************************************************
  Function: locUpdateTurnSignals
  
  Update turn signal values.
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateTurnSignals(void) {
    


}


/***********************************************************
  Function: locUpdateSpeed
  
  Update speed value.
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateSpeed(void) {

}


/***********************************************************
  Function: locUpdateRpm
  
  Update RPM & Gear value.
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateRpm(void) {

}


/***********************************************************
  Function: locUpdateLight
  
  Update low & high light
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateLight(void) {

}


/***********************************************************
  Function: locUpdateFan
  
  Update fan
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateFan(void) {

}


/***********************************************************
  Function: locUpdateOil
  
  Update oil
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateOil(void) {

}


/***********************************************************
  Function: locUpdateBattery
  
  Update battery
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateBattery(void) {

}


/***********************************************************
  Function: locUpdateAirbag
  
  Update airbag
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateAirbag(void) {
  
}


/***********************************************************
  Function: locUpdateBelt
  
  Update belt
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateBelt(void) {

}


/***********************************************************
  Function: locUpdateBreak
  
  Update break
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateBreak(void) {
 
}


/***********************************************************
  Function: locUpdateWindshield
  
  Update windshield
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateWindshield(void) {
  
}


/***********************************************************
  Function: locUpdateWiping
  
  Update wiping
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateWiping(void) {

}


/***********************************************************
  Function: locUpdateCoolingLiquid
  
  Update cooling liquid  value.
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateCoolingLiquid(void) {

}




/***********************************************************
  Function: locUpdateFuel
  
  Update fuel value.
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateFuel(void) {

}


/***********************************************************
  Function: locUpdateGearSelect
  
  Update gear selection.
  
  Parameters:
  void
  
  Returns:
  void
*/

static void locUpdateGearSelect(void) {

}


/***********************************************************
  Section: Global Functions

  See: <app_cl_carmodel.h>
*/


/***********************************************************
  Function: CarModel_Init
*/

void CarModel_Init(void)
{

}


/***********************************************************
  Function: CarModel_DeInit
*/

void CarModel_DeInit(void)
{
}


/***********************************************************
  Function: CarModel_Update
  
  Car model update
  
  updates the car model data.
  
  Parameters:
  none
  
  Returns:
  void
*/

void CarModel_Update(void)
{
        locUpdateSpeed();
        locUpdateRpm();
        locUpdateTurnSignals();
        locUpdateLight();
        locUpdateFan();
        locUpdateOil();
        locUpdateBattery();
        locUpdateAirbag();
        locUpdateBelt();
        locUpdateBreak();
        locUpdateWindshield();
        locUpdateWiping();
        locUpdateCoolingLiquid();
        locUpdateFuel();
        locUpdateGearSelect();
}


/***********************************************************
  Function: CarModelSpeedGet
*/

uint32_t CarModelSpeedGet(void) {
    return CarData.Speed;
}

void CarModelSpeedSet(uint32_t Speed) {
    CarData.Speed = Speed;
}


/***********************************************************
  Function: CarModelRpmGet
*/

double CarModelRpmGet(void) {
    return CarData.Rpm;
}

void CarModelRpmSet(uint16_t Rpm) {
    CarData.Rpm = Rpm;
}


/***********************************************************
  Function: CarModelLeftGet
*/

uint8_t CarModelLeftGet(void) {
    return 0;
}

/***********************************************************
  Function: CarModelRightGet
*/

uint8_t CarModelRightGet(void) {
     return 0;
}


/***********************************************************
  Function: CarModelHighLightGet
*/

uint8_t CarModelHighLightGet(void) {
     return 0;
}


/***********************************************************
  Function: CarModelLowLightGet
*/

uint8_t CarModelLowLightGet(void) {
     return 0;
}


/***********************************************************
  Function: CarModelFanGet
*/

uint8_t CarModelFanGet(void) {
    return 0;
}


/***********************************************************
  Function: CarModelOilGet
*/

double CarModelOilGet(void) {
    return CarData.OilPressure;
}


/***********************************************************
  Function: CarModelBatteryGet
*/

double CarModelBatteryGet(void) {
    return CarData.Battery;
}


/***********************************************************
  Function: CarModelAirbagGet
*/

uint8_t CarModelAirbagGet(void) {
     return 0;
}


/***********************************************************
  Function: CarModelBeltGet
*/

uint8_t CarModelBeltGet(void) {
     return 0;
}


/***********************************************************
  Function: CarModelBreakGet
*/

uint8_t CarModelBreakGet(void) {
    return 0;
}


/***********************************************************
  Function: CarModelWindshieldGet
*/

uint8_t CarModelWindshieldGet(void) {
     return 0;
}


/***********************************************************
  Function: CarModelWipingGet
*/

uint8_t CarModelWipingGet(void) {
     return 0;
}


/***********************************************************
  Function: CarModelFuelEmptyGet
*/

uint8_t CarModelFuelEmptyGet(void) {
    uint8_t x;

    if (CarData.FuelLevel > 10) {
        x = 0;
    } else {
        x = 1;
    }
    return x;
}


/***********************************************************
  Function: CarModelCoolingHotGet
*/

uint8_t CarModelCoolingHotGet(void) {
    uint8_t x;

    if (CarData.CoolingTemp < 90) {
        x = 0;
    } else {
        x = 1;
    }
    return x;
}


/***********************************************************
  Function: CarModelFuelLevelGet
*/

double CarModelFuelLevelGet(void) {
    return CarData.FuelLevel;
}


/***********************************************************
  Function: CarModelCoolingTempGet
*/

double CarModelCoolingTempGet(void) {
    return CarData.CoolingTemp;
}

/***********************************************************
  Function: CarModelGearPGet
*/

uint8_t CarModelGearPGet(void) {
     return 0;
}


/***********************************************************
  Function: CarModelGearRGet
*/

uint8_t CarModelGearRGet(void) {
     return 0;
}


/***********************************************************
  Function: CarModelGearNGet
*/

uint8_t CarModelGearNGet(void)  {
    return 0;
}


/***********************************************************
  Function: CarModelGearDGet
*/

uint8_t CarModelGearDGet(void)  {
    return 0;
}


/***********************************************************
  Function: CarModelGearSGet
*/

uint8_t CarModelGearSGet(void) {
    return 0;
}


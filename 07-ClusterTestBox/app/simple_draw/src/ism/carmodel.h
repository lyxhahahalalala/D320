/*
****************************************************************************
PROJECT : ISM Test APP
FILE    : $Id: carmodel.h 4594 2015-01-08 17:13:58Z tobyas.hennig $
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
  Title: carmodel.h

  Definition for the car model.
*/

#ifndef _CARMODEL_H_
#define _CARMODEL_H_

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
  Title: Car model interface
*/


/***********************************************************
  Define: LOC_CM_UPDATE_TICK

  Car model update frequency.
*/

#define LOC_CM_UPDATE_TICK             18


/***********************************************************
  Section: Global Types
*/

/***********************************************************
  Type: CarData_t

  Car parameter value type.

  Members:
  Rpm       - Rounds per minute
  Speed     - Speed in km/h
  Gear      - Gear
  Direction - Accelerate (1), Break (-1), Cruise (0)
  Left      - Turn signal left on/off
  Right     - Turn signal right on/off
*/

typedef struct {
    double Rpm;
    uint32_t  Speed;
    double  OilPressure;
    double  FuelLevel;
    double   CoolingTemp;
    double     Battery;
} CarData_t;


/***********************************************************
  Section: Global Variables
*/

/***********************************************************
  Variable: CarData

  Current car parameter values.
*/

extern CarData_t CarData;


/***********************************************************
  Section: Global Functions
*/

/***********************************************************
  Function: CarModel_Init
  
  Initialize the car model.
  
  Parameters:
  void
  
  Returns:
  void
*/

void CarModel_Init(void);


/***********************************************************
  Function: CarModel_DeInit
  
  Deinitialize the car model.
  
  Parameters:
  void
  
  Returns:
  void
*/

void CarModel_DeInit(void);


/***********************************************************
  Function: CarModel_Update
  
  Car model update
  
  updates the car model data.
  
  Parameters:
  none
  
  Returns:
  void
*/

void CarModel_Update(void);


/***********************************************************
  Function: CarModelSpeedGet

  Get current speed.
  
  Parameters:
  void
  
  Returns:
  Speed in km/h

*/

uint32_t CarModelSpeedGet(void);
void CarModelSpeedSet(uint32_t Speed);


/***********************************************************
  Function: CarModelRpmGet

  Get current rpm.
  
  Parameters:
  void
  
  Returns:
  rpm
*/

double CarModelRpmGet(void);
void CarModelRpmSet(uint16_t Rpm);


/***********************************************************
  Function: CarModelLeftGet

  Get left turn signal status.
  
  Parameters:
  void
  
  Returns:
  Status of left turn signal
*/

uint8_t CarModelLeftGet(void);


/***********************************************************
  Function: CarModelRightGet

  Get right turn signal status.
  
  Parameters:
  void
  
  Returns:
  Status of right turn signal
*/

uint8_t CarModelRightGet(void);


/***********************************************************
  Function: CarModelHighLightGet

  Get light signal status.
  
  Parameters:
  void
  
  Returns:
  Status the signal
*/

uint8_t CarModelHighLightGet(void);


/***********************************************************
  Function: CarModelLowLightGet

  Get light signal status.
  
  Parameters:
  void
  
  Returns:
  Status the signal
*/

uint8_t CarModelLowLightGet(void);


/***********************************************************
  Function: CarModelFanGet

  Get light signal status.
  
  Parameters:
  void
  
  Returns:
  Status the signal
*/

uint8_t CarModelFanGet(void);


/***********************************************************
  Function: CarModelOilGet

  Get light signal status.
  
  Parameters:
  void
  
  Returns:
  Status the signal
*/

double CarModelOilGet(void);


/***********************************************************
  Function: CarModelBatteryGet

  Get light signal status.
  
  Parameters:
  void
  
  Returns:
  Status the signal
*/

double CarModelBatteryGet(void);


/***********************************************************
  Function: CarModelAirbagGet

  Get light signal status.
  
  Parameters:
  void
  
  Returns:
  Status the signal
*/

uint8_t CarModelAirbagGet(void);


/***********************************************************
  Function: CarModelBeltGet

  Get light signal status.
  
  Parameters:
  void
  
  Returns:
  Status the signal
*/


uint8_t CarModelBeltGet(void);

/***********************************************************
  Function: CarModelBreakGet

  Get light signal status.
  
  Parameters:
  void
  
  Returns:
  Status the signal
*/

uint8_t CarModelBreakGet(void);


/***********************************************************
  Function: CarModelWindshieldGet

  Get light signal status.
  
  Parameters:
  void
  
  Returns:
  Status the signal
*/

uint8_t CarModelWindshieldGet(void);


/***********************************************************
  Function: CarModelWipingGet

  Get light signal status.
  
  Parameters:
  void
  
  Returns:
  Status the signal
*/

uint8_t CarModelWipingGet(void);


/***********************************************************
  Function: CarModelFuelEmptyGet

  Get light signal status.
  
  Parameters:
  void
  
  Returns:
  Status the signal
*/

uint8_t CarModelFuelEmptyGet(void);


/***********************************************************
  Function: CarModelCoolingHotGet

  Get light signal status.
  
  Parameters:
  void
  
  Returns:
  Status the signal
*/

uint8_t CarModelCoolingHotGet(void);


/***********************************************************
  Function: CarModelFuelLevelGet

  Get fuel level.
  
  Parameters:
  void
  
  Returns:
  Fuel level
*/

double CarModelFuelLevelGet(void);


/***********************************************************
  Function: CarModelCoolingTempGet

  Get cooling liquid temperature.
  
  Parameters:
  void
  
  Returns:
  Temperatur
*/

double CarModelCoolingTempGet(void);


/***********************************************************
  Function: CarModelGearPGet

  Get status of gear indicator.
  
  Parameters:
  void
  
  Returns:
  Light status
*/

uint8_t CarModelGearPGet(void);


/***********************************************************
  Function: CarModelGearRGet

  Get status of gear indicator.
  
  Parameters:
  void
  
  Returns:
  Light status
*/

uint8_t CarModelGearRGet(void);


/***********************************************************
  Function: CarModelGearNGet

  Get status of gear indicator.
  
  Parameters:
  void
  
  Returns:
  Light status
*/

uint8_t CarModelGearNGet(void);


/***********************************************************
  Function: CarModelGearDGet

  Get status of gear indicator.
  
  Parameters:
  void
  
  Returns:
  Light status
*/

uint8_t CarModelGearDGet(void);


/***********************************************************
  Function: CarModelGearSGet

  Get status of gear indicator.
  
  Parameters:
  void
  
  Returns:
  Light status
*/

uint8_t CarModelGearSGet(void);



#ifdef __cplusplus
}
#endif

#endif /* _CARMODEL_H_ */

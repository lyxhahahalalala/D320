/************************************************************************************************
PROJECT : Serial flash calibration
FILE    : $Id: r_test_sfma.c 1582 2013-10-24 14:42:36Z oliver.achten $
============================================================================
DESCRIPTION
Main funcitons of the SFMA test application
============================================================================
                            C O P Y R I G H T
============================================================================
                           Copyright (c) 2013
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
  Title: Serial flash calibration

*/

#ifndef R_SFMA_CALIB_H
#define R_SFMA_CALIB_H

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Section Includes
*/

#include "r_sfma_api.h"


typedef enum
{
    SF_ERR_OK = 0,
    SF_ERR_NG,
} sf_Error_t;


/***************************************************************************
  typedef: sf_CalConfig_t

  Description:
  This type describes the calibration configuration of the unit.

  Struct members:

  TestSize              - how many bytes to test
  TestStartAddressOs    - offset of the test area from the base address
  BaseMemoryAddr        - serial Flash data base memory address

*/
typedef struct
{
    uint32_t TestSize;
    uint32_t TestStartAddressOs;
    uint32_t BaseMemoryAddr;
} sf_CalConfig_t;


/***************************************************************************
  Function: SF_CalibrationInit

  Description:
  Initialisation function for the calibration routine

  Parameter:
  Unit       - The parameter specifies the instance number.
  CalConfig  - This is a pointer to the <r_sfma_CalConfig_t> structure to the
               configuration of the calibration unit.

  Return value:
  See <sf_Error_t>.
*/
sf_Error_t SF_CalibrationInit(uint32_t Unit, const sf_CalConfig_t * const CalConfig);


/***************************************************************************
  Function: R_SFMA_CalibrationWriteData

  Description:
  Support function for the calibration routine writes data to the memory

  Parameter:
  Unit    - The parameter specifies the instance number.
  Config  - This is a pointer to the <r_sfma_Config_t> structure to the
            configuration of the unit.

  Return value:
  See <sf_Error_t>.
*/
sf_Error_t SF_CalibrationWriteData(uint32_t Unit, r_sfma_Config_t * Config);


/***************************************************************************
  Function: SF_CalibrationRun

  Description:
  Support function for the calibration routine verifies data from the memory

  Parameter:
  Unit    - The parameter specifies the instance number.
  Config  - This is a pointer to the <r_sfma_Config_t> structure to the
            configuration of the unit.
  f_phclk - phase clock supplied to the SFMA macro
  f_b     - output serial clock frequency to the serial flach device x 2

  Return value:
  See <sf_Error_t>.
*/
sf_Error_t SF_CalibrationRun(uint32_t Unit, r_sfma_Config_t * Config, uint32_t f_phclk, uint32_t f_b);


#ifdef __cplusplus
}
#endif

#endif /* R_SFMA_CALIB_H */


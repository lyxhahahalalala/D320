/*
****************************************************************************
PROJECT : VLIB macro driver
FILE    : $Id: r_csisw_api.h 7640 2016-02-12 13:14:23Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Generic part of the macro driver - CSISW (software CSI)
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

#ifndef R_CSISW_API_H_
#define R_CSISW_API_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Title: CSISW Driver API 
  
  An application using CSISW should only need to include this r_csisw_api.h
*/

/*******************************************************************************
  Section: Global Constants 
*/


/*******************************************************************************
  Constant:  R_CSISW_VERSION_HI and R_CSISW_VERSION_LO

  Driver version information  
*/

#define R_CSISW_VERSION_HI 0
#define R_CSISW_VERSION_LO 1

/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Enum: r_csisw_Error_t

  CSISW macro driver error code.
  
  If an error occures these enums give information about the
  reason.

  Values:
  R_CSISW_ERR_OK           - No error
  R_CSISW_ERR_NG           - Unspecified error
*/

typedef enum
{
    R_CSISW_ERR_OK         = 0x0,
    R_CSISW_ERR_NG         ,
    R_CSISW_ERR_RANGE      ,
    R_CSISW_ERR_LOCKED     ,
    R_CSISW_ERR_NOTLOCKED  
} r_csisw_Error_t;

/*******************************************************************************
  Enum: r_csisw_TxRxMode_t

  Tx-only, Rx-only or Tx and Rx configuration

  Values:
    R_CSISW_TX_ONLY   - Tx-only mode
    R_CSISW_RX_ONLY   - Rx-only mode
    R_CSISW_TX_RX     - Tx and Rx enabled mode
    R_CSISW_TX_RX_OFF - Tx and Rx disabled (idle/init state)
*/

typedef enum {
    R_CSISW_TX_ONLY,
    R_CSISW_RX_ONLY,
    R_CSISW_TX_RX,
    R_CSISW_TX_RX_OFF
} r_csisw_TxRxMode_t;

/*******************************************************************************
  Section: Global API Functions
*/

/*******************************************************************************
  Function: R_CSISW_GetVersionStr

  Get the driver version number in human readable form (string). 

  Parameters:
  void
  
  Returns:
  ReceiveString     - Pointer to receive string destination
  
*/

const int8_t * R_CSISW_GetVersionStr(void);

/*******************************************************************************
  Function: R_CSISW_GetMajorVersion

  Get the driver major version number. 

  Parameters:
  void
  
  Returns:
     - Major version number
  
*/

const uint16_t R_CSISW_GetMajorVersion(void); 

/*******************************************************************************
  Function: R_CSISW_GetMinorVersion

  Get the driver minor version number. 

  Parameters:
  void
  
  Returns:
     - Minor version number
  
*/

const uint16_t R_CSISW_GetMinorVersion(void); 


/*******************************************************************************
  Function: R_CSISW_Init

  Driver init function.

  Parameters:
  Unit          - Macro instance number
 
  Returns:
  see: <r_csisw_Error_t>
*/

r_csisw_Error_t R_CSISW_Init(uint32_t Unit);


/*******************************************************************************
  Function: R_CSISW_Enable

  Driver enable function.

  Parameters:
  Unit          - Macro instance number
 
  Returns:
  see: <r_csisw_Error_t>
*/

r_csisw_Error_t R_CSISW_Enable(uint32_t Unit, r_csisw_TxRxMode_t TxRxMode);


/*******************************************************************************
  Function: R_CSISW_Read

  Read one CSI value by software bitbanging.

  Parameters:
  Unit         - Instance number
  Buffer       - Destination array for the Data to be read
  MaxNum       - Max number of data that can be read

  Returns:
  see: <r_csisw_Error_t>
*/

r_csisw_Error_t R_CSISW_Read(uint32_t Unit, void *Buffer, uint32_t MaxNum);


/*******************************************************************************
  Function: R_CSISW_Write

  Write one CSI value by software bitbanging.

  Parameters:
  Unit         - Instance number
  Buffer       - Data to be copied
  Num          - Number of data

  Returns:
  see: <r_csisw_Error_t>
*/

r_csisw_Error_t R_CSISW_Write(uint32_t Unit, void *Buffer, uint32_t Num);


/*******************************************************************************
  Function: R_CSISW_DeInit

  Driver deinit function.

  Parameters:
  Unit       - Macro Instance number

  Returns:
  see: <r_csisw_Error_t>
*/

r_csisw_Error_t R_CSISW_DeInit(uint32_t Unit);


#ifdef __cplusplus
}
#endif

#endif /* R_CSISW_API_H_  */

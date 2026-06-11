/*
****************************************************************************
PROJECT : BSP - GFX - VI SAA71XX driver
FILE    : $Id: saa71xx_api.h 7640 2016-02-12 13:14:23Z florian.zimmermann $
============================================================================
DESCRIPTION
BSP Implementation D1M2(H) MANGO Board
SAA71XX driver (API)
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

#ifndef R_SAA71XX_API_H_
#define R_SAA71XX_API_H_

#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************
  Title: SAA71XX API

  An application using SAA71XX should include this header file.

*/

/***********************************************************
  Section: Global Types
*/

/***********************************************************
  Enum: R_SAA71xx_Error_t

  SAA71XX macro driver error code.

  Values:
  R_SAA71XX_ERR_OK          - No error
  R_SAA71XX_ERR_NG          - Unspecified error
  R_SAA71XX_ERR_PORTINIT    - SAA71XX port init failed
  R_SAA71XX_ERR_LAST        - Delimeter 
*/

typedef enum {
    R_SAA71XX_ERR_OK         = 0x00,
    R_SAA71XX_ERR_NG         = 0x01,
    R_SAA71XX_ERR_PORTINIT,
    R_SAA71XX_ERR_LAST
} R_SAA71xx_Error_t;

typedef enum {
    R_DS3231_ERR_OK         = 0x00,
    R_DS3231_ERR_NG         = 0x01,
    R_DS3231_ERR_PORTINIT,
    R_DS3231_ERR_LAST
} R_DS3231_Error_t;


/***********************************************************
  Enum: R_SAA71xx_VideoMode_t

  Video input mode.

  Values:
  R_SAA71XX_CVBS            - CVBS Mode
  R_SAA71XX_SVIDEO          - SVIDEO Mode
*/

typedef enum {
    R_SAA71XX_CVBS,
    R_SAA71XX_SVIDEO
} R_SAA71xx_VideoMode_t;


/***********************************************************
  Enum: R_SAA71xx_Type_t

  Identifier of the device type.

  Values:
  R_SAA71XX_7113 - SAA7113
  R_SAA71XX_7118 - SAA7118
  R_SAA71XX_NONE - NONE
*/

typedef enum {
    R_SAA71XX_7113 = 0,
    R_SAA71XX_7118 = 1,
    R_SAA71XX_NONE = 0xff
} R_SAA71xx_Type_t;


/***********************************************************
  Type: R_SAA71xx_Info_t

  Probe information data.

  Members:
  Addr    - Address
  Type    - Device type, see <R_SAA71xx_Type_t>
  Name    - Device name, string.
*/

typedef struct {
    uint32_t           Addr;
    R_SAA71xx_Type_t  Type;
    uint8_t          *Name;
} R_SAA71xx_Info_t;


/***********************************************************
  Section: Global API Functions
*/

/***********************************************************
  Function: R_SAA71xx_Init

  Driver init function.

  It is probed for the type of saa (7113/7118). Different 
  settings for the  saa are saved in the file config_saa.h 
  and can be activated by  recompile with the new setting. 

  Parameters:
  Unit       - Instance number
 
  Returns:
  see: <R_SAA71xx_Error_t>
*/

R_SAA71xx_Error_t R_SAA71xx_Init(uint32_t Unit);
R_DS3231_Error_t R_DS3231_Init(uint32_t Unit);


/***********************************************************
  Function: R_SAA71xx_SwitchMode

  Switch saa to svideo or cvbs mode.
 
  Parameters:
  Unit       - Instance number
  Mode       - see: <R_SAA71xx_VideoMode_t>

  Returns:
  see: <R_SAA71xx_Error_t>
*/

R_SAA71xx_Error_t R_SAA71xx_SwitchMode(uint32_t Unit, 
                                              R_SAA71xx_VideoMode_t Mode);

/***********************************************************
  Function: R_SAA71xx_SwitchChannel

  Switch to channel.

  This is only possible if the saa is in cvbs mode. Then only one
  of the connectors on the board is needed. Then 2 channels
  are present. They corrospondent to the 2 connectors present on the
  board. 

  Parameters:
  Unit       - Instance number
  Channel    - Channel number 0..1

  Returns:
  see: <R_SAA71xx_Error_t>
*/

R_SAA71xx_Error_t R_SAA71xx_SwitchChannel(uint32_t Unit, uint32_t Channel);


/***********************************************************
  Function: R_SAA71xx_GetInfo

  Read info data.

  This functions writes information about the probed saa 
  during the init phase into a specified structure.

  Parameters:
  Unit       - Instance number
  Info       - Pointer to an R_SAA71xx_Info_t structure

*/

R_SAA71xx_Error_t R_SAA71xx_GetInfo(uint32_t Unit, 
                                           R_SAA71xx_Info_t *Info);


/***********************************************************
  Function: R_SAA71xx_Disable

  Disable the data output of the saa.

  Parameters:
  Unit       - Instance number

  Returns:
  see: <R_SAA71xx_Error_t>
*/

R_SAA71xx_Error_t R_SAA71xx_Disable(uint32_t Unit);


/***********************************************************
  Function: R_SAA71xx_DeInit

  Driver deinit function.

  Parameters:
  Unit       - Instance number

  Returns:
  see: <R_SAA71xx_Error_t>
*/

R_SAA71xx_Error_t R_SAA71xx_DeInit(uint32_t Unit);

/***********************************************************
  Function: R_SAA71xx_SetErrorCallback
  
  Assign a user function as error handler.

  Parameters:
  *ErrorCallback       - pointer to a user function with 2 paramters (unit and error number)
                        

  Returns:
  void
*/

void R_SAA71xx_SetErrorCallback(void (*ErrorCallback)(uint32_t Unit, 
                                       uint32_t Error));


#ifdef __cplusplus
}
#endif

#endif /* R_SAA71XX_API_H_  */

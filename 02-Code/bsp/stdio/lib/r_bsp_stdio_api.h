/*
****************************************************************************
PROJECT : VLIB - BSP API
FILE    : $Id: r_bsp_stdio_api.h 6500 2015-10-30 16:48:14Z jonathan.grant $
============================================================================ 
Description
API definition for the application test framework
============================================================================
                            C O P Y R I G H T                            
============================================================================
                           Copyright (c) 2012
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
#ifndef BSP_STDIO_API_H_
#define BSP_STDIO_API_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Title: IP test API

  Any application using the BSP STDIO functions 
  should include this header file
  
*/
/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Function: R_BSP_STDIO_Init

  Board/device dependant standard IO init .

 
  Parameters:
  void
    
  Returns:
  void
*/  

void R_BSP_STDIO_Init(void); 

/*******************************************************************************
  Function: R_BSP_STDIO_DeInit

  Board/device dependant standard IO De-Init .

 
  Parameters:
  void
    
  Returns:
  void
*/  

void R_BSP_STDIO_DeInit(void); 



/*******************************************************************************
  Function: R_BSP_STDIO_Printf

  Function to substitute Printf

  Parameters:
  String        - Format string
  ...           - Parameter list 
  
  Returns:
  void
*/

void R_BSP_STDIO_Printf(char *String, ...)
#ifndef _WINDOWS
    __attribute__ ((__format__ (__printf__, 1, 2)));
#else
    ;
#endif

/********************************************************************************
  Function: R_BSP_STDIO_AnPrintf

  Printf for the alpha-numeric display 
  
  If the board doe not have such display, the standard printf 
  (debugger dependant) is used 

  Parameters:
    Line    -   Output to line 1 or 2 
    String  -   printf string 
  Returns:
  void  -

*/

void R_BSP_STDIO_AnPrintf(uint8_t Line, char *String,  ...)
#ifndef _WINDOWS
    __attribute__ ((__format__ (__printf__, 2, 3)));
#else
    ;
#endif


/*******************************************************************************
  Function: R_BSP_STDIO_ReadChar

  Reads a Character.
  
  Reads int8_t out of the input stream and copies to the 
  address 'Character'.

  Parameters:
  Character          - Address of Character Buffer
  
  Returns:
  void
*/

void R_BSP_STDIO_ReadChar(int8_t *Character);




/*******************************************************************************
  Function: R_BSP_STDIO_ReceiveChar

  Reads a Character.
  
  Reads int8_t out of the input stream and copies to the 
  address 'Character'. This fuction waits for input 

  Parameters:
  Character          - Address of Character Buffer
  
  Returns:
  void
*/

void R_BSP_STDIO_ReceiveChar(int8_t *Character);


/*******************************************************************************
  Function: R_BSP_STDIO_WriteString

  Prints a zero terminated string
  
  Parameters:
  Buffer        - Zero terminated string
  
  Returns:
  void
*/
void R_BSP_STDIO_WriteString(char *Buffer);


/*******************************************************************************
  Function: R_BSP_STDIO_ReadString

  Reads maximum ('ReceiveMaxSize' - 1) charters and copies them 
  to 'ReceiveString'.
 
  The string will be terminated with a zero if a CR will be 
  detected or ('ReceiveMaxSize' - 1) is reached.

  Parameters:
  ReceiveString     - Pointer to receive string destination
  ReceiveMaxSize    - Maximum number of charaters to read
  TimeoutCount      - Time out value
  
  Returns:
  void
*/

void R_BSP_STDIO_ReadString(char *ReceiveString, uint32_t ReceiveMaxSize, uint32_t TimeOutCnt);


/*******************************************************************************
  Function: R_BSP_STDIO_ReadValue

  Reads charters and convert them to a number.
  
  Waits for CR limited string input and converts it into 
  a number.

  Parameters:
  void
  
  Returns:
  Converted number
  
*/

uint32_t R_BSP_STDIO_ReadValue(void);



/*******************************************************************************
  Function: R_BSP_STDIO_ReadHex

  Reads a string of max 20 Characters and format this into the hex value.
  
  Waits for CR limited string input (MSB) and converts it into 
  a number.

  Parameters:
  void
  
  Returns:
  Converted number
  
*/

void R_BSP_STDIO_ReadHex(uint32_t *ReceiveHex); 





/*******************************************************************************
  Function: R_BSP_STDIO_SetComPort

  Assign com port for ATF output 
  Use parameter value 0xff to disable UART and enable debugger out 

  Parameters:
  Unit          - Uart channel number 

  Returns:
  void  -
*/

void R_BSP_STDIO_SetComPort(uint32_t Unit);



#ifdef __cplusplus                                   
}                                                    
#endif

#endif /* BSP_STDIO_API_H_*/

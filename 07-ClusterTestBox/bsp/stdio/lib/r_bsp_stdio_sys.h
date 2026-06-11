/*
****************************************************************************
PROJECT : VLIB - BSP API
FILE    : $Id: r_bsp_stdio_sys.h 7262 2016-01-15 13:34:37Z michael.golczewski $
============================================================================ 
Description
Main Module Header for IP test frame
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

#ifndef BSP_STDIO_SYS_H_
#define BSP_STDIO_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Section: Board specific STDIO Service Functions
  
  These functions must be implemented specific in
  'vlib/bsb/board/<any board>/src/stdio/r_bsp_sys_stdio.c'
*/

/*******************************************************************************
  Function: R_BSP_SYS_STDIO_Init

  Initialise the STDIO channel with board defaults   
  Parameters:
  void 
  
  Returns:
  void

*/

void R_BSP_SYS_STDIO_Init(void); 

/*******************************************************************************
  Function: R_BSP_SYS_STDIO_DeInit

  Clear the stdio initialiation  
  
  Parameters:
  void 
  
  Returns:
  void
*/

void R_BSP_SYS_STDIO_DeInit(void); 
    
/*******************************************************************************
  Function: R_BSP_SYS_STDIO_GetActCh
  
  Return the currently active IO chanel 

  Parameters:
  void 
  
  Returns:
   = 0xFF    - Debugger IO is in use 
  != 0xFF    - the UART unit currently used    
  
*/
uint8_t R_BSP_SYS_STDIO_GetActCh(void); 
    
/*******************************************************************************
  Function: R_BSP_SYS_STDIO_SetUart
  Set the UART port for printf output
  
  Parameters:
  Unit        -  UART unit 
  
  Returns:
  void

*/

void R_BSP_SYS_STDIO_SetUart(uint32_t Unit); 


/*******************************************************************************
  Function: R_BSP_SYS_STDIO_Read

  Reads data which has been received.
  
  Maximum 'Max' data is read.

  Parameters:
  String     - Buffer for storing the data
  Max        - Size of the data Buffer

  Returns:
  Number of characters read.
*/

uint32_t R_BSP_SYS_STDIO_Read(uint8_t * String, uint32_t Max) ;


/*******************************************************************************
  Function: R_BSP_SYS_STDIO_Write

  Writes 'Len' number of charaters.

  Parameters:
  Buffer    - Data Buffer
  Len       - Number of bytes to transfer from the Buffer

  Returns:
  void
*/

void R_BSP_SYS_STDIO_Write(uint8_t * Buffer, uint32_t Len);

/*******************************************************************************
  Function: AlphaNumDisplayWrite

    write string to alpha numerical dot-matrix display (DOGM)

    Parameters:
    String      - the string to be output
    Line        - Line to use     

    Returns:
    == R_ERR_OK     - write succesfull
    != R_ERR_OK     - fail    

*/
r_Error_t R_BSP_SYS_STDIO_AnWrite(char *String, uint8_t Line); 

#ifdef __cplusplus
}
#endif

#endif /* BSP_STDIO_SYS_H_ */


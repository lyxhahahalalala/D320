/*
****************************************************************************
PROJECT : VLIB - BSP API
FILE    : $Id: r_bsp_api.h 7262 2016-01-15 13:34:37Z michael.golczewski $
============================================================================ 
DESCRIPTION
BSP API - API for the board support functions
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

#ifndef BSP_API_H_
#define BSP_API_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Title: BSP API

  An application using the Board Support Package should include this header file.
*/

/*******************************************************************************
  Section: Global constants
    Define the maximum character length of board information strings. 
*/

#define BSP_MAX_INFO_LENGTH 30

#define R_BSP_HAS_SDRAM             0x1u
#define R_BSP_HAS_SERIAL_FLASH      0x2u
#define R_BSP_HAS_VOUT0             0x4u
#define R_BSP_HAS_VOUT1             0x8u  
#define R_BSP_HAS_DDR_SF            0x10u
#define R_BSP_HAS_8BIT_SF           0x20u

#define R_BSP_MEM_VRAM              0x1u    /* If multiple VRAMs available, treat them as one. */
#define R_BSP_MEM_VRAM0             0x2u
#define R_BSP_MEM_VRAM1             0x4u
#define R_BSP_MEM_SDRAM             0x8u
#define R_BSP_MEM_SERIAL_FLASH      0x10u



/*******************************************************************************
  Section: Global Types
*/


/*******************************************************************************
  Section: Global API Functions
*/

/*******************************************************************************
  Function: R_BSP_LowInit

  Init BSP hardware on a lower (before main() ) level.

  Parameters:
  void

  Returns:
  == E_OK    - init successfully
  != E_OK    - init failed
*/

void R_BSP_LowInit(void);

/*******************************************************************************
  Function: R_BSP_Init

  Init BSP hardware.

  Parameters:
  void

  Returns:
  == E_OK    - init successfully
  != E_OK    - init failed
*/

r_Error_t R_BSP_Init(void);


/*******************************************************************************
  Function: R_BSP_DeInit

  De-Init BSP hardware.

  Parameters:
  void

  Returns:
  == R_ERR_OK    - deinit successfully
  != R_ERR_OK    - deinit failed
*/

r_Error_t R_BSP_DeInit(void);


/*******************************************************************************
  Function: R_BSP_Capabilities

  Return information about availability of HW functionalities .
  Like i.e HAS_SDRAM, HAS_SERIAL_FLASH, etc 
    
  Parameters:
  void

  Returns:
  bit mask 
*/

uint32_t R_BSP_Capabilities(void); 

/*******************************************************************************
  Function: R_BSP_MemoryAddressAndSize

  Return information about addresses and sizes of memories.
  Sizes and addresses of unavailable memories will be set to zero.
    
  Parameters:
  MemType   - For instance R_BSP_MEM_VRAM, R_BSP_MEM_VRAM0, R_BSP_MEM_VRAM1, R_BSP_MEM_SDRAM, R_BSP_MEM_SERIAL_FLASH
  *Address  - Reference to 32-bit variable to receive the requested address.
  *Size     - Reference to 32-bit variable to receive the requested size.

  Returns:
  == R_ERR_OK    - request successfully
  != R_ERR_OK    - request failed
*/

r_Error_t R_BSP_MemoryAddressAndSize(uint32_t MemType, uint32_t *Address, uint32_t *Size);

/*******************************************************************************
  Function: R_BSP_GetBoardName

  Get the board name in human readable form. 

  Parameters:
  void
  
  Returns:
  ReceiveString     - Pointer to receive string destination
  
*/

const int8_t * R_BSP_GetBoardName(void);  


/*******************************************************************************
  Function: R_BSP_GetBoardVersion

  Get the board version (as character string) in human readable form 

  Parameters:
  void
  
  Returns:
  ReceiveString     - Pointer to receive string destination
  
*/

const int8_t * R_BSP_GetBoardVersion(void);  


//#ifdef BSP_PROVIDE_INFO
/*******************************************************************************
  Function: R_BSP_PrintClkCfg

  This prints the current clock configuration in uer readable formant 
  Just for information and debugging purposes (and thus exported in the API only 
  in case BSP_PROVIDE_INFO is set)
*/

void R_BSP_PrintClkCfg(void); 
//#endif 

#ifdef __cplusplus
}
#endif

#endif /* R_BSP_API_H */

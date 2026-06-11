/*
****************************************************************************
PROJECT : ISM driver
FILE ID : $Id: r_ism_sys.h 4598 2015-01-09 18:27:23Z tobyas.hennig $
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

#ifndef ISM_SYS_H_
#define ISM_SYS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: ISM Support Functions

  The generic ISM driver uses these functions. They have
  to be implemented within the driver library for a concrete
  device.
*/

/*******************************************************************************
  Section: Global Functions
*/


/*******************************************************************************
  Function: R_SYS_ISM_SysSetClockHz

  See: r_sys_ism.h for details    

*/
uint32_t R_SYS_ISM_SysSetClockHz(uint32_t Unit, uint32_t freq);


/*******************************************************************************
  Function: R_SYS_ISM_SysGetClockHz

  See: r_sys_ism.h for details    

*/
uint32_t R_SYS_ISM_SysGetClockHz(uint32_t Unit);


/*******************************************************************************
  Function: R_SYS_ISM_SetPort

  See: r_sys_ism.h for details    

*/
void R_SYS_ISM_SetPort( uint32_t Unit );


/*******************************************************************************
  Function: R_SYS_ISM_ReleasePort

  See: r_sys_ism.h for details    

*/
void R_SYS_ISM_ReleasePort( uint32_t Unit );


/*******************************************************************************
  Function: R_SYS_ISM_EnableInt

  See: r_sys_ism.h for details    

*/
void R_SYS_ISM_EnableInt(uint32_t Unit, r_ism_Interrupt_t Interrupt); 


/*******************************************************************************
  Function: R_SYS_ISM_DisableInt

  See: r_sys_ism.h for details    

*/
void R_SYS_ISM_DisableInt(uint32_t Unit, r_ism_Interrupt_t Interrupt); 


#ifdef __cplusplus
}
#endif

#endif /* ISM_SYS_H_ */


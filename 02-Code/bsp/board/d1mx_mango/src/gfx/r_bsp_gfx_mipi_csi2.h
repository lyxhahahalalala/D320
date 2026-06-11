/*
****************************************************************************
PROJECT : BSP - GFX - MIPI CSI-2
FILE    : $Id: r_bsp_gfx_mipi_csi2.h 7640 2016-02-12 13:14:23Z florian.zimmermann $
============================================================================
DESCRIPTION
BSP Implementation - D1M2 MANGO Board
Init the MIPI CSI-2
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

#ifndef R_MIPI_CSI2_INIT_H_
#define R_MIPI_CSI2_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
  Function: R_MIPI_IsrOVF() / R_MIPI_IsrCTL()
  Interrupt service routines for MIPI controller / buffer
*/
void R_MIPI_IsrOVF(void);
void R_MIPI_IsrCTL(void);


/***********************************************************
  Function: R_BSP_SYS_GFX_MipiCsi2InputConfig() / Disable() / Enable()
  Configure / Enable / Disable the MIPI CSI-2 input
*/
void R_BSP_SYS_GFX_MipiCsi2InputConfig(uint32_t PixelClkFreqHz, uint8_t NoOfLanes);
void R_BSP_SYS_GFX_MipiCsi2InputEnable(void);
void R_BSP_SYS_GFX_MipiCsi2InputDisable(void);


/***********************************************************
  Function: R_BSP_SYS_GFX_MipiGetIsrxxxx()
  Return the current interrupt status (CTL and OVF/ OVF count )
*/
uint32_t R_BSP_GFX_MipiGetIsrStatus(void);
uint32_t R_BSP_GFX_MipiGetIsrOvfStatus(void);
uint32_t R_BSP_GFX_MipiGetIsrOvfStatusCount(void);

#ifdef __cplusplus
}
#endif

#endif /* R_MIPI_CSI2_INIT_H_ */


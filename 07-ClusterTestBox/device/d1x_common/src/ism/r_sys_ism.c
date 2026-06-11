/*
****************************************************************************
PROJECT : ISM device driver
FILE    : $Id: r_sys_ism.c 4975 2015-03-27 16:22:34Z tobyas.hennig $
============================================================================ 
DESCRIPTION
ISM system functions for r7f701412
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
  Title: upd703532 ISM Support Functions

  Implementation of the ISM support functions for the 
  upd703532 device.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_ism_api.h"
#include "r_ism_sys.h"
#include "r_dev_api.h"
#include "r_config_ism.h"

#if defined (R_DBG_PRINT_DEV_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#endif


/*******************************************************************************
  Section: Local Defines
*/


/***********************************************************
  ZDP I/O buffers
*/
#define ISM_SELBISMCHNCFG (0xFFC06028u)
#define ISM_PRMR01SSW (0x000000002)
#define ISM_PRMR02SSW (0x000000008)
#define ISM_PRMR03SSW (0x000000020)
#define ISM_PRMR04SSW (0x000000080)
#define ISM_PRMR05SSW (0x000000200)
#define ISM_PRMR06SSW (0x000000800)


/*******************************************************************************
  Section: Local Variables
*/

static const r_dev_PinConfig_t loc_PinConfigIsm[] = 
/*   Port    Pin       Func      Dir      Feedback     Pull        OpenDrain   HiCurr   HiSpeed  InputType */
{
    /* ISM stepper motors (P16_[0..11] and P17_[0..11] AF1 */
    /* ISM1x (ISM channel 0) */
    {16u,     0u,       1u, R_DEV_PIN_DIRECT,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {16u,     1u,       1u, R_DEV_PIN_DIRECT,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {16u,     2u,       1u, R_DEV_PIN_DIRECT,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {16u,     3u,       1u, R_DEV_PIN_DIRECT,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    /* ISM2x (ISM channel 1) */
    {16u,     4u,       1u, R_DEV_PIN_DIRECT,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {16u,     5u,       1u, R_DEV_PIN_DIRECT,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {16u,     6u,       1u, R_DEV_PIN_DIRECT,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {16u,     7u,       1u, R_DEV_PIN_DIRECT,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    /* ISM5x (ISM channel 4) */
    {16u,     8u,       1u, R_DEV_PIN_DIRECT,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {16u,     9u,       1u, R_DEV_PIN_DIRECT,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {16u,    10u,       1u, R_DEV_PIN_DIRECT,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {16u,    11u,       1u, R_DEV_PIN_DIRECT,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    /* ISM6x (ISM channel 5) */
    {17u,     0u,       1u, R_DEV_PIN_DIRECT,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {17u,     1u,       1u, R_DEV_PIN_DIRECT,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {17u,     2u,       1u, R_DEV_PIN_DIRECT,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {17u,     3u,       1u, R_DEV_PIN_DIRECT,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    /* ISM3x (ISM channel 2) */
    {17u,     4u,       1u, R_DEV_PIN_DIRECT,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {17u,     5u,       1u, R_DEV_PIN_DIRECT,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {17u,     6u,       1u, R_DEV_PIN_DIRECT,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {17u,     7u,       1u, R_DEV_PIN_DIRECT,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    /* ISM4x (ISM channel 3) */
    {17u,     8u,       1u, R_DEV_PIN_DIRECT,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {17u,     9u,       1u, R_DEV_PIN_DIRECT,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {17u,    10u,       1u, R_DEV_PIN_DIRECT,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {17u,    11u,       1u, R_DEV_PIN_DIRECT,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    /* delimiter - do not remove */
    {0u, R_DEV_PIN_LAST,0u,R_DEV_PIN_OUT,0u,R_DEV_PIN_PULLNO,     0u,      0u,         0u,    R_DEV_PIN_CMOS1}  
};

static const r_dev_PinConfig_t loc_PinConfigIsmDisable[] = 
/*   Port    Pin       Func      Dir      Feedback     Pull        OpenDrain   HiCurr   HiSpeed  InputType */
{
    /* Disable ISM stepper motors - set ports to GPIO input (P16_[0..11] and P17_[0..11]) */
    {16u,     0u,       0u, R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {16u,     1u,       0u, R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {16u,     2u,       0u, R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {16u,     3u,       0u, R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {16u,     4u,       0u, R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {16u,     5u,       0u, R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {16u,     6u,       0u, R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {16u,     7u,       0u, R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {16u,     8u,       0u, R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {16u,     9u,       0u, R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {16u,    10u,       0u, R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {16u,    11u,       0u, R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {17u,     0u,       0u, R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {17u,     1u,       0u, R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {17u,     2u,       0u, R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {17u,     3u,       0u, R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {17u,     4u,       0u, R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {17u,     5u,       0u, R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {17u,     6u,       0u, R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {17u,     7u,       0u, R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {17u,     8u,       0u, R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {17u,     9u,       0u, R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {17u,    10u,       0u, R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    {17u,    11u,       0u, R_DEV_PIN_IN,    0u,  R_DEV_PIN_PULLNO,     0u,      1u,         1u,   R_DEV_PIN_CMOS1},  
    /* delimiter - do not remove */
    {0u, R_DEV_PIN_LAST,0u,R_DEV_PIN_OUT,0u,R_DEV_PIN_PULLNO,     0u,      0u,         0u,    R_DEV_PIN_CMOS1}  
};


/*******************************************************************************
  Constant: loc_BaseAddress

  Base address table of Ism macros
*/
static const uint32_t loc_BaseAddress[R_ISM_MACRO_NUM] = 
{
/*ism0*/     0xFFF00000u,
};


/*******************************************************************************
  Section: ISM Macro Driver Support Functions
  
  Comments see: <ism_sys.h>
*/


/*******************************************************************************
  Function: R_SYS_ISM_SysSetClockHz

  See: r_sys_ism.h for details    

*/

uint32_t R_SYS_ISM_SysSetClockHz(uint32_t Unit, uint32_t freq)
{
    static const r_dev_ClkSelConfig_t loc_ClkSelCfgIsm = 
    /*   Cks             SrcId                 Divider   tpReqMsk      */
    {R_DEV_CKS_ISM ,     R_DEV_CKS_SRC_CLKJIT,      1,   0};

    R_DEV_ClkIdSet(&loc_ClkSelCfgIsm);
    freq = R_DEV_ClkFrequencyHz(R_DEV_CKS_ISM); 
    return freq;
}


/*******************************************************************************
  Function: R_SYS_ISM_SysGetClockHz

  See: r_sys_ism.h for details    

*/

uint32_t R_SYS_ISM_SysGetClockHz(uint32_t Unit) 
{
    uint32_t val;
    val = R_DEV_ClkFrequencyHz(R_DEV_CKS_ISM); 
    return val;
}


/*******************************************************************************
  Function: R_SYS_ISM_BaseAddr

  See: r_ism_sys.h for details    

*/
uint32_t R_SYS_ISM_BaseAddr(uint32_t Unit) 
{
    uint32_t addr;
    if (Unit >= R_ISM_MACRO_NUM) 
    {
        return R_BAD_ADDRESS; 
    }
    addr = loc_BaseAddress[Unit];
    return addr;
}


/*******************************************************************************
  Function: R_SYS_ISM_EnableInt
*/

void R_SYS_ISM_EnableInt(uint32_t Unit, r_ism_Interrupt_t Interrupt) 
{
    switch (Unit)
    {
    case 0:
        R_DEV_IntEnable (R_DEV_INT_ISM0REACHED, 1); //IRQ_REACHED indicates that a motor has reached its target/zero position.
        R_DEV_IntEnable (R_DEV_INT_ISM0DONE,    1); 
        R_DEV_IntEnable (R_DEV_INT_ISM0ZPDAD,   1); //IRQ_ZPDAD indicates when the actual ZPD measuremen ts start after the configured blanking delay:
        break;
    default:
        R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "ISM: We do not have this Unit");      
        break; 
    }    
}


/*******************************************************************************
  Function: R_SYS_ISM_DisableInt
*/

void R_SYS_ISM_DisableInt(uint32_t Unit, r_ism_Interrupt_t Interrupt) {
    switch (Unit)
    {
    case 0:
        R_DEV_IntEnable (R_DEV_INT_ISM0REACHED, 0); 
        R_DEV_IntEnable (R_DEV_INT_ISM0DONE,    0); 
        R_DEV_IntEnable (R_DEV_INT_ISM0ZPDAD,   0); 
        break;
    default:
        R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "ISM: We do not have this Unit");      
        break; 
    }    
}


/***********************************************************
  Function: ISM Port Miscellaneous Settings
*/

void R_SYS_ISM_SetPort( uint32_t Unit ) {

    R_DEV_PinInit (loc_PinConfigIsm);                      //configure ism pins using pin configure table

    /* enable the all ports for ZDP operation */
    R_DEV_WRITE_REG(32, ISM_SELBISMCHNCFG,  ISM_PRMR01SSW | 
                                            ISM_PRMR02SSW |
                                            ISM_PRMR03SSW |
                                            ISM_PRMR04SSW |
                                            ISM_PRMR05SSW |
                                            ISM_PRMR06SSW );//use ZDP function
}


/***********************************************************
  Function: ISM to release any output drive (set to gpio input)
*/

void R_SYS_ISM_ReleasePort( uint32_t Unit )
{
    R_DEV_PinInit (loc_PinConfigIsmDisable);

    /* disable the ZDP port buffers */
    R_DEV_WRITE_REG(32, ISM_SELBISMCHNCFG,  0); 
}


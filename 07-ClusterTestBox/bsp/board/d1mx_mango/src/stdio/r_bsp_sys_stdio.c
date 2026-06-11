/*
****************************************************************************
PROJECT : BSP STDIO
FILE    : $Id: r_bsp_sys_stdio.c 4988 2015-03-30 13:06:17Z golczewskim $
============================================================================
DESCRIPTION
BSP Implementation D1X MANGO Board support sys functions - STDIO
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

/*******************************************************************************
  Section: Includes
*/
#include "r_typedefs.h"
#include "r_bsp_stdio_api.h"
#include "r_bsp_stdio_sys.h"

#ifdef USE_RLIN3
#include "r_rlin3_api.h"
#include "r_dev_api.h"
#include "r_bsp_board_config.h"

/*******************************************************************************
  Section: Local Defines
*/

/*******************************************************************************
  Defines for port assignment of Uart (stdio printf)

    The Uart is connected on the D1M2(H) Mango Board
    to the following port pins.

        Uart TX     -> P0_8 (alt. function 1)
        Uart RX     -> P0_9 (alt. function 1)
     
     This requires the jumper connection
        Set Jumper JP22 2-3    (for Uart RX)
        Set Jumper JP41        (for Uart TX)
*/

#define LOC_TX0_PORT  0
#define LOC_TX0_PIN   8
#define LOC_TX0_AFUNC 1
#define LOC_RX0_PORT  0
#define LOC_RX0_PIN   9
#define LOC_RX0_AFUNC 1


/*******************************************************************************
  Section: Local Variables
*/
/* Variable: loc_CommPort 
   UART macro number used for communiction */
static uint8_t loc_CommPort = BSP_DEFAULT_UART; 


/*******************************************************************************
  Constant: loc_PinConfigStdioUart

  UART pin configuration set up
*/

static const r_dev_PinConfig_t loc_PinConfigStdioUart[] = 
/*  Port           Pin          Func           Dir             Feedback     Pull        OpenDrain   HiCurr   HiSpeed  InputType */
{
    {LOC_TX0_PORT, LOC_TX0_PIN, LOC_TX0_AFUNC, R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  /* Uart 0 TX */
    {LOC_RX0_PORT, LOC_RX0_PIN, LOC_RX0_AFUNC, R_DEV_PIN_IN,     0u,  R_DEV_PIN_PULLNO,     0u,      0u,         0u,   R_DEV_PIN_CMOS1},  /* Uart 0 RX */
    /* delimiter - do not remove */
    {0u,            R_DEV_PIN_LAST,0u,           R_DEV_PIN_OUT,    0u,  R_DEV_PIN_PULLNO,    0u,      0u,         0u,    R_DEV_PIN_CMOS1}  
};  

/*******************************************************************************
  Constant: loc_ClkSelectionCfg

  UART cloks 
*/

static const r_dev_ClkSelConfig_t loc_ClkSelectionCfg[] = 
/*   Cks                   SrcId                Divider     tpReqMsk      */
{
    {R_DEV_CKS_RLIN ,     R_DEV_CKS_SRC_PLLFIX_10,   0,   0},  
    /* delimiter - do not remove */
    {R_DEV_CKS_LAST  ,     R_DEV_CKS_SRC_MOSC,  0,   0}
};  
/*******************************************************************************
  Section: Global Device dependent IO Functions


*/

/*******************************************************************************
  Function: R_BSP_SYS_STDIO_SetUart
    
    see <r_bsp_stdio_sys.h> for details
*/

void R_BSP_SYS_STDIO_SetUart(uint32_t Unit) 
{
    r_rlin3_Parameter_t uartparams;
    uint8_t io_state  = 0; 
    if (Unit < 0xFF)
    {    
        if (loc_CommPort != Unit)
        {    
            /* Store original IO state (enabled or disabled) */
            io_state = R_DEV_PinOutStateGet(); 

            #ifdef BSP_FORCE_UART
            /* Force UART IO to override the global IO disable setting */
            R_DEV_PinOutStateSet(1); 
            #endif
            R_DEV_ClkSelection(loc_ClkSelectionCfg);
            /* Pin Init */  
            R_DEV_PinInit(loc_PinConfigStdioUart); 
            
            /* restore previous IO state */
            R_DEV_PinOutStateSet(io_state); 
        }
        
        loc_CommPort = Unit; 
        R_RLIN3_DeInit(loc_CommPort);

        /* UART Init */
        /* unit and settings of macro which is used for menu I/O */
        uartparams.Baudrate    = 115200;
        uartparams.Parity      = R_RLIN3_NONE_PARITY;
        uartparams.Stopbit     = R_RLIN3_ONE_STOPBIT;
        uartparams.Direction   = R_RLIN3_LSB_FIRST;
        uartparams.CharLength  = R_RLIN3_CHARLEN_8BIT;  
        uartparams.TxDataLevel = R_RLIN3_TDL_NOT_INVERTED;
        uartparams.RxDataLevel = R_RLIN3_RDL_NOT_INVERTED;
        uartparams.TxEnable    = R_RLIN3_TX_ENABLED;
        uartparams.RxEnable    = R_RLIN3_RX_ENABLED;
        uartparams.Detection   = R_RLIN3_DET_BIT_OVR;
        R_RLIN3_Init(Unit, &uartparams);
    }
    loc_CommPort = Unit; 

}

/*******************************************************************************
  Function: R_BSP_SYS_STDIO_Init

    see <r_bsp_stdio_sys.h> for details
*/

void R_BSP_SYS_STDIO_Init(void) 
{
    uint8_t io_state = 0; 
    if (loc_CommPort < 0xFF)
    {    
        /* Store original IO state (enabled or disabled) */
        io_state = R_DEV_PinOutStateGet(); 

        #ifdef BSP_FORCE_UART
        /* Force UART IO to override the global IO disable setting */
        R_DEV_PinOutStateSet(1); 
        #endif
        
        R_DEV_ClkSelection(loc_ClkSelectionCfg);

        R_RLIN3_DeInit(loc_CommPort);
        /* Pin Init */  
        R_DEV_PinInit(loc_PinConfigStdioUart); 

        /* restore previous IO state */
        R_DEV_PinOutStateSet(io_state); 

        /* UART Init */
        R_BSP_SYS_STDIO_SetUart(loc_CommPort); 
    }
    
}

/*******************************************************************************
  Function: R_BSP_SYS_STDIO_GetActCh
  
    see <r_bsp_stdio_sys.h> for details
*/
uint8_t R_BSP_SYS_STDIO_GetActCh(void)
{
    return loc_CommPort; 
}
/*******************************************************************************
  Function: R_BSP_SYS_STDIO_DeInit

    see <r_bsp_stdio_sys.h> for details
*/

void R_BSP_SYS_STDIO_DeInit(void) 
{
    R_RLIN3_DeInit(loc_CommPort); 
    loc_CommPort = 0xFF; 
}



/*******************************************************************************
  Function: R_BSP_SYS_STDIO_Write

    see <r_bsp_stdio_sys.h> for details
*/

void R_BSP_SYS_STDIO_Write(uint8_t * Buffer, uint32_t Len)
{
    
    R_RLIN3_Write(loc_CommPort, Buffer, Len);
    R_RLIN3_Flush(loc_CommPort); 
}


/*******************************************************************************
  Function: R_BSP_SYS_STDIO_Read

    see <r_bsp_stdio_sys.h> for details
*/

uint32_t R_BSP_SYS_STDIO_Read(uint8_t * String, uint32_t Max)
{
    uint32_t number = 0u;
    number = R_RLIN3_Read(loc_CommPort, String, Max);
     
    return number;
}

#else /* when no UART driver included, these functions remain "empty" */

void R_BSP_SYS_STDIO_SetUart(uint32_t Unit) 
{
    Unit = Unit; 
}

/*******************************************************************************
  Function: R_BSP_SYS_STDIO_Init
*/
 
void R_BSP_SYS_STDIO_Init(void) 
{
    
}
 
/*******************************************************************************
  Function: R_BSP_SYS_STDIO_DeInit
*/
 
void R_BSP_SYS_STDIO_DeInit(void) 
{

}
 
/*******************************************************************************
  Function: R_BSP_SYS_STDIO_Write
*/

void R_BSP_SYS_STDIO_Write(uint8_t * Buffer, uint32_t Len)
{
    Buffer  = Buffer; 
    Len     = Len; 
}


/*******************************************************************************
  Function: R_BSP_SYS_STDIO_Read
*/

uint32_t R_BSP_SYS_STDIO_Read(uint8_t * String, uint32_t Max)
{
    uint32_t number = 0u;
    String = String;      
    return number;
}

/*******************************************************************************
  Function: R_BSP_SYS_STDIO_GetActCh
  
*/

uint8_t R_BSP_SYS_STDIO_GetActCh(void)
{
    /* When UART is not used, only debugger printf is possible */
    return 0xFF; 
}

#endif /* of #ifdef USE_RLIN3 */


/*******************************************************************************
  Function: R_BSP_SYS_STDIO_AnWrite

    see <r_bsp_stdio_sys.h> for details
*/
r_Error_t R_BSP_SYS_STDIO_AnWrite(char *String, uint8_t Line) 
{
    /* Print32_t to AlphaNumeric Display needs to be implemented here.
       Currently not implemented. By return of R_NG, the print32_t will
       be relocated to R_BSP_STDIO_Printf (part of STDIO BSP)
       -> that means to the UART. */
    return R_NG;
}

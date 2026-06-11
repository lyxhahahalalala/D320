/*
****************************************************************************
PROJECT : Application test framework
FILE    : $Id: r_bsp_stdio_main.c 7262 2016-01-15 13:34:37Z michael.golczewski $
============================================================================
DESCRIPTION
STD I/O handling support
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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "r_typedefs.h"
#include "r_bsp_stdio_api.h"
#include "r_bsp_stdio_sys.h"
#include "r_bsp_board_config.h"


/*******************************************************************************
  Section: Local Constants
*/

/*******************************************************************************
  Define: LOC_BUFFER_MAXSIZE

  Printf buffer max size.
*/
#define LOC_BUFFER_MAXSIZE 250


/*******************************************************************************
  Section: Local Variables
*/

/*******************************************************************************
  Variable: loc_Buffer

  Buffer for String output
*/
static char loc_Buffer[LOC_BUFFER_MAXSIZE];

/*******************************************************************************
  Variable: loc_UsedIo

  Flag indication if the internall debugger I/O pane should be used, 
  or any other interface 
*/
static enum 
{
    LOC_EXTERN_IO  =  0u,
    LOC_DEBUGER_IO =  1
} loc_UsedIo = LOC_DEBUGER_IO; 


/*******************************************************************************
  Section: Local Functions 
*/

/*******************************************************************************
  Section: Global Generic IO Functions

  For comments see file test_api.h
*/

/*******************************************************************************
  Function: R_BSP_STDIO_Printf

    see <r_bsp_stdio_api.h> for details
*/
/* va_start violates several misra rules - but it's compiler "standard" */
#if defined (__ghs__)
#pragma ghs startnomisra
#endif /* __ghs */

void R_BSP_STDIO_Printf(char *String, ...)
{
    va_list ap;

    va_start(ap, String);
    if (LOC_EXTERN_IO == loc_UsedIo) 
    {
        vsprintf(loc_Buffer, String, ap);
        va_end(ap);
        R_BSP_STDIO_WriteString(loc_Buffer);
    } 
    else 
    {
        vprintf(String, ap);
        va_end(ap);
    }
}

/********************************************************************************
  Function: R_BSP_STDIO_AnPrintf

    see <r_bsp_stdio_api.h> for details
*/

void R_BSP_STDIO_AnPrintf(uint8_t Line, char *String,  ...)
{
    va_list ap;
    va_start(ap, String);
    vsprintf(loc_Buffer, String, ap);
    va_end(ap);
    if (R_BSP_SYS_STDIO_AnWrite(loc_Buffer, Line) != R_ERR_OK) 
    {
        /* Display output was not succesfull */
        R_BSP_STDIO_Printf(" String sent to AlphaNum Display Line %d: ", Line); 
        R_BSP_STDIO_Printf("%s\n", String); 
    } 
} 
#if defined (__ghs__)
#pragma ghs endnomisra
#endif /* __ghs */


/*******************************************************************************
  Function: R_BSP_STDIO_SetComPort

    see <r_bsp_stdio_api.h> for details
*/

void R_BSP_STDIO_SetComPort(uint32_t Unit) 
{
    
    if (Unit == 0xFF) 
    {
        loc_UsedIo = LOC_DEBUGER_IO; 
        printf("\n** disable UART, switch to StdI/O **\n"); 
    } 
    else 
    {
        loc_UsedIo = LOC_EXTERN_IO;
        printf("\n** Using UART port %d for stdio **\n", Unit); 
   
        R_BSP_SYS_STDIO_SetUart(Unit); 
    }
}


/*******************************************************************************
  Function: R_BSP_STDIO_WriteString

    see <r_bsp_stdio_api.h> for details
*/

void R_BSP_STDIO_WriteString(char *Buffer) 
{
    uint32_t strt = 0u;
    uint32_t stop = 0u;
    uint32_t len  = 0u;

    strt = 0u;
    stop = 0u;
    while ((Buffer[stop] != 0u) || (strt != stop)) 
    {
        while ((Buffer[stop] != 0u)  && (Buffer[stop] != '\n')) 
        {
            stop++;
        }
        len = stop - strt;
        if (0 != len) 
        {
            R_BSP_SYS_STDIO_Write((uint8_t*)&Buffer[strt], len);
            strt = stop;
        }
        if (Buffer[stop] == '\n') 
        {
            R_BSP_SYS_STDIO_Write((uint8_t *)"\r", 1);   
            stop++;
        }
        
    }
    
}

/*******************************************************************************
  Function: R_BSP_STDIO_ReadString

    see <r_bsp_stdio_api.h> for details
*/

void R_BSP_STDIO_ReadString(char *ReceiveString, uint32_t ReceiveMaxSize, uint32_t TimeOutCnt) 
{
    uint32_t i;
    uint32_t len;

    /* TimeOutCnt handling tbd */
    i = 0u;
    do 
    {     
        do 
        {
            if (LOC_EXTERN_IO == loc_UsedIo) 
            {
                len = R_BSP_SYS_STDIO_Read((uint8_t*)&ReceiveString[i], 1);
            } 
            else 
            {
                ReceiveString[i] = (char)getchar(); 
                len = 1u;
            }
        } while(0 == len); 
        if((ReceiveString[i] != '\n') && (ReceiveString[i] != '\r')) 
        {
            if (LOC_EXTERN_IO == loc_UsedIo) 
            {
                R_BSP_SYS_STDIO_Write((uint8_t*)&ReceiveString[i], 1);
            }
            i++;
        } 
        else 
        {
            if (LOC_EXTERN_IO == loc_UsedIo) 
            {
                R_BSP_SYS_STDIO_Write((uint8_t*)"\n", 1);
                R_BSP_SYS_STDIO_Write((uint8_t*)"\r", 1);
            }
            break;
            
        }
    } while (i < (ReceiveMaxSize - 1)); 
    ReceiveString[i] = (char)0;
    
}


/*******************************************************************************
  Function: R_BSP_STDIO_ReadValue

    see <r_bsp_stdio_api.h> for details
*/

uint32_t R_BSP_STDIO_ReadValue(void) 
{
    uint32_t receiveval;
    char     numbers[20]; 
    
    R_BSP_STDIO_ReadString(numbers, sizeof(numbers), 0);              
    receiveval = strtol(numbers,NULL,10);
    return receiveval;
}

/*******************************************************************************
  Function: R_BSP_STDIO_ReceiveChar

    see <r_bsp_stdio_api.h> for details
*/

void R_BSP_STDIO_ReadChar(int8_t *Character) 
{
    uint8_t got_it = 0;   
    if (LOC_EXTERN_IO == loc_UsedIo) 
    {
         while (0 == got_it) 
         {
            got_it = R_BSP_SYS_STDIO_Read((uint8_t *)Character, 1); 
         }
         R_BSP_SYS_STDIO_Write((uint8_t *)Character, 1); 
    } 
    else 
    {
        *Character = getchar(); 
        getchar();
    } 
}


/*******************************************************************************
  Function: R_BSP_STDIO_ReceiveChar

    see <r_bsp_stdio_api.h> for details
*/

void R_BSP_STDIO_ReceiveChar(int8_t *Character) 
{
    uint32_t len = 0u;
  
    do 
    {
        if (LOC_EXTERN_IO == loc_UsedIo) 
        {
            len = R_BSP_SYS_STDIO_Read((uint8_t *)Character, 1);
        } 
        else 
        {
           *Character = getchar();
            if ( '\n' != *Character)
            {
                getchar();
            }
            len = 1u;
        }
    } while(0 == len);
    if (LOC_EXTERN_IO == loc_UsedIo) 
    {
        R_BSP_SYS_STDIO_Write((uint8_t *)Character, 1);
    } 

}



/*******************************************************************************
  Function: R_BSP_STDIO_ReadHex

    see <r_bsp_stdio_api.h> for details
*/

void R_BSP_STDIO_ReadHex(uint32_t *ReceiveHex) 
{ 
    char buffer [20];
    
    R_BSP_STDIO_ReadString(buffer, sizeof(buffer), 0);                   
    sscanf(buffer, "%x", ReceiveHex);
}


/*******************************************************************************
  Function: R_BSP_STDIO_Init

    see <r_bsp_stdio_api.h> for details
*/
void R_BSP_STDIO_Init(void) 
{
    uint8_t port = R_BSP_SYS_STDIO_GetActCh(); 
    R_BSP_SYS_STDIO_Init(); 
    if (0xFF != port)
    {
        loc_UsedIo = LOC_EXTERN_IO;  
        printf("\n** Using UART port %d for stdio **\n", port); 
    }
    else 
    {
        loc_UsedIo = LOC_DEBUGER_IO;  
    }
}

/*******************************************************************************
  Function: R_BSP_STDIO_DeInit

    see <r_bsp_stdio_api.h> for details
*/
void R_BSP_STDIO_DeInit(void) 
{
    R_BSP_SYS_STDIO_DeInit(); 
    loc_UsedIo = LOC_DEBUGER_IO;  
}


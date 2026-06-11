/*
****************************************************************************
PROJECT : BSP - GFX - VI SAA71XX driver
FILE    : $Id: saa71xx_main.c 4988 2015-03-30 13:06:17Z golczewskim $
============================================================================
DESCRIPTION
BSP Implementation D1M2(H) MANGO Board
SAA71XX driver (main module)
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


/***********************************************************
  Title: Main Module

  Implementation of the API and other global functions.
*/

/***********************************************************
  Section: Includes
*/

#include "r_typedefs.h"     /* Renesas basic types, e.g. uint32_t */
#include "saa71xx_api.h"
#include "saa71xx_sys.h"
#define _SAA71XX_MAIN_GLOBALS_
#include "saa71xx_main.h"
#include "bsp.h"



/***********************************************************
  Section: Local Variables
*/


static const uint8_t loc_R_SAA7113_InitCVBS[][2] = {
    {0x01,0x08},
    {0x02,0xc0},          /* for CVBS input AI11 */
    {0x03,0x33},
    {0x04,0x00},
    {0x05,0x00},
    {0x06,0xe9}, 
    {0x07,0x0d},
    {0x08,0x98},
    {0x09,0x01},	  /* chrominance trap active (default for CVBS) */
    {0x0a,0x80},
    {0x0b,0x47},
    {0x0c,0x40},
    {0x0d,0x00},
    {0x0e,0x03},
    {0x0f,0x0f},    /* enable automatic chrominance gain control */
    {0x10,0x40},
    {0x11,0x0d},
    {0x12,0x7b},
    {0x13,0x00},
    {0x14,0x00},
    {0x15,0x00},
    {0x16,0x00},
    {0x17,0x00},
    {0x18,0x00},
    {0x19,0x00},
    {0x1d,0x00},
    {0x1e,0x00},
};


static const uint8_t loc_R_SAA7113_InitSVIDEO[][2] = {
    {0x01,0x08},
    {0x02,0xc8},        /* for s-video input */
    {0x03,0x33},
    {0x04,0x00},
    {0x05,0x00},
    {0x06,0xe9}, 
    {0x07,0x0d},
    {0x08,0x98},
    {0x09,0x81},	/* chrominance trap bypassed (default for s-video) */
    {0x0a,0x80},
    {0x0b,0x47},
    {0x0c,0x40},
    {0x0d,0x00},
    {0x0e,0x03},
    {0x0f,0x0f},    /* enable automatic chrominance gain control */
    {0x10,0x40},
    {0x11,0x0d},
    {0x12,0x7b},
    {0x13,0x00},
    {0x14,0x00},
    {0x15,0x00},
    {0x16,0x00},
    {0x17,0x00},
    {0x18,0x00},
    {0x19,0x00},
    {0x1d,0x00},
    {0x1e,0x00},
};


static const uint8_t loc_R_SAA7118_InitCVBS[][2] = {
    {0x01,0x47},
    {0x02,0xC0},
    {0x03,0x33},
    {0x04,0x00},
    {0x05,0x00},
    {0x06,0xEB},
    {0x07,0xE0},
    {0x08,0x98},
    {0x09,0x40}, /* 0x01 in saa7113 config */
    {0x0a,0x80},
    {0x0b,0x44},
    {0x0c,0x40},
    {0x0d,0x00},
    {0x0e,0x43},
    {0x0f,0x00},
    {0x10,0x06},
    {0x11,0x00},
    {0x12,0x8d}, /* hblank and vblank=rts0/1 */
    {0x13,0x10},
    {0x14,0x00},
    {0x15,0x11},
    {0x16,0xFE},
    {0x17,0x18},
    {0x18,0x40},
    {0x19,0x80},
    {0x1a,0x00},
    {0x1b,0x00},
    {0x1c,0x00},
    {0x1d,0x00},
    {0x20,0x00},
    {0x21,0x00},
    {0x22,0x00},
    {0x23,0x00},
    {0x24,0x90},
    {0x25,0x90},
    {0x26,0x00},
    {0x27,0x00},
    {0x28,0x00},
    {0x29,0x00},
    {0x2a,0x80},
    {0x2b,0x40},
    {0x2c,0x47},
    {0x2d,0x00},
    {0x2e,0x00},
    {0x2f,0x00},
    {0x80,0x10},
    {0x81,0x00},
    {0x82,0x00},
    {0x83,0x01},
    {0x84,0xa0},
    {0x85,0x10},
    {0x86,0xc5},
    {0x87,0x01},
    {0x88,0xf0},
    {0x89,0x00},
    {0x8a,0x00},
    {0x8b,0x00},
    {0x8c,0x00},
    {0x8d,0x00},
    {0x8e,0x00},
    {0x90,0x00},
    {0x91,0x08},
    {0x92,0x10},
    {0x93,0x80},
    {0x94,0x02},
    {0x95,0x00},
    {0x96,0xd0},
    {0x97,0x02},
    {0x98,0x10},
    {0x99,0x00},
    {0x9a,0x22},
    {0x9b,0x01},
    {0x9c,0xd0},
    {0x9d,0x02},
    {0x9e,0x20},
    {0x9f,0x01},
    {0xa0,0x01},
    {0xa1,0x00},
    {0xa2,0x00},
    {0xa3,0x00},
    {0xa4,0x80},
    {0xa5,0x40},
    {0xa6,0x40},
    {0xa7,0x00},
    {0xa8,0x00},
    {0xa9,0x04},
    {0xaa,0x00},
    {0xab,0x00},
    {0xac,0x00},
    {0xad,0x02},
    {0xae,0x00},
    {0xaf,0x00},
    {0xb0,0x00},
    {0xb1,0x04},
    {0xb2,0x00},
    {0xb3,0x04},
    {0xb4,0x00},
    {0xb5,0x00},
    {0xb6,0x00},
    {0xb7,0x00},
    {0xb8,0x00},
    {0xb9,0x00},
    {0xba,0x00},
    {0xbb,0x00},
    {0xbc,0x00},
    {0xbd,0x00},
    {0xbe,0x00},
    {0xbf,0x00},
    {0x88,0x00},
    {0x88,0x30}
};


static const uint8_t loc_R_SAA7118_InitSVIDEO[][2] = {
    {0x01,0x47},
    {0x02,0xC8},
    {0x03,0x33},
    {0x04,0x00},
    {0x05,0x00},
    {0x06,0xEB},
    {0x07,0xE0},
    {0x08,0x98},
    {0x09,0xc0}, /* 0x81 in saa7113 config */
    {0x0a,0x80},
    {0x0b,0x44},
    {0x0c,0x40},
    {0x0d,0x00},
    {0x0e,0x43},
    {0x0f,0x00},
    {0x10,0x06},
    {0x11,0x00},
    {0x12,0x8d}, /* hblank and vblank=rts0/1 */
    {0x13,0x10},
    {0x14,0x00},
    {0x15,0x11},
    {0x16,0xFE},
    {0x17,0x18},
    {0x18,0x40},
    {0x19,0x80},
    {0x1a,0x00},
    {0x1b,0x00},
    {0x1c,0x00},
    {0x1d,0x00},
    {0x20,0x00},
    {0x21,0x00},
    {0x22,0x00},
    {0x23,0x00},
    {0x24,0x90},
    {0x25,0x90},
    {0x26,0x00},
    {0x27,0x00},
    {0x28,0x00},
    {0x29,0x00},
    {0x2a,0x80},
    {0x2b,0x40},
    {0x2c,0x47},
    {0x2d,0x00},
    {0x2e,0x00},
    {0x2f,0x00},
    {0x80,0x10},
    {0x81,0x00},
    {0x82,0x00},
    {0x83,0x01},
    {0x84,0xa0},
    {0x85,0x10},
    {0x86,0xc5},
    {0x87,0x01},
    {0x88,0xf0},
    {0x89,0x00},
    {0x8a,0x00},
    {0x8b,0x00},
    {0x8c,0x00},
    {0x8d,0x00},
    {0x8e,0x00},
    {0x90,0x00},
    {0x91,0x08},
    {0x92,0x10},
    {0x93,0x80},
    {0x94,0x02},
    {0x95,0x00},
    {0x96,0xd0},
    {0x97,0x02},
    {0x98,0x10},
    {0x99,0x00},
    {0x9a,0x22},
    {0x9b,0x01},
    {0x9c,0xd0},
    {0x9d,0x02},
    {0x9e,0x20},
    {0x9f,0x01},
    {0xa0,0x01},
    {0xa1,0x00},
    {0xa2,0x00},
    {0xa3,0x00},
    {0xa4,0x80},
    {0xa5,0x40},
    {0xa6,0x40},
    {0xa7,0x00},
    {0xa8,0x00},
    {0xa9,0x04},
    {0xaa,0x00},
    {0xab,0x00},
    {0xac,0x00},
    {0xad,0x02},
    {0xae,0x00},
    {0xaf,0x00},
    {0xb0,0x00},
    {0xb1,0x04},
    {0xb2,0x00},
    {0xb3,0x04},
    {0xb4,0x00},
    {0xb5,0x00},
    {0xb6,0x00},
    {0xb7,0x00},
    {0xb8,0x00},
    {0xb9,0x00},
    {0xba,0x00},
    {0xbb,0x00},
    {0xbc,0x00},
    {0xbd,0x00},
    {0xbe,0x00},
    {0xbf,0x00},
    {0x88,0x00},
    {0x88,0x30}
};


static R_SAA71xx_Probe_t loc_R_SAA71xx_Probe[] = {
    {
        (unsigned char *)loc_R_SAA7113_InitSVIDEO,
        sizeof(loc_R_SAA7113_InitSVIDEO) /
        sizeof(loc_R_SAA7113_InitSVIDEO[0]) /
        sizeof(loc_R_SAA7113_InitSVIDEO[0][0]),
        (uint8_t *)"saa7113",
        {0x48,0x4a}
    },
    {
        (unsigned char *)loc_R_SAA7118_InitSVIDEO,
        sizeof(loc_R_SAA7118_InitSVIDEO) /
        sizeof(loc_R_SAA7118_InitSVIDEO[0]) /
        sizeof(loc_R_SAA7118_InitSVIDEO[0][0]),
        (uint8_t *)"saa7118",
        {0x42,0x40}
    }
};

static const int loc_R_SA711xx_ProbeCnt = sizeof(loc_R_SAA71xx_Probe) /
    sizeof(loc_R_SAA71xx_Probe[0]);


/***********************************************************
  Variable: _UserErrorHandler

  A function pointer to a user callback (Applications error handler)  
  
  The value is assigned by the application using the driver 
*/

static void (*_UserErrorHandler)(uint32_t Unit, uint32_t Error) = R_NULL;


/***********************************************************
  Section: Global Functions
*/

/***********************************************************
  Function: R_SAA71xx_ErrorHandler
*/

void R_SAA71xx_ErrorHandler(uint32_t Unit, R_SAA71xx_Error_t Error)
{
    if (_UserErrorHandler != R_NULL)   /* we've got a registered error handler here ...*/
    {
        _UserErrorHandler(Unit, Error);   /* ... and we gonna use it!*/
    } else {                              /* we don't have a handler */
        while(1)
        {
            /* nothing */
        }
    }
}


/***********************************************************
  Section: Global API Functions
*/

/***********************************************************
  Function: R_SAA71xx_SwitchMode
*/

R_SAA71xx_Error_t R_SAA71xx_SwitchMode(uint32_t Unit, R_SAA71xx_VideoMode_t Mode)
{
    if (Mode == R_SAA71XX_CVBS)
    {
        loc_R_SAA71xx_Probe[0].DataTable = (uint8_t*)loc_R_SAA7113_InitCVBS;
        loc_R_SAA71xx_Probe[0].Size      = sizeof(loc_R_SAA7113_InitCVBS) /
            sizeof(loc_R_SAA7113_InitCVBS[0]) / 
            sizeof(loc_R_SAA7113_InitCVBS[0][0]);
        loc_R_SAA71xx_Probe[1].DataTable = (uint8_t*)loc_R_SAA7118_InitCVBS;
        loc_R_SAA71xx_Probe[1].Size      = sizeof(loc_R_SAA7118_InitCVBS) /
            sizeof(loc_R_SAA7118_InitCVBS[0]) / 
            sizeof(loc_R_SAA7118_InitCVBS[0][0]);
    } else {
        loc_R_SAA71xx_Probe[0].DataTable  = (uint8_t*)loc_R_SAA7113_InitSVIDEO;
        loc_R_SAA71xx_Probe[0].Size       = sizeof(loc_R_SAA7113_InitSVIDEO) /
            sizeof(loc_R_SAA7113_InitSVIDEO[0]) /
            sizeof(loc_R_SAA7113_InitSVIDEO[0][0]);
        loc_R_SAA71xx_Probe[1].DataTable  = (uint8_t*)loc_R_SAA7118_InitSVIDEO;
        loc_R_SAA71xx_Probe[1].Size       = sizeof(loc_R_SAA7118_InitSVIDEO) /
            sizeof(loc_R_SAA7113_InitSVIDEO[0]) /
            sizeof(loc_R_SAA7113_InitSVIDEO[0][0]);
    }
    return R_SAA71XX_ERR_OK;
}


/***********************************************************
  Function: R_SAA71xx_Init
*/

R_SAA71xx_Error_t R_SAA71xx_Init(uint32_t Unit)
{
    R_SAA71xx_Error_t     err;
    R_SAA71xx_VideoMode_t vmode;
    int32_t                   i;
    int32_t                   j;
    int32_t                   offset;
    int32_t                   data;
    int32_t                   dataBack;

    err = R_SAA71xx_Sys_PortInit(Unit);
    if (err)
    {
        R_SAA71xx_ErrorHandler(Unit, R_SAA71XX_ERR_PORTINIT);
        return err; 
    }
    err = R_SAA71xx_Sys_I2CInit(Unit);
    if (err)
    {
        R_SAA71xx_ErrorHandler(Unit, err);
        return err; 
    }
    
    vmode = R_SAA71xx_Sys_CvbsSel(Unit);
    R_SAA71xx_SwitchMode(Unit, vmode);
    
    R_SAA71xx_Dev[Unit].Type = R_SAA71XX_NONE;
    for(i = 0; i < loc_R_SA711xx_ProbeCnt; i++)
    {
        if (R_SAA71xx_Dev[Unit].Type != R_SAA71XX_NONE)
        {
            break;
        }
        R_SAA71xx_Dev[Unit].Current = loc_R_SAA71xx_Probe + i;
        for(j = 0; j < 2; j++)
        {
            R_SAA71xx_Dev[Unit].Address = 
                R_SAA71xx_Dev[Unit].Current->Address[j];
            err = R_SAA71xx_Sys_I2CProbe(Unit, R_SAA71xx_Dev[Unit].Address);
            if (err == R_SAA71XX_ERR_OK)
            {
                R_SAA71xx_Dev[Unit].Type=(R_SAA71xx_Type_t)i;
                break;
            }
        }        
    }
    if (R_SAA71xx_Dev[Unit].Type == R_SAA71XX_NONE)
    {
        return R_SAA71XX_ERR_NG;
    }
    
    for(i = 0; i< R_SAA71xx_Dev[Unit].Current->Size; i++)
    {
        offset = R_SAA71xx_Dev[Unit].Current->DataTable[(i << 1) + 0];
        data   = R_SAA71xx_Dev[Unit].Current->DataTable[(i <<1 ) + 1];
        
        /* Write to one SAA71xx register, read back the written value.
           Repeat, if read back value does not equals to written register value. */
        do {
            R_SAA71xx_Sys_I2CWrite(Unit, R_SAA71xx_Dev[Unit].Address,
                                   offset, data);
            /* read back the written value */
            dataBack = R_SAA71xx_Sys_I2CRead(Unit, R_SAA71xx_Dev[Unit].Address,
                                   offset);

            /* In case of Chrominance gain control register (subaddress 0xF)
               skip the check for correct readBack value.
               Reason is that in automatic gain control mode the
               read back value is the gain set by the SAA71xx device,
               not the dummy gain set by register writing.
            */
            if (R_SAA71xx_REG_CHROMGAINCTL == offset)
            {
                dataBack = data;    /* Skip check, by set dataBack to same than data */
            }
        } while (data != dataBack);
    }
    return R_SAA71XX_ERR_OK;
}


/***********************************************************
  Function: R_SAA71xx_SwitchChannel
*/

R_SAA71xx_Error_t R_SAA71xx_SwitchChannel(uint32_t Unit, uint32_t Channel)
{
    switch(Channel)
    {
    case 1:
        R_SAA71xx_Sys_I2CWrite(Unit, R_SAA71xx_Dev[Unit].Address, 0x2u, 0xc0u);
        break;
    case 2:
        R_SAA71xx_Sys_I2CWrite(Unit, R_SAA71xx_Dev[Unit].Address, 0x2u, 0xc2u);
        break;
    default:
        return R_SAA71XX_ERR_OK;
    }
    return R_SAA71XX_ERR_OK;
}


/***********************************************************
  Function: R_SAA71xx_GetInfo
*/

R_SAA71xx_Error_t R_SAA71xx_GetInfo(
    uint32_t               Unit, 
    R_SAA71xx_Info_t *Info) 
{
    R_SAA71xx_Error_t     err;
    uint32_t                i;
    uint32_t                j;    

    R_SAA71xx_Dev[Unit].Type = R_SAA71XX_NONE;
    for(i = 0; i < loc_R_SA711xx_ProbeCnt; i++)
    {
        if (R_SAA71xx_Dev[Unit].Type != R_SAA71XX_NONE)
        {
            break;
        }
        R_SAA71xx_Dev[Unit].Current = loc_R_SAA71xx_Probe + i;
        for(j = 0; j < 2; j++)
        {
            R_SAA71xx_Dev[Unit].Address = 
                R_SAA71xx_Dev[Unit].Current->Address[j];
            err = R_SAA71xx_Sys_I2CProbe(Unit, R_SAA71xx_Dev[Unit].Address);
            if (err == R_SAA71XX_ERR_OK)
            {
                R_SAA71xx_Dev[Unit].Type=(R_SAA71xx_Type_t)i;
                break;
            }
        }        
    }
    if (R_SAA71xx_Dev[Unit].Type == R_SAA71XX_NONE)
    {
        return R_SAA71XX_ERR_NG;
    }
    Info->Addr = R_SAA71xx_Dev[Unit].Address;
    Info->Type = R_SAA71xx_Dev[Unit].Type;
    Info->Name = loc_R_SAA71xx_Probe[R_SAA71xx_Dev[Unit].Type].Name;
    return R_SAA71XX_ERR_OK;
}


/***********************************************************
  Function: R_SAA71xx_Disable
*/

R_SAA71xx_Error_t R_SAA71xx_Disable(uint32_t Unit)
{
    R_SAA71xx_Error_t err;
    R_SAA71xx_Info_t  saa_info;

    err = R_SAA71xx_GetInfo(Unit, &saa_info);
    if (err != R_SAA71XX_ERR_OK)
    {
        return R_SAA71XX_ERR_NG;
    }
    if (saa_info.Type == R_SAA71XX_7113)
    {
        /* stop data on VID from SAA7113 */
        R_SAA71xx_Sys_I2CWrite(Unit, saa_info.Addr, 0x11, 0x00);
        return R_SAA71XX_ERR_OK;
    }
    if (saa_info.Type == R_SAA71XX_7118)
    {
        /* stop data on VID from SAA7118 */
        R_SAA71xx_Sys_I2CWrite(Unit, saa_info.Addr, 0x88, 0x00);
        return R_SAA71XX_ERR_OK;
    }
    return R_SAA71XX_ERR_NG;
}


/***********************************************************
  Function: R_SAA71xx_DeInit
*/

R_SAA71xx_Error_t R_SAA71xx_DeInit(uint32_t Unit)
{
    R_SAA71xx_Error_t err;
    
    err = R_SAA71xx_Sys_PortDeInit(Unit);
    if (err)
    {
        R_SAA71xx_ErrorHandler(Unit, R_SAA71XX_ERR_PORTINIT);
        return err;
    }
    return R_SAA71XX_ERR_OK;
}

/***********************************************************
  Function: R_SAA71xx_SetErrorCallback
*/

void R_SAA71xx_SetErrorCallback(void (*ErrorCallback)(uint32_t Unit, uint32_t Error))
{
    _UserErrorHandler    = ErrorCallback; 
}



///////////////////////////////////////////////////////////////////////////DS3231Init/////////////////////////////
#define DS3231_SLAVE_ADDR              0xD0
#define DS3231_SLAVE_ADDR_RD           0xD1
#define DS3231_SLAVE_ADDR_WR           0xD0//DS3231的寄存器（开始）地址
#define DS3231_REG_BEGIN_ADDR       0x00
#define DS3231_TIME_BEGIN_ADDR      0x00
#define DS3231_ALARM1_BEGIN_ADDR    0x07
#define DS3231_ALARM2_BEGIN_ADDR    0x0B
#define DS3231_CTRL_ADDR            0x0E
#define DS3231_CTRL_STATUS_ADDR     0x0F
#define DS3231_OFFSET_ADDR          0x10
#define DS3231_TEMP_BEGIN_ADDR      0x11

unsigned char bcd2bin(unsigned char bcd) 
{    
unsigned char dump;    
unsigned char dig;    
unsigned char dec;       
dig = bcd & 0x0f;   
dec = (bcd >> 4)*10;   
dump = dig + dec;   
return (dump);
}

unsigned char bin2bcd(unsigned char bin) {
    unsigned char dump;
    unsigned char dig;
    unsigned char dec;

    dig = bin % 10;
    dec = ((bin / 10) << 4);

    dump = dig + dec;
    return (dump);

}

typedef enum
{
    R_RIIC_ERR_OK         = 0x00,
    R_RIIC_ERR_NG         = 0x01,
    R_RIIC_ERR_RANGE      = 0x02,
    R_RIIC_ERR_PORTINIT,
    R_RIIC_ERR_BUFOVERRUN,
    R_RIIC_ERR_RBINIT,
    R_RIIC_ERR_NODATA,
    R_RIIC_ERR_LAST
} r_riic_Error_t;


r_riic_Error_t R_RIIC_transmit_to_slave(uint32_t Unit, uint8_t *Tdata, uint32_t Size);
r_riic_Error_t R_RIIC_receive_from_slave8b(uint32_t Unit, uint8_t SlaveAddress, uint8_t InternalAddress, uint8_t *Rdata, uint32_t Size);
#if 0
/******************************************************************************
 *   功能：  从DS3231读取时间
 *   输出：  pTime 保存从DS3231读取到的时间的指针
    说明：  时间寄存器组7个字节定长
 ******************************************************************************/
void DS1302_ReadTime(DATE *pTime) {
    unsigned char buf[7];

    R_RIIC_receive_from_slave8b(0,DS3231_SLAVE_ADDR,
            DS3231_TIME_BEGIN_ADDR,
            &buf[0], 1);
    R_RIIC_receive_from_slave8b(0,DS3231_SLAVE_ADDR,
            DS3231_TIME_BEGIN_ADDR+1,
            &buf[1], 1);
    R_RIIC_receive_from_slave8b(0,DS3231_SLAVE_ADDR,
            DS3231_TIME_BEGIN_ADDR+2,
            &buf[2], 1);
    R_RIIC_receive_from_slave8b(0,DS3231_SLAVE_ADDR,
            DS3231_TIME_BEGIN_ADDR+3,
            &buf[3], 1);
    R_RIIC_receive_from_slave8b(0,DS3231_SLAVE_ADDR,
            DS3231_TIME_BEGIN_ADDR+4,
            &buf[4], 1);
    R_RIIC_receive_from_slave8b(0,DS3231_SLAVE_ADDR,
            DS3231_TIME_BEGIN_ADDR+5,
            &buf[5], 1);
    R_RIIC_receive_from_slave8b(0,DS3231_SLAVE_ADDR,
            DS3231_TIME_BEGIN_ADDR+6,
            &buf[6], 1);
    
    pTime->byte[0] = buf[0];
    pTime->byte[1] = buf[1];
    pTime->byte[2] = buf[2];
    pTime->byte[3] = buf[3];
    pTime->byte[4] = buf[4];
    pTime->byte[5] = buf[5];
    pTime->byte[6] = buf[6];
}


//读取温度值

unsigned char DS3231_ReadTemp(void) {
    unsigned char buf;

    R_RIIC_receive_from_slave8b(0,DS3231_SLAVE_ADDR,
            DS3231_TEMP_BEGIN_ADDR,
            &buf, 1);

    return buf;
}

/******************************************************************************
 *   功能：  设置DS3231的时间寄存器
 *   输入：  pTime 待设置的时间内容的指针
    说明：  时间寄存器组7个字节定长
 ******************************************************************************/
void DS1302_SetTime(DATE *pTime) {
    
    unsigned char buf[9];
    buf[0] = 0xD0;
    buf[1] = DS3231_TIME_BEGIN_ADDR;
	
    buf[2] = pTime->second;
    buf[3] = pTime->minute;
    buf[4] = pTime->hour;
    buf[5] = pTime->week;
    buf[6] = pTime->day;
    buf[7] = pTime->month;
    buf[8] = pTime->year;
    R_RIIC_transmit_to_slave(0,
            buf, 9);
}


R_DS3231_Error_t R_DS3231_Init(uint32_t Unit)
{
    R_DS3231_Error_t     err;
    int32_t                   i;
    int32_t                   j;
    int32_t                   offset;
    int32_t                   data;
    int32_t                   dataBack;

    err = R_SAA71xx_Sys_PortInit(Unit);
    if (err)
    {
        R_SAA71xx_ErrorHandler(Unit, R_SAA71XX_ERR_PORTINIT);
        return err; 
    }
    err = R_SAA71xx_Sys_I2CInit(Unit);
    if (err)
    {
        R_SAA71xx_ErrorHandler(Unit, err);
        return err; 
    }
    
    
     err = R_SAA71xx_Sys_I2CProbe(Unit, 0xD0);//DS3231 slave address
     if (err != R_SAA71XX_ERR_OK)
     {
         return R_SAA71XX_ERR_NG;
               
		
     }


    return R_SAA71XX_ERR_OK;
}

#endif

/*
****************************************************************************
PROJECT : BSP - GFX - VI SAA71XX driver
FILE    : $Id: sys_saa71xx.c 4988 2015-03-30 13:06:17Z golczewskim $
============================================================================
DESCRIPTION
BSP Implementation D1M2(H) MANGO Board
SAA71XX driver (sys functions) - concreate implementation for D1M2(H) MANGO Board
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
  Section: Includes
*/

#include "r_typedefs.h"     /* Renesas basic types, e.g. uint32_t */
#include "saa71xx_api.h"
#include "saa71xx_sys.h"
#include "r_dev_api.h"
#include "r_gpio_api.h"
#include "r_tick_api.h"

#if USE_RIIC /* ignore, when RIIC driver not available */
 #include "r_riic_api.h"
#endif /* USE_RIIC */

#include "r_bsp_api.h"
#ifdef USE_BSP_STDIO
    #include "r_bsp_stdio_api.h"
#endif
/***********************************************************
  Section: Defines
*/

/*
    Port settings for SAA71xx IIC on D1M2(H) Mango board
    and used RIIC unit.
    
    Jumper setting is required to select RIIC unit
    on D1M2(H) Mango Board.
      a) option1: Set Jumper JP105 to use RIIC unit 0 on port pins P3_[1:0] or
      b) option2: Set Jumper JP104 to use RIIC unit 1 on port pins P2_[1:0]
       - Do not set both Jumpers at the same time! -
    
    Further to use the video signals from the SAA71xx
    the following multiplex selection is required;
      1) On D1x Mango Main Board
       1a) Set DIP Switch DSW31-4. (Sets VINMUX_SEL signal to LOW.)
            -> This will mux Pins P43_[2..12] to ITU (CVBS Video Input)
       1b) Set DIP Switch DSW30-[4..1]. 
            -> This will connect ITU lines D3,D4,D5 and D6.
               (But ensure that nothing drives TCON0 Pins [1,4,5,6]
                by disconnection of JP[70,71,72,73].)

      2) On D1M2(H) Mango Adapter Board
       2a) Switch off DSW2-2. (Sets VI1ITU_SEL signal to LOW.)
            -> This will route the ITU0 signals to the main board.
               Otherwise, a part of the signals is muxed to ITU1.
*/
static uint32_t RiicUnit = 0;


/***********************************************************
  Section: Local Variables
*/

/***********************************************************
  Section: Local Functions
*/


/***********************************************************
  Section: Global Functions
*/

/***********************************************************
  Function: R_SAA71xx_Sys_PortInit
*/

R_SAA71xx_Error_t R_SAA71xx_Sys_PortInit(uint32_t Unit)
{
    static const r_dev_PinConfig_t loc_RiicPinConfig[] = 
    /*  Port Pin Func      Dir      Feedback     Pull        OpenDrain   HiCurr   HiSpeed  InputType */
    {
        /* RIIC unit 0 (on P3); SDA and SCL (data and clock) */
        {3u, 0u, 1u, R_DEV_PIN_OUT, 1u, R_DEV_PIN_PULLNO, 1u, 0u, 0u, R_DEV_PIN_SCHMITT4},
        {3u, 1u, 1u, R_DEV_PIN_OUT, 1u, R_DEV_PIN_PULLNO, 1u, 0u, 0u, R_DEV_PIN_SCHMITT4},
        
        /* RIIC unit 1 (on P2); SDA and SCL (data and clock) */
        {2u, 0u, 3u, R_DEV_PIN_OUT, 1u, R_DEV_PIN_PULLNO, 1u, 0u, 0u, R_DEV_PIN_SCHMITT4},
        {2u, 1u, 3u, R_DEV_PIN_OUT, 1u, R_DEV_PIN_PULLNO, 1u, 0u, 0u, R_DEV_PIN_SCHMITT4},
        
        /* delimiter - do not remove */
        {0u, R_DEV_PIN_LAST,0u,R_DEV_PIN_OUT,0u,R_DEV_PIN_PULLNO,     0u,      0u,         0u,    R_DEV_PIN_CMOS1}  
    };
    #ifdef USE_BSP
    if (RiicUnit == 0)
    {
        R_BSP_STDIO_Printf(" SAA71xx init will use RIIC unit 0 on P3_[1:0].\n");
        R_BSP_STDIO_Printf(" Set Jumper J105 and Remove Jumper JP104.\n");
    }
    if (RiicUnit == 1)
    {
        R_BSP_STDIO_Printf(" SAA71xx init will use RIIC unit 1 on P2_[1:0].\n");
        R_BSP_STDIO_Printf(" Set Jumper J104 and Remove Jumper JP105.\n");
    }
    #endif
    if (RiicUnit > 1)
    {
        return R_SAA71XX_ERR_NG;
    }
    #ifdef USE_BSP
    
    R_BSP_STDIO_Printf(" Mango Main: Set DIP Switch DSW31-4.\n");
    R_BSP_STDIO_Printf(" Mango Main: Set DIP Switch DSW30-[4..1].\n");
    R_BSP_STDIO_Printf(" Mango Adapter (D1M2H): Switch off DSW2-2.\n");
    R_BSP_STDIO_Printf("  To deactivate either do:\n");
    R_BSP_STDIO_Printf("  Mango Main: Clear DIP Switch DSW31-4\n");
    R_BSP_STDIO_Printf("  Or remove define 'ENABLE_CVBS_VIDEO_CAPTURE'.\n");
    
    #endif    
    /* Config IIC port pins used for SAA71xx */
    R_DEV_PinInit(loc_RiicPinConfig);
    R_TICK_WaitMS(0u, 100u);
    
    /* Enable SAA71xx Chip select (CE) - high active */
    /*  -> No s/w setup needed. CE is set high (active)
            by pull-up resistor R277 on Mango Board. */
    
    return R_SAA71XX_ERR_OK;
}


/***********************************************************
  Function: R_SAA71xx_Sys_PortDeInit
*/

R_SAA71xx_Error_t R_SAA71xx_Sys_PortDeInit(uint32_t Unit)
{
#if USE_RIIC /* do nothing, when RIIC driver not available */
    /* disable RIIC */
    R_RIIC_DeInit(RiicUnit);
#endif /* USE_RIIC */
    
    /* Disable SAA Chip select (CE) - high active */
    /*  -> No s/w setup needed. CE is set high (active)
            by pull-up resistor R277 on Mango Board. */
    return R_SAA71XX_ERR_OK;
}


/***********************************************************
  Function: eeSaa_Sys_CvbsSel
*/

R_SAA71xx_VideoMode_t R_SAA71xx_Sys_CvbsSel(uint32_t Unit)
{
    /* select Analog video source */
#if 0
    /* Option 1:
        Analog source is S-Video
        Two RCA connectors with separated luminance (Y) and chroma (C) information.
        Because of this also called Y/C. */
    #ifdef USE_BSP_STDIO
    R_BSP_STDIO_Printf(" S-Video analog VI.\n");
    #endif
    return R_SAA71XX_SVIDEO;
#else
    /* Option 2:
        Analog source is CVBS
        One RCA connector (mostly yellow) with combined video data, blanking and sync.
        Because of this it's called composite video.
        Further the acronym CVBS stands for _C_omposite video, _B_lanking and _S_yncronization.
    */
    #ifdef USE_BSP_STDIO
    R_BSP_STDIO_Printf(" CVBS analog VI.\n");
    #endif
    return R_SAA71XX_CVBS;
#endif
}


/***********************************************************
  Function: R_SAA71xx_Sys_I2CInit
*/

R_SAA71xx_Error_t R_SAA71xx_Sys_I2CInit(uint32_t Unit)
{
#if USE_RIIC /* do nothing, when RIIC driver not available */
    r_riic_Parameter_t Config;
    r_riic_Error_t err;

    /* set RIICn clock
        -> use default clock */

    /* Set IIC to master mode */
    Config.Baudrate = (100u * 1000u);   /* 100 kHz */
    err = R_RIIC_Init(RiicUnit, &Config);
    if (err != R_RIIC_ERR_OK)
    {
        return R_SAA71XX_ERR_NG;
    }
#endif /* USE_RIIC */
    
    return R_SAA71XX_ERR_OK;
}


/***********************************************************
  Function: R_SAA71xx_Sys_I2CProbe
*/

R_SAA71xx_Error_t R_SAA71xx_Sys_I2CProbe(uint32_t Unit, int Address)
{
    /* Regularly this function should transmit the Address byte
       to the I2C and check if a slave ACKs the address.

       !!! Because I don't know how to check the ACK of the slave
       the function will return R_SAA71XX_ERR_OK, when
       the Address given equals to 0x4A.
       The SAA71xx devices on D1x Mango Board is set to this address
       by internal pull-up on RTS0 pin.
       
       Issue: Some SAA7113X on D1x Mango board show address 0x48
               instead of 0x4A. RTS0 signal is floating but long wire.
               Might be a problem. -> Solution: Querry with slave address is used.
    */
#if 1
    uint8_t data;
    r_riic_Error_t ack;
#endif
    
    if (Unit == 0)
    {
#if 1
        /* check for ACK flag; means check if this slave address is used. */
        R_RIIC_Master_start_condition(Unit);
        data = Address;   /* Slave Address */
        ack = R_RIIC_Slave_address_send(Unit, &data);
        R_RIIC_Master_stop_condition(Unit, &data);
        if (ack == R_RIIC_ERR_OK)
        {
            return R_SAA71XX_ERR_OK;
        }
#else
        if (Address == 0x4A)
        {
            return R_SAA71XX_ERR_OK;
        }
#endif /* USE_RIIC */
    }
    return R_SAA71XX_ERR_NG;
}


/***********************************************************
  Function: R_SAA71xx_Sys_I2CWrite
*/

R_SAA71xx_Error_t R_SAA71xx_Sys_I2CWrite(uint32_t Unit, int Address, int Offset, int Data)
{
#if USE_RIIC /* do nothing, when RIIC driver not available */
    uint8_t TData[3];
    
    TData[0] = Address;    /* Address Slave */
    TData[1] = Offset;     /* Offset */
    TData[2] = Data;       /* Data */
    R_RIIC_transmit_to_slave(RiicUnit, TData, 3);   /* Transmit */
#endif /* USE_RIIC */
    R_TICK_WaitMS(0u, 1u);
    /* transmit assumes blocking transfer.
        (No wait for completion implemented here.) */
    
    return R_SAA71XX_ERR_OK;
}


/***********************************************************
  Function: R_SAA71xx_Sys_I2CRead
*/

int R_SAA71xx_Sys_I2CRead(uint32_t Unit, int Address, int Offset)
{
	uint8_t RData[1] = {0};
#if USE_RIIC /* do nothing, when RIIC driver not available */
    R_RIIC_receive_from_slave8b(RiicUnit, Address, Offset, RData, 1);
#endif /* USE_RIIC */
	
	return RData[0];
}


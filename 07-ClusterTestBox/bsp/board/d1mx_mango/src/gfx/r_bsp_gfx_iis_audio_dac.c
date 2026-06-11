/*
****************************************************************************
PROJECT : BSP - GFX - IIS audio DAC init
FILE    : $Id: r_bsp_gfx_iis_audio_dac.c 4988 2015-03-30 13:06:17Z golczewskim $
============================================================================
DESCRIPTION
BSP Implementation - D1M2 MANGO Board
Init the IIS audio Digital Analog Converter
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

#ifdef USE_RIIC    /* ignore this file, when RIIC driver not available */

/***********************************************************
  Section: Includes
*/

#include "r_typedefs.h"     /* Renesas basic types, e.g. uint32_t */
#include "r_dev_api.h"
#include "r_gpio_api.h"
#include "r_tick_api.h"
#include "r_riic_api.h"
#include "r_bsp_stdio_api.h"
#include "r_bsp_gfx_iis_audio_dac.h"

/***********************************************************
  Section: Local Defines
*/

/***********************************************************
  Section: Local Functions
*/

static const r_dev_PinConfig_t loc_RiicPinConfig[] = 
/*  Port Pin Func      Dir      Feedback     Pull        OpenDrain   HiCurr   HiSpeed  InputType */
{
    /* RIIC unit 0 (on P3); SDA and SCL (data and clock) */
    {3u, 0u, 1u, R_DEV_PIN_OUT, 1u, R_DEV_PIN_PULLNO, 1u, 0u, 0u, R_DEV_PIN_SCHMITT4},
    {3u, 1u, 1u, R_DEV_PIN_OUT, 1u, R_DEV_PIN_PULLNO, 1u, 0u, 0u, R_DEV_PIN_SCHMITT4},
        
    /* delimiter - do not remove */
    {0u, R_DEV_PIN_LAST,0u,R_DEV_PIN_OUT,0u,R_DEV_PIN_PULLNO,     0u,      0u,         0u,    R_DEV_PIN_CMOS1}  
};


/***********************************************************
  Section: Global Functions
*/

/***********************************************************
  Function: R_BSP_GFX_IIS_AudioDac_Init
  Init the IIS Audio Digital Analog converter

  Parameters:
  void

  Returns:
  void
*/
void R_BSP_GFX_IIS_AudioDac_Init(void)
{
    /* Initialization of the IIS Audio DAC "LM49450SQ" */
    /* Apply the init sequence to the IIS Audio DAC by IIC bus.
    
        Accessibility to IIS Audio DAC by IIC bus requires
        the Jumper settings on D1M2 Mango Board.
          a) Set JP47 1-2
          b) Set JP47 3-4
        
        Note: IIC bus used for IIS Audio DAC is always
                RIIC unit 0 on port pins P3_0 and P3_1.
    */
    
    uint32_t            RiicUnit = 0;
    r_riic_Parameter_t  Config;
    uint8_t             i;
    uint8_t             data;
    r_riic_Error_t      ack;
    
    /* the init sequence for the "LM49450SQ" */
    uint8_t iis_chip_data[5][3] =
    {
        {0xfa, 0, 0x21},    /* oversampling rate 32 (DAC_MODE = 3) */
        {0xfa, 1, 0x03},    /* MCLK divider ratio 2 */
        {0xfa, 2, 0x87},    /* charge pump osc freq. 67.5 (=MCLK/67.5) */
        {0xfa, 8, 0x1F},    /* Volume 1f = max (+24dB) / 0 = min (-53dB) */
        {0xfa, 3, 0x70}     /* word size 0x70 = 32 / 0x00 = 16 / 0x40 = 24 */
    };
    
    R_BSP_STDIO_Printf("Init Audio DAC device LM49450SQ\n");
    R_BSP_STDIO_Printf(" Set Jumper JP47 1-2 and JP47 3-4 for IIC access to Audio DAC.\n");
    
    /* Set IIC pins*/
    R_DEV_PinInit(loc_RiicPinConfig);
    R_TICK_WaitMS(0u, 100u);
    
    /* setup IIC */
    Config.Baudrate = (100u * 1000u);   /* 100 kHz */
    R_RIIC_Init(RiicUnit, &Config);
    
    /* check for ACK flag; means check if this slave address is used
        -> if not, the Audio DAC device (LM49450SQ) could not be detected by IIC */
    R_RIIC_Master_start_condition(RiicUnit);
    data = iis_chip_data[0][0];   /* Slave Address */
    ack = R_RIIC_Slave_address_send(RiicUnit, &data);
    R_RIIC_Master_stop_condition(RiicUnit, &data);
    R_TICK_WaitMS(0u, 2u);
    
    if (ack == R_RIIC_ERR_OK)
    {
        /* Audio DAC could be found via IIC */

        /* send init sequence via IIC */
        for(i=0; i<5; i++)
        {
            R_RIIC_transmit_to_slave(RiicUnit, iis_chip_data[i], 3);   /* Transmit */
            R_TICK_WaitMS(0u, 2u);
        }
    } else {
        /* Audio DAC could NOT be found via IIC
            Possible reasons can be:
             o Jumper setting missing
                -> Set JP47 1-2 and JP47 3-4 (and ensure B1VCC is 3.3V, e.g. JP5 3-4 set).
             o invalid ACK issue of Audio DAC (LM49450SQ)
                -> Audio-DAC (LM49450SQ) ACK level (low) is electricaly not ok! (~0.5xVCC)
                   Workaround is to configure I2C port input characteristic to Schmitt4 (w/ max. Vil = 0.5VCC)
                   This workaround might not work on all boards. (depending of their exact ACK low level)
                   On such boards the I2C config of Audio-DAC is skipped.
        */
        R_BSP_STDIO_Printf("\nWarning: Audio DAC init failed! (I2C connection not established)\n\n");
    }
    
    R_RIIC_DeInit(RiicUnit);
}

#endif /* USE_RIIC */


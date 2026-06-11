/*
****************************************************************************
PROJECT : BSP - GFX - MIPI CSI-2
FILE    : $Id: r_bsp_gfx_mipi_csi2.c 5251 2015-06-15 12:43:01Z tobyas.hennig $
============================================================================
DESCRIPTION
BSP Implementation D1X MANGO Board
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
Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.     

****************************************************************************
*/

/*
  Section: BSP SYS GFX description 
  
  This is the board specific GFX implementation for the BSP. 
        
*/


/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"     /* Renesas basic types, e.g. uint32_t */

#include "r_tick_api.h"
#include "r_dev_api.h"
#include "r_gpio_api.h"

#include "r_bsp_gfx_api.h"
#include "r_bsp_gfx_sys.h"
#include "r_bsp_board_config.h"

#ifdef USE_BSP_HMI
#include "r_bsp_hmi_api.h"
#endif
#include "r_bsp_gfx_mipi_csi2.h"
#include "r_bsp_stdio_api.h"

/*******************************************************************************
  Section: Local Defines
*/

/* MIPI CSI-2 video input registers */
#define MIPI_BASE_ADDR          0xFFFD3400u

#define MIPION_REG              (MIPI_BASE_ADDR + 0x00u)
#define MIPIMODE_REG            (MIPI_BASE_ADDR + 0x04u)
#define MIPIDATA_DLY_CTL_REG    (MIPI_BASE_ADDR + 0x08u)
#define MIPIRST_CTL_REG         (MIPI_BASE_ADDR + 0x0Cu)
#define MIPIBUF_CTL_REG         (MIPI_BASE_ADDR + 0x10u)
#define MIPISOT_COUNT_REG       (MIPI_BASE_ADDR + 0x14u)
#define MIPIRX_STATE_REG        (MIPI_BASE_ADDR + 0x18u)
#define MIPIWORD_COUNT_REG      (MIPI_BASE_ADDR + 0x1Cu)
#define MIPILP_EN_ON_WC_REG     (MIPI_BASE_ADDR + 0x20u)
#define MIPILINE_BLANK_REG      (MIPI_BASE_ADDR + 0x28u)
#define MIPIRESET_DLY_CTL0_REG  (MIPI_BASE_ADDR + 0x2Cu)
#define MIPIRDT_ERR_ON_REG      (MIPI_BASE_ADDR + 0x34u)
#define MIPIINTSTATUS_REG       (MIPI_BASE_ADDR + 0x40u)
#define MIPIINTENSET_REG        (MIPI_BASE_ADDR + 0x48u)
#define MIPIINTENCLR_REG        (MIPI_BASE_ADDR + 0x4Cu)
#define MIPIINTFFCLR_REG        (MIPI_BASE_ADDR + 0x50u)
#define MIPIEOT_COUNT_REG       (MIPI_BASE_ADDR + 0x74u)
#define MIPIVIN_MODE_REG        (MIPI_BASE_ADDR + 0x9Cu)


/* video selection registers */
#define VDCECTL         0xFFC0601Cu
#define VDCECTL_PXSL    (1ul<<3)



/*******************************************************************************
  Section: Local Constants
*/

static const r_dev_PinConfig_t loc_D1M2H_MipiPinConfig1Lane[] = 
/*  Port Pin Func      Dir      Feedback     Pull        OpenDrain   HiCurr   HiSpeed  InputType */
{
    /* VDCE0 Input MIPI CSI-2 */
    {40, 0,  1u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* VDCE0 In MIPI DATA0P */
    {40, 1,  1u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, 
    {40, 2,  0u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLUP, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, 
    {40, 3,  0u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLDOWN, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* VDCE0 In MIPI DATA1N */
    {40, 4,  1u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* VDCE0 In MIPI CLKP */
    {40, 5,  1u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* VDCE0 In MIPI CLKN */
    
    /* delimiter - do not remove */
    {0u, R_DEV_PIN_LAST,0u,R_DEV_PIN_OUT,0u,R_DEV_PIN_PULLNO,   0u, 0u, 0u, R_DEV_PIN_CMOS1} 
};

static const r_dev_PinConfig_t loc_D1M2H_MipiPinConfig2Lane[] = 
/*  Port Pin Func      Dir      Feedback     Pull        OpenDrain   HiCurr   HiSpeed  InputType */
{
    /* VDCE0 Input MIPI CSI-2 */
    {40, 0,  1u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* VDCE0 In MIPI DATA0P */
    {40, 1,  1u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, 
    {40, 2,  1u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, 
    {40, 3,  1u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* VDCE0 In MIPI DATA1N */
    {40, 4,  1u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* VDCE0 In MIPI CLKP */
    {40, 5,  1u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* VDCE0 In MIPI CLKN */
    
    /* delimiter - do not remove */
    {0u, R_DEV_PIN_LAST,0u,R_DEV_PIN_OUT,0u,R_DEV_PIN_PULLNO,   0u, 0u, 0u, R_DEV_PIN_CMOS1} 
};


/*******************************************************************************
  Section: Local Variables 
*/

static uint32_t loc_MipiIntOvfCount;
static uint32_t loc_MipiIntOvf;
static uint32_t loc_MipiIntCtl;
static uint8_t loc_NoOfLanes;

/*******************************************************************************
  Section: Local Functions
*/

/*******************************************************************************
  Section: Global ISR Functions
*/

void R_MIPI_IsrOVF(void)
{
    
    /* MIPI CSI-2 Buffer overflow error occured. */
    loc_MipiIntOvf = 1;
    loc_MipiIntOvfCount++;
}


void R_MIPI_IsrCTL(void)
{
    uint32_t val;

    /* MIPI CSI-2 Controller error occured. - Wait here.*/
    val = R_DEV_READ_REG(32, MIPIINTSTATUS_REG);

    /* we may lose it so OR it */
    loc_MipiIntCtl |= val;

    /* we logged it so now clear it */
    R_DEV_WRITE_REG(32, MIPIINTFFCLR_REG, val);

#ifdef USE_BSP_HMI
    R_BSP_HMI_SetLed(1, 1);
    R_BSP_HMI_SetLed(1, 0);   
#endif
}


/*******************************************************************************
  Section: Global Functions
*/

void R_BSP_SYS_GFX_MipiCsi2InputConfig(uint32_t PixelClkFreqHz, uint8_t NoOfLanes)
{
    uint32_t regval32;
    r_dev_ClkSelConfig_t mipiClkSelCfg;

    /* clear the status flags */
    loc_MipiIntOvf = 0;
    loc_MipiIntOvfCount = 0;
    loc_MipiIntCtl = 0;


    loc_NoOfLanes = NoOfLanes;

    /* clock settings
        MIPIPCLK Clock Controller C_ISO_PCLK
          -> active by default
        MIPIADCLK Clock Controller C_ISO_MIPIADCLK
          -> set to 160 MHz (PLL0/3)
        MIPIPIXCLK Clock Controller C_ISO_MIPIPIXCLK
          -> select DIV3/6/12 according to pixel clock demand
    */
    /* MIPIADCLK Clock Controller C_ISO_MIPIADCLK
            C_ISO_MIPIADCLK
                -> CKSC_IMIPIPLLS_CTL and fixed DIV3
                Select: PLL0PIX (default) / DIV3
    */
    mipiClkSelCfg.Cks = R_DEV_CKS_MIPIPLL;
    mipiClkSelCfg.SrcId = R_DEV_CKS_SRC_PLL0PIX_3;
    mipiClkSelCfg.StpReqMsk = 0;
    R_DEV_ClkIdSet(&mipiClkSelCfg);
    if ((160*1000*1000) != R_DEV_ClkFrequencyHz(mipiClkSelCfg.Cks))
    {
        for (;;) 
{
            /* error should have 160 MHz C_ISO_MIPIADCLK here.
               C_ISO_MIPIADCLK shall run on PLL0(480MHz) / 3.
               480 MHz / 3 = 160 MHz */
        }
    }
    
    /* MIPIPIXCLK Clock Controller C_ISO_MIPIPIXCLK
            C_ISO_MIPIPIXCLK
                -> CKSC_IMIPIPIXD_CTL (DIV3/6/12 selectable)
                    o 01B: MIPIPLLCLK /3 (default)
                    o 10B: MIPIPLLCLK /6
                    o 11B: MIPIPLLCLK /12
                Select best divider for pixel clock.
                ->  < 14 MHz use DIV12
                -> >= 14 MHz use DIV6
                -> >= 26 MHz use DIV3
        (calculated by D1ML_MIPI_CSI-2_size_usecases_check_MIPI-Sync_2013-07-04.xlsx)
    */
    mipiClkSelCfg.Cks = R_DEV_CKS_MIPIPXL;
    mipiClkSelCfg.SrcId = R_DEV_CKS_SRC_MIPIPLL;
    mipiClkSelCfg.StpReqMsk = 0;
    mipiClkSelCfg.Div = 12;
    if (PixelClkFreqHz >= (14 * 1000 * 1000))
    {
        mipiClkSelCfg.Div = 6;
    }
    if (PixelClkFreqHz >= (26 * 1000 * 1000))
    {
        mipiClkSelCfg.Div = 3;
    }
    R_DEV_ClkIdSet(&mipiClkSelCfg);
    
    /* divider setting */
    R_BSP_STDIO_Printf("MIPI pixel clock divider %d\n", mipiClkSelCfg.Div);
    
    /* Start sequence
    
    MIPI port setup
      1. Select alternative port function: PMC40.PMC40_[5:0] = 11 1111B
      2. Set ports as input: PM40.PM40_[5:0] = 11 1111B
      3. Wait 20 µsec buffer stabilization time
    
    Start sequence
      1. Reset by MIPI_RST_CTL = 0000 0003H
      2. Set up following registers:
        o MIPInMODE
        o MIPInDATA_DLY_CTL
        o MIPInBUF_CTL
        o MIPInSOT_COUNT
        o MIPInWORD_COUNT
        o MIPInLP_EN_ON_WC
        o MIPInLINE_BLANK
        o MIPInRESET_DLY_CTL
        o MIPInRDT_ERR_ON
        o MIPInEOT_COUNT
      3. Release reset by MIPInRST_CTL = 0000 0000H
      4. Enable MIPI transmission by MIPInON.MIPI_ON = 1.

    CAUTION
      Do not change the registers MIPInMODE, MIPInDATA_DLY_CTL, MIPInBUF_CTL, 
      MIPInSOT_COUNT, MIPInWORD_COUNT, MIPInLP_EN_ON_WC, MIPInLINE_BLANK, 
      MIPInRESET_DLY_CTL0, MIPInRDT_ERR_ON, MIPInEOT_COUNT while the MIPI 
      interface is active.
      These registers may only be changed after the stop sequence.
    */
    
    /* port setup */
    if (NoOfLanes == 2)
    {
        R_DEV_PinInit(loc_D1M2H_MipiPinConfig2Lane);
    } 
    else
    { 
        R_DEV_PinInit(loc_D1M2H_MipiPinConfig1Lane);
        /* R_GPIO_WritePin(40, 2, 0); */
        /* R_GPIO_WritePin(40, 3, 1); */
    }
    R_TICK_WaitUS(0, 50);   /* wait more than 20us */
    
    /* VDCE input selection (select MIPI) */
#if 0 /* done in the VI setup */ 
    regval32 = R_DEV_READ_REG(32, VDCECTL);
    regval32 |= VDCECTL_PXSL;
    R_DEV_WRITE_REG(32, VDCECTL, ((regval32 & 0x3Fu) | 0x10000u));
#endif
    
    /* ensure the MIPI is not on incase we need to do a re-config */
    R_DEV_WRITE_REG(32, MIPION_REG, 0u);

    /* 1. step */
    R_DEV_WRITE_REG(32, MIPIRST_CTL_REG, 0x00000003u);
    /* 2. step */
    regval32 = 0x0u;
    regval32 |= (1u<<29u);    /* enable clock line noise filter */

    if(2 == NoOfLanes)
    {
        regval32 |= (1u<<27u);    /* use 2 data lanes */
    }
    else
    {
        regval32 &= ~(1u<<27u);   /* use 1 data lane */
    }
  
    regval32 &= ~(1ul<<25u);   /* Select FIFO method for PHY layer sync        */
    regval32 |= (1ul<<24u);    /* Enable reset for clock lane (after time condition)   */
    regval32 |= (1ul<<23u);    /* Enable reset for data lanes (after time condition)   */
    regval32 &= ~(1ul<<22u);   /* MIPIADCLK       is T_HS-SETTLE count clock   */
    /* regval32 |= (1<<22) Ext. clock div4 is T_HS-SETTLE count clock   */
    regval32 |= (2ul<<16u);    /* RAW8 Input format spec                       */


    /* regval32 |= (1ul<<4u);     disable VTIM             */

    regval32 |= (1ul<<1u);     /* Enable ECC check and correction              */
    regval32 |= (1ul<<0);     /* Enable CRC package data check                */
    R_DEV_WRITE_REG(32, MIPIMODE_REG, regval32);
    
    regval32 = (uint32_t)((uint32_t)(0ul<<3u) | (uint32_t)(0ul<<0u));   /* data input delay 0 * 100ps (for lane 1+2) */
    R_DEV_WRITE_REG(32, MIPIDATA_DLY_CTL_REG, regval32);
    
    R_DEV_WRITE_REG(32, MIPIBUF_CTL_REG, 0);    /* MIPI I/F is used */
    
    regval32 = 0;
    regval32 |= (uint32_t)(20ul<<16u);   /* T_HS-SETTLE time for data count  (Thus CLK_COUNT must be set between 14 and 45.) */
    regval32 |= (uint32_t)(20ul<<0);    /* T_CLK-SETTLE time for clk count (Thus DATA_COUNT must be set between 14 and 23.) */
    R_DEV_WRITE_REG(32, MIPISOT_COUNT_REG, regval32);
    
    regval32 = 0;
    R_DEV_WRITE_REG(32, MIPIWORD_COUNT_REG, regval32);  /* Disable Long Package size limitation */
    
    regval32 = 0;
    regval32 |= (1u<<24u);    /* Enable noise removal mode. */
    regval32 |= (0x5u<<17u);  /* count value for noise removal (recommended value is 05H) */
    /* regval32 |= (1u<<16u)  LP buffer turns on after LP_EN_ON_WC[15:0] read words */
    regval32 |= (uint32_t)(0x20ul<<0u);  /* word count (WC) when to switch the LP BUFFER on. (recommended value is 0020H) */
    R_DEV_WRITE_REG(32, MIPILP_EN_ON_WC_REG, regval32);
    
    regval32 = 4u;   /* Interval until EOT in MIPIADCLK cycles */
    R_DEV_WRITE_REG(32, MIPILINE_BLANK_REG, regval32);
    
    R_DEV_WRITE_REG(32, MIPIRESET_DLY_CTL0_REG, 0x003F003Fu);
    R_DEV_WRITE_REG(32, MIPIRDT_ERR_ON_REG, 0x0);
    
    regval32 = 0;   /*  T_EOT setting - In the case that the data rate is more than
                        266 Mbps per one lane, this register should be set 0. */
    R_DEV_WRITE_REG(32, MIPIEOT_COUNT_REG, regval32);
    
                        /* 0 YUV422 VI format */
    regval32 = 0x1;     /* 1 RGB888 VI format */
                        /* 2 RGB565 VI format */
    R_DEV_WRITE_REG(32, MIPIVIN_MODE_REG, regval32);
    
    
    /* 3. step */
    R_DEV_WRITE_REG(32, MIPIRST_CTL_REG, 0x00000000u);
} 


void R_BSP_SYS_GFX_MipiCsi2InputEnable(void)
{
    uint32_t regval32;

   /* 4. step */
    R_DEV_WRITE_REG(32, MIPION_REG, 1u);
    
    /* Enable MIPI Interrupts */
    /*  Interrupt setting within MIPI controller */
    regval32 = 0x0f03ff2fu;  /* mask all error interrupts */
    R_DEV_WRITE_REG(32, MIPIINTENCLR_REG, regval32);    
    regval32 = 0x0f03ff2fu;  /* clear all error interrupts */
    R_DEV_WRITE_REG(32, MIPIINTFFCLR_REG, regval32);
   
    /* only enable errors for data lane 1 if there are 2 lanes */
    if (2 == loc_NoOfLanes)
    {
        /* all error interrupts enabled,
            except Unknown-X trigger and detect reset trigger */
        regval32 = 0x0003ff2fu; 
    }
    else
    {
        /* all error interrupts enabled (for lane 0),
            except Unknown-X trigger and detect reset trigger */
        regval32 = 0x00021f2fu;
    }

    R_DEV_WRITE_REG(32, MIPIINTENSET_REG, regval32);
    /*  Interrupt setting within INTC controller for MIPI */
    /* MIPI Buffer Overflow interrupt (INTMIPI0OVF) */
    R_DEV_IntSetPrio(R_DEV_INT_MIPICOVF, 3u);
    R_DEV_IntEnable(R_DEV_INT_MIPICOVF, 1u);
    /* MIPI Controller interrupt (INTMIPI0CTL) */
    R_DEV_IntSetPrio(R_DEV_INT_MIPICCTL, 3u);
    R_DEV_IntEnable(R_DEV_INT_MIPICCTL, 1u);
}

void R_BSP_SYS_GFX_MipiCsi2InputDisable(void)
{
    /* VDCE input selection (de-select MIPI) */
#if 0 /* done in the VI setup */ 
    regval32 = R_DEV_READ_REG(32, VDCECTL);
    regval32 &= ~VDCECTL_PXSL;
    R_DEV_WRITE_REG(32, VDCECTL, ((regval32 & 0x3Fu) | 0x10000u));
#endif
    
    /* Disable MIPI Interrupts */
    R_DEV_IntEnable(R_DEV_INT_MIPICOVF, 0u);
    R_DEV_IntEnable(R_DEV_INT_MIPICCTL, 0u);
    
    /* Stop sequence */
    /*  1. Disable the MIPI interface by MIPInON.MIPI_ON = 0. */
    R_DEV_WRITE_REG(32, MIPION_REG, 0u);
}


uint32_t R_BSP_GFX_MipiGetIsrStatus(void)
{
    uint32_t val;

    /* Disable interrupts globally while we get the current ISR value */
    __DI();

    val = loc_MipiIntCtl;
    loc_MipiIntCtl = 0;
   
    /* Enable interrupts globally */
    __EI();

    return val;
}


uint32_t R_BSP_GFX_MipiGetIsrOvfStatus(void)
{
    uint32_t val;

    /* Disable interrupts globally while we get the current ISR value */
    __DI();

    val = loc_MipiIntOvf;
    loc_MipiIntOvf = 0;
   
    /* Enable interrupts globally */
    __EI();

    return val;
}


uint32_t R_BSP_GFX_MipiGetIsrOvfStatusCount(void)
{
    uint32_t val;

    /* Disable interrupts globally while we get the current ISR value */
    __DI();

    val = loc_MipiIntOvfCount;
   
    /* Enable interrupts globally */
    __EI();

    return val;
}

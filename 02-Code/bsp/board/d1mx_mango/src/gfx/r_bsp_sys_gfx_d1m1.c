/*
****************************************************************************
PROJECT : BSP - GFX 
FILE    : $Id: r_bsp_sys_gfx_d1m1.c 7184 2016-01-08 09:48:08Z tobyas.hennig $
============================================================================
DESCRIPTION
BSP Implementation D1X MANGO Board
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
Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.     

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
#include "r_xbus_api.h"

#include "r_bsp_api.h"
#include "r_bsp_gfx_api.h"
#include "r_bsp_gfx_sys.h"
#include "r_bsp_board_config.h"



/*******************************************************************************
  Section: Local Defines
*/

/* Module reset control register */
#define LOC_MRSTC           0xFFF8F400u
#define LOC_PROTSMRST		0xFFF8F484u
#define LOC_PROTCMDMRST     0xFFF8F480u

/* Module reset control register bits */
#define MRSTC_XC0RES		((uint32_t)(1ul<<13u))
#define MRSTC_SDRB0RES		((uint32_t)(1ul<<12u))
#define MRSTC_MLBB0RES      ((uint32_t)(1ul<<11u))
#define MRSTC_LCBI0RES      ((uint32_t)(1ul<<10u))
#define MRSTC_SG4RES        ((uint32_t)(1ul<<9u)) 
#define MRSTC_SG3RES        ((uint32_t)(1ul<<8u)) 
#define MRSTC_SG2RES        ((uint32_t)(1ul<<7u)) 
#define MRSTC_SG1RES        ((uint32_t)(1ul<<6u)) 
#define MRSTC_SG0RES        ((uint32_t)(1ul<<5u)) 
#define MRSTC_ADCE0RES      ((uint32_t)(1ul<<4u)) 
#define MRSTC_ISM0RES       ((uint32_t)(1ul<<3u)) 
#define MRSTC_ETNB0RES      ((uint32_t)(1ul<<2u)) 
#define MRSTC_VDCE1RES      ((uint32_t)(1ul<<1u)) 
#define MRSTC_VDCE0RES      ((uint32_t)(1ul<<0u)) 


/* SDRA (DBSC2) SDR-SDRAM MemC registers (for D1M1H MANGO) */

#define SDRA_BASE       (0xf2fe0000u)

 #define DBSVCR           (SDRA_BASE + 0x0004u)
 #define DBKIND           (SDRA_BASE + 0x0008u)
 #define DBSTATE          (SDRA_BASE + 0x000cu)
 #define DBEN             (SDRA_BASE + 0x0010u)
 #define DBCMDCNT         (SDRA_BASE + 0x0014u)
 #define DBCKECNT         (SDRA_BASE + 0x0018u)
 #define DBAXICNT         (SDRA_BASE + 0x001cu)
 #define DBCONF           (SDRA_BASE + 0x0020u)
 #define DBTR0            (SDRA_BASE + 0x0030u)
 #define DBTR1            (SDRA_BASE + 0x0034u)
 #define DBTR2            (SDRA_BASE + 0x0038u)
 #define DBRFPDN0         (SDRA_BASE + 0x0040u)
 #define DBRFPDN1         (SDRA_BASE + 0x0044u)
 #define DBRFPDN2         (SDRA_BASE + 0x0048u)
 #define DBRFSTS          (SDRA_BASE + 0x004cu)
 #define DBMRCNT          (SDRA_BASE + 0x0060u)
 #define DBRSET           (SDRA_BASE + 0x0070u)
 #define DBRCHOICE0       (SDRA_BASE + 0x0074u)
 #define DBRCHOICE1       (SDRA_BASE + 0x0078u)
 #define DBTTR            (SDRA_BASE + 0x007cu)
 #define DBPDCNT0         (SDRA_BASE + 0x0108u)


/* XC1 QoS Registers (D1M1(H) only) */

#define QOS_BASE       (0xffcd2300u)

#define QOS_MODE       (QOS_BASE + 0x0000u)
#define QOS_MAX_COUNT  (QOS_BASE + 0x0004u)
#define QOS_MAX_RACC   (QOS_BASE + 0x0008u)
#define QOS_MAX_WACC   (QOS_BASE + 0x000cu)

#define QOS_PORT_CPU0   0x0
#define QOS_PORT_CPU1   0x1
#define QOS_PORT_VACC0  0x2
#define QOS_PORT_VACC1  0x3
#define QOS_PORT_JCUA   0x4
#define QOS_PORT_GPU2D  0x5
#define QOS_PORT_ETNB   0x6

/* video config */

/* video selection registers */
#define VDCECTL         0xFFC0601Cu

#define VDCECTL_UPDT1   (1ul<<5)
#define VDCECTL_UPDT0   (1ul<<4)
#define VDCECTL_PXSL    (1ul<<3)
#define VDCECTL_VISL0   (1ul<<0)


/*******************************************************************************
  Section: Local Types 
*/

/* XC1 QoS configuration */

typedef struct {
    uint32_t mode; 
    uint32_t max_cnt; 
    uint32_t max_racc;
    uint32_t max_wacc;
} r_xc1_qos_config_t;

/*******************************************************************************
  Section: Local Constants
*/

static const r_dev_PinConfig_t loc_D1M1_D1M1H_VoDefaultPinConfig[] = 
/*  Port Pin Func      Dir      Feedback     Pull        OpenDrain   HiCurr   HiSpeed  InputType */
{
    /* VDCE0 Output 24bit parallel, HSync+VSync */
    {44, 0,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* VDCE0 Out Data 23 */
    {44, 1,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {44, 2,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {44, 3,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {44, 4,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {44, 5,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {44, 6,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {44, 7,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {44, 8,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {44, 9,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {44, 10, 1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {44, 11, 1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* VDCE0 Out Data 12 */
    {45, 0,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* VDCE0 Out CLKP */
/*  45, 1,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1,  VDCE0 Out CLKN */
    {45, 1,  4u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1} , //VDCE0 Out Tcon3 DE (shared on same pin as CLKN},
    {45, 2,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* VDCE0 Out Data 11 */
    {45, 3,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {45, 4,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {45, 5,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {45, 6,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {45, 7,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {45, 8,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {45, 9,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {45, 10, 1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {45, 11, 1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {45, 12, 1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {45, 13, 1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* VDCE0 Out Data 0 */
    {43, 0,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* VDCE0 Out TCON2 HS */
    {43, 1,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* VDCE0 Out TCON0 VS */
    // {42, 11, 1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* VDCE0 Out TCON3 DE */
    
    /* delimiter - do not remove */
    {0u, R_DEV_PIN_LAST,0u,R_DEV_PIN_OUT,0u,R_DEV_PIN_PULLNO,   0u, 0u, 0u, R_DEV_PIN_CMOS1} 
};

static const r_dev_PinConfig_t loc_D1M1_D1M1H_ViDefaultPinConfig[] = 
/*  Port Pin Func      Dir      Feedback     Pull        OpenDrain   HiCurr   HiSpeed  InputType */
{
    /* VDCE0 Input ITU on P42_[10:0] AF2_In; BT656 from CVBS source via SAA71xx video converter */
    {42, 0,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* VDCE1 In ITU DATA7 */
    {42, 1,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},
    {42, 2,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},
    {42, 3,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},
    {42, 4,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},
    {42, 5,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},
    {42, 6,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},
    {42, 7,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* VDCE1 In ITU DATA0 */
    {42, 8,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* VDCE1 In ITU CLK */
    {42, 9,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* VDCE1 In ITU HSYNC */
    {42, 10, 2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* VDCE1 In ITU VSYNC */
    
    /* delimiter - do not remove */
    {0u, R_DEV_PIN_LAST,0u,R_DEV_PIN_OUT,0u,R_DEV_PIN_PULLNO,   0u, 0u, 0u, R_DEV_PIN_CMOS1} 
};

#if defined (BSP_INIT_QOS_SDRAM) || defined (BSP_INIT_QOS_VRAM)

/* XC1 QoS Configuration for maximum speed at master port */ 
static const r_xc1_qos_config_t qos_config_max[] = 
{
	0x3u,	/* Threshold mode */
	0x7u,   /* Initial cycle count: 8 cycles */
	0x7u,	/* 8 read bursts permitted within 8 cycles */
	0x7u,	/* 8 write bursts permitted within 8 cycles */
};
#endif

#ifdef BSP_INIT_QOS_SDRAM
/* XC1 QoS Configuration for D/AVE HD SDRAM access (assuming single layer VDCE pixel clock of 30MHz) */
/* This mode roughly divides the bandwidth between: D/AVE HD -> 33% read, 33% write
                                                    VDCE: 33% read
	At 100MHz SDRAM clock, this scheme locks maximum VDCE pixel clock at ~30MHz.
	For other pixel clocks / layers, different values should be used!
*/
static const r_xc1_qos_config_t qos_config_dhd_sdram[] = 
{
	0x3u,	/* Threshold mode */
	0x7fu,	/* Initial cycle count: 96 cycles */
	0x0u,	/* 1 read burst permitted within 96 cycles */
	0x7u,	/* 8 write burst permitted within 96 cycles (D/AVE HD write is limited by read ratio) */	
};
#endif

#ifdef BSP_INIT_QOS_VRAM
/* XC1 QoS Configuration for D/AVE HD VRAM access (assuming VDCE pixel clock of 30MHz) */
static const r_xc1_qos_config_t qos_config_dhd_vram[] = 
{
	0x3u,
	0x5fu,
	0x0u,	
	0x7u,		
};
#endif 
/*******************************************************************************
  Section: Local Functions declarations 
*/

void loc_SysGfxInitD1M1(void);
void loc_SysLowInitD1M1(void);

void loc_InitVideoIOPortD1M1(uint32_t *VoPinsAddr, uint32_t *ViPinsAddr);

static void loc_InitSDRAMemMangoRawD1M1H(void);

/*******************************************************************************
  Function: loc_InitVideoIOPortD1M1
  
  Init port pins of Video I/O and select a video configuration.
*/

void loc_InitVideoIOPortD1M1(uint32_t *VoPinsAddr, uint32_t *ViPinsAddr)
{
    uint32_t val;
    
    *VoPinsAddr = (uint32_t) &(loc_D1M1_D1M1H_VoDefaultPinConfig[0]);
    *ViPinsAddr = (uint32_t) &(loc_D1M1_D1M1H_ViDefaultPinConfig[0]);
    
    val = 0;
    /* video input selection VDCE0 */
#if 1
    /* select BT656 on 2nd alternative input on VDCE0 Input
        (i.e. ITU port pins P42_[10..0] */
	val &= ~VDCECTL_PXSL;
	val &= ~VDCECTL_VISL0;
#else
    /* select RGB on 3rd alternative input on VDCE0 Input
        (i.e. RGB666 port pins P42_[15..0] + P44_[5..1] */
	val &= ~VDCECTL_PXSL;
	val |= VDCECTL_VISL0;
#endif
    R_DEV_WRITE_REG(32, VDCECTL, ((val & 0x3Fu) | 0x10000u));

	
    /* video output assignment */
     /* nothing to select for D1M1(H) */
}



/*******************************************************************************
  Function: loc_InitSDRAMemMangoRawD1M1H
  
  Init SDRA(DBSC2) SDR-SDRAM Memc of D1M1H Mango with 16 MByte
  (SDRA supports up to 128 MByte, but D1M1H Mango Board only implement 16 MByte.)
  1 piece of external Micron SDRAM "MT48LC4M32B2P".
  (1x 4Mx32bit = 1x 128Mbit = 16 MByte)
  Use the register based setup.
*/

static void loc_InitSDRAMemMangoRawD1M1H(void)
{
	uint32_t regval;
	uint32_t i;

    /* Release reset for SDRA */
    regval  = R_DEV_READ_REG(32, LOC_MRSTC);
    regval |= MRSTC_SDRB0RES;
    R_DEV_WRITE_PROT_REG(LOC_PROTCMDMRST, LOC_MRSTC, regval);
    while (0 != R_DEV_READ_REG(32, LOC_PROTSMRST))  /* Read PROTSMRST */
    {
        /* PROTSMRST - register protection sequence fail */
    }

    R_DEV_WRITE_REG(32,DBPDCNT0,0x43000000);
    R_DEV_WRITE_REG(32,DBPDCNT0,0x43000001);

    R_DEV_WRITE_REG(32,DBCMDCNT,0x00000003);
	
    /* Wait 200 ms
        R_TICK_WaitMS(0, 200); */
    for(i = 0; i < 4000000; i++)
    {
        R_DEV_Nop();
    }
    
    R_DEV_WRITE_REG(32,DBCONF,0x01210c02u); /* SDRAM Address Configuration
                                               ===========================
                                               - 32-bit width
                                               - BA[0] = A[12]
                                               - BA[1] = A[13]
                                               - MA[12:0] = A[26:14] -> RAS
                                               - MA[12:0] = A[11:2]  -> CAS
                                             */
    R_DEV_WRITE_REG(32,DBTR0,0x02040501u);  /* SDRAM Timing Configuration (1)
                                               ==============================
                                               - CL   = 2 clk
                                               - tRAS = 5 clk
                                               - tRFC = 6 clk
                                               - tRCD = 2 clk
                                             */
    R_DEV_WRITE_REG(32,DBTR1,0x01010101u);  /* SDRAM Timing Configuration (2)
                                               ==============================
                                               - CKEH = 2 clk
                                               - tRP  = 2 clk
                                               - tRRD = 2 clk
                                               - tWR  = 2 clk
                                             */
    R_DEV_WRITE_REG(32,DBTR2,0x01010504u);  /* SDRAM Timing Configuration (3)
                                               ==============================
                                               - tRTP = 2 clk
                                               - tRC  = 2 clk
                                               - RDWR = 6 clk
                                               - WRRD = 5 clk
                                             */	
    
    R_DEV_WRITE_REG(32,DBKIND,0x00000001);  /* Set SDR SDRAM operation */
    
    R_DEV_WRITE_REG(32,DBCMDCNT,0x00000002);/* Execute "Precharge All" */
    /* Wait 20 ms
        R_TICK_WaitMS(0, 20); */
    for(i = 0; i < 400000; i++)
    {
        R_DEV_Nop();
    }
	
	
    R_DEV_WRITE_REG(32,DBCMDCNT,0x00000004);/* Execute "Refresh" */
    /* Wait 20 ms
        R_TICK_WaitMS(0, 20); */
    for(i = 0; i < 400000; i++)
    {
        R_DEV_Nop();
    }
	
		
    R_DEV_WRITE_REG(32,DBCMDCNT,0x00000004);/* Execute "Refresh" */
    /* Wait 20 ms
        R_TICK_WaitMS(0, 20); */
    for(i = 0; i < 400000; i++)
    {
        R_DEV_Nop();
    }
    
    
    R_DEV_WRITE_REG(32,DBMRCNT,0x00000022); /* Execute "Mode Register Set" */
    /* Wait 20 ms
        R_TICK_WaitMS(0, 20); */
    for(i = 0; i < 400000; i++)
    {
        R_DEV_Nop();
    }
    
    
    R_DEV_WRITE_REG(32,DBEN,0x00000001);    /* Enable SDRAM data operations */
	
    R_DEV_WRITE_REG(32,DBRFPDN1,0x00000186);
    R_DEV_WRITE_REG(32,DBRFPDN2,0x0aaa0037);	
    R_DEV_WRITE_REG(32,DBRFPDN0,0x00010000);/* - Enable auto refresh
                                               - No power-down
                                             */
}
#if defined (BSP_INIT_QOS_SDRAM) || defined (BSP_INIT_QOS_VRAM)

/*******************************************************************************
  Function: loc_XC1_ConfigureQoS
  
  Configure XC1 arbiter to control bandwidth distribution in D1M1
  - Bandwidth distribution is static, and therefore requires to be adjusted
    according to the use-case.
*/

static void loc_XC1_ConfigureQoS(uint32_t port, r_xc1_qos_config_t *config)
{
    uint32_t addr_offset = 0x10 * port;
	
	R_DEV_WRITE_REG(32, QOS_MODE      + addr_offset, config->mode);
	R_DEV_WRITE_REG(32, QOS_MAX_COUNT + addr_offset, config->max_cnt);	
	R_DEV_WRITE_REG(32, QOS_MAX_RACC  + addr_offset, config->max_racc);	
	R_DEV_WRITE_REG(32, QOS_MAX_WACC  + addr_offset, config->max_wacc);
}
#endif
#ifdef BSP_INIT_QOS_SDRAM
static void loc_InitXC1QosSDRAM(void){
	loc_XC1_ConfigureQoS(QOS_PORT_CPU0,	(r_xc1_qos_config_t *)&qos_config_max[0]);
	loc_XC1_ConfigureQoS(QOS_PORT_CPU1,	(r_xc1_qos_config_t *)&qos_config_max[0]);
	loc_XC1_ConfigureQoS(QOS_PORT_VACC0,(r_xc1_qos_config_t *)&qos_config_max[0]);
	loc_XC1_ConfigureQoS(QOS_PORT_VACC1,(r_xc1_qos_config_t *)&qos_config_max[0]);
	loc_XC1_ConfigureQoS(QOS_PORT_JCUA,	(r_xc1_qos_config_t *)&qos_config_max[0]);
	loc_XC1_ConfigureQoS(QOS_PORT_GPU2D,(r_xc1_qos_config_t *)&qos_config_dhd_sdram[0]);
	loc_XC1_ConfigureQoS(QOS_PORT_ETNB,	(r_xc1_qos_config_t *)&qos_config_max[0]);
}
#endif
#ifdef BSP_INIT_QOS_VRAM
static void loc_InitXC1QosVRAM(void){
	loc_XC1_ConfigureQoS(QOS_PORT_CPU0,	(r_xc1_qos_config_t *)&qos_config_max[0]);
	loc_XC1_ConfigureQoS(QOS_PORT_CPU1,	(r_xc1_qos_config_t *)&qos_config_max[0]);
	loc_XC1_ConfigureQoS(QOS_PORT_VACC0,(r_xc1_qos_config_t *)&qos_config_max[0]);
	loc_XC1_ConfigureQoS(QOS_PORT_VACC1,(r_xc1_qos_config_t *)&qos_config_max[0]);
	loc_XC1_ConfigureQoS(QOS_PORT_JCUA,	(r_xc1_qos_config_t *)&qos_config_max[0]);
	loc_XC1_ConfigureQoS(QOS_PORT_GPU2D,(r_xc1_qos_config_t *)&qos_config_dhd_vram[0]);
	loc_XC1_ConfigureQoS(QOS_PORT_ETNB,	(r_xc1_qos_config_t *)&qos_config_max[0]);
}
#endif

/*******************************************************************************
  Function: loc_SysGfxInitD1M1
  
    Board and device specific init 
*/
void loc_SysGfxInitD1M1(void) 
{
    if (0 != (R_BSP_Capabilities() & R_BSP_HAS_SDRAM))
    {
        /* SDRAM controller (SDRA) is in D1M1H only */
        loc_InitSDRAMemMangoRawD1M1H();

#ifdef BSP_INIT_QOS_SDRAM
        loc_InitXC1QosSDRAM(); /* Configure QoS so that D/AVE HD doesn't distort video output (SDRAM case) */
#endif
    }

#ifdef BSP_INIT_QOS_VRAM
    loc_InitXC1QosVRAM();   /* Configure QoS so that D/AVE HD doesn't distort video output (VRAM case) */
#endif
}

/*******************************************************************************
  Function: R_BSP_SYS_GFX_LowInitD1M1

    Board and device specific init
*/
void loc_SysLowInitD1M1(void)
{
    loc_SysGfxInitD1M1();
}




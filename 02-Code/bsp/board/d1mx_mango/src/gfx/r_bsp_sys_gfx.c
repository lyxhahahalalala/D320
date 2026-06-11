/*
****************************************************************************
PROJECT : BSP - GFX 
FILE    : $Id: r_bsp_sys_gfx.c 7662 2016-02-15 14:59:20Z matthias.nippert $
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
#include "r_xbus_api.h"

#include "r_bsp_api.h"
#include "r_bsp_gfx_api.h"
#include "r_bsp_gfx_sys.h"
#include "r_bsp_board_config.h"

#ifdef BSP_INIT_SFMA
#include "r_sfma_calib.h"
#include "r_sfma_api.h"
#include "r_sfcdb_macronix.h"
#include "r_bsp_stdio_api.h"
#endif


/*******************************************************************************
  Section: Local Defines
*/

/* video selection registers */
#define VDCECTL         0xFFC0601Cu

#define VDCECTL_UPDT1   (1ul<<5)
#define VDCECTL_UPDT0   (1ul<<4)
#define VDCECTL_PXSL    (1ul<<3)
#define VDCECTL_VOSL    (1ul<<2)
#define VDCECTL_VISL1   (1ul<<1)
#define VDCECTL_VISL0   (1ul<<0)

#define RSDSCFG         0xFFC06020u

#define RSDSCFG_ROEN        (1ul<<3)
#define RSDSCFG_RPHSL(x)    ((x)u<<1)
#define RSDSCFG_RBSW        (1ul<<0)


/* VRAM Aribter Settings */

#define VRAM1CONTROL    0xfffd1000u
#define VRAM2CONTROL    0xfffd2000u
/* IdlTime, NoOfBursts */
#define VRMTRCTL        0x40u
/* Window Size for the restricted master */
#define VRMTRINTVL      0x44u

#ifdef BSP_INIT_SFMA
/* SFMA setup for Macronics MX25L51245G device only

    D1L1 (144 pin) (40MHZ SDR)  1 x MX25L51245G 512Mb: 536,870,912 x 1 bit (64MByte)

    D1L2           (80MHZ DDR)  1 x MX25L51245G 512Mb: 536,870,912 x 1 bit (64MByte)

    D1L2H and above(120MHZ DDR) 2 x MX25L51245G 512Mb: 536,870,912 x 1 bit (128MByte)
*/

/* SFMA macro */
#define SFMA_CH0 0

/* Clock regs */
#define CKSC_ISFMAD_CTL             (0xFFF85380u)
#define CKSC_ISFMAS_CTL             (0xFFF85340u)


/*  Default calibration for Serial flash memory (Macronix device only).

    SPBCLK clock phase shift         3 (range 0 - 7, written to the upper 16 bits)
    Sampling point phase shift       1 (range 0 - 7, written to the lower 16 bits)
*/
#define SFMA_SPBCLK_CLOCK_PHASE_SHIFT     (3u)

/* general setting for most devices */
#define SFMA_SAMPLING_POINT_PHASE_SHIFT   (1u)
#define SFMA_DEFAULT_CAL                  ((SFMA_SPBCLK_CLOCK_PHASE_SHIFT << 16) | SFMA_SAMPLING_POINT_PHASE_SHIFT)

/* D1M1 requires a different default cal */
#define SFMA_D1M1_SAMPLING_POINT_PHASE_SHIFT   (2u)
#define SFMA_D1M1_DEFAULT_CAL             ((SFMA_SPBCLK_CLOCK_PHASE_SHIFT << 16) | SFMA_D1M1_SAMPLING_POINT_PHASE_SHIFT)
#endif


static const r_dev_PinConfig_t loc_D1Mx_SfmaDefaultPinConfig[] = 
/*  Port Pin Func      Dir      Feedback     Pull        OpenDrain   HiCurr   HiSpeed  InputType */
{
    /* SFMA - serial Flash memory pins */
    {21, 0,  1u, R_DEV_PIN_OUT,    0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* O:  SFMA0CLK */
    {21, 1,  1u, R_DEV_PIN_OUT,    0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* O:  SFMA0SSL */
    {21, 2,  1u, R_DEV_PIN_DIRECT, 1u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* IO: SFMA0IO[0..]0 */
    {21, 3,  1u, R_DEV_PIN_DIRECT, 1u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {21, 4,  1u, R_DEV_PIN_DIRECT, 1u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {21, 5,  1u, R_DEV_PIN_DIRECT, 1u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* IO: SFMA0IO[..3]0 */
    
    /* delimiter - do not remove */
    {0u, R_DEV_PIN_LAST,0u,R_DEV_PIN_OUT,0u,R_DEV_PIN_PULLNO,   0u, 0u, 0u, R_DEV_PIN_CMOS1} 
};

/*******************************************************************************
  Section: Local Constants
*/

 static const r_xbus_Cfg_t xbus_cfg = 
 {
 	0,
 	0,
 	{{R_XBUS_AM_CACHE,0x1u,0x98000000u, 0x07F00000u}, /* Cached SDRAM (original addr: 0x58000000) mirror 128MB  */
    {R_XBUS_AM_CACHE, 0x1u,0xa0000000u, 0x1FF00000u},  /* Cached SFMA mirror (0xa000_0000 - 0xBFff_ffff)*/
 	{R_XBUS_AM_DIRECT,0x1u,0x40000000u, 0x1FF00000u}, /* (disabled) buffered SDRAM */
    {R_XBUS_AM_DIRECT,0x1u,0x3FCE8000u, 0x00300000u}}  /* (disabled) buffered VRAM */
 };

/*******************************************************************************
  Section: Local Types 
*/

/*******************************************************************************
  Section: Local Functions declarations (board & device specific) 
*/
extern void loc_SysGfxInitD1M1(void); 
extern void loc_SysLowInitD1M1(void); 
extern void loc_InitVideoIOPortD1M1(uint32_t *VoPinsAddr, uint32_t *ViPinsAddr);

extern void loc_SysGfxInitD1M2(void); 
extern void loc_SysLowInitD1M2(void); 
extern void loc_InitVideoIOPortD1M2(uint32_t *VoPinsAddr, uint32_t *ViPinsAddr);

/*******************************************************************************
  Section: Local Functions declarations 
*/
static void loc_DisableVOPixelClocks(void);
static void loc_InitVideoIOPort(void);

#ifdef BSP_INIT_SFMA
void loc_SetupSFMA(void);
#endif 

static void loc_InitGfxBus(void); 


/*******************************************************************************
  Function: loc_DisableVOPixelClocks
  
  Disable the video output pixel clock as default.
*/

static void loc_DisableVOPixelClocks(void)
{
    r_dev_ClkSelConfig_t ClkSelCfg;
    
    ClkSelCfg.Cks       = R_DEV_CKS_VDCE0CK;
    ClkSelCfg.SrcId     = R_DEV_CKS_SRC_DISABLED;
    ClkSelCfg.Div       = 0;
    ClkSelCfg.StpReqMsk = 0;
    R_DEV_ClkIdSet(&ClkSelCfg);

    if ( 0 != (R_BSP_Capabilities() & R_BSP_HAS_VOUT1))
    {    
        ClkSelCfg.Cks       = R_DEV_CKS_VDCE1CK;
        R_DEV_ClkIdSet(&ClkSelCfg);
    }
}


/*******************************************************************************
  Function: loc_InitVideoIOPort
  
  Init port pins of Video I/O and select a video configuration.
*/

static void loc_InitVideoIOPort(void)
{
    uint32_t vo_pins_addr = 0; 
    uint32_t vi_pins_addr = 0; 
    
    /* get the device type and configure the pins */
    r_dev_Device_t device = R_DEV_GetDev();
    
    if (device > R_DEV_R7F701407) 
    {
        /*D1M2 ...*/  
        loc_InitVideoIOPortD1M2(&vo_pins_addr, &vi_pins_addr);   
    }
    else 
    {
        /* D1M1 */
        loc_InitVideoIOPortD1M1(&vo_pins_addr, &vi_pins_addr);   
    }
    R_DEV_PinInit((const r_dev_PinConfig_t *) vo_pins_addr);
    R_DEV_PinInit((const r_dev_PinConfig_t *) vi_pins_addr);

}


/*******************************************************************************
  Function: loc_InitGfxBus
  
  Configure the Gfx bus (cache) 
*/
static void loc_InitGfxBus(void)
{
    /* Init XBus Bridge */
    if (R_XBUS_ERR_OK == R_XBUS_Init(0)) 
    {
        R_XBUS_Config(0u, (r_xbus_Cfg_t *)&xbus_cfg);
    }
}

#ifdef BSP_INIT_SFMA
/***********************************************************
  Function: loc_SetupSFMA

  SFMA calibration procedure
*/

extern void ClrWdt(void);
void loc_SetupSFMA(void)
{
    r_sfma_Config_t Config;
    r_sfma_Error_t sfma_error;
    // r_dev_Device_t device;
	//uint32_t err = 0;
    /* common SFMA config */
    Config.AddressMode = R_SFMA_ADDRESS_24BIT;
    Config.SerialFlashMemoryPageSize = 256;
    Config.SerialFlashMemoryMaxClock = 40000000;
    Config.SerialFlashMemorySectorSize = 4096;
    Config.CacheMode = R_SFMA_CACHE_OFF;
    // device = R_DEV_GetDev();

    Config.Calibration = SFMA_DEFAULT_CAL;        
    Config.PerformanceEnMode = R_SFMA_PER_EN_MODE_ENABLE;
    Config.SerialFlashMemorySize = ((16 * 1024 * 1024) * 1); /* 16 MByte */
    Config.MemoryNum = R_SFMA_MEMORY_SINGLE;

	Config.Mode = R_SFMA_MODE_SPI_OPERATING;
	Config.DataTransferMode = R_SFMA_SDR_SINGLE_IO;
	Config.Command = &r_sfc_Macronix_24bitAddrCmd_40MHz;

    /* Init the SFMA */
    sfma_error = R_SFMA_Init(SFMA_CH0);
    uint8_t ManufactureID, MemoryType, Capacity;
    sfma_error = R_SFMA_JEDECRead(SFMA_CH0, &ManufactureID, &MemoryType, &Capacity);
    sfma_error = R_SFMA_Open(SFMA_CH0, &Config);
    sfma_error =R_SFMA_ProtectionModeSet(0,R_SFMA_MODE_UNPROTECT);
    
	if(R_SFMA_ERR_OK != sfma_error)
	{
		/* error */
        R_BSP_STDIO_Printf("SFMA configuration error\n");
	}
	
	ClrWdt();

}

#endif


	
/*******************************************************************************
  Function: R_BSP_SYS_GFX_LowInit
  
  See: r_bsp_gfx_sys.h for details    
*/
void R_BSP_SYS_GFX_LowInit(void)
{
    /* get the device type  */
    r_dev_Device_t device = R_DEV_GetDev();
    
    if (device > R_DEV_R7F701407) 
    {
        loc_SysLowInitD1M2();
    }
    else 
    {
        loc_SysLowInitD1M1();
    }
    
}

/*******************************************************************************
  Function: R_BSP_SYS_GFX_Init
  
  See: r_bsp_gfx_sys.h for details    
*/
void R_BSP_SYS_GFX_Init(void) 
{
    volatile uint8_t io_state; 
    /* get the device type  */
    r_dev_Device_t device = R_DEV_GetDev();
    
    loc_DisableVOPixelClocks();
    loc_InitVideoIOPort();

    /* Store original IO state (enabled or disabled) */
    io_state = R_DEV_PinOutStateGet(); 
    #ifdef BSP_FORCE_SFLASH_IO
        /* Force Serial Flash IO to override the global IO disable setting */
        R_DEV_PinOutStateSet(1); 
    #endif
    R_DEV_PinInit(loc_D1Mx_SfmaDefaultPinConfig);

    /* restore previous IO state */
    R_DEV_PinOutStateSet(io_state); 

    loc_InitGfxBus();

    if (device > R_DEV_R7F701407) 
    {
        loc_SysGfxInitD1M2();
    }
    else 
    {
        loc_SysGfxInitD1M1();
    }
    
	
   // loc_SetupSFMA();


}


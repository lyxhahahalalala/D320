/*
****************************************************************************
PROJECT : VLIB - BSP
FILE    : $Id: r_bsp.c 7327 2016-01-22 10:29:57Z matthias.nippert $
============================================================================
DESCRIPTION
BSP Implementation D1Mx MANGO Board
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
  Section: BSP Description

    The "D1Mx-Mango Board" Support Package is intended for the
    following board combination:
    
    BSP Board Name: "D1Mx-MANGO Board"
        + D1M1 or D1M1H MANGO Adapter board
          or D1M2 or D1M2H MANGO Adapter board
        + D1x MANGO Main board
*/


/*******************************************************************************
  Section: Includes
*/
#include "iodefine.h"
#include "r_typedefs.h"     /* Renesas basic types, e.g. uint32_t */

#include "r_gpio_api.h"
#include "r_dev_api.h"
#include "r_tick_api.h"
#include "r_bsp_api.h"

#include "r_config_d1x.h"

#ifdef USE_BSP_STDIO
#include "r_bsp_stdio_api.h"
#endif

#ifdef USE_BSP_HMI
#include "r_bsp_hmi_api.h"
#endif

#ifdef USE_BSP_GFX
#include "r_bsp_gfx_api.h"
#include "r_bsp_gfx_disp_init.h"     /* CSI init for NEC display */
#include "r_bsp_gfx_vi_saa71_init.h" /* SAA71xx video input converter init */
#include "r_bsp_gfx_hdmi_out.h"      /* init HDMI output converters */
#include "r_bsp_gfx_iis_audio_dac.h" /* IIS Audio Digital Analog converter init */
#endif /* USE_BSP_GFX */

/*******************************************************************************
  Section: Local Defines
*/

/* make sure we have the information ready */
/* BSP_SHOW_INFO:     Print Clock Config at the end of BSP init */
/* BSP_PROVIDE_INFO:  Pprovide the API function to call the colck config print (later) */

#ifdef BSP_SHOW_INFO
    #define BSP_PROVIDE_INFO
#endif 

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


/*******************************************************************************
  Constant: LOC_BSP_BOARD_NAME

  Name of the Application Boards (as string)
*/

#define LOC_BSP_BOARD_NAME            "-MANGO"
#define LOC_BSP_BOARD_VERSION         "V1.0"

/*******************************************************************************
  Section: Local Functions prototypes (defined in the board specific files) 
*/
extern const r_dev_ClkSelConfig_t* loc_InitClocksD1M1(r_dev_ClkGenConfig_t * Config);
extern const r_dev_ClkSelConfig_t* loc_InitClocksD1M1H(r_dev_ClkGenConfig_t * Config);
extern const r_dev_ClkSelConfig_t* loc_InitClocksD1M2(r_dev_ClkGenConfig_t * Config);

/*******************************************************************************
  Section: Local Constants 
*/

#ifdef BSP_PROVIDE_INFO
     
/*******************************************************************************
  Local Constants (strings) for clock config informatioin  
*/

static const char *locSrcNames[R_DEV_BUS_ALL] = 
{
    "PLL0",
    "PLL1",
    "PLL2",
    "PLL0PIX",
    "SDRX",
    "ETNBP",
    "MLBP",
    "RSCANP",
    "XCC",
    "ETNBXCC",
    "MLBXCC",
    "CPU",
    "PLL1_INTERNAL",
    "ISOPCLK",
    "FIX",
    "JIT",
    "PLLFIX",
    "LRNG",
    "HRING",
    "ERING",
    "MOSC",
    "SOSC",
 }; 

static const char *locSelNames[R_DEV_CKS_ALL] = 
{
    "WDT0" ,
    "AWOT",
    "RTC",
    "FOUT",
    "CPU",
    "MLB",
    "SFMA",
    "RSCAN",
    "RSCANXIN",
    "SSIF",
    "TAUB0",
    "TAUB2",
    "TAUJ",
    "OSTM",
    "LCBI",
    "ADCE",
    "ISM",
    "RLIN",
    "CLKJIT",
    "CLKFIX",
    "PLLFIX",
    "PLL2IN",
    "DOTCK0",
    "DOTCK1",
    "VDCE0CK",
    "VDCE1CK",
    "RSDSCK",
    "VI0CLK",
    "MIPIPLL",
    "MIPIPXL",
    "LAST",
    "FIX_JIT",
    "FIX_LOW",
    "VI0PIX",
    "VI1PIX"

}; 

static const char *locSrcSelNames[R_DEV_CKS_SRC_ALL] = 
{
    "DISABLED",
    "LRNG",
    "HRING",
    "ERING",
    "MOSC",
    "SOSC",
    "PLL0",
    "PLL0_2",
    "PLL0_4",
    "PLL1",
    "PLL1_4",
    "PLL2",
    "PLL2_4",
    "ISOCPU_4",
    "CLKFIX",
    "CLKFIX_8",
    "CLKJIT",
    "CLKJIT_2",
    "CLKJIT_4",
    "CLKJIT_8",
    "PLLFIX",
    "PLLFIX_10",
    "PLLFIX_20",
    "PLL0PIX",
    "PLL0PIX_3",
    "PLLFIX_3",
    "DOTCK0",
    "DOTCK0_4",
    "DOTCK1",
    "DOTCK1_4",
    "VI0CLK",
    "VI0PIX",
    "VI1PIX",
    "MIPIPXL",
    "MIPIPLL",
    "LAST",
    "DIV"
}; 

/*******************************************************************************
  Function: loc_PrintClkCfg

  This prints the current clock configuration in uer readable formant 
  Just for information and debugging purposes (and thus exported in the API only 
  in case BSP_SHOW_INFO is set)
*/

void R_BSP_PrintClkCfg(void)
{
    uint32_t             i, frq; 
    r_dev_ClkSelConfig_t clk_cfg; 
    
    R_BSP_STDIO_Printf("Board: %s%s, Board Version: %s\n",  R_DEV_GetDevName(), 
                                                            R_BSP_GetBoardName(), 
                                                            R_BSP_GetBoardVersion());

    R_BSP_STDIO_Printf("******* Configured Clock sources *************\n");
    for (i = (uint32_t)R_DEV_CLK_PLL0; i < (uint32_t)R_DEV_BUS_ALL; i++)
    {
        frq = R_DEV_BusFrequencyHz((r_dev_Bus_t)i); 
        if (frq > 1000000)
        {
            R_BSP_STDIO_Printf("%-13s : %3d MHz\n",  locSrcNames[i],  frq/1000000u);
        }
        else if (frq > 0)
        {
            R_BSP_STDIO_Printf("%-13s : %3d KHz\n",  locSrcNames[i],  frq/1000u);
        }
        else 
        {
             R_BSP_STDIO_Printf("%-13s :     OFF\n",  locSrcNames[i]);
           
        }
    }
    R_BSP_STDIO_Printf("\n");
    
    R_BSP_STDIO_Printf("******* Configured Clocks     *************\n");
    for (i = (uint32_t)R_DEV_CKS_WDT0; i < (uint32_t)R_DEV_CKS_ALL; i++)
    {
        if ((uint32_t)R_DEV_CKS_LAST != i) 
        { 

            clk_cfg.Cks         = (r_dev_ClkSel_t)i; 
            clk_cfg.SrcId       = R_DEV_CKS_SRC_DISABLED; 
            clk_cfg.Div         = 0; 
            clk_cfg.StpReqMsk   = 0;   
    
            frq = R_DEV_GetClkConfig(&clk_cfg); 
            
            if (frq > 1000000)
            {
                R_BSP_STDIO_Printf("%-10s : %3d MHz, ",  locSelNames[i],  frq/1000000u);
            }
            else if (frq > 0) 
            {
                R_BSP_STDIO_Printf("%-10s : %3d KHz, ",  locSelNames[i],  frq/1000u);
            }
            else 
            {
                 R_BSP_STDIO_Printf("%-10s :     OFF, ",  locSelNames[i]);
            }
            R_BSP_STDIO_Printf("SrcClk: %-9s, Divider:%4d, StpMsk:%3d\n",  
                locSrcSelNames[clk_cfg.SrcId], clk_cfg.Div, clk_cfg.StpReqMsk);
        }        
    }
    R_BSP_STDIO_Printf("\n");
} 
#endif /* BSP_PROVIDE_INFO */

/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Section: Global Device/Board dependent Functions

  For comments see: <r_bsp_api.h>
*/
extern void LED_BG_ON(void);

/*******************************************************************************
  Function: R_BSP_LowInit

  see <r_bsp_api.h> for details 
*/
void R_BSP_LowInit(void)
{
    uint32_t                val;
    r_dev_ClkGenConfig_t    clk_gen_cfg; 
    const r_dev_ClkSelConfig_t*   clk_sel;
    
    /* get the device type  */
    r_dev_Device_t device = R_DEV_GetDev();

    /* Release all Modules by module reset control register */
    val = (0ul
             | MRSTC_XC0RES                     | MRSTC_MLBB0RES
             | MRSTC_LCBI0RES   | MRSTC_SG4RES  | MRSTC_SG3RES
             | MRSTC_SG2RES     | MRSTC_SG1RES  | MRSTC_SG0RES
             | MRSTC_ADCE0RES   | MRSTC_ISM0RES | MRSTC_ETNB0RES
             | MRSTC_VDCE1RES   | MRSTC_VDCE0RES);  
      
    R_DEV_WRITE_PROT_REG(LOC_PROTCMDMRST, LOC_MRSTC, val);
    while ( 0 != R_DEV_READ_REG(32, LOC_PROTSMRST))  /* Read PROTSMRST */
    {
    }
    
    /* Prepare device (clear all internal config, prepare interrupts) */
    R_DEV_Prepare(); 
    /* Get the right config */
    if (device > R_DEV_R7F701407) 
    {
        clk_sel = loc_InitClocksD1M2(&clk_gen_cfg);
    }
    else 
    {
        if (device > R_DEV_R7F701405) 
        {
            clk_sel = loc_InitClocksD1M1H(&clk_gen_cfg);
        }
        else
        {
            clk_sel = loc_InitClocksD1M1(&clk_gen_cfg);
        }
    }

    /* use the config, call Clock Gen Init */
    R_DEV_ClkGenInit(&clk_gen_cfg, clk_sel);

    /* use the config, store settings (needs to be repeated after low init */
    R_DEV_ClkGenInitStore(&clk_gen_cfg);

    /* Mostly just the DDR memory */
    #ifdef USE_BSP_GFX
        R_BSP_GFX_LowInit();    
    #endif  /* USE_BSP_GFX */
}

/*******************************************************************************
  Function: R_BSP_Init

  see <r_bsp_api.h> for details 
*/
r_Error_t R_BSP_Init(void) 
{
    r_Error_t             ret_val = R_ERR_OK;
    r_dev_ClkGenConfig_t  clk_gen_cfg;

    /* get the device type  */
    r_dev_Device_t device = R_DEV_GetDev();
    
	/* For debug print the  LowInit is moved from startup to here  */
	/* Added define to skip Low Init */
	// #if defined (R_DBG_PRINT_DEV_REGS) || defined (APP_SKIP_LOW_INIT)
	R_BSP_LowInit();
	// #endif
	
    /* Get the config */
    if (device > R_DEV_R7F701407) 
    {
        loc_InitClocksD1M2(&clk_gen_cfg);
    }
    else 
    {
        if (device > R_DEV_R7F701405) 
        {
            loc_InitClocksD1M1H(&clk_gen_cfg);
        }
        else
        {
            loc_InitClocksD1M1(&clk_gen_cfg);
        }
    }
    /* use the config, store settings */
    R_DEV_ClkGenInitStore(&clk_gen_cfg);
    /* Enable interrupts globally */
    __EI();
    
    /* init tick driver */
    if (R_TICK_Init(0) != R_TICK_ERR_OK)
    {
        return R_NG;
    }
    
    /* Init Graphics functions
       XBus-Bridge, Gfx-SS Controller */
    //#ifdef USE_BSP_GFX
        R_BSP_GFX_Init();
    //#endif /* USE_BSP_GFX */
    
    /* init on board functions if required */
    #ifdef USE_BSP_HMI
        R_BSP_HMI_Init();
    #endif
    
    #ifdef USE_BSP_STDIO
		//R_BSP_STDIO_Init(); 
    #endif
    
	#ifdef USE_BSP_GFX
		/* init the NEC display via CSI (required for NL2432HC22-41K) */
        R_BSP_GFX_CSI_NecDisplay_Init();
		
		#ifdef USE_RIIC
			#ifdef R_CVBS_VIDEO_CAPTURE_ENABLE
				/* Enable SAA71xx video input converter */
				// R_BSP_GFX_VI_SAA71xx_Enable();
			#endif /* R_CVBS_VIDEO_CAPTURE_ENABLE */
			
			/* IIS Audio Digital Analog converter init */
			// R_BSP_GFX_IIS_AudioDac_Init();
			
		#endif /* USE_RIIC */
		
		/* init the HDMI output converters */
		// R_BSP_GFX_HDMI_Init();
	#endif /* USE_BSP_GFX */
	
    // R_BSP_PrintClkCfg(); 
    
    return (ret_val);
	
}

/*******************************************************************************
  Function: R_BSP_DeInit

  see <r_bsp_api.h> for details 
*/
r_Error_t R_BSP_DeInit(void) 
{
    return R_ERR_OK;
}

/*******************************************************************************
  Function: R_BSP_Capabilities

  see <r_bsp_api.h> for details 
*/
uint32_t R_BSP_Capabilities(void) 
{
    /* get the device type and configure the pins */
    r_dev_Device_t device = R_DEV_GetDev();
    uint32_t       retval = 0; 
    if (device > R_DEV_R7F701403) 
    {
        retval  = R_BSP_HAS_SERIAL_FLASH; 
        retval |= R_BSP_HAS_VOUT0; 
        retval |= R_BSP_HAS_DDR_SF; 
        retval |= R_BSP_HAS_8BIT_SF; 
    }
    if (device > R_DEV_R7F701405) 
    {
        retval |= R_BSP_HAS_SDRAM; 
    }
    if (device > R_DEV_R7F701407) 
    {
        retval |= R_BSP_HAS_VOUT1; 
    }
    return (retval); 
}

/*******************************************************************************
  Function: R_BSP_MemoryAddressAndSize

  see <r_bsp_api.h> for details 


    Part No.     Name    VRAM0       VRAM1       SDRAM       sFlash
    R7F701401    D1L1    -           -           -           256MB
    R7F701402    D1L2    144kB       -           -           256MB
    R7F701403    D1L2H   144kB       -           -           512MB
    R7F701404+   D1M1    1.55MB      -           -           512MB
    R7F701406+   D1M1H   1.55MB      -           16MB        512MB
    R7F701408+   D1M2    1.55MB      1.55MB      256MB       512MB
    R7F701411+   D1M2H   1.55MB      1.55MB      512MB       512MB


    Part No.     Name    VRAM0       VRAM1       SDRAM       sFlash
    R7F701401    D1L1    -           -           -           0x10000000
    R7F701402    D1L2    0x3FFDC000  -           -           0x10000000
    R7F701403    D1L2H   0x3FFDC000  -           -           0x10000000
    R7F701404+   D1M1    0x3FE74000  -           -           0x10000000
    R7F701406+   D1M1H   0x3FE74000  -           0x40000000  0x10000000
    R7F701408+   D1M2    0x3FCE8000  0x3FE74000  0x40000000  0x10000000
    R7F701411+   D1M2H   0x3FCD8000  0x3FE74000  0x40000000  0x10000000

*/
r_Error_t R_BSP_MemoryAddressAndSize(uint32_t MemType, uint32_t *Address, uint32_t *Size) 
{
    /* get the device type and configure the pins */
    r_dev_Device_t  device = R_DEV_GetDev();
    r_Error_t       retval = R_ERR_OK; 

    uint32_t        loc_Addr = 0;
    uint32_t        loc_Size = 0;
    
    switch(MemType)
    {
    case R_BSP_MEM_VRAM:
        if (device >= R_DEV_R7F701408)   /* D1M2 upwards */
        {
            loc_Addr = D1M2_VRAM0;
            loc_Size = D1M_VRAM0_SIZE+D1M2_VRAM1_SIZE;  /* 3.1MB */
        }
        else if (device >= R_DEV_R7F701404)   /* D1M1 upwards */
        {
            loc_Addr = D1M1_VRAM0;
            loc_Size = D1M_VRAM0_SIZE;  /* 256MB */
        }
        else if (device >= R_DEV_R7F701402)   /* D1L2 upwards */
        {
            loc_Addr = D1L_VRAM0;
            loc_Size = D1L_VRAM0_SIZE;  /* 144kB */
        }
        else
        {
            loc_Addr = 0;
            loc_Size = 0;
        }
        break;
    case R_BSP_MEM_VRAM0:
        if (device >= R_DEV_R7F701408)   /* D1M2 upwards */
        {
            loc_Addr = D1M2_VRAM0;
            loc_Size = D1M_VRAM0_SIZE;  /* 1.55MB */
        }
        else if (device >= R_DEV_R7F701404)   /* D1M1 upwards */
        {
            loc_Addr = D1M1_VRAM0;
            loc_Size = D1M_VRAM0_SIZE;  /* 256MB */
        }
        else if (device >= R_DEV_R7F701402)   /* D1L2 upwards */
        {
            loc_Addr = D1L_VRAM0;
            loc_Size = D1L_VRAM0_SIZE;  /* 144kB */
        }
        else
        {
            loc_Addr = 0;
            loc_Size = 0;
        }
        break;
    case R_BSP_MEM_VRAM1:
        if (device >= R_DEV_R7F701408)   /* D1M2 upwards */
        {
            loc_Addr = D1M2_VRAM1;
            loc_Size = D1M2_VRAM1_SIZE;  /* 1.55MB */
        }
        else
        {
            loc_Addr = 0;
            loc_Size = 0;
        }
        break;
    case R_BSP_MEM_SDRAM:
        loc_Addr = 0x40000000;
        if (device >= R_DEV_R7F701411)   /* D1M2H upwards */
        {
            loc_Size = D1M2_SDRAM_SIZE;  /* 512MB */
        }
        else if (device >= R_DEV_R7F701408)   /* D1M2 upwards */
        {
            loc_Size = D1M2_SDRAM_SIZE/2;  /* 256MB */
        }
        else if (device >= R_DEV_R7F701406)   /* D1M1H upwards */
        {
            loc_Size = D1M1_SDRAM_SIZE;  /* 16MB */
        }
        else
        {
            loc_Addr = 0;
            loc_Size = 0;
        }
        break;
    case R_BSP_MEM_SERIAL_FLASH:
        loc_Addr = 0x10000000;
        if (device >= R_DEV_R7F701403)   /* D1L2H upwards */
        {
            loc_Size = 0x20000000;  /* 512MB */
        }
        else
        {
            loc_Size = 0x10000000;  /* 256MB */
        }
        break;
    default:
        retval = R_PARAMETER_RANGE;
        break;
    }
    
    if (0 != Address)
    {
        *Address = loc_Addr;
    }

    if (0 != Size)
    {
        *Size = loc_Size;
    }

    return (retval); 
}

/*******************************************************************************
  Function: R_BSP_GetBoardName

  see <r_bsp_api.h> for details 
*/
const int8_t * R_BSP_GetBoardName(void) 
{
    return (const int8_t *)LOC_BSP_BOARD_NAME;
}

/*******************************************************************************
  Function: R_BSP_GetBoardVersion

  see <r_bsp_api.h> for details 
*/
const int8_t * R_BSP_GetBoardVersion(void) 
{
    return (const int8_t *)LOC_BSP_BOARD_VERSION;

}


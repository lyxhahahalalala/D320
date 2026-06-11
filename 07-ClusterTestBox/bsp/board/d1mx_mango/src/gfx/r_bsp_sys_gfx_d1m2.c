/*
****************************************************************************
PROJECT : BSP - GFX 
FILE    : $Id: r_bsp_sys_gfx_d1m2.c 7327 2016-01-22 10:29:57Z matthias.nippert $
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


/* SDRB (DBSC3) DDR2-SDRAM MemC registers (for D1M2(H) MANGO) */

 #define DBSVCR         0xFFFA0004u
 #define DBSTATE0       0xFFFA0008u
 #define DBSTATE1       0xFFFA000cu
 #define DBACEN         0xFFFA0010u
 #define DBRFEN         0xFFFA0014u
 #define DBCMD          0xFFFA0018u
 #define DBWAIT         0xFFFA001cu
 #define DBKIND         0xFFFA0020u
 #define DBCONF0        0xFFFA0024u
 #define DBCONF1        0xFFFA0028u
 #define DBPHYTYPE      0xFFFA0030u
 #define DBTR0          0xFFFA0040u
 #define DBTR1          0xFFFA0044u
 #define DBTR2          0xFFFA0048u
 #define DBTR3          0xFFFA0050u
 #define DBTR4          0xFFFA0054u
 #define DBTR5          0xFFFA0058u
 #define DBTR6          0xFFFA005cu
 #define DBTR7          0xFFFA0060u
 #define DBTR8          0xFFFA0064u
 #define DBTR9          0xFFFA0068u
 #define DBTR10         0xFFFA006cu
 #define DBTR11         0xFFFA0070u
 #define DBTR12         0xFFFA0074u
 #define DBTR13         0xFFFA0078u
 #define DBTR14         0xFFFA007cu
 #define DBTR15         0xFFFA0080u
 #define DBTR16         0xFFFA0084u
 #define DBTR17         0xFFFA0088u
 #define DBTR18         0xFFFA008cu
 #define DBTR19         0xFFFA0090u
 #define DBBL           0xFFFA00b0u
 #define DBADJ0         0xFFFA00c0u
 #define DBADJ2         0xFFFA00c8u
 #define DBRFCNF0       0xFFFA00e0u
 #define DBRFCNF1       0xFFFA00e4u
 #define DBRFCNF2       0xFFFA00e8u
 #define DBRNK0         0xFFFA0100u
 #define DBPDNCNF       0xFFFA0180u
 #define DBPDCNT0       0xFFFA0200u
 #define DBPDCNT1       0xFFFA0204u
 #define DBPDCNT2       0xFFFA0208u
 #define DBPDCNT3       0xFFFA020cu
 #define DBPDLCK        0xFFFA0280u
 #define DBPDRGA        0xFFFA0290u
 #define DBPDRGD        0xFFFA02a0u

 #define DBBS0CNT1      0xFFFA0304u

 #define DBLGCNT0       0xFFFA1000u
 #define DBTMVAL00      0xFFFA1004u
 #define DBRQCTR0       0xFFFA1014u
 #define DBTHRES00      0xFFFA1018u
 #define DBTHRES10      0xFFFA101cu
 #define DBTHRES20      0xFFFA1020u
 #define DBLGSTS0       0xFFFA1024u
 #define DBLGQON0       0xFFFA1028u

 #define DBLGCNT1       0xFFFA1100u
 #define DBTMVAL01      0xFFFA1104u
 #define DBRQCTR1       0xFFFA1114u
 #define DBTHRES01      0xFFFA1118u
 #define DBTHRES11      0xFFFA111cu
 #define DBTHRES21      0xFFFA1120u
 #define DBLGSTS1       0xFFFA1124u
 #define DBLGQON1       0xFFFA1128u

 #define DBLGCNT2       0xFFFA1200u
 #define DBTMVAL02      0xFFFA1204u
 #define DBRQCTR2       0xFFFA1214u
 #define DBTHRES02      0xFFFA1218u
 #define DBTHRES12      0xFFFA121cu
 #define DBTHRES22      0xFFFA1220u
 #define DBLGSTS2       0xFFFA1224u
 #define DBLGQON2       0xFFFA1228u

 #define DBLGCNT3       0xFFFA1300u
 #define DBTMVAL03      0xFFFA1304u
 #define DBRQCTR3       0xFFFA1314u
 #define DBTHRES03      0xFFFA1318u
 #define DBTHRES13      0xFFFA131cu
 #define DBTHRES23      0xFFFA1320u
 #define DBLGSTS3       0xFFFA1324u
 #define DBLGQON3       0xFFFA1328u

 #define DBLGCNT4       0xFFFA1400u
 #define DBTMVAL04      0xFFFA1404u
 #define DBRQCTR4       0xFFFA1414u
 #define DBTHRES04      0xFFFA1418u
 #define DBTHRES14      0xFFFA141cu
 #define DBTHRES24      0xFFFA1420u
 #define DBLGSTS4       0xFFFA1424u
 #define DBLGQON4       0xFFFA1428u

 #define DBLGCNT5       0xFFFA1500u
 #define DBTMVAL05      0xFFFA1504u
 #define DBRQCTR5       0xFFFA1514u
 #define DBTHRES05      0xFFFA1518u
 #define DBTHRES15      0xFFFA151cu
 #define DBTHRES25      0xFFFA1520u
 #define DBLGSTS5       0xFFFA1524u
 #define DBLGQON5       0xFFFA1528u

 #define DBLGCNT6       0xFFFA1600u
 #define DBTMVAL06      0xFFFA1604u
 #define DBRQCTR6       0xFFFA1614u
 #define DBTHRES06      0xFFFA1618u
 #define DBTHRES16      0xFFFA161cu
 #define DBTHRES26      0xFFFA1620u
 #define DBLGSTS6       0xFFFA1624u
 #define DBLGQON6       0xFFFA1628u

 #define DBLGCNT7       0xFFFA1700u
 #define DBTMVAL07      0xFFFA1704u
 #define DBRQCTR7       0xFFFA1714u
 #define DBTHRES07      0xFFFA1718u
 #define DBTHRES17      0xFFFA171cu
 #define DBTHRES27      0xFFFA1720u
 #define DBLGSTS7       0xFFFA1724u
 #define DBLGQON7       0xFFFA1728u
	

/* video config */
/* set USE_ITU_INSTEAD_VO0DATAENAB = 1 to enable
   the ITU video input instead of VO0 data enable (that is requried for HDMI on VO0).
   Note: Enabling ITU will cause that HDMI on VO0 can not work. */
#ifndef USE_ITU_INSTEAD_VO0DATAENAB
  #define USE_ITU_INSTEAD_VO0DATAENAB     0u
#endif

/* set USE_DATAENAB_INSTEAD_VO1HSYNC = 1 to enable
   the data enable signal for VO1 that replaces the hsync (that is requried for HDMI on VO1).
   To mirror the data enable signal (from portpin P47_9)
    to the data enable signal the VO1 HDMI converter (signal VO1HDMI_EN, respectively SEL_VO1DE)
    the following jumper setting is required:
    o Set Jumper JP82 1-2 and remove Jumper JP83.   
   Note: Enabling replacing of hsync will cause that regular HS/VS displays on parallel VO1 can not work. */
#ifndef USE_DATAENAB_INSTEAD_VO1HSYNC
  #define USE_DATAENAB_INSTEAD_VO1HSYNC     0u
#endif

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


/*******************************************************************************
  Section: Local Constants
*/

static const r_dev_PinConfig_t loc_D1M2H_VoDefaultPinConfig[] = 
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
    {45, 1,  4u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* VDCE0 Out TCON3 DE (alt. location P45_1 AF4) */
#if USE_ITU_INSTEAD_VO0DATAENAB != 1
    {43, 2,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* VDCE0 Out TCON3 DE */
#endif  /* USE_ITU_INSTEAD_VO0DATAENAB */
    
    /* VDCE1 Output 24bit parallel, HSync+VSync */
    {46, 0,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* VDCE1 Out Data 23 */
    {46, 1,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {46, 2,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {46, 3,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {46, 4,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {46, 5,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {46, 6,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {46, 7,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {46, 8,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {46, 9,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {46, 10, 1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {46, 11, 1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {46, 12, 1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {46, 13, 1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {46, 14, 1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {46, 15, 1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {47, 0,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {47, 1,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {47, 2,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {47, 3,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {47, 4,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {47, 5,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {47, 6,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1},
    {47, 7,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* VDCE1 Out Data 0 */
    {47, 8,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* VDCE1 Out CLK */
    {43, 6,  2u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* VDCE1 Out TCON3 DE (alt. location P43_6 AF2) */
#if USE_DATAENAB_INSTEAD_VO1HSYNC == 0
    {47, 9,  1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* VDCE1 Out HSYNC */
#else
    {47, 9,  2u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* VDCE1 Out DE (on original HSYNC location) */
#endif /* USE_DATAENAB_INSTEAD_VO1HSYNC */
    {47, 10, 1u, R_DEV_PIN_OUT, 0u, R_DEV_PIN_PULLNO, 0u, 0u, 1u, R_DEV_PIN_CMOS1}, /* VDCE1 Out VSYNC */
    
    /* delimiter - do not remove */
    {0u, R_DEV_PIN_LAST,0u,R_DEV_PIN_OUT,0u,R_DEV_PIN_PULLNO,   0u, 0u, 0u, R_DEV_PIN_CMOS1} 
};

static const r_dev_PinConfig_t loc_D1M2H_ViDefaultPinConfig[] = 
/*  Port Pin Func      Dir      Feedback     Pull        OpenDrain   HiCurr   HiSpeed  InputType */
{
    /* VDCE0 Input MIPI CSI-2 */
    {40, 0,  1u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* VDCE0 In MIPI DATA0P */
    {40, 1,  1u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, 
    {40, 2,  1u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, 
    {40, 3,  1u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* VDCE0 In MIPI DATA1N */
    {40, 4,  1u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* VDCE0 In MIPI CLKP */
    {40, 5,  1u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* VDCE0 In MIPI CLKN */

#if USE_ITU_INSTEAD_VO0DATAENAB == 1
    /* VDCE1 Input ITU on P43_[12:2] AF2_In; BT656 from CVBS source via SAA71xx video converter */
    {43, 2,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* VDCE1 In ITU DATA7 */
    {43, 3,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},
    {43, 4,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},
    {43, 5,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},
    {43, 6,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},
    {43, 7,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},
    {43, 8,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1},
    {43, 9,  2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* VDCE1 In ITU DATA0 */
    {43, 10, 2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* VDCE1 In ITU CLK */
    {43, 11, 2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* VDCE1 In ITU HSYNC */
    {43, 12, 2u, R_DEV_PIN_IN,  0u, R_DEV_PIN_PULLNO, 0u, 0u, 0u, R_DEV_PIN_CMOS1}, /* VDCE1 In ITU VSYNC */
#endif  /* USE_ITU_INSTEAD_VO0DATAENAB */
    
    /* delimiter - do not remove */
    {0u, R_DEV_PIN_LAST,0u,R_DEV_PIN_OUT,0u,R_DEV_PIN_PULLNO,   0u, 0u, 0u, R_DEV_PIN_CMOS1} 
};

/*******************************************************************************
  Section: Local Types 
*/
/* --------------------------- */
/* SDRB ARBITER INITIALISATION */

typedef struct {
    uint32_t enable;                    /* Arbiter -> 0: Disabled; 1: Enabled */
    /* Signed values from [-8192..+8191] */
    int32_t initial_value;              /* Initial value, must be bigger or equal to 1 */
    int32_t threshold0;                 /* 1st threshold, must be smaller than initial_value */
    int32_t threshold1;                 /* 2nd threshold, must be smaller than 1st threshold */
    int32_t threshold2;                 /* 3rd threshold, must be smaller than 2nd threshold */
    uint32_t act_to_idle_type;          /* Count the -> 0: Requests; 1: Bytes */
    uint32_t act_to_idle_threshold;     /* Max number of granted bytes/requests before returning to arbitration mode */
    uint32_t reload_type;               /* Reload counter value -> 0: Without carry-over; 1: With carry over */
    uint32_t counter_values_are_signed; /* Interprets initial_value and threshold<n> as -> 0: unsigned; 1: signed values. Signed values will be converted. */
} r_sdrb_arbiter_config_t;


/*******************************************************************************
  Section: Local Functions declarations 
*/

void loc_SysGfxInitD1M2(void);  
void loc_SysLowInitD1M2(void); 

void loc_InitVideoIOPortD1M2(uint32_t *VoPinsAddr, uint32_t *ViPinsAddr);

static void loc_SDRB_ConfigureArbiter(uint32_t port, r_sdrb_arbiter_config_t *config);
static void loc_SDRB_InitialiseArbiter(void);
static void loc_InitSDRBMemMangoRaw(void);

/*******************************************************************************
  Function: loc_InitVideoIOPortD1M2
  
  Init port pins of Video I/O and select a video configuration.
*/

void loc_InitVideoIOPortD1M2(uint32_t *VoPinsAddr, uint32_t *ViPinsAddr)
{
    uint32_t val;
    
    *VoPinsAddr = (uint32_t) &(loc_D1M2H_VoDefaultPinConfig[0]);
    *ViPinsAddr = (uint32_t) &(loc_D1M2H_ViDefaultPinConfig[0]);

    val = 0;
    /* video input selection VDCE0 */
#if 1
    /* select MIPI on VDCE0 Input */
	val |= VDCECTL_PXSL;
#else
    /* select ITU on 2nd alternative input on VDCE0 Input */
	val &= ~VDCECTL_PXSL;
	val &= ~VDCECTL_VISL0;
#endif

    /* video input selection VDCE1 */
    /* select ITU on 2nd alternative input
        (VDCE0_VI_xxx_ITU of P43_[12:2]) on VDCE1 Input */
	val &= ~VDCECTL_VISL1;
	
    /* video output assignment */
#if 1
	/* staight assignment */
	/* VDCE0 video channel 0 to VDCE0_VO port pins */
	/* VDCE1 video channel 1 to VDCE1_VO port pins */
	val &= ~VDCECTL_VOSL;
#else
	/* swapped channel assignment */
	/* VDCE0 video channel 0 to VDCE1_VO port pins */
	/* VDCE1 video channel 1 to VDCE0_VO port pins */
	val |= VDCECTL_VOSL;
#endif
    R_DEV_WRITE_REG(32, VDCECTL, ((val & 0x3Fu) | 0x10000u));
    
    /* RSDS settings for VDCE0_VO port pins */
    val = 0;
#if 1
    /* RSDS output disabled (use LVTTL) */
    val &= ~RSDSCFG_ROEN;
#else
    /* RSDS output enabled*/
    val |= RSDSCFG_ROEN;
    val |= RSDSCFG_RPHSL(0);    /* RSDS phase shift (0..3) x 90degree shift */
    val &= ~RSDSCFG_RBSW;   /* RSDS no swap; even number bit (i.e. data bits 0, 2, 4, 6) first */
#endif
    R_DEV_WRITE_REG(32, RSDSCFG, (val & 0xFu));
}


static void loc_SDRB_ConfigureArbiter(uint32_t port, r_sdrb_arbiter_config_t *config)
{
    uint32_t addr_offset = 0x100 * port;
    uint32_t zero_offset = 0x2000; /* 8192: Equals '0' */
    
    /* Compensate signed values by applying an offset */
    if (1 == config->counter_values_are_signed)
    {
        config->initial_value += zero_offset;
        config->threshold0 +=    zero_offset;
        config->threshold1 +=    zero_offset;
        config->threshold2 +=    zero_offset;
    }
    
    /* Simple sanity check */
    if ((0x3FFF < config->initial_value) || (0x3FFF < config->threshold0)
     || (0x3FFF < config->threshold1)    || (0x3FFF < config->threshold2))
    {
        for (;;) 
        {};   /* Value too big! */
    }
    else if ((zero_offset >= config->initial_value) || (0 > config->threshold0) 
          || (0 > config->threshold1) || (0 > config->threshold2))
    {
        for (;;) 
        {};   /* Value too small! */
    }
    else if ((config->threshold1 > config->threshold0) 
          || (config->threshold2 > config->threshold1))
    {
        for (;;) 
        {};   /* Conditions not met! */
    }
    else 
    {
        
    }
    /* Configure arbiter */
    R_DEV_WRITE_REG(32, DBLGCNT0  + addr_offset,  
        (((config->act_to_idle_type)&0x1u)<<16u) + (((config->reload_type) & 0x1u)<<8u) );

    R_DEV_WRITE_REG(32, DBTMVAL00 + addr_offset,  
        ((uint32_t)config->initial_value) & 0x3FFFFu);
        
    R_DEV_WRITE_REG(32, DBRQCTR0  + addr_offset,  
        (config->act_to_idle_threshold) & 0x3FFFFu);
        
    R_DEV_WRITE_REG(32, DBTHRES00 + addr_offset,  
        ((uint32_t)config->threshold0) & 0x3FFFFu);
    R_DEV_WRITE_REG(32, DBTHRES10 + addr_offset,
        ((uint32_t)config->threshold1) & 0x3FFFFu);
    R_DEV_WRITE_REG(32, DBTHRES20 + addr_offset,
        ((uint32_t)config->threshold2) & 0x3FFFFu);
        
    R_DEV_WRITE_REG(32, DBLGQON0  + addr_offset,((config->enable)& 0x1u));
}

static void loc_SDRB_InitialiseArbiter(void)
{
    uint32_t cnt = 0;
    
#if 0
    /* config matthias */
    r_sdrb_arbiter_config_t config[] = {
      /*                                      ActiveToIdle                      */
      /* EN, Start, Thrsh0, Thrsh1, Thrsh2, Type, Thresh, ReloadType, Signed  */
        {  1,  999,  8191,   1001,   970,    1,    1,      0,          1 },
        {  1,  999,  8191,   1001,   970,    1,    1,      0,          1 },
        {  0, 8180,  1001,   970,    840,    1,    1,      0,          1 },
        {  0, 8180,  1001,   970,    840,    1,    1,      0,          1 },
        {  1, 1040,  1041,   970,    840,    1,    1,      0,          1 },
        {  1, 1000,  1041,   970,    840,    1,    1,      0,          1 },
        {  0, 8180,  1001,   970,    840,    1,    1,      0,          1 },
        {  0, 8180,  1001,   970,    840,    1,    1,      0,          1 },
      /* 0,1,4,5 start with pre-level 1 and 2   */
      /* 2,3,6,7 are fixed to pre-level 0       */
    };
#endif
#if 0
    /* config oliver */
    r_sdrb_arbiter_config_t config_oliver[] = {
      /*                                           ActiveToIdle                      */
      /*  EN, Start,  Thresh 0, Thresh 1, Thresh 2, Type, Thresh, ReloadType, Signed */
        {  1, 0x2001, 0x3ffe,   0x3ffd,   0x3ffc,   0,    1,      0,          0 },
        {  1, 0x2001, 0x3ffe,   0x3ffd,   0x3ffc,   0,    1,      0,          0 },
        {  1, 0x3fff, 0x3,      0x2,      0x1,      0,    1,      0,          0 },
        {  1, 0x3fff, 0x3,      0x2,      0x1,      0,    1,      0,          0 },
        {  1, 0x2001, 0x3fff,   0x3ffe,   0x3ffd,   0,    1,      0,          0 },
        {  1, 0x2001, 0x3fff,   0x3ffe,   0x3ffd,   0,    1,      0,          0 },
        {  1, 0x3fff, 0x3,      0x2,      0x1,      0,    1,      0,          0 },
        {  1, 0x3fff, 0x3,      0x2,      0x1,      0,    1,      0,          0 },
    };
#endif    

#if 1
    /* config hamao */
    r_sdrb_arbiter_config_t config[] = {
      /*                                           ActiveToIdle                      */
      /* EN, Start,  Thresh 0, Thresh 1, Thresh 2, Type, Thresh, ReloadType, Signed  */

        /* SDRAM2 Read  from VDC  */
        {   1, 0x2001, 0x3fff,   0x3001,   0x2001,   1,4096,     1,         0},
        /* SDRAM3 Read  from VDC  */
        {   1, 0x2001, 0x3fff,   0x3001,   0x2001,   1,4096,     1,         0},
        /* SDRAM0 Read  from ACC  */
        {   1, 0x2001, 0x1f89,   0x1f00,   0x1e00,   1,   1,     0,         0},
        /* SDRAM1 Read  for  DAVE */
        {   1, 0x2001, 0x1f89,   0x1f00,   0x1e00,   1,   1,     0,         0},
        /* SDRAM3 Write from VDC  */
        {   1, 0x2001, 0x3fff,   0x3001,   0x2001,   1,4096,     1,         0},
        /* SDRAM4 Write from VDC  */
        {   1, 0x2001, 0x3fff,   0x3001,   0x2001,   1,4096,     1,         0},
        /* SDRAM0 Write for  ACC  */
        /* Max config for ACC is not necessary to get max out VDC performance. 
         Using read configuration instead. */
      /*    1, 0x2001, 0x3fff,   0x3001,   0x2001,   1,4096,     0,         0, */
        {   1, 0x2001, 0x1f89,   0x1f00,   0x1e00,   1,   1,     0,         0},
        /* SDRAM2 Write for  DAVE */
        {   1, 0x2001, 0x1f89,   0x1f00,   0x1e00,   0,   5,     0,         0},
    };
#endif    
    for (cnt = 0; cnt < 8; cnt++)
    {
        loc_SDRB_ConfigureArbiter(cnt, &config[cnt]);
    }
}

#if 0
void loc_InitSDRBMemArbiter_Matthias(void)
{
    /* Experimental part. */
    /* DBTMVAL0n: In this register, set a value greater than 0x2001 (8193). */
    /* cnt_offset_val: DBTMVAL0n is the start value register, so only add and do not substract 
       from offset for start0 to start3. Operation is not guaranteed for lower values. */
	uint32_t cnt_offset_val = 0x2001;
	
	/* "+8190"  This effects that it takes the maximum time for this port to get a higher priority */
    /* Originally, the order was: dhd, cpu, vdc, vdc. It seems that exchanging [0,1] with [2,3] yields the expected performance results */
    uint32_t start0 /*vdc*/ = cnt_offset_val + 1000+80, /* Slightly reduce priority for first port, so second gets a chance to breathe */
             start1 /*vdc*/ = cnt_offset_val + 1000,
             start2 /*cpu*/ = cnt_offset_val + 8180,
             start3 /*dhd*/ = cnt_offset_val + 8180;
	/* "+1"     Counters starting with zero will be immediately in a higher priority  */
    uint32_t thresh0        = cnt_offset_val + 1001;
	/* "-20"    After waiting for more than thresh1-thresh0 clockcycles, the request will slip in the next higher priority */
    uint32_t thresh1        = cnt_offset_val + 970; 
    uint32_t thresh2        = cnt_offset_val + 840;
   
       /* Set-up of DBSC3 bus arbitration units */

    /* AXI read ports */
    
    /* AXI port #0 (ACC) */
    
    R_DEV_WRITE_REG(32, DBLGCNT0,   0x01010100u);
    R_DEV_WRITE_REG(32, DBTMVAL00,  start0); /* Set initial count to 8192 */
    R_DEV_WRITE_REG(32, DBRQCTR0,   0x00000001u);
    R_DEV_WRITE_REG(32, DBTHRES00,  thresh0); /* First threshold */
    R_DEV_WRITE_REG(32, DBTHRES10,  thresh1); /* Second threshold */
    R_DEV_WRITE_REG(32, DBTHRES20,  thresh2); /* Third threshold */
#if 1   /* MNI: This hack *should* put every bus master except the VDCO layers in low priority mode */
    R_DEV_WRITE_REG(32, DBLGQON0,   0x00000001u); /* Enable threshold counter #0 */
#else
    R_DEV_WRITE_REG(32, DBLGQON0,   0x00000000u); /* Enable threshold counter #0 */
#endif

    /* AXI port #1 (DVCC) */
    
    R_DEV_WRITE_REG(32, DBLGCNT1,   0x01010100u); /* 0x0s0t0u00: s1: clear UDF bit; t1: count bytes; t1: Carry over of count value */
    R_DEV_WRITE_REG(32, DBTMVAL01,  start1); /* Set initial count to 8192 */
    R_DEV_WRITE_REG(32, DBRQCTR1,   0x00000001u);
    R_DEV_WRITE_REG(32, DBTHRES01,  thresh0); /* First threshold */
    R_DEV_WRITE_REG(32, DBTHRES11,  thresh1); /* Second threshold */
    R_DEV_WRITE_REG(32, DBTHRES21,  thresh2); /* Third threshold */
#if 1   /* MNI: This hack *should* put every bus master except the VDCO layers in low priority mode */
    R_DEV_WRITE_REG(32, DBLGQON1,   0x00000001u); /* Enable threshold counter #1 */      
#else
    R_DEV_WRITE_REG(32, DBLGQON1,   0x00000000u); /* Enable threshold counter #1 */      
#endif

    /* AXI port #2 (AVCC1) */

    R_DEV_WRITE_REG(32, DBLGCNT2,   0x01010100u);
    R_DEV_WRITE_REG(32, DBTMVAL02,  start2); /* Set initial count to 8192 */
    R_DEV_WRITE_REG(32, DBRQCTR2,   0x00000001u);
    R_DEV_WRITE_REG(32, DBTHRES02,  thresh0); /* First threshold */
    R_DEV_WRITE_REG(32, DBTHRES12,  thresh1); /* Second threshold */
    R_DEV_WRITE_REG(32, DBTHRES22,  thresh2); /* Third threshold */
#if 0   /* MNI: This hack *should* put every bus master except the VDCO layers in low priority mode */
    R_DEV_WRITE_REG(32, DBLGQON2,   0x00000001u); /* Enable threshold counter #2 */      
#else
    R_DEV_WRITE_REG(32, DBLGQON2,   0x00000000u); /* Enable threshold counter #2 */
#endif

    /* AXI port #3 (AVCC2) */

    R_DEV_WRITE_REG(32, DBLGCNT3,   0x01010100u);
    R_DEV_WRITE_REG(32, DBTMVAL03,  start3); /* Set initial count to 8192 */
    R_DEV_WRITE_REG(32, DBRQCTR3,   0x00000001u);
    R_DEV_WRITE_REG(32, DBTHRES03,  thresh0); /* First threshold */
    R_DEV_WRITE_REG(32, DBTHRES13,  thresh1); /* Second threshold */
    R_DEV_WRITE_REG(32, DBTHRES23,  thresh2); /* Third threshold */
#if 0   /* MNI: This hack *should* put every bus master except the VDCO layers in low priority mode */
    R_DEV_WRITE_REG(32, DBLGQON3,   0x00000001u); /* Enable threshold counter #3 */      
#else
    R_DEV_WRITE_REG(32, DBLGQON3,   0x00000000u); /* Enable threshold counter #3 */
#endif

    /* AXI write ports */
    
    /* AXI port #0 (ACC) */
    
    R_DEV_WRITE_REG(32, DBLGCNT4,   0x01010100u);
    R_DEV_WRITE_REG(32, DBTMVAL04,  start0); /* Set initial count to 8192 */
    R_DEV_WRITE_REG(32, DBRQCTR4,   0x00000001u);
    R_DEV_WRITE_REG(32, DBTHRES04,  thresh0); /* First threshold */
    R_DEV_WRITE_REG(32, DBTHRES14,  thresh1); /* Second threshold */
    R_DEV_WRITE_REG(32, DBTHRES24,  thresh2); /* Third threshold */
#if 1   /* MNI: This hack *should* put every bus master except the VDCO layers in low priority mode */
    R_DEV_WRITE_REG(32, DBLGQON4,   0x00000001u); /* Enable threshold counter #4 */
#else
    R_DEV_WRITE_REG(32, DBLGQON4,   0x00000000u); /* Enable threshold counter #4 */
#endif
    
    /* AXI port #1 (DVCC) */
    
    R_DEV_WRITE_REG(32, DBLGCNT5,   0x01010100u);
    R_DEV_WRITE_REG(32, DBTMVAL05,  start1); /* Set initial count to 8192 */
    R_DEV_WRITE_REG(32, DBRQCTR5,   0x00000001u);
    R_DEV_WRITE_REG(32, DBTHRES05,  thresh0); /* First threshold */
    R_DEV_WRITE_REG(32, DBTHRES15,  thresh1); /* Second threshold */
    R_DEV_WRITE_REG(32, DBTHRES25,  thresh2); /* Third threshold */
#if 1   /* MNI: This hack *should* put every bus master except the VDCO layers in low priority mode */
    R_DEV_WRITE_REG(32, DBLGQON5,   0x00000001u); /* Enable threshold counter #5 */
#else
    R_DEV_WRITE_REG(32, DBLGQON5,   0x00000000u); /* Enable threshold counter #5 */
#endif

    /* AXI port #2 (AVCC1) */

    R_DEV_WRITE_REG(32, DBLGCNT6,   0x01010100u);
    R_DEV_WRITE_REG(32, DBTMVAL06,  start2); /* Set initial count to 8192 */
    R_DEV_WRITE_REG(32, DBRQCTR6,   0x00000001u);
    R_DEV_WRITE_REG(32, DBTHRES06,  thresh0); /* First threshold */
    R_DEV_WRITE_REG(32, DBTHRES16,  thresh1); /* Second threshold */
    R_DEV_WRITE_REG(32, DBTHRES26,  thresh2); /* Third threshold */
#if 0   /* MNI: This hack *should* put every bus master except the VDCO layers in low priority mode */
    R_DEV_WRITE_REG(32, DBLGQON6,   0x00000001u); /* Enable threshold counter #6 */
#else
    R_DEV_WRITE_REG(32, DBLGQON6,   0x00000000u); /* Enable threshold counter #6 */
#endif

    /* AXI port #3 (AVCC2) */

    R_DEV_WRITE_REG(32, DBLGCNT7,   0x01010100u);
    R_DEV_WRITE_REG(32, DBTMVAL07,  start3); /* Set initial count to 8192 */
    R_DEV_WRITE_REG(32, DBRQCTR7,   0x00000001u);
    R_DEV_WRITE_REG(32, DBTHRES07,  thresh0); /* First threshold */
    R_DEV_WRITE_REG(32, DBTHRES17,  thresh1); /* Second threshold */
    R_DEV_WRITE_REG(32, DBTHRES27,  thresh2); /* Third threshold */
#if 0   /* MNI: This hack *should* put every bus master except the VDCO layers in low priority mode */
    R_DEV_WRITE_REG(32, DBLGQON7,   0x00000001u); /* Enable threshold counter #7 */
#else
    R_DEV_WRITE_REG(32, DBLGQON7,   0x00000000u); /* Enable threshold counter #7 */
#endif

}

void loc_InitSDRBMemArbiter_Oliver(void)
{
    /* AXI read ports */
    
    /* AXI port #0 (VCC0) */
    
    R_DEV_WRITE_REG(32, DBLGCNT0,   0x01000000u);
    R_DEV_WRITE_REG(32, DBTMVAL00,  0x00002001u);
    R_DEV_WRITE_REG(32, DBRQCTR0,   0x00000001u);
    R_DEV_WRITE_REG(32, DBTHRES00,  0x00003ffeu);
    R_DEV_WRITE_REG(32, DBTHRES10,  0x00003ffdu);
    R_DEV_WRITE_REG(32, DBTHRES20,  0x00003ffcu);

    R_DEV_WRITE_REG(32, DBLGQON0,   0x00000001u); 

    /* AXI port #1 (VCC1) */
    
    R_DEV_WRITE_REG(32, DBLGCNT1,   0x01000000u);
    R_DEV_WRITE_REG(32, DBTMVAL01,  0x2001); 
    R_DEV_WRITE_REG(32, DBRQCTR1,   0x00000001u);
    R_DEV_WRITE_REG(32, DBTHRES01,  0x3ffe); 
    R_DEV_WRITE_REG(32, DBTHRES11,  0x3ffd); 
    R_DEV_WRITE_REG(32, DBTHRES21,  0x3ffc); 

    R_DEV_WRITE_REG(32, DBLGQON1,   0x00000001u);       


    /* AXI port #2 (ACC) */

    R_DEV_WRITE_REG(32, DBLGCNT2,   0x01000000u);
    R_DEV_WRITE_REG(32, DBTMVAL02,  0x3fff); 
    R_DEV_WRITE_REG(32, DBRQCTR2,   0x000000001);
    R_DEV_WRITE_REG(32, DBTHRES02,  0x0003); 
    R_DEV_WRITE_REG(32, DBTHRES12,  0x0002);
    R_DEV_WRITE_REG(32, DBTHRES22,  0x0001); 

    R_DEV_WRITE_REG(32, DBLGQON2,   0x00000000u); 


    /* AXI port #3 (DVCC) */

    R_DEV_WRITE_REG(32, DBLGCNT3,   0x01000000u);
    R_DEV_WRITE_REG(32, DBTMVAL03,  0x3fff); 
    R_DEV_WRITE_REG(32, DBRQCTR3,   0x00000001u);
    R_DEV_WRITE_REG(32, DBTHRES03,  0x0003); 
    R_DEV_WRITE_REG(32, DBTHRES13,  0x0002); 
    R_DEV_WRITE_REG(32, DBTHRES23,  0x0001); 

    R_DEV_WRITE_REG(32, DBLGQON3,   0x00000000u); 


    /* AXI write ports */
    
    /* AXI port #0 (VCC0) */
    
    R_DEV_WRITE_REG(32, DBLGCNT4,   0x0100000u);
    R_DEV_WRITE_REG(32, DBTMVAL04,  0x2001);
    R_DEV_WRITE_REG(32, DBRQCTR4,   0x00000001u);
    R_DEV_WRITE_REG(32, DBTHRES04,  0x3fff);
    R_DEV_WRITE_REG(32, DBTHRES14,  0x3ffe);
    R_DEV_WRITE_REG(32, DBTHRES24,  0x3ffd);

    R_DEV_WRITE_REG(32, DBLGQON4,   0x00000001u);


    /* AXI port #1 (VCC1) */
    
    R_DEV_WRITE_REG(32, DBLGCNT5,   0x01000000u);
    R_DEV_WRITE_REG(32, DBTMVAL05,  0x2001); 
    R_DEV_WRITE_REG(32, DBRQCTR5,   0x00000001u);
    R_DEV_WRITE_REG(32, DBTHRES05,  0x3fff); 
    R_DEV_WRITE_REG(32, DBTHRES15,  0x3ffe); 
    R_DEV_WRITE_REG(32, DBTHRES25,  0x3ffd); 

    R_DEV_WRITE_REG(32, DBLGQON5,   0x00000001u);     


    /* AXI port #2 (ACC) */

    R_DEV_WRITE_REG(32, DBLGCNT6,   0x01000000u);
    R_DEV_WRITE_REG(32, DBTMVAL06,  0x3fff); 
    R_DEV_WRITE_REG(32, DBRQCTR6,   0x00000001u);
    R_DEV_WRITE_REG(32, DBTHRES06,  0x0003); 
    R_DEV_WRITE_REG(32, DBTHRES16,  0x0002); 
    R_DEV_WRITE_REG(32, DBTHRES26,  0x0001);

    R_DEV_WRITE_REG(32, DBLGQON6,   0x00000000u); 


    /* AXI port #3 (DVCC) */

    R_DEV_WRITE_REG(32, DBLGCNT7,   0x01000000u);
    R_DEV_WRITE_REG(32, DBTMVAL07,  0x3fff); 
    R_DEV_WRITE_REG(32, DBRQCTR7,   0x00000001u);
    R_DEV_WRITE_REG(32, DBTHRES07,  0x0003); 
    R_DEV_WRITE_REG(32, DBTHRES17,  0x0002); 
    R_DEV_WRITE_REG(32, DBTHRES27,  0x0001); 

    R_DEV_WRITE_REG(32, DBLGQON7,   0x00000000u);

}
#endif

/*******************************************************************************
  Function: loc_InitSDRBMemMangoRaw
  
  Init SDRB (DBSC3; DDR2-SDRAM) Memc of D1M2(H) Mango with 512 MByte
  2 pieces of external Micron SDRAM "MT47H128M16RT-25E IT".
  (2x 128Mx16bit = 2x 2Gbit = 2x 256 MByte = 512 MByte)
  Use the register based setup.
*/
static void loc_InitSDRBMemMangoRaw(void)
{
    uint32_t regval;
    uint32_t use_32bit;

    /* This BSP covers the initialisation of the D1M2H (1412) and the D1M2 (1410) 
       As the D1M2 only has 16bit SDRAM, the SDRB initialisation needs to be adjusted accordingly */
    if (R_DEV_GetDev() == R_DEV_R7F701412)
    {
        use_32bit = 1u;
    } 
    else 
    {
        use_32bit = 0u;
    }

	/* DBKIND */
    R_DEV_WRITE_REG(32, 0xFFFA0020u,  0x00000005u); /* 3'b101:DDR2-SDRAM */
    /* DBPDNCNF */
    R_DEV_WRITE_REG(32, 0xFFFA0180u,  0x00001000u); /* PDWAIT = 8'h10, PDDLL=0, PDMODE=0 */

    if (1u == use_32bit)
    {    /* DBCONF0 */
        R_DEV_WRITE_REG(32, 0xFFFA0024u,  0x0e030a02u); /* AWRW0=5'h0e(14bit),AWRK=0,AWBK=2'b11(8bank),AWCL0=4'ha(10bit),DW0=2'b10(32bit),2'b10(32bit) */
    } 
    else 
    {
        /* DBCONF0 */
        R_DEV_WRITE_REG(32, 0xFFFA0024u,  0x0e030a01u); /* AWRW0=5'h0e(14bit),AWRK=0,AWBK=2'b11(8bank),AWCL0=4'ha(10bit),DW0=2'b10(32bit),2'b01(16bit) */
    }
    /* DBPHYTYPE */
    R_DEV_WRITE_REG(32, 0xFFFA0030u,  0x00000000u); /* PHYTYPE=2'b00(MSPAD1) */
    /* DBTR0 */
    R_DEV_WRITE_REG(32, 0xFFFA0040u,  0x00000004u); /* CL=4'h4(CL=4) */
    /* DBTR1 */
    R_DEV_WRITE_REG(32, 0xFFFA0044u,  0x00000003u); /* CWL=4'h3(CWL=3) */
    /* DBTR2 */
    R_DEV_WRITE_REG(32, 0xFFFA0048u,  0x00000000u); /* AL=0 */
    /* DBTR3 */
    R_DEV_WRITE_REG(32, 0xFFFA0050u,  0x00000004u); /* TRCD=4'h4(TRCD=4) */
    /* DBTR4 */
    R_DEV_WRITE_REG(32, 0xFFFA0054u,  0x00050004u); /* PREAtoACT=5'h5(PREA=5), PREtoACT=5'h4(TRP=4) */
    /* DBTR5 */
    R_DEV_WRITE_REG(32, 0xFFFA0058u,  0x0000000Eu); /* TRC=6'h0e(ACTtoACT=14) */
    /* DBTR6 */
    R_DEV_WRITE_REG(32, 0xFFFA005Cu,  0x0000000Au); /* TRAS=6'h0a(ACTtoPRE=10) */
    /* DBTR7 */
    R_DEV_WRITE_REG(32, 0xFFFA0060u,  0x00000003u); /* TRRD=4'h3(ACTtoACT(diffbank)=3) */
    /* DBTR8 */
    R_DEV_WRITE_REG(32, 0xFFFA0064u,  0x0000000Bu); /* TFAW=8'hb(TFAW=12) */
    /* DBTR9 */
    if (1u == use_32bit)
    {
        R_DEV_WRITE_REG(32, 0xFFFA0068u,  0x00000002u); /* TRDPR=4'h2(READtoPRE=2) */
    } 
    else 
    {
        R_DEV_WRITE_REG(32, 0xFFFA0068u,  0x00000004u); /* TRDPR=4'h4(READtoPRE=4) */
    }
    /* DBTR10 */
    R_DEV_WRITE_REG(32, 0xFFFA006cu,  0x00000004u); /* TWR=4'h4(WRrecovery=4) */
    /* DBTR11 */
    if (1u == use_32bit)
    {
        R_DEV_WRITE_REG(32, 0xFFFA0070u,  0x00000007u); /* TRDWR=6'h7(READtoWRITE=7) */
    } 
    else 
    {
        R_DEV_WRITE_REG(32, 0xFFFA0070u,  0x00000009u); /* TRDWR=6'h9(READtoWRITE=9) */
    }
    /* DBTR12 */
    if (1u == use_32bit)
    {
        R_DEV_WRITE_REG(32, 0xFFFA0074u,  0x00000007u); /* NEEDS CONFIRMATION! >>TWRRD=6'h7(WRITEtoREAD=7=CWL(3)+BL4/2+tWTR(2)) */
    } 
    else 
    {
        R_DEV_WRITE_REG(32, 0xFFFA0074u,  0x00000009u); /* NEEDS CONFIRMATION! >>TWRRD=6'h9(WRITEtoREAD=9=CWL(3)+BL4/2+tWTR(2)) */
    }
    /* DBTR13 */
    R_DEV_WRITE_REG(32, 0xFFFA0078u,  0x0000002fu); /* TRFC=9'h2f(REFtoACT=47) */
    /* DBTR14 */
    R_DEV_WRITE_REG(32, 0xFFFA007cu,  0x00060002u); /* TCKEHDLL=8'h06,TCKEH=8'h2 */
    /* DBTR15 */
    R_DEV_WRITE_REG(32, 0xFFFA0080u,  0x00000003u); /* TCKEL=4'h3 */
    /* DBTR16 */
    R_DEV_WRITE_REG(32, 0xFFFA0084u,  0x12091001u); /* DQIENLTNCY=1, DQLOFFSET=2, dqltncy=9, dqenltncy=1,WDQL=1 */
    /* DBTR17 */
    R_DEV_WRITE_REG(32, 0xFFFA0088u,  0x00080000u); /* TMOD=6'h08 */
    /* DBTR18 */
    if (1u == use_32bit)
    {
        R_DEV_WRITE_REG(32, 0xFFFA008cu,  0x00070007u); /* RODTL=0,RODTA=7,WODTL=0,WODTA=7 */
    }
    else
    {
        R_DEV_WRITE_REG(32, 0xFFFA008cu,  0x00090009u); /* RODTL=0,RODTA=9,WODTL=0,WODTA=9 */
    }
    
    /* DBBL */
    if (1u == use_32bit)
    {
        R_DEV_WRITE_REG(32, 0xFFFA00b0u,  0x00000002u); /* BL=2(4burst), BL=0(8burst) */
    }
    else
    {
        R_DEV_WRITE_REG(32, 0xFFFA00b0u,  0x00000000u); /* BL=0(2burst), BL=0(8burst) */
    }
    /* DBRNK0 */
    R_DEV_WRITE_REG(32, 0xFFFA0100u,  0x00000001u); /* RODTOUT0=0, WODTOUT0=1 */
    /* DBADJ0 */
    R_DEV_WRITE_REG(32, 0xFFFA00c0u,  0x00010000u); /* FREQRATIO = 1(1:2), CAMODE=0(1/1 command) */
    /* DBADJ2 */
    #if 1
    /*Original*/
    /* ACAPC0=8'h20(32cell), ACAP0=4'h8(8transaction) */
    R_DEV_WRITE_REG(32, 0xFFFA00c8u,  0x00002002u); /* Shorten pipeline to 2 gives best performance */
    /* R_DEV_WRITE_REG(32, 0xFFFA00c8,  0x00002006); */
    /* R_DEV_WRITE_REG(32, 0xFFFA00c8,  0x00002008); */
    #else
    /*Under Test*/
    R_DEV_WRITE_REG(32, 0xFFFA00c8u,  0x00002002u); /* ACAPC0=8'h20(32cell), ACAP0=4'h1(1transaction) */
    #endif
           
    /* Configuration for MSPAD1E */
    /* DBPDCNT0 */
    R_DEV_WRITE_REG(32, 0xFFFA0200u,  0x00000000u); /* dw32=0, dw16=1 */
    /* DBPDCNT1 */
    R_DEV_WRITE_REG(32, 0xFFFA0204u,  0x44000044u); /* db_offset=2, db_enoffset=4, db_ewc_open_offset0=3'b100, db_ewc_open_offset1=3'b100 */
    /* DBPDCNT3 */
    R_DEV_WRITE_REG(32, 0xFFFA020cu,  0x10000000u); /* release the db_stby_n to 1 */

    /* WAIT */
    regval = R_DEV_READ_REG(32, 0xFFFA001Cu); /* wait until read DBWAIT */

    /* reset release MRSTC */   
    /* Release all Modules by module reset control register */
    regval  = R_DEV_READ_REG(32, LOC_MRSTC);
    regval |= (MRSTC_XC0RES | MRSTC_SDRB0RES);
    R_DEV_WRITE_PROT_REG(LOC_PROTCMDMRST, LOC_MRSTC, regval);
    while (0 != R_DEV_READ_REG(32, LOC_PROTSMRST))  /* Read PROTSMRST */
    {
        /* PROTSMRST - register protection sequence fail */
    }
    
    /* DBCMD */
    R_DEV_WRITE_REG(32, 0xFFFA0018u,  0x10040010u); /* power down exit */
    /* DBCMD */
    R_DEV_WRITE_REG(32, 0xFFFA0018u,  0x0000c000u); /* wait(49152cycle=203usec) */
    /* DBCMD */
    R_DEV_WRITE_REG(32, 0xFFFA0018u,  0x11040061u); /* CKE to high */
    /* DBCMD */
    R_DEV_WRITE_REG(32, 0xFFFA0018u,  0x0b000000u); /* PREA */
    /* DBCMD */
    R_DEV_WRITE_REG(32, 0xFFFA0018u,  0x2a000080u); /* EMRS2 > 2x refresh rate*/
    /* DBCMD */
    R_DEV_WRITE_REG(32, 0xFFFA0018u,  0x2b000000u); /* EMRS3 */
    /* DBCMD */
    R_DEV_WRITE_REG(32, 0xFFFA0018u,  0x29000000u); /* EMRS1 */
    /* DBCMD */
    if (1u == use_32bit)
    {
        R_DEV_WRITE_REG(32, 0xFFFA0018u,  0x28000742u); /* EMRS0 */
    }
    else
    {
        R_DEV_WRITE_REG(32, 0xFFFA0018u,  0x28000743u); /* EMRS0 */
    }
    /* DBCMD */
    R_DEV_WRITE_REG(32, 0xFFFA0018u,  0x0b000000u); /* PREA */
    /* DBCMD */
    R_DEV_WRITE_REG(32, 0xFFFA0018u,  0x0c000000u); /* REF */
    /* DBCMD */
    R_DEV_WRITE_REG(32, 0xFFFA0018u,  0x0c000000u); /* REF */
    /* DBCMD */
    R_DEV_WRITE_REG(32, 0xFFFA0018u,  0x00000100u); /* wait(256cycle) */
    /* DBCMD */
    if (1u == use_32bit)
    {
        R_DEV_WRITE_REG(32, 0xFFFA0018u,  0x28000642u); /* MRS0 */
    }
    else
    {
        R_DEV_WRITE_REG(32, 0xFFFA0018u,  0x28000643u); /* MRS0 */
    }

    /* DBCMD */
    R_DEV_WRITE_REG(32, 0xFFFA0018u,  0x000000c8u); /* wait(200cycle) */
    /* DBCMD */
    R_DEV_WRITE_REG(32, 0xFFFA0018u,  0x29000380u); /* EMRS1 */
    /* DBCMD */
    R_DEV_WRITE_REG(32, 0xFFFA0018u,  0x29000000u); /* EMRS1 */
    /* DBRFCNF0 */
    if (1u == use_32bit)
    {
        R_DEV_WRITE_REG(32, 0xFFFA00e0u,  0x00000080u); /* REFTHF=12'h080(128cycle) */
    }
    else
    {
        R_DEV_WRITE_REG(32, 0xFFFA00e0u,  0x00000082u); /* REFTHF=12'h082(???cycle) */
    }
    /* DBRFCNF1 */
    R_DEV_WRITE_REG(32, 0xFFFA00e4u,  0x00100753u); /* REFPMAX=16'h2, REFINT=16'h753(1875) */
    /* DBRFCNF2 */
    R_DEV_WRITE_REG(32, 0xFFFA00e8u,  0x00010001u); /* REFPMIN=1,REFINTS=1 (3.9us automotive refresh)*/
    /* DBRFEN */
    R_DEV_WRITE_REG(32, 0xFFFA0014u,  0x00000001u); /* ARFEN=1 */
    /* DBACEN */
    R_DEV_WRITE_REG(32, 0xFFFA0010u,  0x00000001u); /* ACCEN=1 */

    /* WAIT */
    regval = R_DEV_READ_REG(32, 0xFFFA001Cu);      /* wait until read DBWAIT */
}


/*******************************************************************************
  Function: loc_SysGfxInitD1M2
  
    Board and device specific init 
*/
void loc_SysGfxInitD1M2(void) 
{
    /*MNI*/
    /* Setup of the VRAM arbiter */
    
#if 0   /* Values for 0% and 30% load */
    /* Restrict write transactions, allow 3 x 16-beat read bursts before going back to arbitration */
    R_DEV_WRITE_REG(32, VRAM1CONTROL+VRMTRCTL,   0x10010030u); /* 3 x 16-beat = 48 = 0x30 */
    R_DEV_WRITE_REG(32, VRAM1CONTROL+VRMTRINTVL, 0x00000001u); /* Set the time window for write transactions */
    R_DEV_WRITE_REG(32, VRAM2CONTROL+VRMTRCTL,   0x10010030u); /* */
    R_DEV_WRITE_REG(32, VRAM2CONTROL+VRMTRINTVL, 0x00000001u); /* */
#endif
#if 0   /* Values for 60% load */
    /* Restrict write transactions, allow 3 x 16-beat read bursts before going back to arbitration */
    R_DEV_WRITE_REG(32, VRAM1CONTROL+VRMTRCTL,   0x20010060u); /* 3 x 16-beat = 48 = 0x30 */
    R_DEV_WRITE_REG(32, VRAM1CONTROL+VRMTRINTVL, 0x00000001u); /* Set the time window for write transactions */
    R_DEV_WRITE_REG(32, VRAM2CONTROL+VRMTRCTL,   0x20010060u); /* */
    R_DEV_WRITE_REG(32, VRAM2CONTROL+VRMTRINTVL, 0x00000001u); /* */
#endif
#if 0
    /* Values from Kawahito-san
       IDLTIM	128
       RCTL	0b01
       RTOVFL	0
       WTOVFL	32, 49, 66
    */
    
    #if 0 /* Single layer 48 MHz best case */
        R_DEV_WRITE_REG(32, VRAM1CONTROL+VRMTRCTL,   32<<24 | 1<<16 | 16);
        R_DEV_WRITE_REG(32, VRAM2CONTROL+VRMTRCTL,   32<<24 | 1<<16 | 16);
    #endif
    #if 1 /* Lowest possible settings that have any effect are IDLTIM 29 and WTOVFL 16
             Good for up to 40 MHz singl layer. To reach higher frquency the IDLTIM has
             to be increased. WTOVFL lower than 16 has no effect. IDLTIM lower than 29
             has no effect. */
        R_DEV_WRITE_REG(32, VRAM1CONTROL+VRMTRCTL,   32<<24 | 1<<16 | 16);
        R_DEV_WRITE_REG(32, VRAM2CONTROL+VRMTRCTL,   32<<24 | 1<<16 | 16);
    #endif
    #if 0
        R_DEV_WRITE_REG(32, VRAM1CONTROL+VRMTRCTL,   128<<24 | 1<<16 | 32);
        R_DEV_WRITE_REG(32, VRAM2CONTROL+VRMTRCTL,   128<<24 | 1<<16 | 32);
    #endif
    #if 0
        R_DEV_WRITE_REG(32, VRAM1CONTROL+VRMTRCTL,   128<<24 | 1<<16 | 49);
        R_DEV_WRITE_REG(32, VRAM2CONTROL+VRMTRCTL,   128<<24 | 1<<16 | 49);
    #endif
    #if 0
        R_DEV_WRITE_REG(32, VRAM1CONTROL+VRMTRCTL,   128<<24 | 1<<16 | 66);
        R_DEV_WRITE_REG(32, VRAM2CONTROL+VRMTRCTL,   128<<24 | 1<<16 | 66);
    #endif

    R_DEV_WRITE_REG(32, VRAM1CONTROL+VRMTRINTVL, 0x00000000u);
    R_DEV_WRITE_REG(32, VRAM2CONTROL+VRMTRINTVL, 0x00000000u);
#endif
    /*END MNI*/

}

/*******************************************************************************
  Function: loc_SysLowInitD1M2
  
    Board and device specific init 
*/
void loc_SysLowInitD1M2(void)
{
    loc_InitSDRBMemMangoRaw();   /* use register based SDRB setup */

    #if 0
    loc_InitSDRBMemArbiter_Matthias();
    loc_InitSDRBMemArbiter_Oliver();
    #endif
    loc_SDRB_InitialiseArbiter();
}


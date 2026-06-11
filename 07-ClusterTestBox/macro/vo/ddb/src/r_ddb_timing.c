/*
****************************************************************************
PROJECT : DDB (Display DataBase) driver
AUTHOR  : Michael Golczewski
FILE    : $Id: r_ddb_timing.c 7791 2016-02-24 09:20:46Z oliver.achten $
============================================================================ 
DESCRIPTION
database for display timings (ddb)
============================================================================
                            C O P Y R I G H T
============================================================================
                       Copyright (c) 2013 - 2014
                                  by 
                       Renesas Electronics (Europe) GmbH. 
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================


DISCLAIMER

LICENSEE has read, understood and accepted the terms and conditions defined in
the license agreement, especially the usage rights. In any case, it is
LICENSEE's responsibility to make sure that any user of the software complies
with the terms and conditions of the signed license agreement.

SAMPLE CODE is provided on an 'as is' base and is not part of the licensed software.
It can only be used, copied or modified as defined in the signed license agreement. 
For the sake of clarity, LICENSEE uses this software on his own risk.
****************************************************************************
*/

/*******************************************************************************
  Title: Main Module

  Implementation of the API and other global functions.
*/


/*******************************************************************************
  Section: Includes
*/

#include <string.h>         /* strcmp */
#include "r_typedefs.h"     /* Renesas basic types, e.g. uint32_t */
#include "r_ddb_api.h"      /* display database with display timings */

#define SCREEN_VENDOR "TFT800480-77-E"
#define SCREEN_PIXEL_PITCH 832
#define SCREEN_PIXEL_WIDTH 800
#define SCREEN_PIXEL_HEIGHT 480

#define DDB_HFP 210
#define DDB_HSYNC 40
#define DDB_HBP 46
#define DDB_VFP 22
#define DDB_VSYNC 3
#define DDB_VBP 23
/*******************************************************************************
  Section: Local Defines
*/

#define LOC_DISPLAY_MODE_NUM  (sizeof(loc_DisplayMode) / sizeof(r_ddb_Timing_t))


/*******************************************************************************
  Section: Local variables
*/

static const r_ddb_Timing_t loc_DisplayMode[] = {

/* Display / VO timings */
    
    /* The list is grouped as follows:          */
    /*  + VESA Display Timings                  */
    /*  + Vendor Specific Timings               */
    /*  + Test Specific Timings                 */
    /*  + Camera Specific Timings               */
    /*                                          */
    /*  The Timings are sorted by Resolution,   */
    /*  then byPixel Clock                      */

    /* Information for creating custom timings  */
    /*                                          */
    /* Some HDMI/DVI monitors require the       */
    /* frequencies to be in a specific range.   */
    /*                                          */
    /* EIZO FlexScan EV2336W                    */
    /*      H: 31 - 68 kHz, V: 59 - 61 Hz       */
    /* NEC MultiSync LCD1970NX                  */
    /*      H: 31 - 81 kHz, V: 56 - 75 Hz       */
    /* NEC MultiSync EA224WMI                   */
    /*      H: 31 - 83 kHz, V: 56 - 75 Hz       */
    /*                                          */
    {
    SCREEN_VENDOR,
    SCREEN_PIXEL_WIDTH, SCREEN_PIXEL_HEIGHT,
    {
        SCREEN_PIXEL_WIDTH + ( DDB_HFP + DDB_HSYNC + DDB_HBP ), ( DDB_HFP + DDB_HSYNC + DDB_HBP ), DDB_HFP, DDB_HSYNC
    },
    {
        SCREEN_PIXEL_HEIGHT + ( DDB_VFP + DDB_VSYNC + DDB_VBP ), ( DDB_VFP + DDB_VSYNC + DDB_VBP ), DDB_VFP, DDB_VSYNC
    },
    R_DDB_DISP_FLAG_DESYNC_ACTHI,
    34*1000*1000
	},

/* CEA Display Timings (for HDTV / HDMI) */

	{"CEA_1920x1080_60Hz",
	1920, 1080,
	{2080, 160, 48, 32},
	{1111, 31, 3, 5},
    ((uint32_t)R_DDB_DISP_FLAG_VOEN
    |(uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
	138500000},
	
/* VESA Display Timings (for VGA / HDMI) */
	
    /* VESA 640x480 60Hz (59.94Hz)*/
    {"VESA_640x480_60Hz",
      640u, 480u,
      {800u, 160u, 16u, 96u},
      {525u,  45u, 10u, 2u},
      ((uint32_t)R_DDB_DISP_FLAG_VOEN
      |(uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
      25175000u},

    /* VESA 800x600 60Hz (60.31Hz)*/
    {"VESA_800x600_60Hz",
      800u, 600u,
      {1056u, 256u, 0u, 128u},
      {628u,  28u, 0u, 4u},
      ((uint32_t)R_DDB_DISP_FLAG_VOEN
      |(uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
      40000000u},

    /* VESA 1024x768 60Hz (60.00Hz)*/
    {"VESA_1024x768_60Hz",
      1024u, 768u,
      {1344u, 320u, 24u, 136u},
      {806u,  38u, 3u, 6u},
      ((uint32_t)R_DDB_DISP_FLAG_VOEN
      |(uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
      65000000u},

    /* VESA 1024x768 75Hz (75.00Hz)*/
    {"VESA_1024x768_75Hz",
      1024u, 768u,
      {1312u, 288u, 16u, 96u},
      {800u,  32u, 1u, 3u},
      ((uint32_t)R_DDB_DISP_FLAG_VOEN
      |(uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
      78800000u},	  
	  
    /* VESA 1280x800 60Hz (59.998Hz)*/
    {"VESA_1280x800_60Hz",
      1280, 800u,
      {1680u, 400u, 64u, 136u},
      {828u,  28u, 1u, 3u},
      ((uint32_t)R_DDB_DISP_FLAG_VOEN
      |(uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
      83460000u},

    /* VESA 1280x1024 60Hz (60.02Hz)*/
    /* This is a VESA timing which will not work with default D1ML PLL configuration! */
    /* To use this VESA timing, make sure that your PLL2 has a working frequency of (108MHz*4) 432MHz instead of (120MHz*4) 480MHz ! */
    {"VESA_1280x1024_60Hz",
      1280, 1024u,
      {1688u, 408u, 48u, 112u},
      {1066u,  42u, 1u, 3u},
      ((uint32_t)R_DDB_DISP_FLAG_VOEN
      |(uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
     108000000u},

/* Vendor Specific Display Timings */
	
    /* Renesas NL2432HC22-41K display, touchscreen, 18bit colour, 8.9cm, QVGA portrait
        (used on RAPP V3.xx) (60.28Hz)*/
    {"NEC_QVGA_TOUCH_240x320",
     240u, 320u,
     {256u, 16u, 4u, 8u},
     {324u,  4u, 1u, 2u},
     R_DDB_DISP_FLAG_NONE,
     5000000u},

    /* Hitachi TX09D70VM1CDA display
        (used on RAPP V1.xx and V2.xx) (60.00Hz)*/
    {"HITACHI_QVGA_TFT_240x320",
     240u, 320u,
     {273u, 33u, 11u, 5u},
     {327u,  7u,  2u, 1u},
     ((uint32_t)R_DDB_DISP_FLAG_VOEN | (uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
     5356260u},

    /* Truly TFT1N6257-E display, touchscreen, 18bit colour, 85mm, QVGA portrait */
    {"TRULY_QVGA_TOUCH_240x320",
     240u, 320u,
     {(240+10+10+20), (10+10+20), 10u, 10u},
     {(320+ 4+ 2+ 2),  (4+ 2+ 2),  4u,  2u},
     R_DDB_DISP_FLAG_DESYNC_ACTHI,
     5640000u},
    
    /* source for driving SHARP display, for TCON test purpose */
    {"TCON_SHARP_240x320",
     240u,320u,
     {283u,43u,31u,5u},
     {330u,10u,2u,2u},
     ((uint32_t)R_DDB_DISP_FLAG_VOEN 
       | (uint32_t)R_DDB_DISP_FLAG_HSYNC_ACTHI 
       | (uint32_t)R_DDB_DISP_FLAG_VSYNC_ACTHI),
     7000000u},
      
    /* NL3224BC35-20 18bit colour Renesas QVGA display module 14cm */
    {"NEC_320x240",
      320u, 240u, 
      {404u, 84u, 7u, 36u},
      {262u, 22u, 1u,  9u},
      R_DDB_DISP_FLAG_NONE, 
      7056000u},
           
    /* Toppoly (TPO) TD022SREC6 display */
    {"TOPPOLY_240x320",
      240u, 320u,
      {280u, (280-240), (280-270), (10-0)},
      {328u, (328-320), (328-324), (2-0)},
      R_DDB_DISP_FLAG_NEGCLK,
      7056000u},


    {"SHARP_480x272_LQ0",  /* Sharp 480x272-LQ043T3DX02*/
     480u,272u,
     {525u,  45u, 2u, 41u},
     {286u,  14u, 2u, 10u},
     R_DDB_DISP_FLAG_NONE, 
     9000000u},                     

    /* Sharp display 480x272 LQ042T5DZ01 */
    {"SHARP_480x272",
      480u, 272u,
      {(480+120), 120u, 2u, (120-116-2)},
      {(272+ 40),  40u, 2u, ( 40- 31-2)},
      R_DDB_DISP_FLAG_NONE, 
      9700000u},

    /* GM India Displays */
    {"SHARP_480x272_LCY0",  /* Sharp 480x272-LQ042T5DZ01_LCY09013A */
     480u,272u,
     {598u, 118u, 2u, 5u},
     {305u,  33u, 2u, 5u},
     R_DDB_DISP_FLAG_NONE, 
     9700000u},


    /* NL6448BC26-09 fixed mode */
    {"NEC_640x480_FIX",
     640u, 480u,
     {800u, 160u, 16u, 96u},
     {525u,  45u, 12u, 2u},
     R_DDB_DISP_FLAG_NONE,
     25175000u},

    /* NL6448BC26-09 DE mode */
    {"NEC_640x480_DE",
     640u, 480u,
     {800u, 160u, 16u, 96u},
     {525u,  45u, 12u, 2u},
     ((uint32_t)R_DDB_DISP_FLAG_VOEN | (uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
     25175000u},

    {"HITACHI_WVGA_TFT_800x480",
     800u,480u,
     {1056u,47u,40u,3u},
     {525u,15u,11u,2u},
     ((uint32_t)R_DDB_DISP_FLAG_VOEN | (uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
     25264000u},

    /* NL8048BC19-02 24bit colour Renesas WVGA display module */
    {"NEC_WVGA_TFT_800x480",
     800u,480u,
     {1024u,1024-800u,40u,3u},
     {525u,525-480u,11u,2u},
     ((uint32_t)R_DDB_DISP_FLAG_VOEN | (uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
     30000000u},
     
    /* Truly_800480-50_E WVGA 24bit colour (61,58Hz) */
    {"TRULY_WVGA_TFT_800x480",
     800u,480u,
     {928u,928-800u,40u,48u},
     {525u,525-480u,13u,3u},
     ((uint32_t)R_DDB_DISP_FLAG_VOEN | (uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
     30000000u},         

    /* Sharp 800x480 (WVGA) 60Hz (62,16Hz)*/
    {"SHARP_LCY-13003C-16",
     800, 480,
     {(800+229), 229, 110, 5},
     {(480+40), 40, 4, 1},
     R_DDB_DISP_FLAG_DESYNC_ACTHI,
     33260000},

    /* Sharp 800x480 (WVGA) 30Hz (31,08Hz)*/
    {"SHARP_LCY-13003C-16_30Hz",
     800, 480,
     {(800+229), 229, 110, 5},
     {(480+40), 40, 4, 1},
     R_DDB_DISP_FLAG_DESYNC_ACTHI,
     33260000/2},

    /* P9491LTPS - 4.3" TPG110 (60,06Hz)*/
    {"TPO_800x480",     
     800u, 480u,
     {1056u, 256u, 40u, 5u},
     { 525u,  45u, 10u, 5u},
     R_DDB_DISP_FLAG_NONE,
     33300000u},  

    /* NL10276BC13-01C 24bit colour Renesas XGA display module
       with reduced clock for usage on SCIT board */
    {"NEC_XGA_24BIT_SLOW",
      1024u, 768u, 
      {1344u, 320u, 22u, 37u}, 
      {806u, 38u, 18u,  3u},
      ((uint32_t)R_DDB_DISP_FLAG_VOEN | (uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
      20000000u},
      
    /* NL10276BC13-01C 24bit colour Renesas XGA display module */
    {"NEC_XGA_24BIT",
      1024u, 768u, 
      {1344u, 320u, 22u, 37u}, 
      {806u, 38u, 18u,  3u},
      ((uint32_t)R_DDB_DISP_FLAG_VOEN | (uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI), 
      65000000u},           

     /* Digital TV Renesas WSXGAP MultiSync EA221WM */
    {"NEC_WSXGAP_DVI",
      1680u, 1050u, 
      {1840u, 160u, 48u, 32u}, 
      {1080u, 30u, 3u,  6u},
      ((uint32_t)R_DDB_DISP_FLAG_VOEN | (uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
      119000000u},   


/* Display Timings for Test Purposes (e.g. Non-Standard or Non-vendor) */
  
/* WIDE 1920x720 HDMI display, unknown vendor, syncs also on NEC EA224WMi monitor */

	{"WIDE_1920x720_60Hz",
	1920, 720,
	{2000, 80, 20, 30},
	{730, 10, 3, 3},
	((uint32_t)R_DDB_DISP_FLAG_VOEN
	|(uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
	87600000},

/* 1920x720 timing, works on NEC EA224WMi & EIZO monitors */
	
	{"1920x720_60Hz",
	1920, 720,
    {2392, 472, 32, 408},
    {757, 37, 15, 7},
	((uint32_t)R_DDB_DISP_FLAG_VOEN
	|(uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
    110000000},
	
     /* 400 x 120 @60fps */
    {"400x120_60FPS",   
     400u, 120u, 
     {480u, 80u, 30u, 10u}, 
     {144u, 24u, 10u, 2u},
     R_DDB_DISP_FLAG_DESYNC_ACTHI,
     4150000u},

    /* minimal setting for test purpose */
    {"DISP_MIN",
      320u,240u, 
      {420u,100u, 22u, 2u}, 
      {280u, 40u, 18u,  1u},
      R_DDB_DISP_FLAG_NONE, 
      7056000u},
    
    /* 640x480 30Hz (non VESA - half fps) */
    {"640x480_30Hz",
      640u, 480u,
      {800u, 160u, 16u, 96u},
      {525u,  45u, 10u, 2u},
      ((uint32_t)R_DDB_DISP_FLAG_VOEN
      |(uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
      25175000u/2},

    /*1024x768 40Hz (non VESA - 2/3 fps) */
    {"1024x768_40Hz",
      1024u, 768u,
      {1344u, 320u, 24u, 136u},
      {806u,  38u, 3u, 6u},
      ((uint32_t)R_DDB_DISP_FLAG_VOEN
      |(uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
      43300000u},

    /* 800x480 60Hz (59.94Hz) (Close to VESA - Derived from VESA 800x600 and 640x480 60Hz) */
    {"800x480_60Hz",
      800u, 480u,
      {1056u, 256u, 0u, 128u},
      {525u,  45u, 10u, 2u},
      ((uint32_t)R_DDB_DISP_FLAG_VOEN
      |(uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
      33264000u},

    /* 1280x480 60Hz (Close to VESA - Derived from VESA 1280x800 60Hz) */
    {"1280x480_60Hz",
      1280, 480u,
      {1680u, 400u, 64u, 136u},
      {528u,  48u,  10u, 2u},
      ((uint32_t)R_DDB_DISP_FLAG_VOEN
      |(uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
      53222400u},

    {"1280x480_75Hz",
      1280, 480u,
      {1610u, 400u, 64u, 136u},
      {528u,  48u,  10u, 2u},
      ((uint32_t)R_DDB_DISP_FLAG_VOEN
      |(uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
      65000000u},
	  
    {"TES_1280x720",
     1280u, 720u, 
     {1648u, 368u, 72u, 80u}, 
     {750u, 30u, 3u,  5u},
     ((uint32_t)R_DDB_DISP_FLAG_VOEN | (uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
     74200000u},
    
    /* 1280x768 60Hz */
    {"TFT_1280x768_60Hz",
      1280u, 768u,
      {1664u, 384u, 64u, 128u},
      {798u, 30u, 3u, 7u},
      ((uint32_t)R_DDB_DISP_FLAG_VOEN
      |(uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
      79500000u},
    
    {"TES_1280x1024",
     1280u, 1024u, 
     {844*2u, (844*2)-1280 , 22u, 37u}, 
     {1066u, 1066-1024u, 18u, 3u},
     ((uint32_t)R_DDB_DISP_FLAG_VOEN | (uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
     108000000u},
     
    /* 1280x1024 60Hz PLL:120MHz - 
       This is a VESA timing, adapted such that it work with a PLL that cannot be set to 108MHz but 120MHz only */
    /* To use the REAL VESA "1280x1024_60Hz" timing, make sure that your PLL has a working frequency of a multiple of 108MHz! */
    {"1280x1024_60Hz_PLL120MHz",
      1280, 1024u,
      {1876u, 596u, 48u, 112u},
      {1066u,  42u, 1u, 3u},
      ((uint32_t)R_DDB_DISP_FLAG_VOEN
      |(uint32_t)R_DDB_DISP_FLAG_DESYNC_ACTHI),
     120000000u},


    /* pseudo display with size 4096x4096u, for test purpose */
    {"DISP_PSEUDO_4kx4k",
      4096u, 4096u, 
      {4096+1024u, 1024u, 128u, 256u}, 
      {4096+1024u, 1024u, 128u, 256u},
      R_DDB_DISP_FLAG_NONE, 
      50000000u},
      
/* Camera / VI timings */
      
    /* Sony EVI-R51 camera, ITU656 (YUV interlaced) */
    {"VI_CAMERA_SONY_EVI-R51",
     704u, 288u,
     {704u, 0u, 0u, 0u},
     {288u, 0u, 0u, 0u},
     R_DDB_DISP_FLAG_NONE,
     27000000u},
    };  
    
    

/*******************************************************************************
  Section: Global functions
*/

/*******************************************************************************
  Function: R_DDB_GetDisplayTiming
*/
r_ddb_Timing_t *R_DDB_GetDisplayTiming(const char *DisplayId)
{
    uint32_t i;
    int32_t  x;

    for(i = 0u; i < LOC_DISPLAY_MODE_NUM; i++)
    {
        x = strcmp(loc_DisplayMode[i].Name, DisplayId);
        if (x == 0)
        {
            return (r_ddb_Timing_t*) &loc_DisplayMode[i];
        }
    }    
    return 0;
}


/*
****************************************************************************
PROJECT : MEMDB
FILE    : $Id: r_memdb_main.c 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================
DESCRIPTION
MEMDB memory database
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
#include "r_typedefs.h"
#include "r_memdb_api.h"


/*******************************************************************************
  Section: Local Defines
*/

/*******************************************************************************
  Constants: Bit flags
  
  LOC_MEMORY_NUM - number of memories in the loc_MemoryMode table
*/

#define LOC_MEMORY_NUM     (sizeof(loc_MemoryMode) / sizeof(r_memdb_Timing_t))


/*******************************************************************************
  Section: Local Variables
*/

/*******************************************************************************
  Variable: loc_MemoryMode

  A table with entries for different memory devices.
*/

static const r_memdb_Timing_t loc_MemoryMode[] = {
        {
        "FPGA",    /* NEC SRAM 16Mb */
        16u,         /* Data width in bits */
        2u,          /* Memory type: 0 = SDR-SDRAM (dyn.), 1 = SRAM (static), 2 = Flash (static), 3 = DDR1-SDRAM (dyn.) */
        16384u,       /* Size of memory device in KB */
        0u,          /* Dynamic: width of column address */
        0u,          /* Dynamic: width of row address */
        0u,          /* Dynamic: width of bank address */
        0u,          /* Dynamic: trc time in ns active to active command trc */
        0u,          /* Dynamic: txsr time in ns exit self refresh to active command txsr */
        0u,          /* Dynamic: trcar time in ns auto refresh period trcar */
        0u,          /* Dynamic: twr time in ns data to precharge twr */
        0u,          /* Dynamic: trp time in ns precharge period */
        0u,          /* Dynamic: trpa time in ns precharge all */
        0u,           /* Dynamic: trtp time in ns read->precharge */      
        0u,          /* Dynamic: trrd time in ns min. interval between active commands */
        0u,          /* Dynamic: tfaw time in ns min. interval to activate 4 banks */        
        0u,          /* Dynamic: trcd time in ns min. delay active to rd/wr commands trcd */
        0u,          /* Dynamic: trfc time in ns min. refresh->act delay */    
        0u,          /* Dynamic: tmod time in ns MRS command to ODT update delay */
        0u,          /* Dynamic: tckemin time in clock cycles cke power-down timing */        
        0u,          /* Dynamic: trasmin time in ns min. delay active to precharge commands tras_min */
        0u,          /* Dynamic: CasLatency time delay read command and availability of first data */
        0u,          /* Dynamic: time in ns between consecutive refresh cycles */
        0u,          /* Dynamic: Number of auto-refreshes during init */   
        0u,          /* Dynamic: Number of ns to SDRAM inputs stable after power up */                                              
        0u,          /* Static: Page size in words (0 = no page mode support) */
        255u,          /* Static: tprc time in ns page mode read cycle */
        255u,          /* Static: tbta time in ns memory idle cycles rd2wr or wr2rd or rd2rd */
        255u,         /* Static: twp  time in ns write pulse width*/
        255u,          /* Static: twr time in ns write address/data hold time */
        255u,          /* Static: tas time in ns write address setup time */
        255u          /* Static: trc time in ns read cycle time */
                               
    },
    {
        "UPD44160116-A15",    /* NEC SRAM 16Mb */
        16u,         /* Data width in bits */
        1u,          /* Memory type: 0 = SDR-SDRAM (dyn.), 1 = SRAM (static), 2 = Flash (static), 3 = DDR1-SDRAM (dyn.) */
        2048u,       /* Size of memory device in KB */
        0u,          /* Dynamic: width of column address */
        0u,          /* Dynamic: width of row address */
        0u,          /* Dynamic: width of bank address */
        0u,          /* Dynamic: trc time in ns active to active command trc */
        0u,          /* Dynamic: txsr time in ns exit self refresh to active command txsr */
        0u,          /* Dynamic: trcar time in ns auto refresh period trcar */
        0u,          /* Dynamic: twr time in ns data to precharge twr */
        0u,          /* Dynamic: trp time in ns precharge period */
        0u,          /* Dynamic: trpa time in ns precharge all */
        0u,          /* Dynamic: trtp time in ns read->precharge */      
        0u,          /* Dynamic: trrd time in ns min. interval between active commands */
        0u,          /* Dynamic: tfaw time in ns min. interval to activate 4 banks */     
        0u,          /* Dynamic: trcd time in ns min. delay active to rd/wr commands trcd */
        0u,          /* Dynamic: trfc time in ns min. refresh->act delay */    
        0u,          /* Dynamic: tmod time in ns MRS command to ODT update delay */
        0u,          /* Dynamic: tckemin time in clock cycles cke power-down timing */   
        0u,          /* Dynamic: trasmin time in ns min. delay active to precharge commands tras_min */
        0u,          /* Dynamic: CasLatency time delay read command and availability of first data */
        0u,          /* Dynamic: time in ns between consecutive refresh cycles */
        0u,          /* Dynamic: Number of auto-refreshes during init */   
        0u,          /* Dynamic: Number of ns to SDRAM inputs stable after power up */                                              
        0u,          /* Static: Page size in words (0 = no page mode support) */
        0u,          /* Static: tprc time in ns page mode read cycle */
        7u,          /* Static: tbta time in ns memory idle cycles rd2wr or wr2rd or rd2rd */
        10u,         /* Static: twp  time in ns write pulse width*/
        1u,          /* Static: twr time in ns write address/data hold time */
        0u,          /* Static: tas time in ns write address setup time */
        15u          /* Static: trc time in ns read cycle time */
                               
    },
    {
        "UPD442012AGY-BB55X",    /* NEC SRAM 2Mb */
        16u,         /* Data width in bits */
        1u,          /* Memory type: 0 = SDR-SDRAM (dyn.), 1 = SRAM (static), 2 = Flash (static), 3 = DDR1-SDRAM (dyn.) */
        256u,        /* Size of memory device in KB */
        0u,          /* Dynamic: width of column address */
        0u,          /* Dynamic: width of row address */
        0u,          /* Dynamic: width of bank address */
        0u,          /* Dynamic: trc time in ns active to active command trc */
        0u,          /* Dynamic: txsr time in ns exit self refresh to active command txsr */
        0u,          /* Dynamic: trcar time in ns auto refresh period trcar */
        0u,          /* Dynamic: twr time in ns data to precharge twr */
        0u,          /* Dynamic: trp time in ns precharge period */
        0u,          /* Dynamic: trpa time in ns precharge all */
        0u,          /* Dynamic: trtp time in ns read->precharge */      
        0u,          /* Dynamic: trrd time in ns min. interval between active commands */
        0u,          /* Dynamic: tfaw time in ns min. interval to activate 4 banks */ 
        0u,          /* Dynamic: trcd time in ns min. delay active to rd/wr commands trcd */
        0u,          /* Dynamic: trfc time in ns min. refresh->act delay */    
        0u,          /* Dynamic: tmod time in ns MRS command to ODT update delay */
        0u,          /* Dynamic: tckemin time in clock cycles cke power-down timing */   
        0u,          /* Dynamic: trasmin time in ns min. delay active to precharge commands tras_min */
        0u,          /* Dynamic: CasLatency time delay read command and availability of first data */
        0u,          /* Dynamic: time in ns between consecutive refresh cycles */
        0u,          /* Dynamic: Number of auto-refreshes during init */   
        0u,          /* Dynamic: Number of ns to SDRAM inputs stable after power up */                                              
        0u,          /* Static: Page size in words (0 = no page mode support) */
        0u,          /* Static: tprc time in ns page mode read cycle */
        30u,         /* Static: tbta time in ns memory idle cycles rd2wr or wr2rd or rd2rd */
        40u,         /* Static: twp  time in ns write pulse width*/
        0u,          /* Static: twr time in ns write address/data hold time */
        0u,          /* Static: tas time in ns write address setup time */
        50u          /* Static: trc time in ns read cycle time */
                               
     },
    {
        "S29GL256N11",    /* Spansion MirrorBit Flash 256Mb */
        16u,          /* Data width in bits */
        2u,           /* Memory type: 0 = SDR-SDRAM (dyn.), 1 = SRAM (static), 2 = Flash (static), 3 = DDR1-SDRAM (dyn.) */
        32768u,       /* Size of memory device in KB */
        0u,           /* Dynamic: width of column address */
        0u,           /* Dynamic: width of row address */
        0u,           /* Dynamic: width of bank address */
        0u,           /* Dynamic: trc time in ns active to active command trc */                            
        0u,           /* Dynamic: txsr time in ns exit self refresh to active command txsr */               
        0u,           /* Dynamic: trcar time in ns auto refresh period trcar */                             
        0u,           /* Dynamic: twr time in ns data to precharge twr */                                   
        0u,           /* Dynamic: trp time in ns precharge period */                                        
        0u,          /* Dynamic: trpa time in ns precharge all */
        0u,          /* Dynamic: trtp time in ns read->precharge */      
        0u,          /* Dynamic: trrd time in ns min. interval between active commands */
        0u,          /* Dynamic: tfaw time in ns min. interval to activate 4 banks */ 
        0u,           /* Dynamic: trcd time in ns min. delay active to rd/wr commands trcd */               
        0u,          /* Dynamic: trfc time in ns min. refresh->act delay */    
        0u,          /* Dynamic: tmod time in ns MRS command to ODT update delay */
        0u,          /* Dynamic: tckemin time in clock cycles cke power-down timing */   
        0u,           /* Dynamic: trasmin time in ns min. delay active to precharge commands tras_min */    
        0u,           /* Dynamic: CasLatency time delay read command and availability of first data */
        0u,           /* Dynamic: time in ns between consecutive refresh cycles */                          
        0u,           /* Dynamic: Number of auto-refreshes during init */   
        0u,           /* Dynamic: Number of ns to SDRAM inputs stable after power up */                                              
        8u,           /* Static: Page size in words (0 = no page mode support) */                           
        25u,          /* Static: tprc time in ns page mode read cycle */                                    
        20u,          /* Static: tbta time in ns memory idle cycles rd2wr or wr2rd or rd2rd */              
        110u,         /* Static: twp  time in ns write pulse width*/                                        
        45u,          /* Static: twr time in ns write address/data hold time */                             
        0u,           /* Static: tas time in ns write address setup time */                                 
        110u          /* Static: trc time in ns read cycle time */                                          

     },
     {
        "MT47H128M16RT-25E",    /* Micron 128Mx16 DDR2 SDRAM for D1M2H Mango Board  */
        16u,          /* Data width in bits */
        4u,           /* Memory type: 0 = SDR-SDRAM (dyn.), 1 = SRAM (static), 2 = Flash (static), 3 = DDR1-SDRAM (dyn.) */
        262144u,      /* Size of memory device in KB */
        10u,          /* Dynamic: width of column address */
        14u,          /* Dynamic: width of row address */
        3u,           /* Dynamic: width of bank address */
        58u,          /* Dynamic: trc time in ns active to active command trc */                            
        75u,          /* Dynamic: txsr time in ns exit self refresh to active command txsr */               
        0u,           /* Dynamic: trcar time in ns auto refresh period trcar */                             
        15u,          /* Dynamic: twr time in ns data to precharge twr */                                   
        13u,          /* Dynamic: trp time in ns precharge period */                                        
        13u,          /* Dynamic: trpa time in ns precharge all */
        8u,           /* Dynamic: trtp time in ns read->precharge */      
        10u,          /* Dynamic: trrd time in ns min. interval between active commands */
        45u,          /* Dynamic: tfaw time in ns min. interval to activate 4 banks */ 
        13u,          /* Dynamic: trcd time in ns min. delay active to rd/wr commands trcd */               
        75u,          /* Dynamic: trfc time in ns min. refresh->act delay */    
        12u,          /* Dynamic: tmod time in ns MRS command to ODT update delay */
        3u,           /* Dynamic: tckemin time in clock cycles cke power-down timing */   
        45u,          /* Dynamic: trasmin time in ns min. delay active to precharge commands tras_min */    
        5u,           /* Dynamic: CasLatency time delay read command and availability of first data */
        3900u,        /* Dynamic: time in ns between consecutive refresh cycles */ 
        4u,           /* Dynamic: Number of auto-refreshes during init */ 
        400000u,      /* Dynamic: Number of ns to SDRAM inputs stable after power up */                        
        0u,           /* Static: Page size in words (0 = no page mode support) */                           
        0u,           /* Static: tprc time in ns page mode read cycle */                                    
        0u,           /* Static: tbta time in ns memory idle cycles rd2wr or wr2rd or rd2rd */              
        0u,           /* Static: twp  time in ns write pulse width*/                                        
        0u,           /* Static: twr time in ns write address/data hold time */                             
        0u,           /* Static: tas time in ns write address setup time */                                 
        0u            /* Static: trc time in ns read cycle time */                                          
    }, 
{
        "SCIT-RAM-GFX-2_SDRB",    /* SCIT-RAM-GFX-2 Board using D1X SDRB Memory Controller */
        16u,          /* Data width in bits */
        4u,           /* Memory type: 0 = SDR-SDRAM (dyn.), 1 = SRAM (static), 2 = Flash (static), 3 = DDR1-SDRAM (dyn.) */
        65536u,       /* Size of memory device in KB */
        10u,          /* Dynamic: width of column address */
        13u,          /* Dynamic: width of row address */
        2u,           /* Dynamic: width of bank address */
        58u,          /* Dynamic: trc time in ns active to active command trc */                            
        75u,          /* Dynamic: txsr time in ns exit self refresh to active command txsr */               
        0u,           /* Dynamic: trcar time in ns auto refresh period trcar */                             
        15u,          /* Dynamic: twr time in ns data to precharge twr */                                   
        13u,          /* Dynamic: trp time in ns precharge period */                                        
        13u,          /* Dynamic: trpa time in ns precharge all */
        8u,           /* Dynamic: trtp time in ns read->precharge */      
        10u,          /* Dynamic: trrd time in ns min. interval between active commands */
        45u,          /* Dynamic: tfaw time in ns min. interval to activate 4 banks */ 
        13u,          /* Dynamic: trcd time in ns min. delay active to rd/wr commands trcd */               
        75u,          /* Dynamic: trfc time in ns min. refresh->act delay */    
        12u,          /* Dynamic: tmod time in ns MRS command to ODT update delay */
        3u,           /* Dynamic: tckemin time in clock cycles cke power-down timing */   
        45u,          /* Dynamic: trasmin time in ns min. delay active to precharge commands tras_min */    
        3u,           /* Dynamic: CasLatency time delay read command and availability of first data */
        1300u,        /* Dynamic: time in ns between consecutive refresh cycles */ 
        4u,           /* Dynamic: Number of auto-refreshes during init */ 
        400000u,      /* Dynamic: Number of ns to SDRAM inputs stable after power up */                        
        0u,           /* Static: Page size in words (0 = no page mode support) */                           
        0u,           /* Static: tprc time in ns page mode read cycle */                                    
        0u,           /* Static: tbta time in ns memory idle cycles rd2wr or wr2rd or rd2rd */              
        0u,           /* Static: twp  time in ns write pulse width*/                                        
        0u,           /* Static: twr time in ns write address/data hold time */                             
        0u,           /* Static: tas time in ns write address setup time */                                 
        0u            /* Static: trc time in ns read cycle time */                                          
    }, 
    {
        "EDS1216AGTA75",    /* Elpida SDRAM 128Mb (8Mx16)*/
        16u,          /* Data width in bits */
        0u,           /* Memory type: 0 = SDR-SDRAM (dyn.), 1 = SRAM (static), 2 = Flash (static), 3 = DDR1-SDRAM (dyn.) */
        16384u,       /* Size of memory device in KB */
        9u,           /* Dynamic: width of column address */
        12u,          /* Dynamic: width of row address */
        2u,           /* Dynamic: width of bank address */
        68u,          /* Dynamic: trc time in ns active to active command trc */                            
        10u,          /* Dynamic: txsr time in ns exit self refresh to active command txsr */               
        68u,          /* Dynamic: trcar time in ns auto refresh period trcar */                             
        15u,          /* Dynamic: twr time in ns data to precharge twr */                                   
        20u,          /* Dynamic: trp time in ns precharge period */                                        
        0u,          /* Dynamic: trpa time in ns precharge all */
        0u,          /* Dynamic: trtp time in ns read->precharge */      
        0u,          /* Dynamic: trrd time in ns min. interval between active commands */
        0u,          /* Dynamic: tfaw time in ns min. interval to activate 4 banks */ 
        20u,          /* Dynamic: trcd time in ns min. delay active to rd/wr commands trcd */               
        0u,          /* Dynamic: trfc time in ns min. refresh->act delay */    
        0u,          /* Dynamic: tmod time in ns MRS command to ODT update delay */
        0u,          /* Dynamic: tckemin time in clock cycles cke power-down timing */   
        45u,          /* Dynamic: trasmin time in ns min. delay active to precharge commands tras_min */    
        2u,           /* Dynamic: CasLatency time delay read command and availability of first data */
        15625u,       /* Dynamic: time in ns between consecutive refresh cycles */ 
        8u,           /* Dynamic: Number of auto-refreshes during init */ 
        300000u,      /* Dynamic: Number of ns to SDRAM inputs stable after power up */                        
        0u,           /* Static: Page size in words (0 = no page mode support) */                           
        0u,           /* Static: tprc time in ns page mode read cycle */                                    
        0u,           /* Static: tbta time in ns memory idle cycles rd2wr or wr2rd or rd2rd */              
        0u,           /* Static: twp  time in ns write pulse width*/                                        
        0u,           /* Static: twr time in ns write address/data hold time */                             
        0u,           /* Static: tas time in ns write address setup time */                                 
        0u            /* Static: trc time in ns read cycle time */                                          
    },
    {
        "MT48LC16M16A2-7E",    /* Micron SDRAM 256Mb (4Mx16x4)*/
        16u,          /* Data width in bits */
        0u,           /* Memory type: 0 = SDR-SDRAM (dyn.), 1 = SRAM (static), 2 = Flash (static), 3 = DDR1-SDRAM (dyn.) */
        32768u,       /* Size of memory device in KB */
        9u,           /* Dynamic: width of column address */
        13u,          /* Dynamic: width of row address */
        2u,           /* Dynamic: width of bank address */
        60u,          /* Dynamic: trc time in ns active to active command trc */                            
        67u,          /* Dynamic: txsr time in ns exit self refresh to active command txsr */               
        66u,          /* Dynamic: trcar time in ns auto refresh period trcar */                             
        14u,          /* Dynamic: twr time in ns data to precharge twr */                                   
        15u,          /* Dynamic: trp time in ns precharge period */                                        
        0u,          /* Dynamic: trpa time in ns precharge all */
        0u,          /* Dynamic: trtp time in ns read->precharge */      
        0u,          /* Dynamic: trrd time in ns min. interval between active commands */
        0u,          /* Dynamic: tfaw time in ns min. interval to activate 4 banks */ 
        15u,          /* Dynamic: trcd time in ns min. delay active to rd/wr commands trcd */               
        0u,          /* Dynamic: trfc time in ns min. refresh->act delay */    
        0u,          /* Dynamic: tmod time in ns MRS command to ODT update delay */
        0u,          /* Dynamic: tckemin time in clock cycles cke power-down timing */   
        37u,          /* Dynamic: trasmin time in ns min. delay active to precharge commands tras_min */    
        2u,           /* Dynamic: CasLatency time delay read command and availability of first data */
        7812u,        /* Dynamic: time in ns between consecutive refresh cycles */ 
        2u,           /* Dynamic: Number of auto-refreshes during init */ 
        100000u,      /* Dynamic: Number of ns to SDRAM inputs stable after power up */                        
        0u,           /* Static: Page size in words (0 = no page mode support) */                           
        0u,           /* Static: tprc time in ns page mode read cycle */                                    
        0u,           /* Static: tbta time in ns memory idle cycles rd2wr or wr2rd or rd2rd */              
        0u,           /* Static: twp  time in ns write pulse width*/                                        
        0u,           /* Static: twr time in ns write address/data hold time */                             
        0u,           /* Static: tas time in ns write address setup time */                                 
        0u            /* Static: trc time in ns read cycle time */                                          
    },
    {
        "IS42R16100E",    /* ISSI SDRAM used on SCIT */
        16u,          /* Data width in bits */
        0u,           /* Memory type: 0 = SDR-SDRAM (dyn.), 1 = SRAM (static), 2 = Flash (static), 3 = DDR1-SDRAM (dyn.) */
        2048u,        /* Size of memory device in KB */
        8u,           /* Dynamic: width of column address */
        11u,          /* Dynamic: width of row address */
        1u,           /* Dynamic: width of bank address */
        70u,          /* Dynamic: trc time in ns active to active command trc */                            
        70u, /* =trc     Dynamic: txsr time in ns exit self refresh to active command txsr */               
        70u,          /* Dynamic: trcar time in ns auto refresh period trcar */                             
        (2u*7u), /* 2xClk)Dynamic: twr time in ns data to precharge twr */                                   
        20u,          /* Dynamic: trp time in ns precharge period */                                        
        0u,          /* Dynamic: trpa time in ns precharge all */
        0u,          /* Dynamic: trtp time in ns read->precharge */      
        0u,          /* Dynamic: trrd time in ns min. interval between active commands */
        0u,          /* Dynamic: tfaw time in ns min. interval to activate 4 banks */ 
        20u,          /* Dynamic: trcd time in ns min. delay active to rd/wr commands trcd */               
        0u,          /* Dynamic: trfc time in ns min. refresh->act delay */    
        0u,          /* Dynamic: tmod time in ns MRS command to ODT update delay */
        0u,          /* Dynamic: tckemin time in clock cycles cke power-down timing */   
        42u,          /* Dynamic: trasmin time in ns min. delay active to precharge commands tras_min */    
        3u,           /* Dynamic: CasLatency time delay read command and availability of first data */
        (64000000u/2048u),    /* Dynamic: time in ns between consecutive refresh cycles */ 
        2u,           /* Dynamic: Number of auto-refreshes during init */ 
        100000u,      /* Dynamic: Number of ns to SDRAM inputs stable after power up */                        
        0u,           /* Static: Page size in words (0 = no page mode support) */                           
        0u,           /* Static: tprc time in ns page mode read cycle */                                    
        0u,           /* Static: tbta time in ns memory idle cycles rd2wr or wr2rd or rd2rd */              
        0u,           /* Static: twp  time in ns write pulse width*/                                        
        0u,           /* Static: twr time in ns write address/data hold time */                             
        0u,           /* Static: tas time in ns write address setup time */                                 
        0u            /* Static: trc time in ns read cycle time */                                          
    },
    {
        "IS42R16100E_DOUBLE",    /* ISSI SDRAM used on SCIT; this setting is a workaround
                                    for Synopsis Memc. The settings pretends to be one device
                                    with 4MB, but in fact is two devices with 2MB each.
                                    changes compared to "IS42R16100E":
                                    -> Size of memory device doubled
                                    -> row address width increased by 1 (doubles address space)
                                     */
        16u,          /* Data width in bits */
        0u,           /* Memory type: 0 = SDR-SDRAM (dyn.), 1 = SRAM (static), 2 = Flash (static), 3 = DDR1-SDRAM (dyn.) */
        (2u*2048u),    /* Size of memory device in KB */
        8u,           /* Dynamic: width of column address */
        (11u+1u),      /* Dynamic: width of row address */
        1u,           /* Dynamic: width of bank address */
        70u,          /* Dynamic: trc time in ns active to active command trc */                            
        70u, /* =trc     Dynamic: txsr time in ns exit self refresh to active command txsr */               
        70u,          /* Dynamic: trcar time in ns auto refresh period trcar */                             
        (2u*7u), /* 2xClk)Dynamic: twr time in ns data to precharge twr */                                   
        20u,          /* Dynamic: trp time in ns precharge period */                                        
        0u,          /* Dynamic: trpa time in ns precharge all */
        0u,          /* Dynamic: trtp time in ns read->precharge */      
        0u,          /* Dynamic: trrd time in ns min. interval between active commands */
        0u,          /* Dynamic: tfaw time in ns min. interval to activate 4 banks */ 
        20u,          /* Dynamic: trcd time in ns min. delay active to rd/wr commands trcd */               
        0u,          /* Dynamic: trfc time in ns min. refresh->act delay */    
        0u,          /* Dynamic: tmod time in ns MRS command to ODT update delay */
        0u,          /* Dynamic: tckemin time in clock cycles cke power-down timing */   
        42u,          /* Dynamic: trasmin time in ns min. delay active to precharge commands tras_min */    
        3u,           /* Dynamic: CasLatency time delay read command and availability of first data */
        (64000000/2048),    /* Dynamic: time in ns between consecutive refresh cycles */ 
        2u,           /* Dynamic: Number of auto-refreshes during init */ 
        100000u,      /* Dynamic: Number of ns to SDRAM inputs stable after power up */                        
        0u,           /* Static: Page size in words (0 = no page mode support) */                           
        0u,           /* Static: tprc time in ns page mode read cycle */                                    
        0u,           /* Static: tbta time in ns memory idle cycles rd2wr or wr2rd or rd2rd */              
        0u,           /* Static: twp  time in ns write pulse width*/                                        
        0u,           /* Static: twr time in ns write address/data hold time */                             
        0u,           /* Static: tas time in ns write address setup time */                                 
        0u            /* Static: trc time in ns read cycle time */                                          
    },
    {
        "H5DU5162ETR-E3",   /* Hynix DDR1-SDRAM used on SCIT-MEM-GFX-2 board
            As used in D1x SCIT bitstreams w/ the SDRA memory controller (DBSC2).
            The H5DU5162ETR-E3 is speed grade E3. It supports 200MHz@CL3u,
            166MHz@CL2.5u, 133MHz@CL2 that means the timings (CL-tRCD-tRP)
            for DDR400 (3-3-3), DDR333 (2.5-3-3), DDR266A (2-3-3)
            or DDR266B (2.5-3-3).
            The timing set below is intended for 120 MHz operation made
            up from datasheet spec
            (M:\Auto\07_Info_Pool\KnowHow_Pool\SDRAM\Hynix_Hyundai\Hynix_DDR1_512Mbit_H5DU5162ETR-E3C.pdf)
            according to 133MHz@CL2; DDR266A (133Mhz, CL=2):
            -> tCK = 7.5ns, CL=2u, BL=4u, tRCD = 3*tCK = 22.5ns,
               tRC = 9*tCK = 67.5ns, tRAS = 5*tCK = 37.5ns */
        16u,         /* Data width in bits */
         3u,           /* Memory type: 0 = SDR-SDRAM (dyn.), 1 = SRAM (static), 2 = Flash (static), 3 = DDR1-SDRAM (dyn.) */
        ((512u/8u)*1024u),   /* Size of memory device in KB */
        10u,         /* Dynamic: width of column address */
        13u,         /* Dynamic: width of row address */
        2u,          /* Dynamic: width of bank address */
        68u,         /* Dynamic: trc time in ns active to active command trc */
        200u,        /* Dynamic: txsr time in ns exit self refresh to active command txsr */
        75u,         /* Dynamic: trcar time in ns auto refresh period trcar */
        15u,         /* Dynamic: twr time in ns data to precharge twr */
        20u,         /* Dynamic: trp time in ns precharge period */
        0u,          /* Dynamic: trpa time in ns precharge all */
        0u,          /* Dynamic: trtp time in ns read->precharge */      
        0u,          /* Dynamic: trrd time in ns min. interval between active commands */
        0u,          /* Dynamic: tfaw time in ns min. interval to activate 4 banks */ 
        23u,         /* Dynamic: trcd time in ns min. delay active to rd/wr commands trcd */
        0u,          /* Dynamic: trfc time in ns min. refresh->act delay */    
        0u,          /* Dynamic: tmod time in ns MRS command to ODT update delay */
        0u,          /* Dynamic: tckemin time in clock cycles cke power-down timing */   
        45u,         /* Dynamic: trasmin time in ns min. delay active to precharge commands tras_min */
        2u,          /* Dynamic: CasLatency time delay read command and availability of first data */
        7800u,       /* Dynamic: time in ns between consecutive refresh cycles */
        2u,          /* Dynamic: Number of auto-refreshes during init */
        200000u,      /* Dynamic: Number of ns to SDRAM inputs stable after power up */
        0u,           /* Static: Page size in words (0 = no page mode support) */
        0u,           /* Static: tprc time in ns page mode read cycle */
        0u,           /* Static: tbta time in ns memory idle cycles rd2wr or wr2rd or rd2rd */
        0u,           /* Static: twp  time in ns write pulse width*/
        0u,           /* Static: twr time in ns write address/data hold time */
        0u,           /* Static: tas time in ns write address setup time */
        0u            /* Static: trc time in ns read cycle time */
    },
    {
        "IS45S16400F-7TLA1",    /* ISSI 16 bit SDRAM used on Delphi Board */
        16u,          /* Data width in bits */
        0u,           /* Memory type: 0 = SDR-SDRAM (dyn.), 1 = SRAM (static), 2 = Flash (static), 3 = DDR1-SDRAM (dyn.) */
        8192u,        /* Size of memory device in KB */
        8u,           /* Dynamic: width of column address */
        12u,          /* Dynamic: width of row address */
        2u,           /* Dynamic: width of bank address */
        63u,          /* Dynamic: trc time in ns active to active command trc */                            
        63u, /* =trc     Dynamic: txsr time in ns exit self refresh to active command txsr */ 
        63u,          /* Dynamic: trcar time in ns auto refresh period trcar */                             
        15u,          /* 2xClk)Dynamic: twr time in ns data to precharge twr */                                   
        20u,          /* Dynamic: trp time in ns precharge period */                                        
        0u,          /* Dynamic: trpa time in ns precharge all */
        0u,          /* Dynamic: trtp time in ns read->precharge */      
        0u,          /* Dynamic: trrd time in ns min. interval between active commands */
        0u,          /* Dynamic: tfaw time in ns min. interval to activate 4 banks */ 
        20u,          /* Dynamic: trcd time in ns min. delay active to rd/wr commands trcd */               
        0u,          /* Dynamic: trfc time in ns min. refresh->act delay */    
        0u,          /* Dynamic: tmod time in ns MRS command to ODT update delay */
        0u,          /* Dynamic: tckemin time in clock cycles cke power-down timing */   
        42u,          /* Dynamic: trasmin time in ns min. delay active to precharge commands tras_min */    
        2u,           /* Dynamic: CasLatency time delay read command and availability of first data */
        (64000000u/4096u),    /* Dynamic: time in ns between consecutive refresh cycles */ 
        2u,           /* Dynamic: Number of auto-refreshes during init */ 
        100000u,      /* Dynamic: Number of ns to SDRAM inputs stable after power up */                        
        0u,           /* Static: Page size in words (0 = no page mode support) */                           
        0u,           /* Static: tprc time in ns page mode read cycle */                                    
        0u,           /* Static: tbta time in ns memory idle cycles rd2wr or wr2rd or rd2rd */              
        0u,           /* Static: twp  time in ns write pulse width*/                                        
        0u,           /* Static: twr time in ns write address/data hold time */                             
        0u,           /* Static: tas time in ns write address setup time */                                 
        0u            /* Static: trc time in ns read cycle time */                                          
    },    
    {
        "IS45S32200E-7TLA1",    /* ISSI 32 bit SDRAM used on Delphi Board */
        32u,          /* Data width in bits */
        0u,           /* Memory type: 0 = SDR-SDRAM (dyn.), 1 = SRAM (static), 2 = Flash (static), 3 = DDR1-SDRAM (dyn.) */
        8192u,        /* Size of memory device in KB */
        8u,           /* Dynamic: width of column address */
        11u,          /* Dynamic: width of row address */
        2u,           /* Dynamic: width of bank address */
        70u,          /* Dynamic: trc time in ns active to active command trc */                            
        77u,          /* Dynamic: txsr time in ns exit self refresh to active command txsr */ 
        70u,          /* Dynamic: trcar time in ns auto refresh period trcar */                             
        20u,          /* 2xClk)Dynamic: twr time in ns data to precharge twr */                                   
        20u,          /* Dynamic: trp time in ns precharge period */                                        
        0u,          /* Dynamic: trpa time in ns precharge all */
        0u,          /* Dynamic: trtp time in ns read->precharge */      
        0u,          /* Dynamic: trrd time in ns min. interval between active commands */
        0u,          /* Dynamic: tfaw time in ns min. interval to activate 4 banks */ 
        20u,          /* Dynamic: trcd time in ns min. delay active to rd/wr commands trcd */               
        0u,          /* Dynamic: trfc time in ns min. refresh->act delay */    
        0u,          /* Dynamic: tmod time in ns MRS command to ODT update delay */
        0u,          /* Dynamic: tckemin time in clock cycles cke power-down timing */   
        42u,          /* Dynamic: trasmin time in ns min. delay active to precharge commands tras_min */    
        2u,           /* Dynamic: CasLatency time delay read command and availability of first data */
        (64000000u/4096u),    /* Dynamic: time in ns between consecutive refresh cycles */ 
        2u,           /* Dynamic: Number of auto-refreshes during init */ 
        100000u,      /* Dynamic: Number of ns to SDRAM inputs stable after power up */                        
        0u,           /* Static: Page size in words (0 = no page mode support) */                           
        0u,           /* Static: tprc time in ns page mode read cycle */                                    
        0u,           /* Static: tbta time in ns memory idle cycles rd2wr or wr2rd or rd2rd */              
        0u,           /* Static: twp  time in ns write pulse width*/                                        
        0u,           /* Static: twr time in ns write address/data hold time */                             
        0u,           /* Static: tas time in ns write address setup time */                                 
        0u            /* Static: trc time in ns read cycle time */                                          
    },    
    {
            "MT48LC16M16A2P-75IT",    /* Micron SDRAM 256Mb (4Mx16x4)used with Umbrella Dx4 Piggy  */
        16u,          /* Data width in bits */
        0u,           /* Memory type: 0 = SDR-SDRAM (dyn.), 1 = SRAM (static), 2 = Flash (static), 3 = DDR1-SDRAM (dyn.) */
        32768u,       /* Size of memory device in KB */
        9u,           /* Dynamic: width of column address */
        13u,          /* Dynamic: width of row address */
        2u,           /* Dynamic: width of bank address */
        66u,          /* Dynamic: trc time in ns active to active command trc */                            
        75u,          /* Dynamic: txsr time in ns exit self refresh to active command txsr */               
        66u,          /* Dynamic: trcar time in ns auto refresh period trcar */                             
        15u,          /* Dynamic: twr time in ns data to precharge twr */                                   
        20u,          /* Dynamic: trp time in ns precharge period */                                        
        0u,          /* Dynamic: trpa time in ns precharge all */
        0u,          /* Dynamic: trtp time in ns read->precharge */      
        0u,          /* Dynamic: trrd time in ns min. interval between active commands */
        0u,          /* Dynamic: tfaw time in ns min. interval to activate 4 banks */ 
        20u,          /* Dynamic: trcd time in ns min. delay active to rd/wr commands trcd */               
        0u,          /* Dynamic: trfc time in ns min. refresh->act delay */    
        0u,          /* Dynamic: tmod time in ns MRS command to ODT update delay */
        0u,          /* Dynamic: tckemin time in clock cycles cke power-down timing */   
        44u,          /* Dynamic: trasmin time in ns min. delay active to precharge commands tras_min */    
        3u,           /* Dynamic: CasLatency time delay read command and availability of first data */
        (64000000u/8192u),    /* Dynamic: time in ns between consecutive refresh cycles */ 
        2u,           /* Dynamic: Number of auto-refreshes during init */ 
        100000u,      /* Dynamic: Number of ns to SDRAM inputs stable after power up */                        
        0u,           /* Static: Page size in words (0 = no page mode support) */                           
        0u,           /* Static: tprc time in ns page mode read cycle */                                    
        0u,           /* Static: tbta time in ns memory idle cycles rd2wr or wr2rd or rd2rd */              
        0u,           /* Static: twp  time in ns write pulse width*/                                        
        0u,           /* Static: twr time in ns write address/data hold time */                             
        0u,           /* Static: tas time in ns write address setup time */                                 
        0u            /* Static: trc time in ns read cycle time */                                          
    },
    {
            "MT48LC4M32B2B5-6IT",    /* Micron SDRAM 128Mb (1Mx32x4)used with Umbrella Dx4   */
        32u,          /* Data width in bits */
        0u,           /* Memory type: 0 = SDR-SDRAM (dyn.), 1 = SRAM (static), 2 = Flash (static), 3 = DDR1-SDRAM (dyn.) */
        16384u,          /* Size of memory device in KB */
        8u,           /* Dynamic: width of column adddress */
        12u,          /* Dynamic: width of row adddress */
        2u,           /* Dynamic: width of bank adddress */
        60u,          /* Dynamic: trc time in ns active to active command trc */                            
        70u,          /* Dynamic: txsr time in ns exit self refresh to active command txsr */               
        60u,          /* Dynamic: trcar time in ns auto refresh period trcar */                             
        12u,          /* Dynamic: twr time in ns data to precharge twr */                                   
        18u,          /* Dynamic: trp time in ns precharge period */                                        
        0u,          /* Dynamic: trpa time in ns precharge all */
        0u,          /* Dynamic: trtp time in ns read->precharge */      
        0u,          /* Dynamic: trrd time in ns min. interval between active commands */
        0u,          /* Dynamic: tfaw time in ns min. interval to activate 4 banks */ 
        18u,          /* Dynamic: trcd time in ns min. delay active to rd/wr commands trcd */               
        0u,          /* Dynamic: trfc time in ns min. refresh->act delay */    
        0u,          /* Dynamic: tmod time in ns MRS command to ODT update delay */
        0u,          /* Dynamic: tckemin time in clock cycles cke power-down timing */   
        42u,          /* Dynamic: trasmin time in ns min. delay active to precharge commands tras_min */    
        3u,           /* Dynamic: CasLatency time delay read command and availability of first data */
        (64000000u/4096u),    /* Dynamic: time in ns between consecutive refresh cycles */ 
        2u,           /* Dynamic: Number of auto-refreshes during init */ 
        100000u,      /* Dynamic: Number of ns to SDRAM inputs stable after power up */                        
        0u,           /* Static: Page size in words (0 = no page mode support) */                           
        0u,           /* Static: tprc time in ns page mode read cycle */                                    
        0u,           /* Static: tbta time in ns memory idle cycles rd2wr or wr2rd or rd2rd */              
        0u,           /* Static: twp  time in ns write pulse width*/                                        
        0u,           /* Static: twr time in ns write address/data hold time */                             
        0u,           /* Static: tas time in ns write address setup time */                                 
        0u            /* Static: trc time in ns read cycle time */                                          
    },
    {
            "LEHUA",    /* LEHUA eDRAM in DP4-H */
        32u,          /* Data width in bits */
        0u,           /* Memory type: 0 = SDR-SDRAM (dyn.), 1 = SRAM (static), 2 = Flash (static), 3 = DDR1-SDRAM (dyn.) */
        8u*1024u,      /* Size of memory device in KB */
        6u,           /* Dynamic: width of column address */
        13u,          /* Dynamic: width of row address */
        2u,           /* Dynamic: width of bank address */
        30u,          /* Dynamic: trc time in ns active to active command trc */                            
        30u,          /* Dynamic: txsr time in ns exit self refresh to active command txsr */               
        30u,          /* Dynamic: trcar time in ns auto refresh period trcar */                             
        15u,          /* Dynamic: twr time in ns data to precharge twr */                                   
        15u,          /* Dynamic: trp time in ns precharge period */                                        
        0u,          /* Dynamic: trpa time in ns precharge all */
        0u,          /* Dynamic: trtp time in ns read->precharge */      
        0u,          /* Dynamic: trrd time in ns min. interval between active commands */
        0u,          /* Dynamic: tfaw time in ns min. interval to activate 4 banks */ 
        15u,          /* Dynamic: trcd time in ns min. delay active to rd/wr commands trcd */               
        0u,          /* Dynamic: trfc time in ns min. refresh->act delay */    
        0u,          /* Dynamic: tmod time in ns MRS command to ODT update delay */
        0u,          /* Dynamic: tckemin time in clock cycles cke power-down timing */   
        15u,          /* Dynamic: trasmin time in ns min. delay active to precharge commands tras_min */    
        3u,           /* Dynamic: CasLatency time delay read command and availability of first data */
        (2000000/4096),    /* Dynamic: time in ns between consecutive refresh cycles */ 
        2u,           /* Dynamic: Number of auto-refreshes during init */ 
        100000u,      /* Dynamic: Number of ns to SDRAM inputs stable after power up */                        
        0u,           /* Static: Page size in words (0 = no page mode support) */                           
        0u,           /* Static: tprc time in ns page mode read cycle */                                    
        0u,           /* Static: tbta time in ns memory idle cycles rd2wr or wr2rd or rd2rd */              
        0u,           /* Static: twp  time in ns write pulse width*/                                        
        0u,           /* Static: twr time in ns write address/data hold time */                             
        0u,           /* Static: tas time in ns write address setup time */                                 
        0u            /* Static: trc time in ns read cycle time */                                          
    }
};
/* end of loc_MemoryMode[] */


/*******************************************************************************
  Section: Local Functions
*/

/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Section: Global API Functions
  
  For API documentation see r_memdb_api.h
*/

/*******************************************************************************
  Function: R_MEMDB_GetTiming
*/

r_memdb_Timing_t *R_MEMDB_GetTiming(const char* Name) {
    uint32_t i, x;

    for(i = 0u; i < LOC_MEMORY_NUM; i++)
    {
        x = strcmp(loc_MemoryMode[i].Name, Name);
        if (x == 0u)
        {
            return (r_memdb_Timing_t*) &loc_MemoryMode[i];
        }
    }
    return 0;
}

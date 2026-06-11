/*
****************************************************************************
PROJECT : MEMDB
FILE    : $Id: r_memdb_api.h 3935 2014-09-23 08:52:32Z golczewskim $
============================================================================ 
DESCRIPTION
MEMDB memory database
============================================================================
                            C O P Y R I G H T                            
============================================================================
                           Copyright (c) 2013
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

#ifndef MEMDB_API_H_
#define MEMDB_API_H_

#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
  Title: MEMDB API

  An application using the memory database (MEMDB) should include this header file.
*/

/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Type: r_memdb_Timing_t
  
  Memory database timing structure.

  The structure describing the memory and its configuration.
  
  Members:
  *Name;              - Memory device name 
  DataWidth;          - Data width in bits 
  Type;               - Memory type: 0 = SDR-SDRAM (dyn.), 1 = SRAM (static), 2 = Flash (static), 3 = DDR1-SDRAM (dyn.)
  MemSize;            - Size of memory device in KB
  D_ColAddrWidth;     - Dynamic: width of column adddress
  D_RowAddrWidth;     - Dynamic: width of row adddress 
  D_BankAddrWidth;    - Dynamic: width of bank adddress
  D_trc;              - Dynamic: time in ns active to active command trc
  D_txsr;             - Dynamic: time in ns exit self refresh to active command txsr
  D_trcar;            - Dynamic: time in ns auto refresh period trcar 
  D_twr;              - Dynamic: time in ns data to precharge twr 
  D_trp;              - Dynamic: time in ns precharge period 
  D_trcd;             - Dynamic: time in ns min. delay active to rd/wr commands trcd
  D_trasmin;          - Dynamic: time in ns min. delay active to precharge commands tras_min
  D_CasLatency;       - Dynamic: time delay read command and availability of first data 
  D_RefreshCycle;     - Dynamic: time in ns between consecutive refresh cycles 
  D_NumInitRef;       - Dynamic: Number of auto-refreshes during init 
  D_tinit;            - Dynamic: Number of ns to SDRAM inputs stable after power up
  S_PageSize;         - Static: Page size in words (0 = no page mode support) 
  S_tprc;             - Static: time in ns page mode read cycle 
  S_tbta;             - Static: time in ns memory idle cycles rd2wr or wr2rd or rd2rd 
  S_twp;              - Static: time in ns write pulse width
  S_twr;              - Static: time in ns write address/data hold time 
  S_tas;              - Static: time in ns write address setup time 
  S_trc;              - Static: time in ns read cycle time 

*/

typedef struct {
    /* Dynamic = DDR1-SDRAM, SDR-SDRAM; Static = Flash, SRAM, ROM */
    const char      *Name;              /* Memory device name */
    uint8_t         DataWidth;          /* Data width in bits */
    uint8_t         Type;               /* Memory type: 0 = SDR-SDRAM (dyn.), 1 = SRAM (static), 2 = Flash (static), 3 = DDR1-SDRAM (dyn.), 4 = DDR2-SDRAM (dyn.) */
    uint32_t        MemSize;            /* Size of memory device in KB */
    uint8_t         D_ColAddrWidth;     /* Dynamic: width of column adddress */
    uint8_t         D_RowAddrWidth;     /* Dynamic: width of row adddress */
    uint8_t         D_BankAddrWidth;    /* Dynamic: width of bank adddress */
    uint8_t         D_trc;              /* Dynamic: time in ns active to active command trc */
    uint8_t         D_txsr;             /* Dynamic: time in ns exit self refresh to active command txsr */
    uint8_t         D_trcar;            /* Dynamic: time in ns auto refresh period trcar */
    uint8_t         D_twr;              /* Dynamic: time in ns data to precharge twr */
    uint8_t         D_trp;              /* Dynamic: time in ns precharge */
    uint8_t         D_trpa;             /* Dynamic: time in ns precharge all */
    uint8_t         D_trtp;             /* Dynamic: time in ns read->precharge */      
    uint8_t         D_trrd;             /* Dynamic: time in ns min. interval between active commands */
    uint8_t         D_tfaw;             /* Dynamic: time in ns min. interval to activate 4 banks */
    uint8_t         D_trcd;             /* Dynamic: time in ns min. delay active to rd/wr commands trcd */
    uint8_t         D_trfc;             /* Dynamic: time in ns min. refresh->act delay */    
    uint8_t         D_tmod;             /* Dynamic: time in ns MRS command to ODT update delay */
    uint8_t         D_tckemin;          /* Dynamic: time in clock cycles cke power-down timing */
    uint8_t         D_trasmin;          /* Dynamic: time in ns min. delay active to precharge commands tras_min */
    uint8_t         D_CasLatency;       /* Dynamic: time delay read command and availability of first data */
    uint32_t        D_RefreshCycle;     /* Dynamic: time in ns between consecutive refresh cycles */
    uint8_t         D_NumInitRef;       /* Dynamic: Number of auto-refreshes during init */
    uint32_t        D_tinit;            /* Dynamic: Number of ns to SDRAM inputs stable after power up */
    uint8_t         S_PageSize;         /* Static: Page size in words (0 = no page mode support) */
    uint8_t         S_tprc;             /* Static: time in ns page mode read cycle */
    uint8_t         S_tbta;             /* Static: time in ns memory idle cycles rd2wr or wr2rd or rd2rd */
    uint8_t         S_twp;              /* Static: time in ns write pulse width*/
    uint8_t         S_twr;              /* Static: time in ns write address/data hold time */
    uint8_t         S_tas;              /* Static: time in ns write address setup time */
    uint8_t         S_trc;              /* Static: time in ns read cycle time */
    
} r_memdb_Timing_t;


/*******************************************************************************
  Section: Global API Functions
*/

/*******************************************************************************
  Function: R_MEMDB_GetTiming

  Returns the Timing structure corresponding to Name of the memory

  Parameters:
  *Name         - Pointer to the name string of the memory
 
  Returns:
  see: <r_memdb_Timing_t>
*/

r_memdb_Timing_t *R_MEMDB_GetTiming(const char* Name);


#ifdef __cplusplus
}
#endif

#endif /* MEMDB_API_H_  */

/*
****************************************************************************
PROJECT : VLIB Macro Driver  
FILE    : $Id: r_xbus_main.c 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Driver for the XBUS macro
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
Purpose: only for testing, not for mass production

DISCLAIMER

LICENSEE has read, understood and accepted the terms and conditions defined in
the license agreement, especially the usage rights. In any case, it is
LICENSEE's responsibility to make sure that any user of the software complies
with the terms and conditions of the signed license agreement.

SAMPLE CODE is not part of the licensed software, as such it must not be used in
mass-production applications. It can only be used for evaluation and
demonstration purposes at customer's premises listed in the signed license
agreement.

****************************************************************************
*/

/*******************************************************************************
  Title: XBUS Main Module

  Implementation of the API and other global functions.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_dev_api.h"
#include "r_config_xbus.h"
#include "r_xbus_api.h"
#define XBUS_MAIN_GLOBALS_
#include "r_xbus_main.h"
#include "r_xbus_regs.h"
#include "r_xbus_sys.h"

#ifdef USE_ROS
#include "r_os_api.h"
#endif



/*******************************************************************************
  Section: Local Constants
*/
/*

  Enum: loc_CacheCmd_t

  XBUS cache commands 

  0000 IDX_LD_TAG ON
  Read from TRAM/Status-FF/LRU-FF of specified index/way.
  Write to AXCTAGLO, AXCLRU registers.
  Case of TE=0: write tag under ECC=Off. AXCTAGECC holds previous value.
  Case of TE=1 & TC=0: write tag/checkbits before correction.
  Case of TE=1 & TC=1: write tag/checkbits after correction.
  
  0001B IDX_ST_TAG 
  Read AXCTAGLO, AXCLRU registers.
  Case of AXCTAGECC.TE=0: Tag check bits are generated automatically.
  Case of AXCTAGECC.TE=1: AXCTAGECC.TAGECC is used.
  Write to TRAM/Status-FF/LRU-FF of specified index/way.
  
  0010B IDX_LD_DATA 
  Read from DRAM of specified index/double word.
  Write to AXCDATALO/AXCDATAHI/AXCECC registers.
  Case of DE=0: write data under ECC=Off. AXCDATAECC holds previous value.
  Case of DE=1 & DC=0 : write data/checkbits before correction.
  Case of DE=1 & DC=1 : write data/checkbits after correction.
  Hardware automatically compensates corkscrew allocation.
  
  0011B IDX_ST_DATA 
  Read from AXCDATALO/AXCDATAHI/AXCECC registers.
  Case of AXCDATAECC.DE 0 : Data check bits are generated automatically.
  Case of AXCDATAECC.DE 1 : AXCDATAECC.DATAECC(0/1) are used.
  Write to DRAM of specified index/doubleword.
  Hardware automatically compensates corkscrew allocation.
  
  0100B IDX_WB_INV 
  Check status of specified index/way.
  Case of Valid=1 & Dirty=1 : Writeback & Invalidate specified entry.
  
  0101B FILL  
  Fill data to cache way specified by LRU.

  0110B HIT_WB_INV 
  Check status of specified address.
  Case of Hit & Dirty=1 : Writeback & Invalidate specified entry.
  Case of Hit & Dirty=0 : Invalidate specified entry
  Case of Miss : No operation.
  
  0111B CREATE_DIRTY 
  Create dirty status.
  Case of Hit : Set D=1
  Case of Miss : Write specified address to TRAM.
  Case of V=0 : Write 0 to DRAM
  Case of V=1 & D=1 : with writeback.
  
  1xxxB CLEAR 
  Clear target : All of (Valid, Dirty, LRU) bits.
  
*/

typedef enum 
{
   IDX_LD_TAG      = 0x0u,  
   IDX_ST_TAG      = 0x1u,
   IDX_LD_DATA     = 0x2u,
   IDX_ST_DATA     = 0x3u,
   IDX_WB_INV      = 0x4u,
   FILL            = 0x5u,
   HIT_WB_INV      = 0x6u,
   CREATE_DIRTY    = 0x7u,
   CLEAR           = 0x8u,
} loc_CacheCmd_t; 
/*******************************************************************************
  Constant: loc_XbusVersion

  A string containig the macro driver version information  
*/


#define LOC_XBUS_VERSIONPREFIX    "@(#)XBUS Version "
#define LOC_XBUS_VERSIONPOSTFIX   " - "


#define LOC_XBUS_VERSION      { LOC_XBUS_VERSIONPREFIX  R_XSTR (R_XBUS_VERSION_HI) "." \
                                R_XSTR(R_XBUS_VERSION_LO) LOC_XBUS_VERSIONPOSTFIX \
                                __DATE__ R_SPACE __TIME__ }



/*******************************************************************************
  Section: Local Types 
*/

typedef struct name
{
    uint32_t Start;
    uint32_t End;
    uint8_t  Mode; 
} locCacheArea_t;

/*******************************************************************************
  Section: Local Variables
*/

/*******************************************************************************
  Variable: locXcLock

  Neccessay to aqurire a thread mutex  
*/
#ifdef USE_ROS
static R_OS_ThreadMutex_t locXcLock;  
#endif


/*******************************************************************************
  Variable: locBaseAddr

  Base Address of the XBUS config registers. The value is defined in the 
  r_config_xbus.h file.    
*/
static uint32_t locBaseAddr = 0; 

/*******************************************************************************
  Variable: loc_ErrorHandler

  A function pointer to a user callback (Applications error handler)  
  
  The value is assigned by the application using the driver 
*/

static void (*loc_ErrorHandler )(uint32_t Unit, uint32_t Error) = 0u;

/*******************************************************************************
  Variable: CacheAreas

  Local storage for cache config  
*/
static locCacheArea_t CacheAreas[4]; 

/*******************************************************************************
  Variable: locMaxIdx

  Amount of cache indexes (device dependant)   
*/
static uint32_t locMaxIdx = 0;
 
/*******************************************************************************
  Section: local Functions
*/
void loc_WriteCmd(uint32_t CmdVal) ;



/*******************************************************************************
  Functions loc_WriteCmd
  
  Write the given command and waits for the execution to be finished. 
*/
void loc_WriteCmd(uint32_t CmdVal) 
{
    volatile uint32_t reg_val = CmdVal; 
    /* Write command */
    R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_CACHECMD, reg_val); 
    reg_val = R_XBUS_READ_REG(32,locBaseAddr + R_XBUS_CACHECMD);  
    /* Wait for cmd complete */
    while (0 != (reg_val & 0x1u))
    {
        reg_val = R_XBUS_READ_REG(32,locBaseAddr + R_XBUS_CACHECMD);  
    }
}
/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_XBUS_CheckMacroParams
  
  See: <xbus_main.h>
*/
r_xbus_Error_t R_XBUS_CheckMacroParams(uint32_t Unit)
{
    /* Feel free to add any more usefull parameter checks */
    if (R_XBUS_MACRO_NUM <= Unit)
    {
        return  R_XBUS_ERR_RANGE; 
    }
    return R_XBUS_ERR_OK;  
}

/*******************************************************************************
  Function: R_XBUS_ErrorHandler
  
  See: <xbus_main.h>
*/

void R_XBUS_ErrorHandler(uint32_t Unit, r_xbus_Error_t Error) 
{
    if (0 != loc_ErrorHandler) 
    {      /* we've got a registered error handler here ...*/
        loc_ErrorHandler(Unit, (uint32_t) Error);      /* ... and we gonna use it!*/
    } 
    else 
    {
        R_SYS_XBUS_ErrorHandler(Unit, Error); 
    }
}

/*******************************************************************************
  Section: Global API Functions
  
  See: <xbus_api.h>
*/

/*******************************************************************************
  Function: R_XBUS_GetVersionStr

  See: <xbus_api.h> 
*/
const int8_t * R_XBUS_GetVersionStr(void)
{
    static const int8_t version[] = LOC_XBUS_VERSION;
    return version;
}

/*******************************************************************************
  Function: R_XBUS_GetMajorVersion

  See: <xbus_api.h> 
*/
const uint16_t R_XBUS_GetMajorVersion(void) 
{
    return (uint16_t)R_XBUS_VERSION_HI; 
}

/*******************************************************************************
  Function: R_XBUS_GetMinorVersion

  See: <xbus_api.h> 
*/
const uint16_t R_XBUS_GetMinorVersion(void) 
{
    return (uint16_t)R_XBUS_VERSION_LO; 
}



/*******************************************************************************
  Function: R_XBUS_Init

  See: <xbus_api.h> 
*/

 r_xbus_Error_t R_XBUS_Init(uint32_t Unit) 
 {
    r_xbus_Error_t    err       = R_XBUS_ERR_OK;
    volatile uint32_t reg_val   = 0; 
    loc_CacheCmd_t    cmd       = CLEAR; 
    uint32_t          i         = 0; 
    uint32_t          j         = 0; 
    err = R_XBUS_CheckMacroParams(Unit); 
    if (R_XBUS_ERR_OK != err) 
    {
        return err; 
    }
    /* Get register base address */
    locBaseAddr = R_XBUS_SYS_BaseAddr(Unit); 
    /* get max index (depending on the device) */
    locMaxIdx   = R_XBUS_SYS_GetMaxIdx(Unit); 
    
    /* check if we have a device with some cache  */
    if (0 == locMaxIdx) 
    {
        locBaseAddr = 0; 
        return R_XBUS_ERR_NG;
    }
    
    /* 1. lock clear */
    R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_SYSERR, 0); 
    R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_ERRCNT, 0); 

    reg_val = R_XBUS_READ_REG(32, locBaseAddr + R_XBUS_CFG); 
 
     reg_val  = 0; 
    R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_TAGLO,   reg_val); 
    R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_LRU,     reg_val); 
    R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_DATALO,  reg_val); 
    R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_DATAHI,  reg_val); 
    R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_DATAECC, reg_val); 
    R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_TAGECC,  reg_val); 
 
    /* 2. initialize  */

    R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_SYSERR, 0); 
    R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_ERRCNT, 0); 
    R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_ERRADR, 0); 
 
    /* initialise Cache RAM */
    
    for (i = 0; i < locMaxIdx ; i++) 
    { 
        for (j = 0; j < (R_XBUS_CACHE_BLK_NB * R_XBUS_CACHE_WAY_NB); j++) 
        {
            cmd         = IDX_ST_TAG; 
            reg_val     = (uint32_t)cmd; 
            reg_val   <<= 1u;
            /* Command activation */
            reg_val    |= 1u;
            /* add next index to the command */
            reg_val    |= (i<<5u); 
            /* Block&way are bit 19-16 and not 15-12 */
             reg_val   |= (j<<16u); 
           
            /* Write command */
            loc_WriteCmd(reg_val); 
           
            /* clear command type */
            reg_val     &=(uint32_t) (~(uint32_t)0x0000001eu);  
            cmd         = IDX_ST_DATA; 
            /* Set new command */
            reg_val     |= ((uint32_t)cmd <<1); 

            /* Command activation */
            reg_val    |= 1u;
    
            /* Write command */
            loc_WriteCmd(reg_val); 
        }
    }

    
    /* init cache config, disable cache area  */
    /*  disable wait insertion */
    reg_val = 0x00000303;
    R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_CFG, reg_val); 
 
    /* synchronise ... whatever */
    reg_val = R_XBUS_READ_REG(32,locBaseAddr + R_XBUS_REQSYNC);  
    
    #ifdef USE_ROS
    R_OS_ThreadMutexInit(&locXcLock, 0);
    #endif

    return err;
}


/*******************************************************************************
  Function: R_XBUS_Invalidate

  See: <xbus_api.h> 
*/

 r_xbus_Error_t R_XBUS_Invalidate(uint32_t Unit) 
 {
    r_xbus_Error_t    err       = R_XBUS_ERR_OK;
    volatile uint32_t reg_val   = 0; 
    volatile uint32_t tag_lo    = 0; 
    loc_CacheCmd_t    cmd       = CLEAR; 
    uint32_t          i         = 0; 
    uint32_t          j         = 0; 
    err = R_XBUS_CheckMacroParams(Unit); 
    if (R_XBUS_ERR_OK != err) 
    {
        return err; 
    }
    /* Get register base address */
    locBaseAddr = R_XBUS_SYS_BaseAddr(Unit); 
    /* get max index (depending on the device) */
    locMaxIdx   = R_XBUS_SYS_GetMaxIdx(Unit); 
    
    /* check if we have a device with some cache  */
    if (0 == locMaxIdx) 
    {
        locBaseAddr = 0; 
        return R_XBUS_ERR_NG;
    }
    
    /* 1. lock clear */
    R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_SYSERR, 0); 
    R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_ERRCNT, 0); 

    reg_val = R_XBUS_READ_REG(32, locBaseAddr + R_XBUS_CFG); 
 
    reg_val  = 0; 
 
    /* Invalidate (but don't write back) Cache RAM */
    
    for (i = 0; i < locMaxIdx ; i++) 
    { 
        for (j = 0; j < R_XBUS_CACHE_WAY_NB; j++) 
        {
            cmd         = IDX_LD_TAG; 
            reg_val     = (uint32_t)cmd; 
            reg_val   <<= 1u;
            /* Command activation */
            reg_val    |= 1u;
            /* add next index to the command */
            reg_val    |= (i<<5u); 
            /* Block&way are bit 19-16 and not 15-12 */
             reg_val   |= (j<<16u); 
           
            /* Write command */
            loc_WriteCmd(reg_val); 
           
            tag_lo = R_XBUS_READ_REG(32, locBaseAddr + R_XBUS_TAGLO); 
            /* Clear valid bit */
            tag_lo &= 0xFFFFFFFEu; 
            R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_TAGLO, tag_lo); 
            
              /* clear command type */
            reg_val     &=(uint32_t) (~(uint32_t)0x0000001eu);  
         
            /* clear command type */
            cmd         = IDX_ST_TAG; 
            /* Set new command */
            reg_val     |= ((uint32_t)cmd <<1); 
             /* Command activation */
            reg_val     |= 1u;

            /* Write command */
            loc_WriteCmd(reg_val); 
        }
    }
    
    #ifdef USE_ROS
    R_OS_ThreadMutexInit(&locXcLock, 0);
    #endif

    return err;
}


/*******************************************************************************
  Function: R_XBUS_DeInit

  See: <xbus_api.h> 
*/

 r_xbus_Error_t R_XBUS_DeInit(uint32_t Unit)
 {
    r_xbus_Error_t err = R_XBUS_ERR_OK;
    locBaseAddr = 0; 
    return err;
}


/*******************************************************************************
  Function: R_XBUS_Config
*/

r_xbus_Error_t R_XBUS_Config(uint32_t Unit, r_xbus_Cfg_t * Cfg)
{
    /* Init X-BUS */
    r_xbus_Error_t    err       = R_XBUS_ERR_OK;
    volatile uint32_t reg_val   = 0; 
    uint32_t          i,j;  
    if (0 == locBaseAddr) 
    {
        /* You need to call init first */
        R_XBUS_ErrorHandler(0, err); 
        return R_XBUS_ERR_NOTLOCKED; 
    }    
    /* Set base & mask */
    /* Cached */
    reg_val = Cfg->AreaCfg[0].BaseAddr; 
    R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_CABASE0, reg_val); 
    reg_val =  Cfg->AreaCfg[0].AddrMask; 
    R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_CAMASK0, reg_val); 
    
    reg_val =  Cfg->AreaCfg[1].BaseAddr; 
    R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_CABASE1, reg_val); 
    reg_val =  Cfg->AreaCfg[1].AddrMask; 
    R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_CAMASK1, reg_val); 
    
    /* buffered */
    reg_val = Cfg->AreaCfg[2].BaseAddr; 
    R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_BFBASE0, reg_val); 
    reg_val =  Cfg->AreaCfg[2].AddrMask; 
    R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_BFMASK0, reg_val); 

    reg_val =  Cfg->AreaCfg[3].BaseAddr; 
    R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_BFBASE1, reg_val); 
    reg_val =  Cfg->AreaCfg[3].AddrMask; 
    R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_BFMASK1, reg_val); 
    
    for (i = 0; i < 4; i++)
    {
        /* Check if cache disabled */
        CacheAreas[i].Mode  = Cfg->AreaCfg[i].Mode; 
        CacheAreas[i].Start = Cfg->AreaCfg[i].BaseAddr;
        CacheAreas[i].End   = Cfg->AreaCfg[i].BaseAddr;
        reg_val             = Cfg->AreaCfg[i].AddrMask; 
        j                   = 0; 
        while (((reg_val >> 20u) & 0x1u) > 0)
        {
            /* The cache "mask" means the size equals to 
            1MB per bit set in the R_XBUS_CAMASK0 register 
            the first 0 (starting from bit 20) 
            in the mask is the delimeter */
            CacheAreas[i].End += ((1024u * 1024u) << j); 
            reg_val          >>= 1u; 
            j++;      
        }        
    }
    /* Store the config */    
    /* Start with the initial value for the CFG register */
    reg_val = 0x00000303;

    if (R_XBUS_AM_CACHE == Cfg->AreaCfg[0].Mode)
    {
        reg_val |= 0x00000004u; 
    }
    else 
    {
        reg_val &= (uint32_t)(~((uint32_t)0x00000004u)); 
    } 
    if (R_XBUS_AM_CACHE == Cfg->AreaCfg[1].Mode)
    {
        reg_val |= 0x00000008u; 
    }
    else 
    {
        reg_val &= (uint32_t)(~((uint32_t)0x00000008u)); 
    } 


    if (R_XBUS_AM_BUFF == Cfg->AreaCfg[2].Mode)
    {
        reg_val |= 0x00000100u; 
    }
    else 
    {
        reg_val &= (uint32_t)(~((uint32_t)0x00000100u)); 
    } 
    if (R_XBUS_AM_BUFF == Cfg->AreaCfg[3].Mode)
    {
        reg_val |= 0x00000200u; 
    }
    else 
    {
        reg_val &= (uint32_t)(~((uint32_t)0x00000200u)); 
    } 
    if (1 == Cfg->WaitInsertion) 
    {
        reg_val |= 0x00020000u; 
    }
    else 
    {
        reg_val &= ~(0x00020000u); 
        
    }
    if (1 == Cfg->SecRewrite) 
    {
        reg_val |= 0x00010000u; 
    }
    else 
    {
        reg_val &= ~(0x00010000u); 
        
    }
    /* Write config */
    R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_CFG, reg_val); 

    /* Clear error registers */

    R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_SYSERR, 0x0);
    R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_ERRADR, 0x0);  

    /* Fill cache */
    
    reg_val = R_XBUS_READ_REG(32,locBaseAddr + R_XBUS_REQSYNC);   

    loc_WriteCmd(reg_val); 
    return R_XBUS_ERR_OK;
}

/*******************************************************************************
  Function: R_XBUS_AreaConfig
*/

r_xbus_Error_t R_XBUS_AreaConfig(uint32_t Unit, uint8_t Area, r_xbus_AreaCfg_t * Cfg)
{
    r_xbus_Error_t    err       = R_XBUS_ERR_OK;
    volatile uint32_t reg_val   = 0; 

    if (0 == locBaseAddr) 
    {
        /* You need to call init first */
        R_XBUS_ErrorHandler(0, err); 
        return R_XBUS_ERR_NOTLOCKED; 
    }    

    if (0 == Area) 
    {
        if (R_XBUS_AM_CACHE == Cfg->Mode)
        {
            reg_val = Cfg->BaseAddr; 
            R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_CABASE0, reg_val); 
            reg_val = Cfg->AddrMask; 
            R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_CAMASK0, reg_val); 
            reg_val =  R_XBUS_READ_REG(32,locBaseAddr + R_XBUS_CFG);  
            reg_val |= 0x4u; 
        }
        else if (R_XBUS_AM_BUFF == Cfg->Mode)
        {
            reg_val = Cfg->BaseAddr; 
            R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_BFBASE0, reg_val); 
            reg_val = Cfg->AddrMask; 
            R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_BFMASK0, reg_val); 
            reg_val =  R_XBUS_READ_REG(32,locBaseAddr + R_XBUS_CFG);  
            reg_val |= 0x100u; 
        }
        else
        {
            R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_CABASE0, 0); 
            R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_CAMASK0, 0); 
            R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_BFBASE0, 0); 
            R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_BFMASK0, 0); 
            reg_val &= (uint32_t)(~((uint32_t)0x104u)); 
        } 
        R_XBUS_WRITE_REG(32,locBaseAddr + R_XBUS_CFG, reg_val);  
    }
    else 
    {
        if (R_XBUS_AM_CACHE == Cfg->Mode)
        {
            reg_val = Cfg->BaseAddr; 
            R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_CABASE1, reg_val); 
            reg_val = Cfg->AddrMask; 
            R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_CAMASK1, reg_val); 
            reg_val =  R_XBUS_READ_REG(32,locBaseAddr + R_XBUS_CFG);  
            reg_val |= 0x8u; 
        }
        else if (R_XBUS_AM_BUFF == Cfg->Mode)
        {
            reg_val = Cfg->BaseAddr; 
            R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_BFBASE1, reg_val); 
            reg_val = Cfg->AddrMask; 
            R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_BFMASK1, reg_val); 
            
            reg_val =  R_XBUS_READ_REG(32,locBaseAddr + R_XBUS_CFG);  
            reg_val |= 0x200u; 
        }
        else
        {
            R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_CABASE1, 0); 
            R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_CAMASK1, 0); 
            R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_BFBASE1, 0); 
            R_XBUS_WRITE_REG(32, locBaseAddr + R_XBUS_BFMASK1, 0); 
            reg_val &= (uint32_t)(~((uint32_t)0x208u)); 
        } 
        R_XBUS_WRITE_REG(32,locBaseAddr + R_XBUS_CFG, reg_val);  
    }
    /* synchronise ... whatever */
    reg_val = R_XBUS_READ_REG(32,locBaseAddr + R_XBUS_REQSYNC);  
    return R_XBUS_ERR_OK;
}

/*******************************************************************************
  Function: R_XBUS_Flush
*/

void R_XBUS_Flush(uint32_t Unit, uint32_t FlushAddr, uint32_t FlushSize)
{
    volatile uint32_t reg_val   = 0;
    uint32_t idx, way, addr; 
    
    /* make sure we flush the relevant size even when
     * the FlushAddr is no aligned to 32 bytes */
    FlushSize += 32;

    if (0 == locBaseAddr) 
    {
        /* You need to call init first */
        R_XBUS_ErrorHandler(0, R_XBUS_ERR_NOTLOCKED); 
        return; 
    }    
    #ifdef USE_ROS
    R_OS_ThreadMutexLock(&locXcLock);
    #endif

    /* Synchronise cache domain */
    reg_val = R_XBUS_READ_REG(32,locBaseAddr + R_XBUS_REQSYNC);   
    
    /* Flush all cache? */
    if (FlushAddr < (uint32_t)R_XBUS_AM_BUFF) 
    {
        /* Flush the buffer as well  */
        if (0 == FlushAddr) 
        {
            reg_val = R_XBUS_READ_REG(32,locBaseAddr + R_XBUS_BUFFLUSH);   
        }
        /* Flush the cache  */
        for (way = 0; way < R_XBUS_CACHE_WAY_NB; way++) 
        {
            for (idx = 0; idx < locMaxIdx; idx++)
            {
                 /* Store (reset) command */
                 reg_val     =  (uint8_t)((uint8_t)IDX_WB_INV << 1u); 
                 reg_val    |= 0x1u; /* 0100B IDX_WB_INV | VLD */
                /* add way   */
                 reg_val   |= (way << 16u); 
                /* add index to the command */
                reg_val    |= (idx << 5u); 
               
                /* Write command */
                loc_WriteCmd(reg_val); 
            }
        } 
    }
    else if ((uint32_t)R_XBUS_AM_BUFF == FlushAddr)
    {
        /* Just buffer flush */
        reg_val = R_XBUS_READ_REG(32,locBaseAddr + R_XBUS_BUFFLUSH);   
    }    
    else /* check the cache areas */
    {
        
        if (FlushAddr >= CacheAreas[3].Start)
        {
            /* address in range and buffered mode on? */
            if ((FlushAddr < CacheAreas[3].End) && (R_XBUS_AM_BUFF == CacheAreas[3].Mode))
            {
                /* Buffered area flush */
                reg_val = R_XBUS_READ_REG(32,locBaseAddr + R_XBUS_BUFFLUSH);  
                #ifdef USE_ROS
                R_OS_ThreadMutexUnlock(&locXcLock);
                #endif

                return; 
            } 
        }    
        if (FlushAddr >= CacheAreas[2].Start)
        {
           /* address in range and buffered mode on? */
            if ((FlushAddr < CacheAreas[2].End) && (R_XBUS_AM_BUFF == CacheAreas[2].Mode))
            {
                /* Buffered area flush */
                reg_val = R_XBUS_READ_REG(32,locBaseAddr + R_XBUS_BUFFLUSH);
                #ifdef USE_ROS
                R_OS_ThreadMutexUnlock(&locXcLock);
                #endif

                return; 
  
            } 
        }    
        if (FlushAddr >= CacheAreas[1].Start)
        {
            /* address in range and cached mode on? */
            if ((FlushAddr < CacheAreas[1].End) && (R_XBUS_AM_CACHE == CacheAreas[1].Mode))
            {
                addr = FlushAddr; 
                while ((addr <= (FlushAddr + FlushSize)) && (addr < CacheAreas[1].End))
                {
                    /* Flush from address*/
                    reg_val   = (uint8_t)((uint8_t)HIT_WB_INV << 1u); 
                    reg_val  |=  0x1u; /* 0110B HIT_WB_INV | VLD */
                    reg_val  |= (addr & 0xFFFFFFE0u);
    
                    /* Write command */
                    loc_WriteCmd(reg_val); 
    
                    /* the next 32 byte */
                    addr += 32; 
                }
                #ifdef USE_ROS
                R_OS_ThreadMutexUnlock(&locXcLock);
                #endif

                return; 
            }    
        }
        if (FlushAddr >= CacheAreas[0].Start)
        {
            /* address in range and cached mode on? */
            if ((FlushAddr < CacheAreas[0].End) && (R_XBUS_AM_CACHE == CacheAreas[0].Mode))
            {
                addr = FlushAddr; 
                while ((addr <= (FlushAddr + FlushSize)) && (addr < CacheAreas[0].End))
                {
                    /* Flush from address*/
                    reg_val   = (uint8_t)((uint8_t)HIT_WB_INV << 1u); 
                    reg_val  |=  0x1u; /* 0110B HIT_WB_INV | VLD */
                    reg_val |= (addr & 0xFFFFFFE0u);
    
                    /* Write command */
                    loc_WriteCmd(reg_val); 
    
                    /* next 32 byte */
                    addr += 32; 
                }
                #ifdef USE_ROS
                R_OS_ThreadMutexUnlock(&locXcLock);
                #endif

                return; 
            }    
        }
    }
    #ifdef USE_ROS
    R_OS_ThreadMutexUnlock(&locXcLock);
    #endif

    
}

/*******************************************************************************
  Function: R_XBUS_SetMode
*/
r_xbus_Error_t R_XBUS_SetMode(uint32_t Unit, uint8_t Area, r_xbus_AreaMode_t Mode) 
{
    r_xbus_Error_t    err       = R_XBUS_ERR_OK;
    volatile uint32_t reg_val   = 0; 
    if (0 == locBaseAddr) 
    {
        /* You need to call init first */
        R_XBUS_ErrorHandler(0, R_XBUS_ERR_NOTLOCKED); 
        return R_XBUS_ERR_NOTLOCKED; 
    }    

    reg_val =  R_XBUS_READ_REG(32,locBaseAddr + R_XBUS_CFG);  
    if (0 == Area) 
    {
        if (R_XBUS_AM_CACHE == Mode)
        {
            reg_val |= 0x4u; 
        }
        else if (R_XBUS_AM_BUFF == Mode)
        {
            reg_val |= 0x100u; 
        }
        else 
        {
            reg_val &= (uint32_t)(~((uint32_t)0x104u)); 
        } 
    }
    else 
    {
        if (R_XBUS_AM_CACHE == Mode)
        {
            reg_val |= 0x8u; 
        }
        else if (R_XBUS_AM_BUFF == Mode)
        {
            reg_val |= 0x200u; 
        }
        else 
        {
            reg_val &= (uint32_t)(~((uint32_t)0x208u)); 
        } 
    }
    R_XBUS_WRITE_REG(32,locBaseAddr + R_XBUS_CFG, reg_val);  
    /* synchronise ... whatever */
    reg_val = R_XBUS_READ_REG(32,locBaseAddr + R_XBUS_REQSYNC);  
    return err;

}

/*******************************************************************************
  Function: R_XBUS_SetErrorCallback
*/

void R_XBUS_SetErrorCallback(void(*ErrorCallback )(uint32_t Unit, uint32_t Error)) 
{
    loc_ErrorHandler = ErrorCallback;
}






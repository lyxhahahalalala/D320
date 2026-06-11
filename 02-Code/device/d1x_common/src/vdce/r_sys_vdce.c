/*
****************************************************************************
PROJECT : VDCE driver
FILE    : $Id: r_sys_vdce.c 7902 2016-03-09 04:26:22Z shinya.tomari $
============================================================================
DESCRIPTION
VDCE system functions
============================================================================
                            C O P Y R I G H T
============================================================================
                           Copyright (c) 2013 - 2015
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

/******************************************************************************
  Title: Generic VDCE Support Functions

    VDCE driver Support Functions

******************************************************************************/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"   /* Renesas basic types, e.g. uint32_t */
#include "r_ddb_api.h"    /* display database with display timings */
#include "r_dev_api.h"
#include "r_config_hwversion.h"
#include "r_config_vdce.h"
#include "r_config_vdce_num.h"
#include "r_vdce_api.h"
#include "r_vdce_sys.h"

/*******************************************************************************
  Section: Local Defines
*/

/* USE_VDCE_RSDS
    0 = LVTTL,
    1 = RSDS
*/
#define USE_VDCE_RSDS    (0)

#define LOC_PRDNAME_THRESHOLD  (300u)
#define LOC_VDCE_REVISION_1    (1u)
#define LOC_VDCE_REVISION_2    (2u)


/* Video output pixel clocks exchange register (D1M2(H) only)
*/
#define LOC_CKSC_IVOEXS_CTL_REG_ADDR    (0xFFF85900u)
#define LOC_PROTCMDD1     (0xfff87000u)


#define LOC_PINV45  (0xFF610B70u)
#define LOC_PINV47  (0xFF610BF0u)
#define LOC_PPCMD45 (0xFFC14B6Cu)
#define LOC_PPCMD47 (0xFFC14BECu)


/* The definition of the interrupt control.
*/
#define LOC_INTC_UNIT0_START   (R_DEV_INT_VDCE0ERR)
#define LOC_INTC_UNIT0_END     (R_DEV_INT_VDCE0IRWLINE)
#define LOC_INTC_UNIT1_START   (R_DEV_INT_VDCE1ERR)
#define LOC_INTC_UNIT1_END     (R_DEV_INT_VDCE1S1WLINE)
#define LOC_INTC_UNIT1_OFFSET  (R_DEV_INT_VDCE1ERR - R_DEV_INT_VDCE0ERR)

#define LOC_MASK_0X00000001 (0x00000001u)
#define LOC_MASK_0X00000100 (0x00000100u)

/* The definition of the device version.

   If this definition is not defined, VDCE works as H/W device version.
   If this definition is defined, VDCE works as specified device version for debug.
   
  Values:
    R_DEV_R7F701403 - VDCE works as D1Lx.
    R_DEV_R7F701407 - VDCE works as D1M1x.
    R_DEV_R7F701410 - VDCE works as D1M2
    R_DEV_R7F701412 - VDCE works as D1M2H
*/
/* #define LOC_VDCE_FORCE_DEV_NUM    (R_DEV_R7F701403) */

/*******************************************************************************
  Section: Local functions
*/
/* list of static functions: */
static uint32_t loc_calcClockDiv(const uint32_t Clkin, const uint32_t Clkout) ; 
static uint32_t loc_Vdce_Pixel_SetClockM2(const uint32_t Unit, const uint32_t Freq);
static uint32_t loc_Vdce_Pixel_SetClockL2M1(const uint32_t Unit, const uint32_t Freq);
static r_vdce_Error_t loc_IntcCtrl(const uint32_t       Unit,
                                   const r_dev_IntSel_t IntSel,
                                   const uint8_t        Enable);
static r_dev_Device_t loc_GetDev(void);
static r_vdce_Error_t loc_IntcCtrlAll(const uint32_t Unit, const uint8_t Enable);
static r_vdce_Error_t loc_IntcCtrl(const uint32_t Unit, const r_dev_IntSel_t IntSel, const uint8_t Enable);

/*******************************************************************************
  Function: loc_calcClockDiv
*/

static uint32_t loc_calcClockDiv(const uint32_t Clkin, const uint32_t Clkout)
{
    uint32_t divider;
    uint32_t divfrq;
    uint32_t divfrq_next;

    /* calc near divider */
    divider = Clkin / Clkout;

    /* test if next divider is better, if yes choose next divider */
    divfrq = Clkin / divider;
    divfrq_next = Clkin / (divider+1u);
    if ( (divfrq - Clkout) > (Clkout - divfrq_next))
    {
        divider += 1u;
    }

    /* divider value 0 is invalid, choose 1 instead */
    if (divider == 0u)
    {
        divider = 1;
    }

    return divider;
}

/*******************************************************************************
  Function: loc_Vdce_Pixel_SetClockM2
*/

static uint32_t loc_Vdce_Pixel_SetClockM2(const uint32_t Unit, const uint32_t Freq)
{
    uint32_t divider;
    uint32_t clkin;
    r_dev_ClkSelConfig_t ClkSelCfg[2];
    
    /* get VDCE clock */
    clkin = R_DEV_BusFrequencyHz(R_DEV_CLK_PLL2);
#if USE_VDCE_RSDS    /* 0=LVTTL, 1=RSDS (divider by 4 for pixel clock, when RSDS is used) */
    clkin /= 4;
#endif /* USE_VDCE_RSDS */
    /* dbg into: Set VDCE clock divider */
    divider = (Freq != 0) ? loc_calcClockDiv(clkin, Freq) : 0;
    if (divider == 1)
    {
        divider = 2;
    }
    if (divider > 255)
    {
        divider = 255;
    }
    
    if (Unit == 0)
    {
        ClkSelCfg[0].Cks       = R_DEV_CKS_DOTCK0;
        ClkSelCfg[0].SrcId     = R_DEV_CKS_SRC_PLL2;
        ClkSelCfg[0].Div       = (uint16_t)divider;
        ClkSelCfg[0].StpReqMsk = 0;
        
        ClkSelCfg[1].Cks       = R_DEV_CKS_VDCE0CK;
#if USE_VDCE_RSDS    /* 0=LVTTL, 1=RSDS (divider by 4 for pixel clock, when RSDS is used) */
        ClkSelCfg[1].SrcId     = R_DEV_CKS_SRC_DOTCK0_4;
#else        
        ClkSelCfg[1].SrcId     = R_DEV_CKS_SRC_DOTCK0;
#endif /* USE_VDCE_RSDS */
        ClkSelCfg[1].Div       = 0;
        ClkSelCfg[1].StpReqMsk = 0;
    }
    else
    {
        ClkSelCfg[0].Cks       = R_DEV_CKS_DOTCK1;
        ClkSelCfg[0].SrcId     = R_DEV_CKS_SRC_PLL2;
        ClkSelCfg[0].Div       = (uint16_t)divider;
        ClkSelCfg[0].StpReqMsk = 0;
        
        ClkSelCfg[1].Cks       = R_DEV_CKS_VDCE1CK;
#if USE_VDCE_RSDS    /* 0=LVTTL, 1=RSDS (divider by 4 for pixel clock, when RSDS is used) */
        ClkSelCfg[1].SrcId     = R_DEV_CKS_SRC_DOTCK1_4;
#else        
        ClkSelCfg[1].SrcId     = R_DEV_CKS_SRC_DOTCK1;
#endif /* USE_VDCE_RSDS */
        ClkSelCfg[1].Div       = 0;
        ClkSelCfg[1].StpReqMsk = 0;
    }
    (void)R_DEV_ClkIdSet(&ClkSelCfg[0]);
    (void)R_DEV_ClkIdSet(&ClkSelCfg[1]);
    
    /* Disable exchange of VDCE0_VO_CLK and VDCE1_VO_CLK */
    R_DEV_WRITE_PROT_REG(LOC_PROTCMDD1, LOC_CKSC_IVOEXS_CTL_REG_ADDR, 0);
    
    return (divider != 0) ? (clkin / divider):0;


}

/*******************************************************************************
  Function: loc_Vdce_Pixel_SetClockL2M1
  
  set VDCE pixel clock (in MHz)
*/

static uint32_t loc_Vdce_Pixel_SetClockL2M1(const uint32_t Unit, const uint32_t Freq)
{
    uint32_t div, clkin;
    r_dev_ClkSelConfig_t ClkSelCfg[2];
    
    /* get VDCE clock */
    clkin = R_DEV_BusFrequencyHz(R_DEV_CLK_PLL0PIX);
    R_DBG_PRINT(R_DBG_MSG_CODE,  "Set VDCE clock divider");
    div = (Freq != 0) ? loc_calcClockDiv(clkin, Freq) : 0;
    if (div == 1)
    {
        div = 2;
    }
    if (div > 255)
    {
        div = 255;
    }
    
    ClkSelCfg[0].Cks       = R_DEV_CKS_DOTCK0;
    
    ClkSelCfg[0].SrcId     = R_DEV_CKS_SRC_PLL0PIX;
    ClkSelCfg[0].Div       = div;
    ClkSelCfg[0].StpReqMsk = 0;
    
    ClkSelCfg[1].Cks       = R_DEV_CKS_VDCE0CK;
    ClkSelCfg[1].SrcId     = R_DEV_CKS_SRC_DOTCK0;
    ClkSelCfg[1].Div       = 0;
    ClkSelCfg[1].StpReqMsk = 0;
    
    R_DEV_ClkIdSet(&ClkSelCfg[0]);
    R_DEV_ClkIdSet(&ClkSelCfg[1]);
    
    return (div != 0) ? (clkin / div):0;
}

/*******************************************************************************
  Function: loc_IntcCtrlAll
*/
static r_vdce_Error_t loc_IntcCtrlAll(const uint32_t Unit, const uint8_t Enable)
{
    r_vdce_Error_t ret;
    r_dev_IntSel_t i;

    ret = R_VDCE_ERR_OK;

    if (Unit < R_VDCE_MACRO_NUM)
    {
        if (Unit == R_VDCE_UNIT0)
        {
            /* Configure all interrupts on the device level */
            for (i = LOC_INTC_UNIT0_START; i <= LOC_INTC_UNIT0_END; i++)
            {
                if ((Enable == 0) || (i != R_DEV_INT_VDCE0GR3VBLANK))
                {
                    R_DEV_IntClearFlag(i);
                    R_DEV_IntEnable(i, Enable);
                }
            }
        }
        else /* Unit == R_VDCE_UNIT1 */
        {
            /* Configure all interrupts on the device level */
            for (i = LOC_INTC_UNIT1_START; i <= LOC_INTC_UNIT1_END; i++)
            {
                if ((Enable == 0) || (i != R_DEV_INT_VDCE1GR3VBLANK))
                {
                    R_DEV_IntClearFlag(i);
                    R_DEV_IntEnable(i, Enable);
                }
            }
        }
    }
    else
    {
        ret = R_VDCE_ERR_RANGE_UNIT;
    }
    
    return ret;
}

/*******************************************************************************
  Function: loc_IntcCtrl
*/
static r_vdce_Error_t loc_IntcCtrl(const uint32_t Unit, const r_dev_IntSel_t IntSel, const uint8_t Enable)
{
    r_vdce_Error_t ret;
    r_dev_IntSel_t int_sel;

    ret = R_VDCE_ERR_OK;

    if (Unit < R_VDCE_MACRO_NUM)
    {
        if (Unit == R_VDCE_UNIT0)
        {
            int_sel = IntSel;
            if ((int_sel >= LOC_INTC_UNIT0_START) && (int_sel <= LOC_INTC_UNIT0_END))
            {
                R_DEV_IntClearFlag(int_sel);
                R_DEV_IntEnable(int_sel, Enable);
            }
            else
            {
                ret = R_VDCE_ERR_RANGE_PARAM;
            }
        }
        else /* Unit == R_VDCE_UNIT1 */
        {
            int_sel = (r_dev_IntSel_t)(IntSel + LOC_INTC_UNIT1_OFFSET);
            if ((int_sel >= LOC_INTC_UNIT1_START) && (int_sel <= LOC_INTC_UNIT1_END))
            {
                R_DEV_IntClearFlag(int_sel);
                R_DEV_IntEnable(int_sel, Enable);
            }
            else
            {
                ret = R_VDCE_ERR_RANGE_PARAM;
            }
        }
    }
    else
    {
        ret = R_VDCE_ERR_RANGE_UNIT;
    }
    
    return ret;
}

/*******************************************************************************
  Function: loc_GetDev
*/
static r_dev_Device_t loc_GetDev(void)
{
    r_dev_Device_t  device;
    
    /* Modify device number if debug. */
#ifdef LOC_VDCE_FORCE_DEV_NUM
    device = LOC_VDCE_FORCE_DEV_NUM;
#else
    device = R_DEV_GetDev();
#endif
    return device;
}

/*******************************************************************************
  Section: VDCE Macro Driver Support Functions

  Comments see: <r_vdce_sys.h>
*/

/*******************************************************************************
  Function: R_VDCE_Sys_Init
*/

r_vdce_Error_t R_VDCE_Sys_Init(const uint32_t Unit)
{
    return R_VDCE_ERR_OK;
}

/*******************************************************************************
  Function: R_VDCE_Sys_DeInit
*/

r_vdce_Error_t R_VDCE_Sys_DeInit(const uint32_t Unit)
{
    return R_VDCE_ERR_OK;
}

/*******************************************************************************
  Function: R_VDCE_Sys_BaseAddrGet
*/

uint32_t R_VDCE_Sys_BaseAddrGet(const uint32_t Unit)
{
    uint32_t ret;
    r_dev_Device_t  device  =  R_DEV_GetDev(); 

    if (Unit == R_VDCE_UNIT0)
    {
        ret = R_VDCE_BASE_ADDR0;
    }
    else if (Unit == R_VDCE_UNIT1)
    {
        if ((device >= R_DEV_R7F701408) && (device <= R_DEV_R7F701412))    /* D1M2x */
        {
            ret = R_VDCE_BASE_ADDR1;
        }
        else
        {
            ret = 0;
        }
    }
    else
    {
        ret = 0;
    }

    return ret;
}

/*******************************************************************************
  Function: R_VDCE_Sys_PixelClockSet
*/

uint32_t R_VDCE_Sys_PixelClockSet(const uint32_t Unit,
                                  const uint32_t Clock)
{
    uint32_t ret;
    r_dev_Device_t Device;

    if (Unit >= R_VDCE_MACRO_NUM)
    {
        ret = 0;
    }
    else
    {
        /* activate VDCE pixel clock */
        /*  - pixel clock generator depends on the current device */
        Device = R_DEV_GetDev();
        if ((Device >= R_DEV_R7F701401) && (Device <= R_DEV_R7F701407))
        {
            ret = loc_Vdce_Pixel_SetClockL2M1(Unit, Clock);
        } else {
            ret = loc_Vdce_Pixel_SetClockM2(Unit, Clock);
        }
    }
    
    return ret;
}

/*******************************************************************************
  Function: R_VDCE_Sys_VdceVersionGet
*/

uint32_t R_VDCE_Sys_VdceVersionGet(void)
{
    uint16_t hw_revision;
    uint32_t vdce_revision;

    hw_revision = (uint16_t)READ_16L(PRDNAME_ADDR);

    if((uint32_t)hw_revision < LOC_PRDNAME_THRESHOLD)
    {
        vdce_revision = LOC_VDCE_REVISION_1;
    }
    else
    {
        vdce_revision = LOC_VDCE_REVISION_2;
    }

    return vdce_revision;
}

/*******************************************************************************
  Function: R_VDCE_Sys_IntcInit
*/

r_vdce_Error_t R_VDCE_Sys_IntcInit(const uint32_t Unit)
{
    return loc_IntcCtrlAll(Unit, 1);
}

/*******************************************************************************
  Function: R_VDCE_Sys_IntcDeinit
*/

r_vdce_Error_t R_VDCE_Sys_IntcDeinit(const uint32_t Unit)
{
    return loc_IntcCtrlAll(Unit, 0);
}

/***************************************************************************
  Function: R_VDCE_Sys_IntcEnable
*/
r_vdce_Error_t R_VDCE_Sys_IntcEnable(const uint32_t       Unit,
                                     const r_dev_IntSel_t IntSel)
{
    return loc_IntcCtrl(Unit, IntSel, 1);
}

/***************************************************************************
  Function: R_VDCE_Sys_IntcDisable
*/
r_vdce_Error_t R_VDCE_Sys_IntcDisable(const uint32_t       Unit,
                                      const r_dev_IntSel_t IntSel)
{
    return loc_IntcCtrl(Unit, IntSel, 0);
}

/*******************************************************************************
  Function: R_VDCE_Sys_PortInit
*/

r_vdce_Error_t R_VDCE_Sys_PortInit(const uint32_t Unit)
{
    return R_VDCE_ERR_OK;
}

/*******************************************************************************
  Function: R_VDCE_Sys_PortDeInit
*/

r_vdce_Error_t R_VDCE_Sys_PortDeInit(const uint32_t Unit)
{
    return R_VDCE_ERR_OK;
}

/*******************************************************************************
  Function: R_VDCE_Sys_HsyncActLevelSet
*/
uint32_t R_VDCE_Sys_HsyncActLevelSet(const uint32_t Unit, const int32_t Level)
{

    /* HS Sync inversion is handled inside VDCE macro */
    return 0;
}

/*******************************************************************************
  Function: R_VDCE_Sys_VsyncActLevelSet

*/
uint32_t R_VDCE_Sys_VsyncActLevelSet(const uint32_t Unit, const int32_t Level)
{

    /* VS Sync inversion is handled inside VDCE macro */
    return 0;
}

/*******************************************************************************
  Function: R_VDCE_Sys_ClockActEdgeSet
*/
uint32_t R_VDCE_Sys_ClockActEdgeSet(const uint32_t Unit, const int32_t Edge) 
{
    uint32_t val;

    if (Unit < R_VDCE_MACRO_NUM)
    {
        if (Unit == 0u)
        {
#if USE_VDCE_RSDS
            if(0u != (uint32_t)Edge)
            {
                /* Clock inversion is handled by phase shift in case of RSDS. */
                /* This is not implemented yet. */
                while (1) 
                {
                    /* T.B.D */
                    /* error: loop forever */
                }
            }
#else
            /* Use port logic (PINV) to invert polarity of VDCE0_VO_CLKP signal on P45_0 */
            val = R_DEV_READ_REG(16, LOC_PINV45);
            if(0u == (uint32_t)Edge)
            {
                val &= ~(uint32_t)LOC_MASK_0X00000001;
            }
            else
            {
                val |= LOC_MASK_0X00000001;
            }
            R_DEV_WRITE_PROT_REG(LOC_PPCMD45, LOC_PINV45, val);
#endif
        }
        if (Unit == 1u)
        {
            /* Use port logic (PINV) to invert polarity of VDCE1_VO_CLK signal on P47_8 */
            val = R_DEV_READ_REG(16, LOC_PINV47);
            if(0u == (uint32_t)Edge)
            {
                val &= ~(uint32_t)LOC_MASK_0X00000100;
            }
            else
            {
                val |= LOC_MASK_0X00000100;
            }
            R_DEV_WRITE_PROT_REG(LOC_PPCMD47, LOC_PINV47, val);
        }
    }

    /* Clock inversion is NOT handled inside VDCE macro */
    return 1;
}

/*******************************************************************************
  Function: R_VDCE_Sys_DesyncActLevelSet
*/
uint32_t R_VDCE_Sys_DesyncActLevelSet(const uint32_t Unit, const int32_t Level) 
{
    /* DE Sync inversion is handled inside VDCE macro */
    return 0;
}


/*******************************************************************************
  Section: VDCE Macro Driver OS interface

  Comments see: <vdce_sys.h>
*/

/*******************************************************************************
  Function: R_VDCE_Sys_Lock
*/

r_vdce_Error_t R_VDCE_Sys_Lock(const uint32_t Unit)
{
    return R_VDCE_ERR_OK;
}

/*******************************************************************************
  Function: R_VDCE_Sys_Unlock
*/

r_vdce_Error_t R_VDCE_Sys_Unlock(const uint32_t Unit)
{
    return R_VDCE_ERR_OK;
}

/*******************************************************************************
  Function: R_VDCE_Sys_AllLock
*/

r_vdce_Error_t R_VDCE_Sys_AllLock(void)
{
    return R_VDCE_ERR_OK;
}

/*******************************************************************************
  Function: R_VDCE_Sys_AllUnlock
*/

r_vdce_Error_t R_VDCE_Sys_AllUnlock(void)
{
    return R_VDCE_ERR_OK;
}

/*******************************************************************************
  Function: R_VDCE_Sys_VIChannelCheck
*/
r_vdce_Error_t R_VDCE_Sys_VIChannelCheck(const uint32_t Unit)
{
    r_vdce_Error_t  ret    = R_VDCE_ERR_OK;
    r_dev_Device_t  device = loc_GetDev();

    if (device <= R_DEV_R7F701403)       /* D1Lx */
    {
        ret = R_VDCE_ERR_NOT_ACCEPTABLE;
    }
    else if (device <= R_DEV_R7F701407)  /* D1M1x */
    {
        /* All units are available */
    }
    else if (device <= R_DEV_R7F701410)  /* D1M2 */
    {
        if (Unit == R_VDCE_UNIT0)
        {
            ret = R_VDCE_ERR_RANGE_UNIT;
        }
    }
    else if (device <= R_DEV_R7F701412)  /* D1M2H */
    {
        /* All units are available */
    }
    else                                 /* Unknown device */
    {
        ret = R_VDCE_ERR_NOT_ACCEPTABLE;
    }
    return ret;
}

/***********************************************************
  Function: R_VDCE_Sys_MaxResolutionGet
*/

r_vdce_Error_t R_VDCE_Sys_MaxResolutionGet ( uint32_t* ResolutionHmax,
                                             uint32_t* ResolutionVmax )
{
    r_vdce_Error_t ret = R_VDCE_ERR_OK;
    r_dev_Device_t  device  =  loc_GetDev(); 

    if ((R_NULL != ResolutionHmax) && (R_NULL != ResolutionVmax))
    {
        *ResolutionHmax = 0;
        *ResolutionVmax = 0;
        if (device <= R_DEV_R7F701403)       /* D1Lx */
        {
            *ResolutionHmax = 480;
            *ResolutionVmax = 320;
        }
        else if (device <= R_DEV_R7F701407)  /* D1M1x */
        {
            *ResolutionHmax = 1024;
            *ResolutionVmax = 1024;
        }
        else if (device <= R_DEV_R7F701412)  /* D1M2x */
        {
            *ResolutionHmax = 1280;
            *ResolutionVmax = 1024;
        }
        else                                 /* Unknown device */
        {
            /* Do nothing */
        }
    }
    else
    {
        ret = R_VDCE_ERR_PARAM_INCORRECT;
    }

    return ret;
}

/***********************************************************
  Function: R_VDCE_Sys_DeviceInfoGet
*/

r_vdce_Error_t R_VDCE_Sys_DeviceInfoGet(r_vdce_DeviceInfo_t* DevInfo)
{
    r_vdce_Error_t ret = R_VDCE_ERR_OK;
    r_dev_Device_t device = loc_GetDev(); 

    if (R_NULL != DevInfo)
    {
        DevInfo->Device       = device;
        DevInfo->VIWithDataEn = R_FALSE;
        
        if (device <= R_DEV_R7F701407)  /* D1Lx, D1M1x */
        {
            DevInfo->VIWithDataEn = R_FALSE;
        }
        else if (device <= R_DEV_R7F701412)  /* D1M2x */
        {
            DevInfo->VIWithDataEn = R_TRUE;
        }
        else 
        {
            /* Do nothing */
        }
    }
    else
    {
        ret = R_VDCE_ERR_PARAM_INCORRECT;
    }

    return ret;
}

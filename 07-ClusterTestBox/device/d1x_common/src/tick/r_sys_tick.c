/*
****************************************************************************
PROJECT : VLIB
FILE ID : $Id: r_sys_tick.c 7402 2016-01-27 15:43:39Z florian.zimmermann $
AUTHOR  : $Author: florian.zimmermann $
============================================================================ 
DESCRIPTION
TICK driver - device dependent part for D1x
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

#include "r_typedefs.h"
#include "r_tick_api.h"
#include "r_tick_sys.h"
#include "r_config_tick.h"
#include "r_ostm_api.h"
#include "r_dev_api.h"

#pragma ghs startnomisra
#include "v800_ghs.h"   /* __GETSR() */
#pragma ghs endnomisra

#if defined (R_DBG_PRINT_DEV_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#endif

/*******************************************************************************
  Section: Local Variables 
*/
static uint32_t loc_CmpVal[R_TICK_MACRO_NUM]; 

/*******************************************************************************
  Section: Local Functions 
*/
static void loc_OstmTimeTick(void);
static uint32_t loc_CheckInt(uint32_t Unit);

/*******************************************************************************
  Function: loc_IsrTimeTick

  ISR for Time Tick via OSTM
  
  Parameters:
  void

  Returns:
  none
*/
static void loc_OstmTimeTick(void)
{
    R_TICK_Isr(0);
}

/*******************************************************************************
  Function: loc_CheckInt

  For time measurements when the DI bit is set.
  The return value is used to locally increment timeMS by 1.
  
  Parameters:
  Unit       - Instance number
  
  Returns:
  uint32_t
*/

static uint32_t loc_CheckInt(uint32_t Unit) 
{
    if (0 == Unit)
    { 
        uint32_t locSR = __GETSR();
        uint32_t locID = (locSR & (1uL << 5u));
        
        /* Interrupts are disabled, we have to take care of the overflow ourselves */
        if (locID != 0)
        {
            uint8_t int_flag = R_DEV_IntGetFlag((r_dev_IntSel_t)(R_DEV_INT_OSTM0 + Unit));
            /* A timer interrupt is pending? Handle it! */
            if (int_flag != 0)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
    else 
    {
        R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "Only one tick instance supported at the moment!");  
    }
    
    return 0;
}



/* Select used OSTM unit */
#define LOC_OSTM_UNIT 0u 

/*******************************************************************************
  Title: TICK Support Functions

  Implementation of the TICK support functions for the D1x device 
*/


/*******************************************************************************
  Function: R_SYS_TICK_HardwareInit
*/
r_tick_Error_t R_SYS_TICK_HardwareInit(uint32_t Unit) 
{
    
    r_ostm_Parameter_t ostm;
    if (0 == Unit)
    { 
        ostm.Mode       = R_OSTM_INTERVAL; 
        /*set compare value for a 1ms interrupt */
        ostm.CmpVal     = R_OSTM_GetClock(LOC_OSTM_UNIT) / 1000; 
        if (0u == ostm.CmpVal) 
        {
            /*ERROR */
            for ( ; ; ){} 
        }
        loc_CmpVal[0]   = ostm.CmpVal; 
        ostm.IntCtl     = R_OSTM_START_INT_DISABLE; 
        
        while (R_OSTM_Init(LOC_OSTM_UNIT, &ostm) != R_OSTM_ERR_OK) 
        {
            /* error init OSTM*/
        }
        R_OSTM_SetIsrCallback(LOC_OSTM_UNIT, R_OSTM_INT_TINT, &loc_OstmTimeTick);
        R_OSTM_EnableInt(LOC_OSTM_UNIT, R_OSTM_INT_TINT);
        /* Start OSTM */
        R_OSTM_Start(LOC_OSTM_UNIT);
    }
    else 
    {
        R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "Only one tick instance supported at the moment!");  
    }    
    return R_TICK_ERR_OK;
}


/*******************************************************************************
  Function: R_SYS_TICK_HardwareDeInit
*/
r_tick_Error_t R_SYS_TICK_HardwareDeInit(uint32_t Unit) 
{
    
    if (0 == Unit)
    { 
        /* disable OSTM */
        R_OSTM_Stop(LOC_OSTM_UNIT);
        R_OSTM_DisableInt(LOC_OSTM_UNIT, R_OSTM_INT_TINT);
    }
    else 
    {
        R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "Only one tick instance supported at the moment!");  
    }    
    return R_TICK_ERR_OK;
}


/*******************************************************************************
  Function: R_TICK_EnableInt
*/
void R_SYS_TICK_EnableInt(uint32_t Unit) 
{
    if (0 == Unit)
    { 
        R_OSTM_EnableInt(LOC_OSTM_UNIT, R_OSTM_INT_TINT);
    }
    else 
    {
        R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "Only one tick instance supported at the moment!");  
    }    
    
}


/*******************************************************************************
  Function: R_TICK_DisableInt
*/
void R_SYS_TICK_DisableInt(uint32_t Unit) 
{
    if (0 == Unit)
    { 
        R_OSTM_DisableInt(LOC_OSTM_UNIT, R_OSTM_INT_TINT);
    }
    else 
    {
        R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "Only one tick instance supported at the moment!");  
    }    
    
}


/*******************************************************************************
  Function: R_SYS_TICK_GetTimeUS
*/

 uint32_t R_SYS_TICK_GetTimeUS(uint32_t Unit, uint32_t TimeMS)
 {
    uint32_t timeUs;
    if (0 == Unit)
    { 
        /* CmpVal is the amount of ticks required for 1ms */
        /* CmpVal = Freq/1000 =>  Freq = CmpVal * 1000   */
        /* time   = counter / Freq */
        /* time   = counter / (CmpVal * 1000)  */
        /* we are counting down */
        /* time   = (CmpVal - CntVal)/ (CmpVal * 1000) */
        /* timeUs = ((CmpVal - CntVal)*1000000)/ (CmpVal * 1000) */
        /* Finally we get: */
        timeUs = ((loc_CmpVal[0] - R_OSTM_GetCounterValue(LOC_OSTM_UNIT)) *1000)/loc_CmpVal[0]; 
        timeUs = ((TimeMS * 1000) + timeUs);

        /* Check if pending interrupt while interrutps are MASKED or DISABLED */
        /* The callee <R_TICK_GetTimeUS> samples timeMS beforehand.
           But it has a built-in retry mechanism in case a time overflow occurred after sampling within this call here.
           This interrupt check is placed as far away from the sampling point of the callee as possible 
           in order to catch not yet processed timer overruns */
        if (loc_CheckInt(Unit) != 0)
        {
            timeUs = ((loc_CmpVal[0] - R_OSTM_GetCounterValue(LOC_OSTM_UNIT)) *1000)/loc_CmpVal[0]; 
            timeUs = (( (TimeMS+1) * 1000) + timeUs);
        }
    }
    else 
    {
        R_DEV_ERROR(Unit, R_DEV_ERR_UNIT, "Only one tick instance supported at the moment!");  
    }    

    return (uint32_t) timeUs;
}


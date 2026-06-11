/*
****************************************************************************
PROJECT : TAUJ driver
AUTHOR  : Michael Golczewski
FILE    : $Id: r_tauj_main.c 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Driver for TAUJ macro
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

****************************************************************************
*/

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
  Title: Main Module

  Implementation of the API and other global functions.
*/


/***********************************************************
  Section: Includes
*/
#include "r_typedefs.h"
#include "r_config_tauj.h"
#include "r_tauj_regs.h"
#include "r_tauj_api.h"
#define  R_TAUJ_MAIN_GLOBALS_
#include "r_tauj_main.h"
#include "r_tauj_sys.h"


/***********************************************************
  Section: Local Defines
*/

/* Timer interval */
#define LOC_TIME_INTERVAL_0                 (0x0000)
#define LOC_TIME_INTERVAL_1                 (0x0001)
#define LOC_TIME_INTERVAL_2                 (0x0002)
#define LOC_TIME_INTERVAL_3                 (0x0003)
#define LOC_TIME_INTERVAL_4                 (0x0004)
#define LOC_TIME_INTERVAL_5                 (0x0005)
#define LOC_TIME_INTERVAL_6                 (0x0006)
#define LOC_TIME_INTERVAL_7                 (0x0007)
#define LOC_TIME_INTERVAL_8                 (0x0008)
#define LOC_TIME_INTERVAL_9                 (0x0009)
#define LOC_TIME_INTERVAL_10                (0x000A)
#define LOC_TIME_INTERVAL_11                (0x000B)
#define LOC_TIME_INTERVAL_12                (0x000C)
#define LOC_TIME_INTERVAL_13                (0x000D)
#define LOC_TIME_INTERVAL_14                (0x000E)
#define LOC_TIME_INTERVAL_15                (0x000F)

/* clock source */
#define LOC_CK_0                            (0x00)
#define LOC_CK_1                            (0x01)
#define LOC_CK_2                            (0x02)
#define LOC_CK_3                            (0x03)

/* Mode */
#define LOC_INTERVAL_TIMER_MODE             (0x00)
#define LOC_RFU_JUDGE_MODE                  (0x01)
#define LOC_CAPTURE_MODE                    (0x02)
#define LOC_RFU_EVENT_COUNT_MODE            (0x03)
#define LOC_ONE_COUNT_MODE                  (0x04)
#define LOC_RFU_TRIGGER_START_MODE          (0x05)
#define LOC_CAPTURE_ONE_COUNT_MODE          (0x06)
#define LOC_RFU_JUDGE_ONE_COUNT_MODE        (0x07)
#define LOC_RFU_INTERVAL_ONE_COUNT_MODE     (0x08)
#define LOC_RFU_UP_DOWN_COUNT_MODE          (0x09)
#define LOC_RFU_PULSE_ONE_COUNT_MODE        (0x0A)
#define LOC_COUNT_CAPTURE_MODE              (0x0B)
#define LOC_GATE_COUNT_MODE                 (0x0C)
#define LOC_CAPTURE_GATE_COUNT_MODE         (0x0D)

#define LOC_NONE                            (0x00)
#define LOC_VALID_EDGE_TIN                  (0x01)
#define LOC_VALID_EITHER_TIN                (0x02)
#define LOC_RFU_START_TRIG_FROM_MASTER      (0x03)
#define LOC_INT_FROM_MASTER                 (0x04)
#define LOC_RFU_INT_FROM_UPPER              (0x05)
#define LOC_RFU_TOUT_DEAD_TIME              (0x06)
#define LOC_RFU_UP_DOWN_FROM_MASTER         (0x07)

#define LOC_COUNT_CLK_PRESCALER             (0x00)
#define LOC_COUNT_CLK_TIN                   (0x01)
#define LOC_COUNT_CLK_RFU                   (0x02)
#define LOC_COUNT_CLK_MASTER_INT_RFU        (0x03)

#define LOC_CHANNEL_ALL                     (0x000F)

#define LOC_FALLING_EDGE                    (0x00)
#define LOC_RISING_EDGE                     (0x01)
#define LOC_BOTH_LOW_WIDTH                  (0x02)
#define LOC_BOTH_HIGH_WIDTH                 (0x03)

#define LOC_NULL 0

/***********************************************************
  Section: Local variables
*/


/***********************************************************
  Section: Local Functions
*/
static uint16_t loc_GetChannelID(r_tauj_Channel_t Channel) ; 
static uint32_t loc_GetChannelAddress(r_tauj_Channel_t Channel); 
static void loc_InitRegs(uint32_t Unit); 
 
/***********************************************************
  Variable: _UserErrorHandler

  A function pointer to a user callback (Applications error handler)

  The value is assigned by the application using the driver
*/

static void (*_UserErrorHandler )(uint32_t Unit, uint32_t Error) = LOC_NULL;



void R_TAUJ_ErrorHandler(uint32_t Unit, r_tauj_Error_t Error) 
{
    if (_UserErrorHandler != LOC_NULL) 
    {      /* we've got a registered error handler here ...*/
        _UserErrorHandler(Unit, Error);      /* ... and we gonna use it!*/
    } 
    else 
    {
        for (;;)
        {                          /* we don't have a handler */
        }                                    /* so, let's wait here */
    }
}


/***********************************************************
  Function: loc_GetChannelID

  converts the channel from an enum to an uint16_t

  Parameters:
  Channel        - TAUJ channel number <r_tauj_Channel_t>

   Returns:
  u16_channel    - uint16_t version of the enum

*/

static uint16_t loc_GetChannelID(r_tauj_Channel_t Channel) 
{
    uint16_t u16_channel = 0x0000;

    if (Channel == R_TAUJ_CHANNEL_0) 
    {
        u16_channel = 0x0000;
    } 
    else if (Channel == R_TAUJ_CHANNEL_1) 
    {
        u16_channel = 0x0001;
    } 
    else if ( Channel == R_TAUJ_CHANNEL_2) 
    {
        u16_channel = 0x0002;
    } 
    else if ( Channel == R_TAUJ_CHANNEL_3) 
    {
        u16_channel = 0x0003;
    } 
    else 
    {
        /* error */
        u16_channel = 0xFFFF;
    }

    return u16_channel;
}


/***********************************************************
  Function: loc_GetChannelAddress

  converts the channel to an offset address and assumes registers
  are spaced by an equal offset of 4 bytes

  Parameters:
  Channel        - TAUJ channel number <r_tauj_Channel_t>

   Returns:
  ChannelAddress - Address offset of four added to each channel

*/

static uint32_t loc_GetChannelAddress(r_tauj_Channel_t Channel) 
{
    uint32_t ChannelAddress = 0;

    if (Channel == R_TAUJ_CHANNEL_0) 
    {
        ChannelAddress = 0;
    } 
    else if ( Channel == R_TAUJ_CHANNEL_1) 
    {
        ChannelAddress = 1 * 4;
    } 
    else if ( Channel == R_TAUJ_CHANNEL_2) 
    {
        ChannelAddress = 2 * 4;
    } 
    else if ( Channel == R_TAUJ_CHANNEL_3) 
    {
        ChannelAddress = 3 * 4;
    } 
    else 
    {
        /* error */
        ChannelAddress = 0xFFFFFFFFu;
    }

    return ChannelAddress;
}

#ifndef R_TAUJ_LIMIT_INIT 
/***********************************************************
  Function: loc_InitRegs

  Updates all the writeable registers in the TAUJ with their default
  reset values

  Parameters:
  Channel        - TAUJ channel number <r_tauj_Channel_t>

   Returns:
   none

*/
static void loc_InitRegs(uint32_t Unit) 
{
    uint32_t i              = 0;
    uint32_t base_address   = 0;
    uint32_t offset_address = 0;

    base_address = R_TAUJ_Dev[Unit].BaseAddr;

    /* channels should be stopped first */
    /* update the TT Channel Stop Trigger Register */
    R_TAUJ_WRITE16(base_address + R_TAUJ0_TT, 0xFFFF);

    /* update the TOE Channel Output Enable Register*/
    R_TAUJ_WRITE16(base_address + R_TAUJ0_TOE, 0);

    for(i = 0; i < R_TAUJ_NUM_CHANNELS; i++) 
    {
        /* update the CMOR Channel Mode OS Register */
        offset_address = R_TAUJ0_CMOR0 + loc_GetChannelAddress((r_tauj_Channel_t) i);
        R_TAUJ_WRITE16(base_address + offset_address, 0);

        /* update the CDR Channel Data Register */
        offset_address = R_TAUJ0_CDR0 + loc_GetChannelAddress((r_tauj_Channel_t) i);
        R_TAUJ_WRITE32(base_address + offset_address, 0);

        /* update the CMUR TAU Channel Mode User Register */
        offset_address = R_TAUJ0_CMUR0 + loc_GetChannelAddress((r_tauj_Channel_t) i);
        R_TAUJ_WRITE16(base_address + offset_address, 0);

        /* update the CSC Channel Status Clear Register*/
        offset_address = R_TAUJ0_CSC0 + loc_GetChannelAddress((r_tauj_Channel_t) i);
        R_TAUJ_WRITE16(base_address + offset_address, 1);
    }

    /* update the TPS Prescaler Clock Select Register */
    R_TAUJ_WRITE16(base_address + R_TAUJ0_TPS, 0xFFFF);

    /* update the BRS Prescaler Baud Rate Setting Register */
    R_TAUJ_WRITE16(base_address + R_TAUJ0_BRS, 0x00FF);

    /* update the TO Channel Output Register */
    R_TAUJ_WRITE16(base_address + R_TAUJ0_TO, 0);

    /* update the RDE Channel Reload Data Enable Register */
    R_TAUJ_WRITE16(base_address + R_TAUJ0_RDE, 0);

    /* update the RDM Channel Reload Data Mode Register */
    R_TAUJ_WRITE16(base_address + R_TAUJ0_RDM, 0);

    /* update the RDT CChannel Reload Data Trigger Register */
    R_TAUJ_WRITE16(base_address + R_TAUJ0_RDT, 0);

    /* update the TS Channel Start Trigger Register */
    R_TAUJ_WRITE16(base_address + R_TAUJ0_TS, 0);

    /* update the TOM Channel Output Mode Register */
    R_TAUJ_WRITE16(base_address + R_TAUJ0_TOM, 0);

    /* update the TOC Channel Output Configuration Register */
    R_TAUJ_WRITE16(base_address + R_TAUJ0_TOC, 0);

    /* update the TOL Channel Output Level Register */
    R_TAUJ_WRITE16(base_address + R_TAUJ0_TOL, 0);

    /* update the EMU Register */
    R_TAUJ_WRITE16(base_address + R_TAUJ0_EMU, LOC_SVSTOP);
}
#endif

/***********************************************************
  Section: Global API Functions
*/

/***********************************************************
  Function: R_TAUJ_Init
*/

r_tauj_Error_t R_TAUJ_Init(uint32_t Unit) 
{
    r_tauj_Error_t error_mode = R_TAUJ_ERR_OK;

    /* check the instance is allowed */
    if ( Unit > (R_TAUJ_MACRO_NUM - 1)) 
    {
        /* instance too high */
        error_mode = R_TAUJ_ERR_INSTANCE;
    } 
    else 
    {
        /* update the BaseAddr */
        R_TAUJ_Dev[Unit].BaseAddr = R_TAUJ_Sys_GetBaseAddr(Unit);
#ifndef TAUJ_LIMIT_INIT 
        loc_InitRegs(Unit);
#endif
        /* Set the default callbacks for all interrupts  */
        R_TAUJ_Dev[Unit].IsrCallback[R_TAUJ_INT_CH0]   = LOC_NULL;
        R_TAUJ_Dev[Unit].IsrCallback[R_TAUJ_INT_CH1]   = LOC_NULL;
        R_TAUJ_Dev[Unit].IsrCallback[R_TAUJ_INT_CH2]   = LOC_NULL;
        R_TAUJ_Dev[Unit].IsrCallback[R_TAUJ_INT_CH3]   = LOC_NULL;

        /* Setup device interrupt controller */
        R_TAUJ_Sys_IntCInit(Unit);
    }

    return error_mode;
}


/***********************************************************
  Function: R_TAUJ_InitChannel
*/

r_tauj_Error_t R_TAUJ_InitChannel(uint32_t Unit, r_tauj_Channel_t Channel) 
{
    r_tauj_Error_t  error_mode      = R_TAUJ_ERR_OK;
    uint16_t        ichannel        = 0;
    uint16_t        u16_temp        = 0;
    uint32_t        base_address    = 0;
    uint32_t        offset_address  = 0;

    /* check the instance is allowed */
    if (Unit > (R_TAUJ_MACRO_NUM - 1)) 
    {
        /* instance too high */
        error_mode = R_TAUJ_ERR_INSTANCE;
    } 
    else 
    {
        ichannel = loc_GetChannelID(Channel);

        /* make sure the channel number is in range */
        if ( ichannel < R_TAUJ_NUM_CHANNELS) 
        {
            base_address = R_TAUJ_Dev[Unit].BaseAddr;

            /* update the CDR */
            offset_address = R_TAUJ0_CDR0 + loc_GetChannelAddress(Channel);
            R_TAUJ_WRITE32(base_address + offset_address, 0);

            /* update the CMOR */
            offset_address = R_TAUJ0_CMOR0 + loc_GetChannelAddress(Channel);
            R_TAUJ_WRITE16(base_address + offset_address, 0);

            /* update the CMUR */
            offset_address = R_TAUJ0_CMUR0 + loc_GetChannelAddress(Channel);
            R_TAUJ_WRITE16(base_address + offset_address, 0);

            /* update the CSC */
            offset_address = R_TAUJ0_CSC0 + loc_GetChannelAddress(Channel);
            R_TAUJ_WRITE16(base_address + offset_address, 0);

            /* update the TS */
            u16_temp = R_TAUJ_READ16(base_address + R_TAUJ0_TS);
            R_TAUJ_CLR_BIT(u16_temp, Channel);
            R_TAUJ_WRITE16(base_address + R_TAUJ0_TS, 0);

            /* update the TT */
            u16_temp = R_TAUJ_READ16(base_address + R_TAUJ0_TT);
            R_TAUJ_CLR_BIT(u16_temp, Channel);
            R_TAUJ_WRITE16(base_address + R_TAUJ0_TT, 0);

            /* update the TO */
            u16_temp = R_TAUJ_READ16(base_address + R_TAUJ0_TO);
            R_TAUJ_CLR_BIT(u16_temp, Channel);
            R_TAUJ_WRITE16(base_address + R_TAUJ0_TO, 0);

            /* update the TOE */
            u16_temp = R_TAUJ_READ16(base_address + R_TAUJ0_TOE);
            R_TAUJ_CLR_BIT(u16_temp, Channel);
            R_TAUJ_WRITE16(base_address + R_TAUJ0_TOE, 0);

            /* update the TOM */
            u16_temp = R_TAUJ_READ16(base_address + R_TAUJ0_TOM);
            R_TAUJ_CLR_BIT(u16_temp, Channel);
            R_TAUJ_WRITE16(base_address + R_TAUJ0_TOM, 0);

            /* update the TOC */
            u16_temp = R_TAUJ_READ16(base_address + R_TAUJ0_TOC);
            R_TAUJ_CLR_BIT(u16_temp, Channel);
            R_TAUJ_WRITE16(base_address + R_TAUJ0_TOC, 0);

            /* update the TOL */
            u16_temp = R_TAUJ_READ16(base_address + R_TAUJ0_TOL);
            R_TAUJ_CLR_BIT(u16_temp, Channel);
            R_TAUJ_WRITE16(base_address + R_TAUJ0_TOL, 0);

            /* update the RDE */
            u16_temp = R_TAUJ_READ16(base_address + R_TAUJ0_RDE);
            R_TAUJ_CLR_BIT(u16_temp, Channel);
            R_TAUJ_WRITE16(base_address + R_TAUJ0_RDE, 0);

            /* update the RDM */
            u16_temp = R_TAUJ_READ16(base_address + R_TAUJ0_RDM);
            R_TAUJ_CLR_BIT(u16_temp, Channel);
            R_TAUJ_WRITE16(base_address + R_TAUJ0_RDM, 0);

            /* update the RDT */
            u16_temp = R_TAUJ_READ16(base_address + R_TAUJ0_RDT);
            R_TAUJ_CLR_BIT(u16_temp, Channel);
            R_TAUJ_WRITE16(base_address + R_TAUJ0_RDT, 0);

        }
        else 
        {
            /* error */
            error_mode = R_TAUJ_ERR_RANGE;
        }
    }

    return error_mode;
}


/***********************************************************
  Function: R_TAUJ_SetClockPeriod
*/

r_tauj_Error_t R_TAUJ_SetClockPeriod(
        uint32_t Unit,
        r_tauj_Clock_t Clock,
        r_tauj_TimeInterval_t TimeInterval)
{
    uint8_t ucTimeInterval = 0;
    r_tauj_Error_t error_mode = R_TAUJ_ERR_OK;
    uint32_t base_address = 0;
    r_tauj_RegTps_t r_tauj_RegTps;

    /* check the instance is allowed */
    if (Unit > (R_TAUJ_MACRO_NUM - 1)) 
    {
        /* instance too high */
        error_mode = R_TAUJ_ERR_INSTANCE;
    }
    else 
    {
        /* ok, update the TPS register value */
        base_address = R_TAUJ_Dev[Unit].BaseAddr;

        /* get the current value */
        r_tauj_RegTps.Word = R_TAUJ_READ16(base_address + R_TAUJ0_TPS);

        /* timer interval */
        if ( TimeInterval == R_TAUJ_TIME_INTERVAL_0) 
        {
            ucTimeInterval = LOC_TIME_INTERVAL_0;
        } 
        else if ( TimeInterval == R_TAUJ_TIME_INTERVAL_1) 
        {
            ucTimeInterval = LOC_TIME_INTERVAL_1;
        } 
        else if ( TimeInterval == R_TAUJ_TIME_INTERVAL_2) 
        {
            ucTimeInterval = LOC_TIME_INTERVAL_2;
        } 
        else if ( TimeInterval == R_TAUJ_TIME_INTERVAL_3) 
        {
            ucTimeInterval = LOC_TIME_INTERVAL_3;
        } 
        else if ( TimeInterval == R_TAUJ_TIME_INTERVAL_4) 
        {
            ucTimeInterval = LOC_TIME_INTERVAL_4;
        } 
        else if ( TimeInterval == R_TAUJ_TIME_INTERVAL_5) 
        {
            ucTimeInterval = LOC_TIME_INTERVAL_5;
        } 
        else if ( TimeInterval == R_TAUJ_TIME_INTERVAL_6) 
        {
            ucTimeInterval = LOC_TIME_INTERVAL_6;
        } 
        else if ( TimeInterval == R_TAUJ_TIME_INTERVAL_7) 
        {
            ucTimeInterval = LOC_TIME_INTERVAL_7;
        } 
        else if ( TimeInterval == R_TAUJ_TIME_INTERVAL_8) 
        {
            ucTimeInterval = LOC_TIME_INTERVAL_8;
        } 
        else if ( TimeInterval == R_TAUJ_TIME_INTERVAL_9) 
        {
            ucTimeInterval = LOC_TIME_INTERVAL_9;
        } 
        else if ( TimeInterval == R_TAUJ_TIME_INTERVAL_10) 
        {
            ucTimeInterval = LOC_TIME_INTERVAL_10;
        } 
        else if ( TimeInterval == R_TAUJ_TIME_INTERVAL_11) 
        {
            ucTimeInterval = LOC_TIME_INTERVAL_11;
        } 
        else if ( TimeInterval == R_TAUJ_TIME_INTERVAL_12) 
        {
            ucTimeInterval = LOC_TIME_INTERVAL_12;
        } 
        else if ( TimeInterval == R_TAUJ_TIME_INTERVAL_13) 
        {
            ucTimeInterval = LOC_TIME_INTERVAL_13;
        } 
        else if ( TimeInterval == R_TAUJ_TIME_INTERVAL_14) 
        {
            ucTimeInterval = LOC_TIME_INTERVAL_14;
        } 
        else if ( TimeInterval == R_TAUJ_TIME_INTERVAL_15) 
        {
            ucTimeInterval = LOC_TIME_INTERVAL_15;
        } 
        else 
        {
            /* error */
            error_mode = R_TAUJ_ERR_RANGE;
        }

        /* if a valid time interval the write the clock */
        if ( error_mode == R_TAUJ_ERR_OK) 
        {
            if ( Clock == R_TAUJ_CK_0) 
            {
                r_tauj_RegTps.Bits.Pcs_Ck0 = ucTimeInterval;
            } 
            else if ( Clock == R_TAUJ_CK_1) 
            {
                r_tauj_RegTps.Bits.Pcs_Ck1 = ucTimeInterval;
            } 
            else if ( Clock == R_TAUJ_CK_2)
            {
                 r_tauj_RegTps.Bits.Pcs_Ck2 = ucTimeInterval;
            } 
            else if ( Clock == R_TAUJ_CK_3) 
            {
                 r_tauj_RegTps.Bits.Pcs_Ck3 = ucTimeInterval;
            } 
            else 
            {
                /* error */
                error_mode = R_TAUJ_ERR_RANGE;
            }
        } 
        else 
        {
            /* error do not write anything, the return value will indicate the error condition */
        }
    }

    /* if no error then update the register */
    if ( error_mode == R_TAUJ_ERR_OK) 
    {
        /* update the TPS Prescaler Clock Select Register */
        R_TAUJ_WRITE16(base_address + R_TAUJ0_TPS, r_tauj_RegTps.Word);
        /* check values! */
        while (R_TAUJ_READ16(base_address + + R_TAUJ0_TPS) != r_tauj_RegTps.Word) 
        {
            R_TAUJ_WRITE16(base_address + R_TAUJ0_TPS, r_tauj_RegTps.Word);
        }           
    }
    return error_mode;
}


/***********************************************************
  Function: R_TAUJ_SetCK3BaudRateClock
*/

r_tauj_Error_t R_TAUJ_SetCK3BaudRateClock(uint32_t Unit, uint8_t Period) 
{
    r_tauj_Error_t error_mode = R_TAUJ_ERR_OK;
    uint32_t base_address = 0;

    /* check the instance is allowed */
    if ( Unit > (R_TAUJ_MACRO_NUM - 1)) 
    {
        /* instance too high */
        error_mode = R_TAUJ_ERR_INSTANCE;
    } 
    else 
    {
        /* ok, update the BRS register value */
        base_address = R_TAUJ_Dev[Unit].BaseAddr;

        /* update the BRS Prescaler Baud Rate Setting Register */
        R_TAUJ_WRITE16(base_address + R_TAUJ0_BRS, Period);
    }

    return error_mode;
}


/***********************************************************
  Function: R_TAUJ_SetClkSource
*/

r_tauj_Error_t R_TAUJ_SetClkSource(uint32_t Unit, r_tauj_Channel_t Channel, r_tauj_Clock_t Clock) 
{
    r_tauj_Error_t error_mode = R_TAUJ_ERR_OK;
    uint32_t base_address = 0;
    uint32_t offset_address = 0;
    r_tauj_RegCmor_t r_tauj_RegCmor;

    /* check the instance is allowed */
    if ( Unit > (R_TAUJ_MACRO_NUM - 1)) 
    {
        /* instance too high */
        error_mode = R_TAUJ_ERR_INSTANCE;
    } 
    else 
    {
        base_address = R_TAUJ_Dev[Unit].BaseAddr;
        offset_address = R_TAUJ0_CMOR0 + loc_GetChannelAddress(Channel);
        r_tauj_RegCmor.Word = R_TAUJ_READ16(base_address + offset_address);

        if ( Clock == R_TAUJ_CK_0) 
        {
            r_tauj_RegCmor.Bits.Cks = LOC_CK_0;
        } 
        else if ( Clock == R_TAUJ_CK_1) 
        {
            r_tauj_RegCmor.Bits.Cks = LOC_CK_1;
        } 
        else if ( Clock == R_TAUJ_CK_2) 
        {
            r_tauj_RegCmor.Bits.Cks = LOC_CK_2;
        } 
        else if ( Clock == R_TAUJ_CK_3) 
        {
            r_tauj_RegCmor.Bits.Cks = LOC_CK_3;
        } 
        else 
        {
            /* error */
            error_mode = R_TAUJ_ERR_RANGE;
        }
    }

    /* if no error then update the register */
    if ( error_mode == R_TAUJ_ERR_OK) 
    {
        /* update the CMOR Channel Mode OS Register */
        R_TAUJ_WRITE16(base_address + offset_address, r_tauj_RegCmor.Word);
                /* check values! */
        while (R_TAUJ_READ16(base_address + offset_address) != r_tauj_RegCmor.Word) 
        {
            R_TAUJ_WRITE16(base_address + offset_address, r_tauj_RegCmor.Word);
        }           
    }
    return error_mode;
}


/***********************************************************
  Function: R_TAUJ_ClearOverflow
*/

r_tauj_Error_t R_TAUJ_ClearOverflow(uint32_t Unit, r_tauj_Channel_t Channel) 
{
    r_tauj_Error_t error_mode = R_TAUJ_ERR_OK;
    uint32_t base_address = 0;
    uint32_t offset_address = 0;

    /* check the instance is allowed */
    if ( Unit > (R_TAUJ_MACRO_NUM - 1)) 
    {
        /* instance too high */
        error_mode = R_TAUJ_ERR_INSTANCE;
    } 
    else 
    {
        if ( Channel == R_TAUJ_CHANNEL_ALL) 
        {
            error_mode = R_TAUJ_ERR_RANGE;
        } 
        else 
        {
            base_address = R_TAUJ_Dev[Unit].BaseAddr;
            offset_address = R_TAUJ0_CSC0 + loc_GetChannelAddress(Channel);
            /* update the CSC Channel Status Clear Register */
            R_TAUJ_WRITE16(base_address + offset_address, 0x0001);
        }
    }

    return error_mode;
}


/***********************************************************
  Function: R_TAUJ_SetMode
*/

r_tauj_Error_t R_TAUJ_SetMode(uint32_t Unit, r_tauj_Channel_t Channel, r_tauj_Mode_t Mode) 
{
    r_tauj_Error_t error_mode = R_TAUJ_ERR_OK;
    uint32_t base_address = 0;
    uint32_t offset_address = 0;
    r_tauj_RegCmor_t r_tauj_RegCmor;

    /* check the instance is allowed */
    if (Unit > (R_TAUJ_MACRO_NUM - 1)) 
    {
        /* instance too high */
        error_mode = R_TAUJ_ERR_INSTANCE;
    } 
    else 
    {
        /* ok, update the CMOR register value */
        base_address = R_TAUJ_Dev[Unit].BaseAddr;
        offset_address = R_TAUJ0_CMOR0 + loc_GetChannelAddress(Channel);
        r_tauj_RegCmor.Word = R_TAUJ_READ16(base_address + offset_address);

        /* timer interval */
        if ( Mode == R_TAUJ_INTERVAL_TIMER_MODE) 
        {
            r_tauj_RegCmor.Bits.Md = LOC_INTERVAL_TIMER_MODE;
        } 
        else if ( Mode == R_TAUJ_RFU_JUDGE_MODE) 
        {
            r_tauj_RegCmor.Bits.Md = LOC_RFU_JUDGE_MODE;
        } 
        else if ( Mode == R_TAUJ_CAPTURE_MODE) 
        {
            r_tauj_RegCmor.Bits.Md = LOC_CAPTURE_MODE;
        } 
        else if ( Mode == R_TAUJ_RFU_EVENT_COUNT_MODE) 
        {
            r_tauj_RegCmor.Bits.Md = LOC_RFU_EVENT_COUNT_MODE;
        } 
        else if ( Mode == R_TAUJ_ONE_COUNT_MODE) 
        {
            r_tauj_RegCmor.Bits.Md = LOC_ONE_COUNT_MODE;
        } 
        else if ( Mode == R_TAUJ_RFU_TRIGGER_START_MODE) 
        {
            r_tauj_RegCmor.Bits.Md = LOC_RFU_TRIGGER_START_MODE;
        } 
        else if ( Mode == R_TAUJ_CAPTURE_ONE_COUNT_MODE) 
        {
            r_tauj_RegCmor.Bits.Md = LOC_CAPTURE_ONE_COUNT_MODE;
        } 
        else if ( Mode == R_TAUJ_RFU_JUDGE_ONE_COUNT_MODE) 
        {
            r_tauj_RegCmor.Bits.Md = LOC_RFU_JUDGE_ONE_COUNT_MODE;
        } 
        else if ( Mode == R_TAUJ_RFU_INTERVAL_ONE_COUNT_MODE) 
        {
            r_tauj_RegCmor.Bits.Md = LOC_RFU_INTERVAL_ONE_COUNT_MODE;
        } 
        else if ( Mode == R_TAUJ_RFU_UP_DOWN_COUNT_MODE) 
        {
            r_tauj_RegCmor.Bits.Md = LOC_RFU_UP_DOWN_COUNT_MODE;
        } 
        else if ( Mode == R_TAUJ_RFU_PULSE_ONE_COUNT_MODE) 
        {
            r_tauj_RegCmor.Bits.Md = LOC_RFU_PULSE_ONE_COUNT_MODE;
        } 
        else if ( Mode == R_TAUJ_RFU_INTERVAL_ONE_COUNT_MODE) 
        {
            r_tauj_RegCmor.Bits.Md = LOC_RFU_INTERVAL_ONE_COUNT_MODE;
        } 
        else if ( Mode == R_TAUJ_COUNT_CAPTURE_MODE) 
        {
            r_tauj_RegCmor.Bits.Md = LOC_COUNT_CAPTURE_MODE;
        } 
        else if ( Mode == R_TAUJ_GATE_COUNT_MODE) 
        {
            r_tauj_RegCmor.Bits.Md = LOC_GATE_COUNT_MODE;
        } 
        else if ( Mode == R_TAUJ_CAPTURE_GATE_COUNT_MODE) 
        {
            r_tauj_RegCmor.Bits.Md = LOC_CAPTURE_GATE_COUNT_MODE;
        } 
        else 
        {
            /* error */
            error_mode = R_TAUJ_ERR_RANGE;
        }
    }

    /* if no error then update the register */
    if (error_mode == R_TAUJ_ERR_OK) 
    {
        /* update the CMOR Channel Mode OS Register */
        base_address = R_TAUJ_Dev[Unit].BaseAddr;
        offset_address = R_TAUJ0_CMOR0 + loc_GetChannelAddress(Channel);
        R_TAUJ_WRITE16(base_address + offset_address, r_tauj_RegCmor.Word);
        /* check values! */
        while (R_TAUJ_READ16(base_address + offset_address) != r_tauj_RegCmor.Word) 
        {
            R_TAUJ_WRITE16(base_address + offset_address, r_tauj_RegCmor.Word);
        }           
    }
    return error_mode;
}


/***********************************************************
  Function: r_tauj_SetCaptureOverflow
*/

r_tauj_Error_t R_TAUJ_SetCaptureOverflow(
    uint32_t Unit,
    r_tauj_Channel_t Channel,
    r_tauj_Overflow_t Overflow)
{
    r_tauj_Error_t error_mode = R_TAUJ_ERR_OK;
    uint32_t base_address = 0;
    uint32_t offset_address = 0;
    r_tauj_RegCmor_t r_tauj_RegCmor;

    /* check the instance is allowed */
    if (Unit > (R_TAUJ_MACRO_NUM - 1)) 
    {
        /* instance too high */
        error_mode = R_TAUJ_ERR_INSTANCE;
    } 
    else 
    {
        /* ok, update the CMOR register value */
        base_address = R_TAUJ_Dev[Unit].BaseAddr;
        offset_address = R_TAUJ0_CMOR0 + loc_GetChannelAddress(Channel);
        r_tauj_RegCmor.Word = R_TAUJ_READ16(base_address + offset_address);

        /* timer interval */
        if ( Overflow == R_TAUJ_OVERFLOW_OPTION_0) 
        {
            r_tauj_RegCmor.Bits.Cos = 0;
        } 
        else if ( Overflow == R_TAUJ_OVERFLOW_OPTION_1) 
        {
            r_tauj_RegCmor.Bits.Cos = 1;
        } 
        else if ( Overflow == R_TAUJ_OVERFLOW_OPTION_2) 
        {
            r_tauj_RegCmor.Bits.Cos = 2;
        } 
        else if ( Overflow == R_TAUJ_OVERFLOW_OPTION_3) 
        {
            r_tauj_RegCmor.Bits.Cos = 3;
        } 
        else 
        {
            /* error */
            error_mode = R_TAUJ_ERR_RANGE;
        }
    }

    /* if no error then update the register */
    if ( error_mode == R_TAUJ_ERR_OK) 
    {
        /* update the CMOR Channel Mode OS Register */
        R_TAUJ_WRITE16(base_address + offset_address, r_tauj_RegCmor.Word);
        /* check values! */
        while (R_TAUJ_READ16(base_address + offset_address) != r_tauj_RegCmor.Word) 
        {
            R_TAUJ_WRITE16(base_address + offset_address, r_tauj_RegCmor.Word);
        }           
    }
    return error_mode;
}


/***********************************************************
  Function: R_TAUJ_EnableIntAtStartUp
*/

r_tauj_Error_t R_TAUJ_EnableIntAtStartUp(
    uint32_t Unit,
    r_tauj_Channel_t Channel,
    uint8_t IntAtStartUp)
{
    r_tauj_Error_t error_mode = R_TAUJ_ERR_OK;
    uint32_t base_address = 0;
    uint32_t offset_address = 0;
    r_tauj_RegCmor_t r_tauj_RegCmor;

    /* check the instance is allowed */
    if ( Unit > (R_TAUJ_MACRO_NUM - 1)) 
    {
        /* instance too high */
        error_mode = R_TAUJ_ERR_INSTANCE;
    } 
    else 
    {
        /* ok, update the CMOR register value */
        base_address = R_TAUJ_Dev[Unit].BaseAddr;
        offset_address = R_TAUJ0_CMOR0 + loc_GetChannelAddress(Channel);
        r_tauj_RegCmor.Word = R_TAUJ_READ16(base_address + offset_address);

        /* timer interval */
        if ( IntAtStartUp == 1) 
        {
            r_tauj_RegCmor.Bits.Md0 = 1;
        } 
        else 
        {
            r_tauj_RegCmor.Bits.Md0 = 0;
        }
    }

    /* if no error then update the register */
    if ( error_mode == R_TAUJ_ERR_OK) 
    {
        /* update the CMOR Channel Mode OS Register */
        base_address = R_TAUJ_Dev[Unit].BaseAddr;
        offset_address = R_TAUJ0_CMOR0 + loc_GetChannelAddress(Channel);
        R_TAUJ_WRITE16(base_address + offset_address, r_tauj_RegCmor.Word);
        /* check values! */
        while (R_TAUJ_READ16(base_address + offset_address) != r_tauj_RegCmor.Word) 
        {
            R_TAUJ_WRITE16(base_address + offset_address, r_tauj_RegCmor.Word);
        }           
    }
    return error_mode;
}


/***********************************************************
  Function: R_TAUJ_SetMaster
*/

r_tauj_Error_t R_TAUJ_SetMaster(uint32_t Unit, r_tauj_Channel_t Channel, uint8_t Master) 
{
    r_tauj_Error_t error_mode = R_TAUJ_ERR_OK;
    uint32_t base_address = 0;
    uint32_t offset_address = 0;
    r_tauj_RegCmor_t r_tauj_RegCmor;

    /* check the instance is allowed */
    if ( Unit > (R_TAUJ_MACRO_NUM - 1)) 
    {
        /* instance too high */
        error_mode = R_TAUJ_ERR_INSTANCE;
    } 
    else 
    {
        /* ok, update the CMOR register value */
        base_address = R_TAUJ_Dev[Unit].BaseAddr;
        offset_address = R_TAUJ0_CMOR0 + loc_GetChannelAddress(Channel);
        r_tauj_RegCmor.Word = R_TAUJ_READ16(base_address + offset_address);

        /* timer interval */
        if ( Master == 1) 
        {
            r_tauj_RegCmor.Bits.Mas = 1;
        } 
        else 
        {
            r_tauj_RegCmor.Bits.Mas = 0;
        }
    }

    /* if no error then update the register */
    if ( error_mode == R_TAUJ_ERR_OK) 
    {
        /* update the CMOR Channel Mode OS Register */
        base_address = R_TAUJ_Dev[Unit].BaseAddr;
        offset_address = R_TAUJ0_CMOR0 + loc_GetChannelAddress(Channel);
        R_TAUJ_WRITE16(base_address + offset_address, r_tauj_RegCmor.Word);
        /* check values! */
        while (R_TAUJ_READ16(base_address + offset_address) != r_tauj_RegCmor.Word) 
        {
            R_TAUJ_WRITE16(base_address + offset_address, r_tauj_RegCmor.Word);
        }           
    }
    return error_mode;
}


/***********************************************************
  Function: R_TAUJ_SetTrigger
*/

r_tauj_Error_t R_TAUJ_SetTrigger(uint32_t Unit, r_tauj_Channel_t Channel, r_tauj_Trigger_t Trigger) 
{
    r_tauj_Error_t error_mode = R_TAUJ_ERR_OK;
    uint32_t base_address = 0;
    uint32_t offset_address = 0;
    r_tauj_RegCmor_t r_tauj_RegCmor;

    /* check the instance is allowed */
    if ( Unit > (R_TAUJ_MACRO_NUM - 1)) 
    {
        /* instance too high */
        error_mode = R_TAUJ_ERR_INSTANCE;
    } 
    else 
    {
        /* ok, update the CMOR register value */
        base_address = R_TAUJ_Dev[Unit].BaseAddr;
        offset_address = R_TAUJ0_CMOR0 + loc_GetChannelAddress(Channel);
        r_tauj_RegCmor.Word = R_TAUJ_READ16(base_address + offset_address);

        /* timer interval */
        if ( Trigger == R_TAUJ_NONE) 
        {
            r_tauj_RegCmor.Bits.Sts = LOC_NONE;
        } 
        else if ( Trigger == R_TAUJ_VALID_EDGE_TIN) 
        {
            r_tauj_RegCmor.Bits.Sts = LOC_VALID_EDGE_TIN;
        } 
        else if ( Trigger == R_TAUJ_VALID_EITHER_TIN) 
        {
            r_tauj_RegCmor.Bits.Sts = LOC_VALID_EITHER_TIN;
        } 
        else if ( Trigger == R_TAUJ_RFU_START_TRIG_FROM_MASTER) 
        {
            r_tauj_RegCmor.Bits.Sts = LOC_RFU_START_TRIG_FROM_MASTER;
        } 
        else if ( Trigger == R_TAUJ_INT_FROM_MASTER) 
        {
            r_tauj_RegCmor.Bits.Sts = LOC_INT_FROM_MASTER;
        } 
        else if ( Trigger == R_TAUJ_RFU_INT_FROM_UPPER) 
        {
            r_tauj_RegCmor.Bits.Sts = LOC_RFU_INT_FROM_UPPER;
        } 
        else if ( Trigger == R_TAUJ_RFU_TOUT_DEAD_TIME) 
        {
            r_tauj_RegCmor.Bits.Sts = LOC_RFU_TOUT_DEAD_TIME;
        } 
        else if ( Trigger == R_TAUJ_RFU_UP_DOWN_FROM_MASTER) 
        {
            r_tauj_RegCmor.Bits.Sts = LOC_RFU_UP_DOWN_FROM_MASTER;
        } 
        else 
        {
            r_tauj_RegCmor.Bits.Sts = 0;
            error_mode = R_TAUJ_ERR_RANGE;
        }
    }

    /* if no error then update the register */
    if ( error_mode == R_TAUJ_ERR_OK) 
    {
        /* update the CMOR Channel Mode OS Register */
        base_address = R_TAUJ_Dev[Unit].BaseAddr;
        offset_address = R_TAUJ0_CMOR0 + loc_GetChannelAddress(Channel);
        R_TAUJ_WRITE16(base_address + offset_address, r_tauj_RegCmor.Word);
        /* check values! */
        while (R_TAUJ_READ16(base_address + offset_address) != r_tauj_RegCmor.Word) 
        {
            R_TAUJ_WRITE16(base_address + offset_address, r_tauj_RegCmor.Word);
        }           
    }
    return error_mode;
}


/***********************************************************
  Function: R_TAUJ_EnableSimultaneousRewrite
*/

r_tauj_Error_t R_TAUJ_EnableSimultaneousRewrite(
    uint32_t Unit,
    r_tauj_Channel_t Channel,
    r_tauj_SimultaneousTrigger_t SimultaneousTriggerSource)
{
    r_tauj_Error_t error_mode = R_TAUJ_ERR_OK;
    uint32_t base_address = 0;
    uint16_t u16_RDE = 0;
    uint16_t u16_channel = 0;

    /* check the instance is allowed */
    if ( Unit > (R_TAUJ_MACRO_NUM - 1)) 
    {
        /* instance too high */
        error_mode = R_TAUJ_ERR_INSTANCE;
    } 
    else 
    {
        /* ok, update the Channel Reload Data Enable Register (RDE) */
        base_address = R_TAUJ_Dev[Unit].BaseAddr;
        u16_RDE = R_TAUJ_READ16(base_address + R_TAUJ0_RDE);

        /* enable all channels */
        if ( Channel == R_TAUJ_CHANNEL_ALL) 
        {
            u16_RDE = LOC_CHANNEL_ALL;
        } 
        else 
        { /* just one channel */
            u16_channel = loc_GetChannelID(Channel);
            R_TAUJ_SET_BIT(u16_RDE, u16_channel);
        }
    }

    /* if no error then update the register */
    if (error_mode == R_TAUJ_ERR_OK) 
    {
        /* update the CMOR Channel Mode OS Register */
        base_address = R_TAUJ_Dev[Unit].BaseAddr;
        R_TAUJ_WRITE16(base_address + R_TAUJ0_RDE, u16_RDE);
    }
    return error_mode;
}


/***********************************************************
  Function: R_TAUJ_EnableSynchronousChannelOperation
*/

r_tauj_Error_t R_TAUJ_EnableSynchronousChannelOperation(uint32_t Unit, r_tauj_Channel_t Channel) 
{
    r_tauj_Error_t error_mode = R_TAUJ_ERR_OK;
    uint32_t base_address = 0;
    uint16_t u16_temp = 0;
    uint16_t u16_channel = 0;

    /* check the instance is allowed */
    if ( Unit > (R_TAUJ_MACRO_NUM - 1)) 
    {
        /* instance too high */
        error_mode = R_TAUJ_ERR_INSTANCE;
    } 
    else 
    {
        /* ok */
        base_address = R_TAUJ_Dev[Unit].BaseAddr;
        u16_temp = R_TAUJ_READ16(base_address + R_TAUJ0_TOM);

        /* enable all channels */
        if ( Channel == R_TAUJ_CHANNEL_ALL) 
        {
            u16_temp = LOC_CHANNEL_ALL;
        } 
        else 
        { /* just one channel */
            u16_channel = loc_GetChannelID(Channel);
            R_TAUJ_SET_BIT(u16_temp, u16_channel);
        }

        /* update the TOM */
        R_TAUJ_WRITE16(base_address + R_TAUJ0_TOM, u16_temp);

        /* verify the register write was succesfull */
        if ( u16_temp != (R_TAUJ_READ16(base_address + R_TAUJ0_TOM))) 
        {
            error_mode = R_TAUJ_ERR_REG_UPDATE_FAIL;
        }
    }
    return error_mode;
}


/***********************************************************
  Function: R_TAUJ_SetCountClock
*/

r_tauj_Error_t R_TAUJ_SetCountClock(
    uint32_t Unit,
    r_tauj_Channel_t Channel,
    r_tauj_CountClock_t CountClock)
{
    r_tauj_Error_t error_mode = R_TAUJ_ERR_OK;
    uint32_t base_address = 0;
    uint32_t offset_address = 0;
    r_tauj_RegCmor_t r_tauj_RegCmor;

    /* check the instance is allowed */
    if ( Unit > (R_TAUJ_MACRO_NUM - 1)) 
    {
        /* instance too high */
        error_mode = R_TAUJ_ERR_INSTANCE;
    } 
    else 
    {
        /* ok, update the CMOR register value */
        base_address = R_TAUJ_Dev[Unit].BaseAddr;
        offset_address = R_TAUJ0_CMOR0 + loc_GetChannelAddress(Channel);
        r_tauj_RegCmor.Word = R_TAUJ_READ16(base_address + offset_address);

        /* timer interval */
        if ( CountClock == R_TAUJ_COUNT_CLK_PRESCALER) 
        {
            r_tauj_RegCmor.Bits.Ccs = LOC_COUNT_CLK_PRESCALER;
        } 
        else if ( CountClock == R_TAUJ_COUNT_CLK_TIN) 
        {
            r_tauj_RegCmor.Bits.Ccs = LOC_COUNT_CLK_TIN;
        } 
        else if ( CountClock == R_TAUJ_COUNT_CLK_RFU) 
        {
            r_tauj_RegCmor.Bits.Ccs = LOC_COUNT_CLK_RFU;
        } 
        else if ( CountClock == R_TAUJ_COUNT_CLK_MASTER_INT_RFU) 
        {
            r_tauj_RegCmor.Bits.Ccs = LOC_COUNT_CLK_MASTER_INT_RFU;
        } 
        else 
        {
            error_mode = R_TAUJ_ERR_RANGE;
        }
    }

    /* if no error then update the register */
    if ( error_mode == R_TAUJ_ERR_OK) 
    {
        /* update the CMOR Channel Mode OS Register */
        base_address = R_TAUJ_Dev[Unit].BaseAddr;
        offset_address = R_TAUJ0_CMOR0 + loc_GetChannelAddress(Channel);
        R_TAUJ_WRITE16(base_address + offset_address, r_tauj_RegCmor.Word);
        /* check values! */
        while (R_TAUJ_READ16(base_address + offset_address) != r_tauj_RegCmor.Word) 
        {
            R_TAUJ_WRITE16(base_address + offset_address, r_tauj_RegCmor.Word);
        }           
    }
    return error_mode;
}


/***********************************************************
  Function: R_TAUJ_SetTinEdge
*/

r_tauj_Error_t R_TAUJ_SetTinEdge(uint32_t Unit, r_tauj_Channel_t Channel, r_tauj_TinEdge_t Edge) 
{
    r_tauj_Error_t error_mode = R_TAUJ_ERR_OK;
    uint32_t base_address = 0;
    uint32_t offset_address = 0;
    r_tauj_RegCmur_t r_tauj_RegCmur;

    /* check the instance is allowed */
    if ( Unit > (R_TAUJ_MACRO_NUM - 1)) 
    {
        /* instance too high */
        error_mode = R_TAUJ_ERR_INSTANCE;
    } 
    else 
    {
        /* ok, update the CMOR register value */
        base_address = R_TAUJ_Dev[Unit].BaseAddr;
        offset_address = R_TAUJ0_CMUR0 + loc_GetChannelAddress(Channel);
        r_tauj_RegCmur.Word = R_TAUJ_READ16(base_address + offset_address);

        /* timer interval */
        if ( Edge == R_TAUJ_FALLING_EDGE) 
        {
            r_tauj_RegCmur.Bits.Tis = LOC_FALLING_EDGE;
        } 
        else if ( Edge == R_TAUJ_RISING_EDGE) 
        {
            r_tauj_RegCmur.Bits.Tis = LOC_RISING_EDGE;
        } 
        else if ( Edge == R_TAUJ_BOTH_LOW_WIDTH) 
        {
            r_tauj_RegCmur.Bits.Tis = LOC_BOTH_LOW_WIDTH;
        } 
        else if ( Edge == R_TAUJ_BOTH_HIGH_WIDTH) 
        {
            r_tauj_RegCmur.Bits.Tis = LOC_BOTH_HIGH_WIDTH;
        } 
        else 
        {
            error_mode = R_TAUJ_ERR_RANGE;
        }
    }
    /* if no error then update the register */
    if ( error_mode == R_TAUJ_ERR_OK) 
    {
        /* update the CMUR Channel Mode OS Register */
        base_address = R_TAUJ_Dev[Unit].BaseAddr;
        offset_address = R_TAUJ0_CMUR0 + loc_GetChannelAddress(Channel);
        R_TAUJ_WRITE16(base_address + offset_address, r_tauj_RegCmur.Word);
           /* check values! */
        while (R_TAUJ_READ16(base_address + offset_address) != r_tauj_RegCmur.Word) 
        {
            R_TAUJ_WRITE16(base_address + offset_address, r_tauj_RegCmur.Word);
        }           
    }
    return error_mode;
}


/***********************************************************
  Function: R_TAUJ_SetPeriod
*/

r_tauj_Error_t R_TAUJ_SetPeriod(uint32_t Unit, r_tauj_Channel_t Channel, uint32_t Period) 
{
    r_tauj_Error_t error_mode = R_TAUJ_ERR_OK;
    uint32_t base_address = 0;
    uint32_t offset_address = 0;

    /* check the instance is allowed */
    if ( Unit > (R_TAUJ_MACRO_NUM - 1)) 
    {
        /* instance too high */
        error_mode = R_TAUJ_ERR_INSTANCE;
    } 
    else 
    {
        /* ok, update the CDR register value */
        base_address = R_TAUJ_Dev[Unit].BaseAddr;
        offset_address = R_TAUJ0_CDR0 + loc_GetChannelAddress(Channel);
        R_TAUJ_WRITE32(base_address + offset_address, Period);
    }

    return error_mode;
}


/***********************************************************
  Function: R_TAUJ_Start
*/

r_tauj_Error_t R_TAUJ_Start(uint32_t Unit, r_tauj_Channel_t Channel) 
{
    r_tauj_Error_t error_mode = R_TAUJ_ERR_OK;
    uint16_t u16_channel = 0;
    uint32_t base_address = 0;
    uint16_t u16_temp = 0;

    /* check the instance is allowed */
    if ( Unit > (R_TAUJ_MACRO_NUM - 1)) 
    {
        /* instance too high */
        error_mode = R_TAUJ_ERR_INSTANCE;
    } 
    else 
    {
        /* ok, update the TAUJ0TS: TAU Channel Start Trigger register */

        /* check the range for the channels */
        if ( Channel > R_TAUJ_CHANNEL_ALL) 
        {
            error_mode = R_TAUJ_ERR_RANGE;
        } 
        else 
        {
            base_address = R_TAUJ_Dev[Unit].BaseAddr;

            /* enable all channels */
            if ( Channel == R_TAUJ_CHANNEL_ALL) 
            {
                u16_temp = LOC_CHANNEL_ALL;
            } 
            else 
            { /* just one channel */
                u16_channel = loc_GetChannelID(Channel);
                R_TAUJ_SET_BIT(u16_temp, u16_channel);
            }
            /* update the TS Channel Start Trigger Register */
            R_TAUJ_WRITE16(base_address + R_TAUJ0_TS, u16_temp);
        }
    }
    return error_mode;
}


/***********************************************************
  Function: R_TAUJ_Stop
*/

r_tauj_Error_t R_TAUJ_Stop(uint32_t Unit, r_tauj_Channel_t Channel) 
{
    uint16_t u16_channel = 0;
    uint32_t base_address = 0;
    uint16_t u16_temp = 0;

    r_tauj_Error_t error_mode = R_TAUJ_ERR_OK;

    /* check the instance is allowed */
    if ( Unit > (R_TAUJ_MACRO_NUM - 1)) 
    {
        /* instance too high */
        error_mode = R_TAUJ_ERR_INSTANCE;
    } 
    else 
    {
        /* set the channel to be stopped in the TAUJ0TT: TAU Channel Stop Trigger Register */

        /* check the range for the channels */
        if ( Channel > R_TAUJ_CHANNEL_ALL) 
        {
            error_mode = R_TAUJ_ERR_RANGE;
        } 
        else 
        {
            /* enable all channels */
            if ( Channel == R_TAUJ_CHANNEL_ALL) 
            {
                u16_temp = LOC_CHANNEL_ALL;
            } 
            else 
            { /* just one channel */
                u16_channel = loc_GetChannelID(Channel);
                R_TAUJ_SET_BIT(u16_temp, u16_channel);
            }

            base_address = R_TAUJ_Dev[Unit].BaseAddr;

            /* update the TT Channel Stop Trigger Register (Write only) */
            R_TAUJ_WRITE16(base_address + R_TAUJ0_TT, u16_temp);
        }
    }
    return error_mode;
}


/***********************************************************
  Function: R_TAUJ_ReloadDataTrigger
*/

r_tauj_Error_t R_TAUJ_ReloadDataTrigger(uint32_t Unit, r_tauj_Channel_t Channel) 
{
    uint16_t u16_channel = 0;
    uint32_t base_address = 0;
    uint16_t u16_temp = 0;

    r_tauj_Error_t error_mode = R_TAUJ_ERR_OK;

    /* check the instance is allowed */
    if (Unit > (R_TAUJ_MACRO_NUM - 1)) 
    {
        /* instance too high */
        error_mode = R_TAUJ_ERR_INSTANCE;
    } 
    else 
    {
        /* check the range for the channels */
        if (Channel > R_TAUJ_CHANNEL_ALL) 
        {
            error_mode = R_TAUJ_ERR_RANGE;
        }
        else 
        { /* enable all channels */
            if ( Channel == R_TAUJ_CHANNEL_ALL) 
            {
                u16_temp = LOC_CHANNEL_ALL;
            } 
            else 
            { /* just one channel */
                u16_channel = loc_GetChannelID(Channel);
                R_TAUJ_SET_BIT(u16_temp, u16_channel);
            }

            base_address = R_TAUJ_Dev[Unit].BaseAddr;

            /* update the RDT Channel Reload Data Trigger Register (Write only) */
            R_TAUJ_WRITE16(base_address + R_TAUJ0_RDT, u16_temp);
        }
    }
    return error_mode;
}


/***********************************************************
  Function: R_TAUJ_SetInvertOutputTOUT
*/

r_tauj_Error_t R_TAUJ_SetInvertOutputTOUT(
    uint32_t Unit,
    r_tauj_Channel_t Channel,
    uint8_t InvertOutput)
{
    r_tauj_Error_t error_mode = R_TAUJ_ERR_OK;
    uint32_t base_address = 0;
    uint16_t u16_tol = 0;
    uint16_t u16_channel = 0;

    /* check the instance is allowed */
    if ( Unit > (R_TAUJ_MACRO_NUM - 1)) 
    {
        /* instance too high */
        error_mode = R_TAUJ_ERR_INSTANCE;
    } 
    else 
    {
        /* ok, invert the output of the TOUTn pin (TOL) */
        base_address = R_TAUJ_Dev[Unit].BaseAddr;
        u16_tol = R_TAUJ_READ16(base_address + R_TAUJ0_TOL);

        /* enable all channels */
        if ( Channel == R_TAUJ_CHANNEL_ALL) 
        {
            if ( InvertOutput == 1) 
            {
                u16_tol = LOC_CHANNEL_ALL;
            } 
            else 
            {
                u16_tol = 0;
            }
        }
        else 
        { /* just one channel */
            u16_channel = loc_GetChannelID(Channel);
            if ( InvertOutput == 1) 
            {
                R_TAUJ_SET_BIT(u16_tol, u16_channel);
            } 
            else 
            {
                R_TAUJ_CLR_BIT(u16_tol, u16_channel);
            }
        }
    }

    /* if no error then update the register */
    if ( error_mode == R_TAUJ_ERR_OK)
    {
        /* update the Channel Output Level Register (TOL) */
        base_address = R_TAUJ_Dev[Unit].BaseAddr;
        R_TAUJ_WRITE16(base_address + R_TAUJ0_TOL, u16_tol);
    }

    return error_mode;
}


/***********************************************************
  Function: R_TAUJ_OutputEnable
*/

r_tauj_Error_t R_TAUJ_OutputEnable(uint32_t Unit, r_tauj_Channel_t Channel) 
{
    r_tauj_Error_t error_mode = R_TAUJ_ERR_OK;
    uint16_t u16_channel = 0;
    uint32_t base_address = 0;
    uint16_t u16_temp = 0;

    /* check the instance is allowed */
    if ( Unit > (R_TAUJ_MACRO_NUM - 1)) 
    {
        /* instance too high */
        error_mode = R_TAUJ_ERR_INSTANCE;
    } 
    else 
    {
        /* ok, update the TAUJ0TOE: TAU Channel Output Enable register */

        /* check the range for the channels */
        if ( Channel > R_TAUJ_CHANNEL_ALL) 
        {
            error_mode = R_TAUJ_ERR_RANGE;
        } 
        else 
        {
            base_address = R_TAUJ_Dev[Unit].BaseAddr;
            u16_temp = R_TAUJ_READ16(base_address + R_TAUJ0_TOE);

            /* enable all channels */
            if ( Channel == R_TAUJ_CHANNEL_ALL) 
            {
                u16_temp = LOC_CHANNEL_ALL;
            } 
            else 
            { /* just one channel */
                u16_channel = loc_GetChannelID(Channel);
                R_TAUJ_SET_BIT(u16_temp, u16_channel);
            }

            /* update the TOE */
            R_TAUJ_WRITE16(base_address + R_TAUJ0_TOE, u16_temp);

            /* verify the register write was succesfull */
            if ( u16_temp != (R_TAUJ_READ16(base_address + R_TAUJ0_TOE))) 
            {
                error_mode = R_TAUJ_ERR_REG_UPDATE_FAIL;
            }
        }
    }
    return error_mode;
}


/***********************************************************
  Function: R_TAUJ_OutputWrite
*/

r_tauj_Error_t R_TAUJ_OutputWrite(uint32_t Unit, r_tauj_Channel_t Channel, uint8_t Value) {
    r_tauj_Error_t error_mode = R_TAUJ_ERR_OK;
    uint16_t u16_channel = 0;
    uint32_t base_address = 0;
    uint16_t u16_temp = 0;

    /* check the instance is allowed */
    if(Unit > (R_TAUJ_MACRO_NUM - 1)) 
    {
        /* instance too high */
        error_mode = R_TAUJ_ERR_INSTANCE;
    } 
    else 
    {
        /* ok, update the TAUJ0TO: TAU Channel Output register */

        /* check the range for the channels */
        if(Channel > R_TAUJ_CHANNEL_ALL) 
        {
            error_mode = R_TAUJ_ERR_RANGE;
        } 
        else 
        {
            base_address = R_TAUJ_Dev[Unit].BaseAddr;

            /* enable all channels */
            if (Channel == R_TAUJ_CHANNEL_ALL) 
            {
				if (Value == 1) 
                {
	                u16_temp = 0xFFFF;
				} 
                else 
                {
	                u16_temp = 0x0000;
				}
            } 
            else 
            { /* just one channel */
                u16_channel = loc_GetChannelID(Channel);
	            u16_temp = R_TAUJ_READ16(base_address + R_TAUJ0_TO);
				if (Value == 1) 
                {
	                R_TAUJ_SET_BIT(u16_temp, u16_channel);
				} 
                else 
                {
	                R_TAUJ_CLR_BIT(u16_temp, u16_channel);
				}
            }
            /* update the TOE */
            R_TAUJ_WRITE16(base_address + R_TAUJ0_TO, u16_temp);
        }
    }
    return error_mode;
}


/***********************************************************
  Function: R_TAUJ_OutputDisable
*/

r_tauj_Error_t R_TAUJ_OutputDisable(uint32_t Unit, r_tauj_Channel_t Channel) 
{
    r_tauj_Error_t error_mode = R_TAUJ_ERR_OK;
    uint16_t u16_channel = 0;
    uint32_t base_address = 0;
    uint16_t u16_temp = 0;

    /* check the instance is allowed */
    if ( Unit > (R_TAUJ_MACRO_NUM - 1)) 
    {
        /* instance too high */
        error_mode = R_TAUJ_ERR_INSTANCE;
    } 
    else 
    {
        /* ok, update the TAUJ0TOE: TAU Channel Output Enable register */

        /* check the range for the channels */
        if ( Channel > R_TAUJ_CHANNEL_ALL) 
        {
            error_mode = R_TAUJ_ERR_RANGE;
        } 
        else 
        {
            base_address = R_TAUJ_Dev[Unit].BaseAddr;
            u16_temp = R_TAUJ_READ16(base_address + R_TAUJ0_TOE);

            /* enable all channels */
            if ( Channel == R_TAUJ_CHANNEL_ALL) 
            {
                u16_temp = 0;
            } 
            else 
            { /* just one channel */
                u16_channel = loc_GetChannelID(Channel);
                R_TAUJ_CLR_BIT(u16_temp, u16_channel);
            }

            /* update the TOE */
            R_TAUJ_WRITE16(base_address + R_TAUJ0_TOE, u16_temp);
        }
    }

    return error_mode;
}


/***********************************************************
  Function: R_TAUJ_GetCaptureData
*/

uint32_t R_TAUJ_GetCaptureData(uint32_t Unit, r_tauj_Channel_t Channel) 
{
    uint32_t u32_temp = 0;
    uint32_t base_address = 0;
    uint32_t offset_address = 0;

    base_address = R_TAUJ_Dev[Unit].BaseAddr;

    /* read the CDR Channel Counter Register */
    offset_address = R_TAUJ0_CDR0 + loc_GetChannelAddress(Channel);
    u32_temp = R_TAUJ_READ32(base_address + offset_address);

    return u32_temp;
}


/***********************************************************
  Function: R_TAUJ_GetCntData
*/

uint32_t R_TAUJ_GetCntData(uint32_t Unit, r_tauj_Channel_t Channel) 
{
    uint32_t u32_temp = 0;
    uint32_t base_address = 0;
    uint32_t offset_address = 0;

    base_address = R_TAUJ_Dev[Unit].BaseAddr;

    /* update the CNT Channel Counter Register */
    offset_address = R_TAUJ0_CNT0 + loc_GetChannelAddress(Channel);
    u32_temp = R_TAUJ_READ32(base_address + offset_address);

    return u32_temp;
}


/***********************************************************
  Function: R_TAUJ_ReadReg
*/

uint16_t R_TAUJ_ReadReg(uint32_t Unit, uint32_t Offset) 
{
    uint16_t u16_temp = 0;
    uint32_t base_address = 0;
    uint32_t offset_address = 0;

    base_address = R_TAUJ_Dev[Unit].BaseAddr;

    offset_address = Offset;
    u16_temp = R_TAUJ_READ16(base_address + offset_address);

    return u16_temp;
}


/***********************************************************
  Function: R_TAUJ_WriteReg
*/

void R_TAUJ_WriteReg(uint32_t Unit, uint32_t Offset, uint16_t Data) 
{
    uint32_t base_address = 0;
    uint32_t offset_address = 0;

    base_address = R_TAUJ_Dev[Unit].BaseAddr;

    offset_address = Offset;
    R_TAUJ_WRITE16(base_address + offset_address, Data);
}


/***********************************************************
  Function: R_TAUJ_SetErrorCallback

*/

void R_TAUJ_SetErrorCallback(void ( *ErrorCallback )(uint32_t Unit, uint32_t Error)) 
{
    _UserErrorHandler = ErrorCallback;
}


/***********************************************************
  Function: R_TAUJ_GetClock
*/

uint32_t R_TAUJ_GetClock(uint32_t Unit) 
{
    return R_TAUJ_Sys_GetClock(Unit);
}


/***********************************************************
  Function: R_TAUJ_SetFilterBypass
*/

uint32_t R_TAUJ_SetFilterBypass(uint32_t Unit) 
{
    return R_TAUJ_Sys_SetFilterBypass(Unit);
}


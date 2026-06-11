/************************************************************************************************
PROJECT : Serial flash calibration routine
FILE    : $Id: r_test_sfma.c 1582 2013-10-24 14:42:36Z oliver.achten $
============================================================================
DESCRIPTION
Main funcitons of the SF calibration application
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

/*******************************************************************************
  Title: Serial Flash calibration application

*/

#ifdef SFMA_USE_BSP_CALIB

/*******************************************************************************
  Section Includes
*/

#include "r_typedefs.h"     /* Renesas basic types, e.g. uint32_t */

#include "r_sfma_api.h"
#include "r_config_sfma.h"
#include "r_sfcdb_macronix.h"

#include "r_dev_api.h"
#include "r_tick_api.h"
#include "r_bsp_stdio_api.h"

#include "r_gpio_api.h"
#include "r_sfma_calib.h"


/***********************************************************
    Constants

*/


/*******************************************************************************
  Constant: CAL_BUFFER_SIZE

  Calibration in words DO NOT CHANGE as this is the page size for erase too
*/
#define CAL_BUFFER_SIZE             (0x200uL / 4uL)


/*******************************************************************************
  Constant: CAL_TEST_PATTERN

  Calibration pattern to use for the calibration data
*/
#define CAL_TEST_PATTERN            (0x00FF00F0uL)


/*******************************************************************************
  Constant: FLASH_SIZE_128MB

  Calibration size of 128 MBytes just for referenxce
*/
#define FLASH_SIZE_128MB            (128uL * 1024uL * 1024uL)


/***************************************************************************
  Constant: SF_DEFAULT_CAL

  Default calibration for Serial flash memory.

    SPBCLK clock phase shift        3 (Upper 16bits)
    Sampling point phase shift      1 (Lower 16 bits)
*/
#define SF_SPBCLK_CLOCK_PHASE_SHIFT    (3u)
#define SF_SAMPLING_POINT_PHASE_SHIFT  (1u)
#define SF_DEFAULT_CAL                 ((SF_SPBCLK_CLOCK_PHASE_SHIFT << 16) | SF_SAMPLING_POINT_PHASE_SHIFT)


/*******************************************************************************
  Variable: loc_cal

  This value holds the user supplied calibration delay.
*/
static uint32_t loc_cal = SF_DEFAULT_CAL;


/*******************************************************************************
  Variable: loc_cal_test_size

  This value holds the size of the memory to test for the calibration.
*/
static uint32_t loc_cal_test_size;


/*******************************************************************************
  Variable: loc_cal_test_start_address_os

  This value holds the offset in memory to test for the calibration.
*/
static uint32_t loc_cal_test_start_address_os;


/*******************************************************************************
  Variable: loc_cal_base_memory_addr

  This value holds the base address of the memory for the calibration.
*/
static uint32_t loc_cal_base_memory_addr;


/***************************************************************************
  Section: Local functions
*/
static sf_Error_t SF_CalibrationVerifyData(uint32_t Unit, r_sfma_Config_t * Config);



/***************************************************************************
  Section: Global functions
*/


/***********************************************************
  Function: SF_CalibrationInit

  see: <sf_cal.h>
*/
sf_Error_t SF_CalibrationInit(uint32_t Unit, const sf_CalConfig_t * const CalConfig)
{
    /* default */
    loc_cal = SF_DEFAULT_CAL;

    /* how many bytes to test */
    loc_cal_test_size = CalConfig->TestSize;

    /* offset of the test area */
    loc_cal_test_start_address_os = CalConfig->TestStartAddressOs;

    /* Base address of the SF data memory */
    loc_cal_base_memory_addr = CalConfig->BaseMemoryAddr;

    return SF_ERR_OK;
}


/***********************************************************
  Function: SF_CalibrationRun

  see: <sf_cal.h>
*/
sf_Error_t SF_CalibrationRun(uint32_t Unit, r_sfma_Config_t * Config, uint32_t f_phclk, uint32_t f_b)
{
uint32_t u32_temp;
uint32_t i;
uint32_t u32_write;
uint32_t error_count;
uint32_t *u32_ptr;
uint32_t max_steps;
uint32_t step;
uint32_t ckdlyrx;
uint32_t ckdlyoc;
r_sfma_Error_t sfma_error;
sf_Error_t sf_error;
uint8_t exit_loop;
uint8_t correct_at_max_step;

    /* init the clock delay */
    ckdlyrx = 0;
    ckdlyoc = 0;

    /* SFMA calibration start */

    /* set SPBCLK clock phase shift depending on f_phclk/f_b ratio
        (Output clock adjustment according to UM section 17.5.14.2 (1) */
    switch (f_phclk / f_b)
    {
        case 2:
            ckdlyoc = 0x2;  /* 010_B */
            break;
        case 3:
            ckdlyoc = 0x3;  /* 011_B*/
            break;
        case 4:
            ckdlyoc = 0x5;  /* 101_B */
            break;
        default:
            sfma_error = R_SFMA_ERR_NOT_SUPPORT_CLOCK;
#ifdef SFMA_CAL_DEBUG
            R_BSP_STDIO_Printf("The f_phclk/f_b ratio '%d' is not supported \n", (f_phclk / f_b));
#endif
            break;            
    }
    
    /* set to MaxShift depends on f_phclk/f_b ratio */
    max_steps = (2 * (f_phclk / f_b)) - 2;

#ifdef SFMA_CAL_DEBUG
    R_BSP_STDIO_Printf("Max steps '%d'\n", max_steps);
#endif

    ckdlyrx = max_steps;

    /* loop exit flag */
    exit_loop = R_FALSE;

    /* correct at the max sapling point flag */
    correct_at_max_step = R_FALSE;

    /* run through the steps */
    for(step = 0; step <= max_steps; step++)
    {
        loc_cal = (ckdlyoc << 16) + (ckdlyrx);

        /* Init the SFMA */
        sfma_error = R_SFMA_Init(Unit);
        if(R_SFMA_ERR_OK == sfma_error)
        {
            /* open */
            Config->Calibration = loc_cal;

            sfma_error = R_SFMA_Open(Unit, Config);
            if(R_SFMA_ERR_OK == sfma_error)
            {
                /* set address space either 64 or 128MByte */
                if(FLASH_SIZE_128MB == Config->SerialFlashMemorySize)
                {
                    sfma_error = R_SFMA_AccessAddressSet(Unit, 0x00000000UL, R_SFMA_ACCESS_RANGE_128MB);
#ifdef SFMA_CAL_DEBUG
                    R_BSP_STDIO_Printf("SFMA access request 128MB\n");
#endif  
                }
                else
                {
                    sfma_error = R_SFMA_AccessAddressSet(Unit, 0x00000000UL, R_SFMA_ACCESS_RANGE_64MB);
#ifdef SFMA_CAL_DEBUG
                    R_BSP_STDIO_Printf("SFMA access request 64MB\n");
#endif 
                }
    
                if(R_SFMA_ERR_OK == sfma_error)
                {
                    /* ptr to the SFMA base */
                    error_count = 0;
                    u32_ptr = (uint32_t *) (loc_cal_base_memory_addr + loc_cal_test_start_address_os);
            
                    u32_write = CAL_TEST_PATTERN;
            
                    /* check the data */
                    for(i = 0; i < CAL_BUFFER_SIZE; i++)
                    {
                        u32_temp = *u32_ptr;
                        u32_ptr++;
            
                        if(u32_temp != u32_write)
                        {
#ifdef SFMA_CAL_DEBUG
                            /*R_BSP_STDIO_Printf("Error: SFMA read data 0x%08x, expected  0x%08x\n", u32_temp, u32_write);*/
#endif                
                            error_count++;
                        }
                    }
            
#ifdef SFMA_CAL_DEBUG
                    R_BSP_STDIO_Printf("  -  Error count %d (bytes)\n\n", error_count * 4);
#endif
            
                    /* no rap round */
                    if(ckdlyrx > 0)
                    {
                        ckdlyrx--;
                    }
            
                    R_SFMA_Close(Unit);
                    R_SFMA_DeInit(Unit);
            
                    /* chosen value is 1 below the first time pass with no errors, it decremented above */
                    if(error_count == 0)
                    {
                        if(step == 0)
                        {
                            /* If the read data is already correct with the maximum sampling point shift, 
                               increment current CKDLYRX[2:0] value by 1.
                            */
                            ckdlyrx += 2; /* decremented above so add 1 extra */
                            correct_at_max_step = R_TRUE;
                        }
                        else
                        {
                            exit_loop = R_TRUE;
                        }
                    }
                    else
                    {
                        /* if the data is correct at the max_step but not at max_step + 1 then exit out 
                           and chose max step
                        */ 
                        if((R_FALSE != correct_at_max_step) && (step == 1))
                        {
                            ckdlyrx = max_steps - 1;
                            exit_loop = R_TRUE;
                        }
                        else
                        {
                            /* nothing */
                        }
                    }
                }
                else
                {

#ifdef SFMA_CAL_DEBUG
                    R_BSP_STDIO_Printf(" - Error\n");
#endif
                    /* error so exit */
                    exit_loop = R_TRUE;
                }
            }
            else
            {
#ifdef SFMA_CAL_DEBUG
                R_BSP_STDIO_Printf(" SFMA open error\n");
#endif
                /* error so exit */
               exit_loop = R_TRUE;
            }
        }
        else
        {
#ifdef SFMA_CAL_DEBUG
            R_BSP_STDIO_Printf(" SFMA Init error\n");
#endif

            /* error so exit */
            exit_loop = R_TRUE;
        }

        /* error or finsihed so exit loop */
        if(R_FALSE != exit_loop)
        {
            break;
        }
        else
        {
            /* nothing */
        }
    }

    /* if no errors */
    if(R_SFMA_ERR_OK == sfma_error)
    {
        loc_cal = (ckdlyoc << 16) + (ckdlyrx);

#ifdef SFMA_CAL_DEBUG
        R_BSP_STDIO_Printf("Chosen SFMA calibration setting: 0x%08x (CKDLY)\n\n", loc_cal);
#endif

        /* verify the data with the new calibration settings */
        sf_error = SF_CalibrationVerifyData(Unit, Config);

        /* we are done */
        R_SFMA_DeInit(Unit);
    }
    else
    {
        /* nothing */
    }

    /* report status */
    if((SF_ERR_OK != sf_error) || (R_SFMA_ERR_OK != sfma_error))
    {
        /* error */
        return SF_ERR_NG;
    }
    else
    {
        /* ok */
        return SF_ERR_OK;
    }
}


/***********************************************************
  Function: R_SFMA_CalibrationWriteData

  see: <sf_cal.h>
*/
sf_Error_t SF_CalibrationWriteData(uint32_t Unit, r_sfma_Config_t * Config)
{
uint32_t i;
uint32_t address;
r_sfma_Error_t sfma_error;
uint32_t loc_buffer[CAL_BUFFER_SIZE];

    /* Init the SFMA */
    sfma_error = R_SFMA_Init(Unit);
    if(R_SFMA_ERR_OK == sfma_error)
    {
        Config->Calibration = loc_cal;
        sfma_error = R_SFMA_Open(Unit, Config);
    
        if(R_SFMA_ERR_OK == sfma_error)
        {
            sfma_error = R_SFMA_ProtectionModeSet(Unit, R_SFMA_MODE_UNPROTECT);

            if(R_SFMA_ERR_OK == sfma_error)
            {
                /* write the address value for the data */
                address = loc_cal_test_start_address_os;
            
                sfma_error = R_SFMA_DataErase(Unit, address, loc_cal_test_size);
            
                /* prepare the buffer */
                for(i = 0; i < CAL_BUFFER_SIZE; i++)
                {
                    loc_buffer[i] = CAL_TEST_PATTERN;
                }
            
                /* check the address range then write the data */
                while(address < (loc_cal_test_start_address_os + loc_cal_test_size))
                {
                    /* write the data this is bytes */
                    sfma_error = R_SFMA_DataWrite(Unit, address, (uint8_t *) &loc_buffer[0], CAL_BUFFER_SIZE * 4);
                    
                    if(R_SFMA_ERR_OK == sfma_error)
                    {
                        address += CAL_BUFFER_SIZE * 4;
                    }
                    else
                    {
                        /* error, something went wrong... exit*/
                        break;
                    } 
                }
            
                R_SFMA_Close(Unit);
                R_SFMA_DeInit(Unit);
            }
        }
    }

    /* report status */
    if(R_SFMA_ERR_OK != sfma_error)
    {
        /* error */
        return SF_ERR_NG;
    }
    else
    {
        /* ok */
        return SF_ERR_OK;
    }
}


/***************************************************************************
  Function: SF_CalibrationVerifyData

  see: <sf_cal.h>
*/
static sf_Error_t SF_CalibrationVerifyData(uint32_t Unit, r_sfma_Config_t * Config)
{
uint32_t u32_temp;
uint32_t i;
uint32_t u32_write;
uint32_t error_count;
uint32_t * u32_ptr;
uint32_t address;
r_sfma_Error_t sfma_error;

    /* Init the SFMA */
    sfma_error = R_SFMA_Init(Unit);
    if(R_SFMA_ERR_OK == sfma_error)
    {
        /* setup the serial flash */
        Config->Calibration = loc_cal;
        sfma_error = R_SFMA_Open(Unit, Config);
        if(R_SFMA_ERR_OK == sfma_error)
        {
            /* clear our error count */
            error_count = 0;
        
            /* set address space either 64 or 128MByte */
            if(FLASH_SIZE_128MB == Config->SerialFlashMemorySize)
            {
                R_SFMA_AccessAddressSet(Unit, 0x00000000UL, R_SFMA_ACCESS_RANGE_128MB);
            }
            else
            {
               R_SFMA_AccessAddressSet(Unit, 0x00000000UL, R_SFMA_ACCESS_RANGE_64MB);
            }
            u32_write = CAL_TEST_PATTERN;

            address = loc_cal_base_memory_addr + loc_cal_test_start_address_os;
            u32_ptr = (uint32_t *) (loc_cal_base_memory_addr + loc_cal_test_start_address_os);

            /* read the data */
            while(address < (loc_cal_base_memory_addr + loc_cal_test_start_address_os + loc_cal_test_size))
            {
                for(i = 0; i < CAL_BUFFER_SIZE; i++)
                {
                    u32_temp = *u32_ptr;
                    u32_ptr++;

                    if(u32_temp != u32_write)
                    {
#ifdef SFMA_CAL_DEBUG
                        R_BSP_STDIO_Printf(" - Error: SFMA (addr: 0x%08x) data = 0x%08x, expected  0x%08x\n", address, u32_temp, u32_write);
#endif
                        error_count++;
                    }
                    address += 4;
                }
            }

            /* no errors just report */
            if(error_count == 0)
            {
#ifdef SFMA_CAL_DEBUG
                R_BSP_STDIO_Printf("No errors found, calibration passed (%d bytes read)\n", loc_cal_test_size);
#endif
            }
            else
            {
#ifdef SFMA_CAL_DEBUG
                R_BSP_STDIO_Printf("Error calibration failed, total error count %d bytes (%d bytes read )\n", error_count * 4, loc_cal_test_size);
#endif
            }
        }
        else
        {
            /* nothing, catch below */
        }
    }
    else
    {
        /* nothing, catch below */
    }

    /* return the status */
    if((R_SFMA_ERR_OK != sfma_error) || (error_count != 0))
    {
        /* something went wrong, close and report */
        R_SFMA_Close(Unit);
        R_SFMA_DeInit(Unit);
        return SF_ERR_NG;
    }
    else
    {
        return SF_ERR_OK;
    }
}

#endif /* SFMA_USE_BSP_CALIB */


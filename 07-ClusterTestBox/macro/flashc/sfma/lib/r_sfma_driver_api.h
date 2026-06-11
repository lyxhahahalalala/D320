/*******************************************************************************
* File Name   : r_sfma_driver_api.h
* Version     : 0.1
* Description : Application API for D1H SFMA Driver
******************************************************************************/
/*****************************************************************************
* History : DD.MM.YYYY Version Description
*         : 08.01.2015 0.01    First Release
******************************************************************************/

/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

#ifndef R_SFMA_DRIVER_API_H
#define R_SFMA_DRIVER_API_H

#include "r_sfma_api.h"

/******************************************************************************
Macro definitions
******************************************************************************/
/* Defines */

#define SFMA_API_LOG                       (1)

#if SFMA_API_LOG
#define SFMA_API_PRINTF(...)               (void)printf(__VA_ARGS__)
#else
#define SFMA_API_PRINTF(...)
#endif

#define SFMA_TOTAL_OPTION_BYTES                  (4)
/******************************************************************************
Typedef definitions
******************************************************************************/
/* Handle to an open SFMA channel */
typedef uint16_t sfma_handle_t;
typedef enum
{
    SFMA_OK,
    SFMA_ERROR_ALREADY_INITIALISED,
    SFMA_ERROR_INITIALISE_FAILED,
    SFMA_ERROR_NOT_INITIALISED,
    SFMA_ERROR_UNINITIALISE_FAILED,
    SFMA_ERROR_NO_HANDLES,
    SFMA_ERROR_INVALID_CHANNEL,
    SFMA_ERROR_SYSTEM,
    SFMA_ERROR_INVALID_HANDLE,
    SFMA_ERROR_PARAMETER_INVALID,
    /*The underlying base driver has reported an error.
    Use R_SFMA_GetStatus to get the error.*/
    SFMA_ERROR_BASE_DRIVER, 
    R_SFMA_ERROR_SFLASH_PROTECTED

} sfma_error_t;

typedef enum
{
    SFMA_READ_STATUS,
    SFMA_READ_CONFIG,
    SFMA_WRITE_STATUS,
    SFMA_WRITE_ENABLE,
    SFMA_WRITE_EXIT_ADDR,
    SET_MODE,
    SET_PROTECTION,
    SECTOR_ERASE,
    SFMA_WRITE,
    SFMA_READ,
    SFMA_SET_SPI_CONFIG,
    SFMA_UPDATE_COMMAND
} sfma_send_command_type_t;

typedef struct
{
    /*Address in flash that will be mapped to zero in the 
    in the external memory mapped access window.*/
    uint32_t addr;
    /*Set the size of the access window.*/
    r_sfma_AccessRange_t range;
} sfma_set_access_address_t;

typedef struct
{
    /*Address in flash*/
    uint64_t addr;
    /*Length of the flash to be erased
     The erase sector and size will be calculated using the following formula:
     erase_sector = (addr / EraseSectorSize);
     erase_addr   = (erase_sector * EraseSectorSize);*/
    uint32_t length;
} sfma_erase_range_t;


typedef struct
{
    sfma_send_command_type_t command_type;
    uint8_t  Write_Status;
    uint8_t  Write_Config;
    uint8_t *ptrRead1;
    uint8_t  *ptrRead2;
    uint8_t  OptionBytes[SFMA_TOTAL_OPTION_BYTES];
    uint8_t  commmand;
    uint8_t  optional_command;
    r_sfma_Config_t  *config;
    uint64_t   Addr;
    uint8_t * Buf;
    uint32_t size;
    r_sfma_SpibscSpimdReg_t *spi_md_reg;
} r_sfma_Send_Command_Buffer_t;


/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/
sfma_error_t R_SFMA_Initialise(void);
sfma_error_t R_SFMA_Uninitialise(void);
sfma_error_t R_SFMA_OpenChannel(uint32_t channel_number, sfma_handle_t *handle);
sfma_error_t R_SFMA_CloseChannel(sfma_handle_t channel_handle);
sfma_error_t R_SFMA_ConfigureChannel(sfma_handle_t channel_handle, const r_sfma_Config_t *configuration);
/* NOTE: The configuration->Mode to be set as  SPI or EXT_SPACE to the function R_SFMA_InitConfiguration to
 * initialise for its mode */
sfma_error_t R_SFMA_InitConfiguration(sfma_handle_t channel_handle, r_sfma_Config_t *configuration);
sfma_error_t R_SFMA_WriteBytes(sfma_handle_t channel_handle, uint32_t address, const Buffer *buffer);
sfma_error_t R_SFMA_ReadBytes(sfma_handle_t channel_handle, uint32_t address, const Buffer *buffer);
sfma_error_t R_SFMA_GetUserStatus(sfma_handle_t channel_handle, r_sfma_Error_t* pError);
sfma_error_t R_SFMA_SetAccessAddress(sfma_handle_t channel_handle, const sfma_set_access_address_t *param);
sfma_error_t R_SFMA_DataRangeErase(sfma_handle_t channel_handle, const uint64_t Addr, const int32_t Size);
sfma_error_t R_SFMA_SendCommand(sfma_handle_t channel_handle, r_sfma_Send_Command_Buffer_t * Command_Buffer);
sfma_error_t R_SFMA_Wait(sfma_handle_t channel_handle,const uint64_t usec);
#if 0
/* TODO: not supported yet */
sfma_error_t R_SFMA_Cancel(sfma_handle_t channel_handle);
#endif

#endif

/*******************************************************************************
* File Name   : r_sfma_iodevice_api.h
* Version     : 0.1
* Description : IODevice interface for D1H SFMA
******************************************************************************/
/*****************************************************************************
* History : DD.MM.YYYY Version Description
*         : 22.10.2015 0.01    First Release
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

#ifndef R_SFMA_IODEVICE_API_H
#define R_SFMA_IODEVICE_API_H

/******************************************************************************
Macro definitions
******************************************************************************/

/* Number of channels */
#define SFMA_CHANNEL_COUNT               (1)

/* Device names */
#define SFMA_IODEVICE_0       "SFMAIODevice0"

/* Password for IODevice */
#define SFMA_PASSWORD         "!systempassword"

/* WriteRegister & BlockType values */
#define SFMA_CHANNEL_OPEN                (0)
#define SFMA_CHANNEL_CLOSE               (1)
#define SFMA_CHANNEL_CANCEL              (2)
#define SFMA_CHANNEL_STATUS              (3)
#define SFMA_CHANNEL_CONFIG              (4)
#define SFMA_CHANNEL_SET_MODE            (5)
#define SFMA_CHANNEL_WRITE_DATA          (6)
#define SFMA_CHANNEL_READ_DATA           (7)
#define SFMA_CHANNEL_INIT_CONFIG         (8)
#define SFMA_CHANNEL_SEND_COMMAND        (9)


/*WriteStatus and ReadStatus StatusNumbers */
#define SFMA_SET_ACCESS_ADDRESS (0)
#define SFMA_ERASE_ADDRESS      (1)

typedef struct
{
    uint64_t   Addr;
    uint8_t * Buf;
    uint32_t size;
} r_sfma_iodevice_send_command_t;

Address R_SFMAMM_IODevice_GetKernelAddress(void);
Address R_SFMAMM_IODevice_GetPhysicalAddress(void);
uint32_t R_SFMAMM_IODevice_GetSize(void);
r_sfma_Error_t  R_SFMA_FlashDevReset(void);
r_sfma_Error_t  R_SFMA_IoSys_Close(const uint32_t Unit);


#endif

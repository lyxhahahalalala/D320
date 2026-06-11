/*******************************************************************************
* File Name   : sfma_params.h
* Version     : 0.1
* Description : Serial Flash Configuration Parameters
******************************************************************************/
/*****************************************************************************
* History : DD.MM.YYYY Version Description
*         : 09.11.2015 0.01    First Release
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
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

#ifndef SFMA_DEFAULT_PARAMS_H
#define SFMA_DEFAULT_PARAMS_H

//Taken from vlib\app\atf\at_sfma_ut\src
#ifdef R_TEST_SFLASH_SPANSION
#define R_TEST_ONBOARD_FLASHROM_MANUFACTURE_ID  (0x1)
#define R_TEST_ONBOARD_FLASHROM_MEMORYTYPE_ID   (0x2)
#define R_TEST_ONBOARD_FLASHROM_CATEGORY_ID     (0x19)

#define R_SFMA_SFLASHCMD_SECTOR_ERASE        (0xD8uL)
#define R_SFMA_SFLASHCMD_BYTE_PROGRAM        (0x02uL)
#define R_SFMA_SFLASHCMD_BYTE_READ           (0x0BuL)
#define R_SFMA_SFLASHCMD_DUAL_READ           (0x3BuL)
#define R_SFMA_SFLASHCMD_QUAD_READ           (0x6BuL)
#define R_SFMA_SFLASHCMD_DUAL_IO_READ        (0xBBuL)
#define R_SFMA_SFLASHCMD_QUAD_IO_READ        (0xEBuL)
#define R_SFMA_SFLASHCMD_DDR_BYTE_READ       (0x0DuL)
#define R_SFMA_SFLASHCMD_DDR_DUAL_READ       (0xBDuL)
#define R_SFMA_SFLASHCMD_DDR_QUAD_READ       (0xEDuL)
#define R_SFMA_SFLASHCMD_WRITE_ENABLE        (0x06uL)
#define R_SFMA_SFLASHCMD_READ_STATUS         (0x05uL)
#define R_SFMA_SFLASHCMD_READ_CONFIG         (0x35uL)
#define R_SFMA_SFLASHCMD_WRITE_STATUS        (0x01uL)
#define R_SFMA_SFLASHCMD_QUAD_PROGRAM        (0x32uL)

#define R_SFMA_SFLASHCMD_SECTOR_ERASE_32BIT  (0xDCuL)
#define R_SFMA_SFLASHCMD_BYTE_READ_32BIT     (0x0CuL)
#define R_SFMA_SFLASHCMD_DUAL_READ_32BIT     (0x3CuL)
#define R_SFMA_SFLASHCMD_QUAD_READ_32BIT     (0x6CuL)
#define R_SFMA_SFLASHCMD_DUAL_IO_READ_32BIT  (0xBCuL)
#define R_SFMA_SFLASHCMD_QUAD_IO_READ_32BIT  (0xECuL)
#define R_SFMA_SFLASHCMD_DDR_BYTE_READ_32BIT (0x0EuL)
#define R_SFMA_SFLASHCMD_DDR_DUAL_READ_32BIT (0xBEuL)
#define R_SFMA_SFLASHCMD_DDR_QUAD_READ_32BIT (0xEEuL)

#define R_SFMA_SFLASHCMD_BYTE_PROGRAM_32BIT  (0x12uL)
#define R_SFMA_SFLASHCMD_QUAD_PROGRAM_32BIT  (0x34uL)

#define R_SFMA_SECTOR_SIZE                   (256uL * 1024uL)  /* Sector size = 256 KB   */
#define R_SFMA_QUAD_IO_MODE_REG              (R_SFMA_CONFIG_REG)
#define R_SFMA_QUAD_IO_MODE_BIT              (0x02)
#define R_SFMA_BLOCK_PROTECT_REG             (R_SFMA_STATUS_REG)
#define R_SFMA_BLOCK_PROTECT_BIT             (0x1C)
#define R_SFMA_WIP_REG                       (R_SFMA_STATUS_REG)
#define R_SFMA_WIP_BIT                       (0x01)
#define R_SFMA_DUMMY_CYCLE_REG               (R_SFMA_CONFIG_REG)
#define R_SFMA_DUMMY_CYCLE_BIT               (0xC0)
#define R_SFMA_DUMMY_CYCLE_SET               (0x00)

#define R_SFMA_DUMMY_CYCLE_SDR               (R_SFMA_SPIBSC_DUMMY_8CYC)
#define R_SFMA_DUMMY_CYCLE_SDR_DUAL          (R_SFMA_SPIBSC_DUMMY_4CYC)
#define R_SFMA_DUMMY_CYCLE_SDR_QUAD          (R_SFMA_SPIBSC_DUMMY_4CYC)
#define R_SFMA_DUMMY_CYCLE_DDR               (R_SFMA_SPIBSC_DUMMY_6CYC)
#define R_SFMA_DUMMY_CYCLE_DDR_DUAL          (R_SFMA_SPIBSC_DUMMY_6CYC)
#define R_SFMA_DUMMY_CYCLE_DDR_QUAD          (R_SFMA_SPIBSC_DUMMY_6CYC)
#define R_SFMA_QUAD_WRITE_ADDR               (R_SFMA_ADDRESS_SIZE_1BIT)

#else
/* Macronix */
#define R_TEST_ONBOARD_FLASHROM_MANUFACTURE_ID  (0xC2)
#define R_TEST_ONBOARD_FLASHROM_MEMORYTYPE_ID   (0x20)
#define R_TEST_ONBOARD_FLASHROM_CATEGORY_ID     (0x1A)

#define R_SFMA_SFLASHCMD_SECTOR_ERASE        (0x21uL)
#define R_SFMA_SFLASHCMD_BYTE_PROGRAM        (0x02uL)
#define R_SFMA_SFLASHCMD_BYTE_READ           (0x0CuL)
#define R_SFMA_SFLASHCMD_DUAL_READ           (0x3CuL)
#define R_SFMA_SFLASHCMD_QUAD_READ           (0x6CuL)
#define R_SFMA_SFLASHCMD_DUAL_IO_READ        (0xBCuL)
#define R_SFMA_SFLASHCMD_QUAD_IO_READ        (0xECuL)
#define R_SFMA_SFLASHCMD_DDR_BYTE_READ       (0x0EuL)
#define R_SFMA_SFLASHCMD_DDR_DUAL_READ       (0xFFuL)
#define R_SFMA_SFLASHCMD_DDR_QUAD_READ       (0xFFuL)
#define R_SFMA_SFLASHCMD_WRITE_ENABLE        (0x06uL)
#define R_SFMA_SFLASHCMD_READ_STATUS         (0x05uL)
#define R_SFMA_SFLASHCMD_READ_CONFIG         (0x15uL)
#define R_SFMA_SFLASHCMD_WRITE_STATUS        (0x01uL)
#define R_SFMA_SFLASHCMD_QUAD_PROGRAM        (0x38uL)

#define R_SFMA_SFLASHCMD_SECTOR_ERASE_32BIT  (0x21uL)
#define R_SFMA_SFLASHCMD_BYTE_READ_32BIT     (0x0CuL)
#define R_SFMA_SFLASHCMD_DUAL_READ_32BIT     (0x3CuL)
#define R_SFMA_SFLASHCMD_QUAD_READ_32BIT     (0x6CuL)
#define R_SFMA_SFLASHCMD_DUAL_IO_READ_32BIT  (0xBEuL)
#define R_SFMA_SFLASHCMD_QUAD_IO_READ_32BIT  (0xEEuL)
#define R_SFMA_SFLASHCMD_DDR_BYTE_READ_32BIT (0x0EuL)
#define R_SFMA_SFLASHCMD_DDR_DUAL_READ_32BIT (0xBEuL)
#define R_SFMA_SFLASHCMD_DDR_QUAD_READ_32BIT (0xEEuL)

#define R_SFMA_SFLASHCMD_BYTE_PROGRAM_32BIT  (0x12uL)
#define R_SFMA_SFLASHCMD_QUAD_PROGRAM_32BIT  (0x3EuL)

#define R_SFMA_SECTOR_SIZE                   (4uL * 1024uL)  /* Sector size = 4 KB   */
#define R_SFMA_QUAD_IO_MODE_REG              (R_SFMA_STATUS_REG)
#define R_SFMA_QUAD_IO_MODE_BIT              (0x40)
#define R_SFMA_BLOCK_PROTECT_REG             (R_SFMA_STATUS_REG)
#define R_SFMA_BLOCK_PROTECT_BIT             (0x3C)
#define R_SFMA_WIP_REG                       (R_SFMA_STATUS_REG)
#define R_SFMA_WIP_BIT                       (0x01)
#define R_SFMA_DUMMY_CYCLE_REG               (R_SFMA_CONFIG_REG)
#define R_SFMA_DUMMY_CYCLE_BIT               (0xC0)
#define R_SFMA_DUMMY_CYCLE_SET               (0x80)

#define R_SFMA_QUAD_WRITE_ADDR               (R_SFMA_ADDRESS_SIZE_4BIT)


#define R_SFMA_EXIT_EXT_ADDRSPC              (0xFF)
#define R_SFMA_RESET_ENABLE                  (0x66)
#define R_SFMA_RESET_VALUE                   (0x99)

#endif




#endif

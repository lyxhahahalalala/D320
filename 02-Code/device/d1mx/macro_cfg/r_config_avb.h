/*
****************************************************************************
PROJECT : AVB device level config
FILE    : $Id: r_config_avb.h 3275 2014-07-14 13:10:30Z dejan.kostic $
============================================================================
DESCRIPTION
AVB configuration for r7f701412
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

********************************************************************************
*/

#ifndef _R_CONFIG_AVB_H_
#define _R_CONFIG_AVB_H_

/*******************************************************************************
  Title: AVB device configuration

  Configuration of the device dependanant part of the AVB macro
*/

/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constant: R_AVB_MACRO_NUM

  Number of macros in the device.
*/
#define R_AVB_MACRO_NUM         1u



/*******************************************************************************
  Constant: R_AVB_BASE

   Base address for the macro registers
*/
#define R_AVB_BASE        0xFFDE0000u   /* To be corrected */


/***********************************************************
  Channel number
*/
#define R_AVB_BECHANNEL      0      /* best effort channel */
#define R_AVB_NCCHANNEL      1      /* network control channel */
#define R_AVB_SCHANNEL1      2      /* stream channel 1 */
#define R_AVB_SCHANNEL2      3      /* stream channel 2 */


/***********************************************************
  number of TX/RX descriptor
*/
#define R_AVB_TXDESC_Q_NUM      7       /* tx descriptor number */

#define R_AVB_RXDESC_BE_NUM     7       /* rx descriptor number (best effort) */
#define R_AVB_RXDESC_NC_NUM     7       /* rx descriptor number (network control) */
#define R_AVB_RXDESC_S_NUM      7       /* rx descriptor number (stream) */


/***********************************************************
  number of Tx/Rx queues
*/
#define R_AVB_TXQ_NUM            4      /* tx queue number */
#define R_AVB_RXQ_NUM            4      /* rx queue number */
#define R_AVB_RXSTREAMQ_NUM      2      /* rx stream queue number */
#define R_AVB_RXQ_MAXNUM         18     /* max. rx queue number */
#define R_AVB_RXSTREAMQ_MAXNUM   16     /* max. rx stream queue number */


/***********************************************************
  Macro: R_AVB_WRITE_REG

  Write AVBC register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_AVB_REGS
    #define R_AVB_WRITE_REG(SIZE, ADDR, VALUE) {R_DBG_PrintRegCmd((SIZE), (ADDR), (VALUE));  *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#else
    #define R_AVB_WRITE_REG(SIZE, ADDR, VALUE) { *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#endif


/***********************************************************
  Macro: R_AVB_READ_REG

  Read AVBC register with predefined size.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_AVB_REGS
    #define R_AVB_READ_REG(SIZE, ADDR) (R_DBG_PrintRegRead##SIZE##(ADDR))
#else
    #define R_AVB_READ_REG(SIZE, ADDR)  (*((volatile uint##SIZE##_t*)((ADDR))))
#endif

#endif /* CONFIG_AVB_H_ */


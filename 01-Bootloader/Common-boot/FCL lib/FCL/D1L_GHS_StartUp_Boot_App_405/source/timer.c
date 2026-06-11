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
* (c) 2014 Renesas Electronics Corporation All rights reserved.
*******************************************************************************/


/******************************************************************************
* File Name     :timer.c
* Version       : 1.0
* Device(s)     : R7F701403xAFP RH850/D1L
* Tool-Chain    : GHS
* Description   : This file is a sample of the clock setting.
* Operation     : -
*******************************************************************************
*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 24.11.2016 1.00    First Release
******************************************************************************/
/******************************************************************************
Includes <System Includes> , gProject Includesh
******************************************************************************/
#include  "Device.h"
#include  "Dev_api.h"
//#include  "timer.h"

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
void   TAUB0_Init(void);


/*
============================================================================
 Sub Functions
============================================================================
*/
void TAUB0_Init(void) {                 /* initialize timer TAUB0 channel 0 to interval timer mode */
    /* set TAUB0 clock -> CLKFIX (80 MHz) */
    protected_write(SYSPROTCMDD1,SYSPROTSD1,SYSCKSC_ITAUB01S_CTL,0x01u); /* CLKFIX */
    while(0x01u !=  SYSCKSC_ITAUB01S_ACT){}

    /* Specifies the CK3 , CK2 CK1 and CK0 clock
    TAUBnTPS   - TAUBn Prescaler Clock Select Register
    b15:b12      TAUBnPRS3 - Specifies the CK3 clock - PCLK / 8
    b11:b 8      TAUBnPRS2 - Specifies the CK2 clock - PCLK / 8
    b 7:b 4      TAUBnPRS1 - Specifies the CK1 clock - PCLK / 8
    b 3:b 0      TAUBnPRS0 - Specifies the CK0 clock - PCLK / 8 */ 
	TAUB0TPS=0x0009;                      /* specify clock frequency of CK0...CK3  (CK0 -> 80 MHz / (2 ^ 7) = 625 KHz) */

    /* Controls channel m operation
    TAUBnCMORm - TAUBn Channel Mode OS Register
    b15:b14      TAUBnCKS  - Selects the operation clock                      - Selected Operation CK0
    b13                    - Reserved set to 0
    b12          TAUBnCCS0 - Selects the count clock for TAUBnCNTm counter    - Must be set to 0
    b11          TAUBnMAS  - Specifies the channel as master or slave channel
                             during synchronous channel operation             - Must be set to 0
    b10:b 8      TAUBnSTS  - Selects the external start trigger               - Must be set to 000'b
    b 7:b 6      TAUBnCOS  - Specifies when the capture register TAUBnCDRm
                             and the overflow flag TAUBnCSRm.TAUBnOVF of
                             channel m are updated.                           - Must be set to 00'b
    b 5                    - Reserved set to 0
    b 4:b 1      TAUBnMD   - Specifies the operation mode. For details, refer
                             to the settings for individual functions.        - Must be set to 000'b
    b 0          TAUBnMD0  - Specifies whether INTTAUBnIm is output at the
                             beginning of count operation (when a start
                             trigger is entered) or not.                      - INTTAUBnIm is not output */

	TAUB0CMOR0=0x0000;                    /* interval timer mode */

	 /* Data register for the capture/compare value
   TAUBnCDRm  - TAUBn Channel Data Register
   b15:b 0      TAUBnCDR  - 100ms */
	TAUB0CDR0=62500;                      /* compare register in interval timer mode (625KHz / 62500 = 10 Hz) */

	TAUB0TS=0x0001;                       /* start channel 0 */
}



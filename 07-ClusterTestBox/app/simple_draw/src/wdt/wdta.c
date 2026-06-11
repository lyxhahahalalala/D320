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
* File Name     : watchdog.c
* Version       : 1.0
* Device(s)     : R7F701404xAFP RH850/D1M1
* Tool-Chain    : GHS
* Description   : This file is for the window watchdog timer setting.
* Operation     : -
*******************************************************************************
*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 11.10.2016 1.00    First Release
******************************************************************************/

/******************************************************************************
Includes <System Includes> , ÅgProject IncludesÅh
******************************************************************************/
#include    "iodefine.h"


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/
//#define _VAC_USED_

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

void    WDT_Clear(void);


/******************************************************************************
Private global variables and functions
******************************************************************************/


/******************************************************************************
* Function Name : void R_ADC_Init( void )
* Description   : This function initializes WDT.
* Argument      : none
* Return Value  : none
******************************************************************************/
void WDT_Init(void)
{
	/* Source Clock Setting for C_AWO_WDTA
	CKSC_AWDTAD_CTL - C_AWO_WDTA Source Clock Selection Register
	b31:b 2                       - Reserved set to 0
	b 1:b 0           AWDTADCSID  - Source Clock Setting for C_AWO_WDTA  - LS IntOsc fRL (240 kHz) / 128  */

	protected_write(SYS.PROTCMD0.UINT32, SYS.PROTS0.UINT32, SYS.CKSC_AWDTA0D_CTL.UINT32, 0x00000001UL);
	while (SYS.CKSC_AWDTA0D_ACT.UINT32 != 0x00000001UL)
	{
		/* Waiting for CKSC_AWDTA0D_CTL to set. */
	}

	/* Set a mode of WDT
	WDTAnMD         - WDTA Mode Register
	b7                            - Reserved set to 0
	b6:b4             WDTAnOVF    - Selects the overflow interval time  - 512 / WDTATCKI
	                                                                      WDTATCKI = fRL(240 kHz) / 128  = 1,875Hz
	                                                                      overflow = 512*2 / WDTATCKI      = 273*2ms =546ms
	b3                WDTAnWIE    - Enables/disables the 75% interrupt
	                                request INTWDTAn.                   - INTWDTAn enabled
	b2                WDTAnERM    - Specifies the error mode.           - Reset mode
	b1:b0             WDTAnWS     - Selects the window-open period.     - 100% */
	//WDTA0.MD.UINT8                      = 0x0FU;
	WDTA0.MD.UINT8                      = 0x1FU;

	/* Set interrupt flags */
	//INTC1.EIC0.UINT16 = 0x0047;

	/* set WDTA0 start*/
#ifdef _VAC_USED_
	WDTA0.EVAC = (0xAC - WDTA0.REF);          /* start & clear */
#else
	WDTA0.WDTE.UINT8 = 0xAC;
#endif
}


/******************************************************************************
* Function Name : void R_WDT_Clear( void )
* Description   : This function clears WDT.
* Argument      : none
* Return Value  : none
******************************************************************************/
void WDT_Clear(void)
{
	/* Set a mode of WDT
	WDTAnEVAC       - WDTA Enable VAC Register
	b7:b0             WDTAnEVAC   - Control the start / restart of the WDTAn count. */
#ifdef _VAC_USED_
	WDTA0.EVAC = (0xAC - WDTA0.REF);          /* start & clear */
#else
	WDTA0.WDTE.UINT8 = 0xAC;
#endif
}


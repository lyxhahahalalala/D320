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
* File Name     : r_interrupt.c
* Version       : 1.0
* Device(s)     : R7F701403xAFP RH850/F1L
* Tool-Chain    : Ghs
* Description   : This file is a sample of the interrupt.
* Operation     : -
*******************************************************************************
*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 09.10.2016 1.00    First Release
******************************************************************************/
/******************************************************************************
Includes <System Includes> , ÅgProject IncludesÅh
******************************************************************************/
#include    "Device.h"
#include "Port.h"

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
void INTTAUB0I0(void);                   

void INTP5(void);
/*
============================================================================
 Variables
============================================================================
*/

/*
============================================================================
 Interrupt Service Routines - ISR
============================================================================
*/
__interrupt void INTTAUB0I0(void) {     /* TAUB0 interrupt service routine */
//	Port_Toggle(PORT42, PIN0);
        PORT_ISOPNOT10_0 = 1;
        PORT_ISOPNOT10_1 = 1;
	 PORT_ISOPNOT10_2 = 1;
}    


/*
============================================================================
 Interrupt Service Routines - ISR
============================================================================
*/
__interrupt void INTP5(void) {     /* TAUB0 interrupt service routine */
//	Port_Toggle(PORT42, PIN1);
	INTC1EIC11 &= 0xEFFF;
}    


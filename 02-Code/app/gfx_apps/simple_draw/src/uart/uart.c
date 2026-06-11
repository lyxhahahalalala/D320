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
* File Name     : r_uart.c
* Version       : 1.0
* Device(s)     : R7F701403AFP RH850/D1L
* Tool-Chain    :GHS Multi
* Description   : This file is a sample of the UART (RLIN30).
* Operation     : -
*******************************************************************************
*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 04.11.2016 1.00    First Release
******************************************************************************/

/******************************************************************************
Includes <System Includes> 
******************************************************************************/
#include  "iodefine.h"

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
void init_Uart( void );

/******************************************************************************
Private global variables and functions
******************************************************************************/
volatile int tmp = 0;

/******************************************************************************
* Function Name : void init_Uart( void )
* Description   : This function initialize UART. (RLIN3-UART)
* Argument      : none
* Return Value  : none
******************************************************************************/
void init_Uart(void)
{	
	/* Set UARTTX as P0_2 and UARTRX as P0_3 */

    PORT_AWO.PMC0.UINT16  |= (0x000C);
    PORT_AWO.PFCE0.UINT16 &= (0xFFF3);
    PORT_AWO.PFC0.UINT16  &= (0xFFF3);
    PORT_AWO.PM0.UINT16   &= ~(1<<2);
    PORT_AWO.PM0.UINT16   |= 1 << 3;
/**/
	
	/* Source Clock Setting for C_ISO_LIN
	CKSC_ILINS_CTL - C_ISO_LIN Source Clock Selection Register
	b31:b 2                    - Reserved set to 0
	b 1:b 0          ILINSCSID - Source Clock Setting for C_ISO_LIN  - CLKJIT/1 = 80MHz  */
	protected_write(SYS.PROTCMDD1.UINT32, SYS.PROTSD1.UINT32, SYS.CKSC_IRLINS_CTL.UINT32, 0x02);
	
	while(SYS.CKSC_IRLINS_ACT.UINT32 != 0x02)
	{
	    /* Wait for CKSC_ILINS_CTL to set. */
	}
	/* mask the interrupts */
	PBG.FSGD0BPROT0.UINT32 = 0x07FFFFFFUL;
	INTC2.EIC217.UINT16	|=	0x0080U;
	INTC2.EIC216.UINT16	|=	0x0080U;
	INTC2.EIC215.UINT16	|=	0x0080U;

	/* Choose the shift to a reset mode.
	RLN3nLCUC      - UART Control Register
	b7:b1                      - Reserved set to 0
	b0               OM0       - LIN Reset - LIN reset mode is caused */
	RLN32.LCUC.UINT8 = 0x00U;
	while((RLN32.LMST.UINT8 & 0x01U) != 0x00U)
	{
	    /* Wait for LIN reset mode. */
	}

	/* Select the UART interface mode.
	RLN3nLMD       - UART Mode Register
	b7:b6                      - Reserved set to 0
	b5               LRDNFS    - LIN Reception Data Noise Filtering Disable - Noise filter is enabled
	b4:b2                      - Reserved set to 0
	b1:b0            LMD       - LIN/UART Mode Select                       - UART mode */
	RLN32.LMD.UINT8 = 0x01U;

	/* Select the bit sampling count and prescaler clock.
	RLN3nLWBR      - LIN Wake-Up Baud Rate Select Register
	b7:b4            NSPB      - Bit Sampling Count Select  - 16 sampling
	b3:b1            LPRS      - Prescaler Clock Select     - 1/1
	b0                         - Reserved set to 0 */
	RLN32.LWBR.UINT8 = 0xF0U;

	/* Set the baud rate prescaler
	RLN3nLBRP01    - UART Baud Rate Prescaler 01 Reg
	b15:b0           BRP       - The formula for baud rate is described below.
	                             Baud rate for UART
	                             = { Frequency of LIN communication clock source} * (RLN3nLWBR.LPRS[2:0] selection clock)
	                                 / (RLN3nLBRP01 + 1) / the number of selected NSPB[3:0] [bps]
	                             = 80MHz * ( 1 / 1 ) / ( 42 + 1 ) / 16 = 11,5200(11,6279)  */
	RLN32.LBRP01.UINT16 = 44U;  // 521U=9600    84=57600   44=115200

	/* Enables or disables detection of the framing error , overrun error and bit error.
	RLN3nLEDE      - UART Error Detection Enable Register
	b7:b4                      - Reserved set to 0
	b3               FERE      - Framing Error Detection Enable - Enables framing error detection
	b2               OERE      - Overrun Error Detection Enable - Enables overrun error detection
	b1                         - Reserved set to 0
	b0               BERE      - Bit Error Detection Enable     - Enables bit error detection */
	RLN32.LEDE.UINT8                 = 0x0CU;

	/* Enables or disables detection of the framing error , overrun error and bit error.
	RLN3nLBFC      - UART Configuration Register
	b7                         - Reserved set to 0
	b6               UTPS      - UART Output Polarity Switch       - Transmit data normal output
	b5               URPS      - UART Input Polarity Switch        - Reception data normal output
	b4:b3            UPS       - UART Parity Select                - Even Parity		no parity
	b2               USBLS     - UART Stop Bit length Select       - Stop bit:1 bit
	b1               UBOS      - UART Transfer Format Order Select - LSB First
	b0               UBLS      - UART Character Length Select      - UART 8 bits communication */
	RLN32.LBFC.UINT8                 = 0x00U;

	/*RLN3nLUOR1:	UART Option Register 1
	b7:b5			-Reserved to 0
	b4		UECD	- Expansion Bit Comparison Disable	set to 0, disable
	b3		UTIGTS	- Transmission Interrupt Generation Timing Select	set to 1,Transmission interrupt is generated at the completion of transmission.
	b2		UEBDCE	- Expansion Bit Data Comparison Enable
	b1		UEBDL	- Expansion Bit Detection Level Select
	b0		UEBE	- Expansion Bit Enable	*/
	RLN32.LUOR1.UINT8	= 0x08U;

	/* Choose the cancellation of the reset mode.
	RLN3nLCUC      - UART Control Register
	b7:b1                      - Reserved set to 0
		b0               OM0       - LIN Reset - LIN reset mode is canceled */
	RLN32.LCUC.UINT8 = 0x01U;
	while((RLN32.LMST.UINT8 & 0x01U) != 0x01U)
	{
	    /* Wait for cancellation of the LIN reset mode. */
	}

	/* Set the interrupt flags */
	INTC2.EIC217.UINT16	=	0x0047;
	INTC2.EIC216.UINT16	=	0x0047;
	INTC2.EIC215.UINT16	=	0x0047;

	/* Enables reception and transmission
	RLN3nLUOER     - UART Operation Enable Register
	b7:b2                      - Reserved set to 0
	b1               UROE      - Reception Enable     - Enables reception
	b0               UTOE      - Transmission Enable  - Enables transmission */
	RLN32.LUOER.UINT8 	= 	0x03U;
}

uart_send(char *szStr, int num)
{
	unsigned int i = 0;
	for(i = 0; i < num; i++)
	{
		RLN32.LUTDR.UINT16  = (unsigned short)szStr++;
	}
}

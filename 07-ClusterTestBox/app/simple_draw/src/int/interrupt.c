/*******************************************************************************
* DISCLAIMER
* This software is supplied by FANGYAN Electronics Corporation and is only
* intended for use with FANGYAN products. No other uses are authorized. This
* software is owned by FANGYAN Electronics Corporation and is protected under
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
* FANGYAN reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.shfangyan.com/
* (c) 2017 FANGYAN Electronics Corporation All rights reserved.
*******************************************************************************/


/******************************************************************************
* File Name     : interrupt.c
* Version       : 1.0
* Device(s)     : R7F701404 RH850/D1M1
* Tool-Chain    : GHS
* Description   : This file is for the interrupt procedures.
* Operation     : -
*******************************************************************************
*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 28.02.2017 v1.00   First Release
******************************************************************************/
/******************************************************************************
Includes <System Includes> , gProject Includesh
******************************************************************************/
#include "rscan.h"
#include "iodefine.h"
#include "bsp.h"
#include "string.h"
#include "r_typedefs.h"


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
void	INTTAUB0I0_interrupt(void);   //17
void	INTTAUB0I1_interrupt(void);   //18

/******************************************************************************
Private global variables and functions
******************************************************************************/
volatile unsigned int uig_INT_TAUB0_counter;
volatile unsigned int can_id = 0;
volatile char  dlc = 0;
volatile unsigned int  msg1 = 0, msg2 = 0;
static CAN_MESSAGE message0;
static CAN_MESSAGE message1;
volatile int temp = 0;

/******************************************************************************
* Function Name : void INTTAUB0I0(void)
* Description   : This function is a INT for TAUB0.
* Argument      : none
* Return Value  : none
******************************************************************************/
#pragma interrupt priority7_interrupt(enable=false , priority=EIINT_PRIORITY7, callt=false, fpu=false)
void priority7_interrupt(void)
{
    /* TAUB0 interrupt service routine */
    uig_INT_TAUB0_counter ++;             /* count increase at interrupt occurences */
}

#pragma interrupt can0_interrupt(enable=false , channel=17, callt=false, fpu=false)
void can0_interrupt(void)
{
#if 0
    BOOL bRet = 0;
    memset((void*)&message0, 0, sizeof(CAN_MESSAGE));
    bRet = R_CAN0_Receive_RxBuf((uint32_t *)&(message0.id), (uint8_t *)&(message0.len), (unsigned int*)&(message0.data[0]), (unsigned int*)&(message0.data[4]));
    if(bRet)
    {
        Parse_Can0(&message0);
    }
#endif
}

#pragma interrupt can1_interrupt(enable=false , channel=18, callt=false, fpu=false)
void can1_interrupt(void)
{
#if 0
    BOOL bRet = 0;
    memset((void*)&message1, 0, sizeof(CAN_MESSAGE));
    bRet = R_CAN1_Receive_RxBuf(&(message1.id), &(message1.len), (unsigned int*)&(message1.data[0]), (unsigned int*)&(message1.data[4]));
    if(bRet)
    {
        Parse_Can1(&message1);
    }
#endif
}

/******************************************************************************
* Function Name : void INTTAUB1I1(void)
* Description   : This function is a INT for TAUB1.
* Argument      : none
* Return Value  : none
******************************************************************************/
//TAUB1  channel 0 for SM
#pragma interrupt ism_interrupt(enable=false , channel=33, callt=false, fpu=false)
#if 0
void ism_interrupt(void)
{
    /* TAUB1  channel 0 interrupt service routine */
	static unsigned char speed[6] = {0, 0, 0, 0, 0, 0};
/**/
	if(sm_steps[0] > 0)
	{
        if(speed[0] >= sm_speed[0])
		{
            speed[0] = 0;
			sm_run(0);
		}
		else speed[0]++;
    }
	else speed[0] = 0;
	
	if(sm_steps[1] > 0)
	{
        if(speed[1] >= sm_speed[1])
		{
            speed[1] = 0;
			sm_run(1);
		}
		else speed[1]++;
    }
	else speed[1] = 0;
/**/
}
#endif

//TAUB1  channel 1 for 
#pragma interrupt INTTAUB1I1_interrupt(enable=false , channel=34, callt=false, fpu=false)
void INTTAUB1I1_interrupt(void)
{
    /* TAUB1  channel 0 interrupt service routine */
    //suig_INT_TAUB0_counter ++;             /* count increase at interrupt occurences */
}

/******************************************************************************
* Function Name : void INTTAUJ1(void)
* Description   : This function is a INT for TAUJ1.
* Argument      : none
* Return Value  : none
******************************************************************************/
//TAUJ0  channel 0    -- for buzzer
#pragma interrupt INTTAUJ0I0_interrupt(enable=false , channel=121, callt=false, fpu=false)
void INTTAUJ0I0_interrupt(void)
{
    //PORT_ISO.P3.UINT16 = (PORT_ISO.P3.UINT16 &~ 0x0200) | (PORT_ISO.P3.UINT16 ^ 0x0200);
}

//TAUJ0  channel 1    -- for 10ms timer
#pragma interrupt t10mm_interrupt(enable=false , channel=122, callt=false, fpu=false)
void t10mm_interrupt(void)
{
    //PORT_ISO.P3.UINT16 = (PORT_ISO.P3.UINT16 &~ 0x0200) | (PORT_ISO.P3.UINT16 ^ 0x0200);
    Interrupt10ms();
}

/******************************************************************************
* Function Name : void WDTA(void)
* Description   : This function is for watchdog procedure.
* Argument      : none
* Return Value  : none
******************************************************************************/
//WDTA
#pragma interrupt INTWDTA0_interrupt(enable=false , channel=0, callt=false, fpu=false)
void INTWDTA0_interrupt(void)
{
    /* WDTA interrupt service routine */
    uig_INT_TAUB0_counter ++;             /* count increase at interrupt occurences */
}


/*
============================================================================
 Interrupt Service Routines - ISR
============================================================================
*/
unsigned char rvBuffer[64] = {0};
unsigned int  iCount       = 0;
/* RLIN3 UART transmission interrupt service routine */
#pragma interrupt INTRLIN32UR0(enable=false, channel=217, callt=false, fpu=false)
void INTRLIN32UR0(void)
{
	temp = RLN32.LEST.UINT8;  //INTC2.EIC217.UINT16 &= 0xEFFF;
	rvBuffer[iCount++] = RLN32.LURDR.UINT8[0];
	temp = RLN32.LURDR.UINT8[0];
	temp = RLN32.LURDR.UINT8[0];
}

/* RLIN3 UART reception interrupt service routine */
#pragma interrupt INTRLIN32UR1(enable=false, channel=216, callt=false, fpu=false)
void INTRLIN32UR1(void)
{
	//RLN32.LUTDR.UINT16 = RLN32.LURDR.UINT16;
	temp = RLN32.LEST.UINT8;  //INTC2.EIC216.UINT16 &= 0xEFFF;
	temp = RLN32.LURDR.UINT16 & 0x00FF;
	temp = RLN32.LURDR.UINT16 & 0x00FF;
}

/* RLIN3 UART status interrupt service routine */
#pragma interrupt INTRLIN32UR2(enable=false, channel=215, callt=false, fpu=false)
void INTRLIN32UR2(void)
{
	temp = RLN32.LEST.UINT8;
	if (RLN32.LEST.UINT8 & 0x4CU)	/* Overrun error, Framing error, Parity error  */
	{
	    temp = RLN32.LST.UINT8;//dummy            = RLN32.LURDR.UINT8;        /* Clear UART reception data register  */
	}
	/* Clear error status
	RLN3nLEST  - UART Error Status Register
	b7                   - Reserved set to 0
	b6           UPER    - Parity Error Flag           - Clear the Parity Error Flag
	b5           IDMT    - ID Matching Flag            - Clear the ID Matching Flag
	b4           EXBT    - Expanded Bit Detection Flag - Clear the Expanded Bit Detection Flag
	b3           FER     - Framing Error Flag          - Clear the Framing Error Flag
	b2           OER     - Overrun Error Flag          - Clear the Overrun Error Flag
	b1                   - Reserved set to 0
	b0           BER     - Bit Error Flag              - Clear the Bit Error Flag */
	RLN32.LEST.UINT8 = 0x00U;
}

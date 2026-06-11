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
* File Name     : timer.c
* Version       : 1.0
* Device(s)     : R7F701403xAFP RH850/D1L
* Tool-Chain    : GHS
* Description   : This file is a sample of the clock setting.
* Operation     : -
*******************************************************************************
*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 25.10.2016 1.00    First Release
******************************************************************************/
/******************************************************************************
Includes <System Includes> , gProject Includesh
******************************************************************************/
#include    "iodefine.h"


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
void   init_timers( void );

/******************************************************************************
Private global variables and functions
******************************************************************************/
static	void	init_TAUB0C0C1(void);
static	void	init_TAUB1C0C1(void);
static	void	init_TAUJ0C1(void);

volatile static int tmp;

/******************************************************************************
* Function Name : void init_timers( void )
* Description   : This function initializes the clock.
* Argument      : none
* Return Value  : none
******************************************************************************/
void PBG_disable1(void)
{
    PBG.FSGD0BPROT0.UINT32 = 0x07FFFFFF;
    PBG.FSGD0BPROT1.UINT32 = 0x07FFFFFF;
}

void init_timers(void)
{
    PBG_disable1();
	
    protected_write(SYS.PROTCMDD1.UINT32, SYS.PROTSD1.UINT32, SYS.CKSC_ITAUB01S_CTL.UINT32, 0x01u); /* TAUB0 clock = CLKFIX/1 = 80 MHz)*/
    while(0x01u !=  SYS.CKSC_ITAUB01S_ACT.UINT32){}
    
    init_TAUB0C0C1();
    init_TAUB1C0C1();
    init_TAUJ0C1();
}


/******************************************************************************
* Function Name : static void init_TAUB0( void )
* Description   : This function sets the TAUB channel 0 to interval timer mode
* Argument      : none
* Return Value  : none
******************************************************************************/
static void init_TAUB0C0C1(void)
{
    //channel-0
    TAUB0.TPS.UINT16   = 0x000Cu;                    /* specify clock frequency of CK0...CK3  (CK0 -> 80 MHz / 128(2 ^ 7) = 625 KHz) */
    TAUB0.CMOR0.UINT16 = 0x0000u;                    /* bit10-8 = 000 interval timer function */
    TAUB0.CDR0         = 6250u;                     /* compare register in interval timer mode (625KHz / 6250 = 100 Hz = 10ms) */  
    INTC1.EIC17.UINT16 = 0x0047u;		     /* unmasked timer TAUB0 chanel 0 interrupt(17) 
										and enabled reference table jump method
										and seted interrupt priority levels = 7*/
										
    //channel-1
    TAUB0.CMOR1.UINT16 = 0x0000u;                    /* bit10-8 = 000 interval timer function */
    TAUB0.CDR1         = 6250u;                     /* compare register in interval timer mode (625KHz / 62500 = 10 Hz = 100ms) */
    INTC1.EIC18.UINT16 = 0x0047u;
	
    TAUB0.TS.UINT16    = 0x0003u;                    /* start TAUB0 channel 0 1 */
}

/******************************************************************************
* Function Name : static void init_TAUB1( void )
* Description   : This function sets the TAUB channel 0 to interval timer mode
* Argument      : none
* Return Value  : none
******************************************************************************/
static void init_TAUB1C0C1(void)
{
    TAUB1.TPS.UINT16   = 0x0007u;                    /* specify clock frequency of CK0...CK3  (CK0 -> 80 MHz / 128(2 ^ 7) = 625 KHz) */
    TAUB1.CMOR0.UINT16 = 0x0000u;                    /* bit10-8 = 000 interval timer function */
    TAUB1.CDR0         = 250u;                     /* compare register in interval timer mode (625KHz / 62500 = 10 Hz = 100ms) */  
    INTC2.EIC33.UINT16 = 0x0047u;

    TAUB1.CMOR1.UINT16 = 0x0000u;                    /* bit10-8 = 000 interval timer function */
    TAUB1.CDR1         = 62500u;                     /* compare register in interval timer mode (625KHz / 62500 = 10 Hz = 100ms) */
    INTC2.EIC34.UINT16 = 0x0047u;

    TAUB1.TS.UINT16    = 0x0003u;                    /* start TAUB0 channel 0 1 */
}

/******************************************************************************
* Function Name : static void init_TAUJ0( void )
* Description   : This function sets the TAUJ channel 0 to interval timer mode
* Argument      : none
* Return Value  : none
******************************************************************************/
static void init_TAUJ0C1(void)
{
    /* initialize timer TAUJ0 channel 1 to interval timer mode */
    TAUJ0.CMOR1.UINT16  = 0x0000u;                    /* bit10-8 = 000 interval timer function */
    TAUJ0.CDR1          = 6250u;                      /* compare register in interval timer mode (625KHz / 62500 = 10 Hz = 100ms) */  
    INTC2.EIC122.UINT16 = 0x0047u;
    TAUJ0.TS.UINT8    |= 0x02u;                    /* start TAUJ0 channel 1 */
}

void start_TAUJ0_C1()
{
    TAUJ0.TS.UINT8    |= 0x02u;                    /* start TAUJ0 channel 1 */
}

void stop_TAUJ0_C1()
{
    TAUJ0.TT.UINT8    |= 0x02u;                    /* stop  TAUJ0 channel 1 */
}



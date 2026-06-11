/******************************************************************************
* File Name     : ostm0.c
* Version       : 1.00
* Device(s)     : RH850F1L
* Debugger    : GreenHills
* Description   : This file include the UART function
* Creation Date : 2013/2/27
*******************************************************************************/
#include "types.h"
#include "device.h"
#include "ostm0.h"
#include "Cpu.h"
#include "upgrade.h"
#include "bt_pub.h"
#include "bt_uds.h"

#if defined _USE_CSPlus
#pragma ioreg
#elif defined _USE_GHS
#include "device.h"
#endif 



uint32_t systemTimer_1ms;

/******************************************************************************
** Function: 	void OSTM0_INIT(void)
** Description: Initialize OSTIMER.
** Parameter:	None
** Return:  	None
******************************************************************************/
void OSTM0_INIT(void)
{
	/*C_ISO_OSTM source clock selection register
	bit32:0	reserved
	bit1:0	Source clock selection for C_ISO_OSTM
			00B: Disabled
			01B: CLKJIT
			10B: C_ISO_CPUCLK /4 (default)
			11B: MainOsc fX
	*/
	protected_write(SYSPROTCMDD1, SYSPROTSD1, SYSCKSC_IOSTMS_CTL, 0x03);
	while(SYSCKSC_IOSTMS_ACT==0x0){};
	
	/* OSTMnCMP — OSTMn Compare Register
	In interval timer mode: start value of the down-counter
	In free-run compare mode: compare value */
	OSTM0CMP = (8*1000); /* 8000*(1/8000000) = 1ms*/
	
	/* OSTMnCTL — OSTMn Control Register
	bit1 OSTMnMD1  Specifies the operating mode for the counter:
		 0: Interval timer mode		---set
		 1: Free-run compare mode
	bit0 OSTMnMD0  Controls the generation of OSTMTINT interrupt requests at the start of counting:
		 0: Interrupts when counting starts are disabled.	---set
		 1: Interrupts when counting starts are enabled. */
	OSTM0CTL = 0x00;
	
	//enable INTOSTM0
	PBGFSGD0BPROT0 = 0x07FFFFFFUL;			
	INTC2EIC125 = 0x47;
	
	systemTimer_1ms = 0;
	
}

/******************************************************************************
** Function: 	void OSTM0_START(void)
** Description: Start the counter .
** Parameter:	None
** Return:  	None
******************************************************************************/
void OSTM0_START(void)
{
	OSTM0TS = 0X01; //Start the counter.
	while((OSTM0TE & 0X01) != 0X01); //Wait whether the counter is enable.
}


/******************************************************************************
** Function: 	void OSTM0_STOP(void)
** Description: Stop the counter.
** Parameter:	None
** Return:  	None
******************************************************************************/
void OSTM0_STOP(void)
{
    OSTM0TT = 0x01; //Stop the counter.
    while((OSTM0TE & 0X01) != 0X00); //Wait whether the counter is disable.
}


/******************************************************************************
** Function: 	void _intostm0()
** Description: Used for interrupt.
** Parameter:	None
** Return:  	None
******************************************************************************/
#pragma ghs section text = ".R_FCL_CODE_USRINT"
 __interrupt void INTOSTM0()
{
	uint32_t cnt1msTask = 0;
	
    cnt1msTask++;
	
	if(udsProgRequest)
	{
		if(eEraseActive == Erase_Active)
		{
			systemTimer_1ms+=100;
		}
		else
		{
			systemTimer_1ms++;
		}
		
		BT_DoChanges_Proc(); //检查是否有任何BT做改变是必要的。
		BT_Tp_Proc();
		BT_Uds_Proc();
		Can_MainFunction_Write();
		
	}
	
}
    
/* __interrupt void EINTPRIO_0()
{
    static unsigned char ticks1 = 0;
    if(__STSR( EIIC )== 0x107D)
    {
		// OSTM0_INT_CB();
		ticks1++;
		if(ticks1>9)
		{
			ticks1=0;
			// PORT_ISOP42 ^= (1<<0);
			// PORT_ISOP42 ^= (1<<1);
		}
    }
} */



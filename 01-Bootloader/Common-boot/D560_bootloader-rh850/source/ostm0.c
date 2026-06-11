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

/******************************************************************************
** Function: 	void OSTM0_INIT(void)
** Description: Initialize OSTIMER.
** Parameter:	None
** Return:  	None
******************************************************************************/
void OSTM0_INIT(uint32_t compare_value)
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
	
	/*OSTMnCMP ¡ª OSTMn Compare Register
	In interval timer mode: start value of the down-counter
	In free-run compare mode: compare value
	*/
	OSTM0CMP = (8*1000*compare_value);		/*1/8000000*800000 = 100ms*/
	/*OSTMnCTL ¡ª OSTMn Control Register
	1 	OSTMnMD1 	Specifies the operating mode for the counter:
		0: Interval timer mode			---set
		1: Free-run compare mode
	0 	OSTMnMD0 	Controls the generation of OSTMTINT interrupt requests at the start of counting:
		0: Interrupts when counting starts are disabled.
							---set
		1: Interrupts when counting starts are enabled.
	*/
	OSTM0CTL = 0x00;
	
	PBGFSGD0BPROT0 = 0x07FFFFFFUL;			
	INTC2EIC125 = 0x47;					//enable INTOSTM0
}


/******************************************************************************
** Function: 	void OSTM0_DEINIT(void)
** Description: Enable the interrupt.
** Parameter:	None
** Return:  	None
******************************************************************************/
void OSTM0_DEINIT(void)
{
	INTC2EIC125 = 0x0047;
}


/******************************************************************************
** Function: 	void OSTM0_START(void)
** Description: Start the counter .
** Parameter:	None
** Return:  	None
******************************************************************************/
void OSTM0_START(void)
{
	OSTM0TS = 0X01;				  		                    //Start the counter.
	while((OSTM0TE & 0X01) != 0X01);	                    //Wait whether the counter is enable.
}


void OSTM0_enable_int(void) 
{
	PBGFSGD0BPROT0 = 0x07FFFFFFUL;			
	INTC2EIC125 = 0x0047;
}


/******************************************************************************
** Function: 	void OSTM0_STOP(void)
** Description: Stop the counter.
** Parameter:	None
** Return:  	None
******************************************************************************/
void OSTM0_STOP(void)
{
    OSTM0TT = 0x01;                                         //Stop the counter.
    while((OSTM0TE & 0X01) != 0X00);                        //Wait whether the counter is disable.
}




/******************************************************************************
** Function: 	void _intostm0()
** Description: Used for interrupt.
** Parameter:	None
** Return:  	None
******************************************************************************/
#if defined _USE_CSPlus                                    //use Cubesuite+ compiler                                                                                             
    #pragma interrupt INTOSTM0 _intostm0
    void _intostm0(void)
#elif defined _USE_GHS                                     //use Green hills compiler
    __interrupt void INTOSTM0()
#endif
{
   OSTM0_INT_CB();
}
    
__interrupt void EINTPRIO_0()
{
    static unsigned char ticks1;
    if(__STSR( EIIC )== 0x107D)
    {
	  OSTM0_INT_CB();
#if 1
	  ticks1++;
	  if(ticks1>9)
	  {
	      ticks1=0;
//	      PORT_ISOP42 ^= (1<<0);
//	      PORT_ISOP42 ^= (1<<1);
	  }
#endif
    }
}



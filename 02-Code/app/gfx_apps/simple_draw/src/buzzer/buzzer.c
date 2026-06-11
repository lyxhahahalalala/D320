/******************************************************************************
  Includes <System Includes> 
  ******************************************************************************/
#include "iodefine.h"
#include "r_typedefs.h"


void TAUB_Init(void)
{
	/* for TAUB0 channel 0 clock source */
	/* set TAUB0 clock -> CLKFIX (80 MHz) */
	
	/* TAUB0 clock = CLKFIX/8 = 10 MHz)*/
	//protected_write(SYS.PROTCMDD1.UINT32, SYS.PROTSD1.UINT32, SYS.CKSC_ITAUB01S_CTL.UINT32,0x02u);
	//while(0x01u !=  SYS.CKSC_ITAUB01S_ACT.UINT32);  
	
	TAUB0.TPS.UINT16=0x0006u;      /* specify clock frequency of CK0...CK3  (CK0 -> 10 MHz / 256(2 ^ 8) = 39 KHz) */
	// TAUB0.CMOR3.UINT16=0x1006u;    /* bit4-0 = 00110  Event count function   bit 7-6 = 00 Updated upon detection of a TAUBTTINm input valid edge.*/
	// TAUB0.CMUR3.UINT8=0x01u;       /*rising edge effective*/
	// TAUB0.CDR3 =0;                 /*When effective edges are detected (TAUBnCDRm + 1) times, INTTAUBnIm is generated TAUBnCNTm then loads the TAUBnCDRm value and subsequently continues to operat*/
	
	TAUB0.CMOR5.UINT16 = 0x1006u;  /* bit10-8 = 000 interval timer function */
	TAUB0.CDR5=0;                  /* compare register in interval timer mode (625KHz / 625 = 1000 Hz = 1ms) */
	TAUB0.CMUR5.UINT8  = 0x01u;
	// INTC1.EIC20.UINT16 = 0x0047u;  /* unmasked timer TAUB0 chanel 0 interrupt(17) and enabled reference table jump method and seted interrupt priority levels = 7*/
	INTC1.EIC22.UINT16 = 0x0047u;
	TAUB0.TS.UINT16    = 0x0020u;  /* start TAUB0 channel 5 */    
	
}

void TAUB_DeInit(void)
{
	TAUB0.TS.UINT16 = 0x0000u;
}




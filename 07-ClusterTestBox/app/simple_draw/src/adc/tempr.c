
#include "iodefine.h"

/* initialize timer TAUB0 channel 0 to interval timer mode */
void init_TAUB0(void)
{
	TAUB0.TPS.UINT16   = 0x0006;                      /* specify clock frequency of CK0...CK3  (CK0 -> 40MHz / (2 ^ 6) = 625KHz) */
	TAUB0.CMOR0.UINT16 = 0x0000;                      /* interval timer mode */
	TAUB0.CDR0         = 62500;                       /* compare register in interval timer mode ( 625KHz \ 65535 =  10 Hz) */
	TAUB0.CMOR2.UINT16 = 0x0000;                      /* interval timer mode */
	TAUB0.CDR2         = 62500;                       /* compare register in interval timer mode ( 625KHz \ 65535 =  10 Hz) */
	TAUB0.TS.UINT16    = 0x0001;                      /* start channel 0 */
}

void init_TempS(void)
{
	//INTC1EIC19   = 0x0047;   /* demask timer TAUB0 channel 2 interrupt and enable reference table jump method */
	//ADCE0TSNVCR    = 0x65;   // Temp sensor physical channel number = 37 and Upper Limit checked by ADCE0ULLMTBR0

	ADCE0.TSNVCR.UINT32     = 0x25;       // Temp sensor physical channel number = 37
	ADCE0.TSNSGTSEL.UINT32  = 0x10;       // Select AD conversion Trigger TAUB0 Channel 2 TTSNSEL4
	ADCE0.TSNSMPCR.UINT32   = 0xF0;       // 0xf0 6 if 40MHz
	ADCE0.TSNSGCR.UINT32    = 0x1;        // TSNTRGMD = 1 ADC start trigger <- waits for TAUB0I2
	//ADCE0SGSTCR0   = 0x1;             // SGACT should be 0
	SELB.TSNCNTCMP          = 0x1f40;     // 8000 CLks of 40 MHz -> 200
	SELB.TSNCR.UINT32       = 0x1;
	while(!SELB.TSNSTAT.UINT32);
	TAUB0.TS.UINT16  = 0x4;              // Enable counter of channel 2 when temp sensor output is stable
}

unsigned int get_Temp(void)
{
   volatile unsigned int adcVal;
   volatile unsigned HVal, LVal;
   volatile float temp;

   HVal = (SCDS0.OPBT10.UINT32  >> 20);
   LVal = (SCDS0.OPBT10.UINT32  & 0xfff00) >> 8;
   
   adcVal = ADCE0.PWDTSNDR.UINT32;
   temp = (float)(125.0 /((float)HVal - (float)LVal)) * (signed int)(adcVal - LVal) + 25;
 
   return (unsigned int)temp;
}



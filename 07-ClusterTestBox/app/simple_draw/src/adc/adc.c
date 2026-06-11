/*
============================================================================
 PROJECT: Can project
============================================================================
                           C O P Y R I G H T
============================================================================
 Copyright (c) 2017 by SHFY. All rights reserved.
 
============================================================================


============================================================================
 Enviroment:  Devices:     RH850/D1M1 / R7F701404
              Debugger:    GHS Multi
============================================================================
*/

/******************************************************************************
Macro definitions
******************************************************************************/
#include    "iodefine.h"


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
void    ADC0_sg1_start(void);


/*
============================================================================
 Interrupt Service Routines - ISR
============================================================================
*/

/*ADCE0 Scan group 1 read () */
int adce_read()    //only for test 
{
    volatile unsigned int iRet = 0, i = 0;
    while(1)
    {
        ADC0_sg1_start();
        iRet = (ADCE0.DIR00.UINT32 & 0x0fff);ADC0_sg1_start();
        iRet = (ADCE0.DIR01.UINT32 & 0x0fff);ADC0_sg1_start();
        iRet = (ADCE0.DIR02.UINT32 & 0x0fff);ADC0_sg1_start();
        iRet = (ADCE0.DIR03.UINT32 & 0x0fff);ADC0_sg1_start();
        iRet = (ADCE0.DIR04.UINT32 & 0x0fff);ADC0_sg1_start();
        iRet = (ADCE0.DIR05.UINT32 & 0x0fff);ADC0_sg1_start();
	 iRet = (ADCE0.DIR06.UINT32 & 0x0fff);
    } 
}

void module_reset_release()
{
    protected_write(SYS.PROTCMDMRST.UINT32, SYS.PROTSMRST.UINT32, SYS.MRSTC.UINT32,(0x00003fff));
}

void PBG_disable(void)
{
    PBG.FSGD0BPROT0.UINT32 = 0x07FFFFFF;
    PBG.FSGD0BPROT1.UINT32 = 0x07FFFFFF;
}

/*ADCE0 Scan group 1 read () */
unsigned int ADC_ReadChannelValue(unsigned char index)
{
    volatile unsigned int iRet = 0;

    //adce_read();     //for ad test using
    
   ADC0_sg1_start();
    switch(index)
    {
		case 0:
		    iRet = (ADCE0.DIR00.UINT32 & 0x0fff);
		    break;
		case 1:
		    iRet = (ADCE0.DIR01.UINT32 & 0x0fff);
		    break;
		case 2:
		    iRet = (ADCE0.DIR02.UINT32 & 0x0fff);
		    break;
		case 3:
		    iRet = (ADCE0.DIR03.UINT32 & 0x0fff);
		    break;
		case 4:
		    iRet = (ADCE0.DIR04.UINT32 & 0x0fff);
		    break;
		case 5:
		    iRet = (ADCE0.DIR05.UINT32 & 0x0fff);
		    break;
		case 6:
		    iRet = (ADCE0.DIR06.UINT32 & 0x0fff);
		    break;
		case 7:
		   iRet = (ADCE0.DIR07.UINT32 & 0x0fff);
		    break;
		case 8:
		    iRet = (ADCE0.DIR08.UINT32 & 0x0fff);
		    break;
		case 9:
		    iRet = (ADCE0.DIR09.UINT32 & 0x0fff);
		    break;
		case 10:
		    iRet = (ADCE0.DIR10.UINT32 & 0x0fff);
		    break;
		case 11:
		    iRet = (ADCE0.DIR11.UINT32 & 0x0fff);
		    break;
		case 12:

		default:
		    break;
    }
    return iRet;
}

void ADC_Init(void)
{
    module_reset_release();
    PBG_disable();
    
    ADCE0.VCR00.UINT32 = 0x000000010;
    ADCE0.VCR01.UINT32 = 0x000000011;
    ADCE0.VCR02.UINT32 = 0x000000012;
    ADCE0.VCR03.UINT32 = 0x000000013;
    ADCE0.VCR04.UINT32 = 0x000000014;
    ADCE0.VCR05.UINT32 = 0x000000015;
    ADCE0.VCR06.UINT32 = 0x000000016;
    ADCE0.VCR07.UINT32 = 0x000000017;
    ADCE0.VCR08.UINT32 = 0x000000018;
    ADCE0.VCR09.UINT32 = 0x000000019;
    ADCE0.VCR10.UINT32 = 0x00000001A;
    ADCE0.VCR11.UINT32 = 0x00000001B;
    
    /* Bits[5..0]=0 -> Select AP0_0 as Physical Channel (Can set Self-diagnostic mode)
                                      Bits[7,6]=00b -> No Upper Limit/Lower Limit  check
                             Bit[8]=0 -> disable A/D Conversion End Interrupt Enable 
                             Bit[9]=0 -> A/D conversion of the hold value is performed during a self-diagnostic
                             Bit[14..12]=0 -> do not set the MPX value to be transferred to an external analog multiplexer.
                             Bit[15]=0 -> do not enable MPX
                         */
    
    ADCE0.ADCR.UINT32 = 0x00000002;   /* Bits[1,0]=10b -> Asynchronous suspend
                                    Bit[4]=0 -> CTYP: 12-bit mode
                             Bit[5]=0 -> CRAC: PWDDR and ADCAnDRj are set to right align
                             Bit[7]=0 -> The self-diagnostic voltage circuit is turned off
                         */
    
    ADCE0.SMPCR.UINT32 = 0x00000012;  /* Bits[7..0]=00010010b   Set sampling time to 24cycles/40MHz */
 
    ADCE0.SFTCR.UINT32 = 0x00000000;  /* Bit[2]=0 -> Overwrite Error Interrupt disabled
                             Bit[3]=0 -> Upper Limit/Lower Limit Error Interrupt disabled
                             Bit[4]=0 -> When the A/D conversion result is read, the A/D conversion result is not cleared by hardware.
                          */
  
    ADCE0.DGCTL0.UINT32=0x00000000;   /*  No setting for Self-Diagnostic voltage level */
    ADCE0.DGCTL1.UINT32=0x00000000;   /*  No channels selected for Self-Diagnostic */
  
    ADCE0.PDCTL1.UINT32=0x00000000;   /*  Bits[15..0]=0 -> ANIn[00:15] Pull-down enable is off */
    ADCE0.PDCTL2.UINT32=0x00000000;   /*  Bits[19..0]=0 -> ANI0[16:35], ANI1[16:23] Pull-down enable is off*/

    ADCE0.SGTSEL1.UINT32=0x00000000;  /*  Bits[8..0]=0 ->External input HW trigger is disabled   */
    ADCE0.SGTSEL2.UINT32=0x00000000;  /*  Bits[8..0]=0 ->External input HW trigger is disabled   */
    ADCE0.SGTSEL3.UINT32=0x00000000;  /*  Bits[8..0]=0 ->External input HW trigger is disabled   */

    ADCE0.SGVCSP1.UINT32=0x00000000;  /*  Bits[5..0]=0 -> select the virtual channel 0 from which the scan is to be started */
    ADCE0.SGVCEP1.UINT32=0x0000000b;  /*  Bits[5..0]=B -> select the virtual channel 0 from which the scan is to be ended */  
    ADCE0.SGMCYCR1.UINT32=0x00000003; /*  Bits[1,0]=3 -> specify the number  of times for scanning in multicycle scan mode */
  
    ADCE0.SGCR1.UINT32=0x00000018;    /*  Bit[0]=0 -> SGx_TRG trigger is disabled
                             Bits[3,2]=0 -> Channel repeat times is one
                             Bit[4]=1 -> INT_SGx is output when the scan for SGx ends
                             Bit[5]=0 -> Multicycle scan mode
                             */
			     
    //INTC2.EIC66.UINT16 = 0x004f;       /* enable Scan group 1 interrupt (INTADCE0I1) with reference table jump  */
    ADC0_sg1_start();
}

void ADC0_sg1_start(void)
{

    ADCE0.SGSTCR1.UINT32 = 0x1; /* Scan Group 1 Start Trigger */
	
		
    for(int i = 0; i < 3000; i++);       //delay a while
		
}

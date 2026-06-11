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
* File Name     :buzzer.c
* Version       : 1.0
* Device(s)     : R7F701403xAFP RH850/D1L
* Tool-Chain   : GHS
* Description   : This file is a sample of the clock setting.
* Operation     : -
*******************************************************************************
*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 08.10.2016 1.00    First Release
******************************************************************************/
/******************************************************************************
Includes <System Includes> , gProject Includesh
******************************************************************************/
#include    "iodefine.h"
#include    "r_typedefs.h"

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
void   init_buzzer(void);
void   start_buzzer(void);
void   stop_buzzer(void);
static void init_TAUJ0C0(void);

float Duty;


/******************************************************************************
* Function Name : void init_buzzer(void)
* Description   : This function inits port3-9 for buzzer pulse input.
* Argument      : none
* Return Value  : none
******************************************************************************/
void init_buzzer(void)  /* initialize ports */
{  
    /* Set a port mode for input port mode or output port mode
    PMn    - Port Mode Register
    b15:b0   PMn_n    - Specifies input/output mode of
                        the corresponding pin.                - set to input port mode            */
    PORT_ISO.PM3.UINT16               &= 0xfdff;  //output

    /* Set a port mode or alternative mode.
    PMCn   - Port Mode Control Register
    b15:b0   PMCn_n   - Specifies the operation mode of
                        the corresponding pin.                - set to port mode                  */
    PORT_ISO.PMC3.UINT16              &= 0xfdff;

    /* Set an input buffer for enables or disables.
    PIBCn  - Port Input Buffer Control Register
    b15:b0   PIBCn_n  - Enables/disables the input buffer.    - set to input buffer enable        */
    PORT_ISO.P3.UINT16  = 0x0200;
    
    PBG.FSGD0BPROT0.UINT32 = 0x07FFFFFF;  //for INT2 operation need
    PBG.FSGD0BPROT1.UINT32 = 0x07FFFFFF;
    init_TAUJ0C0();
}


/******************************************************************************
* Function Name : static void init_TAUJ0( void )
* Description   : This function sets the TAUJ channel 0 to interval timer mode
* Argument      : none
* Return Value  : none
******************************************************************************/
static void init_TAUJ0C0(void)
{
    /* initialize timer TAUJ0 channel 0 to interval timer mode */

    /* for TAUJ0 channel 0 clock source */
    /* set TAUJ0 clock -> CLKFIX (80 MHz) */
    protected_write(SYS.PROTCMDD1.UINT32, SYS.PROTSD1.UINT32, SYS.CKSC_ITAUJS_CTL.UINT32, 0x01u); /* TAUJ0 clock = CLKFIX/1 = 80 MHz)*/
    while(0x01u !=  SYS.CKSC_ITAUJS_ACT.UINT32){}

    TAUJ0.TPS.UINT16    = 0x0007u;                    /* specify clock frequency of CK0...CK3  (CK0 -> 80 MHz / 128(2 ^ 7) = 625 KHz) */
    TAUJ0.CMOR0.UINT16  = 0x0000u;                    /* bit10-8 = 000 interval timer function */
    TAUJ0.CDR0          = 1000u;                     /* compare register in interval timer mode (625KHz / 62500 = 10 Hz = 100ms) */  
    INTC2.EIC121.UINT16 = 0x0047u;		     /* unmasked timer TAUJ0 chanel 0 interrupt(121) and enabled reference table jump method */											
}

void start_timer()
{
    TAUJ0.TS.UINT8    |= 0x0Fu;                    /* start TAUJ0 channel 0 */
}

void stop_buzzer()
{
    TAUJ0.TT.UINT8    |= 0x01u;                    /* stop  TAUJ0 channel 0 */
}

void TAUB_Init(void)
{
  /* for TAUB0 channel 0 clock source */
  /* set TAUB0 clock -> CLKFIX (80 MHz) */

  //protected_write(SYS.PROTCMDD1.UINT32, SYS.PROTSD1.UINT32, SYS.CKSC_ITAUB01S_CTL.UINT32,0x02u); /* TAUB0 clock = CLKFIX/8 = 10 MHz)*/
   //while(0x01u !=  SYS.CKSC_ITAUB01S_ACT.UINT32){}  

  TAUB0.TPS.UINT16=0x0006u;                         /* specify clock frequency of CK0...CK3  (CK0 -> 10 MHz / 256(2 ^ 8) = 39 KHz) */
  TAUB0.CMOR7.UINT16=0x1006u;                    /* bit4-0 = 00110  Event count function   bit 7-6 = 00 Updated upon detection of a TAUBTTINm input valid edge.*/
   /* bit10-8 = 000 Software trigger   bit 12 = 1 count clock:Valid edge of TAUBTTINm input signal
	bit 11 = 0 slave  0001 0000 0000 0110
   */
  TAUB0.CMUR7.UINT8=0x01u;       			/*rising edge effective*/
  TAUB0.CDR7=0;                                       /*When effective edges are detected (TAUBnCDRm + 1) times, INTTAUBnIm is generated
										TAUBnCNTm then loads the TAUBnCDRm value and subsequently continues to operat
   										*/

  TAUB0.CMOR5.UINT16=0x1006u;                    /* bit10-8 = 000 interval timer function */
  TAUB0.CDR5=0;                                            /* compare register in interval timer mode (625KHz / 625 = 1000 Hz = 1ms) */
  TAUB0.CMUR5.UINT8=0x01u;   
  INTC1.EIC24.UINT16 = 0x0047u;			/* unmasked timer TAUB0 chanel 0 interrupt(17) 
										     and enabled reference table jump method and seted interrupt priority levels = 7*/
  INTC1.EIC22.UINT16 = 0x0047u;											
											
  TAUB0.TS.UINT16     =0x00A0u;                       /* start TAUB0 channel 5 7 */   

}
void PWM_Init( void )
{
	/*The INTC2 registers are not accessible per default as these registers are guarded by
	    the PBUS Guard PBG0B. Before accessing any of these registers the PBG0B protection
	    register FSGD0BPROT0 has to be set to 07FF FFFFH.*/
	//PBGFSGD0BPROT0 = 0x07FFFFFFUL;	
	//INTC2EIC141 |= 0x0080; 		/* demask PWGA0 interrupt and enable reference table jump method */


	/* Setting of the ADCA corresponding to each channel.
	PWSAnPVCRx_y - PWSAnPVCRx_y Register
	b31:b29                   - Reserved set to 0
	b28            PWSAnSLADy - ADCTL Select (odd-numbered channel)                  - Unused. Set to 0'b
	b27            PWSAnVRDTy - This bit indicates the setting value of the
	                            ADCAnPWDVCR.MPXE bit (oddnumbered channel).           - The use of MPX is prohibited.
	b26:b24        PWSAnVRDTy - This bit indicates the setting value of the
	                            ADCAnPWDVCR.MPXV[2:0] bits (oddnumbered channel).     - Unused. Set to 000'b. 
	b23:b22        PWSAnVRDTy - This bit indicates the setting value of the
	                            ADCAnPWDVCR.ULS[1:0] bits (oddnumbered channel).      - Unused. Set to 0'b
	b21:b16        PWSAnVRDTy - This bit indicates the setting value of the
	                            ADCAnPWDVCR.GCTRL[5:0] bits (odd-numbered channel).   - Unused. Set to 0'b
	b15:b13                   - Reserved set to 0
	b12            PWSAnSLADx - ADCTL Select (even-numbered channel)                  - Unused. Set to 0'b.
	b11            PWSAnVRDTx - This bit indicates the setting value of the 
	                            ADCAnPWDVCR.MPXE bit. (evennumbered channel)          - Unused. Set to 0'b.
	b10:b 8        PWSAnVRDTx - These bits indicate the setting value of the
	                            ADCAnPWDVCR.MPXV[2:0] bits. (even-numbered channel)   - Unused. Set to 0'b.
	b 7:b 6        PWSAnVRDTx - These bits indicate the setting value of the
	                            ADCAnPWDVCR.ULS[1:0] bits. (even-numbered channel)   - Upper limit and lower limit are
	                                                                                    checked for ADCAnULLMTBR0.
	                                                                                    Set to 01'b.
	b 5:b 0        PWSAnVRDTx - These bits indicate the setting value of the
	                            ADCAnPWDVCR.GCTRL[5:0] bits. (even-numbered channel)  - Select ANI000 */
	//PWSA0.PVCR00_01.UINT32           = 0x00000050UL;


	/* Setting of the clock cycle of PWMCLKm.
	PWBAnBRSm    - PWBAnBRSm Register
	b15:b11                   - Reserved set to 0
	b10:b 0        PWBAnBRSm  - Register for setting the clock cycle of PWMCLKm.      - PWMCLKm = PCLK / 2 * 16  */
	PWBA0.BRS0.UINT16                = 0x0010U;                /* PWMCLK0 = PCLK / ( 2 * 16 ) = 2.5MHz , (PCLK = 80MHz) */


	/* Sets the setting condition for PWGA_TOUTn output.
	PWGAnCSDR    - PWM Output Set Condition Register
	b15:b12                   - Reserved set to 0
	b11:b 0        PWGAnCSDR  - Setting condition for PWM output. */
	                            /* PWM period = ( 1 / PWMCLK0 ) * (4095+1) = 0.0016384s (TRGOUT0) , (PWMCLK0 = 2.5MHz) */
	                            /* High Level = (PWGAnCRDR - PWGAnCSDR) * ( 1 / PWMCLK0 )                              */
	PWGA11.CSDR.UINT16               = 0x0FFFU / 2U;

	Duty = 0.5;
	/* Sets the reset condition for PWGA_TOUTn output.
	PWGAnCRDR    - PWM Output Set Condition Register
	b15:b12                   - Reserved set to 0
	b11:b 0        PWGAnCRDR  - Reset condition for PWM output. */
	PWGA11.CRDR.UINT16               = 0x0FFFU;		//0x9c3

	/* Sets the generation condition for PWGA_TRGOUTn.
	PWGAnCTDR    - PWGA_TRGOUTn Generation Condition Register
	b15:b12                   - Reserved set to 0
	b11:b 0        PWGAnCTDR  - A/D conversion trigger generation condition for PWSAn.  */
	                            /* A/D trigger position =  PWGA0CTDR * ( 1 / PWMCLK0 )   */
	//PWGA0.CTDR.UINT16               = 0x0FFFU / 2U + 0x400U;


	/* Select the count clock from PWBA.
	PWGAnCTL     - PWGA Control Register
	b 7:b 6                   - Reserved set to 0
	b 0            PWGAnCKS   - Count Clock Enable Input PWMCLK3 to PWMCLK0 Select    - Uses PWMCLK0 as count clock */
	PWGA11.CTL.UINT8                = 0x00U;
}
void PWM_Start( void )
{
	/* Control operations of PWSA.
	PWSAnCTL     - PWSAnCTL Register
	b 7:b 6                   - Reserved set to 0
	b 0            PWSAnENBL  - Operation Permission Control                          - Operation is enabled. */
	PWSA0.CTL.UINT8                 = 0x01U;

	/*The INTC2 registers are not accessible per default as these registers are guarded by
	    the PBUS Guard PBG0B. Before accessing any of these registers the PBG0B protection
	    register FSGD0BPROT0 has to be set to 07FF FFFFH.*/
	//PBGFSGD0BPROT0 = 0x07FFFFFFUL;	
	//INTC2EIC141 = 0x0047; 		/* demask PWGA0 interrupt and enable reference table jump method */

	/* Start for PWMCLKm.
	PWBAnTS      - PWBAnTS Register
	b 7:b 4                   - Reserved set to 0
	b 3            PWBAnTS3   - Start trigger bit for PWMCLK3                         - Unused. set to 0'b
	b 2            PWBAnTS2   - Start trigger bit for PWMCLK2                         - Unused. set to 0'b
	b 1            PWBAnTS1   - Start trigger bit for PWMCLK1                         - Unused. set to 0'b
	b 0            PWBAnTS0   - Start trigger bit for PWMCLK0                         - Starts the output of PWMCLK0. */
	PWBA0.TS.UINT8                  = 0x01U;
	while( (PWBA0.TE.UINT8 & 0x01U) != 0x01U )
	{
	    /* Waiting for PWBA0TE to set. */
	}


	/* Start for respective channels simultaneously.
	PWBAnTS      - PWBAnTS Register
	b31:b 0        SLPWGA     - Trigger start and stop to multiple channels
	                            simultaneously.                                       - Starts the corresponding ch.0 */
	SELB.SLPWGA0.UINT32                   = (1<<11);
}
void PWM_Stop( void )
{
	/*The INTC2 registers are not accessible per default as these registers are guarded by
	    the PBUS Guard PBG0B. Before accessing any of these registers the PBG0B protection
	    register FSGD0BPROT0 has to be set to 07FF FFFFH.*/
	//PBGFSGD0BPROT0 = 0x07FFFFFFUL;	
	//INTC2EIC141                  |= 0x0080U;                     /* Disable INTPWGA0                   */


	/* Stop for respective channels simultaneously.
	PWBAnTS      - PWBAnTS Register
	b31:b 0        SLPWGA     - Trigger start and stop to multiple channels
	                            simultaneously.                                       - Stops the corresponding ch.0 */
	SELB.SLPWGA0.UINT32                   &= ~(1<<11);



	/* Stop for PWMCLKm.
	PWBAnTT      - PWBAnTT Register
	b 7:b 4                   - Reserved set to 0
	b 3            PWBAnTT3   - Stop trigger bit for PWMCLK3                          - Unused. set to 0'b
	b 2            PWBAnTT2   - Stop trigger bit for PWMCLK2                          - Unused. set to 0'b
	b 1            PWBAnTT1   - Stop trigger bit for PWMCLK1                          - Unused. set to 0'b
	b 0            PWBAnTT0   - Stop trigger bit for PWMCLK0                          - Stops the output of PWMCLK0. */
	PWBA0.TT.UINT8                  = 0x01U;
	while( (PWBA0.TE.UINT8 & 0x01U) == 0x01U ) 
	{
	    /* Waiting for PWBA0TE to reset. */
	}


	/* Control operations of PWSA.
	PWSAnCTL     - PWSAnCTL Register
	b 7:b 6                   - Reserved set to 0
	b 0            PWSAnENBL  - Operation Permission Control                          - Operation is disabled. */
	PWSA0.CTL.UINT8                 = 0x00U;
}


/******************************************************************************
* Function Name : void PWMD_Update( void )
* Description   : This function update the PWM-Diag. ch.10
* Argument      : none
* Return Value  : none
******************************************************************************/
void PWM_Update_Duty_Cycle( float duty, unsigned int period)
{
    PWM_Stop();
    if ( (PWGA11.RSF.UINT8 & 0x01U) == 0x00U )
    {
        /* Sets the setting condition for PWGA_TOUTn output.
        PWGAnCSDR    - PWM Output Set Condition Register
        b15:b12                   - Reserved set to 0
        b11:b 0        PWGAnCSDR  - Setting condition for PWM output. */
                                    /* PWM period = ( 1 / PWMCLK0 ) * (4095+1) = 0.0016384s (TRGOUT0) , (PWMCLK0 = 2.5MHz) */
                                    /* High Level = (PWGAnCRDR - PWGAnCSDR) * ( 1 / PWMCLK0 )

				       */
	float dutycycle;
	unsigned int csdrvalue;
	if(period>2047)//PWMCLKm = PCLK / 2 ¡Á n (n = 1 to 2047)
	period = 2047;
	
	PWBA0.BRS0.UINT16               = period;                /* PWMCLK0 = PCLK / ( 2 * 16 ) = 2.5MHz , (PCLK = 80MHz) */
	PWGA11.CTL.UINT8                = 0x00U;
	dutycycle = 1.0 - (duty/100.0);
	Duty = dutycycle ;
	csdrvalue =(unsigned int)( dutycycle * PWGA11.CRDR.UINT16);
	PWGA11.CSDR.UINT16 = csdrvalue;
	
	
        /* Stop for PWMCLKm.
        PWGAnRDT     - Buffer Register Reload Trigger Register
        b7:b1                     - Reserved set to 0
        b0             PWGAnRDT   - Simultaneous Rewrite Request Trigger              - Triggers the simultaneous
                                                                                        rewrite request for the                                                                                      compare registers. */
        PWGA11.RDT.UINT8 = 0x01U;
    }
    else
    {
    }
    PWM_Start();
}

//PWM period = ( 1 / PWMCLK0 ) * (frq+1)
void PWM_Update_period( unsigned long  frq )
{
    float tmp;
    
    if ( (PWGA10.RSF.UINT8 & 0x01U) == 0x00U )
    {
        /* Sets the setting condition for PWGA_TOUTn output.
        PWGAnCSDR    - PWM Output Set Condition Register
        b15:b12                   - Reserved set to 0
        b11:b 0        PWGAnCSDR  - Setting condition for PWM output. */
                                    /* PWM period = ( 1 / PWMCLK0 ) * (4095+1) = 0.0016384s (TRGOUT0) , (PWMCLK0 = 2.5MHz) */
                                    /* High Level = (PWGAnCRDR - PWGAnCSDR) * ( 1 / PWMCLK0 )

				       */
        //PWM period = ( 1 / PWMCLK0 ) * (frq+1)
	 tmp =  frq * Duty;
         PWGA10.CSDR.UINT16 =  (unsigned int)tmp;
	 PWGA10.CRDR.UINT16 =  frq;

        /* Stop for PWMCLKm.
        PWGAnRDT     - Buffer Register Reload Trigger Register
        b7:b1                     - Reserved set to 0
        b0             PWGAnRDT   - Simultaneous Rewrite Request Trigger              - Triggers the simultaneous
                                                                                        rewrite request for the                                                                                      compare registers. */
        PWGA10.RDT.UINT8 = 0x01U;
    }
    else
    {
    }
}


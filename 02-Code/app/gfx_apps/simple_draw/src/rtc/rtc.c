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
* File Name     : RTC.c
* Version       : 1.0
* Device(s)     : R7F701404xAFP RH850/D1M1
* Tool-Chain    : GHS
* Description   : This file is a sample of the clock setting.
* Operation     : -
*******************************************************************************
*******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 10.28.2016 1.10    First Release
******************************************************************************/
/******************************************************************************
Includes <System Includes>
******************************************************************************/
#include "r_typedefs.h"
#include "iodefine.h"
#include "rtc.h"
#include "ds1302.h"


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/




/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
static void init_rtc_clock( void );
static void RTCA_Start_Value(uint32_t time, uint32_t Cal);
static void RTCA_Update_Counter(uint32_t time, uint32_t Cal);
static void Read_RTCA_Counter(uint32_t *time, uint32_t *Cal);


/******************************************************************************
Private global variables and functions
******************************************************************************/


/******************************************************************************
* Function Name : void init_clocks( void )
* Description   : This function initializes the clock.
* Argument      : none
* Return Value  : none
******************************************************************************/
static void init_rtc_clock(void)
{
	/* RTC clock domain setting = 011B: Low Speed IntOsc fRL = 240 KHz */
	protected_write(SYS.PROTCMD0.UINT32, SYS.PROTS0.UINT32, SYS.CKSC_ARTCAS_CTL.UINT32, 0x00000003U); 
	while (SYS.CKSC_ARTCAS_ACT.UINT32 != 0x00000003U)
	{
		
	}
    
	/* RTCATCKI = C_AWO_RTCA */
	/* C_AWO_RTCA clock = 011B:CGC_RTCA_CLK_DIVIDER/1 = 240KHz/4=60KHz */	
	protected_write(SYS.PROTCMD0.UINT32, SYS.PROTS0.UINT32, SYS.CKSC_ARTCAD_CTL.UINT32, 0x00000003U); 
	while (SYS.CKSC_ARTCAD_ACT.UINT32 != 0x00000003U)
	{
		
	}
	
	/* Clock domain C_AWO_RTCA is not stopped in DEEPSTOP mode */
	SYS.CKSC_ARTCAD_STPM.UINT32 = 0x00000001U;
}

/******************************************************************************
* Function Name : void RTCA_Start_Value( void )
* Description   : This function sets the start value of the counter of time and calendar.
* Argument      : 
				time: the hours\minutes\seconds to set
				Cal:  the years\months\days\day of week to set
* Return Value  : none
*****************************************************************************/
static void RTCA_Start_Value(uint32_t time, uint32_t Cal)
{
	/*set hour/minute/second*/
	RTCA0.TIME.UINT32 = time;
	/*set year/month/day/day of week*/
	RTCA0.CAL.UINT32 = Cal;
}

/******************************************************************************
* Function Name : void RTCA_Update_Counter( void )
* Description   : This function updates the counter of the time and calendar.
* Argument      : 
				time: the hours\minutes\seconds to set
				Cal:  the years\months\days\day to set
* Return Value  : none
*****************************************************************************/
static void RTCA_Update_Counter(uint32_t time, uint32_t Cal)
{	
	/*Restarts/stops all clock counters (RTCAnSECC to RTCAnYEARC):
		0: Restarts all clock counters either immediately or immediately after the clock counter write operation finishes.
		1: Stops all clock counters temporarily. The sub-counter is still running.*/
	RTCA0.CTL2.UINT8 |= 0x01;
	while((RTCA0.CTL2.UINT8 & 0x02) == 0x0)
	{
		/*wait the clock counters stop*/
	}

	/*Selects the format of the hours counter RTCAnHOURC and the alarm hour setting register RTCAnALH:
		0: 12-hour format (1 to 12, am/pm)
		1: 24-hour format (0 to 23, military time)*/
	RTCA0.CTL0.UINT8 |= 0x20;

	/*set the start value of the time and calendar counters */
	RTCA_Start_Value(time, Cal);

	/*Restarts/stops all clock counters (RTCAnSECC to RTCAnYEARC):
		0: Restarts all clock counters either immediately or immediately after the clock counter write operation finishes.
		1: Stops all clock counters temporarily. The sub-counter is still running.*/
	RTCA0.CTL2.UINT8 &= 0xFE;
}

/******************************************************************************
* Function Name : void Read_RTCA_Counter( void )
* Description   : This function read the value of the counter of time and calendar.
* Argument      : 
				time: store the hours\minutes\seconds
				Cal:  store the years\months\days\day of week
* Return Value  : none
*****************************************************************************/
static void Read_RTCA_Counter(uint32_t *time, uint32_t *Cal)
{

	/*Restarts/stops all clock counters (RTCAnSECC to RTCAnYEARC):
		0: Restarts all clock counters either immediately or immediately after the clock counter write operation finishes.
		1: Stops all clock counters temporarily. The sub-counter is still running.*/
	RTCA0.CTL2.UINT8 |= 0x01;
	while((RTCA0.CTL2.UINT8 & 0x02) == 0x0)
	{
	}

	/* read the time counter*/
	*time = RTCA0.TIME.UINT32;
	/*read the calendar counter*/
	*Cal = RTCA0.CAL.UINT32;
	
	/*Restarts/stops all clock counters (RTCAnSECC to RTCAnYEARC):
		0: Restarts all clock counters either immediately or immediately after the clock counter write operation finishes.
		1: Stops all clock counters temporarily. The sub-counter is still running.*/
	RTCA0.CTL2.UINT8 &= 0xFE;
}

/******************************************************************************
* Function Name : void Read_RTCA_SRBU( void )
* Description   : This function read the value of the sub_counter.
* Argument      : 
				count: store the sub_counter value
* Return Value  : none
*****************************************************************************/
void Read_RTCA_SRBU(uint32_t *count)
{
	while(RTCA0.CTL2.UINT8 & 0x08)
	{
	}
	/*Triggers transfer of the value of the sub-counter (RTCAnSUBC) to the dedicated read buffer (RTCAnSRBU) or clears the transfer state of the subcounter:
		0: Transfer status (RTCAnRSST) is cleared.
		1: Transfer is triggered. */
	RTCA0.CTL2.UINT8 |= 0x04;
	while((RTCA0.CTL2.UINT8 &0x08) == 0x0)
	{
	}

	/*read the sub_counter value*/
	*count = RTCA0.SRBU.UINT32;
	
	/*Triggers transfer of the value of the sub-counter (RTCAnSUBC) to the dedicated read buffer (RTCAnSRBU) or clears the transfer state of the subcounter:
		0: Transfer status (RTCAnRSST) is cleared.
		1: Transfer is triggered. */
	RTCA0.CTL2.UINT8 &= 0xFB;
}


/******************************************************************************
* Function Name : Internal_RTC_init
* Description   : This function initializes the clock.
* Argument      : none
* Return Value  : none
******************************************************************************/
void Internal_RTC_init(void)
{
	init_rtc_clock();
	
	/* RTCA0 setting */
	/* 	RTCA0CTL0 Bit7: Starts/stops the sub-counter RTCAnSUBC operation.
		0: Stops the sub-counter operation. All output pins and all status flags in control register RTCAnCTL2 are cleared.
		1: Starts the sub-counter operation. The sub-counter counts up. */
	RTCA0.CTL0.UINT8 &= 0x7Fu;
	while(RTCA0.CTL0.UINT8 & 0x40)
	{
	}

	RTCA0.CTL0.UINT8 |= 0x30;

	/* Sets sub-counter overflow value(RTCATCKI - 1 in Hz) = 60 KHz /60000 Hz (927C0H) */
	RTCA0.SCMP.UINT32 = (0x0000EA60 -1);

	/* RTCA1 setting */
	/* Enables 0.5s and 1s interrupt request generation (RTCATINTR/RTCATINT1S): */
	RTCA0.CTL1.UINT8 = 0x0Au;

	/* Set interrupt flags */
	//INTC1.EIC4.UINT16 = 0x0047;
	//INTC1.EIC2.UINT16 = 0x0047;

	/* 12:00:00  20年1月1日星期三*/
	RTCA_Start_Value(0x00120000, 0x20010103);
	
	/* RTCA0 start */
	RTCA0.CTL0.UINT8 |= 0x80u;

}

void Internal_RTC_ReadTime(DATE_t *pTime) // internal rtc
{
	uint32_t time, cal;
	
	Read_RTCA_Counter(&time, &cal);
	
	pTime->second = (time & 0x0000007F);
	pTime->minute = (time >> 8)  & 0x0000007F;
	pTime->hour   = (time >> 16) & 0x0000003F;

	pTime->week   = (cal & 0x00000007);
	pTime->day    = (cal >> 8)  & 0x0000003F;
	pTime->month  = (cal >> 16) & 0x0000001F;
	pTime->year   = (cal >> 24) & 0x000000FF;
}

void Internal_RTC_SetTime(DATE_t *pTime) // internal rtc
{
	volatile uint32_t time = 0, cal = 0;
	
	time |= (pTime->second);
	time |= (pTime->minute << 8);
	time |= (pTime->hour   << 16);
	
	cal  |= (pTime->week);
	cal  |= (pTime->day    << 8);
	cal  |= (pTime->month  << 16);
	cal  |= (pTime->year   << 24);
	
	RTCA_Update_Counter(time, cal);
}



void DS3231_ReadTime(DATE_t *pTime) //external rtc
{
    DS1302_ReadTime(pTime);   
}

void DS3231_SetTime(DATE_t *pTime) //external rtc
{
    DS1302_SetTime(pTime);
}


static uint8_t tbit = 0;
void External_RTC_init(uint8_t rtc_chip)
{
	if(rtc_chip == RTC_DS1302)
	{
		tbit = RTC_DS1302;
		DS1302_init();
	}
	if(rtc_chip == RTC_DS3231)
	{
		tbit = RTC_DS3231;
		// DS3231_Init();
		DS1302_init();
	}
	
}

void External_RTC_ReadTime(DATE_t *pTime)  //external rtc
{
	if(tbit == RTC_DS1302)
		DS1302_ReadTime(pTime);     
	else
		DS3231_ReadTime(pTime);
}

void External_RTC_SetTime(DATE_t *pTime)   //external rtc
{
	if(tbit == RTC_DS1302)
    	DS1302_SetTime(pTime);
    else
     	DS3231_SetTime(pTime);
	
	
}













/** ****************************************************************************
 * @copyright Copyright (c) XXX
 * All rights reserved.
 *
 * @file    pwm.c
 * @brief   pwm application
 * @version V1.0
 * @author  liuwenlong
 * @date    2022-7-8
 * @note <b>history:</b>
 * 1. create file
 *
 */

/******************************************************************************
Includes <System Includes>
******************************************************************************/
#include "r_typedefs.h"
#include "iodefine.h"
#include "pwm.h"


/*
Port     2nd Alternative  3rd Alternative  4th Alternative
  |         I       O          O             I         O
  |         |       |          |             |         |
  |         |       |          |             |         |
P42_0       |     PWGA0O    TAUB2O1          |         |
P42_1       |     PWGA1O    TAUB2O3          |         |
P42_2       |     PWGA2O    TAUB2O5          |         |
P42_3       |     PWGA3O    TAUB2O7          |         |
P42_4       |     PWGA4O    TAUB2O2          |         |
P42_5       |     PWGA5O    TAUB2O6          |         |
P42_6       |     PWGA6O    TAUB2O9          |         |
P42_7       |     PWGA12O   TAUB1O6          |         |
P42_8       |     PWGA13O   TAUB0O15         |         |
P42_9       |     PWGA14O   TAUB1O2          |         |
P42_10      |     PWGA15O   TAUB1O10         |         |
P42_11   TAUB2I9  PWGA7O    TAUB2O10       TAUB0I14 TAUB0O14
P42_12   TAUB2I10 PWGA8O    TAUB2O11       TAUJ0I2  TAUJ0O2
P42_13   TAUB2I11 PWGA9O    TAUB2O13       TAUJ0I3  TAUJ0O3
P42_14            PWGA10O   TAUB2O14       TAUB1I2
P42_15            PWGA11O   TAUB2O15       TAUB1I10
*/

float Duty;

void PWM_Init( void )
{
	/* PWMCLK0 = PCLK / ( 2 * 16 ) = 2.5MHz , (PCLK = 80MHz) */
    PWBA0.BRS0.UINT16  = 0x0010U; 
	
    //BUZZ PWM init
    PWGA7.CSDR.UINT16  = 0x0FFFU / 2U;
    Duty = 0.5;
    PWGA7.CRDR.UINT16  = 0x0FFFU;
    PWGA7.CTL.UINT8    = 0x00U;
	
    //LED_DRV
    PWGA14.CSDR.UINT16 = 0x0FFFU / 2U;
    Duty = 0.5;
    PWGA14.CRDR.UINT16 = 0x0FFFU;
    PWGA14.CTL.UINT8   = 0x00U;
	
    //BL
    PWGA11.CSDR.UINT16 = 0x0FFFU / 2U;
    Duty = 0.5;
    PWGA11.CRDR.UINT16 = 0x0FFFU;
    PWGA11.CTL.UINT8   = 0x00U;
	
    //LCD_DRV
    PWGA6.CSDR.UINT16  = 0x0FFFU / 2U;
    Duty = 0.5;
    PWGA6.CRDR.UINT16  = 0x0FFFU;
    PWGA6.CTL.UINT8    = 0x00U;
	
	PWM_Stop(LED_CHANNEL);
	PWM_Stop(BG_CHANNEL);
	PWM_Stop(LCD_CHANNEL);
	
	
}

void PWM_Start( unsigned char channel )
{
    PWSA0.CTL.UINT8 = 0x01U;
    PWBA0.TS.UINT8  = 0x01U;
    while( (PWBA0.TE.UINT8 & 0x01U) != 0x01U )
    {
		/* Waiting for PWBA0TE to set. */
    }
    SELB.SLPWGA0.UINT32 |=   (1<<channel);
	
}

void PWM_Stop( unsigned char channel )
{
    SELB.SLPWGA0.UINT32 &= ~(1<<channel);
    PWBA0.TT.UINT8       = 0x01U;
    while( (PWBA0.TE.UINT8 & 0x01U) == 0x01U ) 
    {
		/* Waiting for PWBA0TE to reset. */
    }
    PWSA0.CTL.UINT8  = 0x00U;
	
}

void PWM_Deinit(void)
{
    SELB.SLPWGA0.UINT32 = 0;
    PWBA0.TT.UINT8      = 0x01U;
    while( (PWBA0.TE.UINT8 & 0x01U) == 0x01U ) 
    {
		/* Waiting for PWBA0TE to reset. */
    }
    PWSA0.CTL.UINT8  = 0x00U;
	
}

void PWM_Update_Duty_Cycle( unsigned char channel, float duty, unsigned int period)
{
	float    dutycycle;
	uint16_t csdrvalue;
	uint8_t  temp = 0x00;
	
    PWM_Stop(channel);
    switch (channel)
	{
		case 6:
			if ( (PWGA6.RSF.UINT8 & 0x01U) == 0x00U )//p42_6
			{
				if(period>2047)//PWMCLKm = PCLK / 2 × n (n = 1 to 2047)
					period = 2047;
				
				if((20 > duty)&&(0x00 != duty))
				{
					temp = 19;
				}
				else
				{
					temp = duty;
				}
				
				PWBA0.BRS0.UINT16 = period; /* PWMCLK0 = PCLK / ( 2 * period ) = xxMHz , (PCLK = 80MHz) */
				PWGA6.CTL.UINT8   = 0x00U;
				dutycycle = 1.0 - (temp/100.0);
				Duty = dutycycle ;
				csdrvalue =(uint16_t)( dutycycle * PWGA6.CRDR.UINT16);
				PWGA6.CSDR.UINT16 = csdrvalue;
				PWGA6.RDT.UINT8 = 0x01U;
			}
			break;
		case 11:
			if ( (PWGA11.RSF.UINT8 & 0x01U) == 0x00U )//p42_9
			{
				if(period>2047)//PWMCLKm = PCLK / 2 × n (n = 1 to 2047)
					period = 2047;
				PWBA0.BRS0.UINT16 = period; /* PWMCLK0 = PCLK / ( 2 * period ) = xxMHz , (PCLK = 80MHz) */
				PWGA11.CTL.UINT8  = 0x00U;
				dutycycle = 1.0 - (duty/100.0);
				Duty = dutycycle ;
				csdrvalue =(uint16_t)( dutycycle * PWGA11.CRDR.UINT16);
				PWGA11.CSDR.UINT16 = csdrvalue;
				PWGA11.RDT.UINT8 = 0x01U;
			}
			break;
		case 13:
			if ( (PWGA13.RSF.UINT8 & 0x01U) == 0x00U )//p42_8
			{
				if(period>2047)//PWMCLKm = PCLK / 2 × n (n = 1 to 2047)
					period = 2047;
				PWBA0.BRS0.UINT16 = period;                /* PWMCLK0 = PCLK / ( 2 * period ) = xxMHz , (PCLK = 80MHz) */
				PWGA13.CTL.UINT8  = 0x00U;
				dutycycle = 1.0 - (duty/100.0);
				Duty = dutycycle ;
				csdrvalue =(uint16_t)( dutycycle * PWGA13.CRDR.UINT16);
				PWGA13.CSDR.UINT16 = csdrvalue;
				PWGA13.RDT.UINT8 = 0x01U;
			}
			break;
		case 14:
			if ( (PWGA14.RSF.UINT8 & 0x01U) == 0x00U )//p42_9
			{
				if(period>2047)//PWMCLKm = PCLK / 2 × n (n = 1 to 2047)
					period = 2047;
				PWBA0.BRS0.UINT16 = period;                /* PWMCLK0 = PCLK / ( 2 * period ) = xxMHz , (PCLK = 80MHz) */
				PWGA14.CTL.UINT8  = 0x00U;
				dutycycle = 1.0 - (duty/100.0);
				Duty = dutycycle ;
				csdrvalue =(uint16_t)( dutycycle * PWGA14.CRDR.UINT16);
				PWGA14.CSDR.UINT16 = csdrvalue;
				PWGA14.RDT.UINT8 = 0x01U;
			}
			break;
		default:
			break;
    }
    PWM_Start(channel);
	
}













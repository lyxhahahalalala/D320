/*
============================================================================
 PROJECT: Example project
============================================================================
                           C O P Y R I G H T
============================================================================
 Copyright (c) 2014 by Renesas Electronics GmbH. All rights reserved.
 Arcadiastr. 10
 D-40472 Duesseldorf
 Germany
============================================================================

 Warranty disclaimer:
 Because the product(s) is licensed free of charge, there is no warranty of
 any kind whatsoever and expressly disclaimed and excluded by Renesas, either
 expressed or implied, including but not limited to those for non-
 infringement of intellectual property, merchantability and/or fitness for
 the particular purpose. Renesas shall not have any obligation to maintain,
 service or provide bug fixes for the supplied Product(s) and/or the
 Application.

 Each user is solely responsible for determining the appropriateness of
 using the product(s) and assumes all risks associated with its exercise
 of rights under this agreement, including, but not limited to the risks
 and costs of program errors, compliance with applicable laws, damage to
 or loss of data, programs or equipment, and unavailability or
 interruption of operations.

 Limitation of liability:
 In no event shall Renesas be liable to the user for any incidental,
 consequential, indirect, or punitive damage (including but not limited to
 lost profits) regardless of whether such liability is based on breach of
 contract, tort, strict liability, breach of warranties, failure of
 essential purpose or otherwise and even if advised of the possibility of
 such damages. Renesas shall not be liable for any services or products provided
 by third party vendors, developers or consultants identified or referred
 to the user by Renesas in connection with the product(s) and/or the
 application.

============================================================================
 Enviroment:  Devices:     RH850/D1L2 / R7F701403
              Debugger:    GHS Multi 2000
============================================================================

 GHS Multi example project for use with RH850/D1L2 application board
 (Y-RH850-D1X-MB-T1-V1, Y-RH850-D1L2-PB-TET-V1) 

============================================================================
*/

/*
============================================================================
 Includes
============================================================================
*/
#include "Device.h"
#include "Port.h"
#include "Dev_api.h"
#include "stdio.h"
#include "Cpu.h"
/*
============================================================================
 Defines
============================================================================
*/


/*
============================================================================
 Prototypes
============================================================================
*/
extern r_Error_t Clock_Init(void);                 /* initialize clocks */
extern void TAUB0_Init(void);                  /* initialize timer TAUB0 */
extern void INTTAUB0I0(void);

void main(void);
void init_Gpio(void);

/*
============================================================================
 Main Function
============================================================================
*/
void main(void) 
{
	r_Error_t ret;
	ret = Clock_Init();                        /* initialize clocks */
	if(!ret)
	{
		printf("%s","Clock set error, please check the parameter !!!");
	}

	FCLA0CTL6 = 0x04;
	init_Gpio();                         /* initialize ports */
	TAUB0_Init();                         /* initialize timer TAUB0 */

	SET_EBASE(0x00020000); //set Ebase to 0x10000, set RINT to 1
	SET_PSW_EBV();

	INTC1EIC17=0x0047;                    /* demask timer TAUB0 chanel 0 interrupt and enable reference table jump method */
	INTC1EIC11=0x0047;                    /* demask timer TAUB0 chanel 0 interrupt and enable reference table jump method */
	__EI();                               /* enable interrupts globally */
	while(1)
	{                            /* endless loop */
	}
}

void init_Gpio(void)
{
	Port_Init_Simple(PORT0,PIN0,Alter_Func4,Pin_In);			/*P0_0, 4st altermative function, input, INTP5*/

	Port_Init_Simple(PORT10, PIN0, Normal_Port, Pin_Out);	/*P17_6, port mode, output*/
	Port_Init_Simple(PORT10, PIN1, Normal_Port, Pin_Out);	/*P42_0, port mode, output*/
	Port_Init_Simple(PORT10, PIN2, Normal_Port, Pin_Out);	/*P42_1, port mode, output*/
	Port_Clear(PORT10, PIN0);
	Port_Clear(PORT10, PIN1);
	Port_Clear(PORT10, PIN2);
}


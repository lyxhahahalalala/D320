/*
============================================================================
 PROJECT: Example project
============================================================================
                           C O P Y R I G H T
============================================================================
 Copyright (c) 2015 by Renesas Electronics GmbH. All rights reserved.
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
 Enviroment:  Devices:     RH850/D1M2H / R7F701412
              Debugger:    GHS Multi 2000
============================================================================

 GHS Multi example project for use with RH850/D1M2H application board
 (Y-RH850-D1X-MB-T1-V1, Y-RH850-D1M2H-PB-TET-V1) 

============================================================================
*/


//============================================================================
//
// Header file for the RS CAN
//
//============================================================================
#ifndef _RSCAN_H_
#define _RSCAN_H_



/******************************************************************************
 * Macro define
 */
#define GET_BIT(reg, pos)  (((reg) >> (pos)) & 1U)
#define SET_BIT(reg, pos)  ((reg) |= 1U << (pos))
#define CLR_BIT(reg, pos)  ((reg) &= ~(1UL << (pos)))


/*******************************************************************************
 * TYPEDEFS
 */
typedef struct 
{
    volatile unsigned long id;
    volatile unsigned char data[8];
    volatile unsigned char len;
    volatile unsigned char type;
} CAN_MESSAGE;

typedef enum CAN_CHANNEL {
	CAN_CH0 = 0,
	CAN_CH1 = 1,
	CAN_CH2 = 2,
	CAN_CHN = 3
}CanChannel_t;


/*****************************************************************************
 * Function define
 */

void RS_CAN_init(void);

void RS_CAN_RECV_Disabled(CanChannel_t channel);
void RS_CAN_RECV_Enabled(CanChannel_t channel);

void CAN1_ReceiveMessage(CAN_MESSAGE *message);
void CAN2_ReceiveMessage(CAN_MESSAGE *message);

void CAN1_SendMessage(CAN_MESSAGE *message);
void CAN2_SendMessage(CAN_MESSAGE *message);

void CAN1_SendFIFOMessage(CAN_MESSAGE *message);
void CAN2_SendFIFOMessage(CAN_MESSAGE *message);


#endif


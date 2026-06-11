

#include "Can.h"
#include "rscan.h"
#include "R_typedefs.h"
#include "fcl_cfg.h"
#include "r_fcl_types.h"
#include "r_fcl.h"
#include "target.h"
#include "fcl_descriptor.h"
#include "fcl_user.h"

#include "Cpu.h"

CAN_MESSAGE message_Rec;
uint8_t stCAN1;
extern uint8 BT_TxSt;

void Can_Init(void)
{
    RS_CAN_init();
}

Can_ReturnType Can_Write (Can_HwHandleType Hth, const Can_PduType *PduInfo)
{
    uint8_t idx;
    
    CAN_MESSAGE message;

    message.id = PduInfo->id;
    //message.len = PduInfo->length;
    message.len = PduInfo->length;
    
    for(idx=0; idx< message.len; idx++)
    {
		message.data[idx] = PduInfo->sdu[idx];
    }
   
     for(idx=message.len; idx< 8; idx++)
    {
		message.data[idx] = 0x00u;
    }
    
    message.len = 8u;
    CAN1_SendMessage(&message);

    return CAN_OK;
}

void Can_MainFunction_Write(void)
{
    stCAN1 = RSCAN0.TMSTS16.UINT8;
    if(stCAN1  == 0x4)
    {
	RSCAN0.TMSTS16.UINT8 = 0x00;
	BT_TxSt = 1;
    }
}


#pragma ghs section text = ".R_FCL_CODE_USRINT"
/*
 * CAN2 interrupt service routine
 * Receive data from can2 port
 */
__interrupt void INTRCANGRECC(void) {    
    if((RSCAN0.RFSTS2.UINT32 & 0xFF00) > 0)
    {
	CAN2_ReceiveMessage(&message_Rec);
    }

    if((RSCAN0.RFSTS1.UINT32 & 0xFF00) > 0)
    {
	CAN1_ReceiveMessage(&message_Rec);
    }

}

#ifndef _CAN_H
#define _CAN_H

#include "types.h"

typedef uint8 Can_HwHandleType;

typedef enum
{
    CAN_OK,
    CAN_NOT_OK,
    CAN_BUSY

}Can_ReturnType;

#define CanTp_Tx_0x18DAF117     0
#define	CanTp_Rx_0x18DA17FF     1
#define CanTp_Rx_0x18DA17F1     2

/* Size of CanId */
typedef uint32 Can_IdType;

typedef uint16   PduIdType;

typedef uint16  PduLengthType;

typedef struct
{
    uint8* sdu;
    Can_IdType id;
    PduIdType  swPduHandle;
    uint8      length;

}Can_PduType;


extern void Can_Init(void);
extern Can_ReturnType Can_Write(Can_HwHandleType Hth, const Can_PduType *PduInfo);
extern void Can_MainFunction_Write(void);

#endif

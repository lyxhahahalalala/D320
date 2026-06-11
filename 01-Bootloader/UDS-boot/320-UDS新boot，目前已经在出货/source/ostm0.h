#ifndef _OSTM0_H
#define _OSTM0_H

//#define _USE_CSPlus
#define _USE_GHS
//#define _USE_IAR

#if defined _USE_CSPlus
#pragma ioreg
#elif defined _USE_GHS
#include "device.h"
#endif 

void OSTM0_INIT(uint32_t compare_value);
void OSTM0_DEINIT(void);
void OSTM0_START(void);
void OSTM0_STOP(void);
void OSTM0_enable_int(void);
void OSTM0_INT_CB(void);
//#pragma weak OSTM0_INT_CB
#endif

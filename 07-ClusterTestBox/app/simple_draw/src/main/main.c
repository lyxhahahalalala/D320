/*
 *Created on 2017年11月21日, 
 *     客车仪表
 *版本号 V1.07
 */
#include "SYSTEM.h"
#include "init.h"
#include "Motor_sm.h"
#include "Carmodel.h"
#include "r_tick_api.h"
unsigned char uBL;
double airpressure1 = 0;
double airpressure2 = 0;
double updatespeed = 0;
double updaterpm = 0;
double updatesoc = 0;
double updatevol = 0;
uint16_t PowerOff = 0;
uint8_t pOff = 0;
void fdl_init(void);
void System_Init(void);
int main(void) {
    init();
    fdl_init();
    System_Init();
    while (1) {
		WDT_Clear();
		if(F_50ms){
			F_50ms = 0;
			//Count_ADR05();
		}
	}
}

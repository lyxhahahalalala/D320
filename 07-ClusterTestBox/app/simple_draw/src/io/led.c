#include "BSP.h"
#include "iodefine.h"
#include "system.h"

IO_CONFIG_BIT config_io_h;
IO_CONFIG_BIT config_io_l;
DATA_BIT config_ad_p;
DATA_BIT config_ad_r1;
DATA_BIT config_ad_r2;
extern uint8_t etu_type_io;


         
#define MCLK_L        PORT_ISO.P21.BIT.P21_6      
#define MOSI_L           PORT_ISO.P21.BIT.P21_7   
#define CS_L          PORT_ISO.P21.BIT.P21_8            

#define MCLK_H        PORT_ISO.P21.BIT.P21_0
#define MOSI_H           PORT_ISO.P21.BIT.P21_1                 
#define CS_H          PORT_ISO.P21.BIT.P21_2   


void CONFIG_IO(void)
{
   unsigned char i, j;
   unsigned char dump;
   //0-L 1-h 2-oc
 if(g_io_fonfigure.C1_GEARBOX_QULI_CTRL== 0){ config_io_l.C1_GEARBOX_QULI_CTRL= 1; config_io_h.C1_GEARBOX_QULI_CTRL= 0;}else if(g_io_fonfigure.C1_GEARBOX_QULI_CTRL == 1){ config_io_l.C1_GEARBOX_QULI_CTRL= 0;config_io_h.C1_GEARBOX_QULI_CTRL= 1;}else if(g_io_fonfigure.C1_GEARBOX_QULI_CTRL == 2){config_io_l.C1_GEARBOX_QULI_CTRL= 0;config_io_h.C1_GEARBOX_QULI_CTRL= 0;}else;
 if(g_io_fonfigure.C2_REAR_LIGHT_CTRL== 	0){	 config_io_l.C2_REAR_LIGHT_CTRL = 	1;	config_io_h.C2_REAR_LIGHT_CTRL= 	0;}	else if(g_io_fonfigure.C2_REAR_LIGHT_CTRL == 	1){	 config_io_l.C2_REAR_LIGHT_CTRL=	0;config_io_h.C2_REAR_LIGHT_CTRL=	1;}else if(g_io_fonfigure.C2_REAR_LIGHT_CTRL ==	2){	 config_io_l.C2_REAR_LIGHT_CTRL=	0;config_io_h.C2_REAR_LIGHT_CTRL=	0;}else;
 if(g_io_fonfigure.C3_RESERVE_CTRL==	0){	 config_io_l.C3_RESERVE_CTRL =	1;	config_io_h.C3_RESERVE_CTRL=	0;}	else if(g_io_fonfigure.C3_RESERVE_CTRL ==	1){	 config_io_l.C3_RESERVE_CTRL=	0;config_io_h.C3_RESERVE_CTRL=	1;}else if(g_io_fonfigure.C3_RESERVE_CTRL ==	2){	 config_io_l.C3_RESERVE_CTRL=	0;config_io_h.C3_RESERVE_CTRL=	0;}else;
 if(g_io_fonfigure.C4_OK_CTRL==	0){	 config_io_l.C4_OK_CTRL =	1;	config_io_h.C4_OK_CTRL=	0;}	else if(g_io_fonfigure.C4_OK_CTRL ==	1){	 config_io_l.C4_OK_CTRL=	0;config_io_h.C4_OK_CTRL=	1;}else if(g_io_fonfigure.C4_OK_CTRL ==	2){	 config_io_l.C4_OK_CTRL=	0;config_io_h.C4_OK_CTRL=	0;}else;
 if(g_io_fonfigure.C5_CANCEL_CTRL==	0){	 config_io_l.C5_CANCEL_CTRL =	1;	config_io_h.C5_CANCEL_CTRL=	0;}	else if(g_io_fonfigure.C5_CANCEL_CTRL ==	1){	 config_io_l.C5_CANCEL_CTRL=	0;config_io_h.C5_CANCEL_CTRL=	1;}else if(g_io_fonfigure.C5_CANCEL_CTRL ==	2){	 config_io_l.C5_CANCEL_CTRL=	0;config_io_h.C5_CANCEL_CTRL=	0;}else;
 if(g_io_fonfigure.C6_UP_CTRL==	0){	 config_io_l.C6_UP_CTRL =	1;	config_io_h.C6_UP_CTRL=	0;}	else if(g_io_fonfigure.C6_UP_CTRL ==	1){	 config_io_l.C6_UP_CTRL=	0;config_io_h.C6_UP_CTRL=	1;}else if(g_io_fonfigure.C6_UP_CTRL ==	2){	 config_io_l.C6_UP_CTRL=	0;config_io_h.C6_UP_CTRL=	0;}else;
 if(g_io_fonfigure.C7_DOWN_CTRL==	0){	 config_io_l.C7_DOWN_CTRL =	1;	config_io_h.C7_DOWN_CTRL=	0;}	else if(g_io_fonfigure.C7_DOWN_CTRL ==	1){	 config_io_l.C7_DOWN_CTRL=	0;config_io_h.C7_DOWN_CTRL=	1;}else if(g_io_fonfigure.C7_DOWN_CTRL ==	2){	 config_io_l.C7_DOWN_CTRL=	0;config_io_h.C7_DOWN_CTRL=	0;}else;
 if(g_io_fonfigure.C8_HI_LEVEL_CTRL==	0){	 config_io_l.C8_HI_LEVEL_CTRL =	1;	config_io_h.C8_HI_LEVEL_CTRL=	0;}	else if(g_io_fonfigure.C8_HI_LEVEL_CTRL ==	1){	 config_io_l.C8_HI_LEVEL_CTRL=	0;config_io_h.C8_HI_LEVEL_CTRL=	1;}else if(g_io_fonfigure.C8_HI_LEVEL_CTRL ==	2){	 config_io_l.C8_HI_LEVEL_CTRL=	0;config_io_h.C8_HI_LEVEL_CTRL=	0;}else;
 if(g_io_fonfigure.C9_INTAKE_PREHEAT_CTRL==	0){	 config_io_l.C9_INTAKE_PREHEAT_CTRL =	1;	config_io_h.C9_INTAKE_PREHEAT_CTRL=	0;}	else if(g_io_fonfigure.C9_INTAKE_PREHEAT_CTRL ==	1){	 config_io_l.C9_INTAKE_PREHEAT_CTRL=	0;config_io_h.C9_INTAKE_PREHEAT_CTRL=	1;}else if(g_io_fonfigure.C9_INTAKE_PREHEAT_CTRL ==	2){	 config_io_l.C9_INTAKE_PREHEAT_CTRL=	0;config_io_h.C9_INTAKE_PREHEAT_CTRL=	0;}else;
 if(g_io_fonfigure.C10_TRAILER_ABS_CTRL==	0){	 config_io_l.C10_TRAILER_ABS_CTRL =	1;	config_io_h.C10_TRAILER_ABS_CTRL=	0;}	else if(g_io_fonfigure.C10_TRAILER_ABS_CTRL ==	1){	 config_io_l.C10_TRAILER_ABS_CTRL=	0;config_io_h.C10_TRAILER_ABS_CTRL=	1;}else if(g_io_fonfigure.C10_TRAILER_ABS_CTRL ==	2){	 config_io_l.C10_TRAILER_ABS_CTRL=	0;config_io_h.C10_TRAILER_ABS_CTRL=	0;}else;
 if(g_io_fonfigure.C11_AXLE_DIFF_CTRL==	0){	 config_io_l.C11_AXLE_DIFF_CTRL =	1;	config_io_h.C11_AXLE_DIFF_CTRL=	0;}	else if(g_io_fonfigure.C11_AXLE_DIFF_CTRL ==	1){	 config_io_l.C11_AXLE_DIFF_CTRL=	0;config_io_h.C11_AXLE_DIFF_CTRL=	1;}else if(g_io_fonfigure.C11_AXLE_DIFF_CTRL ==	2){	 config_io_l.C11_AXLE_DIFF_CTRL=	0;config_io_h.C11_AXLE_DIFF_CTRL=	0;}else;
 if(g_io_fonfigure.C12_WHEEL_DIFF_CTRL==	0){	 config_io_l.C12_WHEEL_DIFF_CTRL =	1;	config_io_h.C12_WHEEL_DIFF_CTRL=	0;}	else if(g_io_fonfigure.C12_WHEEL_DIFF_CTRL ==	1){	 config_io_l.C12_WHEEL_DIFF_CTRL=	0;config_io_h.C12_WHEEL_DIFF_CTRL=	1;}else if(g_io_fonfigure.C12_WHEEL_DIFF_CTRL ==	2){	 config_io_l.C12_WHEEL_DIFF_CTRL=	0;config_io_h.C12_WHEEL_DIFF_CTRL=	0;}else;
 if(g_io_fonfigure.C13_VBAT_SW_CTRL==	0){	 config_io_l.C13_VBAT_SW_CTRL =	1;	config_io_h.C13_VBAT_SW_CTRL=	0;}	else if(g_io_fonfigure.C13_VBAT_SW_CTRL ==	1){	 config_io_l.C13_VBAT_SW_CTRL=	0;config_io_h.C13_VBAT_SW_CTRL=	1;}else if(g_io_fonfigure.C13_VBAT_SW_CTRL ==	2){	 config_io_l.C13_VBAT_SW_CTRL=	0;config_io_h.C13_VBAT_SW_CTRL=	0;}else;
 if(g_io_fonfigure.C14_Emergency_shutdown_CTRL==	0){	 config_io_l.C14_Emergency_shutdown_CTRL =	1;	config_io_h.C14_Emergency_shutdown_CTRL=	0;}	else if(g_io_fonfigure.C14_Emergency_shutdown_CTRL ==	1){	 config_io_l.C14_Emergency_shutdown_CTRL=	0;config_io_h.C14_Emergency_shutdown_CTRL=	1;}else if(g_io_fonfigure.C14_Emergency_shutdown_CTRL ==	2){	 config_io_l.C14_Emergency_shutdown_CTRL=	0;config_io_h.C14_Emergency_shutdown_CTRL=	0;}else;
 if(g_io_fonfigure.C15_QULI_INDICTOR_CTRL==	0){	 config_io_l.C15_QULI_INDICTOR_CTRL =	1;	config_io_h.C15_QULI_INDICTOR_CTRL=	0;}	else if(g_io_fonfigure.C15_QULI_INDICTOR_CTRL ==	1){	 config_io_l.C15_QULI_INDICTOR_CTRL=	0;config_io_h.C15_QULI_INDICTOR_CTRL=	1;}else if(g_io_fonfigure.C15_QULI_INDICTOR_CTRL ==	2){	 config_io_l.C15_QULI_INDICTOR_CTRL=	0;config_io_h.C15_QULI_INDICTOR_CTRL=	0;}else;
 if(g_io_fonfigure.C16_BRAKE_SW_L1_CTRL==	0){	 config_io_l.C16_BRAKE_SW_L1_CTRL =	1;	config_io_h.C16_BRAKE_SW_L1_CTRL=	0;}	else if(g_io_fonfigure.C16_BRAKE_SW_L1_CTRL ==	1){	 config_io_l.C16_BRAKE_SW_L1_CTRL=	0;config_io_h.C16_BRAKE_SW_L1_CTRL=	1;}else if(g_io_fonfigure.C16_BRAKE_SW_L1_CTRL ==	2){	 config_io_l.C16_BRAKE_SW_L1_CTRL=	0;config_io_h.C16_BRAKE_SW_L1_CTRL=	0;}else;
 if(g_io_fonfigure.C17_BRAKE_SW_L2_CTRL==	0){	 config_io_l.C17_BRAKE_SW_L2_CTRL =	1;	config_io_h.C17_BRAKE_SW_L2_CTRL=	0;}	else if(g_io_fonfigure.C17_BRAKE_SW_L2_CTRL ==	1){	 config_io_l.C17_BRAKE_SW_L2_CTRL=	0;config_io_h.C17_BRAKE_SW_L2_CTRL=	1;}else if(g_io_fonfigure.C17_BRAKE_SW_L2_CTRL ==	2){	 config_io_l.C17_BRAKE_SW_L2_CTRL=	0;config_io_h.C17_BRAKE_SW_L2_CTRL=	0;}else;
 if(g_io_fonfigure.C18_BRAKE_SW_L3_CTRL==	0){	 config_io_l.C18_BRAKE_SW_L3_CTRL =	1;	config_io_h.C18_BRAKE_SW_L3_CTRL=	0;}	else if(g_io_fonfigure.C18_BRAKE_SW_L3_CTRL ==	1){	 config_io_l.C18_BRAKE_SW_L3_CTRL=	0;config_io_h.C18_BRAKE_SW_L3_CTRL=	1;}else if(g_io_fonfigure.C18_BRAKE_SW_L3_CTRL ==	2){	 config_io_l.C18_BRAKE_SW_L3_CTRL=	0;config_io_h.C18_BRAKE_SW_L3_CTRL=	0;}else;
 if(g_io_fonfigure.C19_BRAKE_SW_L4_CTRL==	0){	 config_io_l.C19_BRAKE_SW_L4_CTRL =	1;	config_io_h.C19_BRAKE_SW_L4_CTRL=	0;}	else if(g_io_fonfigure.C19_BRAKE_SW_L4_CTRL ==	1){	 config_io_l.C19_BRAKE_SW_L4_CTRL=	0;config_io_h.C19_BRAKE_SW_L4_CTRL=	1;}else if(g_io_fonfigure.C19_BRAKE_SW_L4_CTRL ==	2){	 config_io_l.C19_BRAKE_SW_L4_CTRL=	0;config_io_h.C19_BRAKE_SW_L4_CTRL=	0;}else;
 if(g_io_fonfigure.C20_BRAKE_SW_R1_CTRL==	0){	 config_io_l.C20_BRAKE_SW_R1_CTRL =	1;	config_io_h.C20_BRAKE_SW_R1_CTRL=	0;}	else if(g_io_fonfigure.C20_BRAKE_SW_R1_CTRL ==	1){	 config_io_l.C20_BRAKE_SW_R1_CTRL=	0;config_io_h.C20_BRAKE_SW_R1_CTRL=	1;}else if(g_io_fonfigure.C20_BRAKE_SW_R1_CTRL ==	2){	 config_io_l.C20_BRAKE_SW_R1_CTRL=	0;config_io_h.C20_BRAKE_SW_R1_CTRL=	0;}else;
 if(g_io_fonfigure.C21_BRAKE_SW_R2_CTRL==	0){	 config_io_l.C21_BRAKE_SW_R2_CTRL =	1;	config_io_h.C21_BRAKE_SW_R2_CTRL=	0;}	else if(g_io_fonfigure.C21_BRAKE_SW_R2_CTRL ==	1){	 config_io_l.C21_BRAKE_SW_R2_CTRL=	0;config_io_h.C21_BRAKE_SW_R2_CTRL=	1;}else if(g_io_fonfigure.C21_BRAKE_SW_R2_CTRL ==	2){	 config_io_l.C21_BRAKE_SW_R2_CTRL=	0;config_io_h.C21_BRAKE_SW_R2_CTRL=	0;}else;
 if(g_io_fonfigure.C22_BRAKE_SW_R3_CTRL==	0){	 config_io_l.C22_BRAKE_SW_R3_CTRL =	1;	config_io_h.C22_BRAKE_SW_R3_CTRL=	0;}	else if(g_io_fonfigure.C22_BRAKE_SW_R3_CTRL ==	1){	 config_io_l.C22_BRAKE_SW_R3_CTRL=	0;config_io_h.C22_BRAKE_SW_R3_CTRL=	1;}else if(g_io_fonfigure.C22_BRAKE_SW_R3_CTRL ==	2){	 config_io_l.C22_BRAKE_SW_R3_CTRL=	0;config_io_h.C22_BRAKE_SW_R3_CTRL=	0;}else;
 if(g_io_fonfigure.C23_BRAKE_SW_R4_CTRL==	0){	 config_io_l.C23_BRAKE_SW_R4_CTRL =	1;	config_io_h.C23_BRAKE_SW_R4_CTRL=	0;}	else if(g_io_fonfigure.C23_BRAKE_SW_R4_CTRL ==	1){	 config_io_l.C23_BRAKE_SW_R4_CTRL=	0;config_io_h.C23_BRAKE_SW_R4_CTRL=	1;}else if(g_io_fonfigure.C23_BRAKE_SW_R4_CTRL ==	2){	 config_io_l.C23_BRAKE_SW_R4_CTRL=	0;config_io_h.C23_BRAKE_SW_R4_CTRL=	0;}else;
 if(g_io_fonfigure.C24_PARK_CTRL==	0){	 config_io_l.C24_PARK_CTRL =	1;	config_io_h.C24_PARK_CTRL=	0;}	else if(g_io_fonfigure.C24_PARK_CTRL ==	1){	 config_io_l.C24_PARK_CTRL=	0;config_io_h.C24_PARK_CTRL=	1;}else if(g_io_fonfigure.C24_PARK_CTRL ==	2){	 config_io_l.C24_PARK_CTRL=	0;config_io_h.C24_PARK_CTRL=	0;}else;
 if(g_io_fonfigure.C25_BCM_CTRL==	0){	 config_io_l.C25_BCM_CTRL =	1;	config_io_h.C25_BCM_CTRL=	0;}	else if(g_io_fonfigure.C25_BCM_CTRL ==	1){	 config_io_l.C25_BCM_CTRL=	0;config_io_h.C25_BCM_CTRL=	1;}else if(g_io_fonfigure.C25_BCM_CTRL ==	2){	 config_io_l.C25_BCM_CTRL=	0;config_io_h.C25_BCM_CTRL=	0;}else;
 if(g_io_fonfigure.C26_PASSAGER_SAFE_L_CTRL==	0){	 config_io_l.C26_PASSAGER_SAFE_L_CTRL =	1;	config_io_h.C26_PASSAGER_SAFE_L_CTRL=	0;}	else if(g_io_fonfigure.C26_PASSAGER_SAFE_L_CTRL ==	1){	 config_io_l.C26_PASSAGER_SAFE_L_CTRL=	0;config_io_h.C26_PASSAGER_SAFE_L_CTRL=	1;}else if(g_io_fonfigure.C26_PASSAGER_SAFE_L_CTRL ==	2){	 config_io_l.C26_PASSAGER_SAFE_L_CTRL=	0;config_io_h.C26_PASSAGER_SAFE_L_CTRL=	0;}else;
 if(g_io_fonfigure.A7_AIR_FILTER_CTRL==	1){	 config_io_l.A7_AIR_FILTER_CTRL =	1;	config_io_h.A7_AIR_FILTER_CTRL=	0;}	else if(g_io_fonfigure.A7_AIR_FILTER_CTRL ==	0){	 config_io_l.A7_AIR_FILTER_CTRL=	0;config_io_h.A7_AIR_FILTER_CTRL=	1;}else if(g_io_fonfigure.A7_AIR_FILTER_CTRL ==	2){	 config_io_l.A7_AIR_FILTER_CTRL=	0;config_io_h.A7_AIR_FILTER_CTRL=	0;}else;
 if(g_io_fonfigure.A8_DRIVER_DOOR_CTRL==	1){	 config_io_l.A8_DRIVER_DOOR_CTRL =	1;	config_io_h.A8_DRIVER_DOOR_CTRL=	0;}	else if(g_io_fonfigure.A8_DRIVER_DOOR_CTRL ==	0){	 config_io_l.A8_DRIVER_DOOR_CTRL=	0;config_io_h.A8_DRIVER_DOOR_CTRL=	1;}else if(g_io_fonfigure.A8_DRIVER_DOOR_CTRL ==	2){	 config_io_l.A8_DRIVER_DOOR_CTRL=	0;config_io_h.A8_DRIVER_DOOR_CTRL=	0;}else;
 if(g_io_fonfigure.A9_SAFETY_BELT_CTRL==	1){	 config_io_l.A9_SAFETY_BELT_CTRL =	1;	config_io_h.A9_SAFETY_BELT_CTRL=	0;}	else if(g_io_fonfigure.A9_SAFETY_BELT_CTRL ==	0){	 config_io_l.A9_SAFETY_BELT_CTRL=	0;config_io_h.A9_SAFETY_BELT_CTRL=	1;}else if(g_io_fonfigure.A9_SAFETY_BELT_CTRL ==	2){	 config_io_l.A9_SAFETY_BELT_CTRL=	0;config_io_h.A9_SAFETY_BELT_CTRL=	0;}else;
 if(g_io_fonfigure.A10_LOW_GEAR_CTRL==	1){	 config_io_l.A10_LOW_GEAR_CTRL =	1;	config_io_h.A10_LOW_GEAR_CTRL=	0;}	else if(g_io_fonfigure.A10_LOW_GEAR_CTRL ==	0){	 config_io_l.A10_LOW_GEAR_CTRL=	0;config_io_h.A10_LOW_GEAR_CTRL=	1;}else if(g_io_fonfigure.A10_LOW_GEAR_CTRL ==	2){	 config_io_l.A10_LOW_GEAR_CTRL=	0;config_io_h.A10_LOW_GEAR_CTRL=	0;}else;
 if(g_io_fonfigure.A22_DRYER_CTRL==	1){	 config_io_l.A22_DRYER_CTRL =	1;	config_io_h.A22_DRYER_CTRL=	0;}	else if(g_io_fonfigure.A22_DRYER_CTRL ==	0){	 config_io_l.A22_DRYER_CTRL=	0;config_io_h.A22_DRYER_CTRL=	1;}else if(g_io_fonfigure.A22_DRYER_CTRL ==	2){	 config_io_l.A22_DRYER_CTRL=	0;config_io_h.A22_DRYER_CTRL=	0;}else;
 if(g_io_fonfigure.A23_AIR_SUSPENSION_CTRL==	1){	 config_io_l.A23_AIR_SUSPENSION_CTRL =	1;	config_io_h.A23_AIR_SUSPENSION_CTRL=	0;}	else if(g_io_fonfigure.A23_AIR_SUSPENSION_CTRL ==	0){	 config_io_l.A23_AIR_SUSPENSION_CTRL=	0;config_io_h.A23_AIR_SUSPENSION_CTRL=	1;}else if(g_io_fonfigure.A23_AIR_SUSPENSION_CTRL ==	2){	 config_io_l.A23_AIR_SUSPENSION_CTRL=	0;config_io_h.A23_AIR_SUSPENSION_CTRL=	0;}else;
 if(g_io_fonfigure.A24_FLOATING_BRIDGE_CTRL==	1){	 config_io_l.A24_FLOATING_BRIDGE_CTRL =	1;	config_io_h.A24_FLOATING_BRIDGE_CTRL=	0;}	else if(g_io_fonfigure.A24_FLOATING_BRIDGE_CTRL ==	0){	 config_io_l.A24_FLOATING_BRIDGE_CTRL=	0;config_io_h.A24_FLOATING_BRIDGE_CTRL=	1;}else if(g_io_fonfigure.A24_FLOATING_BRIDGE_CTRL ==	2){	 config_io_l.A24_FLOATING_BRIDGE_CTRL=	0;config_io_h.A24_FLOATING_BRIDGE_CTRL=	0;}else;
 if(g_io_fonfigure.A25_RESERVE_CTRL==	1){	 config_io_l.A25_RESERVE_CTRL =	1;	config_io_h.A25_RESERVE_CTRL=	0;}	else if(g_io_fonfigure.A25_RESERVE_CTRL ==	0){	 config_io_l.A25_RESERVE_CTRL=	0;config_io_h.A25_RESERVE_CTRL=	1;}else if(g_io_fonfigure.A25_RESERVE_CTRL ==	2){	 config_io_l.A25_RESERVE_CTRL=	0;config_io_h.A25_RESERVE_CTRL=	0;}else;
 if(g_io_fonfigure.A26_RECHARGE_CTRL==	1){	 config_io_l.A26_RECHARGE_CTRL =	1;	config_io_h.A26_RECHARGE_CTRL=	0;}	else if(g_io_fonfigure.A26_RECHARGE_CTRL ==	0){	 config_io_l.A26_RECHARGE_CTRL=	0;config_io_h.A26_RECHARGE_CTRL=	1;}else if(g_io_fonfigure.A26_RECHARGE_CTRL ==	2){	 config_io_l.A26_RECHARGE_CTRL=	0;config_io_h.A26_RECHARGE_CTRL=	0;}else;
 if(g_io_fonfigure.IN_36 ==	0){	 config_io_l.IN_36 =	1;	config_io_h.IN_36=	0;}	else if(g_io_fonfigure.IN_36 ==	1){	 config_io_l.IN_36=	0;config_io_h.IN_36=	1;}else if(g_io_fonfigure.IN_36 ==	2){	 config_io_l.IN_36=	0;config_io_h.IN_36=	0;}else;
 if(g_io_fonfigure.IN_37 ==	0){	 config_io_l.IN_37 =	1;	config_io_h.IN_37=	0;}	else if(g_io_fonfigure.IN_37 ==	1){	 config_io_l.IN_37=	0;config_io_h.IN_37=	1;}else if(g_io_fonfigure.IN_37 ==	2){	 config_io_l.IN_37=	0;config_io_h.IN_37=	0;}else;
 if(g_io_fonfigure.IN_38 ==	0){	 config_io_l.IN_38 =	1;	config_io_h.IN_38=	1;}	else if(g_io_fonfigure.IN_38 ==	1){	 config_io_l.IN_38=	0;config_io_h.IN_38=	1;}else if(g_io_fonfigure.IN_38 ==	2){	 config_io_l.IN_38=	1;config_io_h.IN_38=	1;}else;
 if(g_io_fonfigure.IN_39 ==	0){	 config_io_l.IN_39 =	1;	config_io_h.IN_39=	1;}	else if(g_io_fonfigure.IN_39 ==	1){	 config_io_l.IN_39=	0;config_io_h.IN_39=	1;}else if(g_io_fonfigure.IN_39 ==	2){	 config_io_l.IN_39=	1;config_io_h.IN_39=	1;}else;
 if(g_io_fonfigure.IN_40 ==	0){	 config_io_l.IN_40 =	1;	config_io_h.IN_40=	1;}	else if(g_io_fonfigure.IN_40 ==	1){	 config_io_l.IN_40=	0;config_io_h.IN_40=	1;}else if(g_io_fonfigure.IN_40 ==	2){	 config_io_l.IN_40=	1;config_io_h.IN_40=	1;}else;



   CS_H = 0;
   for(j = 5; j >0; j--)
   {
        dump = config_io_h.byte[j-1];
        for(i = 0; i < 8; i++)
        {
            MCLK_H = 0;
            if((dump & 0x80) == 0x80)
                MOSI_H= 1;
            else
                MOSI_H = 0;
            MCLK_H = 1;
            dump = (dump << 1);
        }
    }
   
    CS_H = 1;


     CS_L = 0;
       
   for(j = 5; j >0; j--)
      {
           dump = config_io_l.byte[j-1];
           for(i = 0; i < 8; i++)
           {
               MCLK_L = 0;
               if((dump & 0x80) == 0x80)
                   MOSI_L= 1;
               else
                   MOSI_L = 0;
               MCLK_L = 1;
               dump = (dump << 1);
           }
       }
      
       CS_L = 1;
}
void CONFIG_AD(void)
{

	return;
}


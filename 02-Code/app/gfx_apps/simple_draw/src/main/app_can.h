/** ****************************************************************************
 * @copyright Copyright (c) XXX
 * All rights reserved.
 *
 * @file    app_can.h
 * @brief   can application
 * @version V1.0.0
 * @author  liubing
 * @date    2019-3-5
 * @warning No Warnings
 * @note <b>history:</b>
 * 1.
 *
 */
#ifndef APP_CAN_H__
#define APP_CAN_H__

/*******************************************************************************
 * INCLUDES
 */
#include "r_typedefs.h"
#include "app_dm1.h"


#ifdef __cplusplus
extern "C"{
#endif

/**
 * \defgroup CAN APP:CAN
 * @{
 */


/*******************************************************************************
 * DEBUG SWITCH MACROS
 */


/*******************************************************************************
 * MACROS
 */

#define CAN_BL_App         (0x1u)
#define ICU_VERSIONS       "V01.12.43" //版本格式必须为: Vxx.yy.zz，xx:主版本号，yy:次版本号，zz:修订版本号
#define VERSIONS_DATE      (260518u)
#define VERSIONS_YEAR      (VERSIONS_DATE/10000)
#define VERSIONS_MONTH     (VERSIONS_DATE%10000/100)
#define VERSIONS_DAY       (VERSIONS_DATE%100)

#define VERSIONS_MAJOR	   ((ICU_VERSIONS[1] - 48) * 10 + (ICU_VERSIONS[2] - 48))
#define VERSIONS_MINOR	   ((ICU_VERSIONS[4] - 48) * 10 + (ICU_VERSIONS[5] - 48))
#define VERSIONS_REVISION  ((ICU_VERSIONS[7] - 48) * 10 + (ICU_VERSIONS[8] - 48))


#define VSPEED_ENLARGE_FLG   1

#define CAN_FRAME_ST_INIT           0
#define CAN_FRAME_ST_RECVED         1
#define CAN_FRAME_ST_TIMEOUT        2


// 软件版本暂定
// C5104 : eol_icutype = 0
// C3104 : eol_icutype = 1
// C3106 : eol_icutype = 2
// #define ICU_VERSIONS_C5104  "V01.08.02"
// #define ICU_VERSIONS_C3104  "V01.08.01"
// #define ICU_VERSIONS_C3106  "V01.08.00"



/*******************************************************************************
 * TYPEDEFS
 */
typedef enum {
	NODE_BCAN = 0,
	NODE_PCAN,
	NODE_ALL
	
} CanNode_e; 


typedef enum{
	NORMAL_MODE = 0,
	STANDBY_MODE = 1,
	GO_TO_SLEEP_MODE = 2,
}can_mode_t;


#define CAN1_STB  PORT_ISO.P3.BIT.P3_2
#define CAN1_EN   PORT_ISO.P3.BIT.P3_3
#define CAN2_STB  PORT_ISO.P3.BIT.P3_7
#define CAN2_EN   PORT_ISO.P3.BIT.P3_8

#define CAN1_ERR     PORT_ISO.PPR3.BIT.PPR3_4
#define CAN2_ERR     PORT_ISO.PPR3.BIT.PPR3_9

#define MCU_WAKE_CAN2  PORT_ISO.P1.BIT.P1_0
#define MCU_WAKE_CAN1  PORT_ISO.P1.BIT.P1_1

#define CAN_2_CH  2
#define CAN_1_CH  1



/**
 * \brief can 接收
 */

typedef struct canRxFrameLCfg {
    uint32_t id;		// can id
    uint32_t tmoMs;     // 超时时间
    void (*tmoHandle)(uint32_t id, uint8_t *buffer);	//超时处理
    void (*recvHandle)(uint32_t id, uint8_t *buffer);	//接收处理
}canRxFrameLCfg_t;

typedef struct {
    const canRxFrameLCfg_t *lcfg;
    uint8_t (*buffer)[8];		//指向帧缓冲区数组
    uint32_t *timer;          	//指向帧计时数
    uint8_t *st;				//帧状

}canRxFrameCfg_t;


#pragma pack(1)
typedef union {
	struct {
		unsigned bit1 : 1;
		unsigned bit2 : 1;
		unsigned bit3 : 1;
		unsigned bit4 : 1;
		unsigned bit5 : 1;
		unsigned bit6 : 1;
		unsigned bit7 : 1;
		unsigned bit8 : 1;
	};
	struct {
		unsigned bit12 : 2;
		unsigned bit34 : 2;
		unsigned bit56 : 2;
		unsigned bit78 : 2;
	};
	struct {
		unsigned bit1234 : 4;
		unsigned bit5678 : 4;
	};
	
#if 0
	struct {
		unsigned   : 1;
        unsigned  bit23 : 2;
        unsigned  bit45 : 2;
		unsigned  bit67 : 2;
		unsigned   : 1;
    };
	struct {
		unsigned  bit123 : 3;
		unsigned  bit456 : 3;
		unsigned   : 2;
    };
	struct {
		unsigned   : 1;
		unsigned  bit234 : 3;
		unsigned  bit567 : 3;
		unsigned   : 1;
    };
	struct {
		unsigned   : 2;
		unsigned  bit345 : 3;
		unsigned  bit678 : 3;
    };
	struct {
		unsigned   : 1;
		unsigned  bit2345 : 4;
		unsigned   : 3;
    };
	struct {
		unsigned   : 2;
		unsigned  bit3456 : 4;
		unsigned   : 2;
    };
	struct {
		unsigned   : 3;
		unsigned  bit4567 : 4;
		unsigned   : 1;
    };
	
	struct {
		uint8_t  bit12345 : 5;
		uint8_t   : 3;
    };
	struct {
		uint8_t   : 1;
		uint8_t  bit23456 : 5;
		uint8_t   : 2;
    };
	struct {
		uint8_t   : 2;
		uint8_t  bit34567 : 5;
		uint8_t   : 1;
    };
	struct {
		uint8_t   : 3;
		uint8_t  bit45678 : 5;
    };
	
	struct {
		uint8_t  bit123456 : 6;
		uint8_t   : 2;
    };
	struct {
		uint8_t   : 1;
		uint8_t  bit234567 : 6;
		uint8_t   : 1;
    };
	struct {
		uint8_t   : 2;
		uint8_t  bit345678 : 6;
    };
	
	struct {
		uint8_t  bit1234567 : 7;
		uint8_t   : 1;
    };
	struct {
		uint8_t   : 1;
		uint8_t  bit2345678 : 7;
    };
#endif
    unsigned char byte;
} BYTE_t;
#pragma pack()


#pragma pack(1)
typedef union {
    uint8_t bData[8];
    struct {
	BYTE_t byte1;
	BYTE_t byte2;
	BYTE_t byte3;
	BYTE_t byte4;
	BYTE_t byte5;
	BYTE_t byte6;
	BYTE_t byte7;
	BYTE_t byte8;
    };
}GeneralUse_t;
#pragma pack()


// EPB
#pragma pack(1)
typedef union {
    uint8_t bData[8];
    struct {
        // byte 1~2
        uint8_t res1;
		uint8_t handbrake_release:4, /* 手刹释放 */
				res2:4;
		uint8_t temporary_parking_brake_activated:2, /* 临停激活 */
				byte3_bit34:2,
				byte3_bit56:2,
				byte3_bit78:2;
		uint8_t res4;
		
        // byte 5
        uint8_t parking_brake;
        // byte 6~8
        uint8_t work_mode;
		// byte 7
        uint8_t Trailer_test:2,
		        res7:6;
		// byte 8
		uint8_t res8;
		
    };
}EPB_18FF3C50_t;
#pragma pack()


#pragma pack(1)
typedef union {
    uint8_t bData[8];
    struct {
        // byte 0
        uint8_t res1;
        // byte 1-2
        uint16_t outputvoltage;             

        // byte 3-4
        uint16_t outputcurrent;           
        //byte5-6
        uint16_t res2;
        // byte 7
        uint8_t soc;

    };
}BMS_0CFFEAF4_t;
#pragma pack()

#pragma pack(1)
typedef union {
    uint8_t bData[8];
    struct {
        // byte 0-1
        uint16_t linecurrent;
        // byte 2-3
        uint16_t linevoltage;             

        // byte 4-5
        uint16_t rpm;           //0.25r/bit
        //byte6-7
        uint16_t torch;

    };
}MCU_0CFF11EF_t;
#pragma pack()

#pragma pack(1)
typedef union {
    uint8_t bData[8];
    struct {
		
	uint8_t gear : 2,
			powerlimit:2,
			res1:4;

	uint8_t	extendmiles;

	uint8_t xunhang_status:2,
			pto_status:2,
			ready:1,
			cruise_switch:1,
			res2:2;

	uint8_t xunhangspeed;

	uint16_t piorpm;

	uint8_t status:4,
			mode:2,
			gear_status:2;
			
	uint8_t res4;

    };
}VCU_18FFF531_t;
#pragma pack()

//lyx
#pragma pack(1)
typedef union {
    uint8_t bData[8];
    struct {
        uint8_t ready:1,
                res1:2,
                ep_fault_level_warning:1,
                park_switch_status:1,
                low_battery_warning:1,
                hv_batt_cutoff:1,
                eps_fault:1;

        uint8_t vcu_mot_over_temp:1,
                soc_low_warning:1,
                res2:6;

        uint8_t res3[6];
    };
}VCU_04F02270_t;
#pragma pack()

#pragma pack(1)
typedef union {
    uint8_t bData[8];
    struct {
        uint8_t batt_soc;

        uint8_t chg_line_sts:2,
                charging_indication:2,
                res1:4;

        uint16_t pack_current;
        uint16_t pack_voltage;
        uint16_t abs_vehicle_speed;
    };
}VCU_04F02370_t;
#pragma pack()

#pragma pack(1)
typedef union {
    uint8_t bData[8];
    struct {
        uint16_t eps_sys_fault_code;
        uint16_t acm_sys_fault_code;
        uint8_t drive_mcu_sys_fault_code;
        uint8_t stir_mcu_sys_fault_code;
        uint8_t suction_head_mcu_sys_fault_code;
        uint8_t front_conveyor_mcu_sys_fault_code;
    };
}VCU_04F02470_t;
#pragma pack()

#pragma pack(1)//lyx
typedef union {
    uint8_t bData[8];
    struct {
        uint8_t vcu_sys_fault_code;
        uint8_t bms_sys_fault_code;
        uint16_t dcdc_sys_fault_code;
        uint16_t pdu_sys_fault_code;
        uint8_t power_unit_mcu_sys_fault_code;
        uint8_t sprinkle_tape_mcu_sys_fault_code;
    };
}VCU_04F02570_t;
#pragma pack()

#pragma pack(1)//lyx
typedef union {
    uint8_t bData[8];
    struct {
        uint8_t vcu_sys_fault_level:2,
                drive_mcu_sys_fault_level:2,
                eps_sys_fault_level:2,
                acm_sys_fault_level:2;

        uint8_t dcdc_sys_fault_level:2,
                bms_sys_fault_level:2,
                stir_mcu_sys_fault_level:2,
                suction_head_mcu_sys_fault_level:2;

        uint8_t front_conveyor_mcu_sys_fault_level:2,
                pdu_sys_fault_level:2,
                power_unit_mcu_sys_fault_level:2,
                sprinkle_tape_mcu_sys_fault_level:2;

        uint8_t res[5];
    };
}VCU_04F02670_t;
#pragma pack()

#pragma pack(1)
typedef union {
    uint8_t bData[8];
    struct {
        uint8_t drive_mcu_temperature;
        uint8_t drive_mot_temperature;
        uint8_t eps_dcac_temperature;
        uint8_t eps_mot_temperature;
        uint8_t acm_dcac_temperature;
        uint8_t acm_mot_temperature;
        uint8_t eps_dcdc_temperature;
        uint8_t res;
    };
}VCU_04F02770_t;
#pragma pack()

#pragma pack(1)
typedef union {
    uint8_t bData[8];
    struct {
        uint8_t stir_mcu_temperature;
        uint8_t stir_mot_temperature;
        uint8_t suction_head_mcu_temperature;
        uint8_t suction_head_mot_temperature;
        uint8_t front_conveyor_mcu_temperature;
        uint8_t front_conveyor_mot_temperature;
        uint8_t res[2];
    };
}VCU_04F02870_t;
#pragma pack()

#pragma pack(1)
typedef union {
    uint8_t bData[8];
    struct {
        uint8_t power_unit_mcu_temperature;
        uint8_t power_unit_mot_temperature;
        uint8_t sprinkle_tape_mcu_temperature;
        uint8_t sprinkle_tape_mot_temperature;
        uint8_t res[4];
    };
}VCU_04F02970_t;
#pragma pack()

#pragma pack(1)
typedef union {
    uint8_t bData[8];
    struct {
        uint16_t drive_mot_speed;
        uint16_t eps_mot_speed;
        uint16_t acm_mot_speed;
        uint8_t brk_air_pressure1;
        uint8_t res;
    };
}VCU_04F02A70_t;
#pragma pack()

#pragma pack(1)
typedef union {
    uint8_t bData[8];
    struct {
        uint16_t stir_mot_speed;
        uint16_t suction_head_mot_speed;
        uint16_t front_conveyor_mot_speed;
        uint8_t brk_air_pressure2;
        uint8_t res;
    };
}VCU_04F02B70_t;
#pragma pack()

#pragma pack(1)
typedef union {
    uint8_t bData[8];
    struct {
        uint16_t power_unit_mot_speed;
        uint16_t sprinkle_tape_mot_speed;
        uint8_t res[4];
    };
}VCU_04F02C70_t;
#pragma pack()

#pragma pack(1)
typedef union {
    uint8_t bData[8];
    struct {
        uint8_t k1_relay_state:2,
                k2_relay_state:2,
                k3_relay_state:2,
                k4_relay_state:2;

        uint8_t k5_relay_state:2,
                k6_relay_state:2,
                k7_relay_state:2,
                k8_relay_state:2;

        uint8_t k9_relay_state:2,
                k10_relay_state:2,
                k11_relay_state:2,
                k12_relay_state:2;

        uint8_t k13_relay_state:2,
                k14_relay_state:2,
                k15_relay_state:2,
                k16_relay_state:2;

        uint8_t res[4];
    };
}VCU_04F02D70_t;
#pragma pack()

#pragma pack(1)
typedef union {
    uint8_t bData[8];
    struct {
        uint16_t positive_insulation;
        uint16_t negative_insulation;
        uint16_t system_insulation;
        uint8_t res[2];
    };
}BMS_1885EFF3_t;
#pragma pack()

#pragma pack(1)
typedef union {
    uint8_t bData[8];
    struct {
        uint8_t max_temperature;
        uint8_t min_temperature;
        uint8_t avg_temperature;
        uint8_t max_temp_csc_no;
        uint8_t max_temp_position;
        uint8_t min_temp_csc_no;
        uint8_t min_temp_position;
        uint8_t res;
    };
}BMS_1886EFF3_t;
#pragma pack()

#pragma pack(1)
typedef union {
    uint8_t bData[8];
    struct {
        uint16_t max_ucell;
        uint16_t min_ucell;
        uint16_t avg_ucell;
        uint8_t res[2];
    };
}BMS_1887EFF3_t;
#pragma pack()

#pragma pack(1)
typedef union {
    uint8_t bData[8];
    struct {
        uint8_t max_ucell_csc_no;
        uint16_t max_ucell_position;
        uint8_t min_ucell_csc_no;
        uint16_t min_ucell_position;
        uint8_t res[2];
    };
}BMS_1888EFF3_t;
#pragma pack()

#pragma pack(1)
typedef union {
    uint8_t bData[8];
    struct {
	uint8_t  wheelID;
	uint8_t  temperature;
	unsigned pressure:10;
	unsigned res1:4;
	unsigned fast_pressure_lost:2;

	unsigned res2:8;
	
	unsigned res3:2;
	unsigned pressure_status:4;
	unsigned res4:2;

	unsigned over_temperature_status:2;
	unsigned res5:6;
	
	unsigned res6:8;
	
    };
}BCM_18FFE633_t;
#pragma pack()

#pragma pack(1)
typedef	union {
	struct {
		unsigned H2concentration:16;
		unsigned :16;
		uint32_t :32;
	};
	uint8_t bData[8];
}FCV_0x18FFA5F6_t;
#pragma pack()



extern uint32_t wheelupdated;

extern BCM_18FFE633_t Wheels[4][4];
extern uint8_t  wheelupdatedtimer[4][4];
extern uint8_t  backvalue;
extern uint8_t  study;
extern uint8_t  tire;
extern uint8_t  learnstatus;

extern uint32_t wake_can_cnt;
extern uint8_t  Test_Mode;




void can_init(void);
void can_rxRoutine(void);

uint8_t * can_getPCanBuffer(uint32_t canid);
uint8_t can_getPCanRxState(uint32_t canid);
uint8_t * can_getBCanBuffer(uint32_t canid);
uint8_t can_getBCanRxState(uint32_t canid);

uint8_t *can_GetCanRxBuffer(CanNode_e node_index, uint32_t canid);
uint8_t can_GetCanRxState(CanNode_e node_index, uint32_t canid);

void can_chip_mode_switch(uint8_t channel,can_mode_t mode);

void Task10ms_can_send(void);
void Task100ms_can_send(void);

void Bcan_to_Pcan_N10ms(void);
void Bcan_to_Pcan_N100ms(void);

void Pcan_to_Bcan_N10ms(void);
void Pcan_to_Bcan_N100ms(void);

void PCAN_Send_DM1(void) ;
void PCAN_Send_TestRequest(uint8_t req);
void PCAN_Send_TestResult(void);
void PCAN_UDS_SEND (uint8_t msg_buf[], uint8_t length);

void canBusOffDetectionTask(void);
extern uint8_t  can_busoff_flg[3];

void BCAN_Send_TPMSLearningEnd(void);
extern void PCAN_Send_AUXIO1(void);
extern void CAN_Send_TCO1(void);

void CAN_Send_MotSpeedObj1(void);
void CAN_Send_MotSpeedObj2(void);
/**
 * end of group CAN
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif




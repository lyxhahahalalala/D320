/***************************************************************************//**
    \file          obd-dtc.h
    \author        
    \mail          
    \version       0
    \date          2016-10-10
    \description   obd dtc code, according SIO 14229(2006) D.2.2
*******************************************************************************/
#ifndef	__OBD_DTC_H_
#define	__OBD_DTC_H_
/*******************************************************************************
    Include Files
*******************************************************************************/
// #include <stdint.h>
#include "r_typedefs.h"
#include "uds_type.h"
#include "obd_dtc_private.h"
/*******************************************************************************
    Type Definition
*******************************************************************************/

#define DTC_FORMAT_15031                  (0x00)
#define DTC_FORMAT_14229                  (0x01)
#define DTC_AVAILABILITY_STATUS_MASK      (0x7F)

#define DTC_TEST_PASSED     (1u)
#define DTC_TEST_NORESULT   (0u)
#define DTC_TEST_FAILED     (-1u)

#define DTC_OFF             (TRUE)
#define DTC_ON              (FALSE)

/* uds DTC group */
typedef enum __OBD_DTC_TEST__
{
	OBD_DTC_TEST_NORESULT = 0,
	OBD_DTC_TEST_PASSED,
	OBD_DTC_TEST_FAILED
}obd_dtc_test_t;

/* uds DTC group */
typedef enum __UDS_DTC_GROUP__
{
	UDS_DTC_GROUP_EMISSION = 0,
	UDS_DTC_GROUP_NETWORK = 0xC00000,
	UDS_DTC_GROUP_ALL = 0xFFFFFF
}uds_dtc_group;

typedef enum __OBD_DTC_NAME_T__
{
    DTC_FRONTAIRSENSOR_VOLTAGE_HIGH = 0,//前桥回路气压传感器电压高(523004,3)
    DTC_FRONTAIRSENSOR_VOLTAGE_LOW,//前桥回路气压传感器电压低(523004,4)
    DTC_REARAIRSENSOR_VOLTAGE_HIGH, //后桥回路气压传感器电压高(523005,3)
    DTC_REARAIRSENSOR_VOLTAGE_LOW,  //后桥回路气压传感器电压低(523005,4)
    DTC_CLUSTERINFAULT1,			   //内部故障(523007,12)
    DTC_CLUSTERINFAULT2,			    //内部故障(523034,8)仪表实时时钟故障
    DTC_CAN1OFF,						//仪表:CAN 1总线故障(523008,8)
    DTC_CAN2OFF,						//CAN 2总线故障(523009,8)
    DTC_BCMTIMEOUT,					//BCM报文超时(523010,8)
    DTC_LIGHT_WATERTEMP_HIGH,			//水温过高指示灯故障(523020,12)
    DTC_LIGHT_DOOROPEN,				//门开报警灯故障(523022,12)
    DTC_L1_BRAKE_FRICTION,				//第一桥左轮摩擦片磨损(523023,12)
    DTC_R1_BRAKE_FRICTION,
    DTC_L2_BRAKE_FRICTION,
    DTC_R2_BRAKE_FRICTION,
    DTC_L3_BRAKE_FRICTION,				//第一桥左轮摩擦片磨损(523023,12)
    DTC_R3_BRAKE_FRICTION,
    DTC_L4_BRAKE_FRICTION,
    DTC_R4_BRAKE_FRICTION,
    DTC_AMT,				//离合器磨损(523031,0)
    DTC_AMT_AIRPRESS_LOW,	//空气压力过低(523032,0)
    DTC_TPMSTIMEOUT,
    DTC_ABSTIMEOUT,
    DTC_EBSTIMEOUT,
    DTC_ENVTMPSENSORFAULT,//环境温度传感器信号异常(171,2)
    DTC_ENVTMPSENSORVOLTAGE_HIGH,//环境温度传感器电压高或短路到电源或开路
    DTC_ENVTMPSENSORVOLTAGE_LOW,//环境温度传感器短路到地
    DTC_AMTOILTEMPSENSOR_H,//变速箱油温传感器电压高(177,3)
    DTC_AMTOILTEMPSENSOR_L,//变速箱油温传感器电压低(177,4)
    DTC_LIGHT_CHARGE,//不充电指示灯故障(523038,12)
    DTC_LIGHT_PARK,//驻车制动指示灯故障(523039,12)
    DTC_LIGHT_SEATBELT,//安全带指示灯故障(523040,12)
    DTC_AIRPRESSURE_LOW,//:空气压力过低
    DTC_ATMTIMEOUT,//AMT CAN报文超时(523042,8)
    DTC_SCUTIMEOUT,//SCU CAN报文超时(523043,8)
    DTC_LDWSTIMEOUT,//LDWS CAN报文超时(523047,8)
    DTC_AEBSTIMEOUT,//AEBS CAN报文超时(523048,8)
    
}obd_dtc_name_t;
typedef enum{
	PASSED,
	IN_TESTING,
	FAILED,
}DTCTestResult;

typedef DTCTestResult (*DetectFun)(void);
/* uds read/write data typedef */
#pragma pack(1)

typedef struct _DtcNode{
	const uint32_t DTCCode;
	DetectFun DetectFunction;//故障检测函数
	const uint16_t EEpromAddr;//dtc status ee address
	const uint16_t SnapShotEEpromAddr;//快照信息地址
	const uint8_t TripLimitTimes;
	dtc_status_t DTCStatus;
	uint8_t OldCounter;
	uint8_t GoneCounter;
	uint8_t TripCounter;//故障待定计数器
	uint8_t FaultOccurrences;//故障出现次数
	

}DTCNode;


typedef struct _Snapshot{
	 uint8_t snapshotRecordNumber;
	 uint16_t snapshotID;
	 uint8_t* dataPointer;
	 uint8_t dataLength;
}Snapshot;
#pragma pack()

/*******************************************************************************
    Function  Definition
*******************************************************************************/
/**
 * obd_dtc_ctrl - set the dtc off flag
 *
 * @val : dtc off value, TRUE or FALSE
 *
 * returns:
 *     void
 */
void
obd_dtc_ctrl (bool_t val);
/**
 * obd_update_dtc - manage the dtc status bits in a fault test operation cycle
 *
 * @dtc_n : dtc index
 * @test_result : obd fault detect result
 *
 * returns:
 *     void
 *
 */
void
uds_update_obddtc (uint16_t dtc_n, obd_dtc_test_t test_result);


/**
 * clear_dtc_by_group - clear dtc data by group
 *
 * @group : the dtc group need to clear
 *
 * returns:
 *     dtc data len
 */

void
clear_dtc_by_group (uint32_t group);

/**
 * get_dtc_number_by_status_mask - get dtc number by status mask
 *
 * @st_mask : dtc status mask
 *
 * returns:
 *     dtc number
 */
uint16_t
get_dtc_number_by_status_mask (uint8_t st_mask);

/**
 * get_dtc_by_status_mask - get dtc by status mask
 *
 * @dtc_buf : the dest buf to store dtc data
 * @buf_len : the len of the buf
 * @st_mask : dtc status mask
 *
 * returns:
 *     dtc data len
 */
uint16_t
get_dtc_by_status_mask (uint8_t dtc_buf[], uint16_t buf_len, uint8_t st_mask);
/**
 * get_supported_dtc - get all supported dtc
 *
 * @dtc_buf : the dest buf to store dtc data
 * @buf_len : the len of the buf
 *
 * returns:
 *     dtc data len
 */
uint16_t
get_supported_dtc (uint8_t dtc_buf[], uint16_t buf_len);


/**
 * get_dtc_snopshot_by_dtc_number - read all supported dtc to buffer
 *

 *
 * returns:
 *    no
 */
 uint16_t get_dtc_snopshot_by_dtc_number(uint8_t dtc_buf[], uint16_t buf_len, uint32_t eeprom_address, uint8_t ReordMask) ;
/**
 * EEROM_ReadWords - read eeprom data by address and length
 *
 * @dst : the target address
 * @rbuf[] : data to read into
 * @length : data length to read
 *
 * returns:
 *     no
 */
 void 
 EEROM_ReadWords(unsigned long dst,unsigned int rbuf[],unsigned char length);
/**
 * Diagnostic_EEProm_Write - write eeprom data by address and length
 *
 * @address : the target address
 * @data[] : data to write 
 * @size : data length to write
 *
 * returns:
 *     failed or pass
 */
uint8_t 
Diagnostic_EEProm_Write(uint32_t address, uint8_t size, uint8_t data[]);
/**
 * Diagnostic_EEProm_Read - reas eeprom data by address and length
 *
 * @address : the target address
 * @data : data pointer of readback data
 * @size : data length to read
 *
 * returns:
 *     failed or pass
 */
uint8_t 
Diagnostic_EEProm_Read(uint32_t address, uint8_t size, uint8_t *data);
/**
 * uds_diagnostic_load_alldata - load all dtc info
 *
 * parameters: no
 * returns:
 *    null
 */
void 
uds_diagnostic_load_alldata(void);
/**
 * get_dtcnode_by_code - get dtc node info
 *
 * @dtcCode : the dtc code of the target node
 *
 * returns:
 *     dtc node pointer
 */
DTCNode* 
get_dtcnode_by_code(uint32_t dtcCode);
/**
 * uds_diagnostic_dtc_process - dtc check, record process , all the dtc items
 *
 * parameters: no
 * returns:
 *     null
 */
void 
uds_diagnostic_dtc_process(void);

/**
 * uds_diagnostic_save_alldtc - save dtc info to eeprom
 *
 * parameters: no
 * returns:
 *     null
 */
void 
uds_diagnostic_save_alldtc(void);

/**
 * uds_eeprom_init -uds related info init
 *
 * parameters: no
 * returns:
 *     no
 */
void 
uds_eeprom_init(void);

/**
 * uds_diagnostic_save_singledtc - save certain dtc status info to eeprom
 *
 * @: dtcnumber dtc number
 * returns:
 *     null
 */
void 
uds_diagnostic_save_singledtc(uint8_t dtcnumber);
#endif
/****************EOF****************/

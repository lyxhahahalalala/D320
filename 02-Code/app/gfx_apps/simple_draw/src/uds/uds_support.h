/***************************************************************************//**
    \file          uds-support.h
    \author        
    \mail          
    \version       0
    \date         2020-03-26
    \description
*******************************************************************************/
#ifndef	__UDS_SUPPORT_H_
#define	__UDS_SUPPORT_H_
/*******************************************************************************
    Include Files
*******************************************************************************/
// #include <stdint.h>
#include "r_typedefs.h"
#include "uds_type.h"
/*******************************************************************************
    Type Definition
*******************************************************************************/
/* uds read/write data read-write mode */
typedef enum __UDS_RWDATA_RW__
{
	UDS_RWDATA_RDONLY = 0,  /*仅读*/
	UDS_RWDATA_RDWR,        /*可读写*/
    UDS_RWDATA_RDWR_WRONCE, /*可写入一次*/
    UDS_RWDATA_RDWR_INBOOT  /*在boot中读写*/
}uds_rwdata_rw;

/* uds read/write data read-write mode */
typedef enum __UDS_RWDATA_TYPE__
{
	UDS_RWDATA_HEX = 0,
	UDS_RWDATA_ASCII,
	UDS_RWDATA_BCD
}uds_rwdata_type;

/* uds read/write data read-write mode */
typedef enum __UDS_RWDATA_STORE__
{
	UDS_RWDATA_RAM = 0, /*读写 RAM*/
	UDS_RWDATA_DFLASH,  /*读写 dataflash*/
	UDS_RWDATA_EEPROM,  /*读写 eeprom*/
	UDS_RWDATA_ROM      /*读写 ROM*/
}uds_rwdata_store;

/* uds read/write data typedef */
typedef struct __UDS_RWDATA_T__
{
    uint16_t did;    /* 0100 - EFFF and F010 - F0FF for vehicleManufacturerSpecific */
    uint8_t *p_data;
    uint8_t dlc;
	uint32_t ee_address;
    uds_rwdata_rw rw_mode;
    uds_rwdata_store rw_store;
}uds_rwdata_t;

/* uds io control type */
typedef struct __UDS_IOCTRL_T__
{
    uint16_t did;
    uint8_t *p_data;
    uint8_t dlc;
    uint8_t default_value;
    uint8_t step;
    bool_t  enable;
    void (* init_ioctrl) (void);
    void (* stop_ioctrl) (void);
}uds_ioctrl_t;



/* uds routine control status */
typedef enum __UDS_ROUTINE_STATUS__
{
	UDS_RT_ST_IDLE = 0,
	UDS_RT_ST_RUNNING = 0x01,
	UDS_RT_ST_SUCCESS = 0x02,
	UDS_RT_ST_FAILED = 0x03
}uds_routine_status;

/* uds routine control */
typedef struct __UDS_RTCTROL_T__
{
    uint16_t rid;
    uds_routine_status rtst;
    uint8_t (* init_routine) (void);
    uint8_t (* run_routine) (void);
    uint8_t (* stop_routine) (void);
}uds_rtctrl_t;

#define RWDATA_CNT  49
#define IOCTRL_CNT  5
#define RTCTRL_CNT  2
#define LOGICDATA_CNT  22
extern const uds_rwdata_t rwdata_list[RWDATA_CNT];
extern const uds_rwdata_t Logicdata_list[LOGICDATA_CNT];
extern uds_ioctrl_t ioctrl_list[IOCTRL_CNT];
extern uds_rtctrl_t rtctrl_list[RTCTRL_CNT];


#define RWDATA_NUM  (sizeof(rwdata_list) / sizeof(uds_rwdata_t))
#define LOGICDATA_NUM  (sizeof(Logicdata_list) / sizeof(uds_rwdata_t))
#define IOCTRL_NUM  (sizeof(ioctrl_list) / sizeof(uds_ioctrl_t))
#define RTCTRL_NUM  (sizeof(rtctrl_list) / sizeof(uds_rtctrl_t))

enum{
/* 0*/HMPULSE = 0,
/* 1*/BIAOTOUXISHU,
/* 2*/BSXFENZI,
/* 3*/BSXFM,
/* 4*/LANGUAGE,
/* 5*/HOUCHULI,
/* 6*/FADONGJIORKAJI,
/* 7*/EMSECORPM,
/* 8*/EMSOVERRPM,
/* 9*/ATMTYPE,
/*10*/ABSEBS,
/*11*/TPMSTYPE,
/*12*/ECASTYPE,
/*13*/HUANSUQI,
/*14*/MCPWARN,
/*15*/ESCTYPE,
/*16*/LDWSSYS,
/*17*/PASSENGERSEATBELT,
/*18*/SENSORNUM,
/*19*/VEHICLETYPE,
/*20*/AEBSSYS,
/*21*/PAIFANG,
/*22*/ACTYPE,
/*23*/VECU,
/*24*/BAOYANGLICHENG,
/*25*/BAOYANGZHOUQI,
/*26*/BIANSUXIANGXINGHAoSUBI,
/*27*/DRIVETYPE,
/*28*/OVERSPEEDVALUE,
/*29*/QIYABAOJINGZHI,
/*30*/BSDCONFIG,
/*31*/PILAOJIASHIALARMCONFIG,
/*32*/PEPSCONFIG,
/*33*/BATTCOOLMODE,
/*34*/TBOX,
/*35*/SHUIWEI,
/*36*/XIANSUFUN,
/*37*/YINGJIZHUANXIANG,
/*38*/XINGSHIJILUYI,
/*39*/EPBTYPE,

/*40*/ICUTYPE,
/*41*/SPEED_ENLARGE_FACTOR,

/*42*/RANQIPINGCONFIG,
/*43*/RANQIXIELOUCONFIG,
/*44*/CIOMCONFIG,
/*45*/DIANYEZHUANXIANGCONFIG,
/*46*/JIASHISHITYPE,
/*47*/D600GANZAOQICONFIG,
/*48*/D600EPBCONFIG,
/*49*/
RWDATA_ID_NUM


};
enum{
	INDEX_F110 = 0,
	INDEX_F111,
	INDEX_F112,
	INDEX_F187,
	INDEX_F188,
	INDEX_F189,
	INDEX_F1A0,
	INDEX_F1A1,
	INDEX_F1A2,
	INDEX_F1A3,
	INDEX_F1A4,
	INDEX_F1A5,
	INDEX_F1A6,
	INDEX_F190,
	INDEX_F191,
	INDEX_F198,
	INDEX_F19B,
	INDEX_F19D,
	INDEX_F15A,
	INDEX_F123,
	INDEX_F010,
	INDEX_F011,
};

/*******************************************************************************
    Function  Definition
*******************************************************************************/
/**
 * uds_ioctrl_allstop - main handle of io control
 *
 * @void : 
 *
 * returns:
 *     void
 */
void uds_ioctrl_allstop (void);

/**
 * uds_load_rwdata - load read / write data from eeprom to ram
 *
 * @void : 
 *
 * returns:
 *     void
 */
void uds_load_rwdata (void);

/**
 * eol_data_read - read eol data on ign on
 *
 * @void :
 *
 * returns:
 *     0 - ok
 */
void eol_data_read(void);
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
uint8_t Diagnostic_EEProm_Write(uint32_t address, uint8_t size, uint8_t data[]);
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
uint8_t Diagnostic_EEProm_Read(uint32_t address, uint8_t size, uint8_t *data);
/**
 * EEROM_WriteEOL - reas eeprom data access
 *
 * @address : the target address
 * @data : data to be write
 *
 * returns:
 *     failed or pass
 */
uint32_t EEROM_WriteEOL(uint32_t Address, uint32_t data);//need complete
/**
 * EEROM_ReadWord - reas eeprom data access
 *
 * @dst : the target address
 *
 * returns:
 *     read back data
 */
unsigned int EEROM_ReadWord(unsigned long dst);//need complete

extern const uds_rwdata_t rwdata_list[RWDATA_CNT];
extern const uds_rwdata_t Logicdata_list[LOGICDATA_CNT];
extern uint8_t eol_hmpulse[4];
extern uint8_t eol_biaotouxishu[2];
extern uint8_t eol_biaosuxiangsubifenzi;
extern uint8_t eol_biaosuxiangsubifenmu;
extern uint8_t eol_biaosuxiangsubifenzi;
extern uint8_t eol_language;
extern uint8_t eol_atmtype;
extern uint8_t eol_ebs;
extern uint8_t eol_tpmsyesno;
extern uint8_t eol_ecastype;
extern uint8_t eol_huansuqi;
extern uint8_t eol_mcpbaojing;
extern uint8_t eol_esc;
extern uint8_t eol_ldwstype;
extern uint8_t eol_chengkecesaftybelt;
extern uint8_t eol_airsensortype;
extern uint8_t eol_vehicletype;
extern uint8_t eol_aebstype;
extern uint8_t eol_actype;
extern uint8_t eol_vecutype;
extern uint8_t eol_drivetype;
extern uint8_t eol_overspeedvalue;
extern uint16_t B_value;
extern float    HM_Value;
extern uint8_t eol_houchuli;
extern uint8_t eol_fadongjiorkaji;
extern uint8_t eol_emsjingjizhuansu[4];
extern uint8_t eol_emsbaojingzhaunsu[2];
extern uint8_t eol_vecu;
extern uint8_t eol_baoyanglicheng[2];
extern uint8_t eol_baoyangzhouqi;
extern uint8_t eol_biansuxiangxinghaojisubi[2];

extern uint8_t eol_qiyabaojingzhi[4];
extern uint8_t eol_bdsconfig;
extern uint8_t eol_dfmsconfig;
extern uint8_t eol_pepsconfig;
extern uint8_t eol_battcoolmd;

extern uint8_t eol_Tbox;
extern uint8_t eol_XianSuFunction; 
extern uint8_t eol_shuiwei;
extern uint8_t eol_yingjizhuanxiang;
extern uint8_t eol_xingshijiluyi;
extern uint8_t eol_epbtype;

extern uint8_t eol_ranqipingconfig;
extern uint8_t eol_ranqixielouconfig;
extern uint8_t eol_ciomconfig;
extern uint8_t eol_dianyezhuanxiangconfig;

extern uint8_t eol_jiashishitype;
extern uint8_t eol_d600ganzaoqiconfig;
extern uint8_t eol_d600epbconfig;

extern uint8_t eol_icutype;
extern uint8_t eol_speed_enlarge_factor;

extern uint16_t atmModelSpdRatio;
extern uint16_t QiyaLowLmt;
extern uint16_t QiyaHighLmt;
extern uint8_t  progrm_checkSuccFlg;

extern uint8_t F110_Integra_SVN[23];
extern uint8_t F111_App_SVN[23];
extern uint8_t F112_Bsw_SVN[23];

extern uint8_t F187_Spare_PN[13];
extern uint8_t F188_Ecu_SN[13];
extern uint8_t F189_Ecu_SVN[13];
extern uint8_t F1A0_RT_VN[5];
extern uint8_t F1A1_App_VN[5];
extern uint8_t F1A2_HW_Ver[5];
extern uint8_t F1A3_CalibData_VN[5];
extern uint8_t F1A4_SysSupplier_ID[5];
extern uint8_t F1A5_SysSupEol_MD[6];
extern uint8_t F1A6_SysSupEol_MSN[11];

extern uint8_t F190_VIN[17];
extern uint8_t F191_VehMan_Ecu_HN[13];
extern uint8_t F198_RepairShp_TSN[20];
extern uint8_t F19B_Calibration_Date[4];
extern uint8_t F19D_Ecu_Ins_Date[4];
extern uint8_t F15A_Audit_Trail[17];
extern uint8_t F123_Fbl_VN[5];

extern uint8_t F010_HW_Ver[8];
extern uint8_t F011_SW_Ver[8];

#endif
/****************EOF****************/

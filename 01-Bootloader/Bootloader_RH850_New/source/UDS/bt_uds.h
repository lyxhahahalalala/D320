
#ifndef _BT_UDS_H
#define _BT_UDS_H

/*!
 *********************************************************************
 * \moduldescription
 *          Header file for BT KWP services.
 *
 * \scope       INTERN
 *********************************************************************
 */

/*
 *********************************************************************
 * Includes
 *********************************************************************
 */
#include  "bt_pub.h"


extern bool udsProgRequest;


/* Length of negative response */
#define BTPROG_NR_LEN  0x01

/* Negative response codes */
#define BTPROG_GENREJECT               0x10  /* General reject                           */
#define BTPROG_SRVNOTSUPP              0x11  /* Service not supported                    */
#define BTPROG_SUBFCTSUPP              0x12  /* Sub function not supported		         */
#define BTPROG_IMLOIF                  0x13  /* Incorrect Msg Length of Invalid Format	 */
#define BTPROG_BRC                     0x21  /* Busy Repeat Code 				         */
#define BTPROG_CONDNCORR               0x22  /* Conditions not correct                   */
#define BTPROG_RNC          		   0x23  /* Routine not complete                     */
#define BTPROG_RSE                     0x24  /* Request Sequence Error			         */
#define BTPROG_RCROOR                  0x31  /* Request out of range                     */
#define BTPROG_ACCDENIED               0x33  /* Security access denied                   */
#define BTPROG_INVALID_KEY             0x35  /* Invalid key                              */
#define BTPROG_ENOA_DU8                0x36  /* exceedNumberOfAttempts                   */
#define BTPROG_RTDNE_DU8               0x37  /* requiredTimeDelayNotExpired              */
#define BTPROG_NODATACPTED             0x40  /* No more data are accepted                */
#define BTPROG_TDS                     0x71  /* transfer data suspended	                 */
#define BTPROG_GPF                     0x72  /* general Programming failure              */
#define BTPROG_WBSC                    0x73  /* wrong block sequence counter 		     */
#define BTPROG_SUBFCTSUPPSESS          0x7E  /* subfunc not supported in current session */
#define BTPROG_SRVNOTSUPPINCURSESSION  0x7F  /* Sevcie not supported in current session  */
#define BTPROG_CSERROR                 0xFA  /* Status checksum error                    */
#define BTPROG_ERASINGACT              0xFB  /* Status erasing active                    */
#define BTPROG_ERASINGERR              0xFD  /* Status erasing error                     */
#define BTPROG_SBREPROGFAIL            0xFE  /* Status reprogramming of BT fails         */

#define BTRANDOM32_INIT          0x0B6951A59UL
#define BTRANDOM32_MULTIPLIER    0x01C5A5035UL

#define STDIDRST_NULL     0
#define STDIDRST_OK       1
#define STDIDRST_LENERR   2
#define STDIDRST_RDERR    3




extern uint32_t BTEF_adStart_u32;
extern uint32_t BTEF_adSize_u32;
extern uint8_t  F123_Fbl_VN[5];

extern uint32_t systemTimer_1ms;

#define getCurrentTime systemTimer_1ms

typedef enum __UDS_RWDATA_RW__
{
	UDS_RWDATA_RDONLY = 0,
	UDS_RWDATA_RDWR,
    UDS_RWDATA_RDWR_WRONCE,
    UDS_RWDATA_RDWR_INBOOT
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
	UDS_RWDATA_RAM = 0,
	UDS_RWDATA_DFLASH,
	UDS_RWDATA_EEPROM,
	UDS_RWDATA_ROM
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


extern const uds_rwdata_t Logicdata_list[20];

enum{
/* 0*/INDEX_F110 = 0,
/* 1*/INDEX_F111,
/* 2*/INDEX_F112,
/* 3*/INDEX_F187,
/* 4*/INDEX_F188,
/* 5*/INDEX_F189,
/* 6*/INDEX_F1A0,
/* 7*/INDEX_F1A1,
/* 8*/INDEX_F1A2,
/* 9*/INDEX_F1A3,
/*10*/INDEX_F1A4,
/*11*/INDEX_F1A5,
/*12*/INDEX_F1A6,
/*13*/INDEX_F190,
/*14*/INDEX_F191,
/*15*/INDEX_F198,
/*16*/INDEX_F19B,
/*17*/INDEX_F19D,
/*18*/INDEX_F15A,
/*19*/INDEX_F123,

};



/* Extern declaration of services */
extern void BTDsc(BT_ServiceMsg_t* Msg_ps);
extern void BTRset_ECU(void);
extern void BTRId(BT_ServiceMsg_t* Msg_ps);
extern void BTWId(BT_ServiceMsg_t* Msg_ps);
extern void BTSecA(BT_ServiceMsg_t* Msg_ps);
extern void BTPF_ReqDwnl(BT_ServiceMsg_t* Msg_ps);
extern void BTPF_TransferData(BT_ServiceMsg_t* Msg_ps);
extern void BTPF_TransferExit(BT_ServiceMsg_t* Msg_ps);
extern void BTRset(BT_ServiceMsg_t* Msg_ps);
extern void BTTp ( BT_ServiceMsg_t* Msg_ps );
extern void BTStRt(BT_ServiceMsg_t* Msg_ps);


/* Declaration for Service Init */

/* routine declaration for Routine Control Service */
extern void BTEF_Start(BT_ServiceMsg_t* Msg_ps);
extern void BTCCFL_Start(BT_ServiceMsg_t* Msg_ps);

extern void BTCCFL_Do8s16r(void);
extern void EraseDo(void);
extern void BTPF_ProgDo(void);

extern uint16_t CL_MsrvChsCrc16_CalcChsCrc16Ccitt(const uint8_t* PAR_adStrt, const uint8_t* PAR_adEnd, uint16_t PAR_numStrtChs);

extern unsigned int EEROM_ReadWord(unsigned long dst);
uint8_t Diagnostic_EEProm_Write(uint32_t address, uint8_t size, uint8_t data[]);
uint8_t Diagnostic_EEProm_Read(uint32_t address, uint8_t size, uint8_t *data);



#endif

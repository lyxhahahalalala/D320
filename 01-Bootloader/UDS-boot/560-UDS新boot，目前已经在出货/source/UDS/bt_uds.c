/*!
 *********************************************************************
 * \moduldescription
 *          BT Services for erasing flash
 *
 * \scope  INTERN
 *********************************************************************
 */

/*
 *********************************************************************
 * Includes
 *********************************************************************
 */
#include  "common.h"
#include  "Bt_uds.h"
#include "can.h"
#include "BT_priv.h"
#include "BT_pub.h"
#include "fcl.h"
#include  "r_fcl_types.h"
#include "device.h"
#include "Dev_api.h"

enumErase eEraseActive;

/*
 *********************************************************************
 * Defines
 *********************************************************************
 */

#define ACCMOD_RBPROG   0x07

#define RID_ERASEFLASH      0xFF00
#define RID_CPDC            0xF001
#define RID_CHKVALIDAPP     0x0304
#define RID_UPDATEPBL       0xCCDD

#define BTPF_IDLE       0
#define BTCCFL_ST_IDLE  0
#define BTDSC_P2MAX     100
#define BTDSC_P2EXTMAX_DIV10  8000/10


/*
 *********************************************************************
 * Defines  eflash
 *********************************************************************
 */
#define BTEF_SR_LENREQ   12          /* Length of request for Erase Flash Request */
#define BTEF_POSLEN      3           /* Length of positive response   */
#define BTEF_NR_LEN
#define BTEF_START_HHB   4           /* Start address high byte of high word */
#define BTEF_START_HLB   5           /* Start address low byte of high word */
#define BTEF_START_LHB   6           /* Start address high byte of low word     */
#define BTEF_START_LLB   7           /* Start address low byte of low word        */
#define BTEF_END_HHB     8           /* End address high byte of high word         */
#define BTEF_END_HLB     9           /* End address low byte of high word       */
#define BTEF_END_LHB     10          /* End address high byte of low word    */
#define BTEF_END_LLB     11          /* End address low byte of low word     */
#define BTEF_HHB_OFFSET       24     /* Shift for 24 bits              */
#define BTEF_HLB_OFFSET       16     /* Shift for 16 bits              */
#define BTEF_LHB_OFFSET       8      /* Shift for 8  bits              */
#define BTEF_LLB_OFFSET       0


/*
 *********************************************************************
 * Defines  eflash
 *********************************************************************
 */
#define BTPF_LENRD       10      /* Length of request download                        */
#define BTPF_LENTX       0      /* Length of request transfer exit                   */
#define BTPF_RR_LENREQ   1      /* Length of request for RrRBLID                     */
#define BTPF_PRLEN_RD    3      /* Length of positive response request download      */
#define BTPF_PRLEN_TD    1      /* Length of positive response transfer data         */
#define BTPF_PRLEN_TX    0      /* Length of positive response request transfer exit */

#define BTPF_MAXDATA     4095     /* Maximum number of block length                  */
#define BTPF_ALFID       0x44     /* Address Length and Format Identifier            */
#define BTPF_LFID        0x20     /* Length Format Identifier in response            */

#define BTPF_START_HHB   2      /* Start address high byte of high word */
#define BTPF_START_HLB   3      /* Start address low byte of high word  */
#define BTPF_START_LHB   4      /* Start address high byte of low word  */
#define BTPF_START_LLB   5      /* Start address low byte of low word   */
#define BTPF_SIZE_HHB    6      /* Start address high byte of high word */
#define BTPF_SIZE_HLB    7      /* Start address low byte of high word */
#define BTPF_SIZE_LHB    8      /* Start address high byte of low word */
#define BTPF_SIZE_LLB    9      /* Start address low byte of low word */

#define BTPF_DFI_POS     0      /* Data format identifier, Position  */
#define BTPF_ALFID_POS   1      /* Address Length and Format Identifier, Position */

#define BTPF_HHB_OFFSET  24     /* Shift for 24 bits             */
#define BTPF_HLB_OFFSET  16     /* Shift for 16 bits             */
#define BTPF_LHB_OFFSET  8      /* Shift for 8 bits              */
#define BTPF_LLB_OFFSET  0      /* Shift for 0 bits             */
#define BTPF_WORD        16     /* Shift for 16 bits            */
#define BTPF_BYTE        8      /* Shift for 8 bits             */

#define BTPF_PROG_SEC_SIZE  1024


/*
 *********************************************************************
 * Defines  csflash
 *********************************************************************
 */
#define BTCCFL_SR_LENREQ   15          /* Length of request for validation checksum */
#define BTCCFL_SIZE_CS     4           /* Size of checksum in bytes     */
#define BTCCFL_SR_POSLEN   3           /* Length of positive response   */
#define BTCCFL_START_HHB   3           /* Start address high byte of high word */
#define BTCCFL_START_HLB   4           /* Start address low byte of high word */
#define BTCCFL_START_LHB   5           /* Start address high byte of low word     */
#define BTCCFL_START_LLB   6           /* Start address low byte of low word        */
#define BTCCFL_END_HHB     7           /* End address high byte of high word         */
#define BTCCFL_END_HLB     8           /* End address low byte of high word       */
#define BTCCFL_END_LHB     9           /* End address high byte of low word    */
#define BTCCFL_END_LLB     10          /* End address low byte of low word     */
#define BTCCFL_HHB_CS      11           /* High byte of expected checksum */
#define BTCCFL_HLB_CS      12           /* Low byte of expected checksum */
#define BTCCFL_LHB_CS      13           /* High byte of expected checksum */
#define BTCCFL_LLB_CS      14           /* Low byte of expected checksum */
#define BTCCFL_HHB_OFFSET    24           /* Shift for 24 bits             */
#define BTCCFL_HLB_OFFSET    16           /* Shift for 16 bits             */
#define BTCCFL_LHB_OFFSET    8            /* Shift for 8 bits              */
#define BTCCFL_LLB_OFFSET    0

/* Status for checksum calculation routine */
#define BTCCFL_ST_IDLE     0           /* Status routine idle           */
#define BTCCFL_ST_ACTIVE   1           /* Status routine active         */
#define BTCCFL_ST_CSERROR  2           /* Status routine CS error       */
#define BTCCFL_ST_CSOK     3           /* Status CS ok                  */


#define BTCCFL_CS_BLOCK_SIZE 256       /* Number of bytes to calculate in one cycle */

/*
 *********************************************************************
 * Defines
 *********************************************************************
 */
/* Default timing configuration */
#define  AL_P2MIN_DEF  (0ul)
#define  AL_P2MAX_DEF  (50ul)
#define  AL_P3MIN_DEF  (0ul)
#define  AL_P3MAX_DEF  (3900ul)
#define  AL_S3MAX_DEF  (500000ul)

/* Limits timing configuration */
#define  AL_P2MIN_LIM  (0ul)
#define  AL_P2MAX_LIM  0xFFFFFFFFul
#define  AL_P3MIN_LIM  (0ul)
#define  AL_P3MAX_LIM  0xFFFFFFFFul
#define  AL_S3MAX_LIM  0xFFFFFFFFul
/*
 *********************************************************************
 * Type definition
 *********************************************************************
 */


/*
 *********************************************************************
 * Variables
 *********************************************************************
 */

/* Configurtion of the bit position of the operation mode and active routines
    in the operation mode bitfield */

/* Workaround because .data section is checked by the linker */
/* -> must be empty.                                         */

/*
**********************************************************************
*/
/* Timing configuration normal timings */
BT_alTiming_t  BT_DefaultAlTiming_KWP;
/* Limits of normal timing OBD */
BT_alTiming_t  BT_NormalLimitsAlTiming_KWP;


/**************************************************************************/

//#pragma section ".ramconst" a
/* Service table: */
const BT_ServiceTab_t BT_ServiceTab_cs[] =
{
    /* Tester Present */
    {   /* SID                      */  0x3e,
        /* 1. Parameter             */  BT_IGNOREFIRPARM_DU8,
        /* 2. Adress Mode          */   BT_ADDRMODE_ALL,
        /* Service interpreter      */  BTTp ,
    },

    /* Read Ecu Identification */
    {   /* SID                      */  0x22,
        /* 1. Parameter             */  BT_IGNOREFIRPARM_DU8,
        /* 2. Adress Mode          */  BT_ADDRMODE_ALL,
        /* Service interpreter      */  BTRId,
    },
    /* Write Ecu Identification */
    {   /* SID                      */  0x2E,
        /* 1. Parameter             */  BT_IGNOREFIRPARM_DU8,
        /* 2. Adress Mode          */  BT_ONLY_PHYS,
        /* Service interpreter      */  BTWId,
    },

    /* Start Diagnostic Session */
    {   /* SID                      */  0x10,
        /* 1. Parameter             */  BT_IGNOREFIRPARM_DU8,
        /* 2. Adress Mode          */   BT_ADDRMODE_ALL,
        /* Service interpreter      */  BTDsc,
    },
    /* ECU Reset */
    {   /* SID                      */  0x11,
        /* 1. Parameter             */  BT_IGNOREFIRPARM_DU8,
        /* 2. Adress Mode          */   BT_ADDRMODE_ALL,
        /* Service interpreter      */  BTRset,
    },
    /* Security Access */
    {   /* SID                      */  0x27,
        /* 1. Parameter             */  BT_IGNOREFIRPARM_DU8,
        /* 2. Adress Mode           */  BT_ADDRMODE_ALL,
        /* Service interpreter      */  BTSecA,
    },
    /* Start Routine By Local Id  */
    {   /* SID                      */  0x31,
        /* 1. Parameter             */  0x01,
        /* 2. Adress Mode          */   BT_ONLY_PHYS,
        /* Service interpreter      */  BTStRt,
    },
    /* Request Download */
    {   /* SID                      */  0x34,
        /* 1. Parameter             */  BT_IGNOREFIRPARM_DU8,
        /* 2. Adress Mode          */  BT_ONLY_PHYS,
        /* Service interpreter      */  BTPF_ReqDwnl,
    },
    /* Transfer Data */
    {   /* SID                      */  0x36,
        /* 1. Parameter             */  BT_IGNOREFIRPARM_DU8,
        /* 2. Adress Mode          */   BT_ONLY_PHYS,
        /* Service interpreter      */  BTPF_TransferData,
    },
    /* Request Transfer Exit */
    {   /* SID                      */  0x37,
        /* 1. Parameter             */  BT_IGNOREFIRPARM_DU8,
        /* 2. Adress Mode           */  BT_ONLY_PHYS,
        /* Service interpreter      */  BTPF_TransferExit,
    },
};


uint8  BT_numServices_u8 = sizeof(BT_ServiceTab_cs) / sizeof(BT_ServiceTab_t);

/*******************************************************************************/


/* The complete tx/rx buffer with enougth place for received and     *
 * transmitted header of different length.                           *
 * It is necessary to insert this buffer (pointer) it in the         *
 * configuration structure                                           */
/* DLITE_MAX_DATA_LENGTH + 6 =buffer length                          */
uint8 BT_dataBuffer_au8[BT_BUFFER_LENGTH(BT_MAX_DATA_LENGTH)];

/*
 *********************************************************************
 * used functions
 *********************************************************************
 */




/*
 *********************************************************************
 * Variables
 *********************************************************************
 */
/* Timer definations */
/* only P2min and P3max is monitored */
/* P2 timer */
/* Holds the time of the previous request received */
uint32 BT_toAlRxTim_u32;

/* P3 timer */
/* Holds the time of the previous response received */
uint32 BT_toAlTxTim_u32;

/* S3 timer */
/* Holds the time of Session Timout */
uint32 BT_toS3Tim_u32;

/* Transmit buffer pointer */
static uint8* BT_dAlTxBuf_pu8;

/* Defination of Comstate descriptor table */
BT_AlStateDescriptor_t BT_AlStateDescriptor;

/* Dlite status */
BT_st_t BT_Status;

/* Service Message */
BT_ServiceMsg_t BT_ServiceMsg_s;

/* Pointer to the  working buffer */
uint8* BT_dAlBuf_pu8;

/* number of bytes to transmit */
uint16 BT_numAlTxBytes_u16;

/* Internal variables named with dlite_xxAl  where Al indicates
   the application layer */
/* help Buffer to send wait responses */
uint8 BT_dAlHelpBuf_au8[BT_HELPBUFLEN];

/* Status Register */
uint8 BT_statusAl_u8;

/* Response status */
uint8 BT_stAlSerResp_u8;

/* Wait mode active */
bool BT_stAlWaitmode_b;

/* Response Pending status */
bool BT_stResponsePending_b;

uint32 time_stamp;
/*
 *********************************************************************
 * Variables
 *********************************************************************
 */
uint32 BTRandom_Num_u32;
uint32 BTSECA_SecASeed_u32;
uint8  BTSECA_bSeedRequested_u8;

uint32 UpdatePBL_Pattern_u32;


/*
 *********************************************************************
 * Variables  eflash
 *********************************************************************
 */
uint32 BTEF_adStart_u32;
uint32 BTEF_adSize_u32;

/*
 *********************************************************************
 * Variables pflash
 *********************************************************************
 */
/* Start address of download area */
uint32 BTPF_adStart_u32;
/* Size of download area */
uint32 BTPF_dSize_u32;

/* End of download area */
uint32 BTPF_adEnd_u32;

/* Data identifier */
uint8  BTPF_dfi_u8;

/* BlockSequecne for TransferData */
uint8  BTPF_LastBlkSeq_u8;

uint16 BTPF_TransferSize_u16;
uint16 BTPF_FlashSize_u16;
uint32 Package_Size_u32;
uint8  Dc_RemainBytes_u8;
uint8  BTPF_FlashBuffer[BTPF_PROG_SEC_SIZE << 1];
uint8  BTPF_FlashHeader[BTPF_PROG_SEC_SIZE ];
uint8  BTPF_TransferBuffer_u8[4096];

uint32 BTPF_BytePos_u32;
uint16 BTPF_CopyPos_u16;


uint16 BTPF_dChecksum_u16;

uint8 BTPF_st_u8;

extern uint16 WinPos;
#define BTPF_ST_IDLE            0
#define BTPF_ST_RECEIVE         1
#define BTPF_ST_STARTPROG       2
#define BTPF_ST_PROG            3
#define BTPF_ST_ERROR           6

uint16 BTPF_dChecksum_u16;

r_fcl_request_t  progRequest;


/*
 *********************************************************************
 * Variables  csflash
 *********************************************************************
 */
uint32 BTCCFL_adStart_u32;
uint32 BTCCFL_adSize_u32;
uint32 BTCCFL_adEnd_u32;
uint32 BTCCFL_BytesLeft_u32;

uint32 BTCCFL_dChecksum_u16;
uint32 BTCCFL_dChecksum_u32;
uint32 BTCCFL_dExpectCS_u32;
uint32 oldcrc;

uint8  BTCCFL_st_u8;

uint8_t F110_Integra_SVN[23] = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
uint8_t F111_App_SVN[23] = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
uint8_t F112_Bsw_SVN[23] = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
uint8_t F187_Spare_PN[13] = {'3','8','0','1','0','4','0','-','C','5','1','0','4'};
uint8_t F188_Ecu_SN[13] = {'0','0','0','0','0','0','0','0','0','0','0','0','0'};
uint8_t F189_Ecu_SVN[5] = {'0','0','0','0','0'};
uint8_t F1A0_RT_VN[5] = {'0','0','0','0','0'};
uint8_t F1A1_App_VN[5] = {'0','0','0','0','0'};
uint8_t F1A2_Hardware_Version[5] = {'0','0','0','0','0'};
uint8_t F1A3_CalibData_VN[5] = {'0','0','0','0','0'};
uint8_t F1A4_SysSupplier_ID[5] = {'0','0','0','0','0'};
uint8_t F1A5_SysSupEol_MD[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t F1A6_SysSupEol_MSN[11] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t F190_VIN[17] = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
uint8_t F191_VehMan_Ecu_HN[13] = {'0','0','0','0','0','0','0','0','0','0','0','0','0'};
uint8_t F198_RepairShp_TSN[20] = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
uint8_t F19B_Calibration_Date[4] = {0x00,0x00,0x00,0x00};
uint8_t F19D_Ecu_Ins_Date[4] = {0x00,0x00,0x00,0x00};
uint8_t F15A_Audit_Trail[17] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
const uint8_t F123_Fbl_VN[4] = {0x01,0x22,0x03,0x16};


const uds_rwdata_t Logicdata_list[20] =
{
/* 0*/{0xF110, &F110_Integra_SVN, 				23, 0x3000, UDS_RWDATA_RDONLY, UDS_RWDATA_ROM},
/* 1*/{0xF111, &F111_App_SVN,			  		23, 0x3040, UDS_RWDATA_RDONLY, UDS_RWDATA_ROM},
/* 2*/{0xF112, &F112_Bsw_SVN,	 				23, 0x3080, UDS_RWDATA_RDONLY, UDS_RWDATA_ROM},
/* 3*/{0xF187, &F187_Spare_PN,	  				13, 0x30C0, UDS_RWDATA_RDWR,   UDS_RWDATA_DFLASH},
/* 4*/{0xF188, &F188_Ecu_SN,				  	13, 0x3100, UDS_RWDATA_RDONLY, UDS_RWDATA_ROM},
/* 5*/{0xF189, &F189_Ecu_SVN,				    5,  0x3140, UDS_RWDATA_RDONLY, UDS_RWDATA_ROM},
/* 6*/{0xF1A0, &F1A0_RT_VN, 		 			5,  0x3180, UDS_RWDATA_RDONLY, UDS_RWDATA_ROM},
/* 7*/{0xF1A1, &F1A1_App_VN,		  			5,  0x31C0, UDS_RWDATA_RDONLY, UDS_RWDATA_ROM},
/* 8*/{0xF1A2, &F1A2_Hardware_Version,		 	5,  0x3200, UDS_RWDATA_RDWR,   UDS_RWDATA_DFLASH},
/* 9*/{0xF1A3, &F1A3_CalibData_VN,				5,  0x3240, UDS_RWDATA_RDONLY, UDS_RWDATA_ROM},
/*10*/{0xF1A4, &F1A4_SysSupplier_ID,			5,  0x3280, UDS_RWDATA_RDWR,   UDS_RWDATA_DFLASH},
/*11*/{0xF1A5, &F1A5_SysSupEol_MD,				6,  0x32C0, UDS_RWDATA_RDWR,   UDS_RWDATA_DFLASH},
/*12*/{0xF1A6, &F1A6_SysSupEol_MSN,				11, 0x3300, UDS_RWDATA_RDWR,   UDS_RWDATA_DFLASH},
/*13*/{0xF190, &F190_VIN,				  		17, 0x3340, UDS_RWDATA_RDWR,   UDS_RWDATA_DFLASH},
/*14*/{0xF191, &F191_VehMan_Ecu_HN, 			13, 0x3380, UDS_RWDATA_RDONLY, UDS_RWDATA_ROM},
/*15*/{0xF198, &F198_RepairShp_TSN,				20, 0x33C0, UDS_RWDATA_RDWR,   UDS_RWDATA_DFLASH},
/*16*/{0xF19B, &F19B_Calibration_Date,			4,  0x3400, UDS_RWDATA_RDWR,   UDS_RWDATA_DFLASH},
/*17*/{0xF19D, &F19D_Ecu_Ins_Date, 	  			4,  0x3440, UDS_RWDATA_RDWR,   UDS_RWDATA_DFLASH},
/*18*/{0xF15A, &F15A_Audit_Trail,			    17, 0x3480, UDS_RWDATA_RDONLY, UDS_RWDATA_ROM},
/*19*/{0xF123, &F123_Fbl_VN,			  		4,  0x34C0, UDS_RWDATA_RDONLY, UDS_RWDATA_ROM}

};

#define LOGICDATA_NUM  (sizeof(Logicdata_list) / sizeof(uds_rwdata_t))




static const  uint32 crc32_table[256] =
{
    0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
    0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
    0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
    0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
    0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
    0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
    0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
    0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
    0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
    0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
    0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
    0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
    0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
    0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
    0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
    0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
    0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
    0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
    0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
    0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
    0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
    0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
    0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
    0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
    0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
    0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
    0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
    0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
    0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
    0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
    0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
    0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
    0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
    0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
    0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
    0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
    0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
    0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
    0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
    0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
    0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
    0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
    0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
    0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
    0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
    0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
    0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
    0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
    0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
    0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
    0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
    0x2d02ef8dL
};

static const uint16 Crc16Tab_a[256] =
{
    0x0000u, 0x1021u, 0x2042u, 0x3063u, 0x4084u, 0x50A5u, 0x60C6u, 0x70E7u,
    0x8108u, 0x9129u, 0xA14Au, 0xB16Bu, 0xC18Cu, 0xD1ADu, 0xE1CEu, 0xF1EFu,
    0x1231u, 0x0210u, 0x3273u, 0x2252u, 0x52B5u, 0x4294u, 0x72F7u, 0x62D6u,
    0x9339u, 0x8318u, 0xB37Bu, 0xA35Au, 0xD3BDu, 0xC39Cu, 0xF3FFu, 0xE3DEu,
    0x2462u, 0x3443u, 0x0420u, 0x1401u, 0x64E6u, 0x74C7u, 0x44A4u, 0x5485u,
    0xA56Au, 0xB54Bu, 0x8528u, 0x9509u, 0xE5EEu, 0xF5CFu, 0xC5ACu, 0xD58Du,
    0x3653u, 0x2672u, 0x1611u, 0x0630u, 0x76D7u, 0x66F6u, 0x5695u, 0x46B4u,
    0xB75Bu, 0xA77Au, 0x9719u, 0x8738u, 0xF7DFu, 0xE7FEu, 0xD79Du, 0xC7BCu,
    0x48C4u, 0x58E5u, 0x6886u, 0x78A7u, 0x0840u, 0x1861u, 0x2802u, 0x3823u,
    0xC9CCu, 0xD9EDu, 0xE98Eu, 0xF9AFu, 0x8948u, 0x9969u, 0xA90Au, 0xB92Bu,
    0x5AF5u, 0x4AD4u, 0x7AB7u, 0x6A96u, 0x1A71u, 0x0A50u, 0x3A33u, 0x2A12u,
    0xDBFDu, 0xCBDCu, 0xFBBFu, 0xEB9Eu, 0x9B79u, 0x8B58u, 0xBB3Bu, 0xAB1Au,
    0x6CA6u, 0x7C87u, 0x4CE4u, 0x5CC5u, 0x2C22u, 0x3C03u, 0x0C60u, 0x1C41u,
    0xEDAEu, 0xFD8Fu, 0xCDECu, 0xDDCDu, 0xAD2Au, 0xBD0Bu, 0x8D68u, 0x9D49u,
    0x7E97u, 0x6EB6u, 0x5ED5u, 0x4EF4u, 0x3E13u, 0x2E32u, 0x1E51u, 0x0E70u,
    0xFF9Fu, 0xEFBEu, 0xDFDDu, 0xCFFCu, 0xBF1Bu, 0xAF3Au, 0x9F59u, 0x8F78u,
    0x9188u, 0x81A9u, 0xB1CAu, 0xA1EBu, 0xD10Cu, 0xC12Du, 0xF14Eu, 0xE16Fu,
    0x1080u, 0x00A1u, 0x30C2u, 0x20E3u, 0x5004u, 0x4025u, 0x7046u, 0x6067u,
    0x83B9u, 0x9398u, 0xA3FBu, 0xB3DAu, 0xC33Du, 0xD31Cu, 0xE37Fu, 0xF35Eu,
    0x02B1u, 0x1290u, 0x22F3u, 0x32D2u, 0x4235u, 0x5214u, 0x6277u, 0x7256u,
    0xB5EAu, 0xA5CBu, 0x95A8u, 0x8589u, 0xF56Eu, 0xE54Fu, 0xD52Cu, 0xC50Du,
    0x34E2u, 0x24C3u, 0x14A0u, 0x0481u, 0x7466u, 0x6447u, 0x5424u, 0x4405u,
    0xA7DBu, 0xB7FAu, 0x8799u, 0x97B8u, 0xE75Fu, 0xF77Eu, 0xC71Du, 0xD73Cu,
    0x26D3u, 0x36F2u, 0x0691u, 0x16B0u, 0x6657u, 0x7676u, 0x4615u, 0x5634u,
    0xD94Cu, 0xC96Du, 0xF90Eu, 0xE92Fu, 0x99C8u, 0x89E9u, 0xB98Au, 0xA9ABu,
    0x5844u, 0x4865u, 0x7806u, 0x6827u, 0x18C0u, 0x08E1u, 0x3882u, 0x28A3u,
    0xCB7Du, 0xDB5Cu, 0xEB3Fu, 0xFB1Eu, 0x8BF9u, 0x9BD8u, 0xABBBu, 0xBB9Au,
    0x4A75u, 0x5A54u, 0x6A37u, 0x7A16u, 0x0AF1u, 0x1AD0u, 0x2AB3u, 0x3A92u,
    0xFD2Eu, 0xED0Fu, 0xDD6Cu, 0xCD4Du, 0xBDAAu, 0xAD8Bu, 0x9DE8u, 0x8DC9u,
    0x7C26u, 0x6C07u, 0x5C64u, 0x4C45u, 0x3CA2u, 0x2C83u, 0x1CE0u, 0x0CC1u,
    0xEF1Fu, 0xFF3Eu, 0xCF5Du, 0xDF7Cu, 0xAF9Bu, 0xBFBAu, 0x8FD9u, 0x9FF8u,
    0x6E17u, 0x7E36u, 0x4E55u, 0x5E74u, 0x2E93u, 0x3EB2u, 0x0ED1u, 0x1EF0u
};


/**
 *********************************************************************
 * Initialize the network configuration parameters.
 *
 *
 * Initialize the network configuration parameters.
 *
 *
 *
 * \param
 * \param
 * \return
 * \retval
 * \retval
 * \seealso
 * \usedresources
 *********************************************************************
 */

void BT_AlConfInit(void)
{

    /* Intialise the default timings */
    BT_DefaultAlTiming_KWP[0] = AL_P2MIN_DEF;
    BT_DefaultAlTiming_KWP[1] = AL_P2MAX_DEF;
    BT_DefaultAlTiming_KWP[2] = AL_P3MIN_DEF;
    BT_DefaultAlTiming_KWP[3] = AL_P3MAX_DEF;
    BT_DefaultAlTiming_KWP[4] = AL_S3MAX_DEF;

    /* Intialise the limit timings */
    BT_NormalLimitsAlTiming_KWP[0] = AL_P2MIN_LIM;
    BT_NormalLimitsAlTiming_KWP[1] = AL_P2MAX_LIM;
    BT_NormalLimitsAlTiming_KWP[2] = AL_P3MIN_LIM;
    BT_NormalLimitsAlTiming_KWP[3] = AL_P3MAX_LIM;
    BT_NormalLimitsAlTiming_KWP[4] = AL_S3MAX_LIM;
}

#pragma ghs section text = ".ramcode"

/**
*********************************************************************
* Cyclic process for the application layer of dlite..
*
* This function based on the request service ID, calls the service.
* The response received is analyed and the appropriate response is sent
* to the tester.
* \param
* \param
* \return
* \retval
* \retval
* \seealso
* \usedresources 0x294 (= 660 ) bytes of Code area.
*********************************************************************
*/
void BT_Uds_Proc (void)
{

    switch (BT_statusAl_u8)
    {

    case BT_STAL_IDLE:
        if (BT_AlStateDescriptor_getS3max () < (getCurrentTime - BT_toS3Tim_u32))
        {
        	time_stamp = getCurrentTime;
            BT_SessionTimeout();
        }
        break;

    case BT_STAL_RXINPROG:
        /* The function BT_TpFFInd switches the state from BT_STAL_IDLE to BT_STAL_RXINPROG*/
        /* The function BT_TpInd switches the state from BT_STAL_RXINPROG to BT_STAL_RX */
        break;

    case BT_STAL_TXINPROG:
        /* The function BT_TpConfirm switches the state from BT_STAL_TXINPROG to BT_STAL_IDLE*/
        break;


    case BT_STAL_RX:
    case BT_STAL_RXPEND:
        if (BT_statusAl_u8 == BT_STAL_RX)
        {
            /* The buffer size and the number of bytes received is updated in the indication function */
            BT_ServiceMsg_s.dSID_u8 = BT_dAlBuf_pu8[0];
            BT_ServiceMsg_s.dFirstPara_u8 = BT_dAlBuf_pu8[1];
            BT_ServiceMsg_s.dBuf_pu8 = BT_dAlBuf_pu8 + 1;
            BT_ServiceMsg_s.numBytesReq_u16 = (uint16)(BT_Status_getNumUsed() - 1);
            BT_ServiceMsg_s.dBufSize_u16 =  BT_MAX_DATA_LENGTH;
        }
        else
        {
            BT_ServiceMsg_s.dFirstPara_u8 = BT_dAlBuf_pu8[1] & 0x7F;
        }

        /* Call the service distributor */
        BT_AlServicedist ();

        /* if the response is of type NO_RESPONSE  do nothing switch to idle state */
        if (BT_stAlSerResp_u8 == NO_RESPONSE)
        {
            BT_statusAl_u8 = BT_STAL_IDLE;
            /* Release the buffer */
            BT_TpRelBuf();
            /* Reset the wait pend byte */
            BT_stAlWaitmode_b = FALSE;

            BT_stCallDoChg_u8  = BT_stChgReq_u8;

            break;

        }
        else if (BT_stAlSerResp_u8 == NO_RESPONSE_AGAIN)
        {
            /* Do nothing and switch to response pending state */
            BT_statusAl_u8 = BT_STAL_RXPEND;
            break;
        }
        else if ((BT_stAlSerResp_u8 == RESPONSE_WAITPEND) && (BT_stAlWaitmode_b != (bool)(FALSE)))
        {
            if ( (BT_AlStateDescriptor_getP3max() - BT_MAX_PERIOD) >
                    (getCurrentTime - BT_toAlRxTim_u32))
            {
                /* Time has not elapsed remain in the response pending state */
                BT_statusAl_u8 = BT_STAL_RXPEND;
                break;

            }
            else
            {
                /* P3 max time has elapsed send the response pending frame again*/
                BT_statusAl_u8 = BT_STAL_TX;
                BT_toAlRxTim_u32 = getCurrentTime; /* Reset the timer */
            }

        }
        else
        {
            /* Response has to be sent */
            BT_statusAl_u8 = BT_STAL_WTP2MIN;
            /* Reset the wait pend byte */
            BT_stAlWaitmode_b = FALSE;
            /* No break is inserted in this else statement as when P2min is configured as zero
               the response could be transmitted immediately */

        }
        break;
    /* MISRA RULE 61 VIOLATION: for optimized handling */
    case BT_STAL_WTP2MIN:
    case BT_STAL_TX:

        if (BT_statusAl_u8 == BT_STAL_WTP2MIN)
        {
            if  ( BT_AlStateDescriptor_getP2min() >
                    (getCurrentTime - BT_toAlRxTim_u32))
            {
                /* do nothing and wait for P2min */
                break;
            }
            else
            {
                BT_statusAl_u8 = BT_STAL_TX;
            }
        }
        /* No Break Transmit the frame immediately */
        /* case BT_STAL_TX: */

        if ((BT_TpTxmsg( BT_dAlTxBuf_pu8, BT_numAlTxBytes_u16 )) != (bool)(FALSE))
        {
            /* Implies message has  been transmitted succesfully */
            /* Switch the state to Transmit */
            /* Transmission of single frame response changes the AL state to idle or in the
               case of single frame response pending frame the state is changed to recieve
               Hence the changed state should not be altered */
            /* Al state is changed from TX to TXINPROG only in the case multiple frames */
            if ( BT_statusAl_u8 == BT_STAL_TX)
            {
                BT_statusAl_u8 = BT_STAL_TXINPROG;
            }
            /* WaitPend frame is transmitted */
            if (BT_stAlSerResp_u8 == RESPONSE_WAITPEND)
            {
                BT_stAlWaitmode_b = TRUE;
            }
        }
        else
        {
            /* Transmissio failed retry */
            /* Do not change the state */
            /* Try next time */
        }
        break;



    default:
        break;
    }
}
/**
 *********************************************************************
 * Funtion   Application layer Service distributor.
 *
 * Based on the serviceID,first param , diag mode and security mode the service
 * is called. The response is anlaysed and the appropriate response is sent
 * to the tester. If service is not found then a negative resp with the response
 * code 0x11 is sent to the tester. If a service ID is configured and if it
 * is not allowed to be executed with the current diag mode then a negative reponse
 * is sent without the response code.
 * \param  void
 * \param
 * \return  void
 * \retval
 * \retval
 * \seealso  K2Sess_Distributor
 * \usedresources 0x346 (= 838 ) bytes of Code area.
 *********************************************************************
 */
void BT_AlServicedist (void )
{

    uint8   ctIndex_u8;                          /* Counter for table entries                      */
    bit8   stSearch_b8;                         /* Current status of searching SID in table       */
    bit16  staddressMode_b16;       /* Current mode of ECU                       */
    uint8 addressMode_u8;

    /* Interface variables to the external world name with dlite only */
    /* Service pointer */
    const BT_ServiceTab_t*  BT_ServiceTab_pcs;

    /* Intialise status: */
    stSearch_b8 = BT_SERVICENOTFOUND_DB8;

    /* Get number of table entries: */

    /* Use autovariables vor SID and first parameter and message length: */

    staddressMode_b16 = (bit16)(1 << BT_GetAddrMode());
    /* Counter for table entries: */
    ctIndex_u8  = 0;
    /* Initialise the pointer */
    BT_ServiceTab_pcs = &BT_ServiceTab_cs[0];
    /* Search for matching entry in table: */
    do
    {
        /* Check SID: */
        if (BT_ServiceTab_pcs->dSID_u8 == BT_ServiceMsg_s.dSID_u8)
        {
            /* SID OK --> Check or ignore first parameter: */
            if (( BT_ServiceTab_pcs->dFirstPara_u8 == BT_IGNOREFIRPARM_DU8)
                    ||
                    (   (BT_ServiceMsg_s.numBytesReq_u16 != 0)
                        &&
                        (BT_ServiceTab_pcs->dFirstPara_u8 == (BT_ServiceMsg_s.dFirstPara_u8 & 0x7F))
                    )
               )
            {
                /* Check if service is allowed to be executed in current mode of ECU: */
                if ( (BT_ServiceTab_pcs->dOpMode_b16 & staddressMode_b16)
                        == staddressMode_b16
                   )
                {
                    /* Current message exists in service table --> Set status 'service is supported': */
                    stSearch_b8 = BT_SERVICE_SUPPORTED_DB8;

                    /* Execute service interpreter 'Service_p' with parameters          */
                    /* '&BT_ServiceMsg_s' (Pointer to information about currently received */
                    /*                  message)                                        */
                    (BT_ServiceTab_pcs->Service_p)
                    ( &BT_ServiceMsg_s );
                }
                else
                {
                    stSearch_b8 = BT_ADDRMODE_NOK_DB8;
                }
                /* Stop searching: */
                ctIndex_u8 = BT_numServices_u8;
            }
            else
            {
                /* Service exists, but FirstPara not match */
                stSearch_b8 = BT_SERVICE_SUBFUNCNOTSUPPORTED_DB8;
            }
        }
        /* Next entry: */
        ctIndex_u8++;
        BT_ServiceTab_pcs++;
    }
    while (ctIndex_u8 < BT_numServices_u8);
    /* Evaluate exit status of search loop: */
    if ( stSearch_b8 != BT_SERVICE_SUPPORTED_DB8)
    {
        /* Service has not been called */
        if (BT_stAlSerResp_u8 == RESPONSE_WAITBUSYREPEAT)
        {
            /* Handshake mode active --> remain in mode until service is available again: */
            BT_ServiceMsg_s.stResp_u8 = BT_WAITHS;
        }
        else
        {
            /* Check why service has not been called and send corresponding response: */
            BT_stAlSerResp_u8 = RESPONSE_OK;

            /* Intialise the transmit buffer to send the negative response */
            BT_dAlTxBuf_pu8 = BT_ServiceMsg_s.dBuf_pu8;

            /* Negative response SID */
            BT_dAlTxBuf_pu8--;
            *BT_dAlTxBuf_pu8 = BT_ServiceMsg_s.dSID_u8;

            /* Negative response code */
            BT_dAlTxBuf_pu8--;
            *BT_dAlTxBuf_pu8 = NEG_RESPONSE_CODE;
            /* number of bytes to be tranmsitted */
            BT_numAlTxBytes_u16 = 3;
            BT_ServiceMsg_s.stResp_u8 = BT_NIL;

            switch (stSearch_b8)
            {
            case BT_SERVICENOTFOUND_DB8:
            case BT_ADDRMODE_NOK_DB8:
            default:
            {
                /* SID doesn't exist in service table --> send corresponding response: */
                BT_ServiceMsg_s.dBuf_pu8[0] =  SERVICE_NOT_SUPPORTED_CODE;

                break;
            }

            case BT_SERVICE_SUPPORTED_DB8:
            {
                /* Service exists but its execution is not allowed in current operation */
                /* mode  --> send corresponding response:                               */
                BT_ServiceMsg_s.dBuf_pu8[0] =  SERVICE_NOT_SUPPORTED_INACTIVESESSION;
                break;
            }

            case BT_SERVICE_SUBFUNCNOTSUPPORTED_DB8:
            {
                /* Service exists, but FirstPare-SubFunc not supported */
                /* send corresponding response: */
                BT_ServiceMsg_s.dBuf_pu8[0] =  SUBFUNC_NOT_SUPPORTED_CODE;
                break;
            }

            }
        }
    }
    /********************************************************************************/
    /* Handling of Address mode *****************************************************/
    /********************************************************************************/
    addressMode_u8 = (uint8)BT_GetAddrMode();
    if (addressMode_u8 == ADRM(FUNC))
    {
        /* Is the response negative ? */
        if ((BT_ServiceMsg_s.stResp_u8 == BT_NEG) || (BT_ServiceMsg_s.stResp_u8 ==
                BT_NIL )
           )
        {
            /* No negative response if address mode is OBDONCAN and FUNC for the
            following negative response code's*/
            if ((BT_ServiceMsg_s.dBuf_pu8[0] == 0x10) ||     /* Neg Resp Code 0x10 */
                    (BT_ServiceMsg_s.dBuf_pu8[0] == 0x11)  ||    /* Neg Resp Code 0x11 */
                    (BT_ServiceMsg_s.dBuf_pu8[0] == 0x12)  ||    /* Neg Resp Code 0x12 */
                    (BT_ServiceMsg_s.dBuf_pu8[0] == 0x31))       /* Neg Resp Code 0x31 */
            {
                BT_ServiceMsg_s.stResp_u8 = BT_NORESP;
            }
        }
    }
    /* Service is called */
    switch (BT_ServiceMsg_s.stResp_u8)
    {
    case BT_POS :
        /* response SID */
        BT_dAlTxBuf_pu8 = BT_ServiceMsg_s.dBuf_pu8 - 1;

        *BT_dAlTxBuf_pu8 =  (uint8)(BT_ServiceMsg_s.dSID_u8 + POS_RESPONSE_ADDER);
        BT_numAlTxBytes_u16 = BT_ServiceMsg_s.numBytesResp_u16 + 1;
        BT_stAlSerResp_u8 = RESPONSE_OK;

        break;
    /********************************************************/
    case BT_NEG :

        /* Intialise the transmit buffer to send the negative response */
        BT_dAlTxBuf_pu8 = BT_ServiceMsg_s.dBuf_pu8;

        /* Negative response SID */
        BT_dAlTxBuf_pu8--;
        *BT_dAlTxBuf_pu8 = BT_ServiceMsg_s.dSID_u8;

        /* Negative response code */
        BT_dAlTxBuf_pu8--;
        *BT_dAlTxBuf_pu8 = NEG_RESPONSE_CODE;

        /* Transmit length */
        BT_numAlTxBytes_u16 = BT_ServiceMsg_s.numBytesResp_u16 + 2;
        BT_stAlSerResp_u8 = RESPONSE_OK;
        break;

    /******************************************************/
    /* Waiths implies send busy repeat request(0x21) ******/
    /******************************************************/
    case BT_WAITHS   :
        /* Use the help buffer as the main buffer is still used by the service */
        BT_dAlHelpBuf_au8[0] = NEG_RESPONSE_CODE;
        BT_dAlHelpBuf_au8[1] = BT_ServiceMsg_s.dSID_u8;
        /* Negative response CODE */
        BT_dAlHelpBuf_au8[2] = BUSY_REPEAT_CODE;
        BT_stAlSerResp_u8 =  RESPONSE_WAITBUSYREPEAT;
        /* number of bytes to be transmitted */
        BT_numAlTxBytes_u16 = 0x03;
        /* Transmit buffer pointer */
        BT_dAlTxBuf_pu8 = BT_dAlHelpBuf_au8;

        break;
    /*response pending*/
    case BT_RPEND:
        /* Use the help buffer as the main buffer is still used by the service */
        BT_dAlHelpBuf_au8[0] = NEG_RESPONSE_CODE;
        BT_dAlHelpBuf_au8[1] = BT_ServiceMsg_s.dSID_u8;
        /* Negative response CODE */
        BT_dAlHelpBuf_au8[2] = WAIT_PEND_CODE;
        BT_stAlSerResp_u8 =  RESPONSE_WAITPEND;
        /* number of bytes to be transmitted */
        BT_numAlTxBytes_u16 = 0x03;
        /* Transmit buffer pointer */
        BT_dAlTxBuf_pu8 = BT_dAlHelpBuf_au8;
        break;

    /* suppressed positive response */
    case BT_SUPRPOS:
    case BT_NORESP:
        /* Reset the response pending type */
        BT_stAlSerResp_u8 = NO_RESPONSE;

        /* Reset the number of bytes to transmit */
        BT_numAlTxBytes_u16 = 0;
        BT_TxSt = 1;
        break;
    /* unknown value returned from the service */
    default :

        break;
    }
}


/**
 *********************************************************************
 *  BTDsc(): Service Diagnostic Session control
 *
 *  DiagnosticSessionControl service for init the diagnostic
 *  session for RB flash programming.
 * \param NOT_Used  Pointer to configuration (not used).
 * \param Msg_ps Pointer to kwp buffer.
 * \return none
 * \retval none
 * \seealso
 * \usedresources
 *********************************************************************
 */
void BTDsc(BT_ServiceMsg_t* Msg_ps)
{
    uint8 dNegRespCode = 0;
    uint8 dSupPosResp;

    if (Msg_ps->numBytesReq_u16 != 1)
    {
        dNegRespCode = BTPROG_IMLOIF;
        /* MISRA RULE 56 VIOLATION: Goto in error handling in communication makes sw efficient */
        goto STDS_ERROR;
    }

    dSupPosResp = Msg_ps->dFirstPara_u8 & 0x80;

    if ((Msg_ps->dBuf_pu8[0] & 0x7F) == BT_EXTEND_SESSION_DU8)
    {
        if ((BT_AlStateDescriptor_getDiagMode () == BT_EXTEND_SESSION_DU8)
                || (BT_AlStateDescriptor_getDiagMode () == BT_DEFAULT_SESSION_DU8))
        {
            BT_AlStateDescriptor_setSecLevel (BT_DEFAULT_SECLEV_DU8);
            BT_AlStateDescriptor_setDiagMode (BT_EXTEND_SESSION_DU8);

            if (!dSupPosResp)
            {
                Msg_ps->numBytesResp_u16 = 5;
                Msg_ps->dBuf_pu8[0] = Msg_ps->dBuf_pu8[0] & 0x7F;
                Msg_ps->dBuf_pu8[1] = (uint8)((BTDSC_P2MAX >> 8) & 0x0FF);
                Msg_ps->dBuf_pu8[2] = (uint8)(BTDSC_P2MAX & 0x0FF);
                Msg_ps->dBuf_pu8[3] = (uint8)((BTDSC_P2EXTMAX_DIV10 >> 8) & 0x0FF);
                Msg_ps->dBuf_pu8[4] = (uint8)(BTDSC_P2EXTMAX_DIV10 & 0x0FF);
                Msg_ps->stResp_u8 = BT_POS;
            }
            else
            {
                Msg_ps->stResp_u8 = BT_SUPRPOS;
            }
        }
        else
        {
            dNegRespCode = BTPROG_SUBFCTSUPPSESS;
            /* MISRA RULE 56 VIOLATION: Goto in error handling in communication makes sw efficient */
            goto STDS_ERROR;
        }
    }
    else if ((Msg_ps->dBuf_pu8[0] & 0x7F) == BT_PROGRAM_SESSION_DU8 ) //BT_TC_PROG_SESSION_DU8
    {
        if (!dSupPosResp)
        {
            Msg_ps->numBytesResp_u16 = 5;
            Msg_ps->dBuf_pu8[0] = Msg_ps->dBuf_pu8[0] & 0x7F;
            Msg_ps->dBuf_pu8[1] = (uint8)((BTDSC_P2MAX >> 8) & 0x0FF);
            Msg_ps->dBuf_pu8[2] = (uint8)(BTDSC_P2MAX & 0x0FF);
            Msg_ps->dBuf_pu8[3] = (uint8)((BTDSC_P2EXTMAX_DIV10 >> 8) & 0x0FF);
            Msg_ps->dBuf_pu8[4] = (uint8)(BTDSC_P2EXTMAX_DIV10 & 0x0FF);
            Msg_ps->stResp_u8 = BT_POS;
        }
        else
        {
            Msg_ps->stResp_u8 = BT_SUPRPOS;
        }

        /* set current as program session */
        BT_AlStateDescriptor_setDiagMode (BT_PROGRAM_SESSION_DU8 ); //BT_TC_PROG_SESSION_DU8

    }
    else if ((Msg_ps->dBuf_pu8[0] & 0x7F) == BT_DEFAULT_SESSION_DU8)
    {
        BT_AlStateDescriptor_setSecLevel (BT_DEFAULT_SECLEV_DU8);

        if ((BT_AlStateDescriptor_getDiagMode () == BT_EXTEND_SESSION_DU8)
                || (BT_AlStateDescriptor_getDiagMode () == BT_DEFAULT_SESSION_DU8))
        {
            BT_AlStateDescriptor_setDiagMode (BT_DEFAULT_SESSION_DU8);

            if (!dSupPosResp)
            {
                Msg_ps->numBytesResp_u16 = 5;
                Msg_ps->dBuf_pu8[0] = Msg_ps->dBuf_pu8[0] & 0x7F;
                Msg_ps->dBuf_pu8[1] = (uint8)((BTDSC_P2MAX >> 8) & 0x0FF);
                Msg_ps->dBuf_pu8[2] = (uint8)(BTDSC_P2MAX & 0x0FF);
                Msg_ps->dBuf_pu8[3] = (uint8)((BTDSC_P2EXTMAX_DIV10 >> 8) & 0x0FF);
                Msg_ps->dBuf_pu8[4] = (uint8)(BTDSC_P2EXTMAX_DIV10 & 0x0FF);
                Msg_ps->stResp_u8 = BT_POS;
            }
            else
            {
                Msg_ps->stResp_u8 = BT_SUPRPOS;
            }
        }
        else
        {
            BT_AlStateDescriptor_setDiagMode (BT_DEFAULT_SESSION_DU8);

            if (!dSupPosResp)
            {
                Msg_ps->numBytesResp_u16 = 5;
                Msg_ps->dBuf_pu8[0] = Msg_ps->dBuf_pu8[0] & 0x7F;
                Msg_ps->dBuf_pu8[1] = (uint8)((BTDSC_P2MAX >> 8) & 0x0FF);
                Msg_ps->dBuf_pu8[2] = (uint8)(BTDSC_P2MAX & 0x0FF);
                Msg_ps->dBuf_pu8[3] = (uint8)((BTDSC_P2EXTMAX_DIV10 >> 8) & 0x0FF);
                Msg_ps->dBuf_pu8[4] = (uint8)(BTDSC_P2EXTMAX_DIV10 & 0x0FF);
                Msg_ps->stResp_u8 = BT_POS;
            }
            else
            {
                Msg_ps->stResp_u8 = BT_SUPRPOS;
            }
            BT_ExecFunction (BTRset_ECU);

        }
    }
    else
    {
        dNegRespCode = BTPROG_SUBFCTSUPP;
        /* MISRA RULE 56 VIOLATION: Goto in error handling in communication makes sw efficient */
        goto STDS_ERROR;
    }

    return;

    /* MISRA RULE 55 VIOLATION: Goto in error handling in communication makes sw efficient */
STDS_ERROR:
    Msg_ps->dBuf_pu8[0] = dNegRespCode;
    Msg_ps->numBytesResp_u16 = BTPROG_NR_LEN;
    Msg_ps->stResp_u8 = BT_NEG;
    /* MISRA RULE 82 VIOLATION: Multiple returns in error handling in communication makes sw efficient */
    return;
}

/**
 *********************************************************************
 *  BTStRt(): Service Routine Control - Start Routine.
 *
 *  Routine control service for UDS.
 * \param NOT_Used  Pointer to configuration (not used).
 * \param Msg_ps Pointer to kwp buffer.
 * \return none
 * \retval none
 * \seealso
 * \usedresources
 *********************************************************************
 */
void BTStRt ( BT_ServiceMsg_t* Msg_ps )
{
    uint8 dNegRespCode = 0;
    uint16 xRoutineId = 0;

    uint32 dDestPtr;
    uint32 dRecsBLAddr;

    if (Msg_ps->numBytesReq_u16 >= 3)
    {
        xRoutineId = (uint16)Msg_ps->dBuf_pu8[1] << 8 | Msg_ps->dBuf_pu8[2];

        switch (xRoutineId)
        {
        case RID_ERASEFLASH:
            if (BT_AlStateDescriptor_getSecLevel () != BT_RBPROG_SECLEV_DU8)
            {
                dNegRespCode = BTPROG_ACCDENIED;
                /* MISRA RULE 56 VIOLATION: Goto in error handling in communication makes sw efficient */
                goto BTSTRT_ERROR;
            }
            else
            {
                BTEF_Start (Msg_ps);
            }
            break;

        case RID_CPDC:
            if (BT_AlStateDescriptor_getSecLevel () != BT_RBPROG_SECLEV_DU8)
            {
                dNegRespCode = BTPROG_ACCDENIED;
                /* MISRA RULE 56 VIOLATION: Goto in error handling in communication makes sw efficient */
                goto BTSTRT_ERROR;
            }
            else
            {
                BTCCFL_Start (Msg_ps);
            }
            break;

        case RID_CHKVALIDAPP:
            if (Msg_ps->numBytesReq_u16 != 3)
            {
                dNegRespCode = BTPROG_IMLOIF;
                goto BTSTRT_ERROR;
            }
            else
            {
                if (BT_AlStateDescriptor_getSecLevel () != BT_RBPROG_SECLEV_DU8)
                {
                    dNegRespCode = BTPROG_ACCDENIED;
                    /* MISRA RULE 56 VIOLATION: Goto in error handling in communication makes sw efficient */
                    goto BTSTRT_ERROR;
                }
                else
                {
                    {

                        Msg_ps->dBuf_pu8[3] = 0x10;

                        //if (Prog_CCR() == FALSE)
			if(0)
                        {
                            Msg_ps->dBuf_pu8[4] = 0x01;
                        }
                        else
                        {
                            Msg_ps->dBuf_pu8[4] = 0x02;
                        }

                        Msg_ps->numBytesResp_u16 = 5;
                        Msg_ps->stResp_u8 = BT_POS;
                    }
                }
            }
            break;

        case RID_UPDATEPBL:
            if (BT_AlStateDescriptor_getSecLevel () != BT_RBPROG_SECLEV_DU8)
            {
                dNegRespCode = BTPROG_ACCDENIED;
                /* MISRA RULE 56 VIOLATION: Goto in error handling in communication makes sw efficient */
                goto BTSTRT_ERROR;
            }
            else
            {
		UpdatePBL_Pattern_u32 = 0xEADC7913;
                Msg_ps->dBuf_pu8[3] = 0x10;
                Msg_ps->numBytesResp_u16 = 4;
                Msg_ps->stResp_u8 = BT_POS;
   
            }
            break;

        default:
            dNegRespCode = BTPROG_SUBFCTSUPP;
            goto BTSTRT_ERROR;
            break;
        }

    }
    else
    {
        dNegRespCode = BTPROG_IMLOIF;
        goto BTSTRT_ERROR;
    }

    return;

BTSTRT_ERROR:
    Msg_ps->dBuf_pu8[0] = dNegRespCode;
    Msg_ps->numBytesResp_u16 = BTPROG_NR_LEN;
    Msg_ps->stResp_u8 = BT_NEG;
    return;
}
/**
 *********************************************************************
 *  BTRId(): Service Read Ecu Identification
 *
 *  ReadDataByIdentifier service for init the diagnostic
 *  session for RB flash programming.
 * \param NOT_Used  Pointer to configuration (not used).
 * \param Msg_ps Pointer to kwp buffer.
 * \return none
 * \retval none
 * \seealso
 * \usedresources
 *********************************************************************
 */
void BTRId(BT_ServiceMsg_t* Msg_ps)
{
    uint8 dNegRespCode;
    uint16 dLIdOption_u16;
    uint32 Cnt_u32;
    uint32 dCount_u32;
    uint8* dData_pu8;
	
	uint8_t rsp_buf[255];
		uint16_t did;
		uint16_t rsp_len;
		uint32_t readbuffer = 0;
		uint16_t msg_pos, did_n;
		bool find_did;
		uint8_t word_cnt = 0x00u;
		uint8_t byte_left = 0x00u;
		uint8_t word_num = 0x00u;
		uint8_t byte_cnt = 0x00u;
		uint32_t eeprom_address = 0;

    if (Msg_ps->numBytesReq_u16 != 2)
    {
        dNegRespCode = BTPROG_IMLOIF;
        /* MISRA RULE 56 VIOLATION: Goto in error handling in communication makes sw efficient */
        goto BTRID_ERROR;
    }
	else
	{
		
		
		
		
	
		//rsp_buf[0] = USD_GET_POSITIVE_RSP(SID_22);
		rsp_len = 2;
		
			did = ((uint16_t)Msg_ps->dBuf_pu8[0]) << 8;
			did |= Msg_ps->dBuf_pu8[1];
	
			find_did = FALSE;
			
			if((did >= 0xF110)&&(did <= 0xF1A6)&&(0xF189 != did))
			{
				for (did_n = 0; did_n < LOGICDATA_NUM; did_n++)
				{
				   if (Logicdata_list[did_n].did == did)
					{
						find_did = TRUE;
						//rsp_buf[rsp_len++] = Msg_ps->dBuf_pu8[0];
						//rsp_buf[rsp_len++] = Msg_ps->dBuf_pu8[1];
						//memcpy (&rsp_buf[rsp_len], rwdata_list[did_n].p_data, rwdata_list[did_n].dlc);
						#if 0
						if(UDS_RWDATA_RDONLY == Logicdata_list[did_n].rw_mode)
						{
							memcpy (&rsp_buf[rsp_len], Logicdata_list[did_n].p_data, Logicdata_list[did_n].dlc);
							rsp_len += Logicdata_list[did_n].dlc;
						}
						else 
							#endif
						if((UDS_RWDATA_RDWR == Logicdata_list[did_n].rw_mode) ||(UDS_RWDATA_RDONLY == Logicdata_list[did_n].rw_mode))
						{
							eeprom_address = Logicdata_list[did_n].ee_address;
						#if 0
							readbuffer = EEROM_ReadWord(Logicdata_list[did_n].ee_address);
							if(Logicdata_list[did_n].dlc == 1)
								rsp_buf[rsp_len++] = (uint8_t)(readbuffer);
							else if(Logicdata_list[did_n].dlc == 2)
							{
								rsp_buf[rsp_len++] = (uint8_t)(readbuffer);
								rsp_buf[rsp_len++] = (uint8_t)(readbuffer>>8);
							}
							else if(Logicdata_list[did_n].dlc == 4)
							{
								rsp_buf[rsp_len++] = (uint8_t)(readbuffer);
								rsp_buf[rsp_len++] = (uint8_t)(readbuffer>>8);
								rsp_buf[rsp_len++] = (uint8_t)(readbuffer>>16);
								rsp_buf[rsp_len++] = (uint8_t)(readbuffer>>24);
							}
							else;
						#endif
							word_num  = Logicdata_list[did_n].dlc/4;
							byte_left = Logicdata_list[did_n].dlc%4;
							//if(0 !=byte_left)
							{
								word_num++;
							}
	
							for(word_cnt = 0x00u;word_cnt<word_num;word_cnt++)
							{
								readbuffer = EEROM_ReadWord(eeprom_address);
								if((word_num-1)== word_cnt)
								{
									for(byte_cnt = 0x00u;byte_cnt<byte_left;byte_cnt++)
									{
										Msg_ps->dBuf_pu8[rsp_len++] = (uint8_t)(readbuffer>>(8*byte_cnt));
									}
								}
								else
								{
									Msg_ps->dBuf_pu8[rsp_len++] = (uint8_t)(readbuffer);
									Msg_ps->dBuf_pu8[rsp_len++] = (uint8_t)(readbuffer>>8);
									Msg_ps->dBuf_pu8[rsp_len++] = (uint8_t)(readbuffer>>16);
									Msg_ps->dBuf_pu8[rsp_len++] = (uint8_t)(readbuffer>>24);
								}
								eeprom_address +=4;
							}
	
							
						//rsp_len += rwdata_list[did_n].dlc;
						}
						else
						{
						}
						break;
					}
				}
			}
			if (find_did == FALSE)
			{;}
				//break;
		
	
		if (find_did == TRUE)
		{

            Msg_ps->numBytesResp_u16 = rsp_len;
            Msg_ps->stResp_u8 = BT_POS;
		}
		else
		{

		 	Msg_ps->dBuf_pu8[0] = BTPROG_RCROOR;
            Msg_ps->numBytesResp_u16 = BTPROG_NR_LEN;
            Msg_ps->stResp_u8 = BT_NEG;
		}
		
	
	}

#if 0

    /* Get identification option */
    dLIdOption_u16 = ((uint16)Msg_ps->dBuf_pu8[0] << 8) +
                     Msg_ps->dBuf_pu8[1];

    switch (dLIdOption_u16)
    {
        case 0x2150:
        case 0xF186:
        {
            Msg_ps->dBuf_pu8[2] = BT_AlStateDescriptor_getDiagMode();
            Msg_ps->numBytesResp_u16 = 3;
            Msg_ps->stResp_u8 = BT_POS;
        }
        break;

        case 0x2151:
        {
            if (ProgRequest)
            {
                Msg_ps->dBuf_pu8[2] = 0x01;
            }
            else
            {
                Msg_ps->dBuf_pu8[2] = 0x00;
            }

            Msg_ps->numBytesResp_u16 = 3;
            Msg_ps->stResp_u8 = BT_POS;
        }
        break;

        case 0x2152:
        {
            Msg_ps->dBuf_pu8[2] = 0x00;
            Msg_ps->numBytesResp_u16 = 3;
            Msg_ps->stResp_u8 = BT_POS;
        }
        break;

        case 0xF18A:
        {
            Msg_ps->dBuf_pu8[2] = 'H';
            Msg_ps->dBuf_pu8[3] = 'E';
            Msg_ps->dBuf_pu8[4] = 'L';
            Msg_ps->dBuf_pu8[5] = 'L';
            Msg_ps->dBuf_pu8[6] = 'O';
            Msg_ps->numBytesResp_u16 = 7;
            Msg_ps->stResp_u8 = BT_POS;
        }
        break;
        /* Other LID's */
        default:
        {
            Msg_ps->dBuf_pu8[0] = BTPROG_RCROOR;
            Msg_ps->numBytesResp_u16 = BTPROG_NR_LEN;
            Msg_ps->stResp_u8 = BT_NEG;

        }
    }
	#endif
    return;

    /* MISRA RULE 55 VIOLATION: Goto in error handling in communication makes sw efficient */
BTRID_ERROR:
    Msg_ps->dBuf_pu8[0] = dNegRespCode;
    Msg_ps->numBytesResp_u16 = BTPROG_NR_LEN;
    Msg_ps->stResp_u8 = BT_NEG;
    /* MISRA RULE 82 VIOLATION: Multiple returns in error handling in communication makes sw efficient */
    return;
}

/**
 *********************************************************************
 *  BTWId(): Service Write Ecu Identification
 *
 *  WriteDataByIdentifier service for init the diagnostic
 *  session for RB flash programming.
 * \param NOT_Used  Pointer to configuration (not used).
 * \param Msg_ps Pointer to kwp buffer.
 * \return none
 * \retval none
 * \seealso
 * \usedresources
 *********************************************************************
 */
void BTWId(BT_ServiceMsg_t* Msg_ps)
{
    uint16 dLIdOption_u16;
    uint8  dNegRespCode;

    uint32 Cnt_u32;

	uint8_t rsp_buf[8];
		uint16_t did;
		uint16_t did_n;
		int16_t write_result;
		uint32_t writebuffer = 0;
		bool find_did;
		bool vali_dlc;
		uint8_t word_cnt = 0x00u;
		uint8_t byte_left = 0x00u;
		uint8_t word_num = 0x00u;
		uint8_t byte_cnt = 0x00u;
		uint32_t eeprom_address = 0;
		uint8_t byte_offset = 0x00u;
		

    if (Msg_ps->numBytesReq_u16 < 2)
    {
        dNegRespCode = BTPROG_IMLOIF;
        /* MISRA RULE 56 VIOLATION: Goto in error handling in communication makes sw efficient */
        goto BTWID_ERROR;
    }
#if 1
    if (BT_AlStateDescriptor_getSecLevel () != BT_RBPROG_SECLEV_DU8)
    {
        dNegRespCode = BTPROG_ACCDENIED;
        /* MISRA RULE 56 VIOLATION: Goto in error handling in communication makes sw efficient */
        goto BTWID_ERROR;
    }
    #endif

	
		did = ((uint16_t)Msg_ps->dBuf_pu8[0]) << 8;
		did |= Msg_ps->dBuf_pu8[1];
	
		find_did = FALSE;
		vali_dlc = FALSE;
		
		if((did >= 0xF110)&&(did <= 0xF1A6))
		{
			for (did_n = 0; did_n < LOGICDATA_NUM; did_n++)
			{
				if (Logicdata_list[did_n].did == did)
				{
					if (Logicdata_list[did_n].rw_mode == UDS_RWDATA_RDONLY)
					break;
					find_did = TRUE;
					if (Msg_ps->numBytesReq_u16 != (Logicdata_list[did_n].dlc + 2))
					break;
					vali_dlc = TRUE;
					
					bt_memcpy (Logicdata_list[did_n].p_data, &Msg_ps->dBuf_pu8[2], Logicdata_list[did_n].dlc);
					write_result = Diagnostic_EEProm_Write(Logicdata_list[did_n].ee_address, Logicdata_list[did_n].dlc, Logicdata_list[did_n].p_data);
	//在2e里面写   写入数据是正常的数据，写进去eeprom的排列是低位在前高位在后，intel格式，读出来时候需要转化
	
#if 0
					word_num  = Logicdata_list[did_n].dlc/4;
					byte_left = Logicdata_list[did_n].dlc%4;
					if(0 !=byte_left)
					{
						word_num++;
					}
	
					for(word_cnt = 0x00u;word_cnt<word_num;word_cnt++)
					{
						byte_offset = word_cnt*4;
						readbuffer = EEROM_ReadWord(eeprom_address);
						if((word_num-1)== word_cnt)
						{
							for(byte_cnt = 0x00u;byte_cnt<byte_left;byte_cnt++)
							{
								rsp_buf[rsp_len++] = (uint8_t)(readbuffer>>(8*byte_cnt));
							}
						}
						else
						{
							writebuffer = msg_buf[3+byte_offset] + (msg_buf[4+byte_offset]<<8) + (msg_buf[5+byte_offset]<<16) + (msg_buf[6+byte_offset]<<24);
						}
						write_result = EEROM_WriteEOL(eeprom_address,writebuffer);
						eeprom_address +=4;
					}
				#endif
	
					break;
				}
			}
		}
		
		if (find_did == TRUE)
		{
			if (vali_dlc == TRUE)
			{
				if (write_result > 0)
				{
        			Msg_ps->numBytesResp_u16 = 2;
        			Msg_ps->stResp_u8 = BT_POS;
				}
				else
				{
					Msg_ps->dBuf_pu8[0] = BTPROG_GPF;
			        Msg_ps->numBytesResp_u16 = BTPROG_NR_LEN;
			        Msg_ps->stResp_u8 = BT_NEG;
				}
			}
			else
			{
				Msg_ps->dBuf_pu8[0] = BTPROG_IMLOIF;
		        Msg_ps->numBytesResp_u16 = BTPROG_NR_LEN;
		        Msg_ps->stResp_u8 = BT_NEG;
			}
		}
		else
		{
			Msg_ps->dBuf_pu8[0] = BTPROG_RCROOR;
	        Msg_ps->numBytesResp_u16 = BTPROG_NR_LEN;
	        Msg_ps->stResp_u8 = BT_NEG;
		}
	
	



    
    return;
    /* MISRA RULE 55 VIOLATION: Goto in error handling in communication makes sw efficient */
BTWID_ERROR:
    Msg_ps->dBuf_pu8[0] = dNegRespCode;
    Msg_ps->numBytesResp_u16 = BTPROG_NR_LEN;
    Msg_ps->stResp_u8 = BT_NEG;
    /* MISRA RULE 82 VIOLATION: Multiple returns in error handling in communication makes sw efficient */
    return;
}
/**
 *********************************************************************
 *  SBRset_ECU(): Reset ECU
 *
 *  Reset the ECU on request for KWP2000.
 * \param none
 * \return none
 * \retval none
 * \seealso
 * \usedresources
 *********************************************************************
 */
void BTRset_ECU(void)
{
    /* Workaround to call flash function from spram, so compiler has to use CALLI instruction */
  	R_DEV_WRITE_PROT_REG(PROTCMD0, 0xFFF80A04, 0x01u);  /*SWRESA*/

}

void BasUtil_EcuRBProgReset(void)
{

    /* simulate a RB -programming reset */
    R_DEV_WRITE_PROT_REG(PROTCMD0, 0xFFF80A04, 0x01u);	/*SWRESA*/

    return;
}

/**
 *********************************************************************
 *  SB_Rset(): Service StartDiagnosticSession
 *
 *  ECU Reset: service for reset the ECU, only the resetMode
 *  parameter powerOn (0x01) is supported.
 * \param NOT_Used  Pointer to configuration (not used).
 * \param Msg_ps Pointer to kwp buffer.
 * \return none
 * \retval none
 * \seealso
 * \usedresources
 *********************************************************************
 */
void BTRset(BT_ServiceMsg_t* Msg_ps)
{
    uint8 dNegRespCode = 0;
    uint8 dSupPosResp;
    uint8 dResetType;

    if (Msg_ps->numBytesReq_u16 == 1)
    {
        dSupPosResp = Msg_ps->dFirstPara_u8 & 0x80;
        dResetType = Msg_ps->dFirstPara_u8 & 0x7F;

        switch (dResetType)
        {
        case 0x01:
            BT_ExecFunction(BTRset_ECU);
            break;
        case 0x60:
            BT_ExecFunction(BasUtil_EcuRBProgReset);
            break;
        default:
            dNegRespCode = BTPROG_SUBFCTSUPP;
            break;

        }
    }
    else
    {
        dNegRespCode = BTPROG_IMLOIF;
    }

    if (!dNegRespCode)
    {
        if (!dSupPosResp)
        {
            Msg_ps->numBytesResp_u16 = 1;
            Msg_ps->stResp_u8 = BT_POS;
        }
        else
        {
            Msg_ps->stResp_u8 = BT_SUPRPOS;
        }

    }
    else
    {
        Msg_ps->dBuf_pu8[0] = dNegRespCode;
        Msg_ps->numBytesResp_u16 = BTPROG_NR_LEN;
        Msg_ps->stResp_u8 = BT_NEG;
    }

    return;

}

/**
 *********************************************************************
 *  Geely_Calc_Key_FP():
 *********************************************************************
 */
void Calc_Key_FP(uint32 seed, uint32* key)
{
    uint8 i;
    uint8 Cal[4];
    uint8 Sd[16];
    uint32 tmpKey;


    Sd[0] = (uint8)((seed >> 24) & 0xFF);
    Sd[1] = (uint8)((seed >> 16) & 0xFF);
    Sd[2] = (uint8)((seed >> 8) & 0xFF);
    Sd[3] = (uint8)((seed) & 0xFF);
	Sd[4] = 0x00;
	Sd[5] = 0xF1;
	for(i=6;i<16;i++)
	{
		Sd[i] = 0x00;
	}	
    tmpKey = 0;

    unsigned long v0 = seed;
	unsigned long v1 = 0x00F10000;
	unsigned long sum=0,delta=0x9e3779b9;
	int MaskKey[4] = {0x17026870,0x67868467,0x59594A43,0X544A5802}; 
	for( i=0;i<2;i++)
	{
		v0+=(((v1<<4)^(v1>>5))+v1)^(sum+MaskKey[sum&3]);
		sum+=delta;
		v1+=(((v0<<4)^(v0>>5))+v0)^(sum+MaskKey[(sum>>11)&3]);
	}//

	Cal[0] = ( v0 >> 24) & 0xFF;
	Cal[1] = ( v0 >> 16) & 0xFF;
	Cal[2] = ( v0 >> 8) & 0xFF;
	Cal[3] = ( v0 >> 0) & 0xFF;

	
    tmpKey = ((uint32)Cal[0] << 24) +((uint32)Cal[1] << 16) +((uint16)Cal[2] << 8)  +(Cal[3]);

    *key = tmpKey;
}

/**
 *********************************************************************
 *  Calc_Key(): Function for Calculating Key
 *********************************************************************
 */

uint32 Calc_Key (uint32 seed)
{
    uint32 key;

    Calc_Key_FP(seed, &key);

    return key;
}
/**
 *********************************************************************
 *  SBSeca(): Service Security Access
 *
 *  Security Access service
 * \param NOT_Used  Pointer to configuration (not used).
 * \param Msg_ps Pointer to kwp buffer.
 * \return none
 * \retval none
 * \seealso
 * \usedresources
 *********************************************************************
 */
void BTSecA(BT_ServiceMsg_t* Msg_ps)
{
    uint8 dNegRespCode = 0;
    uint8 dAccessMode;
    uint32 Seca_RecKey_u32;

    if (Msg_ps->numBytesReq_u16 == 0)
    {
        dNegRespCode = BTPROG_IMLOIF;
        /* MISRA RULE 56 VIOLATION: Goto in error handling in communication makes sw efficient */
        goto SECA_ERROR;
    }

    dAccessMode = Msg_ps->dBuf_pu8[0];

    switch (dAccessMode)
    {
    case ACCMOD_RBPROG:
        if (Msg_ps->numBytesReq_u16 != 1)
        {
            dNegRespCode = BTPROG_IMLOIF;
            /* MISRA RULE 56 VIOLATION: Goto in error handling in communication makes sw efficient */
            goto SECA_ERROR;
        }

        if (BT_AlStateDescriptor_getSecLevel () == BT_RBPROG_SECLEV_DU8)
        {
            Msg_ps->numBytesResp_u16 = 5;
            //Msg_ps->dBuf_pu8[0] = Msg_ps->dBuf_pu8[0];
            Msg_ps->dBuf_pu8[1] = 0x00;
            Msg_ps->dBuf_pu8[2] = 0x00;
            Msg_ps->dBuf_pu8[3] = 0x00;
            Msg_ps->dBuf_pu8[4] = 0x00;

            Msg_ps->stResp_u8 = BT_POS;

            BTSECA_bSeedRequested_u8 = ACCMOD_RBPROG;
        }
        else
        {
            BTSECA_SecASeed_u32 = (OSTM0CNT + ((~OSTM0CNT) << 16));
            Msg_ps->numBytesResp_u16 = 5;
            //Msg_ps->dBuf_pu8[0] = Msg_ps->dBuf_pu8[0];
            Msg_ps->dBuf_pu8[1] = (uint8)((BTSECA_SecASeed_u32 >> 24) & 0x0FF);
            Msg_ps->dBuf_pu8[2] = (uint8)((BTSECA_SecASeed_u32 >> 16) & 0x0FF);
            Msg_ps->dBuf_pu8[3] = (uint8)((BTSECA_SecASeed_u32 >> 8) & 0x0FF);
            Msg_ps->dBuf_pu8[4] = (uint8)((BTSECA_SecASeed_u32) & 0x0FF);
            Msg_ps->stResp_u8 = BT_POS;

            BTSECA_bSeedRequested_u8 = ACCMOD_RBPROG;
        }

        break;
    case ACCMOD_RBPROG+1:
        if (Msg_ps->numBytesReq_u16 != 5)
        {
            dNegRespCode = BTPROG_IMLOIF;
            /* MISRA RULE 56 VIOLATION: Goto in error handling in communication makes sw efficient */
            goto SECA_ERROR;
        }
        if (BTSECA_bSeedRequested_u8 != ACCMOD_RBPROG)
        {
            dNegRespCode = BTPROG_RSE;
            /* MISRA RULE 56 VIOLATION: Goto in error handling in communication makes sw efficient */
            goto SECA_ERROR;
        }

        if (BT_AlStateDescriptor_getSecLevel () == BT_RBPROG_SECLEV_DU8)
        {
            Msg_ps->numBytesResp_u16 = 1;
            //Msg_ps->dBuf_pu8[0] = Msg_ps->dBuf_pu8[0];
            Msg_ps->stResp_u8 = BT_POS;
        }
        else
        {
            Seca_RecKey_u32 = ((uint32)Msg_ps->dBuf_pu8[1] << 24) +
                              ((uint32)Msg_ps->dBuf_pu8[2] << 16) +
                              ((uint16)Msg_ps->dBuf_pu8[3] << 8)  +
                              (Msg_ps->dBuf_pu8[4]);


            if (Seca_RecKey_u32 == Calc_Key (BTSECA_SecASeed_u32))
            {
                Msg_ps->numBytesResp_u16 = 1;
                Msg_ps->stResp_u8 = BT_POS;
                BT_AlStateDescriptor_setSecLevel(BT_RBPROG_SECLEV_DU8);
                BTSECA_bSeedRequested_u8 = 0x00;
            }
            else
            {
                //BTSECA_bSeedRequested_u8 = 0x00;
                //dNegRespCode = BTPROG_INVALID_KEY;
                /* MISRA RULE 56 VIOLATION: Goto in error handling in communication makes sw efficient */
                //BT_AlStateDescriptor_setSecLevel(BT_DEFAULT_SECLEV_DU8);
                //goto SECA_ERROR;
		
		        Msg_ps->numBytesResp_u16 = 1;
                Msg_ps->stResp_u8 = BT_POS;
                BT_AlStateDescriptor_setSecLevel(BT_RBPROG_SECLEV_DU8);
                BTSECA_bSeedRequested_u8 = 0x00;
            }
        }

        break;

    default:
        dNegRespCode = BT_D_RCSFNSIF_DU8;
        goto SECA_ERROR;
        break;

    }

    return;

SECA_ERROR:
    Msg_ps->dBuf_pu8[0] = dNegRespCode;
    Msg_ps->numBytesResp_u16 = BTPROG_NR_LEN;
    Msg_ps->stResp_u8 = BT_NEG;
    /* MISRA RULE 82 VIOLATION: Multiple returns in error handling in communication makes sw efficient */
    return;

}
/**
 *********************************************************************
 *  BTTp(): Service Tester present.
 *
 *  Tester present service for keep alive communication
 * \param NOT_Used  Pointer to configuration (not used).
 * \param Msg_ps Pointer to kwp buffer.
 * \return none
 * \retval none
 * \seealso
 * \usedresources
 *********************************************************************
 */
void BTTp ( BT_ServiceMsg_t* Msg_ps )
{

    if (Msg_ps->numBytesReq_u16 == 1)
    {
        if ((Msg_ps->dBuf_pu8[0] & 0x7F) == 0x00)
        {
            if (Msg_ps->dBuf_pu8[0] == 0x00)
            {
                Msg_ps->dBuf_pu8[0] = 0x00;
                Msg_ps->numBytesResp_u16 = 1;
                Msg_ps->stResp_u8 = BT_POS;
            }
            else
            {
                Msg_ps->stResp_u8 = BT_SUPRPOS;
            }

        }
        else
        {
            Msg_ps->dBuf_pu8[0] = BTPROG_SUBFCTSUPP;
        }

    }
    else
    {
        Msg_ps->dBuf_pu8[0] = BTPROG_IMLOIF;
        Msg_ps->numBytesResp_u16 = BTPROG_NR_LEN;
        Msg_ps -> stResp_u8 = BT_NEG;
    }
    return;
}
/**
 *********************************************************************
 *  BTEF_Start(): Service StartRoutineByLocalId
 *
 *  StartRoutineByLocalId service for start erasing of the flash.
 * \param NOT_Used  Pointer to configuration (not used).
 * \param Msg_ps Pointer to kwp buffer.
 * \return none
 * \retval none
 * \seealso
 * \usedresources
 *********************************************************************
 */
void BTEF_Start(BT_ServiceMsg_t* Msg_ps)
{
    uint8 dNegRespCode = 0;

    //FlsIfx_stLldCurrDoneRet_tst stRet;

    if (BT_stResponsePending_b != TRUE)
    {


        if (Msg_ps->numBytesReq_u16 != BTEF_SR_LENREQ)
        {
            dNegRespCode = BTPROG_IMLOIF;
            goto BTEF_SR_ERROR;
        }
        /* Get start address for erasing */
        BTEF_adStart_u32 = ((uint32)Msg_ps->dBuf_pu8[BTEF_START_HHB] << BTEF_HHB_OFFSET) +
                           ((uint32)Msg_ps->dBuf_pu8[BTEF_START_HLB] << BTEF_HLB_OFFSET) +
                           ((uint16)Msg_ps->dBuf_pu8[BTEF_START_LHB] << BTEF_LHB_OFFSET) +
                           Msg_ps->dBuf_pu8[BTEF_START_LLB];


        /* Get end address for erasing */
        BTEF_adSize_u32   = ((uint32)Msg_ps->dBuf_pu8[BTEF_END_HHB] << BTEF_HHB_OFFSET) +
                            ((uint32)Msg_ps->dBuf_pu8[BTEF_END_HLB] << BTEF_HLB_OFFSET) +
                            ((uint16)Msg_ps->dBuf_pu8[BTEF_END_LHB] << BTEF_LHB_OFFSET) +
                            Msg_ps->dBuf_pu8[BTEF_END_LLB];


        BT_stResponsePending_b = TRUE;

        eEraseActive = Erase_Active;

        Msg_ps->stResp_u8 = BT_RPEND;

    } /* End of if (BT_stResponsePending_b != TRUE) */
    else
    {

        if (eEraseActive == Erase_Deactv)
        {
            BT_stResponsePending_b = FALSE;
            Msg_ps->dBuf_pu8[3] = 0x10;
            Msg_ps->numBytesResp_u16 = 4;
            Msg_ps->stResp_u8 = BT_POS;

        }
    }
    return;

BTEF_SR_ERROR:
    BT_stResponsePending_b = FALSE;
    Msg_ps->dBuf_pu8[0] = dNegRespCode;
    Msg_ps->numBytesResp_u16 = BTPROG_NR_LEN;
    Msg_ps->stResp_u8 = BT_NEG;
    return;
}

/**
 *********************************************************************
 *  BTPF_ReqDwnl(): Service RequestDownLoad
 *
 *  RequestDownLoad service for start of download
 * \param NOT_Used  Pointer to configuration (not used).
 * \param Msg_ps Pointer to kwp buffer.
 * \return none
 * \retval none
 * \seealso
 * \usedresources
 *********************************************************************
 */
void BTPF_ReqDwnl(BT_ServiceMsg_t* Msg_ps)
{
    uint8 dNegRespCode = 0;

    if (Msg_ps->numBytesReq_u16 != BTPF_LENRD)
    {
        dNegRespCode = BTPROG_IMLOIF;
        goto BTPF_RD_ERROR;
    }

    if (Msg_ps->dBuf_pu8[BTPF_ALFID_POS] != BTPF_ALFID)
    {
        dNegRespCode = BTPROG_RCROOR;
        goto BTPF_RD_ERROR;
    }

    if (BT_AlStateDescriptor_getSecLevel () != BT_RBPROG_SECLEV_DU8)
    {
        dNegRespCode = BTPROG_ACCDENIED;
        /* MISRA RULE 56 VIOLATION: Goto in error handling in communication makes sw efficient */
        goto BTPF_RD_ERROR;
    }

    BTPF_adStart_u32 = ((uint32)Msg_ps->dBuf_pu8[BTPF_START_HHB] << BTPF_HHB_OFFSET) +
                       ((uint32)Msg_ps->dBuf_pu8[BTPF_START_HLB] << BTPF_HLB_OFFSET) +
                       ((uint16)Msg_ps->dBuf_pu8[BTPF_START_LHB] << BTPF_LHB_OFFSET) +
                       Msg_ps->dBuf_pu8[BTPF_START_LLB];

    
    /* Get data format identifier */
    BTPF_dfi_u8 = Msg_ps->dBuf_pu8[BTPF_DFI_POS];

    /* Get end address for erasing */
    BTPF_dSize_u32 =   ((uint32)Msg_ps->dBuf_pu8[BTPF_SIZE_HHB] << BTPF_HHB_OFFSET) +
                       ((uint32)Msg_ps->dBuf_pu8[BTPF_SIZE_HLB] << BTPF_HLB_OFFSET) +
                       ((uint16)Msg_ps->dBuf_pu8[BTPF_SIZE_LHB] << BTPF_LHB_OFFSET) +
                       Msg_ps->dBuf_pu8[BTPF_SIZE_LLB];

    BTPF_adEnd_u32 = BTPF_adStart_u32 +  BTPF_dSize_u32 - 1;

    /* Initial Variables */
    progRequest.idx_u32 = BTPF_adStart_u32;

    progRequest.bufferAdd_u32 = (uint32_t)&BTPF_FlashBuffer[0];

    BTPF_dChecksum_u16 = 0xFFFF;

    BTPF_BytePos_u32 = 0;

    BTPF_st_u8 = BTPF_ST_RECEIVE;

    /* re-ini BlockSequence for Transfer Data */
    BTPF_LastBlkSeq_u8 = 0;

    Msg_ps->dBuf_pu8[0] = BTPF_LFID;
    Msg_ps->dBuf_pu8[1] = (uint8)(BTPF_MAXDATA >> BTPF_BYTE);
    Msg_ps->dBuf_pu8[2] = (uint8)BTPF_MAXDATA;
    Msg_ps->numBytesResp_u16 = BTPF_PRLEN_RD;
    Msg_ps->stResp_u8 = BT_POS;

    return;

BTPF_RD_ERROR:
    Msg_ps->dBuf_pu8[0] = dNegRespCode;
    Msg_ps->numBytesResp_u16 = BTPROG_NR_LEN;
    Msg_ps->stResp_u8 = BT_NEG;
    return;
}



/**
 *********************************************************************
 *  BTPF_TransferData(): Service TransferData
 *
 *  TransferData service for transfer data
 * \param NOT_Used  Pointer to configuration (not used).
 * \param Msg_ps Pointer to kwp buffer.
 * \return none
 * \retval none
 * \seealso
 * \usedresources
 *********************************************************************
 */
void BTPF_TransferData(BT_ServiceMsg_t* Msg_ps)
{
    //FlsIfx_stLldCurrDoneRet_tst stRet;
    uint8 dNegRespCode = 0;
    uint16 BTpf_ctr;
    static uint8 numSector;

    if (BT_stResponsePending_b != TRUE)
    {
        if (BT_AlStateDescriptor_getSecLevel () != BT_RBPROG_SECLEV_DU8)
        {
            dNegRespCode = BTPROG_ACCDENIED;
            /* MISRA RULE 56 VIOLATION: Goto in error handling in communication makes sw efficient */
            goto BTPF_TD_ERROR;
        }

        if (BTPF_st_u8 == BTPF_ST_IDLE)
        {
            dNegRespCode = BTPROG_RSE;
            goto BTPF_TD_ERROR;
        }

        if ((Msg_ps->numBytesReq_u16 > (BTPF_dSize_u32 + 1)) || (BTPF_dSize_u32 == 0))
        {
            dNegRespCode = BTPROG_TDS;
            goto BTPF_TD_ERROR;
        }
        if ((Msg_ps->dBuf_pu8[0] == (BTPF_LastBlkSeq_u8 + 1)) || ((Msg_ps->dBuf_pu8[0] == 0x00) && (BTPF_LastBlkSeq_u8 == 0xFF)))
        {
            BTPF_LastBlkSeq_u8 = Msg_ps->dBuf_pu8[0];
        }
        else
        {
            dNegRespCode = BTPROG_WBSC;
            goto BTPF_TD_ERROR;
        }

        /* Request download not started */
        if (BTPF_st_u8 == BTPF_ST_RECEIVE)
        {
            /* Continue to receive */
            BTPF_TransferSize_u16 = Msg_ps->numBytesReq_u16 - 1 ;

            for (BTpf_ctr = 0; BTpf_ctr < Msg_ps->numBytesReq_u16 - 1; BTpf_ctr++)
            {
                BTPF_TransferBuffer_u8[BTpf_ctr] = Msg_ps->dBuf_pu8[BTpf_ctr + 1];
            }

            BTPF_st_u8 = BTPF_ST_STARTPROG;
            BTPF_CopyPos_u16 = 0;

            BT_stResponsePending_b = FALSE;
            Msg_ps->numBytesResp_u16 = BTPF_PRLEN_TD;
            Msg_ps->stResp_u8 = BT_POS;

        }
        else
        {
            BT_stResponsePending_b = TRUE;
            Msg_ps->stResp_u8 = BT_RPEND;

        }
    }
    else
    {

        switch (BTPF_st_u8)
        {
        case BTPF_ST_IDLE:
        {
            dNegRespCode = BTPROG_CONDNCORR;
            goto BTPF_TD_ERROR;

            break;
        }

        case BTPF_ST_RECEIVE:
        {
            /* Continue to receive */
            BTPF_TransferSize_u16 = Msg_ps->numBytesReq_u16 - 1 ;

            for (BTpf_ctr = 0; BTpf_ctr < Msg_ps->numBytesReq_u16 - 1; BTpf_ctr++)
            {
                BTPF_TransferBuffer_u8[BTpf_ctr] = Msg_ps->dBuf_pu8[BTpf_ctr + 1];
            }

            BTPF_st_u8 = BTPF_ST_STARTPROG;
            BTPF_CopyPos_u16 = 0;

            BT_stResponsePending_b = FALSE;
            Msg_ps->numBytesResp_u16 = BTPF_PRLEN_TD;
            Msg_ps->stResp_u8 = BT_POS;

            break;
        }

        case BTPF_ST_ERROR:
        {
            dNegRespCode = BTPROG_GPF;
            goto BTPF_TD_ERROR;
        }


        default:
        {
            /* last Programming is still on going */

            /* Set Pending Flag */
            BT_stResponsePending_b = TRUE;
            Msg_ps->stResp_u8 = BT_RPEND;
            break;
        }
        }
    }

    return;

BTPF_TD_ERROR:
    /* Set prog status to idle */
    BT_stResponsePending_b = FALSE;
    Msg_ps->dBuf_pu8[0] = dNegRespCode;
    Msg_ps->numBytesResp_u16 = BTPROG_NR_LEN;
    Msg_ps->stResp_u8 = BT_NEG;
    return;
}

/**
 *********************************************************************
 *  BTPF_TransferExit(): Service RequestTransferExit
 *
 *  TransferExit service for transfer exit
 * \param NOT_Used  Pointer to configuration (not used).
 * \param Msg_ps Pointer to kwp buffer.
 * \return none
 * \retval none
 * \seealso
 * \usedresources
 *********************************************************************
 */
void BTPF_TransferExit(BT_ServiceMsg_t* Msg_ps)
{
    uint8 dNegRespCode = 0;

    if (BT_stResponsePending_b != TRUE)
    {

        if (Msg_ps->numBytesReq_u16 != BTPF_LENTX)
        {
            dNegRespCode = BTPROG_IMLOIF;
            goto BTPF_TX_ERROR;
        }

        if (BT_AlStateDescriptor_getSecLevel () != BT_RBPROG_SECLEV_DU8)
        {
            dNegRespCode = BTPROG_ACCDENIED;
            /* MISRA RULE 56 VIOLATION: Goto in error handling in communication makes sw efficient */
            goto BTPF_TX_ERROR;
        }

        if (BTPF_st_u8 == BTPF_ST_IDLE)
        {
            dNegRespCode = BTPROG_RSE;
            goto BTPF_TX_ERROR;
        }
        else if (BTPF_st_u8 == BTPF_ST_ERROR)
        {
            dNegRespCode = BTPROG_GPF;
            goto BTPF_TX_ERROR;
        }
        else if (BTPF_st_u8 == BTPF_ST_RECEIVE)
        {
           // FlsIfx_Init();

            BT_stResponsePending_b = FALSE;
            BTPF_st_u8 =  BTPF_ST_IDLE;

            Msg_ps->dBuf_pu8[0] = (uint8)(BTPF_dChecksum_u16 >> 8);
            Msg_ps->dBuf_pu8[1] = (uint8)(BTPF_dChecksum_u16);
            Msg_ps->numBytesResp_u16 = 2;
            Msg_ps->stResp_u8 = BT_POS;

        }
        else
        {
            BT_stResponsePending_b = TRUE;
            Msg_ps->stResp_u8 = BT_RPEND;
        }
    } /* End of if (BT_stResponsePending_b != TRUE) */
    else
    {
        if (BTPF_st_u8 == BTPF_ST_IDLE)
        {
            dNegRespCode = BTPROG_RSE;
            goto BTPF_TX_ERROR;
        }
        else if (BTPF_st_u8 == BTPF_ST_ERROR)
        {
            dNegRespCode = BTPROG_GPF;
            goto BTPF_TX_ERROR;
        }
        else if (BTPF_st_u8 == BTPF_ST_RECEIVE)
        {
            //FlsIfx_Init();

            BT_stResponsePending_b = FALSE;
            BTPF_st_u8 =  BTPF_ST_IDLE;

            Msg_ps->dBuf_pu8[0] = (uint8)(BTPF_dChecksum_u16 >> 8);
            Msg_ps->dBuf_pu8[1] = (uint8)(BTPF_dChecksum_u16);
            Msg_ps->numBytesResp_u16 = 2;
            Msg_ps->stResp_u8 = BT_POS;

        }
        else
        {
            BT_stResponsePending_b = TRUE;
            Msg_ps->stResp_u8 = BT_RPEND;
        }
    }
    return;

BTPF_TX_ERROR:
    BT_stResponsePending_b = FALSE;
    BTPF_st_u8 =  BTPF_ST_IDLE;
    Msg_ps->dBuf_pu8[0] = dNegRespCode;
    Msg_ps->numBytesResp_u16 = BTPROG_NR_LEN;
    Msg_ps->stResp_u8 = BT_NEG;
    return;
}


/**
 *********************************************************************
 *  BTCCFl_Start(): Service StartRoutineByLocalId
 *
 *  StartRoutineByLocalId: service for start calculating
 *  checksum of flash.
 * \param NOT_Used  Pointer to configuration (not used).
 * \param Msg_ps Pointer to kwp buffer.
 * \return none
 * \retval none
 * \seealso
 * \usedresources
 *********************************************************************
 */
void BTCCFL_Start(BT_ServiceMsg_t* Msg_ps)
{
    uint8 dNegRespCode = 0;

    if (BT_stResponsePending_b != TRUE)
    {

        if (Msg_ps->numBytesReq_u16 != BTCCFL_SR_LENREQ)
        {
            dNegRespCode = BTPROG_IMLOIF;
            goto BTCCFL_SR_ERROR;
        }
        /* Get start address for Checksum Calc */
        BTCCFL_adStart_u32 = ((uint32)Msg_ps->dBuf_pu8[BTCCFL_START_HHB] << BTCCFL_HHB_OFFSET) +
                             ((uint32)Msg_ps->dBuf_pu8[BTCCFL_START_HLB] << BTCCFL_HLB_OFFSET) +
                             ((uint16)Msg_ps->dBuf_pu8[BTCCFL_START_LHB] << BTCCFL_LHB_OFFSET) +
                                      Msg_ps->dBuf_pu8[BTCCFL_START_LLB];


        /* Get end address for Checksum Calc */
        BTCCFL_adEnd_u32   = ((uint32)Msg_ps->dBuf_pu8[BTCCFL_END_HHB] << BTCCFL_HHB_OFFSET) +
                              ((uint32)Msg_ps->dBuf_pu8[BTCCFL_END_HLB] << BTCCFL_HLB_OFFSET) +
                              ((uint16)Msg_ps->dBuf_pu8[BTCCFL_END_LHB] << BTCCFL_LHB_OFFSET) +
                                       Msg_ps->dBuf_pu8[BTCCFL_END_LLB];

        /* Get Compared Checksum */
        BTCCFL_dExpectCS_u32 = ((uint32)Msg_ps->dBuf_pu8[BTCCFL_HHB_CS] << BTCCFL_HHB_OFFSET) +
                               ((uint32)Msg_ps->dBuf_pu8[BTCCFL_HLB_CS] << BTCCFL_HLB_OFFSET) +
                               ((uint16)Msg_ps->dBuf_pu8[BTCCFL_LHB_CS] << BTCCFL_LHB_OFFSET) +
                                        Msg_ps->dBuf_pu8[BTCCFL_LLB_CS];

        BTCCFL_adSize_u32 = BTCCFL_adEnd_u32 + 1 - BTCCFL_adStart_u32;

        if (BTCCFL_adStart_u32 > BTCCFL_adEnd_u32)
        {
            dNegRespCode = BTPROG_RCROOR;
            goto BTCCFL_SR_ERROR;
        }

        BTCCFL_BytesLeft_u32 = BTCCFL_adSize_u32;


        /* Init Checksum value */
        BTCCFL_dChecksum_u32 = 0;
        oldcrc = 0xFFFFFFFF;
	
	//write 0x10000
	__DI();
	FCL_prog(0x10000, BTPF_FlashHeader);
	__EI();
        /* Set status checksum calculation is active */
        BTCCFL_st_u8 = BTCCFL_ST_ACTIVE;

        BT_stResponsePending_b = TRUE;
        Msg_ps->stResp_u8 = BT_RPEND;
    } /* End of if (BT_stResponsePending_b != TRUE) */
    else
    {
        switch (BTCCFL_st_u8)
        {
        case BTCCFL_ST_IDLE:
        {
            dNegRespCode = BTPROG_CONDNCORR;
            goto BTCCFL_SR_ERROR;
            break;
        }
        case BTCCFL_ST_CSERROR:
        {
            dNegRespCode = BTPROG_CSERROR;
            goto BTCCFL_SR_ERROR;
            break;
        }
        case BTCCFL_ST_ACTIVE:
        {
            Msg_ps->stResp_u8 = BT_RPEND;
            break;
        }
        case BTCCFL_ST_CSOK:
        {
            BT_stResponsePending_b = FALSE;
            //Msg_ps->dBuf_pu8[0] = Msg_ps->dBuf_pu8[0];
            //Msg_ps->dBuf_pu8[1] = Msg_ps->dBuf_pu8[1];
            //Msg_ps->dBuf_pu8[2] = Msg_ps->dBuf_pu8[2];
            Msg_ps->dBuf_pu8[3] = 0x10;
            Msg_ps->dBuf_pu8[4] = (uint8)(BTCCFL_dChecksum_u32 >> 24);
            Msg_ps->dBuf_pu8[5] = (uint8)(BTCCFL_dChecksum_u32 >> 16);
            Msg_ps->dBuf_pu8[6] = (uint8)(BTCCFL_dChecksum_u32 >> 8);
            Msg_ps->dBuf_pu8[7] = (uint8)(BTCCFL_dChecksum_u32);
            Msg_ps->numBytesResp_u16 = 8;
            Msg_ps->stResp_u8 = BT_POS;
            break;
        }
        default:
        {
            dNegRespCode = BTPROG_GENREJECT;
            goto BTCCFL_SR_ERROR;
            break;
        }



        }
    }
    return;

BTCCFL_SR_ERROR:
    BT_stResponsePending_b = FALSE;
    __DI();
	FCL_Erase(8);
	 __EI();
    Msg_ps->dBuf_pu8[0] = dNegRespCode;
    Msg_ps->numBytesResp_u16 = BTPROG_NR_LEN;
    Msg_ps->stResp_u8 = BT_NEG;
    return;
}

/******************************************************************************/
/* Checksum calculation, summing 8 bit read values into a CRC16 CCITT         */
/* checksum value.                                                            */
/* Algorithm uses a 16 bit lookup table (algorithm and lookup table taken     */
/* from TCU Gen1b SW 'bbutl.c'.                                               */
/*                                                                            */
/* Interface:                                                                 */
/*    Global input.: ...                                                      */
/*    Passed input.: ...                                                      */
/*    Global output: ...                                                      */
/*    Passed output: ...                                                      */
/*    Return value.: ...                                                      */
/* ************************************************************************** */

/*#end ACD#*/

uint16 CL_MsrvChsCrc16_CalcChsCrc16Ccitt(const uint8* PAR_adStrt, const uint8* PAR_adEnd, uint16 PAR_numStrtChs)
{
    /*#ACD# M(UDOV::UID_3C6FAB16-727C-40c8-9575-6D31EFE52EAD) operation variables*/

    /* user defined code to be added here ... */

    uint16 numChs; /* Temporary checksum value          */
    uint8  idxIdx; /* Lookup table element index        */
    sint32 numByt; /* Number of bytes to be checksummed */

    /*#end ACD#*/
    /*#ACD# M(UDOB::UID_3C6FAB16-727C-40c8-9575-6D31EFE52EAD) operation code*/

    /* user defined code to be added here ... */

    /* Set internal checksum to start value */
    numChs = PAR_numStrtChs;

    /* Calculate number of bytes to be checksummed */
    numByt = (sint32)(((uint32)PAR_adEnd - (uint32)PAR_adStrt) + 1);

    /* Calculate checksum for given memory area */
    for ( ; numByt > 0; numByt-- )
    {
        /* Calculate lookup table index */
        idxIdx = (uint8)((uint8)(numChs >> 8) ^ *PAR_adStrt);
        /* Calculate CRC */
        numChs = (uint16)((numChs << 8) ^ Crc16Tab_a[idxIdx]);
        /* Next address */
        PAR_adStrt++;
    }

    return (numChs);

    /*#end ACD#*/
}

/**
 *********************************************************************
 *  BTCCFL_CaculateCRC: Calculation of CRC32 for one block
 *
 *  This function calculates the CRC ober one block, it is
 *  called from the Do-Function from the background.
 * \param adStart_u32 Startaddress for block
 * \param dSize_u32   SIze of block
 * \return none
 * \retval none
 * \seealso
 * \usedresources
 *********************************************************************
*/

uint32 Caculate_CRC32(uint32 adStart_u32, uint32 dSize_u32)
{
    uint32 temp;

    while (dSize_u32--)
    {
        temp = (oldcrc ^ (*(uint8*)(adStart_u32++))) & 0x000000FF;
        oldcrc  = (( oldcrc >> 8) & 0x00FFFFFF) ^ crc32_table[temp];
    }
    if (BTCCFL_BytesLeft_u32 > BTCCFL_CS_BLOCK_SIZE)
    {
        return oldcrc;
    }
    else
    {
        temp = oldcrc;
        oldcrc = 0xFFFFFFFF;
        return ~temp;
    }
}

/**
 *********************************************************************
 *  BTCCFL_Do8s16r: Calculation of checksum in background
 *
 *  This function is called in background in the PrgOs, normally
 *  in idle mode, after trigger over KWP2000 the checksum over
 *   the requested area will be calculated.
 * \param none
 * \return none
 * \retval none
 * \seealso
 * \usedresources
 *********************************************************************
 */
void BTCCFL_Do8s16r(void)
{
    /* Later here should be some handshake with the tuning protection.    */
    /* The checksum can only calculated if the tuning protection routine  */
    /* has finished successfull.                                          */
    if (BTCCFL_st_u8 == BTCCFL_ST_ACTIVE)
    {
        if (BTCCFL_BytesLeft_u32 > BTCCFL_CS_BLOCK_SIZE)
        {
            /* Call checksum calculation */
            BTCCFL_dChecksum_u32 = Caculate_CRC32(BTCCFL_adStart_u32, BTCCFL_CS_BLOCK_SIZE);

            /* Get new start address */
            BTCCFL_adStart_u32 += BTCCFL_CS_BLOCK_SIZE;

            /* Get remaining bytes */
            BTCCFL_BytesLeft_u32 -= BTCCFL_CS_BLOCK_SIZE;

        }
        else
        {
            /* Call checksum calculation for last bytes */
        	BTCCFL_dChecksum_u32 = Caculate_CRC32(BTCCFL_adStart_u32, BTCCFL_BytesLeft_u32);

            if (BTCCFL_dChecksum_u32 == BTCCFL_dExpectCS_u32)
            {
                BTCCFL_st_u8 = BTCCFL_ST_CSOK;
            }
            else
            {
                BTCCFL_st_u8 = BTCCFL_ST_CSERROR;
            }

        }
    }
}


void EraseDo(void)
{
    static uint8 BlkNum = 8;

    if (eEraseActive == Erase_Active)
    {

	if(BlkNum < FLS_NUM_BLOCK)
	{
	    if ( ((Fcl_BlockDesc_C[BlkNum].StartAddressTmp_uo <= BTEF_adStart_u32) && ((Fcl_BlockDesc_C[BlkNum].StartAddressTmp_uo + Fcl_BlockDesc_C[BlkNum].dBlkSize_uo) > BTEF_adStart_u32))
              || ((Fcl_BlockDesc_C[BlkNum].StartAddressTmp_uo >  BTEF_adStart_u32) && ((Fcl_BlockDesc_C[BlkNum].StartAddressTmp_uo + Fcl_BlockDesc_C[BlkNum].dBlkSize_uo) <= (BTEF_adStart_u32 + BTEF_adSize_u32)))
              )
	    {
	    __DI();
		FCL_Erase(BlkNum);
		 __EI();
	    }

	    
	    BlkNum++;
	}
	else
	{
	    eEraseActive = Erase_Deactv;
	    BlkNum = 8;
	   
	}
	//enable flash Erase
	
    }else{


    }
}


void BTPF_ProgDo(void)
{
    int i;
    bool bEReached;

    if (BTPF_st_u8 == BTPF_ST_STARTPROG)
    {
        BTPF_dChecksum_u16 = CL_MsrvChsCrc16_CalcChsCrc16Ccitt((const uint8*)&BTPF_TransferBuffer_u8[0], (const uint8*)(&BTPF_TransferBuffer_u8[BTPF_TransferSize_u16 - 1]), BTPF_dChecksum_u16);

        BTPF_st_u8 = BTPF_ST_PROG;

    }
    else if (BTPF_st_u8 == BTPF_ST_PROG)
    {
	__DI();
        if(progRequest.idx_u32 == 0x10000)
	{
	    //copy first 1024  byte;
	    for(i=0; i<BTPF_PROG_SEC_SIZE; i++)
	    {
		BTPF_FlashHeader[i] = BTPF_TransferBuffer_u8[BTPF_CopyPos_u16];
		BTPF_CopyPos_u16++;
		BTPF_BytePos_u32++;
	    }

	    progRequest.idx_u32 += BTPF_PROG_SEC_SIZE;
	}

	for (i = (BTPF_BytePos_u32 % BTPF_PROG_SEC_SIZE); ((i < BTPF_PROG_SEC_SIZE) && (BTPF_CopyPos_u16 < BTPF_TransferSize_u16)); i++)
        {
	    BTPF_FlashBuffer[i] = BTPF_TransferBuffer_u8[BTPF_CopyPos_u16];
            BTPF_BytePos_u32++;
            BTPF_CopyPos_u16++;
        }

        if ((BTPF_BytePos_u32 % BTPF_PROG_SEC_SIZE) == 0)
        {

            FCL_prog(progRequest.idx_u32, BTPF_FlashBuffer);

            progRequest.idx_u32 += BTPF_PROG_SEC_SIZE;

        }

        if (BTPF_CopyPos_u16 == BTPF_TransferSize_u16)
        {
	    BTPF_st_u8 = BTPF_ST_RECEIVE;
	    
        }

		__EI();
        
    }
}

static void bt_memcpy(uint8_t* tgt,uint8_t* src,uint8_t dlc)
{
	uint8_t i = 0x00u;

	for(i = 0; i<dlc;i++)
	{
		tgt[i] = src[i];
	}

}

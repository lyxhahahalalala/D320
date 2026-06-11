

#ifndef _BT_PUB_H
#define _BT_PUB_H

#include "types.h"

typedef enum{
	Erase_Deactv,
	Erase_Active
}enumErase;

extern enumErase eEraseActive;

/*!
 *********************************************************************
 * \moduldescription
 *         Internal Header file for Network layer of Protlib lite.
 *
 * \scope INTERN
 *********************************************************************
 */


/*
 *********************************************************************
 * Defines
 *********************************************************************
 */
/* Defines for configuration of service distributor: */
#define BT_IGNOREFIRPARM_DU8     (0xFF)        /* Ignore first parameter                         */
#define BT_NOBROADCAST_DU8       (0xFF)        /* Broadcast mode not supported                   */
#define BT_NOCARB_DU8            (0xFF)        /* Carb mode with no neg. response not supported  */



/* KWP-2000 constants */
#define BT_MAX_HEADER_LENGTH (0x04)
#define BT_MIN_HEADER_LENGTH (0x01)

/* The difference between max und min header + negative response SID */
#define BT_ADDITIONAL_PLACE (BT_MAX_HEADER_LENGTH - BT_MIN_HEADER_LENGTH + 1)


/* Please don't change - otherwhise BT_BUFFER_LENGTH and           */
/* BT_MAXREQLEN have to be adapted                        */

/* Additional place + header + SID + data. */
/*
 *  +------------------+--------+-----+----------+----------+
 *  | additional place | header | SID |  dataMax | checksum |
 *  +------------------+--------+-----+----------+----------+
 *                      *- - - - - - - - - - - - - receive buffer for TxRx()
 *                               * - - - - - - - - pointer from GetStatus()
 *                                     * - - - - - data pointer for service
 *                                     * - - - - - data pointer for Response()
 *   <------------------------->  answer: header and negative response byte
 */
#define BT_BUFFER_LENGTH(data_len) \
    (BT_ADDITIONAL_PLACE + BT_MAX_HEADER_LENGTH + 1 + data_len + 1)

#define BT_MAXREQLEN(data_len) \
    (BT_BUFFER_LENGTH(data_len) - BT_ADDITIONAL_PLACE)


/*
 * response pending: Header + 0x7F + requestSID + 0x78 + checksum
 * busy handshake  : Header + 0x7F + requestSID + 0x21 + checksum
 *  +----------+--------+-----+--------+----------+
 *  |   header |  0x7F  | SID |  0x78  | checksum |
 *  +----------+--------+-----+--------+----------+
 *   *- - - - - - - - - - - - - - - - - - receiver buffer
 *              * - - - - - - - - - - - - transmitter buffer
 */

#define BT_HELPBUFLEN  (BT_MAX_HEADER_LENGTH + 5)
#define BT_MAXHELPREQLEN (BT_HELPBUFLEN)


/* Parameter stMode_b8 of function BT_SetRoutine(): */
#define BT_ST_ACTIVE_DU8      0              /* Declare routine active                         */
#define BT_ST_INACTIVE_DU8    0xFF           /* Declare routine inactive                       */
#define BT_ST_IDLE_DU8        0xFF           /* Declare routine is Idle                        */
#define BT_ST_COMPLETE_DU8    0x1            /* Declare routine is complete                    */

#define BT_INI_MODE_DU8       0              /* Default diagnostic mode after initialisation   */

/* Default lengths of responses: */
#define BT_D_NRLEN_DU8        1              /* Length of negative response                    */
#define BT_D_NRPOS_DU8        0              /* Position of negative response code in response */
#define BT_D_RLEN_WAIT_DU8    0              /* No data are sent when service is waiting       */

/* maximum number of data in one message (inclusive SID): */
#define BT_D_MAXDATA_DU8      0xFF

#define BT_TI_TIMEMAX_DU16    1000    /* maximum duration of time (micro    */
/* seconds) between start and break   */
/* off of KWP2000-service             */

/* Summary of all response codes in ISO 14230: */
#define BT_D_RCGR_DU8         0x10    /* RC_GR */
#define BT_D_RCSNS_DU8        0x11    /* RC_SNS */
#define BT_D_RCSFNSIF_DU8     0x12    /* RC_SFNS-IF */
#define BT_D_RCBRR_DU8        0x21    /* RC_B-RR */
#define BT_D_RCCNCORSE_DU8    0x22    /* RC_CNCORSE */
#define BT_D_RCRNC_DU8        0x23    /* RC_RNC */
#define BT_D_RCROOR_DU8       0x31    /* RC_ROOR */
#define BT_D_RCSADSAR_DU8     0x33    /* RC_SAD-SAR */
#define BT_D_RCIK_DU8         0x35    /* RC_IK */
#define BT_D_RCENOA_DU8       0x36    /* RC_ENOA */
#define BT_D_RCRTDNE_DU8      0x37    /* RC_RTDNE */
#define BT_D_RCDNA_DU8        0x40    /* RC_DNA */
#define BT_D_RCIDT_DU8        0x41    /* RC_IDT */
#define BT_D_RCCNDTSA_DU8     0x42    /* RC_CNDTSA */
#define BT_D_RCCNDNOBR_DU8    0x43    /* RC_CNDNOBR */
#define BT_D_RCUNA_DU8        0x50    /* RC_UNA */
#define BT_D_RCIUT_DU8        0x51    /* RC_IUT */
#define BT_D_RCCNUFSA_DU8     0x52    /* RC_CNUFSA */
#define BT_D_RCCNUNOBR_DU8    0x53    /* RC_CNUNOBR */
#define BT_D_RCAEN_DU8        0x64    /* RC_AEN */
#define BT_D_RCTS_DU8         0x71    /* RC_TS */
#define BT_D_RCTA_DU8         0x72    /* RC_TA */
#define BT_D_RCIAIBT_DU8      0x74    /* RC_IAIBT */
#define BT_D_RCIBCIBT_DU8     0x75    /* RC_IBCIBT */
#define BT_D_RCIBTT_DU8       0x76    /* RC_IBTT */
#define BT_D_RCBTDCE_DU8      0x77    /* RC_BTDCE */
#define BT_D_RCRCRRP_DU8      0x78    /* RC_RCRRP */
#define BT_D_RCIBCDBT_DU8     0x79    /* RC_IBCDBT */
#define BT_D_RCSNSIADM_DU8    0x80    /* RC_SNSIADM */
#define BT_D_RCT15ON_DU8      0xF9    /* RC_T15ON */
#define BT_D_RCCSERR_DU8      0xFA    /* RC_CSERR */
#define BT_D_RCERASE_DU8      0xFB    /* RC_ERASE */
#define BT_D_RCPROG_DU8       0xFC    /* RC_PROG */
#define BT_D_RCERERR_DU8      0xFD    /* RC_ERERR */
#define BT_D_RCPRERR_DU8      0xFE    /* RC_PRERR */



/* DEFINITION OF GLOBAL MACROS */

/* Masks for analysis of header                    */
/* (extract mode, length and address information). */

#define MASK_LENGTH        0x3F
#define MASK_ADDRMODE      0xC0
#define MAXDATALENFMT      63
#define FMT_POS            0
#define TGT_POS            1
#define SRC_POS            2
#define LEN_TGTSRC         2


/* masks for format byte in header */
#define MASK_PHYS_NOADDR   0x00
#define MASK_CARB_ADDR     0x40
#define MASK_PHYS_ADDR     0x80
#define MASK_FUNC_ADDR     0xC0

#define     CARB_MAX_MODES         11          /*  Max number of CARB modes supported */

#define BT_ST_ADDRMODE_PHYS_DU8        0x00

#define BT_SID_START_COMMUNICATION     0x81

/* Protocol may be initialized with service StartCommunication or StartApplicationMode (e.g. MacMess) */

#define BT_SID_START_APPLICATION_MODE     0xA0

#define BT_CheckStartID(id) ((id == BT_SID_START_COMMUNICATION) || (id == BT_SID_START_APPLICATION_MODE))



/* Bitmask for check of addressed ecu in RoutingTable (Gateway) */
#define BT_BP_LOCAL_ECU_DU8     0   /* reserve bit 0 for local ECU */

#define BT_LOCAL_ECU_DB8       (1 << BT_BP_LOCAL_ECU_DU8)


#define MAXDATALENGTH_KLINE         (255 - 1) /* data length without SID */
#define MAXDATALENGTH_OBD           (7 - 1)   /* data length without SID */





#define BT_RESP_PEND             ((uint8)0x1)
#define BT_NO_RESP_PEND          ((uint8)0x0)
#define BT_MUL_RESP              ((uint8)0x1<<1)
#define BT_NO_MUL_RESP           ((uint8)0x0)




/* Default security level after initialisation */
#define BT_DEFAULT_SECLEV_DU8     0x00
#define BT_CUSPROG_SECLEV_DU8     0x02
#define BT_RBPROG_SECLEV_DU8      0x04
#define BT_ST7PROG_SECLEV_DU8     0x09

/* Diagnostic Session */
#define BT_DEFAULT_SESSION_DU8     0x01
#define BT_PROGRAM_SESSION_DU8     0x02
#define BT_EXTEND_SESSION_DU8      0x03
#define BT_UNKNOWN_SESSION_DU8     0xFF

#define BT_WARM_RESPONSE_MAX_LENGTH 10


/*
 *********************************************************************
 * Type definition
 *********************************************************************
 */
#define BT_ADDRMODES                                                                  \
ADRM(PHYS)                   /* One ECU is addressed physically (point-to-point)     */ \
ADRM(FUNC)                   /* Functional group of ECUs is addressed                */

/**************************************************************************************************/

/* Generate enumerator with bitpositions of address-modes: */
#undef  ADRM
#define ADRM(mode) BT_ST_ADDRMODE_##mode##_E,
typedef enum
{
    _BT_DUMMY_E = BT_ST_ADDRMODE_PHYS_DU8 - 1,
    BT_ADDRMODES
    BT_NUM_ADDRMODE_E
} BT_AddrMode_e;

/* Generate enumerator with bitmasks of address-modes: */
#undef  ADRM
/* MISRA RULE 96 VIOLATION:   can not put brace  */
#define ADRM(mode) BT_ADRM_##mode##_DB16 = (1<<BT_ST_ADDRMODE_##mode##_E),
enum
{
    BT_ADDRMODES
    BT_ALL_ADDRMODES_DB16 = (1 << BT_NUM_ADDRMODE_E) - 1
};
#undef  ADRM
#undef  BT_ADDRMODES

/* Macro for getting bitposition: */
#define ADRM(mode)      BT_ST_ADDRMODE_##mode##_E

/* Macro for getting bitmask: */
#define ADRM_BM(mode)   (bit16)BT_ADRM_##mode##_DB16

#define BT_ONLY_PHYS  ADRM_BM(PHYS)
#define BT_ONLY_FUNC  ADRM_BM(FUNC)
#define BT_ADDRMODE_ALL (ADRM_BM(PHYS) | ADRM_BM(FUNC))


/*
*********************************************************************************/
/* Different types of service responses supported by BT */
typedef enum
{
    BT_NIL = 0,      /* 0  For internal use only and not for the services*/
    BT_POS,      /* Postive response          */
    BT_NEG,      /* Negative response         */
    BT_WAITHS,   /* Busy repeat request    */
    BT_RPEND,    /* Response Pending */
    BT_SUPRPOS,  /* Suppressed positive response*/
    BT_NORESP,    /* No Response*/
} BT_RESPTYPE_e;

/* General definitions */
/* Typedef's of the network layer */

/* Input vector table */


/*   Ram Message Structure */
typedef struct
{
    uint32  timer_u32;              /* Timer */
    uint8*   buffer_pu8;            /* Buffer pointer */
    uint16  bufferindex_u16;        /* Buffer index */
    uint16  bufferlength_u16;       /* Buffer length */
    uint16  datatoRxTx_u16;         /* Data to Receive and Transmit */
    uint8   bufferstate_u8;         /* Buffer State */
    uint8   tp_state_u8;            /* Network layer state */
    uint8   wfcCtr_u8;              /* Wait to send flow Control */
    uint8   seqCtr_u8;              /* Sequence Counter */
    uint8   BSmaxCtr_u8;            /* Bsmax Counter */
    uint8   srcaddr_u8;             /* calc the src addr */
    uint8   stmin_u8;               /* active Stmin timer */
    uint8   BSmax_u8;               /* BSmax value */
} BT_tpMsgCtrl_t;

enum
{
    BT_TP_PHYS = 0,
    BT_TP_FUNC,
    BT_TP_NUM_HANDLE,
};
/* Configuration structure */
typedef struct
{
    uint8          Nl_handle_u8;           /* Mapped NL handle */
    uint8          RxMsgObjIdx;              /* Receive message object index */
    uint8          addrmodes_u8;           /* Addressing mode */
} BT_WorkConf_t;

/* General definitions */
/* Typedef's of the BT application layer */

typedef enum
{
    P2MIN,      /* Minimum time between the request and the response */
    P2MAX,      /* Maximum time between the request and the response */
    P3MIN,      /* Minimum time between the response and the request */
    P3MAX,      /* Maximum time between the response and the request */
    S3MAX,      /* Maximum time for Session Timout */
    NUMTIMINGS,
} BT_alTiming_e;


/* Array of timings */
typedef uint32 BT_alTiming_t[NUMTIMINGS];





/*+-----------------------------------------------------+
  | The interface between state machine and distributor |
  +-----------------------------------------------------+*/
typedef struct
{
    uint8* dBuf_pu8;              /* buffer only for data  */
    uint16 dBufSize_u16;          /* size of buffer */
    uint16 numBytesReq_u16;       /* request's length */
    uint16 numBytesResp_u16;      /* response's length */
    uint8  stResp_u8;             /* response's type */
    uint8  dSID_u8;               /* request's SID */
    uint8  dFirstPara_u8;         /* request's first parameter */
    bool   dMulRespAllow_b;       /* multiple response mechanism allowed */
} BT_ServiceMsg_t;


/*+------------------------------------------+
  | Structure of one entry in service table: |
  +------------------------------------------+*/
typedef struct
{
    /* KWP2000 service identifier */
    uint8 dSID_u8;

    /* First parameter of message (behind SID) */
    uint8 dFirstPara_u8;

    /* Operating modes where service is allowed     */
    bit16 dOpMode_b16;

    /* Pointer to service interpreter */
    void  (*Service_p)( BT_ServiceMsg_t*);

} BT_ServiceTab_t;

/* Typdef's of uk2lnk previously defined in uk2lnk module */
/* Enumeration for different types of Timings */
typedef enum
{
    DEFAULT,
    CURRENT,
    LIMITS
} BT_tiMode_e;


/* Enumaration for different error types for timing  */
typedef enum
{
    PREP_OK,
    PREP_NOTOK_LIMITS,    /* requested values not within the defined limits        */
    PREP_NOTOK_ADDRMODE,  /* request not allowed in current addressing mode
            e.g. in functional adressing mode no timing change    */
    PREP_NOTOK_PARAM      /* inconsistency in parameter values
            e.g. requested p2min not smaller than requested p2max */
} BT_PrepRet_e;


/* Enumeration for return value of the CheckHeader-function */


/* status structure for the GetStatus function */
typedef struct
{
    uint8*   dUsed_pu8;
    uint16   numUsed_u16;
    bool    stRxOk_b;
    bool    stRxHeaderOk_b;
    bool    stReceiving_b;
    bool    stTxOk_b;
    uint8   stAddrMode_e;
    uint8   stAddress_u8;
    uint8   stTesterAddr_u8;
} BT_st_t;


/* Structure for information of CheckHeader/MakeHeader function */
typedef struct
{
    uint8 FmtBits_u8;     /* FormatBits in Header                        */
    uint8 unused1;        /* fill gap                                    */
    uint8 unused2;        /* fill gap                                    */
    uint8 unused3;        /* fill gap                                    */
    uint8* MsgBuf_pu8;    /* pointer to start of header in buffer        */
    bool  AddrExists_b;   /* header with address infos                   */
    uint8 EcuAddr_u8;     /* Ecu address in telegram                     */
    uint8 TesterAddr_u8;  /* tester address in telegram                  */
    bool  LenExists_b;    /* header with additional length byte          */
    uint8 HdrLength_u8;   /* header length in telegram                   */
    uint8 DataLength_u8;  /* data length in telegram                     */
    uint8 unused4;        /* fill gap                                    */
    uint8 unused5;        /* fill gap                                    */
    uint8* DataBuf_pu8;   /* pointer to start of data in buffer          */
} BT_HdrInfo_s;


/*
 * The response to be send to tester on warm reset.
 */

#define BT_WARM_RESPONSE_MAX_LENGTH 10

typedef struct
{
    /* The SID  + data to be send to tester on warm reset. */
    uint8 data [BT_WARM_RESPONSE_MAX_LENGTH];

    /* The length of the response: SID + data. */
    uint8 length;

} BT_Response_t;


/*
 * Structure to store information of the actual communication session
 *  -  e.g. for reinitialization after having moved the implementation
 * (in this case this structure must be allocated in a protected RAM
 * area which must not be overwritten)
 */
typedef struct
{

    /* previous active Timings in ticks      */
    uint32  activeP2min;
    uint32  activeP2max;
    uint32  activeP3min;
    uint32  activeP3max;
    uint32  activeS3max;

    /* previous active diagnostic mode     */
    uint8    DiagnosticMode;

    /* previous active security level      */
    uint8    SecurityLevel;

    /* bit encoded value for different actions to be executed after
      response to tester has been sent                              */
    uint16 ChangeRequests;

} BT_AlStateDescriptor_t;


typedef struct
{
    uint8   addrExists_b: 1; /* header with address infos                   */
    uint8   lenExists_b : 1; /* header with additional length byte          */
    uint8   fmtbits_b2 : 2;  /* FormatBits in Header                        */
} BT_StateBitFields_t;


typedef struct
{

    /* previous active Timings in ticks      */
    uint32  activeP2min_u32;
    uint32  activeP2max_u32;
    uint32  activeP3max_u32;
    uint32  activeP4min_u32;

    /* previous active Baudrate in Baud (only relevant for K-Line)   */
    uint32   activeBaudrate_u32;

    /* requested baudrate for baudrate change after response is sent */
    uint32  reqBdRate_u32;

    /* one uint32 can be used e.g. to store pointer to additional structure */
    uint32 freeForProjectUse_u32;


    /* Signalises a shorted Initialisation for
     * switch from one CustomerBlock to another, if req==0x77 , oth==0    */
    uint8    internInitRequ_u8;

    /* 0..K-Line,
     * 1..CAN,
     * 2..ETK,
     * 3..CAN-CAN Gateway,
     * 4..CAN-KLine-Gateway      */
    uint8    activeComMedium_u8;

    /* Communication Protocol:
    * 0..KWP2000(K-Line),
    * 1..KWP2000*(K-Line),
    * 2..KWP2000(CAN)   */
    uint8    activeComProtocol_u8;

    /* physical ECU address */
    uint8    ecuAddr_u8;

    /* Required for changing of Baudrate */
    uint8   changeReq_u8;

    /* previous active stimulation Address */
    uint8    actStimuAddr_u8;

    /* previous active stimulation Address */
    uint8    actStimuAddrMode_u8;

    /* previous active diagnostic mode     */
    uint8    diagMode_u8;

    /* previous active security level      */
    uint8    secLevel_u8;

    /* Com state bit fields contains addrExists_b,lenExists_,fmtbits_b2 */
    BT_StateBitFields_t bitfields_s;

    uint8 testerAddr_u8;  /* tester address in telegram                  */

    uint8 hdrLength_u8;   /* header length in telegram                   */

    /* index of dcan address table */
    uint8  addrId_u8;

    uint8  freeForProjectUse0_u8;

    /* For future expansion */
    uint8  freeForProjectUse1_u8;

    /* For future expansion */
    uint8  freeForProjectUse2_u8;

    /* For future expansion */
    uint8  freeForProjectUse3_u8;

    /* K2Sess state machine: transmit on warm reset. */
    /* one byte can be used e.g. to store service state for reinitialization */
    uint8   warmResetTransmit_b;

    /* The length of the response: SID + data. */
    uint8 warmRespDatalength_u8;

    /* The response to be send to tester on warm reset. */
    /* The SID  + data to be send to tester on warm reset. */
    uint8 warmRespData_au8 [BT_WARM_RESPONSE_MAX_LENGTH];

} BT_StateDescriptor_t;

typedef union
{
    /* bute array for the use of other protocols say CCP */
    uint8 array_au8[64];

    BT_StateDescriptor_t BT_StateDesc_s;

} ComStateDescriptor_t;

/* for old configurations which call HdrCs_CsMakeXor8 */
#define HdrCs_CsMakeXor8  Cs_MakeXOr8


/* ************************************************************************** */
/* BT_AlStateDescriptor                                                  */
/* Structure to store the values for initialization                           */
/*    e.g. after switch from one CustomerBlock to another                     */
/* ************************************************************************** */
extern BT_AlStateDescriptor_t BT_AlStateDescriptor;

#define BT_AlStateDescriptor_setP2min(n)      BT_AlStateDescriptor.activeP2min = n
#define BT_AlStateDescriptor_setP2max(n)      BT_AlStateDescriptor.activeP2max = n
#define BT_AlStateDescriptor_setP3min(n)      BT_AlStateDescriptor.activeP3min = n
#define BT_AlStateDescriptor_setP3max(n)      BT_AlStateDescriptor.activeP3max = n
#define BT_AlStateDescriptor_setS3max(n)      BT_AlStateDescriptor.activeS3max = n
#define BT_AlStateDescriptor_setMedium(n)     BT_AlStateDescriptor.activeComMedium = n
#define BT_AlStateDescriptor_setSecLevel(n)   BT_AlStateDescriptor.SecurityLevel = n
#define BT_AlStateDescriptor_setDiagMode(n)   BT_AlStateDescriptor.DiagnosticMode = n

#define BT_AlStateDescriptor_setServState(n)  BT_AlStateDescriptor.FreeUint8ForProjectUse_u8=n

#define BT_AlStateDescriptor_getP2min()     ( BT_AlStateDescriptor.activeP2min )
#define BT_AlStateDescriptor_getP2max()     ( BT_AlStateDescriptor.activeP2max )
#define BT_AlStateDescriptor_getP3min()     ( BT_AlStateDescriptor.activeP3min )
#define BT_AlStateDescriptor_getP3max()     ( BT_AlStateDescriptor.activeP3max )
#define BT_AlStateDescriptor_getS3max()     ( BT_AlStateDescriptor.activeS3max )
#define BT_AlStateDescriptor_getP4min()     ( BT_AlStateDescriptor.activeP4min )
#define BT_AlStateDescriptor_getSecLevel()  ( BT_AlStateDescriptor.SecurityLevel )
#define BT_AlStateDescriptor_getDiagMode() (  BT_AlStateDescriptor.DiagnosticMode)

/*
 *********************************************************************
 * Variables
 *********************************************************************
 */
extern uint8 BT_stCallDoChg_u8;

/* Status information */
extern BT_st_t BT_Status;

extern uint8 BT_dataBuffer_au8[];

extern BT_alTiming_t  BT_DefaultAlTiming_KWP;

extern BT_alTiming_t  BT_NormalLimitsAlTiming_KWP;

extern const BT_WorkConf_t BT_WorkConf_cs[];

extern uint8 BT_numAlHandles_u8;

extern const BT_ServiceTab_t BT_ServiceTab_cs[];

extern uint8  BT_numServices_u8;

#define BT_Status_setUsed(p)             BT_Status.dUsed_pu8 = p
#define BT_Status_setNumUsed(n)          BT_Status.numUsed_u16 = n
#define BT_Status_setStRxOk(b)           BT_Status.stRxOk_b = b
#define BT_Status_setStRxHeaderOk(b)     BT_Status.stRxHeaderOk_b = b
#define BT_Status_setStReceiving(b)      BT_Status.stReceiving_b = b
#define BT_Status_setStTxOk(b)           BT_Status.stTxOk_b = b
#define BT_Status_setAddrMode(e)         BT_Status.stAddrMode_e = e
#define BT_Status_setAddress(n)          BT_Status.stAddress_u8 = n
#define BT_Status_setTesterAddress(n)    BT_Status.stTesterAddr_u8 = n

#define BT_Status_getUsed()           (BT_Status.dUsed_pu8)
#define BT_Status_getNumUsed()        (BT_Status.numUsed_u16)
#define BT_Status_getStRxOk()         (BT_Status.stRxOk_b)
#define BT_Status_getStRxHeaderOk()   (BT_Status.stRxHeaderOk_b)
#define BT_Status_getStReceiving()    (BT_Status.stReceiving_b)
#define BT_Status_getStTxOk()         (BT_Status.stTxOk_b)
#define BT_Status_getAddrMode()       (BT_Status.stAddrMode_e)
#define BT_Status_getAddress()        (BT_Status.stAddress_u8)
#define BT_Status_getTesterAddress()  (BT_Status.stTesterAddr_u8)


/*
 *********************************************************************
 * Prototypes
 *********************************************************************
 */
/* Function reference */
extern void BT_TpRxmsg (uint8 msgObjIdex, uint8 dlc_u8, uint8* data_pu8 );

/* Function to be called after the transmission of the last message */
extern void BT_DoChanges(void);

/* Function to be called after Session Timeout */
extern void BT_SessionTimeout(void);

/* Function to set the P2 & P3 timings */
extern void   BT_SetTimings( void );

/* Function to set the Exec function which will be called after the transmission of the
   response */
extern void BT_ExecFunction(void (*fct)(void));

/* Declare routine active/inactive */
extern void  BT_SetRoutine(bit32 dRoutId_b32, uint8 stMode_b8);

extern void  BT_SetSecLevel(uint8 dSecLevel_u8);
extern bool  BT_GetPerm(bit32 dProtection_b32);
extern void  BT_ComIni(void);

/* Function to be  called by the operating system */
/* Function for intialisation */
extern void BT_Init ( void );

/* function to intialise the configuration item of Al layer */
extern void BT_AlConfInit (void);

/* Get the address type of the active handle */
extern uint8 BT_GetAddrType(uint8 MsgObjIdx);
extern uint8 BT_GetMsgHandle(uint8 MsgObjIdx);
extern void BT_SetAddrMode(uint8 stNewMode_u8);
extern uint8 BT_GetAddrMode(void);
/* The functions should be called in the same order as mentioned below */
/* by the OS */
/* cyclic function to called for polling of the recevied message */
extern void BT_CanReceive_Proc ( void );
extern void BT_DoChanges_Proc ( void );

/* Function to be called to process the received CAN frames  */
extern void BT_Tp_Proc ( void );
/* application layer interface functions */
/* Process task call.*/
extern void BT_Uds_Proc (void);
#endif

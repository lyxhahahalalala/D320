


#ifndef _BT_PRIV_H
#define _BT_PRIV_H


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
 * Includes
 *********************************************************************
 */
// #include "bt.h"
#include "bt_priv.h"
#include "bt_uds.h"
#include "bt_pub.h"
#include "upgrade.h"

/*
 *********************************************************************
 * Defines
 *********************************************************************
 */

#define TP_TOBSMAX     25ul  /* Configure the BSmax timeout value in ms */
#define TP_TOCRMAX     150ul /* Configure the Crmax timeout value in ms */
#define TP_BSMAX       0     /* Configure the Block Size max value is 255 */
#define TP_STMIN       0     /* Configure the minimum seperation time in ms . Max value 255ms */
#define TP_NUMWAIT2FC  0     /* Configure the maximum number of waittosend FC to be sent*/

#define BT_MAX_PERIOD       1    /* Maximum time (microsec) between two calls of BT_Al_Proc */
#define BT_MAX_DATA_LENGTH  4095 /* The max length of data in telegram.  */

#define PADDING_TYPE       (0x00)
#define TP_ADDRESSOFFSET   (0x600)


/* Types of frames */
#define N_PCI_SF           (0x00)  /* PCI for Single frame */
#define N_PCI_FF           (0x10)  /* PCI for First frame */
#define N_PCI_CF           (0x20)  /* PCI for consecutive frame */
#define N_PCI_FC           (0x30)  /* PCI for Flow control frame */

/* Types of Flow control */
#define FC_CTS             (0x00)  /* Continue to send flow status */
#define FC_WAIT            (0x01)  /* Wait to send flow status */
#define FC_OVERFLOW        (0x02)  /* Overcflow status */


/* Definations for the application layer */
/* Defines for configuration of service distributor: */
#define BT_SERVICENOTFOUND_DB8             (0x00)   /* Service identifier not found                   */
#define BT_SERVICE_SUPPORTED_DB8           (MBIT0)  /* Service identifier exists in table             */
#define BT_SERVICE_SUBFUNCNOTSUPPORTED_DB8 (MBIT1)  /* Service identifier exists, but FirstPara not match */
#define BT_DIAMODE_OK_DB8                  (MBIT2)  /* Service is supported in current diag. mode     */
#define BT_ADDRMODE_NOK_DB8                (MBIT3)  /* Service is not supported in current address mode     */

#define POS_RESPONSE_ADDER                    ((uint8_t)0x40)
#define NEG_RESPONSE_CODE                     ((uint8_t)0x7F)
#define SERVICE_NOT_SUPPORTED_CODE            ((uint8_t)0x11)
#define SERVICE_NOT_SUPPORTED_INACTIVESESSION ((uint8_t)0x7F)
#define SUBFUNC_NOT_SUPPORTED_CODE            ((uint8_t)0x12)
#define WAIT_PEND_CODE                        (0x78)
#define BUSY_REPEAT_CODE                      (0x21)

#define BT_RESETCHGREQ      0x00
#define BT_TIMCHGREQ        0x01
#define BT_EXECFUNCREQ      0x02

/*
 *********************************************************************
 * Type definition
 *********************************************************************
 */

/* Different Status of the network layer */
typedef enum
{
    TP_ST_IDLE,        /* Idle state */
    TP_ST_WAITFC,      /* Wait for Flow control */      /* TX IN PROG */
    TP_ST_WAITCF,      /* Wait for consequtive frame */ /* RX IN PROG */
    TP_ST_SENDFC,      /* Send flow control */          /* RX IN PROG */
    TP_ST_SENDCF,      /* Send consequtive frames */    /* TX IN PROG */
    TP_ST_SENDFC_OVR,  /* Send over flow control */     /* RX IN PROG */
} BT_StatusNl_e;

/* NL buffer State */
typedef enum
{
    TP_STBUFF_IDLE,   /* Buffer is in the idle state */
    TP_STBUFF_AL,     /* Buffer is locked by the application layer */
    TP_STBUFF_NL,     /* Buffer is locked by the network layer */
} BT_StatusNlBuf_e;

/* Types of Result */
typedef enum
{
    TP_OK,            /* Network layer transmission/Reception is O.K */
    TP_TIMEOUT_A,     /* Frame transmission timeout */
    TP_TIMEOUT_Bs,    /* Sender side timeout */
    TP_TIMEOUT_Cr,    /* Receiver side timeout */
    TP_WRONG_SN,      /* Wrong Sequence number */
    TP_INVALID_FS,    /* Invalid FS */
    TP_UNEXP_PDU,     /* Unexpected frame */
    TP_WFT_OVRN,      /* Wait frame over run */
    TP_BUFFER_OVFLW,  /* Buffer overflow */
    TP_WRONG_DLC,     /* Wrong DLC */
    TP_ERROR,         /* Unknown Error */
} BT_ReturnStatusNl_e;

/* Type definition for the application layer  */

/* Types of Responses */
typedef enum
{
    NO_RESPONSE_AGAIN = 0x00, /*再次无响应*/
    NO_RESPONSE,              /*无响应*/
    RESPONSE_OK,              /*响应成功*/
    RESPONSE_PENDING,         /*响应等待*/
    RESPONSE_WAITPEND,        /*响应等待-待定*/
    RESPONSE_WAITBUSYREPEAT,  /*响应等待-忙重复*/
} BT_RESPONSE_e;

/* Different Al states */
typedef enum
{
    BT_STAL_IDLE,     /* Idle state */
    BT_STAL_RX,       /* Receive state */
    BT_STAL_RXINPROG, /* Receive state */
    BT_STAL_RXPEND,
    BT_STAL_TX,       /* Transmit state */
    BT_STAL_TXINPROG, /* Transmit in Progress */
    BT_STAL_WTP2MIN,  /* wait for P2Min state */
} BT_StatusAL_e;

/*
 *********************************************************************
 * Variables
 *********************************************************************
 */

/* Defination of the timing variables in ticks */
/* Bsmax timings in ticks */
//extern uint32_t BT_TpTiming_toBsmax_u32;
#define BT_TpTiming_toBsmax_u32 (TP_TOBSMAX)

/* Crmax timings in ticks */
//extern uint32_t BT_TpTimingCfg_toCrmax_u32;
#define BT_TpTimingCfg_toCrmax_u32 (TP_TOCRMAX)

#define BT_TpBlockSizeCfg_u8 TP_BSMAX

#define BT_TpStminCfg_u8  TP_STMIN
#define BT_TpWfc_u8       TP_NUMWAIT2FC




extern uint32_t BT_toAlTxTim_u32;    /* P3 timer */
extern uint32_t BT_toAlRxTim_u32;    /* P2 timer */
extern uint32_t BT_toS3Tim_u32;      /* S3 timer */

extern uint8_t  BT_stChgReq_u8;      /* Byte Register to store the change requests */
extern uint8_t* BT_dAlBuf_pu8;       /* General definitions */
extern uint8_t  BT_statusAl_u8;      /* Status Register */
extern bool     BT_stAlWaitmode_b;   /* Wait mode active */
extern uint8_t  BT_stAlSerResp_u8;   /* Response status */
extern uint16_t BT_numAlTxBytes_u16; /* Number of bytes to be transmitted */

// extern BT_tpMsgCtrl_t  BT_tpMsgCtrl_s;
extern BT_tpMsgCtrl_t* BT_tpMsgCtrl_ps;
extern Can_PduType     BT_TxPdu;
extern uint8_t         BT_TxSt;
extern uint8_t         BT_RestartFlg;

/*
 *********************************************************************
 * Prototypes
 *********************************************************************
 */

extern void (*BT_reqFunction_pf)(void);

extern bool BT_TpTxmsg (uint8_t* databuff_pu8, uint16_t  datalength_u16 );

extern void BT_TpFFInd ( void );
/* Indication */
extern void BT_TpInd (uint8_t Nl_handle_u8, uint16_t  dataLength_u8, uint16_t Nl_Result_u16);

/* Confirmation */
extern void BT_TpConfirm ( uint16_t Nl_Result_u16 );

/* Setting of the buffer */
extern void BT_TpSetBuffer( uint8_t* data_pu8, uint16_t bufferlength_u16 );

/* Release the CAN buffer (i.e) change to idle state */
extern void BT_TpRelBuf (void);

/* funtion to intialise the network layer */
extern void  BT_TpInit( void);

/* funtion to intialise the application layer */
extern void BT_AlInit ( void );

/* application layer service distributor funtion */
extern void BT_AlServicedist (void );

/* To copy the len data from the src pointer to the destination pointer */
extern void BT_CopyData(uint8_t* src_pu8, uint8_t* dest_pu8, uint8_t len_u8);



#endif

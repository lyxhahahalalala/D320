
/*
 *********************************************************************
 * Includes
 *********************************************************************
 */
#include "common.h"
#include "BT_priv.h"
#include "Bt_uds.h"
#include "types.h"

/*
 *********************************************************************
 * Variables
 *********************************************************************
 */

/* Definition of the RAM structure */
BT_tpMsgCtrl_t BT_tpMsgCtrl_s;


/**
 *********************************************************************
 * Funtion to intialise the network layer.
 *
 * This function intialise the ram structure BT_tpMsgCtrl_s
 *
 * \param
 * \param
 * \return
 * \retval
 * \retval
 * \seealso
 * \usedresources 0x3a (= 58 ) bytes of Code area.
 *********************************************************************
 */

void  BT_TpInit( void)
{
    /* Setting of the Message RAM structure */
    BT_tpMsgCtrl_ps = &BT_tpMsgCtrl_s;
    BT_tpMsgCtrl_ps->bufferindex_u16 = 0;
    BT_tpMsgCtrl_ps->wfcCtr_u8 = 0;
    BT_tpMsgCtrl_ps->seqCtr_u8 = 0;
    BT_tpMsgCtrl_ps->BSmaxCtr_u8 = 0;  /* BSmax Counter */
    BT_tpMsgCtrl_ps->tp_state_u8 = TP_ST_IDLE;
    /* Buffer pointer is initalised by the Al_Dlite_Init function */
    /* Number of handles */
    /* Change the bufferstate to idle */
    BT_tpMsgCtrl_ps->bufferstate_u8 = TP_STBUFF_IDLE;
}


/**
 *********************************************************************
 * Funtion to intialise the application layer.
 *
 * detailed description of function
 *
 * \param
 * \param
 * \return
 * \retval
 * \retval
 * \seealso
 * \usedresources 0x82 (= 130 ) bytes of Code area.
 *********************************************************************
 */

void BT_AlInit ( void )
{
    /* Initialization of the buffer */
    BT_dAlBuf_pu8 = &BT_dataBuffer_au8[BT_ADDITIONAL_PLACE];

    /* Set the buffer pointer and the buffer length */
    BT_TpSetBuffer(BT_dAlBuf_pu8, BT_MAXREQLEN(BT_MAX_DATA_LENGTH));

    /* Reset the status to idle */
    BT_statusAl_u8 = BT_STAL_IDLE;


    /* Reset the wait mode */
    BT_stAlWaitmode_b = FALSE;

    /* Reset the response pending type */
    BT_stAlSerResp_u8 = NO_RESPONSE;

    /* Reset the number of bytes to transmit */
    BT_numAlTxBytes_u16 = 0;

    /* Set the dlite_dochanges buffer to NULL */
    BT_reqFunction_pf = NULL;

    /* Reset the dlite_dochanges register */
    BT_stChgReq_u8 = BT_RESETCHGREQ;  // Reset the change request to zero
    BT_stCallDoChg_u8  = BT_stChgReq_u8; // Reset the change request to zero

    /* Reset the timers */
    BT_toAlRxTim_u32 = getCurrentTime;
    BT_toAlTxTim_u32 = getCurrentTime;
    BT_toS3Tim_u32 = getCurrentTime;

    /* reset the P3 max timers */
    BT_AlStateDescriptor_setP3max(BT_DefaultAlTiming_KWP[P3MAX]);

    /* reset S3 timer */
    BT_AlStateDescriptor_setS3max(BT_DefaultAlTiming_KWP[S3MAX]);

    if (ProgRequest)
    {
        /* set current as default session */
        BT_AlStateDescriptor_setDiagMode(BT_PROGRAM_SESSION_DU8);

    }
    else
    {
        /* set current as default session */
        BT_AlStateDescriptor_setDiagMode(BT_DEFAULT_SESSION_DU8);
    }

    /* Default security level */
    BT_AlStateDescriptor_setSecLevel(BT_DEFAULT_SECLEV_DU8);
}

/**
 *********************************************************************
 * function to intialize the dlite modules.
 *
 * detailed description of function
 *
 * \param
 * \param
 * \return
 * \retval
 * \retval
 * \seealso
 * \usedresources 0x6 (= 6 ) bytes of Code area.
 *********************************************************************
 */

void BT_Init ( void )
{
    /* Intialize the network layer */
    BT_TpInit();
    /* Intialize the configuration items of Nl layer */
    BT_AlConfInit();
    /* Intialize the application layer */
    BT_AlInit();

    BT_TxPdu.id = 0x18DAF117;
    BT_TxPdu.length = 8;
    BT_TxPdu.swPduHandle = 0;
    BT_TxPdu.sdu = NULL;

    eEraseActive = Erase_Deactv;

}



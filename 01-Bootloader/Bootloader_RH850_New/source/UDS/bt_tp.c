
/*
 *********************************************************************
 * Includes
 *********************************************************************
 */
#include "common.h"
#include "bt_priv.h"
#include "upgrade.h"
#include "bt_uds.h"
#include "dev_api.h"
#include "string.h"
/*
 *********************************************************************
 * used functions
 *********************************************************************
 */
/**************************************************************************/

/*
 *********************************************************************
 * Variables
 *********************************************************************
 */

const BT_WorkConf_t BT_WorkConf_cs[] =
{
    {BT_TP_PHYS, CanTp_Rx_0x18DA17F1,     ADRM(PHYS)},
    {BT_TP_FUNC, CanTp_Rx_0x18DA17FF,     ADRM(FUNC)},
};

uint8_t BT_numAlHandles_u8 = sizeof(BT_WorkConf_cs) / sizeof(BT_WorkConf_t);

uint8_t BT_stAddrMode_u8;   /* Current active address mode */


BT_tpMsgCtrl_t* BT_tpMsgCtrl_ps;
Can_PduType     BT_TxPdu;
uint8_t         BT_TxSt = 0;
uint8_t         BT_RestartFlg = 0;

void (* BT_reqFunction_pf)(void);


/*
 *********************************************************************
 * Variables
 *********************************************************************
 */
/* Array to store the requested changed timings */
static uint32_t  BT_dReqTim_au8[NUMTIMINGS]; //数组来存储所请求更改的计时

/* Mirror image of BT_stChgReq_u8 this is set only after
   the transmission of the last message frame of the response
   and is cleared in dlite_dochanges */
uint8_t  BT_stCallDoChg_u8;

#pragma ghs section text = ".ramcode"

/**
 *********************************************************************
 * This process should be called from progos.
 *
 * This function checks if any BT do change is neccessary.
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
void BT_DoChanges_Proc(void) //检查是否有任何BT做改变是必要的。
{
    /* Check if the service has requested to do some changes */
    if (BT_stCallDoChg_u8 != 0) //检查服务是否请求进行一些更改
    {
        /* Check if the previous frame has been transmitted succesfully */
        if (BT_TxSt == 1 || BT_RestartFlg == 1) //检查上一帧是否传输成功
        {
			//由服务调用以设置计时
            BT_DoChanges(); //在 BT_DoChanges 内部，在服务进行必要的更改后，变量 BT_stCallDoChg_u8 将被重置
            /* Inside BT_DoChanges after making the required changes by the service
               the variable BT_stCallDoChg_u8 will be resetted */
        }
    }
}

/**
 *********************************************************************
 * Function to be called by the service to set the timings
 *
 *
 *
 * \param void
 * \param
 * \return
 * \retval
 * \retval
 * \seealso
 * \usedresources
 *********************************************************************
 */
void BT_SetTimings(void)
{
    BT_stChgReq_u8 |= BT_TIMCHGREQ;
}

/**
 *********************************************************************
 * Function to be called by the service to set the function pointer, which
 * will be called after the transmission of the response.
 *
 *
 * \param function pointer of type void xyy ( void )
 * \param
 * \return
 * \retval
 * \retval
 * \seealso
 * \usedresources
 *********************************************************************
 */
void BT_ExecFunction(void (*fct)(void))
{
    BT_reqFunction_pf = fct;
    /* store given function to be executed */
    BT_stChgReq_u8 |= BT_EXECFUNCREQ;
    /* Set bit: execute function after transmission of response */
	
    return;
}

/**
 *********************************************************************
 * Function to be called by the service to set the timings
 *
 * Function to be called after the transmission of the last message
 *
 * \param void
 * \param
 * \return
 * \retval
 * \retval
 * \seealso
 * \usedresources
 *********************************************************************
 */
void BT_DoChanges(void)
{
	uint32_t setReflashMarker = 0xCAFEBEE0;
    /* There are communication parameters to be changed ? */
    if (BT_stChgReq_u8 > 0)
    {
        /* Timing parameters shall be changed */
        if  ((BT_stChgReq_u8 & BT_TIMCHGREQ) > 0 )
        {
            /* Take over the requested timing values to the actual timing values and */
            BT_AlStateDescriptor_setP2min(BT_dReqTim_au8[P2MIN]);
            BT_AlStateDescriptor_setP2max(BT_dReqTim_au8[P2MAX]);
            BT_AlStateDescriptor_setP3min(BT_dReqTim_au8[P3MIN]);
            BT_AlStateDescriptor_setP3max(BT_dReqTim_au8[P3MAX]);
            /* reset timing change flag. */
            BT_stChgReq_u8 &= (uint8_t)(~BT_TIMCHGREQ);
        }
        /* Execute the given function */
        if ((BT_stChgReq_u8 & BT_EXECFUNCREQ) != 0 )
        {
            /* reset function flag */
            BT_stChgReq_u8 &= (uint8_t)(~BT_EXECFUNCREQ);
            if (BT_reqFunction_pf != NULL)
            {
				memcpy((uint32_t *)ReflashMarker, &setReflashMarker, 4);
                BT_reqFunction_pf(); //software reset
				
            }
            /*set function pointer to NULL. */
            BT_reqFunction_pf    = NULL;
        }
        BT_stChgReq_u8 = BT_RESETCHGREQ;     // Reset the change request to zero
        BT_stCallDoChg_u8  = BT_stChgReq_u8; // Reset the change request to zero
    }
    else
    {
        /* no changes to do */
    }
    return;
}

/**
 *********************************************************************
 * Function to be called when S3 timeout
 *
 *
 *
 * \param void
 * \param
 * \return
 * \retval
 * \retval
 * \seealso
 * \usedresources
 *********************************************************************
 */
void BT_SessionTimeout(void)
{
    /* Reset when S3 Timeout */
	
	// uint32_t setReflashMarker = 0xCAFEBEE8;
	// memcpy((uint32_t *)ReflashMarker, &setReflashMarker , 4);
	// for(uint32_t i = 0; i < 500000; i++); //等待发送完成
    // R_DEV_WRITE_PROT_REG(PROTCMD0, 0xFFF80A04, 0x01u);	/*SWRESA*/
}

/**
 *********************************************************************
 * Function : This function is called by the application layer to transmit
 *            the responses.
 *
 * The function transmits the response identified by the network handle, the
 * pointer to the buffer and the data length to be transmitted. Till the
 * transmission of the request is completed or failed the buffer is locked
 * by the network layer.
 * The network layer shall signals the completion of the messag transmission
 * to the service user by means of the function BT_TpConfirm
 *
 * \param   databuff_pu8:Pointer to the buffer from where the data needs to transmitted
 * \param   datalength_u8: The number of bytes to be transmitted.
 * \retval  The function returns TRUE ( non zero val )when FF or SF is transmitted succesfully
 * \seealso  Refer to section 6.2.1 N_USData.request of IS0 15765-2 document.
 *
 * \usedresources 0x12e (= 302 ) bytes of Code area.
 *********************************************************************
 */
bool BT_TpTxmsg(uint8_t* databuff_pu8, uint16_t  datalength_u16 )
{
    uint8_t* buffer_pu8;
    uint8_t  buffer_au8[8];
    uint8_t  idx_u8;
    bool     retst_b;
    uint8_t  candataLen_u8;

    retst_b = (bool)(FALSE);
    /* Transmission of  a new message is allowed only when the state is in IDLE */
    if (BT_tpMsgCtrl_ps->tp_state_u8 == TP_ST_IDLE) //只有网络层处于空闲状态，才被允许传输
    {
        /* Buffer is intialised with zeros for zero padding */
        for (idx_u8 = 0; idx_u8 < 8; idx_u8++)
        {
            buffer_au8[idx_u8] = PADDING_TYPE; //本地缓冲区初始化为零填充
        }
        /* reset the buffer pointer  */
        buffer_pu8 = buffer_au8; //初始化本地缓存区指针

        /* Setting of the Message RAM structure */
        BT_tpMsgCtrl_ps->buffer_pu8 = databuff_pu8; //把数据存储到网络层
        /* lock the buffer by the network layer */
        BT_tpMsgCtrl_ps->bufferstate_u8 = TP_STBUFF_NL;  //通过网络层锁定缓冲区
        BT_tpMsgCtrl_ps->datatoRxTx_u16 = datalength_u16; //需要传输的数据长度
        /* Intialise the Message control structure to transmit the new message */
        /*初始化Message控制结构以传输新消息*/
		BT_tpMsgCtrl_ps->bufferindex_u16 = 0;
        BT_tpMsgCtrl_ps->wfcCtr_u8 = 0;
        BT_tpMsgCtrl_ps->seqCtr_u8 = 1;   /* Sequence counter must start from 1 */
        BT_tpMsgCtrl_ps->BSmaxCtr_u8 = 0; /* Bsmax Counter */

        /* Active Network layer handle */
        candataLen_u8 = 7;

        /* Check message to be transmitted is greater than the maximum can data length */
        if (BT_tpMsgCtrl_ps->datatoRxTx_u16 > candataLen_u8) //检查要传输的消息是否大于最大可以数据长度
        {
            /* Multiple frame transmission */ /*多帧传输的第一帧*/
            *buffer_pu8++ = N_PCI_FF | ((datalength_u16 >> 8) & 0x0F); //buffer_pu8[0] = 0x10 | ((datalength_u16 >> 8) & 0x0F);
            *buffer_pu8++ = (uint8_t)(datalength_u16 & 0xff);          //buffer_pu8[1] = (uint8_t)(datalength_u16 & 0xff);

            BT_tpMsgCtrl_ps->tp_state_u8 = TP_ST_WAITFC;
            BT_tpMsgCtrl_ps->timer_u32 = getCurrentTime + BT_TpTiming_toBsmax_u32;
            /* In the first frame the only 5 or 6 data bytes could be sent */
            candataLen_u8--;
            BT_TxPdu.length = candataLen_u8 + 2; //BT_TxPdu.length = 6 + 2
        }
        else
        {
            /* Single frame transmission */ /*单帧传输*/
            *buffer_pu8++ = (uint8_t)(N_PCI_SF | datalength_u16); //buffer_pu8[0] = (uint8_t)(0x00 | datalength_u16);
            candataLen_u8 = (uint8_t)datalength_u16;
            BT_TxPdu.length = candataLen_u8 + 1;
        }
		/*把网络层缓存区数据，复制到本地缓存区，复制长度：若为多帧传输则为6，若为单帧传输则为7*/
        BT_CopyData(BT_tpMsgCtrl_ps->buffer_pu8, buffer_pu8, candataLen_u8);
		/*下一帧的缓存数据序号*/
        BT_tpMsgCtrl_ps->bufferindex_u16 = BT_tpMsgCtrl_ps->bufferindex_u16 + candataLen_u8;
		
        /* Call the can transmit function */
        BT_TxPdu.sdu = buffer_au8;
        if (Can_Write(CanTp_Tx_0x18DAF117, &BT_TxPdu) == CAN_OK)
        {
            /* Frame configured successfully for transmission */
            if (BT_tpMsgCtrl_ps->datatoRxTx_u16 <= candataLen_u8) //检查要传输的消息是否小于等于最大可以数据长度
            {
                /* Transmitted frame is single frame give a confirmation of the transmission */
                BT_TpConfirm(TP_OK); //传输帧是对单帧的传输进行确认
            }
            /* Return frame successfully configured */
            retst_b = (bool)(TRUE);
        }
        else
        {
            /* Frame not configured successfully */
            /* Rechange the state to idle */
            BT_tpMsgCtrl_ps->tp_state_u8 = TP_ST_IDLE;
        }

    }
    BT_TxSt = 0;

    return (retst_b);
}

#pragma ghs section text = ".ramcode"

/**
 *********************************************************************
 * Function to receive the Network layer CAN frames and analyze the frame type.
 *
 * detailed description of function
 *
 * \param  dlc_u8       :data length of the message.
 * \param  data_pu8     :pointer to the data buffer.
 * \return
 * \retval
 * \retval
 * \seealso Tp_rxmsg implementation of EDC 16
 * \usedresources 0x438 (= 1080 ) bytes of Code area.
 *********************************************************************
 */
void BT_TpRxmsg(uint8_t msgObjIdex, uint8_t dlc_u8, uint8_t* data_pu8 )
{
    uint8_t* buffer_pu8;
    uint8_t buffer_au8[8];
    uint16_t idx_u16;
    uint16_t datalenFF_u16;
    uint8_t address_u8;
    uint8_t Nl_handle_u8;
    uint8_t numPciTyp_u8;
    uint8_t numPcilownib_u8;
    uint8_t dataLen_u8;
    buffer_pu8 = buffer_au8;

    for ( idx_u16 = 0; idx_u16 < dlc_u8; idx_u16++)
    {
		/* Copy the data from the CAN buffer to the local buffer to 
		avoid overwritting of the buffer by the CAN driver software*/
        // 将CAN缓冲区中的数据复制到本地缓冲区，以避免CAN驱动软件覆盖该缓冲区
		buffer_au8[idx_u16] = *(data_pu8 ++);
    }

    /* Check if buffer is locked by the application */
    if (BT_tpMsgCtrl_ps->bufferstate_u8 == TP_STBUFF_AL) //检查缓冲区是否被应用程序锁定
    {
        return;
    }
    else
    {
        /* lock the buffer by the network layer */
        BT_tpMsgCtrl_ps->bufferstate_u8 = TP_STBUFF_NL; //通过网络层锁定缓冲区
    }

    if ((BT_tpMsgCtrl_ps->tp_state_u8 == TP_ST_SENDCF)) //判断网络层状态
    {
        /* Transmit in progress ignore the received frame */
        /* Refer Handling of unexpected frames in document ISO 15765-2 */
        return;
    }
    else
    {
        dataLen_u8 = 7;
    }

    /*  PCI information to decide the type of the frame*/
    numPciTyp_u8 = (*buffer_pu8) & (0xf0);        //接收帧类型
    numPcilownib_u8  = (*buffer_pu8++) & (0x0f);  //若为单帧，则为接收帧有效数据长度（字节数），若为连续帧，则为接收帧有效数据长度的高字节

    if (BT_tpMsgCtrl_ps->tp_state_u8 == TP_ST_WAITFC ) //判断网络层状态
    {
        if (numPciTyp_u8 != N_PCI_FC) //判断接收帧类型是否为 PCI流控制帧
        {
            return;/* Transmit in progress ignore the unexpected frame */
        }
    }
    Nl_handle_u8 = BT_GetMsgHandle(msgObjIdex);  //获取接收报文ID的映射
    address_u8 = BT_GetAddrType(msgObjIdex);     //获取寻址模式
    if (address_u8 == ADRM(FUNC))  //判断寻址模式是否为功能寻址
    {
        if ( numPciTyp_u8 != N_PCI_SF ) //判断接收帧类型是否为 PCI单帧
        {
            /* ignore the frame */
            return;
        }
    }
    /* The type of frame received is analysed */
    switch (numPciTyp_u8)
    {
    /* Single frame received */
    case N_PCI_SF:
        /* Check for valid data length */
        if ((numPcilownib_u8 == 0) || (numPcilownib_u8 > dataLen_u8)) //判断接收帧是否为有效帧，若不是，则不在往下执行
        {
            /* Invalid data frame ignore */
            return;
        }
        /* check if the nl is in the idle state */
        if (BT_tpMsgCtrl_ps->tp_state_u8 != TP_ST_IDLE) //判断网络层状态
        {
            if (address_u8 == ADRM(PHYS))  //判断寻址模式是否为物理寻址
            {
                /* Reception in progress, terminate the reception and start receiving the new frame */
                /* Refer to Table 17 Handling of unexpected arrival of a network layer N_PDU of ISO 15765-2 doc */
                BT_TpInd(Nl_handle_u8, BT_tpMsgCtrl_ps->bufferindex_u16, TP_UNEXP_PDU );
            }
            else
            {
                /*If the N_AI of the received N_PDU is different from the segmented message,
                  an on-going reception/transmission shall be continued */
                /* Refer to ISO15765-2 : 2011 Chapter 8.7.3 */
                /* Ignore the frame */
                return;
            }
        }
        /* lock the buffer to the Network layer */
        BT_tpMsgCtrl_ps->bufferstate_u8 = TP_STBUFF_NL; //将缓冲区状态被所定
        /* Process the Single frame */
        BT_tpMsgCtrl_ps->bufferindex_u16 = 0;

        /* Copy the contents from the local buffer to the main buffer */
        for (idx_u16 = 0; idx_u16 < numPcilownib_u8; idx_u16++)
        {
            *(&BT_tpMsgCtrl_ps->buffer_pu8[BT_tpMsgCtrl_ps->bufferindex_u16]) = *buffer_pu8++;
            BT_tpMsgCtrl_ps->bufferindex_u16++;
        }

        BT_TpInd(Nl_handle_u8, numPcilownib_u8, TP_OK);
        break;

    /* First frame handling */
    case N_PCI_FF:
		/* Get the valid data length */
        datalenFF_u16 = (numPcilownib_u8 << 8) + *buffer_pu8++;  //获取接收帧有效数据长度
        /* Check if the data length of the first frame is a valid data length */
        /* datalenFF_u16 must have value greater the 7 ( without Ext )  to recognise
         it has valid frame */
        /* Refer ISO15765-2 First frame data length error handling section 7.4.2.2 */

        if (datalenFF_u16 <= (uint16_t) dataLen_u8) 
        {
            return ;
        }

        /* Actual data bytes in first frame is either 6 or 5 */
        dataLen_u8--;
        if (BT_tpMsgCtrl_ps->tp_state_u8 != TP_ST_IDLE)  //判断网络层状态
        {
            /* Terminate the current reception and start receving the new frame */
            /* Refer to Table 17 Handling of unexpected arrival of a network layer N_PDU of ISO 15765-2 doc */
            BT_TpInd(Nl_handle_u8, BT_tpMsgCtrl_ps->bufferindex_u16, TP_UNEXP_PDU );
        }
        /* Process the First frame */
        /* lock the buffer to the Network layer */
        BT_tpMsgCtrl_ps->bufferstate_u8 = TP_STBUFF_NL;
		
        BT_tpMsgCtrl_ps->datatoRxTx_u16 = datalenFF_u16;  //需要接收的数据长度
        BT_tpMsgCtrl_ps->bufferindex_u16 = 0;

        /* Reset the WFC counter */
        BT_tpMsgCtrl_ps->wfcCtr_u8 = 0;
        if (BT_tpMsgCtrl_ps->datatoRxTx_u16 <= BT_tpMsgCtrl_ps->bufferlength_u16) //接收帧有效数据长度是否小于等于实际分配数据缓存长度（数据缓存长度 = 4095 + 6 = 4101）
        {
            for (idx_u16 = 0; idx_u16 < dataLen_u8; idx_u16++)
            {
                *(&BT_tpMsgCtrl_ps->buffer_pu8[BT_tpMsgCtrl_ps->bufferindex_u16]) = *buffer_pu8++; //接收帧有效数据锁定到网络层
                BT_tpMsgCtrl_ps->bufferindex_u16++;
            }

            /* The first expected sequence number */
            BT_tpMsgCtrl_ps->seqCtr_u8 = 1;
            /* Give an indication that the first frame is received */
            BT_TpFFInd(); //如果此时是空闲状态切换为正在接收。
            BT_tpMsgCtrl_ps->tp_state_u8 = TP_ST_SENDFC;
            /* Transmit the FC immediately */
            BT_tpMsgCtrl_ps->timer_u32 = getCurrentTime; //当前时间锁定到网络层 
        }
        else
        {
            /* Data to receive is more than the actual buffer allocated.  
			Send flow control frame with overflow status*/
            BT_tpMsgCtrl_ps->tp_state_u8 = TP_ST_SENDFC_OVR; //接收的数据大于实际分配的缓冲区。发送流控制帧溢出状态
        }
        break;

    /* Flow control frame is received */
    case N_PCI_FC:
        /* Process the FC only if it is awaited */
        if (BT_tpMsgCtrl_ps->tp_state_u8 == TP_ST_WAITFC) //判断网络层状态
        {
            if (numPcilownib_u8 == FC_OVERFLOW) //
            {
                BT_tpMsgCtrl_ps->tp_state_u8 = TP_ST_IDLE;
                /* overflow frame received abort the transmission */
                /* Refer to Table 12  Definition of FlowStatus of ISO 15765-2 doc*/
                BT_TpConfirm (TP_BUFFER_OVFLW);
            }
            else
            {
                if (numPcilownib_u8 == FC_WAIT)
                {
                    /* Wait the next flow control message */
                    /* Refer to section 7.5 and 7.6.3 of ISO15765-2 */
                    BT_tpMsgCtrl_ps->timer_u32 = getCurrentTime + BT_TpTiming_toBsmax_u32;
                }
                else
                {
                    if (numPcilownib_u8 == FC_CTS)
                    {
                        /* Transmit the consequtive frame immediately */
                        /* load with the current system time */
                        BT_tpMsgCtrl_ps->timer_u32 = getCurrentTime;
                        BT_tpMsgCtrl_ps->tp_state_u8 = TP_ST_SENDCF;
                        BT_tpMsgCtrl_ps->BSmax_u8 = BT_tpMsgCtrl_ps->BSmaxCtr_u8 = *buffer_pu8++;
                        BT_tpMsgCtrl_ps->stmin_u8 = *buffer_pu8;
                    }
                    else
                    {
                        /* unexpected type of FC  is ignored*/
                        return;
                    }
                }
            }
        }
        break;

    /*  Consequtive Frame handling */
    case N_PCI_CF:
        /* Process the CF only if it is awaited */
        if (BT_tpMsgCtrl_ps->tp_state_u8 == TP_ST_WAITCF) //判断网络层状态
        {
            if (numPcilownib_u8 == BT_tpMsgCtrl_ps->seqCtr_u8)
            {
                BT_tpMsgCtrl_ps->seqCtr_u8++;
                BT_tpMsgCtrl_ps->seqCtr_u8 = (BT_tpMsgCtrl_ps->seqCtr_u8 & 0x0F);
                /*check if the frame received is last frame or not */
                if (BT_tpMsgCtrl_ps->datatoRxTx_u16 <= ((BT_tpMsgCtrl_ps->bufferindex_u16) + dataLen_u8)) //检查收到是否为单位尺寸的是否是最后一帧
                {
                    /* Last Frame Received */
                    for ( idx_u16 = BT_tpMsgCtrl_ps->bufferindex_u16; idx_u16 < BT_tpMsgCtrl_ps->datatoRxTx_u16; idx_u16++)
                    {
                        *(&BT_tpMsgCtrl_ps->buffer_pu8[BT_tpMsgCtrl_ps->bufferindex_u16]) = *buffer_pu8++;
                        BT_tpMsgCtrl_ps->bufferindex_u16++;
                    }
                    BT_TpInd(Nl_handle_u8, BT_tpMsgCtrl_ps->bufferindex_u16, TP_OK );
                }
                else
                {
                    for (idx_u16 = 0; idx_u16 < dataLen_u8; idx_u16++)
                    {
                        *(&BT_tpMsgCtrl_ps->buffer_pu8[BT_tpMsgCtrl_ps->bufferindex_u16]) = *buffer_pu8++;
                        BT_tpMsgCtrl_ps->bufferindex_u16++;
                    }
                    BT_tpMsgCtrl_ps->timer_u32 = getCurrentTime + BT_TpTimingCfg_toCrmax_u32;
                    /* Monitor Bsmax value */
                    /* decrement Bsmax counter only when BSmax is not configured as zero */
                    if (BT_tpMsgCtrl_ps->BSmax_u8 != 0)
                    {
                        if (BT_tpMsgCtrl_ps->BSmaxCtr_u8 != 0)
                        {
                            BT_tpMsgCtrl_ps->BSmaxCtr_u8--;
                        }
                        if (BT_tpMsgCtrl_ps->BSmaxCtr_u8 == 0)
                        {
                            /* BsmaxCtr is zero switch the state to send flow control frame */
                            BT_tpMsgCtrl_ps->tp_state_u8 = TP_ST_SENDFC;
                            /* Reset the timer to send the flow control frame immediately */
                            BT_tpMsgCtrl_ps->timer_u32 = getCurrentTime;
                        }
                    }
                }
            }
            else
            {
                /* Wrong sequence number */
                /* Refer to section 7.4.3.2 Sequence number error handling of ISO 15765-2 doc */
                BT_TpInd(Nl_handle_u8, BT_tpMsgCtrl_ps->bufferindex_u16, TP_WRONG_SN );
            }
        }
        break;
    /* Unknown frame ignore */
    default:
        break;
    }
    /* End of switch statement */
    /* MISRA RULE 82 VIOLATION: Multiple returns in receive message makes sw efficient */
    return;
}

#pragma ghs section text = ".ramcode"

/**
 *********************************************************************
 * Funtion This function is called when the network layer receives FF.
 *
 * The function switches the state idle to receive in progress.
 *
 * \param Nl_length_u16:The length of to be received request
 * \return
 * \retval
 * \retval
 * \seealso Refer to section 6.2.3 N_USData_FF.indication of IS0 15765-2 document.
 * \usedresources Refer the map file.
 *********************************************************************
 */
void BT_TpFFInd(void)
{
    /* if in idle mode switch to the receive state */
    if ( BT_statusAl_u8 == BT_STAL_IDLE )
    {
        BT_statusAl_u8 = BT_STAL_RXINPROG;
    }
}

/**
 *********************************************************************
 * Funtion  This function is called by the network layer when it is
 * receiving mode .
 *
 * This function is called by the network layer, when the complete request
 * either single or segmented  is received or while receving,when there is a Cr timeout
 * or wait frame overrun is there or when unexpected first frame or single frame is received
 * or a frame with wrong sequence number is received.
 *
 * \param data_pu8     :The pointer onto which the data has been recieved.
 * \param dataLength_u8:The length of the received request.
 * \param Nl_Result_u16:The result of the received request.
 * \retval
 * \retval
 * \seealso  Refer to section 6.2.4 N_USData.indication of IS0 15765-2 document.
 * \usedresources  Refer the map file.
 *********************************************************************
 */
void BT_TpInd(uint8_t Nl_handle_u8, uint16_t  dataLength_u8, uint16_t Nl_Result_u16)
{
    uint8_t  idx_u8;
    bool     stSearch_b = FALSE;
    const BT_WorkConf_t*  BT_WorkConf_pcs;
	
    BT_WorkConf_pcs = BT_WorkConf_cs;
	
    if ( Nl_Result_u16 == TP_OK )
    {
        for (idx_u8 = 0; ((idx_u8 < (uint32_t)BT_numAlHandles_u8) && (stSearch_b == (bool)FALSE)); idx_u8++ )
        {
            if (BT_WorkConf_pcs->Nl_handle_u8 == Nl_handle_u8)
            {
                /* Set the default timings  after P3max timeout */
                /* protocol switching should take place only after P3max time expires otherwise updation
                    time will not work properly */
                /* P3MAX 时间超时后，设置默认时间 */
				/* 协议切换只能在 P3MAX 时间到期后进行，否则更新时间将无法正常工作 */
				if ( (BT_AlStateDescriptor_getP3max())  < (getCurrentTime - BT_toAlTxTim_u32))
                {
                    BT_AlStateDescriptor_setP2min(BT_DefaultAlTiming_KWP[P2MIN]);
                    BT_AlStateDescriptor_setP2max(BT_DefaultAlTiming_KWP[P2MAX]);
                    BT_AlStateDescriptor_setP3min(BT_DefaultAlTiming_KWP[P3MIN]);
                    BT_AlStateDescriptor_setP3max(BT_DefaultAlTiming_KWP[P3MAX]);
                    BT_AlStateDescriptor_setS3max(BT_DefaultAlTiming_KWP[S3MAX]);
                }
				
                /* Matching entry found therefore jump out of the for loop */
                stSearch_b = TRUE;
            }
            else
            {
                BT_WorkConf_pcs++;
            }
        }
        if (stSearch_b != (bool)FALSE)
        {
            /* Valid Configured message is received */
            /* lock the buffer by the application layer */
            BT_tpMsgCtrl_ps->bufferstate_u8 = TP_STBUFF_AL; //通过应用层锁定缓冲区
            /* load the recive timer with the current system time */
            BT_toAlRxTim_u32 = getCurrentTime;
            /* Reset the P3 timer */
            BT_toAlTxTim_u32 = getCurrentTime;
            /* Reset the S3 timer */
            BT_toS3Tim_u32 = getCurrentTime;
            /* Set the datalength of the message received including the SID */
            BT_Status_setNumUsed(dataLength_u8); //设置接收到的消息的数据长度，包括SID
            if ((BT_statusAl_u8 == BT_STAL_RXINPROG ) || (BT_statusAl_u8 == BT_STAL_IDLE ))
            {
                BT_statusAl_u8 = BT_STAL_RX;
            }
            BT_SetAddrMode(BT_WorkConf_pcs->addrmodes_u8);
        }

    }
    else if ((Nl_Result_u16 == TP_TIMEOUT_Cr) \
		  || (Nl_Result_u16 == TP_WFT_OVRN) \
	      || (Nl_Result_u16 == TP_UNEXP_PDU) \
		  || (Nl_Result_u16 == TP_WRONG_SN))
    {
        if (BT_statusAl_u8 == BT_STAL_RXINPROG )
        {
            BT_statusAl_u8 = BT_STAL_IDLE;
        }
    }
    else
    {
        /* Do nothing unrecognised indication */
    }
    BT_tpMsgCtrl_ps->tp_state_u8 = TP_ST_IDLE;
}

/**
 *********************************************************************
 * Funtion  This function is called by  the network layer.
 *
 *
 * This function confirms the completion of the reqest to transfer the
 * the response.
 *
 * \param Nl_Result_u16 : Provides the status of the message transmit request.
 * \return
 * \retval
 * \retval
 * \seealso     Refer to section 6.2.2 N_USData.confirm of IS0 15765-2 document.
 * \usedresources Refer the map file.
 *********************************************************************
 */
void BT_TpConfirm(uint16_t Nl_Result_u16)
{
    if (Nl_Result_u16 == TP_OK)
    {
        if ((BT_stAlSerResp_u8 == RESPONSE_PENDING ) || (BT_stAlSerResp_u8 == RESPONSE_WAITPEND))
        {
            /* Switch the state to receive */
            BT_statusAl_u8 = BT_STAL_RXPEND;
            BT_tpMsgCtrl_ps->bufferstate_u8 = TP_STBUFF_AL;
        }
        else
        {
            BT_statusAl_u8 = BT_STAL_IDLE;
            BT_tpMsgCtrl_ps->bufferstate_u8 = TP_STBUFF_IDLE;
            /* Response sent succesfully. Call DoChanges */
            BT_stCallDoChg_u8 = BT_stChgReq_u8; //成功发送响应，调用 BT_DoChanges
        }
    }
    else if ((Nl_Result_u16 == TP_TIMEOUT_Bs) || (Nl_Result_u16 == TP_BUFFER_OVFLW) )
    {
        /* The Al state will be in transmit in progress state this is changed
        to idle state so that the application layer is ready to receive the new frame */
        BT_statusAl_u8 = BT_STAL_IDLE;
    }
    else
    {
        /* Do nothing required to avoid MISRA warnings */
    }
    /* Time of the last transmitted response */
    BT_toAlTxTim_u32 = getCurrentTime; /* Start the P3 timer */
    BT_toS3Tim_u32   = getCurrentTime; /* Reset the S3 timer */
    BT_tpMsgCtrl_ps->tp_state_u8 = TP_ST_IDLE;
    /* Switch the buffer to the orginal receive buffer */
    /* Set the buffer pointer and the buffer length to the right location */
	BT_TpSetBuffer(BT_dAlBuf_pu8, BT_MAXREQLEN(BT_MAX_DATA_LENGTH)); // 将缓冲区指针和缓冲区长度设置到设置网络缓冲区
	
}
/**
 *********************************************************************
 * Funtion Set the network buffer and buffer length to the given inputs.
 *
 * detailed description of function
 *
 * \param  data_pu8 :   Pointer to the buffer.
 * \param  bufferlength_u16: length of the buffer.
 * \return void
 * \retval
 * \retval
 * \seealso
 * \usedresources 0x12 (= 18 ) bytes of Code area .
 *********************************************************************
 */
void BT_TpSetBuffer(uint8_t* data_pu8, uint16_t bufferlength_u16)
{
    if (BT_tpMsgCtrl_ps->bufferstate_u8 == TP_STBUFF_IDLE)
    {
        BT_tpMsgCtrl_ps->buffer_pu8 = data_pu8;
        BT_tpMsgCtrl_ps->bufferlength_u16 = bufferlength_u16;
    }
}
/**
 *********************************************************************
 * changes the buffer to the idle states.
 *
 * detailed description of function
 *
 * \param
 * \param
 * \return
 * \retval
 * \retval
 * \seealso
 * \usedresources 0xe (= 14 ) bytes of Code area.
 *********************************************************************
 */
void BT_TpRelBuf(void)
{
    BT_tpMsgCtrl_ps->bufferstate_u8 = TP_STBUFF_IDLE;
}

/**
 *********************************************************************
 * Funtion Copys len number of data from the src to destination.
 *
 * \param  src_pu8 : pointer from where the data needs to be copied.
 * \param  dest_pu8: pointer to where the data will be copied.
 * \return len_u8  : The number of bytes to be copied.
 * \retval
 * \retval
 * \seealso
 * \usedresources 0x12 (= 18 ) bytes of Code area.
 *********************************************************************
 */
void BT_CopyData(uint8_t* src_pu8, uint8_t* dest_pu8, uint8_t len_u8)
{
    while (len_u8 > 0)
    {
        *dest_pu8++ = *src_pu8++;
        len_u8--;
    }
}

/**
 *********************************************************************
 * Function to be called cyclically at the same rate as BT_CanReceive_Proc.
 *
 * This function triggers the transmission time related events such as
 * sending of flow control message, consecutive frames and also monitoring of timeouts.
 *
 * \param
 * \param
 * \return
 * \retval
 * \retval
 * \seealso Refer to table 16 and section 7.6.3 of ISO 15765-2 document.
 * \usedresources 0x2e4 (= 740 ) bytes of Code area.
 *********************************************************************
 */
void BT_Tp_Proc (void) /******************************/
{
    uint8_t* buffer_pu8;
    uint8_t buffer_au8[8];
    uint8_t numBytesTx_u8;
    uint8_t idx_u8;
    uint8_t dummy_u8 = 0;
	
    /* clearing the buffer contents */
    /* Buffer is intialised with zeros for zero padding */
    for (idx_u8 = 0; idx_u8 < 8; idx_u8++)
    {
        buffer_au8[idx_u8] = PADDING_TYPE;
    }
    /* Reintialise the bufferptr */
    buffer_pu8 = buffer_au8;

    if (((int32_t)(BT_tpMsgCtrl_ps->timer_u32 - getCurrentTime) <= (int32_t)(0x00) ) && (BT_tpMsgCtrl_ps->tp_state_u8 != TP_ST_IDLE))
    {
        switch (BT_tpMsgCtrl_ps->tp_state_u8)
        {
        case TP_ST_SENDFC:  /* Send flow control message */
            /* check if buffer is locked by the application */
            if (BT_tpMsgCtrl_ps->bufferstate_u8 == TP_STBUFF_AL)
            {
                /* Buffer is locked by the application send wait to send flow control message */
                if (BT_tpMsgCtrl_ps->wfcCtr_u8 >= BT_TpWfc_u8)
                {
                    BT_tpMsgCtrl_ps->tp_state_u8 = TP_ST_IDLE;
                    /* Refer to section 7.6.3 Wait frame error handling of ISO 15765-2 */
                    BT_TpInd(dummy_u8, BT_tpMsgCtrl_ps->bufferindex_u16, TP_WFT_OVRN );
                }
                else
                {
                    *buffer_pu8++ = N_PCI_FC | FC_WAIT; //0x31
					
                    BT_TxPdu.sdu = buffer_au8;
                    BT_TxPdu.length = 3;
                    if (Can_Write(CanTp_Tx_0x18DAF117, &BT_TxPdu) == CAN_OK) //BT_TxPdu.sdu[0] = 0x31
                    {
                        /* Frame configured successfully for transmission */
                        BT_tpMsgCtrl_ps->wfcCtr_u8++;
                        BT_tpMsgCtrl_ps->timer_u32 = getCurrentTime + (BT_TpTiming_toBsmax_u32 >> 1);
                    }
                    else
                    {
                        /* Frame not configured successfully for transmission */
                        /* Do nothing retry sending the frame again in next cycle */
                    }

                }
            }
            else
            {
                /* The buffer is locked by Network layer
                hence send FC( CTS ) at the earliest */
                *buffer_pu8++ = N_PCI_FC | FC_CTS; //0x30

                *buffer_pu8++ = BT_TpBlockSizeCfg_u8;
                *buffer_pu8 = BT_TpStminCfg_u8;
				
                BT_TxPdu.sdu = buffer_au8;
                BT_TxPdu.length = 3;
                if (Can_Write(CanTp_Tx_0x18DAF117, &BT_TxPdu) == CAN_OK) //BT_TxPdu.sdu[0] = 0x30
                {
                    /* Switch the state to Receive consequtive frames */
                    BT_tpMsgCtrl_ps->tp_state_u8 = TP_ST_WAITCF;
                    /* load the Bsmax value */
                    BT_tpMsgCtrl_ps->BSmax_u8    =  BT_TpBlockSizeCfg_u8;
                    /* load the BSmax counter */
                    BT_tpMsgCtrl_ps->BSmaxCtr_u8 = BT_TpBlockSizeCfg_u8;
                    BT_tpMsgCtrl_ps->timer_u32 = getCurrentTime + BT_TpTimingCfg_toCrmax_u32;
                }
                else
                {
                    /* Frame not configured successfully for transmission */
                    /* Do nothing, retry sending the frame again in next cycle */
                }
            }
            break;
        
		case TP_ST_SENDFC_OVR:

            *buffer_pu8++ = 0x30 | FC_OVERFLOW; //0x32
			
            BT_TxPdu.sdu = buffer_au8;
            BT_TxPdu.length = 3;
            if (Can_Write(CanTp_Tx_0x18DAF117, &BT_TxPdu) == CAN_OK) //BT_TxPdu.sdu[0] = 0x32
            {
                BT_tpMsgCtrl_ps->tp_state_u8 = TP_ST_IDLE;
            }
            else
            {
                /* Frame not successfully configured try to resend in next cycle */
				/* 帧配置不成功，请在下一个周期中重新发送 */
            }
            break;
        
		case TP_ST_SENDCF:  /* Send consequtive frames */
            if ((BT_tpMsgCtrl_ps->BSmax_u8 != 0) && (BT_tpMsgCtrl_ps->BSmaxCtr_u8 == 0))
            {
                /* Bsmax counter has reached zero, wait for the next flow control frame */
                BT_tpMsgCtrl_ps->tp_state_u8 = TP_ST_WAITFC;
                BT_tpMsgCtrl_ps->timer_u32 = getCurrentTime + BT_TpTiming_toBsmax_u32;
            }
            else
            {
                numBytesTx_u8 = 7;

                *buffer_pu8++ = N_PCI_CF | BT_tpMsgCtrl_ps->seqCtr_u8;  //0x20 + 
                /* check if last frame needs to be transmitted */
                if ( (BT_tpMsgCtrl_ps->bufferindex_u16 + numBytesTx_u8) >= BT_tpMsgCtrl_ps->datatoRxTx_u16) //检查最后一帧是否需要发送
                {
                    /* last frame transmitted */
                    numBytesTx_u8 = (uint8_t)(BT_tpMsgCtrl_ps->datatoRxTx_u16 - BT_tpMsgCtrl_ps->bufferindex_u16);
                    BT_CopyData(&BT_tpMsgCtrl_ps->buffer_pu8[BT_tpMsgCtrl_ps->bufferindex_u16], buffer_pu8, numBytesTx_u8);
					
                    BT_TxPdu.sdu = buffer_au8;
                    BT_TxPdu.length = numBytesTx_u8 + 1;
                    if (Can_Write(CanTp_Tx_0x18DAF117, &BT_TxPdu) == CAN_OK ) //BT_TxPdu.sdu[0] = 0x20 | seqCtr_u8 
                    {
                        BT_tpMsgCtrl_ps->bufferindex_u16 = BT_tpMsgCtrl_ps->bufferindex_u16 + numBytesTx_u8;
                        /* Give a confirmation that the last frame has been transmitted */
                        BT_TpConfirm(TP_OK); //请确认最后一帧已传输
                    }
                    else
                    {
                        /* Frame not successfully configured. Try to reconfigure in the next cycle */
						/* 帧配置不成功。试着在下一个周期中重新配置 */
                    }
                }
                else
                {
                    BT_CopyData(&BT_tpMsgCtrl_ps->buffer_pu8[BT_tpMsgCtrl_ps->bufferindex_u16], buffer_pu8, numBytesTx_u8);
					
                    BT_TxPdu.sdu = buffer_au8;
                    BT_TxPdu.length = numBytesTx_u8 + 1;
                    if (Can_Write(CanTp_Tx_0x18DAF117, &BT_TxPdu) == CAN_OK) //BT_TxPdu.sdu[0] = 0x20 | seqCtr_u8 
                    {
                        BT_tpMsgCtrl_ps->seqCtr_u8++;
                        BT_tpMsgCtrl_ps->seqCtr_u8 = (BT_tpMsgCtrl_ps->seqCtr_u8) & 0x0f;
                        BT_tpMsgCtrl_ps->bufferindex_u16 = BT_tpMsgCtrl_ps->bufferindex_u16 + numBytesTx_u8;
                        BT_tpMsgCtrl_ps->tp_state_u8 = TP_ST_SENDCF;
                        BT_tpMsgCtrl_ps->timer_u32 = getCurrentTime + BT_tpMsgCtrl_ps->stmin_u8;
                        if (BT_tpMsgCtrl_ps->BSmax_u8 != 0)
                        {
                            BT_tpMsgCtrl_ps->BSmaxCtr_u8--;
                        }
                    }
                    else
                    {
                        /* Frame not successfully configured. Try to reconfigure in the next cycle*/
                    }
                }
            }
            break;
		
        case TP_ST_WAITFC:  /* Flow control Bs not received */
            /* Refer to Table 16  Network layer timeout error handling of ISO 15765-2 doc*/
            BT_TpConfirm ( TP_TIMEOUT_Bs);
            break;
		
        case TP_ST_WAITCF:  /* Consequtive frame is not received */
            /* Refer to Table 16  Network layer timeout error handling of ISO 15765-2 doc*/
            BT_TpInd(dummy_u8, BT_tpMsgCtrl_ps->bufferindex_u16, TP_TIMEOUT_Cr );
            break;
		
        default:
            break;
        }
    }
}

/**
 **************************************************************************************************
 * Get the address type of the active handle
 *
 * \param  NL handle
 * \param
 * \return
 * \retval  uint8_t: addrmodes_u8
 * \retval
 * \seealso
 * \usedresources .
 **************************************************************************************************
 */
uint8_t BT_GetAddrType(uint8_t MsgObjIdx)
{
    uint idx_ui;
    const BT_WorkConf_t* workConf_pcs;
    workConf_pcs = BT_WorkConf_cs;

    for (idx_ui = 0; idx_ui < (uint32_t)BT_numAlHandles_u8; idx_ui++ )
    {
        if (workConf_pcs[idx_ui].RxMsgObjIdx == MsgObjIdx)
        {
            break;
        }
    }

    return ((uint8_t)(workConf_pcs[idx_ui].addrmodes_u8));

}

uint8_t BT_GetMsgHandle(uint8_t MsgObjIdx)
{
    uint idx_ui;
    const BT_WorkConf_t* workConf_pcs;
    workConf_pcs = BT_WorkConf_cs;
    for (idx_ui = 0; idx_ui < (uint32_t)BT_numAlHandles_u8; idx_ui++ )
    {
        if (workConf_pcs[idx_ui].RxMsgObjIdx == MsgObjIdx)
        {
            break;
        }
    }
	
    return ((uint8_t)(workConf_pcs[idx_ui].Nl_handle_u8));
	
}

/**
 **************************************************************************************************
 * Function to  Set diagnostic mode.
 *
 * detailed description of function
 *
 * \param   uint8_t: Diagnostic mode
 * \param
 * \return  none
 * \retval
 * \retval
 * \seealso
 * \usedresources
 **************************************************************************************************
 */
void BT_SetAddrMode(uint8_t stNewMode_u8)
{
    BT_stAddrMode_u8 = stNewMode_u8;
	
    return;
}

uint8_t BT_GetAddrMode(void)
{
    return BT_stAddrMode_u8;
}



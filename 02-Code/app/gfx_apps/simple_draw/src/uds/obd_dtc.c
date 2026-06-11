/************* ************************************************** *****************
  \file          obd-dtc.c
  \author        
  \mail          
  \version       0.01
  \date          2016-10-10
  \description   obd dtc code, according SIO 14229(2006) D.2.2
  *******************************************************************************/

/*******************************************************************************
  Include Files
  *******************************************************************************/
#include "obd_dtc.h"
#include "obd_dtc_private.h"
#include "string.h"
/*******************************************************************************
  Type declaration
  *******************************************************************************/

/*******************************************************************************
  Global Varaibles
  the obd_dtc_para value should be connect with enum obd_dtc_name_t
  *******************************************************************************/
#define MAX_DTC_NUMBER                        2  /*最大DTC个数*/
#define MAX_DID_NUMBER                        15 /*最大DID个数*/
#define MAX_SNAPSHOT_NUMBER                   10 /*最大快照信息个数*/
#define MAX_GROUP_NUMBER                      5  /*最大DTC组个数*/
#define DTCSnapshotRecordNumberOfIdentifiers  2  /*快照信息中定义的成员个数*/
#define DTC_BYTE_NUMBER_TO_SAVE	5
#define DTC_STATUS_BYTE 1

uint8_t DiagnosticBuffStore[MAX_SNAPSHOT_NUMBER*DTCSnapshotRecordNumberOfIdentifiers]; //发送数据的缓存
uint8_t EnableDTCDetect;  //使能故障码检测
uint16_t CurrentSpeed = 0x0895;
uint8_t CurrentTime[6] = {0x20,0x02,0x28,0x10,0x36,0x40};
uint16_t CurrentRpm = 0x3040;
uint16_t CurrentVoltage = 0x1234;
static DTCNode DTCS[MAX_DTC_NUMBER]={
	//DTCCode  DetectFunction  EEpromAddr  SnapShotEEpromAddr  TripLimitTimes  DTCStatus  
    {0x900001,             0,     0x2000,             0x2040,             50,         0,    0,0,0,0},
    {0x900002,             0,     0x2080,             0x20C0,             50,         0,    0,0,0,0},
	
	// {0x41234A,             0,     0x3000,             0x3040,             50,         0,    0,0,0,0}
	
};

static Snapshot SnapShots[MAX_SNAPSHOT_NUMBER] = {
//snapshotRecordNumber   snapshotID 	*dataPointer  		dataLength
//    {1,					0x9102,		&CurrentSpeed,	2},
//    {2,					0x9103,		CurrentTime,	         6},
0,

};


/*******************************************************************************
  Function  Definition
  *******************************************************************************/
/**
 * obd_dtc_ctrl - set the dtc off flag
 *
 * @val : enable or disable DTCDetect  value, TRUE or FALSE
 *
 * returns:
 *     void
 */
void
obd_dtc_ctrl (bool_t val)
{
    EnableDTCDetect = val;
}

/**
 * obd_dtc_clear - clear a obd dtc data
 *
 * @dtc_n : dtc index
 *
 * returns:
 *     void
 */
static void
obd_dtc_clear (uint16_t dtc_n)
{
    uint8_t writbuf[128];
    memset(writbuf,0,128);
    //  for(uint8_t i = 0 ; i < MAX_DTC_NUMBER ; i++)
    {
	Diagnostic_EEProm_Write(DTCS[dtc_n].EEpromAddr, 64, writbuf);//clear dtcstatus
	Diagnostic_EEProm_Write(DTCS[dtc_n].SnapShotEEpromAddr, 0x80, writbuf);//clear snapshots
    }
    if (dtc_n >= MAX_DTC_NUMBER) return;

    DTCS[dtc_n].DTCStatus.DTCStatusByte = 0u;
    DTCS[dtc_n].DTCStatus.DTCbit.TestNotCompleteSinceLastClear = 1;      /*Bit 4*/
    DTCS[dtc_n].DTCStatus.DTCbit.TestNotCompleteThisMonitoringCycle = 1; /*Bit 6*/
    DTCS[dtc_n].FaultOccurrences = 0;
    DTCS[dtc_n].OldCounter= 0;
    DTCS[dtc_n].GoneCounter= 0;
    DTCS[dtc_n].TripCounter= 0;
}



/*******************************************************************************
  Function  Definition - for uds service 14 hex & 19 hex
  *******************************************************************************/
/**
 * get_dtc_number_by_status_mask - get dtc number by status mask
 *
 * @st_mask : dtc status mask
 *
 * returns:
 *     dtc number
 */
uint16_t
get_dtc_number_by_status_mask (uint8_t st_mask)
{
    uint16_t dtc_n;
    uint8_t dtc_st;
    uint16_t dtc_num;

    dtc_num = 0;
    for (dtc_n = 0; dtc_n < MAX_DTC_NUMBER; dtc_n++)
    {
        dtc_st = (DTCS[dtc_n].DTCStatus.DTCStatusByte & DTC_AVAILABILITY_STATUS_MASK);
        if (0 != (dtc_st & st_mask))
            dtc_num++;
    }

    return dtc_num;
}
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
get_dtc_by_status_mask (uint8_t dtc_buf[], uint16_t buf_len, uint8_t st_mask)
{
    uint16_t dtc_n;
    uint8_t dtc_st;
    uint16_t dtc_dlc;

    dtc_dlc = 0;
    for (dtc_n = 0; dtc_n < MAX_DTC_NUMBER; dtc_n++)
    {
        dtc_st = DTCS[dtc_n].DTCStatus.DTCStatusByte;
        if (0 != (dtc_st & st_mask))
        {
            if ((dtc_dlc + 4) <= buf_len)
            {
                dtc_buf[dtc_dlc++] = (uint8_t)(DTCS[dtc_n].DTCCode>> 16);
                dtc_buf[dtc_dlc++] = (uint8_t)(DTCS[dtc_n].DTCCode >> 8);
                dtc_buf[dtc_dlc++] = (uint8_t)(DTCS[dtc_n].DTCCode>> 0);
                dtc_buf[dtc_dlc++] = (dtc_st & DTC_AVAILABILITY_STATUS_MASK);
            }
            else
            {
                break;
            }
        }
    }

    return dtc_dlc;
}
/**
 * get_dtc_snopshot_by_dtc_number  - get dtc snapshot by dtc number
 *
 * this interface will be used by 1904h
 * 
 * @dtc_buf : the dest buf to store dtc data
 * @buf_len : the len of the buf
 * @EEPromAddr : dtc snapshot eeprom address
 * @DTCSnapshotRecordNumber: dtc snapshot record number  if 0xff, report all snapshots
 *
 * returns:
 *     dtc data len
 *
 */
uint16_t 
get_dtc_snopshot_by_dtc_number(uint8_t dtc_buf[], uint16_t buf_len, uint32_t EEPromAddr, uint8_t DTCSnapshotRecordNumber) {
    //uint8_t i,j;
    uint16_t ResponseLength;
    static uint8_t SnapShotsBuffer[41];
    Diagnostic_EEProm_Read(EEPromAddr, 41 , SnapShotsBuffer);
    if(SnapShotsBuffer[0]<1) return 0;//no snapshots
    if(DTCSnapshotRecordNumber!=0xff)
	{
		if(SnapShotsBuffer[0]<DTCSnapshotRecordNumber)
			return 0;//no this record(record number too big)
    }
    if(DTCSnapshotRecordNumber != 0xFF)//how many or which recod to read
    {
    	dtc_buf[0] = DTCSnapshotRecordNumber;
		dtc_buf[1]  = DTCSnapshotRecordNumberOfIdentifiers;
		ResponseLength = 2;
		Diagnostic_EEProm_Read(EEPromAddr + 1 + (8*(DTCSnapshotRecordNumber-1)), 8 , dtc_buf + ResponseLength);
		ResponseLength += 8;

    }
    else
	{
		ResponseLength = 0;
		for(uint8_t m = 0;m<MAX_SNAPSHOT_NUMBER;m++)
		{
		    if(m<SnapShotsBuffer[0])
			{
				dtc_buf[ResponseLength] = m+1;
				ResponseLength++;
				dtc_buf[ResponseLength]  = DTCSnapshotRecordNumberOfIdentifiers;
				ResponseLength++;
				Diagnostic_EEProm_Read(EEPromAddr + 1 + (8*m), 8 , dtc_buf + ResponseLength);
				ResponseLength += 8;
			}

		}

    }
    return ResponseLength;
/*
  //DTCSnapshotRecordNumberOfIdentifiers 表示快照信息中定义的成员量
  if(DTCSnapshotRecordNumber != 0xFF)//how many or which recod to read
  {
  dtc_buf[0] = DTCSnapshotRecordNumber;
  dtc_buf[1]  = DTCSnapshotRecordNumberOfIdentifiers;
  ResponseLength = 2;
  for(i = 0 ; i < MAX_SNAPSHOT_NUMBER ; i++)
  {
  if(SnapShots[i].snapshotRecordNumber == DTCSnapshotRecordNumber)
  {
  dtc_buf[ResponseLength] = (uint8_t)(SnapShots[i].snapshotID >> 8);
  dtc_buf[ResponseLength + 1] = (uint8_t)(SnapShots[i].snapshotID);
  ResponseLength += 2;
  Diagnostic_EEProm_Read(EEPromAddr, SnapShots[i].dataLength , dtc_buf + ResponseLength);
  ResponseLength += SnapShots[i].dataLength;
  break;
  }
  }

  }
  else
  {

  //   DTCSnapshotRecordNumber   DTCSnapshotRecordNumberOfIdentifiers DTCSnapshotRecord[] -->DTCSnapshotRecordNumber   DTCSnapshotRecordNumberOfIdentifiers DTCSnapshotRecord[] 

  ResponseLength = 0;
  for(i = 0 ; i < MAX_SNAPSHOT_NUMBER ; i++)
  {
  dtc_buf[ResponseLength] = i+1;
  ResponseLength++;
  dtc_buf[ResponseLength]  = DTCSnapshotRecordNumberOfIdentifiers;
  ResponseLength++;
  dtc_buf[ResponseLength] = (uint8_t)(SnapShots[i].snapshotID >> 8);
  dtc_buf[ResponseLength + 1] = (uint8_t)(SnapShots[i].snapshotID);
  ResponseLength += 2;
  Diagnostic_EEProm_Read(EEPromAddr, SnapShots[i].dataLength , dtc_buf + ResponseLength);
  ResponseLength += SnapShots[i].dataLength;
  EEPromAddr+=SnapShots[i].dataLength;
  }
  }
  return ResponseLength;
  */
}
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
get_supported_dtc (uint8_t dtc_buf[], uint16_t buf_len)
{
    uint16_t dtc_n;
    uint8_t dtc_st;
    uint16_t dtc_dlc;

    dtc_dlc = 0;
    for (dtc_n = 0; dtc_n < MAX_DTC_NUMBER; dtc_n++)
    {
        dtc_st = DTCS[dtc_n].DTCStatus.DTCStatusByte;

        if ((dtc_dlc + 4) <= buf_len)
        {
            dtc_buf[dtc_dlc++] = (uint8_t)(DTCS[dtc_n].DTCCode >> 16);
            dtc_buf[dtc_dlc++] = (uint8_t)(DTCS[dtc_n].DTCCode >> 8);
            dtc_buf[dtc_dlc++] = (uint8_t)(DTCS[dtc_n].DTCCode >> 0);
            dtc_buf[dtc_dlc++] = (dtc_st & DTC_AVAILABILITY_STATUS_MASK);
        }
        else
        {
            break;
        }
    }

    return dtc_dlc;
}
/**
 * clear_dtc_by_group - clear dtc data by group
 *
 * @group : the dtc group need to clear
 *
 * returns:
 *     dtc data len
 */
void
clear_dtc_by_group (uint32_t group)
{
    uint16_t dtc_n;
    switch (group)
    {
	case UDS_DTC_GROUP_EMISSION:
	    for (dtc_n = 0; dtc_n < MAX_DTC_NUMBER; dtc_n++)
	    {
		if (DTCS[dtc_n].DTCCode <= DTCG_EMISSION_END)
		    obd_dtc_clear (dtc_n);
	    }
	    break;
	case UDS_DTC_GROUP_NETWORK:
	    for (dtc_n = 0; dtc_n < MAX_DTC_NUMBER; dtc_n++)
	    {
		if (DTCS[dtc_n].DTCCode >= DTCG_NETWORK_START && DTCS[dtc_n].DTCCode <= DTCG_NETWORK_END)
		    obd_dtc_clear (dtc_n);
	    }
	    break;
	case UDS_DTC_GROUP_ALL:
	    for (dtc_n = 0; dtc_n < MAX_DTC_NUMBER; dtc_n++)
	    {
		obd_dtc_clear (dtc_n);
	    }
	    break;
	default:
	    break;
    }
}

/**
 * get_dtcnode_by_code - get dtc node info
 *
 * @dtcCode : the dtc code of the target node
 *
 * returns:
 *     dtc node pointer
 */
DTCNode* 
get_dtcnode_by_code(uint32_t dtcCode)
{
    uint8_t i;
	
    for(i = 0 ; i < MAX_DTC_NUMBER;i++)
    {	
		if((DTCS[i].DTCCode & 0xFFFFFF) == (dtcCode & 0xFFFFFF))
		{
			return DTCS+i;
		}
    }
	
	return NULL;
}

/**
 * uds_diagnostic_add_dtc - add a dtc function
 *
 * @DTCCode dtc by code
 * @MonitorFun dtc check function
 * @DectecPeroid not used by now
 * @ValidTimes not used by now
 * returns:
 *     TRUE or FALSE
 */
static uint8_t 
uds_diagnostic_add_dtc(uint32_t DTCCode,DetectFun MonitorFun,uint8_t DectecPeroid, uint8_t ValidTimes)
{
    for(uint8_t i = 0;i <MAX_DTC_NUMBER;i++){
	if(DTCCode == DTCS[i].DTCCode){
	    DTCS[i].DetectFunction = MonitorFun;
	    //DTCS[i].TripLimitTimes= ValidTimes;
	    //DTCS[i].TripCounter = DectecPeroid;
	    return TRUE;
	}

    }
    return FALSE;

}

/**
 * uds_diagnostic_add_dtcsnapshot - add a dtc snapshot
 *
 * @recordNumber snapshot record number
 * @ID snapshotID
 * @datap dataPointer
 * @size datasize
 * returns:
 *     null
 */
void 
uds_diagnostic_add_dtcsnapshot(uint8_t recordNumber , uint16_t ID , uint8_t* datap , uint8_t size)
{

    if(recordNumber == 0) return;
    if(recordNumber < MAX_SNAPSHOT_NUMBER)
    {
	SnapShots[recordNumber-1].snapshotRecordNumber = recordNumber;
	SnapShots[recordNumber-1].snapshotID = ID;
	SnapShots[recordNumber-1].dataPointer = datap;
	SnapShots[recordNumber-1].dataLength = size;

    }
    else;
}


/**
 * uds_diagnostic_save_snapshotdata - save snapshot data to eeprom
 *
 * @EEPromAddr snapshot address of eeprom
 * returns:
 *     null
 */

/*--------------------------------------------------------------------------------------
  EEPromAddr
  0xA000-0xA003            0xA004                    0xA005+8*i - 0xA008+8*i         0xA009+8*i-0xA00C+8*i
  DTC and status          total snapshots              ID1 and data                                       ID2  and data  



  ---------------------------------------------------------------*/
void 
uds_diagnostic_save_snapshotdata(uint8_t dtcnum, uint32_t EEPromAddr)
{
    uint8_t length = 0;
    uint8_t i;
    uint8_t total_snapshots[81] = {0};
    Diagnostic_EEProm_Read(EEPromAddr, 81, total_snapshots);
    if( total_snapshots[0] >= 10)return;
    length++;
    for(i = 0 ; i < DTCSnapshotRecordNumberOfIdentifiers; i++)
    {
	// total_snapshots[length + 8* total_snapshots[0]] = uint8_t
    	memcpy(total_snapshots + length + 8* total_snapshots[0],&SnapShots[i].snapshotID,2);
	length+=2;
	memcpy(total_snapshots + length + 8* total_snapshots[0],SnapShots[i].dataPointer,SnapShots[i].dataLength);
	length += SnapShots[i].dataLength;
    }
    total_snapshots[0]++;

    Diagnostic_EEProm_Write(EEPromAddr , 81 , total_snapshots);

    uds_diagnostic_save_singledtc(dtcnum);
}
/**
 * uds_diagnostic_dtc_handle - dtc check, record process
 *
 * @DtcNode which dtc node to handle
 * returns:
 *     null
 */
static void 
uds_diagnostic_dtc_handle(DTCNode* DtcNode)
{
    // uint8_t OperateCycleChange = FALSE;
    uint8_t CurrentResult;

    if(DtcNode!= NULL && DtcNode->DetectFunction != NULL)
    {
		CurrentResult = DtcNode->DetectFunction();
    }
    else
    {
		// CurrentResult = PASSED;
		return ;
    }

    if(CurrentResult == PASSED)
    {
		if(DtcNode->DTCStatus.DTCbit.TestNotCompleteThisMonitoringCycle == 1)
		{
			DtcNode->DTCStatus.DTCbit.TestNotCompleteThisMonitoringCycle = 0;//14229-1-Figure D.9-2
			// OperateCycleChange = TRUE;
		}
		if(DtcNode->DTCStatus.DTCbit.TestNotCompleteSinceLastClear == 1)
		{
			DtcNode->DTCStatus.DTCbit.TestNotCompleteSinceLastClear = 0;//14229-1-Figure D.9-1
		}
		if(DtcNode->DTCStatus.DTCbit.TestFailed == 1)
		{
			DtcNode->DTCStatus.DTCbit.TestFailed = 0;//14229-1-Figure D.9-7
		}
	#if 0
		if(DtcNode->DTCStatus.DTCbit.PendingDTC == 1)
		{
			DtcNode->DTCStatus.DTCbit.PendingDTC = 0;//14229-1-Table D.5
		}
	#endif
    }
    else if(CurrentResult == FAILED)
    {
		if(DtcNode->DTCStatus.DTCbit.TestNotCompleteThisMonitoringCycle == 1)
		{
			DtcNode->DTCStatus.DTCbit.TestNotCompleteThisMonitoringCycle = 0;//14229-1-Figure D.9-2
			// OperateCycleChange = TRUE;
		}

		if(DtcNode->DTCStatus.DTCbit.TestNotCompleteSinceLastClear == 1)
		{
			DtcNode->DTCStatus.DTCbit.TestNotCompleteSinceLastClear = 0;//14229-1-Figure D.9-1
		}
		if(DtcNode->DTCStatus.DTCbit.TestFailed == 0)
		{
			DtcNode->DTCStatus.DTCbit.TestFailed = 1;//14229-1-Figure D.9-3,8
		}
		if(DtcNode->DTCStatus.DTCbit.TestFailedThisMonitoringCycle ==0 )
		{

			DtcNode->TripCounter++;
			//DtcNode->DTCStatus.DTCbit.ConfirmedDTC = 1;
			//uds_diagnostic_save_snapshotdata(DtcNode->SnapShotEEpromAddr);
	#if 1
			if(DtcNode->TripCounter >= DtcNode->TripLimitTimes && DtcNode->DTCStatus.DTCbit.TestFailed == 1)//iso14229-1 Frigure D.4
			{
				DtcNode->DTCStatus.DTCbit.TestFailedThisMonitoringCycle = 1;//本次操作循环测试失败14229-1-Figure D.9-4
				if(DtcNode->DTCStatus.DTCbit.ConfirmedDTC != 1)
				{
					DtcNode->TripCounter = 0;
					DtcNode->DTCStatus.DTCbit.ConfirmedDTC = 1;

				}
				for(uint8_t i = 0;i<MAX_DTC_NUMBER;i++){
					if(DtcNode->DTCCode == DTCS[i].DTCCode){
						uds_diagnostic_save_snapshotdata(i, DtcNode->SnapShotEEpromAddr);
						break;
					}
				}
			}
			if(DtcNode->FaultOccurrences < 0xff)
			{
				DtcNode->FaultOccurrences++;
			}
	#endif
		}
		DtcNode->DTCStatus.DTCbit.PendingDTC = 1;//未确认的诊断故障码，14229-1-Figure D.9-5
		DtcNode->DTCStatus.DTCbit.TestFailedSinceLastClear = 1;//自上次清除后测试失败14229-1-Figure D.9-6	
		DtcNode->OldCounter = 0;

    }
    else ;

    if(DtcNode->DTCStatus.DTCbit.TestFailedThisMonitoringCycle == 0 && DtcNode->DTCStatus.DTCbit.TestNotCompleteThisMonitoringCycle == 0)//iso14229-1 Frigure D.4
    {
		DtcNode->TripCounter = 0;
    }
}

/**
 * uds_diagnostic_dtc_process - dtc check, record process , all the dtc items
 *
 * parameters: no
 * returns:
 *     null
 */
void 
uds_diagnostic_dtc_process(void)
{

    if(EnableDTCDetect != FALSE)
    {
	for(uint8_t i = 0 ; i < MAX_DTC_NUMBER ; i++)
	{
	    uds_diagnostic_dtc_handle(DTCS + i);
	}


    }
}
/**
 * uds_diagnostic_save_alldtc - save dtc info to eeprom
 *
 * parameters: no
 * returns:
 *     null
 */
void 
uds_diagnostic_save_alldtc(void)
{

    uint8_t i;
    uint8_t  DtcBytes[4];
    uint8_t  DtcBytesRead[4];
    for(i = 0 ; i < MAX_DTC_NUMBER ; i++)
    {
	if(DTCS[i].DTCStatus.DTCbit.TestNotCompleteSinceLastClear == 0 && DTCS[i].DTCStatus.DTCbit.TestFailedThisMonitoringCycle == 0)
	{
	    DTCS[i].DTCStatus.DTCbit.PendingDTC = 0;
	}

	if(DTCS[i].DTCStatus.DTCbit.TestFailedThisMonitoringCycle == 0  && DTCS[i].DTCStatus.DTCbit.ConfirmedDTC == 1)
	{
	    if(DTCS[i].OldCounter >= 40)//故障码老化机制
	    {
		DTCS[i].OldCounter = 0;
		DTCS[i].GoneCounter++;
		DTCS[i].DTCStatus.DTCStatusByte = 0x50;//when clear bit4 and bit6 must be setted to 1 and others 0
		DTCS[i].FaultOccurrences = 0;
		DTCS[i].OldCounter = 0;
		DTCS[i].GoneCounter = 0;
		DTCS[i].TripCounter = 0;
	    }
	    else
	    {
		DTCS[i].OldCounter++;
	    }
	}
	else
	{
	    DTCS[i].OldCounter = 0;
	}
	DTCS[i].DTCStatus.DTCStatusByte &= 0xBD;//bit1 and bit6 can not be saved
	//memcpy(DtcBytes + DtcBytesToSave,&(DTCS[i].DTCStatus),DTC_BYTE_NUMBER_TO_SAVE);
	//DtcBytesToSave += DTC_BYTE_NUMBER_TO_SAVE;
	DtcBytes[0] = DTCS[i].DTCStatus.DTCStatusByte;
	Diagnostic_EEProm_Write(DTCS[i].EEpromAddr, DTC_STATUS_BYTE , DtcBytes);//&(DTCS[i].DTCStatus.DTCStatusByte));
	Diagnostic_EEProm_Read(DTCS[i].EEpromAddr, 1,DtcBytesRead);
    }
    //Diagnostic_EEProm_Write(DtcStartAddr , DtcBytesToSave , DtcBytes);

}
/**
 * uds_diagnostic_save_singledtc - save certain dtc status info to eeprom
 *
 * @: dtcnumber dtc number
 * returns:
 *     null
 */
void 
uds_diagnostic_save_singledtc(uint8_t dtcnumber)
{
    uint8_t  DtcBytes[4];
    uint8_t  DtcBytesRead[4];
    DtcBytes[0] = DTCS[dtcnumber].DTCStatus.DTCStatusByte;
    Diagnostic_EEProm_Write(DTCS[dtcnumber].EEpromAddr, DTC_STATUS_BYTE , DtcBytes);//&(DTCS[i].DTCStatus.DTCStatusByte));
    Diagnostic_EEProm_Read(DTCS[dtcnumber].EEpromAddr, 1,DtcBytesRead);

    //Diagnostic_EEProm_Write(DtcStartAddr , DtcBytesToSave , DtcBytes);

}
#if 0
/**
 * NMGetLimpHome - example check handler
 *
 * parameters: no
 * returns:
 *     failed or pass
 */
static uint8_t 
NMGetLimpHome(void)
{
    if (1)
    {
		return 2;//failed
    }
    else
    {
		return 0;//passed
    }
}
#endif
/**
 * check_flash - check flash pass or fail, need done by yourself
 *
 * parameters: no
 * returns:
 *     failed or pass
 */
extern uint8_t check_flash(void);
/**
 * cluster_dtc_u0001check - cluster dtc 0001 check handler
 *
 * parameters: no
 * returns:
 *     failed or pass
 */
static uint8_t  cluster_dtc_u0001check(void){
    uint8_t result = 0;
    result = check_flash();
    return result;


}
/**
 * check_rtc- check rtc pass or fail, need done by yourself
 *
 * parameters: no
 * returns:
 *     failed or pass
 */
extern uint8_t check_rtc(void);
/**
 * cluster_dtc_u0001check - cluster dtc 0001 check handler
 *
 * parameters: no
 * returns:
 *     failed or pass
 */
static uint8_t  cluster_dtc_u0002check(void){
    uint8_t result = 0;
    result = check_rtc();
    return result;


}
/**
 * uds_diagnostic_load_alldtc - load all dtcs
 *
 * parameters: no
 * returns:
 *     null
 */
static void 
uds_diagnostic_load_alldtc(void)
{

    uint8_t i;
    for(i = 0 ; i < MAX_DTC_NUMBER ; i++)//load all dtc status
    {
		Diagnostic_EEProm_Read(DTCS[i].EEpromAddr, 1, (uint8_t *)&(DTCS[i].DTCStatus));
		//Diagnostic_EEProm_Read(DTCS[i].EEpromAddr, DTC_BYTE_NUMBER_TO_SAVE , &(DTCS[i].DTCStatus));
		DTCS[i].DTCStatus.DTCStatusByte |= 0x40;//when IGN ON, bit6 default 1,
		DTCS[i].DTCStatus.DTCStatusByte &= 0xFD;//when IGN ON, bit1 default 0

    }

    uds_diagnostic_add_dtc(0x900001, (DetectFun)cluster_dtc_u0001check, 10, 1 );  //load dtc judgement function
    uds_diagnostic_add_dtc(0x900002, (DetectFun)cluster_dtc_u0002check, 10, 50 ); //load dtc judgement function
    //uds_diagnostic_add_dtc(0x41234A,cluster_dtc_u0001check,10, 100 ); //load dtc judgement function
    uds_diagnostic_add_dtcsnapshot(1,0x9102, (uint8_t *)&CurrentSpeed, 2);   //load snapshot id and data pointer
    uds_diagnostic_add_dtcsnapshot(2,0x9103, (uint8_t *)&CurrentVoltage, 2); //load snapshot id and data pointer
}




/**
 * uds_diagnostic_load_alldata - load all dtc info
 *
 * parameters: no
 * returns:
 *     no
 */
void 
uds_diagnostic_load_alldata(void)
{
    uint8_t writbuf[81];
    for(uint8_t i = 0;i<81;i++)
		writbuf[i] = i;
    uds_diagnostic_load_alldtc();

    //  for(uint8_t m = 0;m<3;m++)
    //	Diagnostic_EEProm_Write(DTCS[0].SnapShotEEpromAddr, 81,writbuf);
    obd_dtc_ctrl(TRUE);
    for(uint8_t i = 0;i<81;i++)
		writbuf[i] =0;
    Diagnostic_EEProm_Read(DTCS[0].SnapShotEEpromAddr, 81,writbuf);

}

/**
 * uds_eeprom_init -uds related info init
 *
 * parameters: no
 * returns:
 *     no
 */
void 
uds_eeprom_init(void)
{
    uint8_t writbuf[128];
    memset(writbuf,0,128);
    for(uint8_t i = 0 ; i < MAX_DTC_NUMBER ; i++)
    {
	Diagnostic_EEProm_Write(DTCS[i].EEpromAddr, 64, writbuf);//clear dtcstatus
	Diagnostic_EEProm_Write(DTCS[i].SnapShotEEpromAddr, 0x80, writbuf);//clear snapshots
	//Diagnostic_EEProm_Read(DTCS[i].EEpromAddr, DTC_BYTE_NUMBER_TO_SAVE , &(DTCS[i].DTCStatus));
	DTCS[i].DTCStatus.DTCStatusByte |= 0x40;//when IGN ON, bit6 default 1,
	DTCS[i].DTCStatus.DTCStatusByte &= 0xFD;//when IGN ON, bit1 default 0

    }

}
/****************EOF****************/

#include "r_typedefs.h"
#include "fcl_user.h"
#include "fcl_cfg.h"
#include "r_fcl_Types.h"
#include "r_fcl.h"

#if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
#if R_FCL_COMPILER == R_FCL_COMP_GHS
#pragma ghs section text = ".R_FCL_CODE_USR"
#elif R_FCL_COMPILER == R_FCL_COMP_IAR
#pragma location = "R_FCL_CODE_USR"
#elif R_FCL_COMPILER == R_FCL_COMP_REC
#pragma section text "R_FCL_CODE_USR"
#endif
#endif



/************************************************************************************************************
 * Function name: FCL_Erase()
 ***********************************************************************************************************/
/**
 * execute fcl erease function. not used in this part
 *
 * @param[in,out] : uint8_t idx: current erase block id
 * @return        no
 */
/***********************************************************************************************************/
void FCL_Erase(uint8_t idx, uint8_t cnt)
{
    r_fcl_request_t     myRequest;
    FCLUser_Open ();//get access to flash
    /* prepare environment */
    myRequest.command_enu = R_FCL_CMD_PREPARE_ENV;
    R_FCL_Execute (&myRequest);
#if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
    while (R_FCL_BUSY == myRequest.status_enu)
    {
	R_FCL_Handler ();
    }
#endif
	
    /* erase block 8-126*/
    myRequest.command_enu = R_FCL_CMD_ERASE;
    myRequest.idx_u32     = idx; /* erased range = APP_START_ADDR to end */
    myRequest.cnt_u16     = cnt;
    R_FCL_Execute (&myRequest);
#if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
    while (R_FCL_BUSY == myRequest.status_enu)
    {
	R_FCL_Handler ();
    }
#endif

    FCLUser_Close ();
	
	
}


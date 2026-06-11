
#include "fcl_Priv.h"
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
 * Function name: FCL_prog()
 ***********************************************************************************************************/
/**
 * execute fcl program function. not used in this part
 *
 * @param[in,out] address to write to, write content
 * @return        1: failed
 0: success
 */
/***********************************************************************************************************/
uint8_t FCL_prog(uint32_t address, char write_buf[])
{
	#pragma alignvar(4)
    r_fcl_request_t     myRequest;  
    
    FCLUser_Open ();//get access to flash
	
    /* write 1024 bytes to address  */
    myRequest.command_enu   = R_FCL_CMD_WRITE;
    myRequest.bufferAdd_u32 = (uint32_t)&write_buf[0];
    myRequest.idx_u32       = address;
    myRequest.cnt_u16       = 4;  /* written bytes = 256 * cnt_u16 */
    R_FCL_Execute (&myRequest);
#if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
    while (R_FCL_BUSY == myRequest.status_enu)
    {
		R_FCL_Handler ();
    }
#endif

    if (R_FCL_OK != myRequest.status_enu) //error
    {
		FCLUser_Close ();
		return 1;
    }
	
    FCLUser_Close ();
	
    return 0;
	
}


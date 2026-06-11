#ifdef ENABLE_QAC_TEST
    #pragma PRQA_MESSAGES_OFF 0292
#endif
/************************************************************************************************************
* File Name     : $Source: main.c $
* Mod. Revision : $Revision: 1.17 $
* Mod. Date     : $Date: 2016/05/31 11:11:54JST $
* Device(s)     : RV40 Flash based RH850 microcontroller
* Description   : Application sample for usage of Renesas Flash Control Library (FCL)
************************************************************************************************************/

/************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only  intended for use with
* Renesas products. No other uses are authorized. This software is owned by Renesas Electronics
* Corporation and is protected under all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE,
* WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR
* ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR
* CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the
* availability of this software. By using this software, you agree to the additional terms and conditions
* found by accessing the  following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2015-2016 Renesas Electronics Corporation. All rights reserved.
************************************************************************************************************/

#ifdef ENABLE_QAC_TEST
    #pragma PRQA_MESSAGES_ON 0292
#endif

/************************************************************************************************************
* MISRA Rule:   MISRA-C 2004 rule 3.1 (QAC message 0292)
* Reason:       To support automatic insertion of revision, module name etc. by the source
*               revision control system it is necessary to violate the rule, because the
*               system uses non basic characters as placeholders.
* Verification: The placeholders are used in commentaries only. Therefore rule violation cannot
*               influence code compilation.
************************************************************************************************************/


/************************************************************************************************************
Includes   <System Includes> , "Project Includes"
************************************************************************************************************/
#include "r_typedefs.h"
#include "fcl_cfg.h"
#include "r_fcl_types.h"
#include "r_fcl.h"
#include "target.h"
#include "fcl_descriptor.h"
#include "fcl_user.h"
#include "Cpu.h"
#include "rscan.h"
//#include "bsp.h"

/************************************************************************************************************
Macro definitions
************************************************************************************************************/
#define FLMD0_PROTECTION_OFF    (0x01u)
#define FLMD0_PROTECTION_ON     (0x00u)
#define REINITIALIZE_BUFFER     for (i = 0; i < sizeof (writeBuffer_u08); i++)          \
                                {                                                       \
                                    writeBuffer_u08[i] = i+2;                             \
                                }                                                       \
                                for (i = 0; i < (sizeof (readBuffer_u32) >> 2); i++)    \
                                {                                                       \
                                    readBuffer_u32[i]  = 0xaa;                          \
                                }

/************************************************************************************************************
Typedef definitions
************************************************************************************************************/


/************************************************************************************************************
Exported global variables (to be accessed by other files)
************************************************************************************************************/

/* This array reserves the copy area in the device RAM     0x8000  32k*/
#define FCL_RAM_EXECUTION_AREA_SIZE 0x8000


#pragma ghs startdata
#pragma ghs section bss = ".FCL_RESERVED"
#define R_FCL_NOINIT


R_FCL_NOINIT uint8_t FCL_Copy_area[FCL_RAM_EXECUTION_AREA_SIZE];


#pragma ghs section bss = default
#pragma ghs enddata

CAN_MESSAGE message_CCVS;
CAN_MESSAGE message_Rec;
uint8_t tau1_counter;

/************************************************************************************************************
Private global variables and functions
************************************************************************************************************/

void FCL_Ctrl (void);
void init_TAUB0(void);                  /* initialize timer TAUB0 */
void INTTAUB0I0(void);                  /* TAUB0 interrupt service routine */

void init_TAUB0(void) {                 /* initialize timer TAUB0 channel 0 to interval timer mode */
  TAUB0TPS=0x0007;                      /* specify clock frequency of CK0...CK3  (CK0 -> 40 MHz / (2 ^ 7) = 312.5 KHz) */
  TAUB0CMOR0=0x0000;                    /* interval timer mode */
  TAUB0CDR0=31250;                      /* compare register in interval timer mode (312.5KHz / 31250 = 10 Hz) */
  TAUB0TS=0x0001;                       /* start channel 0 */
}





/************************************************************************************************************
 * Function name: <Add function name>
 ***********************************************************************************************************/
/**
 * <Add function description>
 *
 * @param[in,out] <Add parameter>      <Add parameter description>
 * @return        <Add return value>
 */
/***********************************************************************************************************/
void main (void)
{
    r_fcl_status_t ret;
    #if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
    uint32_t   (* fpFct)(void);
    #endif
     tau1_counter = 0;
    /* initialize CPU and clocks */
    R_FCL_INIT_CPU_AND_FPERIPHERAL_CLK

    SET_EBASE(0xFED80000); //set Ebase to 0xFEDE0000--FCL_RAM_ADDRESS, set RINT to 1
    SET_PSW_EBV();
    init_TAUB0();   
	//init_TAUB1();
    RS_CAN_init();
    INTC1EIC17=0x0047;                    /* demask timer TAUB0 chanel 0 interrupt and enable reference table jump method */
    __EI();                                          /* enable interrupts globally */
    message_CCVS.id = 0x0C19A7A1;
    message_CCVS.data[0] = 0x01;
    message_CCVS.data[2] = 0xff;
    message_CCVS.data[3] = 0x02;
    message_CCVS.data[4] = 0x03;
    message_CCVS.data[5] = 4;
    message_CCVS.data[6] = 5;
    message_CCVS.data[7] = 6;
    message_CCVS.len = 8;
    message_CCVS.type = 0;
    CAN2_SendMessage(&message_CCVS);
    
    while(1) {                            /* endless loop */
		
        while(tau1_counter<10)//timeout control, wait here for 200ms
	{
                  ;
	}
		if(message_Rec.data[0]==0x00)
		{//goto slefprogrammming
            
			ret = R_FCL_Init (&sampleApp_fclConfig_enu);
    			if (R_FCL_OK == ret)
    			{
				
    	    			ret = R_FCL_CopySections ();//This function is used to copy some FCL code sections(defined in ld files) to a specified destination address 
									          //in RAM. From that location code can be executed while Code Flash is not available.
									          //The R_FCL_CopySections function shall be executed from ROM. 
    			}

    			if (R_FCL_OK == ret)
    			{
        			#if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
					//&FCL_Ctrl: ROM address of copied function 
          			  fpFct = ( uint32_t (*)() )R_FCL_CalcFctAddr ( (uint32_t)(&FCL_Ctrl) );//Calculate new address after copy process
          			  /*
          			       This function calculates the new address of a function copied from ROM to RAM. To 
					calculate the new address of the function, the copied function must be located in one of 
					the FCL linker segments described in Linker sections.

			       */
           			  fpFct ();//execute from New RAM address of function.
        			#else
            			FCL_Ctrl ();
      				  #endif


			}
		   }
			else
			{


			}
	}


  }
    
    




#pragma ghs section text = ".R_FCL_CODE_USR"
/*
 contains user code that has to be executed in parallel with FCL 
 operation when code flash is unavailable.
*/

void FCL_Ctrl (void)
{
    r_fcl_request_t     myRequest;

    r_fcl_request_t mySecRequest;

    uint8_t             writeBuffer_u08[512];
    uint32_t            readBuffer_u32[8];
    uint16_t            i;
    uint32_t  (* jr_app)(void);
    while(1)
    {
    
    CAN_BOOT_ExecutiveCommand();


    }

    REINITIALIZE_BUFFER;

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


    /* get block count */
    myRequest.command_enu   = R_FCL_CMD_GET_BLOCK_CNT;
    myRequest.bufferAdd_u32 = (uint32_t)&readBuffer_u32[0];
    R_FCL_Execute (&myRequest);

    /* get block end address of block 2 */
    REINITIALIZE_BUFFER;
    myRequest.command_enu   = R_FCL_CMD_GET_BLOCK_END_ADDR;
    myRequest.bufferAdd_u32 = (uint32_t)&readBuffer_u32[0];
    myRequest.idx_u32       = 0x2;                   /* result: 0x5fff */
    R_FCL_Execute (&myRequest);

    /* erase block 2 and 3 */
    myRequest.command_enu = R_FCL_CMD_ERASE;
    myRequest.idx_u32     = 0x2;                   /* erased range = 0x4000 to 0x7fff */
    myRequest.cnt_u16     = 2;
    R_FCL_Execute (&myRequest);
    #if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
        while (R_FCL_BUSY == myRequest.status_enu)
        {
            R_FCL_Handler ();
        }
    #endif

    /* write 512 bytes to address 0x40000 (start of block 2) */
    REINITIALIZE_BUFFER;
    myRequest.command_enu = R_FCL_CMD_WRITE;
    myRequest.bufferAdd_u32 = (uint32_t)&writeBuffer_u08[0];
    myRequest.idx_u32       = 0x4000;
    myRequest.cnt_u16       = 2;                    /* written bytes = 256 * cnt_u16 */
    R_FCL_Execute (&myRequest);
    #if R_FCL_COMMAND_EXECUTION_MODE == R_FCL_HANDLER_CALL_USER
        while (R_FCL_BUSY == myRequest.status_enu)
        {
            R_FCL_Handler ();
        }
    #endif
    while(1);
    FCLUser_Close ();

    jr_app= ( uint32_t (*)() )0x20000;
    jr_app();
} /* FCL_Ctrl */


#pragma ghs section text = ".R_FCL_CODE_USRINT"
/*

 contains user interrupt routines that may be executed in parallel 
with FCL operation when code flash is unavailable. 
*/

__interrupt void INTTAUB0I0(void) {     /* TAUB0 interrupt service routine */
  
  tau1_counter++;
  if(tau1_counter == 50)
  {
  tau1_counter  = 0;
  CAN2_SendMessage(&message_CCVS);
  }
  
}

__interrupt void INTRCANGRECC(void) {     /* TAUB0 interrupt service routine */
  
  
CAN2_ReceiveMessage(&message_Rec);
  
}



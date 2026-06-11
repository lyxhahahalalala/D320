/*
****************************************************************************
PROJECT : Vlib Device Driver 
FILE    : $Id: r_dev_asm.s 3577 2014-08-14 12:13:44Z matthias.nippert $
============================================================================ 
DESCRIPTION
assembler support function 
============================================================================
                            C O P Y R I G H T
============================================================================
                       Copyright (c) 2013 - 2014
                                  by 
                       Renesas Electronics (Europe) GmbH. 
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: only for testing, not for mass production

DISCLAIMER

LICENSEE has read, understood and accepted the terms and conditions defined in
the license agreement, especially the usage rights. In any case, it is
LICENSEE's responsibility to make sure that any user of the software complies
with the terms and conditions of the signed license agreement.

SAMPLE CODE is not part of the licensed software, as such it must not be used in
mass-production applications. It can only be used for evaluation and
demonstration purposes at customer's premises listed in the signed license
agreement.

****************************************************************************
*/

/***********************************************************
  Section: Global Functions
*/

        .text

/***********************************************************
  Function: _R_DEV_Nop

  Nop instruction that can be used instead of asm("nop")
  for the sake of MISRA compliance.  

  Parameters:
  
*/
        .globl _R_DEV_Nop
_R_DEV_Nop: 
		nop
		jmp     [lp]

/***********************************************************
  Function: _R_DEV_SyncP

  CPU pipeline sync 

  Parameters: none
  
*/
        .globl _R_DEV_SyncP
_R_DEV_SyncP: 
		syncp
		jmp     [lp]

/***********************************************************
  Function: _R_DEV_SyncM

  CPU memory sync 

  Parameters: none
  
*/
        .globl _R_DEV_SyncM
_R_DEV_SyncM: 
		syncm
		jmp     [lp]



/***********************************************************
  Function: _R_DEV_SysCall

  Do a SysCall to get elevated rights.
  
  Parameters:
  
*/
        .globl _R_DEV_SysCall
_R_DEV_SysCall: 
        /* 0x10: TRAP1 (Vector 0x10 is equal to a EIIC value of 0x50) */
        trap    0x10    
		jmp     [lp]
		
		
		
/***********************************************************
  Function: _R_DEV_Trap1

  Trap handler of the DEV driver.

  This function is called by 'R_DEV_SysCall'
  
  Parameters:
  
*/
        .globl _TRAP1
_TRAP1: 
        .globl _R_DEV_Trap1
_R_DEV_Trap1:
        pushsp	lp-lp
        
        /* sub-routine */
        jarl    _R_DEV_SysCallElevated,lp

        popsp	lp-lp
	    eiret
	
	

/***********************************************************
  Function: _R_DEV_EnterUserModeElevated

  Sets the UserMode bit of the PSW.
  Calling this function reduces the privileges of the application.
  To be called from <R_DEV_SysCallElevated>.

  Parameters:
  
*/
        .globl _R_DEV_EnterUserModeElevated
_R_DEV_EnterUserModeElevated: 
        /* EIPSW: regID: 1, selID: 0 <= We're supposed to be in TRAP1 */
        /* FEPSW: regID: 3, selID: 0 */
        /* PSW:   regID: 5, selID: 0 */
        /* The selection of the correct xxPSW can be done by evaluating PSW.NP and PSW.DI */
		mov     (1<<30), r7    /* Load Bitmask for UM into r7 */
		stsr    1,  r6, 0      /* Load PSW into r6 */
		or      r7, r6         /* Apply Bitmask to r6 */
		ldsr    r6, 1,  0      /* Load r6 into PSW */
		jmp     [lp]
		
		
		
/***********************************************************
  Function: _R_DEV_EnterSupervisorModeElevated

  Clears the UserMode bit of the FEPSW.
  Calling this function elevates the privileges of the application.
  To be called from <R_DEV_SysCallElevated>.

  Parameters:
  
*/
        .globl _R_DEV_EnterSupervisorModeElevated
_R_DEV_EnterSupervisorModeElevated: 
        /* EIPSW: regID: 1, selID: 0 <= We're supposed to be in TRAP1 */
        /* FEPSW: regID: 3, selID: 0 */
        /* PSW:   regID: 5, selID: 0 */
        /* The selection of the correct xxPSW can be done by evaluating PSW.NP and PSW.DI */
		mov     ~(1<<30), r7   /* Load Bitmask for UM into r7 */
		stsr    1,  r6, 0      /* Load EIPSW into r6 */
		and     r7, r6         /* Apply Bitmask to r6 */
		ldsr    r6, 1,  0      /* Load r6 into EIPSW */
		jmp     [lp]



        
       /* EOF */
	


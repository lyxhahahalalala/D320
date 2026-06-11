/*
****************************************************************************
PROJECT : DRW2D
FILE    : r_drw2d_os_no_os.c
============================================================================ 
DESCRIPTION
DRW2D NO-OS abstraction layer
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

DISCLAIMER

LICENSEE has read, understood and accepted the terms and conditions defined in
the license agreement, especially the usage rights. In any case, it is
LICENSEE's responsibility to make sure that any user of the software complies
with the terms and conditions of the signed license agreement.

****************************************************************************
*/

#include <r_typedefs.h>
#include <r_drw2d_os.h>


/***********************************************************
  Section: Local Functions
*/


/***********************************************************
  Section: Thread functions
*/



/***********************************************************
  Function: R_DRW2D_OS_Thread_Create
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Thread_Create(r_drw2d_OS_ThreadInfo_t *ThreadInfo) 
{
    return R_DRW2D_OS_ERR_OK;
}



/***********************************************************
  Function: R_DRW2D_OS_Thread_Wait
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Thread_Wait(r_drw2d_OS_ThreadInfo_t *ThreadInfo,
                                          uint32_t              Timeout
                                          )
{
    return R_DRW2D_OS_ERR_OK;
}



/***********************************************************
  Function: R_DRW2D_OS_Thread_GetId
*/
r_drw2d_OS_ThreadId_t R_DRW2D_OS_Thread_GetId(void)
{
   
    return R_DRW2D_OS_ERR_OK;
}



/***********************************************************
  Function: R_DRW2D_OS_Thread_Destroy
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Thread_Destroy(r_drw2d_OS_ThreadInfo_t *ThreadInfo)
{
    return R_DRW2D_OS_ERR_OK;
}


/***********************************************************
  Section: Mutex functions
*/



/***********************************************************
  Function: R_DRW2D_OS_Mutex_Create
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Mutex_Create(r_drw2d_OS_Mutex_t *Mutex)
{
    return R_DRW2D_OS_ERR_OK;
}



/***********************************************************
  Function: R_DRW2D_OS_Mutex_Destroy
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Mutex_Destroy(r_drw2d_OS_Mutex_t *Mutex)
{
    return R_DRW2D_OS_ERR_OK;
}



/***********************************************************
  Function: R_DRW2D_OS_Mutex_Lock
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Mutex_Lock(r_drw2d_OS_Mutex_t *Mutex)
{
    return R_DRW2D_OS_ERR_OK;
}



/***********************************************************
  Function: R_DRW2D_OS_Mutex_Unlock
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Mutex_Unlock(r_drw2d_OS_Mutex_t *Mutex)
{
    return R_DRW2D_OS_ERR_OK;
}





/***********************************************************
  Section: Semaphore functions
*/



/***********************************************************
  Function: R_DRW2D_OS_Semaphore_Create
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Semaphore_Create(r_drw2d_OS_Semaphore_t *Sem, 
                                               uint32_t                InitialValue
                                               )
{
    return R_DRW2D_OS_ERR_OK;
}



/***********************************************************
  Function: R_DRW2D_OS_Semaphore_Destroy
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Semaphore_Destroy(r_drw2d_OS_Semaphore_t *Sem)
{
    return R_DRW2D_OS_ERR_OK;
}



/***********************************************************
  Function: R_DRW2D_OS_Semaphore_Wait
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Semaphore_Wait(r_drw2d_OS_Semaphore_t *Sem)
{
    return R_DRW2D_OS_ERR_OK;
}



/***********************************************************
  Function: R_DRW2D_OS_Semaphore_Post
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Semaphore_Post(r_drw2d_OS_Semaphore_t *Sem)
{
    return R_DRW2D_OS_ERR_OK;
}





/***********************************************************
  Section: Memory utility functions
*/



/***********************************************************
  Function: R_DRW2D_OS_Mem_Alloc
*/
void *R_DRW2D_OS_Mem_Alloc(uint32_t NumBytes, uint32_t Alignment, r_drw2d_OS_Mem_t Type)
{
    return 0;
}



/***********************************************************
  Function: R_DRW2D_OS_Mem_Free
*/
void R_DRW2D_OS_Mem_Free(void *Addr)
{
}



/***********************************************************
  Function: R_DRW2D_OS_Mem_Clear
*/
void R_DRW2D_OS_Mem_Clear(void *Addr, uint32_t NumBytes)
{
}



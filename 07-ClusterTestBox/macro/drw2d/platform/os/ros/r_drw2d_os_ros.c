/*
****************************************************************************
PROJECT : DRW2D
FILE    : r_drw2d_os_ros.c
============================================================================ 
DESCRIPTION
DRW2D OS abstraction layer
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
#include <stdlib.h>
#include <string.h>
#include <r_bsp_stdio_api.h>   /* Board support STDIO package */

#include <r_drw2d_os.h> /* (note) includes r_os_api.h */
#include <r_cdi_api.h>

/***********************************************************
  Section: Local Defines
*/


/***********************************************************
  Section: Local Functions
*/


static void *loc_thread__entry(void *Param);
/***********************************************************
  Function: loc_thread__entry
*/
static void *loc_thread__entry(void *Param) {
    r_drw2d_OS_ThreadInfo_t *info = (r_drw2d_OS_ThreadInfo_t *) Param;

    info->Function(info);

    return R_NULL; /* never reached ? */
}



/***********************************************************
  Section: Global Functions
*/
/***********************************************************
  Section: Thread functions
*/
/***********************************************************
  Function: R_DRW2D_OS_Thread_Create
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Thread_Create(r_drw2d_OS_ThreadInfo_t *ThreadInfo)
{
    r_drw2d_OS_Error_t ret;
    int32_t error;
    
    if (R_NULL != ThreadInfo)
    {
        R_OS_ThreadAttr_t  attr;
        R_OS_ThreadSchedParam_t sched_param;

        sched_param.SchedPriority = ThreadInfo->Prio;

        error = R_OS_ThreadAttrInit(&attr);
        
        if(0 == error)
        {
            error = R_OS_ThreadAttrSetSchedParam(&attr, &sched_param);
        }
        if(0 == error)
        {
            error = R_OS_ThreadAttrSetStackAddr(&attr, ThreadInfo->StackAddr);
        }
        if(0 == error)
        {
            error = R_OS_ThreadAttrSetStackSize(&attr, ThreadInfo->StackSize);
        }
        if(0 == error)
        {
            error = R_OS_ThreadCreate(&(ThreadInfo->Thread),
                                      &attr,
                                      &loc_thread__entry,
                                      ThreadInfo
                                      );
        }

        if (0 == error)
        {
            if (0 != ThreadInfo->Thread)
            {
                ret = R_DRW2D_OS_ERR_OK;
            }
            else
            {
                ret = R_DRW2D_OS_ERR_THREAD_CREATE;
            }
        }
        else
        {
            ret = R_DRW2D_OS_ERR_THREAD_CREATE;
        }
    }
    else
    {
        ret = R_DRW2D_OS_ERR_THREAD_INFO;
    }

    return ret;
}



/***********************************************************
  Function: R_DRW2D_OS_Thread_Wait
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Thread_Wait(r_drw2d_OS_ThreadInfo_t *ThreadInfo,
                                          uint32_t                 Timeout
                                          )
{
    r_drw2d_OS_Error_t ret;

    if (R_NULL != ThreadInfo)
    {
        if (0 == Timeout)
        {
            if (0 != ThreadInfo->Thread)
            {
                uint32_t retROS = R_OS_ThreadJoin(ThreadInfo->Thread, R_NULL);

                if (0 == retROS)
                {
                    /* Succeeded */
                    ret = R_DRW2D_OS_ERR_OK;
                }
                else
                {
                    ret = R_DRW2D_OS_ERR_THREAD_WAIT;
                }
            }
            else
            {
                /* Failed: Invalid thread handle */
                ret = R_DRW2D_OS_ERR_THREAD_HANDLE;
            }
        }
        else
        {
            /* ROS only supprotes waitinf for an infinite time. */
            ret = R_DRW2D_OS_ERR_PARAM_NOT_SUPPORTED;
        } 
    }
    else
    {
        /* Failed: Invalid thread info */
        ret = R_DRW2D_OS_ERR_THREAD_INFO;
    }

    return ret;
}



/***********************************************************
  Function: R_DRW2D_OS_Thread_GetId
*/
r_drw2d_OS_ThreadId_t R_DRW2D_OS_Thread_GetId(void)
{
    return (r_drw2d_OS_ThreadId_t) R_OS_ThreadSelf();
}



/***********************************************************
  Function: R_DRW2D_OS_Thread_Sleep
*/
void R_DRW2D_OS_Thread_Sleep(uint32_t MilliSeconds)
{
    R_OS_ThreadSleep(MilliSeconds);
}



/***********************************************************
  Function: R_DRW2D_OS_Thread_Exit
*/
void R_DRW2D_OS_Thread_Exit(r_drw2d_OS_ThreadInfo_t *ThreadInfo,
                            void                    *RetVal)
{

    ThreadInfo->Result = RetVal;

    R_OS_ThreadExit(RetVal);
}



/***********************************************************
  Function: R_DRW2D_OS_Thread_Destroy
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Thread_Destroy(r_drw2d_OS_ThreadInfo_t *ThreadInfo)
{
    r_drw2d_OS_Error_t ret;

    ret = R_DRW2D_OS_Thread_Wait(ThreadInfo, R_DRW2D_OS_THREAD_WAIT_INFINITE);

    /* (note) Param error checking already done by Wait() */

    return ret;
}




/***********************************************************
  Section: Mutex functions
*/



/***********************************************************
  Function: R_DRW2D_OS_Mutex_Create
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Mutex_Create(r_drw2d_OS_Mutex_t *Mutex)
{
    r_drw2d_OS_Error_t ret;
    
    if (R_NULL != Mutex)
    {
        if (0 == R_OS_ThreadMutexInit(Mutex, 0))
        {
            /* Succeeded */
            ret = R_DRW2D_OS_ERR_OK;
        }
        else
        {
            ret = R_DRW2D_OS_ERR_MUTEX_CREATE;
        }
    }
    else
    {
        /* Failed: Invalid mutex handle */
        ret = R_DRW2D_OS_ERR_MUTEX_HANDLE;
    }

    return ret;
}



/***********************************************************
  Function: R_DRW2D_OS_Mutex_Destroy
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Mutex_Destroy(r_drw2d_OS_Mutex_t *Mutex)
{
    r_drw2d_OS_Error_t ret;

    if (R_NULL != Mutex)
    {
        if (0 == R_OS_ThreadMutexDestroy(Mutex))
        {
            /* Succeeded */
            ret = R_DRW2D_OS_ERR_OK;
        }
        else
        {
            ret = R_DRW2D_OS_ERR_MUTEX_DESTROY;
        }
    }
    else
    {
        /* Failed: Invalid mutex handle */
        ret = R_DRW2D_OS_ERR_MUTEX_HANDLE;
    }

    return ret;
}



/***********************************************************
  Function: R_DRW2D_OS_Mutex_Lock
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Mutex_Lock(r_drw2d_OS_Mutex_t *Mutex)
{
    r_drw2d_OS_Error_t ret;

    if (R_NULL != Mutex)
    {
        if (0 == R_OS_ThreadMutexLock(Mutex))
        {
            /* Succeeded */
            ret = R_DRW2D_OS_ERR_OK;
        }
        else
        {
            ret = R_DRW2D_OS_ERR_MUTEX_LOCK;
        }
    }
    else
    {
        /* Failed: Invalid mutex handle */
        ret = R_DRW2D_OS_ERR_MUTEX_HANDLE;
    }

    return ret;
}



/***********************************************************
  Function: R_DRW2D_OS_Mutex_Unlock
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Mutex_Unlock(r_drw2d_OS_Mutex_t *Mutex)
{
    r_drw2d_OS_Error_t ret;

    if (R_NULL != Mutex)
    {
        if (0 == R_OS_ThreadMutexUnlock(Mutex))
        {
            /* Succeeded */
            ret = R_DRW2D_OS_ERR_OK;
        }
        else
        {
            ret = R_DRW2D_OS_ERR_MUTEX_UNLOCK;
        }
    }
    else
    {
        /* Failed: Invalid mutex handle */
        ret = R_DRW2D_OS_ERR_MUTEX_HANDLE;
    }

    return ret;
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
    r_drw2d_OS_Error_t ret;

    if(R_NULL != Sem)
    {
        int32_t rosRet = R_OS_SemInit(&Sem->Sem, 
                                      0 /*PShared*/,
                                      InitialValue
                                      );

        ret = (R_ERR_OK == rosRet) ? R_DRW2D_OS_ERR_OK : R_DRW2D_OS_ERR_SEM_CREATE;
    }
    else
    {
        /* Failed: Semaphore address is NULL */
        ret = R_DRW2D_OS_ERR_SEM_HANDLE;
    }

    return ret;
}



/***********************************************************
  Function: R_DRW2D_OS_Semaphore_Destroy
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Semaphore_Destroy(r_drw2d_OS_Semaphore_t *Sem)
{
    r_drw2d_OS_Error_t ret;

    if(R_NULL != Sem)
    {
        int32_t rosRet = R_OS_SemDestroy(&Sem->Sem);

        ret = (R_ERR_OK == rosRet) ? R_DRW2D_OS_ERR_OK : R_DRW2D_OS_ERR_SEM_DESTROY;
    }
    else
    {
        /* Failed: Semaphore address is NULL */
        ret = R_DRW2D_OS_ERR_SEM_HANDLE;
    }

    return ret;
}



/***********************************************************
  Function: R_DRW2D_OS_Semaphore_Wait
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Semaphore_Wait(r_drw2d_OS_Semaphore_t *Sem)
{
    r_drw2d_OS_Error_t ret;

    if(R_NULL != Sem)
    {
        int32_t rosRet = R_OS_SemWait(&Sem->Sem);

        ret = (R_ERR_OK == rosRet) ? R_DRW2D_OS_ERR_OK : R_DRW2D_OS_ERR_SEM_WAIT;
    }
    else
    {
        /* Failed: Semaphore address is NULL */
        ret = R_DRW2D_OS_ERR_SEM_HANDLE;
    }

    return ret;
}



/***********************************************************
  Function: R_DRW2D_OS_Semaphore_Post
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Semaphore_Post(r_drw2d_OS_Semaphore_t *Sem)
{
    r_drw2d_OS_Error_t ret;

    if(R_NULL != Sem)
    {
        int32_t rosRet = R_OS_SemPost(&Sem->Sem);

        ret = (R_ERR_OK == rosRet) ? R_DRW2D_OS_ERR_OK : R_DRW2D_OS_ERR_SEM_POST;
    }
    else
    {
        /* Failed: Semaphore address is NULL */
        ret = R_DRW2D_OS_ERR_SEM_HANDLE;
    }

    return ret;
}





/***********************************************************
  Section: Memory utility functions
*/



/***********************************************************
  Function: R_DRW2D_OS_Mem_Alloc
*/
void *R_DRW2D_OS_Mem_Alloc(uint32_t NumBytes, uint32_t Alignment, r_drw2d_OS_Mem_t Type)
{
    void *ret = R_NULL;

    if(NumBytes > 0)
    {
        r_cdi_HpType_t cdi_HpType = (Type == R_DRW2D_OS_MEM_CPU) ? CPU : VIDEO;
        if(0 == Alignment)
        {
            Alignment = 1;
        }

        if((0 == (Alignment & (Alignment - 1))) && (0 != Alignment))
        {
            uint8_t *actAddr = (uint8_t*)malloc(NumBytes + Alignment + sizeof(r_cdi_HpType_t));

            if(R_NULL != actAddr)
            {
                *(r_cdi_HpType_t*)actAddr = cdi_HpType;
                actAddr += sizeof(r_cdi_HpType_t);

                if(Alignment > 1)
                {
                    uint32_t actAddrOff = (((uint32_t)actAddr) & (Alignment - 1));
                    uint32_t actAddrSkip = (Alignment - actAddrOff);
                    actAddr = (actAddr + actAddrSkip);
                    actAddr[-1] = actAddrSkip;
                    ret = actAddr;
                }
                else
                {
                    *actAddr = 1;
                    ret = (actAddr + 1);
                }
            }
        }
    }

    return ret;
}



/***********************************************************
  Function: R_DRW2D_OS_Mem_Free
*/
void R_DRW2D_OS_Mem_Free(void *Addr)
{
    if(R_NULL != Addr)
    {
        uint8_t *actAddr = Addr;
        actAddr = (actAddr - actAddr[-1]);
        actAddr -= sizeof(r_cdi_HpType_t);
        free(actAddr);
    }
}



/***********************************************************
  Function: R_DRW2D_OS_Mem_Clear
*/
void R_DRW2D_OS_Mem_Clear (void *Addr, uint32_t NumBytes)
{
    if(R_NULL != Addr)
    {
        memset(Addr, 0, (size_t) NumBytes);
    }
}



/***********************************************************
  Function: R_DRW2D_OS_Mem_Copy

  Copy memory area

  Arguments:
     DestAddr - Start address of allocated memory
     NumBytes - Number of bytes to clear

*/
void R_DRW2D_OS_Mem_Copy(void *DestAddr, const void *SrcAddr, uint32_t NumBytes)
{
    memcpy(DestAddr, SrcAddr, NumBytes);
}





/*
****************************************************************************
PROJECT : Drw2D driver
FILE    : $Id: r_drw2d_os.h 7049 2015-12-14 10:21:19Z michael.golczewski $
============================================================================
DESCRIPTION
Driver for DRW2D
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

#ifndef R_DRW2D_OS_H_
#define R_DRW2D_OS_H_

#ifdef __cplusplus
extern "C" {
#endif


/* (note) Since the API must not use dynamic memory allocation we need to include the OS header file(s)
           to reserve memory for OS objects (e.g. CRITICAL_SECTION).
 */
#if defined(R_DRW2D_OS_WIN32)
#include <windows.h>
#elif defined(R_DRW2D_OS_RENESAS)
#include <r_os_api.h>
#elif defined(R_DRW2D_OS_INTEGRITY)
#include <INTEGRITY.h>
#elif defined(R_DRW2D_OS_NO_OS)
#else
#define R_DRW2D_OS_NO_OS
#endif



/***********************************************************
  Title: DRW2D OS abstraction
  
  DRW2D OS functions.

  Declaration of DRW2D OS functions.
*/



/*******************************************************************************
  Section: Types and enumerations
*/



/***********************************************************
  Enum: r_drw2d_OS_Error_t

  DRW2D OS abstraction layer error code.

  Values:
    R_DRW2D_OS_ERR_OK             - No error occured.
    R_DRW2D_OS_ERR_NOT_SUPPORTED  - (Currently) unsupported feature
    R_DRW2D_OS_ERR_PARAM_NOT_SUPPORTED  - unsupported parameter
    R_DRW2D_OS_ERR_THREAD_CREATE  - Failed to create OS-specific thread
    R_DRW2D_OS_ERR_THREAD_INFO    - Invalid thread info
    R_DRW2D_OS_ERR_THREAD_HANDLE  - Invalid thread handle
    R_DRW2D_OS_ERR_THREAD_TIMEOUT - Timeout while waiting for thread.
    R_DRW2D_OS_ERR_THREAD_WAIT    - Failed to wait for OS thread
    R_DRW2D_OS_ERR_THREAD_DESTROY - Failed to destroy OS thread
    R_DRW2D_OS_ERR_MUTEX_HANDLE   - Invalid mutex handle
    R_DRW2D_OS_ERR_MUTEX_CREATE   - Failed to create mustex
    R_DRW2D_OS_ERR_MUTEX_DESTROY  - Failed to destroy mutex
    R_DRW2D_OS_ERR_MUTEX_LOCK     - Failed to lock mutex
    R_DRW2D_OS_ERR_MUTEX_UNLOCK   - Failed to unclock mutex
    R_DRW2D_OS_ERR_SEM_HANDLE     - Invalid semaphore handle
    R_DRW2D_OS_ERR_SEM_CREATE     - Failed to create semaphore
    R_DRW2D_OS_ERR_SEM_DESTROY    - Failed to destroy semaphore
    R_DRW2D_OS_ERR_SEM_WAIT       - Failed to wait for semaphore
    R_DRW2D_OS_ERR_SEM_POST       - Failed to post semaphore
*/
typedef enum
{
    R_DRW2D_OS_ERR_OK,
    R_DRW2D_OS_ERR_NOT_SUPPORTED,
    R_DRW2D_OS_ERR_PARAM_NOT_SUPPORTED,
    R_DRW2D_OS_ERR_THREAD_CREATE,
    R_DRW2D_OS_ERR_THREAD_INFO,
    R_DRW2D_OS_ERR_THREAD_HANDLE,
    R_DRW2D_OS_ERR_THREAD_TIMEOUT,
    R_DRW2D_OS_ERR_THREAD_WAIT,
    R_DRW2D_OS_ERR_THREAD_DESTROY,
    R_DRW2D_OS_ERR_MUTEX_HANDLE,
    R_DRW2D_OS_ERR_MUTEX_CREATE,
    R_DRW2D_OS_ERR_MUTEX_DESTROY,
    R_DRW2D_OS_ERR_MUTEX_LOCK,
    R_DRW2D_OS_ERR_MUTEX_UNLOCK,
    R_DRW2D_OS_ERR_SEM_HANDLE,
    R_DRW2D_OS_ERR_SEM_CREATE,
    R_DRW2D_OS_ERR_SEM_DESTROY,
    R_DRW2D_OS_ERR_SEM_WAIT,
    R_DRW2D_OS_ERR_SEM_POST,

    NUM_R_DRW2D_OS_ERR_CODES
} r_drw2d_OS_Error_t;



/***********************************************************
  Enum: r_drw2d_OS_Mem_t

  DRW2D framework OS abstraction layer memory allocation type.

  Values:
    R_DRW2D_OS_MEM_CPU   - Memory to be used by the CPU only
    R_DRW2D_OS_MEM_VIDEO - Memory to be used as video memory
*/
typedef enum
{
	R_DRW2D_OS_MEM_CPU   = 0,
	R_DRW2D_OS_MEM_VIDEO = 1
} r_drw2d_OS_Mem_t;



/***********************************************************
  Type: r_drw2d_OS_Thread_t

  OS-specific thread handle type.

  On R_OS, this will internally be cast to <R_OS_Thread_t> (uint32_t).

  On Win32, this will internally be cast to HANDLE (void*).

  On Linux, this will internally be cast to pthread_t.
*/
#if defined(R_DRW2D_OS_WIN32)
typedef HANDLE r_drw2d_OS_Thread_t;
#elif defined(R_DRW2D_OS_RENESAS)
typedef R_OS_Thread_t r_drw2d_OS_Thread_t;
#elif defined(R_DRW2D_OS_INTEGRITY)
typedef Task r_drw2d_OS_Thread_t;
#elif defined(R_DRW2D_OS_NO_OS)
#else
#error Unable to determine r_drw2d_OS_Thread_t
#endif



/***********************************************************
  Type: r_drw2d_OS_ThreadId_t

  Unique thread identifier.

  May not necessarily be the same as the thread handle (<r_drw2d_OS_Thread_t>).

  For example, when using pthreads, the pthread_t may be a structure (implementation dependent)
   that cannot be casted to a void pointer.

  See also:
   <R_DRW2D_OS_Thread_GetId>
*/
typedef void *r_drw2d_OS_ThreadId_t;



/*******************************************************************************
  Type: r_drw2d_OS_ThreadInfo_t
*/
typedef struct r_drw2d_OS_ThreadInfo_s r_drw2d_OS_ThreadInfo_t;



/***********************************************************
  Type: r_drw2d_OS_ThreadFxn_t
  
  Thread function type.

*/
typedef void (*r_drw2d_OS_ThreadFxn_t) (r_drw2d_OS_ThreadInfo_t *ThreadInfo);



/***********************************************************
  Type: r_drw2d_OS_Mutex_t
  
  Abstract type representing an OS mutex .
*/
#if defined(R_DRW2D_OS_WIN32)
typedef CRITICAL_SECTION r_drw2d_OS_Mutex_t;
#elif defined(R_DRW2D_OS_RENESAS)
typedef R_OS_ThreadMutex_t r_drw2d_OS_Mutex_t;
#elif defined(R_DRW2D_OS_INTEGRITY)
typedef LocalMutex r_drw2d_OS_Mutex_t;
#elif defined(R_DRW2D_OS_NO_OS)
typedef void* r_drw2d_OS_Mutex_t;
#else
#error Unable to determine r_drw2d_OS_Mutex_t
#endif



/***********************************************************
  Constant: R_DRW2D_OS_THREAD_WAIT_INFINITE
  
  'Magic' timeout value for <R_DRW2D_OS_Thread_Wait>.
*/
#define R_DRW2D_OS_THREAD_WAIT_INFINITE  0





/*******************************************************************************
  Section: Structures
*/


#ifndef R_DRW2D_OS_NO_OS
/*******************************************************************************
  Struct: r_drw2d_OS_ThreadInfo_s
  
  Thread Information.

  Will be passed to the thread entry function.

  
  Members:
    Function  - Thread function
    UserData  - Pointer to User Data
    Thread    - OS-specific thread handle or structure
    Result    - Result of the thread function (should be set by thread function before it returns).
*/

typedef struct r_drw2d_OS_ThreadInfo_s
{
    r_drw2d_OS_ThreadFxn_t Function;
    void *                 UserData;
    uint32_t               Prio;
    void *                 StackAddr;
    uint32_t               StackSize;
    r_drw2d_OS_Thread_t    Thread;
    void *                 Result;
} r_drw2d_OS_ThreadInfo_s_t;
#endif /* R_DRW2D_OS_NO_OS */


/*******************************************************************************
  Struct: r_drw2d_OS_Semaphore_t
*/
typedef struct
{
#if defined(R_DRW2D_OS_WIN32)
   HANDLE Handle;
#elif defined(R_DRW2D_OS_RENESAS)
   R_OS_Sem_t Sem;
#elif defined(R_DRW2D_OS_INTEGRITY)
   Semaphore Sem;
#elif defined(R_DRW2D_OS_NO_OS)
   int8_t unused;
#else
#error Unable to determine r_drw2d_OS_Semaphore_t
#endif
} r_drw2d_OS_Semaphore_t;





/***********************************************************
  Section: Thread functions
*/



/***********************************************************
  Function: R_DRW2D_OS_Thread_Create

  Creates and starts a new thread.


  Arguments:
    ThreadInfo - Pointer to thread info. The entry *Function* and *UserData* fields must be set prior to this call.

  Returns:
    Error code. <R_DRW2D_OS_ERR_OK> if thread was started successfully.

  See also:
    <R_DRW2D_OS_Thread_Wait>, <R_DRW2D_OS_Thread_Destroy>
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Thread_Create (r_drw2d_OS_ThreadInfo_t *ThreadInfo);



/***********************************************************
  Function: R_DRW2D_OS_Thread_Wait

  Waits until the thread terminates or a time-out interval elapses.

  Unless the function timed out, the return value of the thread function will be stored in the *Result* field of the *ThreadInfo* structure.


  Arguments:
    ThreadInfo - Pointer to thread info.
       Timeout - Time-out in Milliseconds. 0 to wait indefinitely (see <R_DRW2D_OS_THREAD_WAIT_INFINITE>).

  Returns:
    Error code. <R_DRW2D_OS_ERR_OK> if the thread function returned, <R_DRW2D_OS_ERR_THREAD_TIMEOUT> if a timeout occured while waiting for the thread function to return.

  See also:
    <R_DRW2D_OS_Thread_Create>, <R_DRW2D_OS_Thread_Destroy>
    
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Thread_Wait (r_drw2d_OS_ThreadInfo_t *ThreadInfo,
                                                    uint32_t                 Timeout
                                                    );



/***********************************************************
  Function: R_DRW2D_OS_Thread_GetId

  Returns:
    Unique identifier for the current thread
*/
r_drw2d_OS_ThreadId_t R_DRW2D_OS_Thread_GetId (void);



/***********************************************************
  Function: R_DRW2D_OS_Thread_Sleep

  Sleep for the given number of milliseconds.

  Arguments:
    MilliSeconds - Number of milliseconds to sleep
*/
void R_DRW2D_OS_Thread_Sleep (uint32_t MilliSeconds);



/***********************************************************
  Function: R_DRW2D_OS_Thread_Exit

  Terminate thread. Must be called from thread.

  Arguments:
    RetVal - Return value. Stored in ThreadInfo->Result.
*/
void R_DRW2D_OS_Thread_Exit (r_drw2d_OS_ThreadInfo_t *ThreadInfo,
                                      void                    *RetVal
                                      );



/***********************************************************
  Function: R_DRW2D_OS_Thread_Destroy

  De-initializes a thread and destroys the OS specific thread handle.

  Arguments:
    ThreadInfo - Pointer to thread info.

  See also:
    <R_DRW2D_OS_Thread_Create>, <R_DRW2D_OS_Thread_Wait>

*/
r_drw2d_OS_Error_t R_DRW2D_OS_Thread_Destroy (r_drw2d_OS_ThreadInfo_t *ThreadInfo);



/***********************************************************
  Section: Mutex functions
*/



/***********************************************************
  Function: R_DRW2D_OS_Mutex_Create

  Creates and initializes an OS mutex.

  Arguments:
    Mutex - Reference to OS-specific mutex handle or structure, see <r_drw2d_OS_Mutex_t>.

  Returns:
    Error code. <R_DRW2D_OS_ERR_OK> if the mutex was created successfully.

  See also:
   <R_DRW2D_OS_Mutex_Destroy>, <R_DRW2D_OS_Mutex_Lock>, <R_DRW2D_OS_Mutex_Unlock>
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Mutex_Create (r_drw2d_OS_Mutex_t *Mutex);



/***********************************************************
  Function: R_DRW2D_OS_Mutex_Destroy

  Destroys a mutex.

  Arguments:
    Mutex - The mutex to be destroyed.

  Returns:
    Error code. <R_DRW2D_OS_ERR_OK> if the mutex has been destroyed else <R_DRW2D_OS_ERR_MUTEX_DESTROY>.

  See also:
   <R_DRW2D_OS_Mutex_Create>, <R_DRW2D_OS_Mutex_Lock>, <R_DRW2D_OS_Mutex_Unlock>
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Mutex_Destroy (r_drw2d_OS_Mutex_t *Mutex);



/***********************************************************
  Function: R_DRW2D_OS_Mutex_Lock

  Locks a mutex.

  Arguments:
    Mutex - The mutex to be locked.

  Returns:
    Error code. <R_DRW2D_OS_ERR_OK> if the mutex has been locked.

  See also:
   <R_DRW2D_OS_Mutex_Create>, <R_DRW2D_OS_Mutex_Destroy>, <R_DRW2D_OS_Mutex_Unlock>
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Mutex_Lock (r_drw2d_OS_Mutex_t *Mutex);



/***********************************************************
  Function: R_DRW2D_OS_Mutex_Unlock

  Unlocks a mutex.

  Arguments:
    Mutex - The mutex to be unlocked.

  Returns:
    Error code. <R_DRW2D_OS_ERR_OK> if the mutex has been unlocked.

  See also:
   <R_DRW2D_OS_Mutex_Create>, <R_DRW2D_OS_Mutex_Destroy>, <R_DRW2D_OS_Mutex_Lock>
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Mutex_Unlock (r_drw2d_OS_Mutex_t *Mutex);





/***********************************************************
  Section: Semaphore functions
*/



/***********************************************************
  Function: R_DRW2D_OS_Semaphore_Create

  Creates a semaphore.
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Semaphore_Create (r_drw2d_OS_Semaphore_t *Sem,
                                                         uint32_t                InitialValue
                                                         );



/***********************************************************
  Function: R_DRW2D_OS_Semaphore_Destroy

  Destroys a semaphore.
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Semaphore_Destroy (r_drw2d_OS_Semaphore_t *Sem);



/***********************************************************
  Function: R_DRW2D_OS_Semaphore_Wait

  Wait until semaphore can be decremented.
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Semaphore_Wait (r_drw2d_OS_Semaphore_t *Sem);



/***********************************************************
  Function: R_DRW2D_OS_Semaphore_Post

  Increment semaphore.
*/
r_drw2d_OS_Error_t R_DRW2D_OS_Semaphore_Post (r_drw2d_OS_Semaphore_t *Sem);





/***********************************************************
  Section: Memory utility functions
*/



/***********************************************************
  Function: R_DRW2D_OS_Mem_Alloc

  Allocate memory.

  Arguments:
     NumBytes - Number of bytes to allocate
    Alignment - Address alignment (power of two) (0/1=any, 2=align to 16bit word boundary, 4=align to 32bit dword boundary, ..)
	     Type - R_DRW2D_OS_MEM_CPU (CPU usage only) or R_DRW2D_OS_MEM_VIDEO (CPU+GPU usage)

  Returns:
    Start address of allocated memory or R_NULL if the allocation failed.
*/
void *R_DRW2D_OS_Mem_Alloc (uint32_t NumBytes, uint32_t Alignment, r_drw2d_OS_Mem_t Type);



/***********************************************************
  Function: R_DRW2D_OS_Mem_Free

  Free memory previously allocated with <R_DRW2D_OS_Mem_Alloc>.

  Arguments:
     Addr - Start address of allocated memory

*/
void R_DRW2D_OS_Mem_Free (void *Addr);



/***********************************************************
  Function: R_DRW2D_OS_Mem_Clear

  Clear memory area (fill with 0)

  Arguments:
        Addr - Start address of allocated memory
    NumBytes - Number of bytes to clear

*/
void R_DRW2D_OS_Mem_Clear (void *Addr, uint32_t NumBytes);



/***********************************************************
  Function: R_DRW2D_OS_Mem_Copy

  Copy memory area

  Arguments:
     DestAddr - Start address of allocated memory
     NumBytes - Number of bytes to clear

*/
void R_DRW2D_OS_Mem_Copy (void *DestAddr, const void *SrcAddr, uint32_t NumBytes);


#ifdef __cplusplus
}
#endif 

#endif /* R_DRW2D_OS_H_ */

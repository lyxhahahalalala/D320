/*
****************************************************************************
PROJECT : CDI
FILE    : $Id: r_cdi_rbuffer.c 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Common Driver Interface
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

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_cdi_api.h"


    

/*******************************************************************************
  Section: Macros
*/


/*******************************************************************************
  Macro: LOC_CDI_GET_PTR
*/
#define LOC_CDI_GET_PTR(ptr, num, size) ( (void*)( ((uint32_t)(ptr)) + ((num) * (size)) ) )

/*******************************************************************************
  Macro: LOC_CDI_INC_PTR
*/
#define LOC_CDI_INC_PTR(ptr, size)      {(ptr) = (void*)( (uint32_t)(ptr) + (size) );}

/*******************************************************************************
  Macro: LOC_DIO_COPY
*/
#define LOC_DIO_COPY(src,dst,size)  {                              \
    uint8_t copy_i;                                                \
    switch(size) {                                                 \
        case 1:                                                    \
            *((uint8_t*)(dst))  = *((uint8_t*)(src));                  \
            break;                                                 \
        case 2:                                                    \
            *((uint16_t*)(dst)) = *((uint16_t*)(src));                 \
            break;                                                 \
        case 4:                                                    \
            *((uint32_t*)(dst)) = *((uint32_t*)(src));                 \
            break;                                                 \
        default:                                                   \
            for(copy_i = 0u; copy_i < (size) ; copy_i++) {            \
                ((uint8_t*)(dst))[copy_i] = ((uint8_t*)(src))[copy_i]; \
            }                                                      \
            break;                                                 \
    }                                                              \
} 
    
/*******************************************************************************
  Macro: LOC_DIO_DEL
*/
#define LOC_DIO_DEL(dst,size,val)  {                              \
    uint8_t copy_i;                                                \
    switch(size) {                                                 \
        case 1:                                                    \
            *((uint8_t*)(dst))  = ((uint8_t)(val));                  \
            break;                                                 \
        case 2:                                                    \
            *((uint16_t*)(dst)) = ((uint16_t)(val));                 \
            break;                                                 \
        case 4:                                                    \
            *((uint32_t*)(dst)) = ((uint32_t)(val));                 \
            break;                                                 \
        default:                                                   \
            for(copy_i = 0u; copy_i < (size) ; copy_i++) {            \
                ((uint8_t*)(dst))[copy_i] = ((uint8_t)(val)); \
            }                                                      \
            break;                                                 \
    }                                                              \
} 
    

/*******************************************************************************
  Section: Local Functions
*/
static void *loc_RbPtrNext(r_cdi_RBuf_t *RBuf, void *Ptr);

/*******************************************************************************
  Function: loc_RbPtrNext

  Return next position in a ring buffer.

  Parameters:
  RBuf       - Ring buffer structure
  Ptr        - Current position in buffer

  Returns:
  Next position in ring buffer.
*/

static void *loc_RbPtrNext(r_cdi_RBuf_t *RBuf, void *Ptr)
{
    uint8_t size;
    void *   buf;
    
    size = RBuf->Size;
    buf  = RBuf->Buffer;
    /* Special case: End of buffer, wrap around required */
    if (Ptr == LOC_CDI_GET_PTR( buf, RBuf->Num - 1u, size)) 
    {   
        Ptr = LOC_CDI_GET_PTR( buf, 0u, size);
    } 
    /* Normal case: Increase pointer by data size */
    else 
    {
        LOC_CDI_INC_PTR(Ptr, size);
    }
    return Ptr;
}


/*******************************************************************************
  Section: Global API Ring Buffer Functions
*/

/*******************************************************************************
 Function: R_CDI_RbSetup
*/

 int32_t R_CDI_RbSetup(r_cdi_RBuf_t *RBuf, void *Buf, uint32_t Num, uint8_t Size)
 {
    RBuf->Start  = Buf;
    RBuf->Stop   = Buf;
    RBuf->Buffer = Buf;
    RBuf->Num    = Num;
    RBuf->Size   = Size;
    return 0;
}


/*******************************************************************************
 Function: R_CDI_RbWrite
*/
 uint32_t R_CDI_RbWrite(r_cdi_RBuf_t *RBuf, void *Buffer, uint32_t Num) 
{
    void  *next;
    uint32_t  n;

    next = RBuf->Stop;
    n    = 0u;
    while (0 != Num) 
    {
        next = loc_RbPtrNext(RBuf, next);
        if (next == RBuf->Start) 
        {
            break;
        }
        LOC_DIO_COPY(Buffer, RBuf->Stop, RBuf->Size);
        RBuf->Stop  = next;
        LOC_CDI_INC_PTR(Buffer, RBuf->Size);
        Num--;
        n++;
    }
    return n;
}


/*******************************************************************************
 Function: R_CDI_RbRead
*/
 uint32_t R_CDI_RbRead(r_cdi_RBuf_t *RBuf, void *Buffer, uint32_t MaxNum) 
{
    uint32_t n;

    n = 0u;
    while (0 != MaxNum) 
    {
        if (RBuf->Start == RBuf->Stop) 
        {
            break;
        }
        LOC_DIO_COPY(RBuf->Start, Buffer, RBuf->Size);
#if 0
        LOC_DIO_DEL(RBuf->Start, RBuf->Size, 0xff);     /* clear for debugging */
#endif
        RBuf->Start = loc_RbPtrNext(RBuf, RBuf->Start);
        n++;
        LOC_CDI_INC_PTR(Buffer, RBuf->Size);
        MaxNum--;
    }
    return n;
}

/*******************************************************************************
 Function: R_CDI_RbSize
*/
 uint32_t R_CDI_RbNum(r_cdi_RBuf_t *RBuf) 
 {
    uint32_t start;
    uint32_t stop;
    uint32_t ret;
   
    start   = (uint32_t) RBuf->Start;
    stop    = (uint32_t) RBuf->Stop;
    ret     = 0u;
    
    if(start > stop) 
    {
        ret  = (RBuf->Num) * (RBuf->Size);
    }
    ret += stop;
    ret -= start;
    if(RBuf->Size > 1) 
    {
        ret /= RBuf->Size;
    }
    return ret;
}

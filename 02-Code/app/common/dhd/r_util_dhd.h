/***********************************************************************************************
PROJECT : VLIB
FILE    : $Id: r_util_dhd.h 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
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
#ifndef R_UTIL_DHD_H_
#define R_UTIL_DHD_H_


#ifdef __cplusplus
extern "C" {
#endif
/*******************************************************************************
  Section: Defines
*/
#define DHD_ALIGN(ADDR,BLOCK)      ( (ADDR+(BLOCK-1)) & ~(BLOCK-1) )
#ifndef DHD_USER_INSTANCES
  #define DHD_USER_INSTANCES (1u)
#endif
#ifndef DHD_JOBSIZE
  #define DHD_JOBSIZE      (512u)
#endif
#ifndef DHD_JOBCOUNT
  #define DHD_JOBCOUNT       (8u)
#endif
#ifndef DHD_RINGSIZE
  #define DHD_RINGSIZE    (1024u)
#endif
/* one DHD context with a config with 2 tex units, 3 color unit passes and 4 const colors */
#define DHD_STATESTORAGE (480u) 

/* CAUTION: you have to adapt this when using dhd_alloc* manually or when using 
 * DHD performance counter or when using DHD command list feature! 
 */
#define DHD_RINGSIZE_ALLOC      DHD_ALIGN(DHD_RINGSIZE+4,128) 
#define DHD_STATESTORAGE_ALLOC  DHD_ALIGN(DHD_STATESTORAGE,128)
#define DHD_MEMORY_SIZE         (DHD_RINGSIZE_ALLOC+DHD_USER_INSTANCES*(DHD_STATESTORAGE_ALLOC+DHD_JOBCOUNT*DHD_JOBSIZE))
/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: SetupMemManager

  This function sets up the CDI memory management.
  It shows how the memory is initialised.

  Parameter:
    void

  Returns:
    void
*/
dhd_uint32_t R_UTIL_DHD_Config(dhd_gpu_ptr_t Base, dhd_uint32_t VidSize, r_cdi_Heap_t *Heap);
void R_UTIL_DHD_Init(dhd_uint32_t Unit);
dhd_uint32_t R_UTIL_DHD_DeInit(dhd_uint32_t Unit);

#ifdef R_USE_FASTSOFTDAVE
void R_UTIL_DHD_FSDRegBuf(void* Buf, uint32_t Size);
#endif


#ifdef __cplusplus
}
#endif

#endif /* R_UTIL_DHD_H_ */

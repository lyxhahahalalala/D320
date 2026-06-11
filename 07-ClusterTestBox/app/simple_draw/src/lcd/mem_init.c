/************************************************************************************************
PROJECT : simple_draw app
FILE    : $Id: vo_init.c 1175 2013-08-02 13:15:50Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Init functions for memory
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
#include "r_typedefs.h"     /* Renesas basic types, e.g. uint32_t */
#include "r_config_d1x.h"
#include "r_dev_api.h"
#include "r_cdi_api.h"
#include "mem_init.h"
#include "davehd_driver.h"
#include "r_util_dhd.h"
#include "config.h"
#include "main.h"

#include "r_ddb_api.h"
#include "r_wm_api.h"
/*******************************************************************************
  Section: Local defines
*/
#define LOC_VID_HP_BLOCK_SIZE  (128u)
#define LOC_VID_HP_BLOCK_NB    (VRAM_HEAP_SIZE / LOC_VID_HP_BLOCK_SIZE)
#define ALIGN(ADDR,BLOCK)      ( (ADDR+(BLOCK-1)) & ~(BLOCK-1) )
#define VRAM_HEAP_BASE         ALIGN((loc_VRAM0+DHD_MEMORY_SIZE), LOC_VID_HP_BLOCK_SIZE)
#if LOC_VOWE_ENABLE
#define VRAM_HEAP_SIZE         (LOC_VOWE_FRAME_BUF_ADDR - VRAM_HEAP_BASE)
#else
#define VRAM_HEAP_SIZE         ((loc_VRAM0 + loc_VRAM0_SIZE) - VRAM_HEAP_BASE)
#endif

/*******************************************************************************
  Section: Local variables
  
*/


r_cdi_Heap_t       loc_VRAM_heap;
r_cdi_Heap_t       loc_lRAM_heap;
r_cdi_HpBlkIndex_t loc_lRAM_heapIdxList[LOC_CPU_HP_BLOCK_NB * LOC_CPU_HP_BLOCK_SIZE];
//r_cdi_HpBlkIndex_t loc_VRAM_heapIdxList[LOC_VID_HP_BLOCK_NB];
uint32_t loc_VRAM0               = 0;
uint32_t loc_VRAM0_SIZE          = 0;

/*******************************************************************************
  Section: Local functions

*/

/*******************************************************************************
  Section: Global functions
*/

/*******************************************************************************
  Function: SetupMemManager

    See: main.h for details
*/
void loc_SetupMemManager(void)
{
    uint32_t x;
    r_dev_Device_t dev;

    dev = R_DEV_GetDev();

    /* D1M1 */
    if(dev == R_DEV_R7F701404 ||
            dev == R_DEV_R7F701405 ||
            dev == R_DEV_R7F701406 ||
            dev == R_DEV_R7F701407)
    {
        loc_VRAM0 = D1M1_VRAM0;
        loc_VRAM0_SIZE = D1M_VRAM0_SIZE;
    }
    /* D1M2 */
    else if(dev == R_DEV_R7F701408 ||
            dev == R_DEV_R7F701410 ||
            dev == R_DEV_R7F701411 ||
            dev == R_DEV_R7F701412)
    {
        loc_VRAM0 = D1M2_VRAM0;
        loc_VRAM0_SIZE = D1M_VRAM0_SIZE + D1M2_VRAM1_SIZE;
    }
    else
    {
        loc_Error(ERR_NG);
    }

    x = R_CDI_InitHeapManager((uint32_t)&loc_lRAM_heapIdxList,
                              &loc_lRAM_heap,
                              0,
                              LOC_CPU_HP_BLOCK_NB,
                              LOC_CPU_HP_BLOCK_SIZE);
    if (x == 1)
    {
         loc_Error(ERR_NG);
    }

    x = R_CDI_InitHeapManager(VRAM_HEAP_BASE,
                              &loc_VRAM_heap,
                              0,
                              LOC_VID_HP_BLOCK_NB,
                              LOC_VID_HP_BLOCK_SIZE);
    if (x == 1)
    {
         loc_Error(ERR_NG);
    }

}


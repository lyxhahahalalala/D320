#ifndef _FCL_PRIV_H
#define _FCL_PRIV_H

#include "types.h"
/* Return type for the function Fls_Tc_GetLogSectorSize */
typedef struct
{
    uint32_t  dBlkSize_uo;         /* Erase status */
    uint32_t  StartAddressTmp_uo;  /* Amount of erased bytes */
} Fcl_BlockInfoRet_tst;

#define FLS_NUM_BLOCK	134

extern const Fcl_BlockInfoRet_tst Fcl_BlockDesc_C[];

#endif

/*
****************************************************************************
PROJECT : CDI
FILE    : $Id: r_cdi_memory.c 7682 2016-02-16 13:05:29Z florian.zimmermann $
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
#ifdef USE_ROS
#include "r_os_api.h"
#endif

#ifdef MEM_DEBUG
#include <stdio.h>
#endif


/*******************************************************************************
  Section: Local Variables
*/
/*******************************************************************************
  Variable: locCpuHeapMng

  Local CPU heap manager
  This local variable is initialised once in the function <eeCdi_InitCpuHeapManager>
  with the given parameters.
  This function must be called when initialising the hardware.
  
*/                         
static r_cdi_HeapMng_t locCpuHeapMng;

/*******************************************************************************
  Variable: locVidHeapMng

  Local video heap manager
  This local variable is initialised once in the function <eeCdi_InitVidHeapManager>
  with the given parameters.
  This function must be called when initialising the hardware.
  
*/                         
static r_cdi_HeapMng_t locVidHeapMng;

/*******************************************************************************
  Variable: locSpecialModeHeapMng

  Local SPECIAL_MODE heap manager
    SPECIAL_MODE is a special type of video memory. It is currently only used in DK4-H (-> VMEM).
    If you use SPECIAL_MODE, make sure that your device supports this type of memory.
    
  This local variable is initialised once in the function <eeCdi_InitSpecialModeHeapManager>
  with the given parameters.
  This function must be called when initialising the hardware.
  
*/                         
static r_cdi_HeapMng_t locSpecialModeHeapMng;

/*******************************************************************************
  Variable: locNextCpuBlockIndex

  
  
*/                         
static r_cdi_HpBlkIndex_t locNextCpuBlockIndex = 0u;

/*******************************************************************************
  Variable: locNextVidBlockIndex

  
  
*/                         
static r_cdi_HpBlkIndex_t locNextVidBlockIndex = 0u;

/*******************************************************************************
  Variable: locNextSpecialModeBlockIndex

  
  
*/                         
static r_cdi_HpBlkIndex_t locNextSpecialModeBlockIndex = 0u;

#ifdef USE_ROS
static R_OS_ThreadMutex_t locCdiMemLock;  
#endif

static uint32_t locFreeVideoBlocks = 0u;
static uint32_t locFreeCPUBlocks = 0u;
static uint32_t locFreeSpecialModeBlocks = 0u;
static uint32_t locNextCPUFreeBlockNb = 0u;
static uint32_t locNextVideoFreeBlockNb = 0u;
static uint32_t locNextSpecialModeFreeBlockNb = 0u;


/*******************************************************************************
  Section: Global API Memory Controller Functions
*/

/*******************************************************************************
 Function: R_CDI_InitHeapManager
*/
uint32_t R_CDI_InitHeapManager(
        uint32_t            HeapAddr,
        r_cdi_Heap_t        *HeapMng, 
        r_cdi_HpBlkIndex_t  *HeapIdxList, 
        uint32_t             NbBlocks, 
        uint32_t             BlkSize)
{

    uint32_t err = 0u;
    uint32_t index;
    
    /* If the pointer HeapIdxList is NULL, then the index list will be placed 
       into the memory whose heap we are initializing just now */
    if (0 == HeapIdxList)
    {
        /* How much memory is required for the HeapIdxList */
        const uint32_t HeapIdxListSize = 
                    sizeof(r_cdi_HpBlkIndex_t) * NbBlocks;
        /* Size of the HeapIdxList in terms of Blocks (Rounded UP) */
        const uint32_t HeapIdxListSizeBlks = 
                    (HeapIdxListSize + (BlkSize-1)) / BlkSize;
        /* Number of Blocks left for normal CDI operation */
        const uint32_t NbBlocksInternal = 
                    NbBlocks - HeapIdxListSizeBlks;
        /* Starting point of integrated HeapIdxList */
        r_cdi_HpBlkIndex_t *HeapIdxListInternal = 
                    (r_cdi_HpBlkIndex_t*)(HeapAddr + (BlkSize*NbBlocksInternal));
        
        /* Transfer new information to existing variables */
        HeapIdxList = HeapIdxListInternal;
        NbBlocks =    NbBlocksInternal;
    }
    
    /* test if the pointers different than NULL */
    if ((0 != NbBlocks) && (0 != HeapAddr)  && ( 0 != HeapIdxList) && ( 0 != HeapMng))
    {
        HeapMng->BaseAddr        = HeapAddr;
        HeapMng->NbOfBlock       = NbBlocks;
        HeapMng->BlockSize       = BlkSize;
        HeapMng->BlockIdxList    = HeapIdxList;
        HeapMng->FreeBlocks      = NbBlocks;
        HeapMng->NextFreeBlockNb = 0u;
        HeapMng->NextBlockIdx    = 0u;
        /* Initialise the block list */
        for (index = 0u; index < NbBlocks; index++)
        {
            *HeapIdxList = 0u;
            HeapIdxList++;
        }
        HeapMng->NextBlockIdx++;
    }
    else 
    {
        err = 1u;
    }
#ifdef USE_ROS
    R_OS_ThreadMutexInit(&locCdiMemLock, 0);
#endif
    return err;
}


/*******************************************************************************
 Function: R_CDI_InitCpuHeapManager
*/
uint32_t R_CDI_InitCpuHeapManager(
        uint32_t hpaddr, 
        r_cdi_HpBlkIndex_t *hpblkindexlist, 
        uint32_t nbblk, 
        uint32_t blksize)
{

    uint32_t err = 0u;
    uint32_t index;
    /* test if the pointers different than NULL */
    if ((0 != hpaddr)  && ( 0 != hpblkindexlist))
    {
        locCpuHeapMng.BaseAddr  = hpaddr;
        locCpuHeapMng.NbOfBlock = nbblk;
        locCpuHeapMng.BlockSize = blksize;
        locCpuHeapMng.Type      = CPU;
        locCpuHeapMng.BlockIndexList = hpblkindexlist;
        locFreeCPUBlocks = nbblk;
        locNextCPUFreeBlockNb = 0u;
        /* Initialise the block list */
        for (index = 0u; index < nbblk; index++)
        {
            *hpblkindexlist = 0u;
            hpblkindexlist++;
        }
        locNextCpuBlockIndex++;
    }
    else 
    {
        err = 1u;
    }
#ifdef USE_ROS
    R_OS_ThreadMutexInit(&locCdiMemLock, 0);
#endif
    return err;
}

/*******************************************************************************
 Function: R_CDI_InitVidHeapManager
*/

uint32_t R_CDI_InitVidHeapManager(
        uint32_t hpaddr, 
        r_cdi_HpBlkIndex_t * hpblkindexlist, 
        uint32_t nbblock, 
        uint32_t blksize)
{

    uint32_t err = 0u;
    uint32_t index;
    /* test if the pointers different than NULL */
    if ((0 != hpaddr)  && ( 0 != hpblkindexlist))
    {
        /* Initialise the heap manager */
        locVidHeapMng.BaseAddr  = hpaddr;
        locVidHeapMng.NbOfBlock = nbblock;
        locVidHeapMng.BlockSize = blksize;
        locVidHeapMng.Type      = VIDEO;
        locFreeVideoBlocks      = nbblock;
        locNextVideoFreeBlockNb = 0u;
        locVidHeapMng.BlockIndexList = hpblkindexlist;
        /* Initialise the block list */
        for (index = 0u; index < nbblock; index++)
        {
            *hpblkindexlist = 0u;
            hpblkindexlist++;
        }
        locNextVidBlockIndex++; 
    }
    else 
    {
        err = 1u;
    }
    return err;
}

/*******************************************************************************
 Function: R_CDI_InitSmHeapManager
*/

uint32_t R_CDI_InitSmHeapManager(
        uint32_t hpaddr, 
        r_cdi_HpBlkIndex_t * hpblkindexlist, 
        uint32_t nbblock, 
        uint32_t blksize)
{

    uint32_t err = 0u;
    uint32_t index;
    /* test if the pointers different than NULL */
    if ((0 != hpaddr)  && ( 0 != hpblkindexlist))
    {
        /* Initialise the heap manager */
        locSpecialModeHeapMng.BaseAddr  = hpaddr;
        locSpecialModeHeapMng.NbOfBlock = nbblock;
        locSpecialModeHeapMng.BlockSize = blksize;
        locSpecialModeHeapMng.Type      = SPECIAL_MODE;
        locFreeSpecialModeBlocks        = nbblock;
        locSpecialModeHeapMng.BlockIndexList = hpblkindexlist;
        locNextSpecialModeFreeBlockNb = 0u;
        /* Initialise the block list */
        for (index = 0u; index < nbblock; index++)
        {
            *hpblkindexlist = 0u;
            hpblkindexlist++;
        }
        locNextSpecialModeBlockIndex++; 
    }
    else 
    {
        err = 1u;
    }
    return err;
}

/*******************************************************************************
 Function: R_CDI_HeapAlloc
*/
uint32_t R_CDI_HeapAlloc(uint32_t size, r_cdi_HpType_t type)
{
    uint32_t addr = 1u;
    uint32_t allocnbblk = 0u;
    uint32_t adjfreeblk = 0u;
    int32_t tmp;
    uint32_t i;
    r_cdi_HeapMng_t *lochpmng;
    r_cdi_HpBlkIndex_t   *locblkindexlist;
    r_cdi_HpBlkIndex_t   startblk;
    r_cdi_HpBlkIndex_t   *currentindex;
    uint32_t  *freeCount;
    uint32_t  *nextBlock;
    uint8_t leave = 0u; 
    r_cdi_HpBlkIndex_t prevBlkIndex = 0;
    r_cdi_HpBlkIndex_t nextBlkIndex = 0;

    
#ifdef USE_ROS
    R_OS_ThreadMutexLock(&locCdiMemLock);
#endif
    /* Check memory type and get the pointer to the corresponding heap manager.*/
    /* if the type is incorrect then 0 is returned */
    if((type == CPU) && (0 != locCpuHeapMng.BaseAddr))
    {
        lochpmng   = &locCpuHeapMng;
        currentindex = &locNextCpuBlockIndex;
        freeCount = &locFreeCPUBlocks;
        nextBlock = &locNextCPUFreeBlockNb;
    } 
    else if((type == SPECIAL_MODE) && (0 != locSpecialModeHeapMng.BaseAddr))
    {
        lochpmng = &locSpecialModeHeapMng;
        currentindex = &locNextSpecialModeBlockIndex;
        freeCount = &locFreeSpecialModeBlocks;  
        nextBlock = &locNextSpecialModeFreeBlockNb; 
    }
    else 
    {
        if((type == VIDEO) && (0 != locVidHeapMng.BaseAddr) )
        {
            lochpmng = &locVidHeapMng;
            currentindex = &locNextVidBlockIndex;
            freeCount = &locFreeVideoBlocks;
            nextBlock = &locNextVideoFreeBlockNb;
        } 
        else 
        {
            addr = 0u;
        }
    }
    if((*freeCount) <= 0)
    {
        addr = 0u;
    }
    
    /* Calculate number of required blocks*/
    if (addr != 0u)
    {
        
        tmp = size;
        /* calculate number of blocks needed*/
        while (tmp > 0)
        {
            allocnbblk++;
            tmp -= lochpmng->BlockSize;;
        }
        /* if not enough blocks left */
        if((*freeCount) <= allocnbblk)
        {
            addr = 0u;
        }
    }
    
    if(addr != 0u) 
    {
        /* Search for n free adjacent blocks */
        locblkindexlist = lochpmng->BlockIndexList;
        addr = 0u;
        i = *nextBlock;
        while (i != ((*nextBlock) - 1)) 
        {
            if (*nextBlock == 0u) 
            {
                if (i == (lochpmng->NbOfBlock - 1)) 
                {
                    leave = 1u; 
                }
            }
            if (0u == leave) 
            {
                if (*(locblkindexlist + i ) == 0u) 
                {
                    if (adjfreeblk == 0u) 
                    {
                        startblk = i;
                    }
                    adjfreeblk++;
                } 
                else 
                {
                    adjfreeblk = 0u;
                    startblk   = 0u;
                }
    
                /* When n free adjacent block are found, all blocks' status are set to used */
                /* Start address is then returned */
                if (adjfreeblk == allocnbblk) 
                {
                    /* If there are blocks before and/or after the current block to be allocated,
                       check their indices and adapt the current block index if necessary. */
                    /* We don't need to check for a previous block, if we are at the start of the list */
                    if (startblk > 0)
                    {
                        prevBlkIndex = *(lochpmng->BlockIndexList + (startblk - 1));
                    }
                    /* We don't need to check for a next block, if we are at the end of the list */
                    if ((startblk + adjfreeblk) < lochpmng->NbOfBlock)
                    {
                        nextBlkIndex = *(lochpmng->BlockIndexList + (startblk + adjfreeblk));
                    }
                    
                    /* Increment the index to be used for allocation if it is already used by the 
                       neighboring blocks */  /* This loop will be executed two times at maximum */
                    while ( ((*currentindex) == prevBlkIndex) || ((*currentindex) == nextBlkIndex) )
                    {
                        (*currentindex)++;
                        if ( 0 == (*currentindex))
                        {
                            (*currentindex) = 1;
                        }
                    }
                    
                    /* Calculate address of allocated memory */
                    addr = lochpmng->BaseAddr + (startblk * lochpmng->BlockSize);
                    /* For the blocks that have been found to be free by above searching pass, 
                       allocate them for this operation by writing the current block index into them. */
                    while(adjfreeblk != 0u) 
                    {
                        *(lochpmng->BlockIndexList + startblk) = *currentindex;
                        startblk++;
                        adjfreeblk--;
                        (*freeCount)--;
                    }
                    
                    /* Increment index for next allocation operation. 
                       In case of an overflow, set index to 1, as zero is reserved for empty blocks. */
                    (*currentindex)++;
                    if ( 0 == (*currentindex))
                    {
                        (*currentindex) = 1;
                    }
                    
                    (*nextBlock) = startblk  % lochpmng->NbOfBlock;
                    leave = 1u; 
                }
                
                if (0u == leave) 
                {    
                    i = (i + 1) % lochpmng->NbOfBlock;
                    if(i == 0u) 
                    {
                        /* we are back to the beginning of the heap */
                        /* then reset the startblock and the adjfreeblk variable*/
                        /* We do not want to have an allocation divided
                           between the end and the start of the heap */
                        
                        adjfreeblk = 0u;
                        startblk   = 0u;
                    }
                }
            } /* end: if (0u == leave)*/
            
            /* check for break conditions */
            if (0 != leave) 
            {
                break; 
            } 
        } /* end: while (i != ((*nextBlock) - 1))  */
    }


    
#ifdef MEM_DEBUG    
    /*  debug code to make a breakpoint32_t on out of memory possible */
    if(!addr)
    {
      if(type==VIDEO)
      {
        addr = 0u; /* break here */
      }
    }
#endif
#ifdef USE_ROS
    R_OS_ThreadMutexUnlock(&locCdiMemLock);
#endif
    return addr;
}



/*******************************************************************************
 Function: R_CDI_Alloc
*/
void * R_CDI_Alloc(r_cdi_Heap_t *HeapMng, uint32_t Size)
{
    uint32_t            addr           = 1u;
    uint32_t            allocNbBlocks  = 0u;
    uint32_t            adjfreeblk     = 0u;
    int32_t             tmp;
    uint32_t            i;
    r_cdi_Heap_t       *lochpmng;
    r_cdi_HpBlkIndex_t  *locblkindexlist;
    r_cdi_HpBlkIndex_t  startblk;
    r_cdi_HpBlkIndex_t *currentindex;
    r_cdi_HpBlkIndex_t *freeCount;
    r_cdi_HpBlkIndex_t *nextBlock;
    uint8_t             leave = 0u; 
    r_cdi_HpBlkIndex_t prevBlkIndex = 0;
    r_cdi_HpBlkIndex_t nextBlkIndex = 0;
    
#ifdef USE_ROS
    R_OS_ThreadMutexLock(&locCdiMemLock);
#endif
    if(0 != HeapMng->BaseAddr)
    {
        lochpmng        = HeapMng;
        currentindex    = &(HeapMng->NextBlockIdx);
        freeCount       = &(HeapMng->FreeBlocks);
        nextBlock       = &(HeapMng->NextFreeBlockNb);
    } 
    else 
    {
        addr = 0u;
    }
    if((*freeCount) <= 0)
    {
        addr = 0u;
    }
    
    if (0 == Size)
    {
        addr = 0u;
    }
    
    /* Calculate number of required blocks*/
    if (addr != 0u)
    {
        tmp = Size;
        /* calculate number of blocks needed*/
        while (tmp > 0)
        {
            allocNbBlocks++;
            tmp -= lochpmng->BlockSize;
        }
        /* if not enough blocks left */
        if((*freeCount) <= allocNbBlocks)
        {
            addr = 0u;
        }
    }
    
    if(addr != 0u) 
    {
        /* Search for n free adjacent blocks */
        locblkindexlist = lochpmng->BlockIdxList;
        addr = 0u;
        i = *nextBlock;
        /* Search through the block index until we are at the starting point again */
        while (i != ((*nextBlock) - 1)) 
        {
            /* Special case: When the starting point was zero, stop when we reach the end of the index */
            if (*nextBlock == 0u) 
            {
                if (i == (lochpmng->NbOfBlock - 1)) 
                {
                   leave = 1u; 
                }
            }
            if (0u == leave) 
            {
                /* If block index states that the current block is free, take it as starting point */
                /* With the following iterations, check that the adjacent block are also free */
                if (*(locblkindexlist + i ) == 0u) 
                {
                    if (adjfreeblk == 0u) 
                    {
                        startblk = i;
                    }
                    adjfreeblk++;
                } 
                else 
                {
                    adjfreeblk = 0u;
                    startblk   = 0u;
                }
    
                /* When n free adjacent block are found, all blocks' status are set to used*/
                /* Start address is then returned */
                if (adjfreeblk == allocNbBlocks) 
                {
                    /* If there are blocks before and/or after the current block to be allocated,
                       check their indices and adapt the current block index if necessary. */
                    /* We don't need to check for a previous block, if we are at the start of the list */
                    if (startblk > 0)
                    {
                        prevBlkIndex = *(lochpmng->BlockIdxList + (startblk - 1));
                    }
                    /* We don't need to check for a next block, if we are at the end of the list */
                    if ((startblk + adjfreeblk) < lochpmng->NbOfBlock)
                    {
                        nextBlkIndex = *(lochpmng->BlockIdxList + (startblk + adjfreeblk));
                    }
                    
                    /* Increment the index to be used for allocation if it is already used by the 
                       neighboring blocks */  /* This loop will be executed two times at maximum */
                    while ( ((*currentindex) == prevBlkIndex) || ((*currentindex) == nextBlkIndex) )
                    {
                        (*currentindex)++;
                        if ( 0 == (*currentindex))
                        {
                            (*currentindex) = 1;
                        }
                    }

                    /* Calculate address of allocated memory */
                    addr = lochpmng->BaseAddr + (startblk * lochpmng->BlockSize);
                    /* Reserve allocated memory in the block index */
                    while(adjfreeblk != 0u) 
                    {
                        *(lochpmng->BlockIdxList + startblk) = *currentindex;
                        startblk++;
                        adjfreeblk--;
                        (*freeCount)--;
                    }
                    /* Increase the index that uniquely identifies one allocation operation */
                    /* In case of an overflow, set index to 1, as zero is reserved for empty blocks. */
                    (*currentindex)++;
                    if ( 0 == (*currentindex))
                    {
                        (*currentindex) = 1;
                    }
                    /* Set next free block number to be directly behind the just allocated memory */
                    (*nextBlock) = startblk  % lochpmng->NbOfBlock;
                    leave = 1u; 
                }
                
                /* Proceed to go through the block index with each iteration */
                if (0u == leave) 
                {    
                    i = (i + 1) % lochpmng->NbOfBlock;
                    if(i == 0u) 
                    {
                        /* we are back to the beginning of the heap */
                        /* then reset the startblock and the adjfreeblk variable*/
                        /* We do not want to have an allocation divided
                           between the end and the start of the heap */
                        
                        adjfreeblk = 0u;
                        startblk   = 0u;
                    }
                }
            } /* if (0u == leave)*/
            /* check for break conditions */
            if (0 != leave) 
            {
                break; 
            } 
        } /* while (i != ((*nextBlock) - 1))  */
    }
    
#ifdef MEM_DEBUG    
    /*  debug code to make a breakpoint32_t on out of memory possible */
    if(!addr)
    {
        addr = 0u; /* break here */
    }
#endif
#ifdef USE_ROS
    R_OS_ThreadMutexUnlock(&locCdiMemLock);
#endif
    return (void *)addr;
}

/*******************************************************************************
 Function: R_CDI_AllocAlign
 */
void * R_CDI_AllocAlign(r_cdi_Heap_t *HeapMng, uint32_t Size, uint32_t Alignment)
{
    uint32_t            addr           = 1u;
    uint32_t            allocNbBlocks  = 0u;
    uint32_t            adjfreeblk     = 0u;
    int32_t             tmp;
    uint32_t            i;
    uint32_t            align          = 1u;
    uint32_t            alignOffset    = 0u;
    r_cdi_Heap_t       *lochpmng;
    r_cdi_HpBlkIndex_t  *locblkindexlist;
    r_cdi_HpBlkIndex_t  startblk       = 0u;
    r_cdi_HpBlkIndex_t *currentindex;
    r_cdi_HpBlkIndex_t *freeCount;
    r_cdi_HpBlkIndex_t *nextBlock;
    uint8_t             leave = 0u; 
    r_cdi_HpBlkIndex_t prevBlkIndex = 0;
    r_cdi_HpBlkIndex_t nextBlkIndex = 0;
    
#ifdef USE_ROS
    R_OS_ThreadMutexLock(&locCdiMemLock);
#endif
    if(0 != HeapMng->BaseAddr)
    {
        lochpmng        = HeapMng;
        currentindex    = &(HeapMng->NextBlockIdx);
        freeCount       = &(HeapMng->FreeBlocks);
        nextBlock       = &(HeapMng->NextFreeBlockNb);
    } 
    else 
    {
        addr = 0u;
    }
    if((*freeCount) <= 0)
    {
        addr = 0u;
    }
    
    /* Calculate number of required blocks*/
    if (addr != 0u)
    {
        tmp = Size;
        /* calculate number of blocks needed*/
        while (tmp > 0)
        {
            allocNbBlocks++;
            tmp -= lochpmng->BlockSize;
        }
        /* if not enough blocks left */
        if((*freeCount) <= allocNbBlocks)
        {
            addr = 0u;
        }
    }
    
    /* Calculate alignment of block index number to mathc required alignment */
    if (Alignment != 0)
    {
        /* If <Alignment> is smaller than <BlockSize>, the alignment is given inherently. */
        /* <Alignment> and <BlockSize> are expected to be powers of two, 
           otherwise a correct alignment might be technically imposssible */
        /* The requested <Alignment> can be bigger than the alignment of <BaseAddr>.
           As long as the <BaseAddr> is aligned to <BlockSize>, this function will 
           find an address which is aligned to the system's address space. */
        if (Alignment >= lochpmng->BlockSize)
        {
            uint32_t virtualIndex;
            
            align = Alignment / lochpmng->BlockSize;
            
            /* What "virtual index" would the first block of our index list have 
               if we started counting at the address 0x00000000 ? */
            virtualIndex = lochpmng->BaseAddr / lochpmng->BlockSize;
            
            /* If the virtual index is not a multiple of our alignment <align>, 
               we need to add an offset. */
            alignOffset = (virtualIndex % align);
        }
    }
    if(addr != 0u) 
    {
        /* Search for n free adjacent blocks */
        locblkindexlist = lochpmng->BlockIdxList;
        addr = 0u;
        i = *nextBlock;
        /* Search through the block index until we are at the starting point again */
        while (i != ((*nextBlock) - 1)) 
        {
            /* Special case: When the starting point was zero, stop when we reach the end of the index */
            if (*nextBlock == 0u) 
            {
                if (i == (lochpmng->NbOfBlock - 1)) 
                {
                   leave = 1u; 
                }
            }
            if (0u == leave) 
            {
                /* If block index states that the current block is free, take it as starting point */
                /* With the following iterations, check that the adjacent block are also free */
                if (*(locblkindexlist + i ) == 0u) 
                {
                    /* Only take it as a starting point if it matches our alignment requirement */
                    /*  .. or if a starting point was already found */
                    if ((startblk != 0) || (((i+alignOffset) % align) == 0u))
                    {
                        if (adjfreeblk == 0u) 
                        {
                            startblk = i;
                        }
                        
                        adjfreeblk++;
                    }
                } 
                else 
                {
                    adjfreeblk = 0u;
                    startblk   = 0u;
                }
    
                /* When n free adjacent block are found, all blocks' status are set to used*/
                /* Start address is then returned */
                if (adjfreeblk == allocNbBlocks) 
                {
                    /* If there are blocks before and/or after the current block to be allocated,
                       check their indices and adapt the current block index if necessary. */
                    /* We don't need to check for a previous block, if we are at the start of the list */
                    if (startblk > 0)
                    {
                        prevBlkIndex = *(lochpmng->BlockIdxList + (startblk - 1));
                    }
                    /* We don't need to check for a next block, if we are at the end of the list */
                    if ((startblk + adjfreeblk) < lochpmng->NbOfBlock)
                    {
                        nextBlkIndex = *(lochpmng->BlockIdxList + (startblk + adjfreeblk));
                    }
                    
                    /* Increment the index to be used for allocation if it is already used by the 
                       neighboring blocks */  /* This loop will be executed two times at maximum */
                    while ( ((*currentindex) == prevBlkIndex) || ((*currentindex) == nextBlkIndex) )
                    {
                        (*currentindex)++;
                        if ( 0 == (*currentindex))
                        {
                            (*currentindex) = 1;
                        }
                    }

                    /* Calculate address of allocated memory */
                    addr = lochpmng->BaseAddr + (startblk * lochpmng->BlockSize);
                    /* Reserve allocated memory in the block index */
                    while(adjfreeblk != 0u) 
                    {
                        *(lochpmng->BlockIdxList + startblk) = *currentindex;
                        startblk++;
                        adjfreeblk--;
                        (*freeCount)--;
                    }
                    /* Increase the index that uniquely identifies one allocation operation */
                    /* In case of an overflow, set index to 1, as zero is reserved for empty blocks. */
                    (*currentindex)++;
                    if ( 0 == (*currentindex))
                    {
                        (*currentindex) = 1;
                    }
                    /* Set next free block number to be directly behind the just allocated memory */
                    (*nextBlock) = startblk  % lochpmng->NbOfBlock;
                    leave = 1u; 
                }
                
                /* Proceed to go through the block index with each iteration */
                if (0u == leave) 
                {    
                    i = (i + 1) % lochpmng->NbOfBlock;
                    if(i == 0u) 
                    {
                        /* we are back to the beginning of the heap */
                        /* then reset the startblock and the adjfreeblk variable*/
                        /* We do not want to have an allocation divided
                           between the end and the start of the heap */
                        
                        adjfreeblk = 0u;
                        startblk   = 0u;
                    }
                }
            } /* if (0u == leave)*/
            /* check for break conditions */
            if (0 != leave) 
            {
                break; 
            } 
        } /* while (i != ((*nextBlock) - 1))  */
    }
    
#ifdef MEM_DEBUG    
    /*  debug code to make a breakpoint on out of memory possible */
    if(!addr)
    {
        addr = 0u; /* break here */
    }
#endif
#ifdef USE_ROS
    R_OS_ThreadMutexUnlock(&locCdiMemLock);
#endif
    
    return (void *)addr;
}

/*******************************************************************************
 Function: R_CDI_HeapResetNextFreeBlockNb
*/
uint32_t R_CDI_HeapResetNextFreeBlockNb(r_cdi_HpType_t type)
{
    uint32_t err = 0u;
    
    switch (type)
    {
    case CPU:
        locNextCPUFreeBlockNb = 1u;
        break;
    case SPECIAL_MODE:
        locNextSpecialModeFreeBlockNb = 1u;
        break;
    case VIDEO:
        locNextVideoFreeBlockNb = 1u;
        break;
    default:
        err = 1u;
        break;
    }
    return err;
}

/*******************************************************************************
 Function: R_CDI_Free
*/
 uint32_t R_CDI_Free(uint32_t Addr, r_cdi_Heap_t *HeapMng)
{
    
    uint32_t            i;
    uint32_t            err = 0u;
    int32_t             blknb;
    r_cdi_Heap_t        *lochpmng;
    r_cdi_HpBlkIndex_t  *locblkindexlist;
    r_cdi_HpBlkIndex_t  blkindex;
    r_cdi_HpBlkIndex_t  *freeCount;
    r_cdi_HpBlkIndex_t  *nextBlock;

#ifdef USE_ROS
    R_OS_ThreadMutexLock(&locCdiMemLock);
#endif
    if(Addr != 0u)
    {
        lochpmng  = HeapMng;
        freeCount = &(HeapMng->FreeBlocks);
        nextBlock = &(HeapMng->NextFreeBlockNb);
    
        /* Calculate block number from addr */
        blknb =  (Addr - (lochpmng->BaseAddr)) / (lochpmng->BlockSize);
        locblkindexlist = lochpmng->BlockIdxList;
        blkindex = locblkindexlist[blknb];
        
        /* If this memory manager is used in an environment, 
           where the pointer of the allocated memory are aligned upwards outside of the CDI, 
           the resulting pointer might leaved the first block of several that are assigned to this allocation operation. */
        /* In such an environment, the "Free" routine must first seach the lowest index number 
           with the same Block ID before starting the cleanup. */

        if(Addr < lochpmng->BaseAddr)
        {
            err = 2u;   /* address is below managed range of this Heap */
        }
        else if (Addr >= (lochpmng->BaseAddr + (lochpmng->BlockSize*lochpmng->NbOfBlock)))
        {
            err = 2u;   /* address is above managed range of this Heap */
        }
        else if (blkindex == 0u)
        {
            err = 2u;   /* already free */
        }
        else
        {
            (*nextBlock) = blknb;
            i = blknb;
            while (locblkindexlist[i] == blkindex)
            {
                locblkindexlist[i] = 0u;
                i++;
                (*freeCount)++;

                /* Stop deallocation if we leave the range of the block index list */
                if (i>lochpmng->NbOfBlock)
                {
                    break;
                }
            }
            /* Reset block index to zero, if all blocks are deallocated */
            if (lochpmng->NbOfBlock == HeapMng->FreeBlocks)
            {
                HeapMng->NextFreeBlockNb = 0u;
            }
        } 
    }
#ifdef USE_ROS
    R_OS_ThreadMutexUnlock(&locCdiMemLock);
#endif

    return err;
}


/*******************************************************************************
 Function: R_CDI_HeapFree
*/
uint32_t R_CDI_HeapFree(uint32_t addr, r_cdi_HpType_t type)
{
    
    uint32_t i, vidsize;
    uint32_t err = 0u;
    int32_t blknb;
    r_cdi_HeapMng_t      *lochpmng;
    r_cdi_HpBlkIndex_t   *locblkindexlist;
    r_cdi_HpBlkIndex_t   blkindex;
    uint32_t           *freeCount;
    uint32_t           *nextBlock;

#ifdef USE_ROS
    R_OS_ThreadMutexLock(&locCdiMemLock);
#endif
    if(addr != 0u)
    {
        
        /* Check memory type and get the pointer to the corresponding heap manager.*/
        /* if the type is incorrect then 1 is returned */
        /* Have to add check on addr, because if short in CPU memory Video is allocated*/
        /* No other way to know it... */
        vidsize = locVidHeapMng.BlockSize*locVidHeapMng.NbOfBlock;
        if((type == VIDEO) || ((addr >= locVidHeapMng.BaseAddr) && (addr < (locVidHeapMng.BaseAddr + vidsize))))
        {
            lochpmng = &locVidHeapMng;
            freeCount = &locFreeVideoBlocks;
            nextBlock = &locNextVideoFreeBlockNb;
        } 
        else if(type == SPECIAL_MODE)
        {
            lochpmng = &locSpecialModeHeapMng;
            freeCount = &locFreeSpecialModeBlocks;
            nextBlock = &locNextSpecialModeFreeBlockNb;
        } 
        else 
        {
            if(type == CPU)
{
                lochpmng = &locCpuHeapMng;
                freeCount = &locFreeCPUBlocks;
                nextBlock = &locNextCPUFreeBlockNb;
            } 
            else 
            {
                err = 1u;
            }
        }
        
        if (err == 0u)
        {
            /* Calculate block number from addr */
            blknb =  (addr - (lochpmng->BaseAddr)) / (lochpmng->BlockSize);
            locblkindexlist = lochpmng->BlockIndexList;
            blkindex = locblkindexlist[blknb];
            
            if(addr < lochpmng->BaseAddr)
            {
                err = 2u;   /* address is below managed range of this Heap */
            }
            else if (addr >= (lochpmng->BaseAddr + (lochpmng->BlockSize*lochpmng->NbOfBlock)))
            {
                err = 2u;   /* address is above managed range of this Heap */
            }
            else if (blkindex == 0u)
            {
                err = 2u;   /* already free */
            }
            else
            {
                if((*nextBlock)> (uint32_t)blknb)
                {
                    (*nextBlock) = blknb;
                }
                
                i = blknb;
                while (locblkindexlist[i] == blkindex)
                {
                    locblkindexlist[i] = 0u;
                    i++;
                    (*freeCount)++;
                    
                    /* Stop deallocation if we leave the range of the block index list */
                    if (i>lochpmng->NbOfBlock)
                    {
                        break;
                    }
                }
                
            } 
        }
    }
#ifdef USE_ROS
    R_OS_ThreadMutexUnlock(&locCdiMemLock);
#endif

    return err;
}

/*******************************************************************************
 Function: R_CDI_HeapFreeAll
*/
uint32_t R_CDI_HeapFreeAll(r_cdi_HpType_t type)
{
    
    uint32_t err = 0u;
    int32_t tmp;
    r_cdi_HeapMng_t *lochpmng;
    r_cdi_HpBlkIndex_t   *locblkindexlist;
    
#ifdef USE_ROS
    R_OS_ThreadMutexLock(&locCdiMemLock);
#endif
    /* Check memory type and get the pointer to the corresponding heap manager.*/
    /* if the type is incorrect then 1 is returned */
    if(type == CPU)
    {
        lochpmng   = &locCpuHeapMng;
        locNextCpuBlockIndex = 1u;
    } 
    else if(type == SPECIAL_MODE)
    {
        lochpmng = &locSpecialModeHeapMng;
        locNextSpecialModeBlockIndex = 1u;
    
    } 
    else 
    {
        if(type == VIDEO)
        {
            lochpmng = &locVidHeapMng;
            locNextVidBlockIndex = 1u;
        } 
        else 
        {
            err = 1u;
        }
    }
    
    if (err == 0u)
    {
        locblkindexlist = lochpmng->BlockIndexList;
        tmp = lochpmng->NbOfBlock - 1;
        while(tmp >= 0)
        {
            locblkindexlist[tmp] = 0u;
            tmp--;
        }
    }
#ifdef USE_ROS
    R_OS_ThreadMutexUnlock(&locCdiMemLock);
#endif
    return err;
}

/*******************************************************************************
 Function: R_CDI_GetHeapSize
*/
uint32_t R_CDI_GetHeapSize(uint32_t addr, r_cdi_HpType_t type)
{
    
    uint32_t i;
    uint32_t size = 0u;
    uint32_t blknb;
    uint32_t vidsize;
    r_cdi_HeapMng_t *lochpmng;
    r_cdi_HpBlkIndex_t   *locblkindexlist;
    r_cdi_HpBlkIndex_t    blkindex;

#ifdef USE_ROS
    R_OS_ThreadMutexLock(&locCdiMemLock);
#endif
    /* Check memory type and get the pointer to the corresponding heap manager.*/
    /* if the type is incorrect then 1 is returned */
    /* Have to add check on addr, because if short in CPU memory Video is allocated*/
        /* No other way to know it... */
    vidsize = locVidHeapMng.BlockSize*locVidHeapMng.NbOfBlock;
    if((type == VIDEO) || ((addr >= locVidHeapMng.BaseAddr) && (addr < (locVidHeapMng.BaseAddr + vidsize))))
    {
        lochpmng = &locVidHeapMng;
    } 
    else 
    {
        if(type == CPU)
        {
            lochpmng   = &locCpuHeapMng;
        } 
        else
        {
            if(type == SPECIAL_MODE)
            {
                lochpmng   = &locSpecialModeHeapMng;
            }
            else
            {
                lochpmng = 0u;
            }
        }
    }
    
    if (0 != lochpmng)
    {
        /* Calculate block number from addr */
        blknb = (addr - (lochpmng->BaseAddr)) / (lochpmng->BlockSize);
        locblkindexlist = lochpmng->BlockIndexList;
        blkindex = locblkindexlist[blknb];
        i = blknb;
        while (locblkindexlist[i] == blkindex)
        {
            size += lochpmng->BlockSize;
            i++;
        }
        /* if no match found, then return size = 0 */
    }
#ifdef USE_ROS
    R_OS_ThreadMutexUnlock(&locCdiMemLock);
#endif
    return size;
}

/*******************************************************************************
 Function: R_CDI_GetVidHeapBlockSize
*/
uint32_t R_CDI_GetVidHeapBlockSize(void)
{
    return locVidHeapMng.BlockSize;
}    

/*******************************************************************************
 Function: R_CDI_GetVidHeapBlockMax
*/
uint32_t R_CDI_GetVidHeapBlockMax(void)
{
    return locVidHeapMng.NbOfBlock;
}

/*******************************************************************************
 Function: R_CDI_GetVidHeapBlockUsed
*/
uint32_t R_CDI_GetVidHeapBlockUsed(void)
{
    return (locVidHeapMng.NbOfBlock-locFreeVideoBlocks);
}

/*******************************************************************************
 Function: R_CDI_GetCpuHeapBlockSize
*/
uint32_t R_CDI_GetCpuHeapBlockSize(void)
{
    return locCpuHeapMng.BlockSize;
}    

/*******************************************************************************
 Function: R_CDI_GetCpuHeapBlockMax
*/
uint32_t R_CDI_GetCpuHeapBlockMax(void)
{
    return locCpuHeapMng.NbOfBlock;
}

/*******************************************************************************
 Function: R_CDI_GetCpuHeapBlockUsed
*/
uint32_t R_CDI_GetCpuHeapBlockUsed(void)
{
    return (locCpuHeapMng.NbOfBlock-locFreeCPUBlocks);
}

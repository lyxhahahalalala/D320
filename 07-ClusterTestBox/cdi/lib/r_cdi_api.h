/*
****************************************************************************
PROJECT : VLIB
FILE    : $Id: r_cdi_api.h 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
CDI 
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

#ifndef R_CDI_API_H_
#define R_CDI_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#define R_CDI_MEM_DEBUG


/*******************************************************************************
  Title: CDI API Functions

  An application using the buffer and/or the heap management 
  must include this file
*/
/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Type: r_cdi_RBuf_t

  Ring buffer structure

  Members:
  Start   - Pointer to valid data start position
  Stop    - Pointer to the first empty position
  Buffer  - Pointer to the data buffer
  Num     - Number of possible data in the Ring buffer
  Size    - Size of the data of buffer (Byte)
*/

typedef  struct {
    void     *Start;
    void     *Stop;
    void     *Buffer;
    uint32_t Num;
    uint8_t  Size;
}r_cdi_RBuf_t;


/*******************************************************************************
  Type: r_cdi_HpBlkIndex_t

  Used to keep track of the reserved blocks: uint16_t.
*/
typedef uint16_t r_cdi_HpBlkIndex_t;


/*******************************************************************************
  Enum: r_cdi_HpType_t

  Enumeration specifying the memory type of a heap block

  Values:
  CPU               - Block is in CPU memory
  VIDEO             - Block is in Video memory
  SPECIAL_MODE      - This mode can only be used on DK4-H Systems!
                      The memory block is located in the video memory but is
                      accessed via a so called "Special Access Mode", which allows to store
                      data in a graphics oriented way.
                      
                      In order to use SPECIAL_MODE, the Vmem Macro has to be enabled and a valid
                      mode must be set. Please refer to "vmem_api.h" and the Chapter "VMEM" in the
                      DK4-H Memory for further information.
                      
                      Be aware that VIDEO and SPECIAL_MODE refer to the same physical Ram, so it is
                      possible to overwrite data if both modes are used at the same time. 
                            
                      While in this mode data access is always 32 bit, but only 18 / 20 / 24 bits 
                      are valid, depending on the VMEM Settings.
                      
                      Bit assignment:
                      
                      18 bit mode: ------------------------------------------------------------------------------------------------
                        
                            HBUS Write  : 31 - 26 | 25 - 24 | 23 - 18 | 17 - 16 | 15 - 10 |  9 -  0
                            Stored Data : 17 - 12 | ignored | 11 -  6 | ignored |  5 -  0 | ignored
                        
                            HBUS Read   : 31 - 26 | 25 - 24 | 23 - 18 | 17 - 16 | 15 - 10 |  9 -  8 |  7 -  0
                            Stored Data : 17 - 12 | 17 - 16 | 11 -  6 | 11 - 10 |  5 -  0 |  5 -  4 |  Fix 0xFF                      
                      
                      20 bit mode: ------------------------------------------------------------------------------------------------
                        
                            HBUS Write  : 31 - 26 | 25 - 24 | 23 - 18 | 17 - 16 | 15 - 10 |  9 -  8 | 7 - 6 | 5 - 0
                            Stored Data : 19 - 14 | ignored | 13 -  8 | ignored |  7 -  2 | ignored | 1 - 0 | ignored
                        
                            HBUS Read   : 31 - 26 | 25 - 24 | 23 - 18 | 17 - 16 | 15 - 10 |  9 -  8 | 7 - 6 | 5 - 4 | 3 - 2 | 1 - 0
                            Stored Data : 19 - 14 | 19 - 18 | 13 -  8 | 13 - 12 |  7 -  2 |  7 -  6 | 1 - 0 | 1 - 0 | 1 - 0 | 1 - 0 
                                                  
                      24 bit mode (6666): -----------------------------------------------------------------------------------------
                      
                            HBUS Write  : 31 - 26 | 25 - 24 | 23 - 18 | 17 - 16 | 15 - 10 |  9 -  8 | 7 - 2 | 1 - 0
                            Stored Data : 23 - 18 | ignored | 17 - 12 | ignored | 11 -  6 | ignored | 5 - 0 | ignored
                        
                            HBUS Read   : 31 - 26 | 25 - 24 | 23 - 18 | 17 - 16 | 15 - 10 |  9 -  8 | 7 - 2 | 1 - 0
                            Stored Data : 23 - 18 | 23 - 22 | 17 - 12 | 17 - 16 | 11 -  6 | 11 - 10 | 5 - 0 | 5 - 4  
                      
                      24 bit mode (8880): -----------------------------------------------------------------------------------------
                      
                            HBUS Write  : 31 - 24 | 23 - 16 | 15 - 8 |  7 - 0 
                            Stored Data : 23 - 16 | 15 -  8 |  7 - 0 | ignored 
                        
                            HBUS Read   : 31 - 24 | 23 - 16 | 15 - 8 |  7 - 0
                            Stored Data : 23 - 16 | 15 -  8 |  7 - 0 | Fix 0xFF  
                  
                      
                      
                      
*/
typedef enum
{
    CPU,
    VIDEO,
    SPECIAL_MODE
}r_cdi_HpType_t;


/*******************************************************************************
  Type: r_cdi_HeapMng_t

  Structure defining a heap manager

  Members:
  BaseAddr       - 32-bit start address of heap
  NbOfBlock      - Number of blocks the heap is divided in
  BlockSize      - Size of one block
  Type           - Memory type. See <r_cdi_HpType_t>
  BlockIndexList - Pointer to an array of type <r_cdi_HpBlkIndex_t>
                   Each block has one index.
*/
typedef struct {
    uint32_t BaseAddr;
    uint32_t NbOfBlock;
    uint32_t BlockSize;
    r_cdi_HpType_t Type;
    r_cdi_HpBlkIndex_t *BlockIndexList;
}r_cdi_HeapMng_t;

/*******************************************************************************
  Type: r_cdi_Heap_t

  Structure defining a heap manager

  Members:
  BaseAddr        - 32-bit start address of heap
  NbOfBlock       - Number of blocks the heap is divided in
  BlockSize       - Size of one block
  BlockIdxList    - Pointer to an array of type <r_cdi_HpBlkIndex_t>
                    Each block has one index.
  NextBlockIdx    - 
  FreeBlocks      -
  NextFreeBlockNb -
*/
typedef struct {
    uint32_t            BaseAddr;
    uint32_t            NbOfBlock;
    uint32_t            BlockSize;
    r_cdi_HpBlkIndex_t *BlockIdxList;
    r_cdi_HpBlkIndex_t  NextBlockIdx; 
    r_cdi_HpBlkIndex_t  FreeBlocks;  
    r_cdi_HpBlkIndex_t  NextFreeBlockNb;  

}r_cdi_Heap_t;


/*******************************************************************************
  Section: Global API Base Functions
*/
/*******************************************************************************
  Function: R_CDI_Init

  Empty function.
 
  Parameters:
  None
 
  Returns:
  int32_t - always 0

*/
 int32_t R_CDI_Init(void);

/*******************************************************************************
  Function: R_CDI_DeInit

  Empty function.
 
  Parameters:
  None
 
  Returns:
  int32_t - always 0 

*/
 int32_t R_CDI_DeInit(void);


/*******************************************************************************
  Section: Global API Ring Buffer Functions
*/

/*******************************************************************************
  Function: R_CDI_RbSetup

  Setup a ring buffer structure.

  Parameters:
  RBuf       - Ring buffer structure. See: <r_cdi_RBuf_t>
  Buf        - Data buffer
  Num        - Number of elements in buffer
  Size       - Size of the data in buffer

  Returns:
  == 0       - Setup OK
  != 0       - Setup failed
*/
 int32_t R_CDI_RbSetup(r_cdi_RBuf_t *RBuf, void *Buf,
                         uint32_t Num, uint8_t Size);

/*******************************************************************************
  Function: R_CDI_RbWrite

  Copy data to ring buffer.

  Parameters:
  RBuf       - Ring buffer. See: <r_cdi_RBuf_t>
  Buffer     - Data buffer
  Num        - Number of bytes in data buffer

  Returns:
  Number of elements, which have been copied.
*/
 uint32_t R_CDI_RbWrite(r_cdi_RBuf_t *RBuf, void *Buffer, 
                              uint32_t Num);
  
/*******************************************************************************
  Function: R_CDI_RbRead

  Copy data from ring buffer.

  Parameters:
  RBuf       - Ring buffer. See: <r_cdi_RBuf_t>
  Buffer     - Data buffer
  MaxNum     - Number of data to read

  Returns:
  Number of elements, which have been copied.
*/                         
 uint32_t R_CDI_RbRead(r_cdi_RBuf_t *RBuf, void *Buffer, 
                             uint32_t MaxNum);
  
/*******************************************************************************
  Function: R_CDI_RbSize
  
  Calculates how much elements are in the buffer

  Parameters:
  RBuf       - Ring buffer. See: <r_cdi_RBuf_t>

  Returns:
  Number of elements, which are in ring buffer.
*/                          
 uint32_t R_CDI_RbNum(r_cdi_RBuf_t *RBuf);


/*******************************************************************************
  Section: Global API Memory Controller Functions
*/

/*******************************************************************************
  Function: R_CDI_InitCpuHeapManager

  Initialise the CPU memory manager with the given parameters.

  Parameters:
  hpaddr         - 32-bit address pointing to an existing heap buffer of type uint32_t.
  hpblkindexlist - Pointer to an array of <r_cdi_HpBlkIndex_t>. 
                   This array contains the indexes of the hp blocks
  nbblk          - Number of blocks in the heap
  blksize        - Size of one heap block in bytes
  
  Returns:
  == 0       - Initialisation OK
  != 0       - Initialisation failed (1 = 1 of the pointer is not correct)
*/
 uint32_t R_CDI_InitCpuHeapManager(
                uint32_t hpaddr, 
                r_cdi_HpBlkIndex_t * hpblkindexlist, 
                uint32_t nbblk, 
                uint32_t blksize);

/*******************************************************************************
  Function: R_CDI_InitVidHeapManager

  Initialise the video memory manager with the given parameters.

  Parameters:
  hpaddr         - Start address of the VID memory.
  hpblkindexlist - Pointer to an array of <r_cdi_HpBlkIndex_t>
                   This array contains the indexes of the hp blocks
  nbblock        - Number of blocks in the heap
  blksize        - Size of one heap block in bytes
  
  Returns:
  == 0       - Initialisation OK
  != 0       - Initialisation failed
*/
 uint32_t R_CDI_InitVidHeapManager(
            uint32_t hpaddr, 
            r_cdi_HpBlkIndex_t * hpblkindexlist, 
            uint32_t nbblock, 
            uint32_t blksize);

/*******************************************************************************
  Function: R_CDI_InitSmHeapManager

  Initialise the SPECIAL_MODE manager with the given parameters.

  The SPECIAL_MODE allows a special access to video memory. It is currently only used in DK4-H (-> VMEM).
  If you use SPECIAL_MODE, make sure that your device supports this type of memory.  
    
  Parameters:
  hpaddr         - Start address of the VID memory.
  hpblkindexlist - Pointer to an array of <r_cdi_HpBlkIndex_t>
                   This array contains the indexes of the hp blocks
  nbblock        - Number of blocks in the heap
  blksize        - Size of one heap block in bytes
  
  Returns:
  == 0       - Initialisation OK
  != 0       - Initialisation failed
*/
 uint32_t R_CDI_InitSmHeapManager(
            uint32_t hpaddr, 
            r_cdi_HpBlkIndex_t * hpblkindexlist, 
            uint32_t nbblock, 
            uint32_t blksize);




/*******************************************************************************
  Function: R_CDI_InitHeapManager

  Initialise the given memory manager with the given parameters.

  Parameters:
  HeapAddr        - Start address of the heap memory.
                    Note: If you need aligned memory addresses, 
                          please make sure that <HeapAddr> is aligned to <BlkSize>.
                          Also, <R_CDI_AllocAlign> will only work correctly if this requirement is met.
  HeapMng         - Pointer to the heap manager structure, see  <r_cdi_HeapMng_t>
  HeapIdxList     - Pointer to an array of <r_cdi_HpBlkIndex_t>
                    This array contains the indexes of the heap blocks
                    Optional:  If this pointer is NULL, the index list will be placed at
                               the end of the here-initialised memory area. The required 
                               memory will automatically be reserved during initialisation.
  NbBlocks        - Number of blocks in the heap
  BlkSize         - Size of one heap block in bytes
  
  Returns:
  == 0       - Initialisation OK
  != 0       - Initialisation failed
*/
uint32_t R_CDI_InitHeapManager(
        uint32_t             HeapAddr, 
        r_cdi_Heap_t        *HeapMng, 
        r_cdi_HpBlkIndex_t  *HeapIdxList,
        uint32_t             NbBlocks, 
        uint32_t             BlkSize);


/*******************************************************************************
  Function: R_CDI_Alloc

  Allocate one or more blocks of the given heap with the size of memory requested.
 
  Parameters:
  HeapMng - Pointer to the heap manager structure, see  <r_cdi_Heap_t>
  Size    - Memory size requested.
  
  Returns:
  !=0     - void pointer to memory start address
  == 0    - Allocation failed
*/
void * R_CDI_Alloc(r_cdi_Heap_t *HeapMng, uint32_t Size);

/***********************************************************
  Function: R_CDI_AllocAlign

  This is the same function as R_CDI_Alloc but with aligned allocations.
  
  The allocation is implemented such that the unused space that is skipped due to alignment 
  is marked as "free" and could still be used when the heap is otherwise exhausted.
  
  NOTE:
  As the alignment is done via a block alignment, it is required that the base address 
  of this CDI heap is already aligned to the block size set during heap initialization.

  Parameters:
  HeapMng   - Pointer to the heap manager structure, see  <r_cdi_Heap_t>
  Size      - Memory size requested.
  Alignment - Alignment of the requested memory. Must be a multiple of the power of two.
  
  Returns:
  !=0     - void pointer to memory start address
  == 0    - Allocation failed
*/
void * R_CDI_AllocAlign(r_cdi_Heap_t *HeapMng, uint32_t Size, uint32_t Alignment);


/*******************************************************************************
  Function: R_CDI_Free

  Free allocated memory. The start address and a pointer to the heap manger in question 
  must be given as parameters.

  Parameters:
  Addr        - Memory start address
  HeapMng     - Pointer to the heap manager structure, see  <r_cdi_Heap_t>
  
  Returns:
  == 0       - Successful
  == 1       - Wrong type
  == 2       - Address not found
*/
uint32_t R_CDI_Free(uint32_t Addr, r_cdi_Heap_t *HeapMng);

            
/*******************************************************************************
  Function: R_CDI_HeapAlloc

  Allocate one or more blocks of heap in function of the size of memory requested.
  The heap type is passed as parameter with the size.

  Parameters:
  size    - Memory size requested.
  type    - Type of memory to be allocated. See <r_cdi_HpType_t>
  
  Returns:
  !=0     - Memory start address
  == 0    - Allocation failed
*/
 uint32_t R_CDI_HeapAlloc(uint32_t size, r_cdi_HpType_t type);

/*******************************************************************************
  Function: R_CDI_HeapResetNextFreeBlockNb

  Reset the pointer where to start searching for free
  memory (of the specified type) for the next allocation.
  
  Parameters:
  type    - Type of memory
  
  Returns:
  == 0       - Successful
  != 0       - Failed
*/
 uint32_t R_CDI_HeapResetNextFreeBlockNb(r_cdi_HpType_t type);

/*******************************************************************************
  Function: R_CDI_HeapFree

  Free allocated memory. The start address, type of the memory to be free
  must be given as parameters.

  Parameters:
  addr    - Memory start address
  type    - Type of memory
  
  Returns:
  == 0       - Successful
  == 1       - Wrong type
  == 2       - Address not found
*/
 uint32_t R_CDI_HeapFree(uint32_t addr, r_cdi_HpType_t type);

/*******************************************************************************
  Function: R_CDI_HeapFreeAll

  Free all blocks of the memory type specified in parameter.
  
  Parameters:
  type    - Type of memory
  
  Returns:
  == 0       - Successful
  != 0       - Failed
*/
 uint32_t R_CDI_HeapFreeAll(r_cdi_HpType_t type);

/*******************************************************************************
  Function: R_CDI_GetHeapSize

  Return the size in byte of the heap block allocated at the address given
  as input.
  
  Parameters:
  addr    - Start address of the heap memory area
  type    - Type of memory
  
  Returns:
  uint32_t       - Size of the reserved memory area starting at "addr"
*/
 uint32_t R_CDI_GetHeapSize(uint32_t addr, r_cdi_HpType_t type);

/*******************************************************************************
  Function: R_CDI_GetVidHeapBlockSize

  Return the size in byte of video heap blocks.
  
  Parameters:
  None
  
  Returns:
  uint32_t       - Size of one video heap block
*/
 uint32_t R_CDI_GetVidHeapBlockSize(void);

/*******************************************************************************
  Function: R_CDI_GetVidHeapBlockMax

  Return the number of video heap blocks.
  
  Parameters:
  None
  
  Returns:
  uint32_t       - Number of video heap blocks
*/
 uint32_t R_CDI_GetVidHeapBlockMax(void);

/*******************************************************************************
  Function: R_CDI_GetVidHeapBlockUsed

  Return the number of video heap blocks already in use.
  
  Parameters:
  None
  
  Returns:
  uint32_t       - Number of video heap blocks already in use
*/
 uint32_t R_CDI_GetVidHeapBlockUsed(void);


/*******************************************************************************
  Function: R_CDI_GetCpuHeapBlockSize

  Return the size in byte of cpu heap blocks.
  
  Parameters:
  None
  
  Returns:
  uint32_t       - Size of one cpu heap block
*/
 uint32_t R_CDI_GetCpuHeapBlockSize(void);

/*******************************************************************************
  Function: R_CDI_GetCpuHeapBlockMax

  Return the number of cpu heap blocks.
  
  Parameters:
  None
  
  Returns:
  uint32_t       - Number of cpu heap blocks
*/
 uint32_t R_CDI_GetCpuHeapBlockMax(void);

/*******************************************************************************
  Function: R_CDI_GetCpuHeapBlockUsed

  Return the number of cpu heap blocks already in use.
  
  Parameters:
  None
  
  Returns:
  uint32_t       - Number of cpu heap blocks already in use
*/
 uint32_t R_CDI_GetCpuHeapBlockUsed(void);

#ifdef __cplusplus
}
#endif

#endif /* R_CDI_API_H_ */

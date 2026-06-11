/***********************************************************************************************
PROJECT : simple_draw
FILE   : $Id: config.h 1138 2013-07-25 13:21:32Z florian.zimmermann $
============================================================================ 
DESCRIPTION
config of the simple_draw
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
#ifndef CONFIG_H_
#define CONFIG_H_


#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Section: Global Defines
*/

  extern uint32_t loc_VRAM0;
  extern uint32_t loc_VRAM0_SIZE;

  #define LOC_DHD_BASE          loc_VRAM0
  #define LOC_VO0_BASE          (LOC_DHD_BASE + DHD_MEMORY_SIZE)

/*******************************************************************************
  Define: HEAP_SIZE

  Drawing engine driver CPU heap memory.

  Byte size of the drawing engine driver local heap. It
  is used for internal structures and for preparation of
  the drawing engine command lists. Its minimum size depends 
  on  the size of local command lists, number of drawing
  contexts and number of video memory blocks.
*/
#define LOC_HEAP_SIZE           (LOC_CPU_HP_BLOCK_NB * LOC_CPU_HP_BLOCK_SIZE)

/*******************************************************************************
  Define: CPU_HP_BLOCK_NB

  Number of blocks the CPU heap is divided into
  Heap Memory is allocated blockwise.
*/
#define LOC_CPU_HP_BLOCK_NB     (0x100)

/*******************************************************************************
  Define: CPU_HP_BLOCK_SIZE

  Size of one CPU heap block 
*/
#define LOC_CPU_HP_BLOCK_SIZE   (0x40)


/*******************************************************************************
  Define: ERRN_NG
  
  Error Constant set to -1
  Is used as return value when an error occurs.
*/
#define ERR_NG -1

/* VOWE work buffer is allocated before VOWE DL buffer */
#define LOC_VOWE_FRAME_BUF_ADDR  (LOC_VOWE_DL_BUF_ADDR - LOC_VOWE_FRAME_BUF_SIZE) 
#define LOC_VOWE_FRAME_BUF_SIZE  (LOC_DISPLAY_STRIDE * LOC_DISPLAY_HEIGHT * 2 * 4)
#define LOC_VOWE_RING_BUF_ADDR   (LOC_VOWE_DL_BUF_ADDR - LOC_VOWE_RING_BUF_SIZE)
#define LOC_VOWE_RING_BUF_SIZE   (0x00020000)
#define LOC_VOWE_RING_BUFF_DELAY (50)

/* VOWE DL buffer(x2) is allocated in bottom of VRAM */
#define LOC_VOWE_DL_BUF_ADDR     ((loc_VRAM0 + loc_VRAM0_SIZE) - (LOC_VOWE_DL_BUF_SIZE * 2))
#define LOC_VOWE_DL_BUF_SIZE     (0x00010000)

#define LOC_WM_UNIT     0
#define LOC_DHD_UNIT    0
#define LOC_DRW2D_UNIT  0
#define LOC_VOWE_UNIT   0

#define LOC_VOWE_ENABLE 0

  /***********************************************************
  test port & pins for measurement
*/

/* if you change this port then update the pin reference locations in the 
   loc_TestPinSetup()
*/
#define TEST_PORT (44)
#define TEST_PIN0 (0)
#define TEST_PIN1 (1)
#define TEST_PIN2 (2)
#define TEST_PIN3 (3)
#define TEST_PIN4 (4)
#define TEST_PIN5 (5)
#define TEST_PIN6 (6)
#define TEST_PIN7 (7)


/* registers for the ISMnEMU (SVSTOP) */
#define ISM_EMU_REG                 (0xFFF00000u + 0x0260u)         
#define ISM_SVDIS_NOT_ALLOW_STOP    (0x00000080u)
#define ISM_SVDIS_ALLOW_STOP        (0x00000000u)

#ifdef __cplusplus
}
#endif

#endif /* CONFIG_H_ */

                



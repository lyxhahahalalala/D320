/*
****************************************************************************
PROJECT : OSTM driver
FILE    : $Id: r_ostm_regs.h 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Driver for OSTM macro
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

#ifndef OSTM_REGS_H_
#define OSTM_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: OSTM Register Offsets

  Only the driver modules shall include this header.
*/

/*******************************************************************************
  Section: Global Defines
*/

/*******************************************************************************
  Constants: OSTM Register Offsets
  
    R_OSTM_CMP     - Compare register
    R_OSTM_CNT     - counter register
    R_OSTM_TO      - output register
    R_OSTM_TOE     - output enable register
    R_OSTM_TE      - count enable status register
    R_OSTM_TS      - count start trigger register
    R_OSTM_TT      - count stop trigger register
    R_OSTM_CTL     - control register
    R_OSTM_EMU     - emulation register
  
*/
  
#ifndef R_OSTM_USER_OFFSET
  #define R_OSTM_USER_OFFSET(x)  (0x00000000)
#endif
#ifndef R_OSTM_OS_OFFSET
  #define R_OSTM_OS_OFFSET(x) (0x00000000)
#endif
  
#define R_OSTM_CMP    (R_OSTM_USER_OFFSET(Unit) + 0x00)
#define R_OSTM_CNT    (R_OSTM_USER_OFFSET(Unit) + 0x04)
#define R_OSTM_TO     (R_OSTM_USER_OFFSET(Unit) + 0x08)
#define R_OSTM_TOE    (R_OSTM_USER_OFFSET(Unit) + 0x0c)
#define R_OSTM_TE     (R_OSTM_USER_OFFSET(Unit) + 0x10)
#define R_OSTM_TS     (R_OSTM_USER_OFFSET(Unit) + 0x14)
#define R_OSTM_TT     (R_OSTM_USER_OFFSET(Unit) + 0x18)
#define R_OSTM_CTL    (R_OSTM_OS_OFFSET(Unit)   + 0x20)
#define R_OSTM_EMU    (R_OSTM_OS_OFFSET(Unit)   + 0x24)


/*******************************************************************************
  Type: R_OSTM_Reg_CtlBit_t
  
  Content of control register CTL2
  
  Members:
    OstmMd0     - Controls Interrupt request at counter start
    OstmMd1     - Specified the counter operation
    Reserved    - Reserved bits 

*/

typedef struct {
    uint8_t OstmMd0    : 1;  
    uint8_t OstmMd1    : 1;  
    uint8_t Reserved   : 6;                            
} R_OSTM_RegCtlBit_t;


/*******************************************************************************
  Union: R_OSTM_RegCtl_t
  
  Simplifies Bit and Byte access to register CTL2    
  
  Members:
  Bit      - Allow bit access - see: <r_uarte_Reg_Ctl2_t>
  Byte     - Allow access ofthe whole byte
*/

typedef union {
    R_OSTM_RegCtlBit_t  Bit;
    uint16_t            Byte;
} R_OSTM_RegCtl_t;



#ifdef __cplusplus
}
#endif

#endif /* OSTM_REGS_H_  */

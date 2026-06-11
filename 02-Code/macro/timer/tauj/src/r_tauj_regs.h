/*
****************************************************************************
PROJECT : TAUJ driver
FILE    : $Id: r_tauj_regs.h 7640 2016-02-12 13:14:23Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Driver for TAUJ macro
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


#ifndef R_TAUJ_REGS_H_
#define R_TAUJ_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
  Title: TAUJ Register Offsets

  Only the driver modules shall include this header.
*/

/***********************************************************
  Section: Global Defines
*/

#ifndef R_TAUJ_USER_OFFSET
#define R_TAUJ_USER_OFFSET( x )  ( 0x00000000 )
#endif
#ifndef R_TAUJ_OS_OFFSET
#define R_TAUJ_OS_OFFSET( x )    ( 0x00000000 )
#endif

/***********************************************************
  Constants: Register Offsets
*/


/* Channel Data Register 0 R/W (16 and 32 bit) */
#define R_TAUJ0_CDR0  ( R_TAUJ_USER_OFFSET( Unit ) + 0x0000 )

/* Channel Data Register 1 R/W (16 and 32 bit) */
#define R_TAUJ0_CDR1  ( R_TAUJ_USER_OFFSET( Unit ) + 0x0004 )

/* Channel Data Register 2 R/W (16 and 32 bit) */
#define R_TAUJ0_CDR2  ( R_TAUJ_USER_OFFSET( Unit ) + 0x0008 )

/* Channel Data Register 3 R/W (16 and 32 bit) */
#define R_TAUJ0_CDR3  ( R_TAUJ_USER_OFFSET( Unit ) + 0x000C )


/* Channel Counter Register 0 R (16 and 32 bit) */
#define R_TAUJ0_CNT0  ( R_TAUJ_USER_OFFSET( Unit ) + 0x0010 )

/* Channel Counter Register 1 R (16 and 32 bit) */
#define R_TAUJ0_CNT1  ( R_TAUJ_USER_OFFSET( Unit ) + 0x0014 )

/* Channel Counter Register 2 R (16 and 32 bit) */
#define R_TAUJ0_CNT2  ( R_TAUJ_USER_OFFSET( Unit ) + 0x0018 )

/* Channel Counter Register 3 R (16 and 32 bit) */
#define R_TAUJ0_CNT3  ( R_TAUJ_USER_OFFSET( Unit ) + 0x001C )


/* Channel Mode OS Register 0 R/W */
#define R_TAUJ0_CMOR0 ( R_TAUJ_OS_OFFSET( Unit ) + 0x0080 )

/* Channel Mode OS Register 1 R/W */
#define R_TAUJ0_CMOR1 ( R_TAUJ_OS_OFFSET( Unit ) + 0x0084 )

/* Channel Mode OS Register 2 R/W */
#define R_TAUJ0_CMOR2 ( R_TAUJ_OS_OFFSET( Unit ) + 0x0088 )

/* Channel Mode OS Register 3 R/W */
#define R_TAUJ0_CMOR3 ( R_TAUJ_OS_OFFSET( Unit ) + 0x008C )


/* Channel Mode User Register 0 R/W */
#define R_TAUJ0_CMUR0 ( R_TAUJ_USER_OFFSET( Unit ) + 0x0020 )

/* Channel Mode User Register 1 R/W */
#define R_TAUJ0_CMUR1 ( R_TAUJ_USER_OFFSET( Unit ) + 0x0024 )

/* Channel Mode User Register 2 R/W */
#define R_TAUJ0_CMUR2 ( R_TAUJ_USER_OFFSET( Unit ) + 0x0028 )

/* Channel Mode User Register 3 R/W */
#define R_TAUJ0_CMUR3 ( R_TAUJ_USER_OFFSET( Unit ) + 0x002C )


/* Channel Status Register 0 R */
#define R_TAUJ0_CSR0 ( R_TAUJ_USER_OFFSET( Unit ) + 0x0030 )

/* Channel Status Register 1 R */
#define R_TAUJ0_CSR1 ( R_TAUJ_USER_OFFSET( Unit ) + 0x0034 )

/* Channel Status Register 2 R */
#define R_TAUJ0_CSR2 ( R_TAUJ_USER_OFFSET( Unit ) + 0x0038 )

/* Channel Status Register 3 R */
#define R_TAUJ0_CSR3 ( R_TAUJ_USER_OFFSET( Unit ) + 0x003C )


/* Channel Status Clear Trigger Register 0 W */
#define R_TAUJ0_CSC0 ( R_TAUJ_USER_OFFSET( Unit ) + 0x0040 )

/* Channel Status Clear Trigger Register 1 W */
#define R_TAUJ0_CSC1 ( R_TAUJ_USER_OFFSET( Unit ) + 0x0044 )

/* Channel Status Clear Trigger Register 2 W */
#define R_TAUJ0_CSC2 ( R_TAUJ_USER_OFFSET( Unit ) + 0x0048 )

/* Channel Status Clear Trigger Register 3 W */
#define R_TAUJ0_CSC3 ( R_TAUJ_USER_OFFSET( Unit ) + 0x004C )


/* Channel Enable Status Register R */
#define R_TAUJ0_TE ( R_TAUJ_USER_OFFSET( Unit ) + 0x0050 )


/* Channel Start Trigger Register W */
#define R_TAUJ0_TS ( R_TAUJ_USER_OFFSET( Unit ) + 0x0054 )


/* Channel Stop Trigger Register W */
#define R_TAUJ0_TT ( R_TAUJ_USER_OFFSET( Unit ) + 0x0058 )


/* Presaler Clock Select Register R/W */
#define R_TAUJ0_TPS ( R_TAUJ_OS_OFFSET( Unit ) + 0x0090 )


/* Presaler Baud Rate Setting Register R/W */
#define R_TAUJ0_BRS ( R_TAUJ_OS_OFFSET( Unit ) + 0x0094 )


/* Channel Output Register R/W */
#define R_TAUJ0_TO ( R_TAUJ_USER_OFFSET( Unit ) + 0x005C )


/* Channel Output Enable Register R/W */
#define R_TAUJ0_TOE ( R_TAUJ_USER_OFFSET( Unit ) + 0x0060 )


/* Channel Output Mode Register R/W */
#define R_TAUJ0_TOM ( R_TAUJ_OS_OFFSET( Unit ) + 0x0098 )


/* Channel Output Configuration Register R/W */
#define R_TAUJ0_TOC ( R_TAUJ_OS_OFFSET( Unit ) + 0x009C )


/* Channel Output Active Level Register R/W */
#define R_TAUJ0_TOL ( R_TAUJ_USER_OFFSET( Unit ) + 0x0064 )


/* Channel Reload Data Enable Register R/W */
#define R_TAUJ0_RDE ( R_TAUJ_OS_OFFSET( Unit ) + 0x00A0 )


/* Channel Reload Data Mode Register R/W */
#define R_TAUJ0_RDM ( R_TAUJ_OS_OFFSET( Unit ) + 0x00A4 )


/* Channel Reload Data Trigger Register W */
#define R_TAUJ0_RDT ( R_TAUJ_USER_OFFSET( Unit ) + 0x0068 )


/* Channel Reload Status Register R */
#define R_TAUJ0_RSF ( R_TAUJ_USER_OFFSET( Unit ) + 0x006C )


/* TAU Emulation Register R/W */
#define R_TAUJ0_EMU ( R_TAUJ_OS_OFFSET( Unit ) + 0x00A8 )


/***********************************************************
  Type: r_tauj_RegCmorBits_t

  Content of the TAUJ0CMORn: TAU Channel Mode OS Register

  Members:
    Cks  - CKS1 and CKS0: Clock Select Bits
    Ccs  - CCS1 and CCS0: Count Clock Select Bits
    Mas  - MAS: Master Mode Bit
    Sts  - STS2 to STS0: Start Trigger Select Bits
    Cos  - COS1 and COS0: Capture Overflow Select Bits
    Res  - Reserved
    Md   - MD4 to MD0: Mode Control Bits
*/

typedef struct {
    uint16_t Md0  : 1;      
    uint16_t Md   : 4;  
    uint16_t Res  : 1;  
    uint16_t Cos  : 2;    
    uint16_t Sts  : 3;    
    uint16_t Mas  : 1;    
    uint16_t Ccs  : 2;    
    uint16_t Cks  : 2;
} r_tauj_RegCmorBits_t;

typedef union {
    r_tauj_RegCmorBits_t Bits;
    uint16_t Word;
} r_tauj_RegCmor_t;


/***********************************************************
  Type: r_tauj_RegCmurBits_t

  Content of the TAUJ0CMURn: TAU Channel Mode User Register

  Members:
    Res  - Reserved
    Tis  - TIS1 and TIS0: TINn Edge Select Bits
*/

typedef struct {
    uint16_t Tis  : 2;    
    uint16_t Res : 14;
} r_tauj_RegCmurBits_t;

typedef union {
    r_tauj_RegCmurBits_t Bits;
    uint16_t Word;
} r_tauj_RegCmur_t;


/***********************************************************
  Type: r_tauj_RegCsrBits_t

  Content of the TAUJ0CSRn: TAU Channel Status Register

  Members:
    Res  - Reserved
    Rfu_Csf  - Count Status Flag Bit (reserved)
    Ovf  - Overflow Flag Bit
*/

typedef struct {
    uint16_t Ovf  : 1;    
    uint16_t Rfu_Csf  : 1;    
    uint16_t Res : 15;
} r_tauj_RegCsrBits_t;

typedef union {
    r_tauj_RegCsrBits_t Bits;
    uint16_t Word;
} r_tauj_RegCsr_t;


/***********************************************************
  Type: r_tauj_RegCscBits_t

  Content of the TAUJ0CSCn: TAU Channel Status Clear Register

  Members:
    Res  - Reserved
    Clov - Clear Overflow Flag Bit
*/

typedef struct {
    uint16_t Clov : 1;    
    uint16_t Res : 15;
} r_tauj_RegCscBits_t;

typedef union {
    r_tauj_RegCscBits_t Bits;
    uint16_t Word;
} r_tauj_RegCsc_t;


/***********************************************************
  Type: r_tauj_RegTpsBits_t

  Content of the TAUJ0TPS: TAU Prescaler Clock Select Register

  Members:
    Pcs_Ck0 - Prescaler Clock Select Bits for CK0
    Pcs_Ck1 - Prescaler Clock Select Bits for CK1
    Pcs_Ck2 - Prescaler Clock Select Bits for CK2
    Pcs_Ck3 - Prescaler Clock Select Bits for CK3
*/

typedef struct {
    uint16_t Pcs_Ck0 : 4;
    uint16_t Pcs_Ck1 : 4;
    uint16_t Pcs_Ck2 : 4;
    uint16_t Pcs_Ck3 : 4;
} r_tauj_RegTpsBits_t;

typedef union {
    r_tauj_RegTpsBits_t Bits;
    uint16_t Word;
} r_tauj_RegTps_t;


/***********************************************************
  Type: r_tauj_RegBrsBits_t

  Content of the TAUJ0BRS: TAU Prescaler Baud Rate Setting Register

  Members:
    Res - Reserved for future use
    Brs - Baud rate clock setting for CK3 (8bit)
*/

typedef struct {
    uint8_t Res : 8;    
    uint16_t Rfu : 8;
} r_tauj_RegBrsBits_t;

typedef union {
    r_tauj_RegBrsBits_t Bits;
    uint16_t Word;
} r_tauj_RegBrs_t;


/***********************************************************
  Type: r_tauj_RegEmuBits_t

  Content of the TAUJ0EMU: TAU Emulation Register

  Members:
    Rfu     - Reserved for future use
    Svs_Dis - Emulation control bit
*/

typedef struct {  
    uint8_t Res : 7;    
    uint8_t Svs_Dis : 1;    
} r_tauj_RegEmuBits_t;

typedef union {
    r_tauj_RegEmuBits_t Bits;
    uint8_t             Byte;
} r_tauj_RegEmu_t;


#endif /* R_TAUJ_REGS_H_  */

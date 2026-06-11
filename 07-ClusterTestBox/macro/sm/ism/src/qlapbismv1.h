//============================================================================
// PROJECT = ISM Type QLAPBISMV1
// MODULE  = $Source$
// VERSION = $Revision: 4975 $
// DATE    = $Date: 2015-03-27 17:22:34 +0100 (Fr, 27 Mrz 2015) $
//============================================================================
//                                 C O P Y R I G H T                                    
//============================================================================
// Copyright (c) 2002 by NEC Electronics (Europe) GmbH. All rights reserved.
// Kanzlerstrasse 2
// D-40472 Duesseldorf
// Germany
//============================================================================
//Purpose: ISM Driver Hardware Register Include
//
//Warranty Disclaimer
//
//Because the Product(s) is licensed free of charge, there is no warranty 
//of any kind whatsoever and expressly disclaimed and excluded by NEC, 
//either expressed or implied, including but not limited to those for 
//non-infringement of intellectual property, merchantability and/or 
//fitness for the particular purpose. 
//NEC shall not have any obligation to maintain, service or provide bug 
//fixes for the supplied Product(s) and/or the Application.
//
//Each User is solely responsible for determining the appropriateness of 
//using the Product(s) and assumes all risks associated with its exercise 
//of rights under this Agreement, including, but not limited to the risks 
//and costs of program errors, compliance with applicable laws, damage to 
//or loss of data, programs or equipment, and unavailability or 
//interruption of operations.
//
//Limitation of Liability
//
//In no event shall NEC be liable to the User for any incidental, 
//consequential, indirect, or punitive damage (including but not limited 
//to lost profits) regardless of whether such liability is based on breach 
//of contract, tort, strict liability, breach of warranties, failure of 
//essential purpose or otherwise and even if advised of the possibility of 
//such damages. NEC shall not be liable for any services or products 
//provided by third party vendors, developers or consultants identified or
//referred to the User by NEC in connection with the Product(s) and/or the 
//Application.
//
//
//
//============================================================================
// Environment: Devices:          All featuring QLAPBISMV1
//              Assembler:        GHS MULTI 2000
//              C-Compiler:       GHS MULTI 2000
//              Linker:           GHS MULTI 2000
//              Debugger:         GHS MULTI 2000
//============================================================================


#ifndef _QLAPBISMV1_H
#define _QLAPBISMV1_H

#ifdef __cplusplus
extern "C" {
#endif


/* ISM Globals (for ISM driver only) */

#define R_ISM_NOIRQ   R_FALSE       /* status report defines */
#define R_ISM_IRQ     R_TRUE

#define R_ISM_INT_DONE      0    /* Interrupt vector definition */
#define R_ISM_INT_REACHED   1
#define R_ISM_INT_ZPDAD     2
#define R_ISM_INT_ZPD       3

#define R_ISM_INT_PERUNIT   9

/* ISM Channel control values */
#define R_ISM_CHANTYPES     ( 6 )


/* ZIS input assignments (correct) as used in D1x */
#define R_ISM_ZIS_SIN_P     ( 0 )
#define R_ISM_ZIS_COS_P     ( 1 )
#define R_ISM_ZIS_SIN_M     ( 2 )
#define R_ISM_ZIS_COS_M     ( 3 )

/* ZIS input assignments are swapped in Dx4-H */
/*
#define R_ISM_ZIS_SIN_P     ( 0 )
#define R_ISM_ZIS_COS_P     ( 2 )
#define R_ISM_ZIS_SIN_M     ( 1 )
#define R_ISM_ZIS_COS_M     ( 3 )
*/

/* ZPD Table entries definitions */

#define R_ISM_DIRECT_H_P    ( 10 )
#define R_ISM_DIRECT_V_P    ( 0 )
#define R_ISM_DIRECT_MASK   ( 0x0F )
#define R_ISM_DIRECT_EN     ( 0x03000000L )
#define R_ISM_DIRECT_H_EN   ( 0x02000000L )
#define R_ISM_DIRECT_V_EN   ( 0x01000000L )
#define R_ISM_DIRECT_H_EN_P ( 25 )
#define R_ISM_DIRECT_V_EN_P ( 24 )
#define R_ISM_ZMT_EN        ( 0x80000000L )
#define R_ISM_ZIS_P         ( 20 )
#define R_ISM_CQI_P         ( 20 )
#define R_ISM_PWM_H_P       ( 10 )
#define R_ISM_PWM_V_P       ( 0 )
#define R_ISM_CZD_P         ( 26 )

#define R_ISM_RECIRC_H      ( 0x00800000L )
#define R_ISM_RECIRC_V      ( 0x00400000L )
#define R_ISM_RECIRC_V_P    ( 22 )
#define R_ISM_RECIRC_H_P    ( 23 )



#define R_ISM_RESETALL      R_TRUE
#define R_ISM_CONTINUE      R_FALSE

/* ISM Constant Values (Characteristics) */ 

#define R_ISM_TB_COUNTS     ( 1023 )
#define R_ISM_PWMDIV_MAX    ( 16 )
#define R_ISM_CMDIV_MAX     ( 256 )

/* Table size definitons */

#define R_ISM_TABLESIZE_STD ( 128 )
#define R_ISM_TABLESIZE_HI  ( 512 )
#define R_ISM_TABLESIZE_ZPD ( 128 )



/* Settings definitions */

#define R_ISM_QUADRANT_1    ( 0 )
#define R_ISM_QUADRANT_2    ( 1 )
#define R_ISM_QUADRANT_3    ( 2 )
#define R_ISM_QUADRANT_4    ( 3 )
#define R_ISM_QUADRANTS     ( 4 )

#define R_ISM_SPEEDS        ( 2 )
#define R_ISM_SPEED_LO      ( 0 )
#define R_ISM_SPEED_HI      ( 1 )
#define R_ISM_DIRECTIONS    ( 2 )
#define R_ISM_DIRECTION_UP  ( 0 )
#define R_ISM_DIRECTION_DN  ( 1 )

#define R_ISM_RECIRC_SIDE_L ( 0 )
#define R_ISM_RECIRC_SIDE_H ( 1 )
#define R_ISM_ANALOGLEVEL0  ( 0 )
#define R_ISM_ANALOGLEVEL1  ( 1 )

#define R_ISM_PWMDUTY_0     ( 0x000 )
#define R_ISM_PWMDUTY_100   ( 0x3FF )

/* Direct I/O control settings */

#define R_ISM_DIRECT_OPEN         ( 0x00 )
#define R_ISM_DIRECT_PTOVDD_ZIS23 ( 0x08 )
#define R_ISM_DIRECT_PTOVSS_ZIS23 ( 0x04 )
#define R_ISM_DIRECT_MTOVDD_ZIS01 ( 0x02 )
#define R_ISM_DIRECT_RECIRC_VDD   ( 0x0A )
#define R_ISM_DIRECT_CURRENT_M    ( 0x06 )
#define R_ISM_DIRECT_MTOVSS_ZIS01 ( 0x01 )
#define R_ISM_DIRECT_CURRENT_P    ( 0x09 )
#define R_ISM_DIRECT_RECIRC_VSS   ( 0x05 )


/*............... The ISM access structures ............................*/

  typedef struct rism_gctl_t {

    volatile uint32_t   gzl       : 1;
    volatile uint32_t   RESERVED0 : 7;
    volatile uint32_t   gce       : 1;
    volatile uint32_t   RESERVED1 : 7;
    volatile uint32_t   gen       : 1;
    volatile uint32_t   RESERVED2 : 7;
    volatile uint32_t   iere      : R_ISM_CHANNELS;
    volatile uint32_t   iedo      : 1;
    volatile uint32_t   iead      : 1;

  } rism_gctl_t;

  typedef struct rism_gstr_t {

    volatile uint32_t   cen       : R_ISM_CHANNELS;
    volatile uint32_t   RESERVED0 : 1;
    volatile uint32_t   gcb       : 1;
    volatile uint32_t   RESERVED1 : 24;

  } rism_gstr_t;

  typedef struct rism_gcfg_t {

    volatile uint32_t   gud       : 8;
    volatile uint32_t   RESERVED0 : 8;
    volatile uint32_t   gtb       : 4;
    volatile uint32_t   RESERVED1 : 12;

  } rism_gcfg_t;

  typedef struct rism_gzpdctl_t {

    volatile uint32_t   gcs       : 3;
    volatile uint32_t   gzf       : 4;
    volatile uint32_t   RESERVED0 : 1;
    volatile uint32_t   grv1      : 4;
    volatile uint32_t   grv2      : 4;
    volatile uint32_t   gfd       : 4;
    volatile uint32_t   gfl       : 4;
    volatile uint32_t   gzo       : 1;
    volatile uint32_t   RESERVED1 : 5;
    volatile uint32_t   gzp       : 1;
    volatile uint32_t   gze       : 1;

  } rism_gzpdctl_t;

  typedef struct rism_gip_t {

    volatile uint32_t   ssre      : R_ISM_CHANNELS;
    volatile uint32_t   ssdo      : 1;
    volatile uint32_t   ssad      : 1;
    volatile uint32_t   RESERVED0 : 24;

  } rism_gip_t;

  typedef struct rism_gstc_t {

    volatile uint32_t   clre      : R_ISM_CHANNELS;
    volatile uint32_t   cldo      : 1;
    volatile uint32_t   clad      : 1;
    volatile uint32_t   RESERVED0 : 24;

  } rism_gstc_t;

  typedef struct rism_ccmr_t {

    volatile uint32_t   cct       : 3;
    volatile uint32_t   RESERVED0 : 2;
    volatile uint32_t   ccp       : 1;
    volatile uint32_t   czp       : 1;
    volatile uint32_t   RESERVED1 : 1;
    volatile uint32_t   cdh       : 4;
    volatile uint32_t   cdv       : 4;
    volatile uint32_t   ctl       : 7;
    volatile uint32_t   RESERVED2 : 9;

  } rism_ccmr_t;

  typedef struct rism_ccmp_t {

    volatile uint32_t   cvp       : 10;
    volatile uint32_t   chp       : 10;
    volatile uint32_t   cqi       : 2;
    volatile uint32_t   RESERVED0 : 10;

  } rism_ccmp_t;

  typedef struct rism_zpdctl_t {

    volatile uint32_t   zmt       : R_ISM_CHANNELS;
    volatile uint32_t   RESERVED0 : 2;
    volatile uint32_t   zis0      : 2;
    volatile uint32_t   RESERVED1 : 2;
    volatile uint32_t   zis1      : 2;
    volatile uint32_t   RESERVED2 : 2;
    volatile uint32_t   zis2      : 2;
    volatile uint32_t   RESERVED3 : 2;
    volatile uint32_t   zis3      : 2;
    volatile uint32_t   RESERVED4 : 2;
    volatile uint32_t   zis4      : 2;
    volatile uint32_t   RESERVED5 : 2;
    volatile uint32_t   zis5      : 2;
    volatile uint32_t   RESERVED6 : 2;

  } rism_zpdctl_t;

  typedef struct rism_zpdcmp_t {

    volatile uint32_t   zbt       : 16;
    volatile uint32_t   RESERVED0 : 15;
    volatile uint32_t   zss       : 1;

  } rism_zpdcmp_t;

  typedef struct rism_chan_t {

    volatile uint32_t   target;
    volatile uint32_t   pdf;
    volatile uint32_t   pal;
    volatile uint32_t   pdl;
    volatile uint32_t   pms;
    volatile uint32_t   phc;
    volatile uint32_t   ps1;
    volatile uint32_t   ps2;
    volatile uint32_t   ps3;
    volatile uint32_t   ps4;

  } rism_chan_t;

  typedef struct rism_cfg_t {
   
    struct rism_chan_t    ch[ R_ISM_CHANNELS ];

  } rism_cfg_t;

  typedef struct rism_var_t {

    volatile uint32_t   acceleration;
    volatile uint32_t   newspeed;
    volatile uint32_t   oldspeed;
    volatile uint32_t   temporary;
    volatile uint32_t   position;
    volatile uint32_t   virtualpos;
    volatile uint32_t   dirspeedflags;

  } rism_var_t;

  typedef struct r_ism_t {

    struct rism_gctl_t    gctl;
    struct rism_gstr_t    gstr;
    struct rism_gcfg_t    gcfg;
    volatile uint32_t         gcnt;
    struct rism_gzpdctl_t gzpdctl;
    struct rism_gip_t     gip;
    struct rism_gstc_t    gstc;
    struct rism_ccmr_t    ccmr[ R_ISM_CHANNELS ];
    volatile uint32_t         czc[ R_ISM_CHANNELS ];
    struct rism_ccmp_t    ccmp[ R_ISM_CHANNELS ];
    volatile uint32_t         czd[ R_ISM_CHANNELS ];
    volatile uint32_t         cioc[ R_ISM_CHANNELS ];
    volatile uint32_t         irs;
    struct rism_zpdctl_t  zpdctl;
    volatile uint32_t         zrs;
    volatile uint32_t         zdr;
    volatile uint32_t         zip;
    volatile uint32_t         clzp;
    struct rism_zpdcmp_t  zpdcmp[ R_ISM_CHANNELS ];
    volatile uint32_t         zaf;
    struct rism_cfg_t     set;
    struct rism_var_t     var[ R_ISM_CHANNELS ];
    volatile uint32_t         emu;

#ifdef R_ISM_PADDING_REGS
    volatile uint8_t   padding[ R_ISM_PADDING_REGS ];
#endif

  } r_ism_t;

  typedef struct rism_pwmcell_t {

    volatile uint32_t         cvp    : 10;
    volatile uint32_t         chp    : 10;
    volatile uint32_t         cqi    : 2;
    volatile uint32_t         ivr    : 1;
    volatile uint32_t         ihr    : 1;
    volatile uint32_t         UNUSED : 8;

  } rism_pwmcell_t;

  typedef struct rism_pwmstd_t {

    struct rism_pwmcell_t std[ R_ISM_SPEEDS ]
                              [ R_ISM_DIRECTIONS ]
                              [ R_ISM_TABLESIZE_STD ];
  } rism_pwmstd_t;

  typedef struct rism_pwmhi_t {

    struct rism_pwmcell_t hi[ R_ISM_TABLESIZE_HI ];

  } rism_pwmhi_t;

  typedef struct rism_zpdtcfg_t {

    uint8_t TableLength;
    uint32_t Table[ R_ISM_TABLESIZE_ZPD ];

  }rism_zpdtcfg_t;

  typedef union rism_pwm_t {

    struct rism_pwmcell_t  hi[ R_ISM_CHANTYPES ]
                              [ R_ISM_TABLESIZE_HI ];
    struct rism_pwmcell_t std[ R_ISM_CHANTYPES ]
                              [ R_ISM_SPEEDS ]
                              [ R_ISM_DIRECTIONS ]
                              [ R_ISM_TABLESIZE_STD ];

#ifdef R_ISM_PADDING_RAM
    volatile uint8_t   padding[ R_ISM_PADDING_RAM ];
#endif

  } rism_pwm_t;

  typedef struct r_ism_zpd_t {

    volatile uint32_t         cmd[ R_ISM_CHANTYPES ]
                              [ R_ISM_TABLESIZE_ZPD ];

#ifdef R_ISM_PADDING_RAM
    volatile uint8_t   padding[ R_ISM_PADDING_RAM ];
#endif

  } r_ism_zpd_t;




#ifdef __cplusplus
}
#endif

#endif


/*
****************************************************************************
PROJECT : Macro driver
FILE    : $Id: int_enabled.h 3577 2014-08-14 12:13:44Z matthias.nippert $
============================================================================
DESCRIPTION
Device dependant configuration file for r7f701412
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

#ifndef INT_ENABLE_H_
#define INT_ENABLE_H_

/* make sure all "needed"   interrupt vectors are available */
#define PRIO_0_ENABLE (1u) 
#define USE_ISM

#ifndef RESET_ENABLE
    #define RESET_ENABLE (1u)
#endif

/* Trap1 Interrupt */
/* This will be used from R_DEV_SysCall */
#ifndef  TRAP1_ENABLE
    #define TRAP1_ENABLE (1u)
#endif

#ifdef USE_SPF
    #ifndef FEMULTI_ENABLE
        #define FEMULTI_ENABLE (1u)
     #endif

     #ifndef  TRAP0_ENABLE
         #define TRAP0_ENABLE (1u)
     #endif
     
     #ifndef  TRAP1_ENABLE
         #define TRAP1_ENABLE (1u)
     #endif
     
#endif

#ifdef USE_PIE
/* PIE Interrupt */
/* The following interrupt will not be activated by a driver, but has 
   to be activated by definining <USE_PIE> in the Makefile. 
   The function <R_PIE_Isr> has to be provided in the application code. */
    #ifndef PIE_ENABLE
        #define PIE_ENABLE (1u)
    #endif
#endif

/*ECM Interrupts*/
#ifdef USE_ECM
    #ifndef FENMI_ENABLE
        #define FENMI_ENABLE (1u)
    #endif
    #ifndef FEINT_ENABLE
        #define FEINT_ENABLE (1u)
    #endif
#endif

/*WDTA Interrupts*/
#ifdef USE_WDTA
    #ifndef INTWDTA0_ENABLE
        #define INTWDTA0_ENABLE (1u)
    #endif
    #ifndef INTWDTA1_ENABLE
        #define INTWDTA1_ENABLE (1u)
    #endif
#endif /*WDTA Interrupts*/

/* External interrupts for intc driver */
#ifdef USE_GPIO
    #ifndef INTP0_ENABLE
        #define INTP0_ENABLE (1u)
    #endif
    
    #ifndef INTP1_ENABLE
        #define INTP1_ENABLE (1u)
    #endif
    
    #ifndef INTP2_ENABLE
        #define INTP2_ENABLE (1u)
    #endif
    
    #ifndef INTP3_ENABLE
        #define INTP3_ENABLE (1u)
    #endif
    
    #ifndef INTP4_ENABLE
        #define INTP4_ENABLE (1u)
    #endif
    
    #ifndef INTP5_ENABLE
        #define INTP5_ENABLE (1u)
    #endif
    
    #ifndef INTP6_ENABLE
        #define INTP6_ENABLE (1u)
    #endif
    
    #ifndef INTP7_ENABLE
        #define INTP7_ENABLE (1u)
    #endif
    
    #ifndef INTP8_ENABLE
        #define INTP8_ENABLE (1u)
    #endif
    
    #ifndef INTP9_ENABLE
        #define INTP9_ENABLE (1u)
    #endif
    
    #ifndef INTP10_ENABLE
        #define INTP10_ENABLE (1u)
    #endif
#endif /* External interrupts for intc driver */


/* interrupts for CSIG driver */
#ifdef USE_CSIG
    /* CSIG 0 */
    #ifndef INTCSIG0IRE_ENABLE
        #define INTCSIG0IRE_ENABLE (1u) 
    #endif
    
    #ifndef INTCSIG0IR_ENABLE
        #define INTCSIG0IR_ENABLE (1u) 
    #endif

    #ifndef INTCSIG0IC_ENABLE
        #define INTCSIG0IC_ENABLE (1u) 
    #endif
    
    /* CSIG 1 */
    #ifndef INTCSIG1IRE_ENABLE
        #define INTCSIG1IRE_ENABLE (1u) 
    #endif
    
    #ifndef INTCSIG1IR_ENABLE
        #define INTCSIG1IR_ENABLE (1u) 
    #endif

    #ifndef INTCSIG1IC_ENABLE
        #define INTCSIG1IC_ENABLE (1u) 
    #endif
    
    /* CSIG 2 */
    #ifndef INTCSIG2IRE_ENABLE
        #define INTCSIG2IRE_ENABLE (1u) 
    #endif
    
    #ifndef INTCSIG2IR_ENABLE
        #define INTCSIG2IR_ENABLE (1u) 
    #endif

    #ifndef INTCSIG2IC_ENABLE
        #define INTCSIG2IC_ENABLE (1u) 
    #endif
#endif /* interrupts for CSIG driver */


/* interrupts for CSIH driver */
#ifdef USE_CSIH
    /* CSIH0 */
    #ifndef INTCSIH0IC_ENABLE
        #define INTCSIH0IC_ENABLE   (1u)
    #endif
    
    #ifndef INTCSIH0IJC_ENABLE
        #define INTCSIH0IJC_ENABLE   (1u)
    #endif
    
    #ifndef INTCSIH0IRE_ENABLE
        #define INTCSIH0IRE_ENABLE   (1u)
    #endif
    
    #ifndef INTCSIH0IR_ENABLE
        #define INTCSIH0IR_ENABLE   (1u)
    #endif
    
    /* CSIH1 */
    #ifndef INTCSIH1IC_ENABLE
        #define INTCSIH1IC_ENABLE   (1u)
    #endif
    
    #ifndef INTCSIH1IJC_ENABLE
        #define INTCSIH1IJC_ENABLE   (1u)
    #endif
    
    #ifndef INTCSIH1IRE_ENABLE
        #define INTCSIH1IRE_ENABLE   (1u)
    #endif
    
    #ifndef INTCSIH1IR_ENABLE
        #define INTCSIH1IR_ENABLE   (1u)
    #endif
#endif  /* interrupts for CSIH driver */


/* interrupts for SG driver */
#ifdef USE_SG
    #ifndef INTSG0TI_ENABLE
        #define INTSG0TI_ENABLE (1u)
    #endif
#endif /* interrupts for SG driver */

/* interrupts for SSIF driver */
#ifdef USE_SSIF
    #ifndef INTSSIF0_ENABLE
        #define INTSSIF0_ENABLE (1u)
    #endif
    #ifndef INTSSIF0RX_ENABLE
        #define INTSSIF0RX_ENABLE (1u)
    #endif
    #ifndef INTSSIF0TX_ENABLE
        #define INTSSIF0TX_ENABLE (1u)
    #endif
    
    #ifndef INTSSIF1_ENABLE
        #define INTSSIF1_ENABLE (1u)
    #endif
    #ifndef INTSSIF1RX_ENABLE
        #define INTSSIF1RX_ENABLE (1u)
    #endif
    #ifndef INTSSIF1TX_ENABLE
        #define INTSSIF1TX_ENABLE (1u)
    #endif
#endif /* interrupts for SSIF driver */

/* interrupts for IIS driver */
#ifdef USE_IIS
    #ifndef INTIISA0TIA_ENABLE
        #define INTIISA0TIA_ENABLE (1u) 
    #endif
    
    #ifndef INTIISA0TITXU_ENABLE
        #define INTIISA0TITXU_ENABLE (1u) 
    #endif
    
    #ifndef INTIISA0TITXT_ENABLE
        #define INTIISA0TITXT_ENABLE (1u) 
    #endif
    
    #ifndef INTIISA0TIRXO_ENABLE
        #define INTIISA0TIRXO_ENABLE (1u) 
    #endif
    
    #ifndef INTIISA0TIRXT_ENABLE
        #define INTIISA0TIRXT_ENABLE (1u) 
    #endif
    
    #ifndef INTIISA0TIFERR_ENABLE
        #define INTIISA0TIFERR_ENABLE (1u) 
    #endif                
#endif  /* interrupts for IIS driver */


/*Interrupts for IIC*/
#ifdef USE_IIC
    #ifndef INTIICB0TIS_ENABLE
       #define INTIICB0TIS_ENABLE (1u)
    #endif

    #ifndef INTIICB0TIA_ENABLE
       #define INTIICB0TIA_ENABLE (1u)
    #endif

    #ifndef INTIICB1TIS_ENABLE
       #define INTIICB1TIS_ENABLE (1u)
    #endif

    #ifndef INTIICB1TIA_ENABLE
       #define INTIICB1TIA_ENABLE (1u)
    #endif

    #ifndef INTIICB0TIS_FLAG
       #define INTIICB0TIS_FLAG
    #endif

    #ifndef INTIICB0TIA_FLAG
       #define INTIICB0TIA_FLAG
    #endif

    #ifndef INTIICB1TIS_FLAG
       #define INTIICB1TIS_FLAG
    #endif

    #ifndef INTIICB1TIA_FLAG
       #define INTIICB1TIA_FLAG
    #endif
#endif /*Interrupts for IIC*/

/* interrupts for RTCA driver */
#ifdef USE_RTCA
    #ifndef INTRTCA01S_ENABLE
        #define INTRTCA01S_ENABLE (1u) 
    #endif
    
    #ifndef INTRTCA0AL_ENABLE
        #define INTRTCA0AL_ENABLE (1u) 
    #endif  
      
    #ifndef INTRTCA0R_ENABLE
        #define INTRTCA0R_ENABLE (1u) 
    #endif    
#endif /* interrupts for RTCA driver */   
    


/* interrupts for AWOT driver */
#ifdef USE_AWOT
    #ifndef INTAWOT_ENABLE
        #define INTAWOT_ENABLE (1u)
    #endif

#endif /* interrupts for AWOT driver */

/* interrupts for OSTM driver */
#ifdef USE_OSTM
    #ifndef INTOSTM0_ENABLE
        #define INTOSTM0_ENABLE (1u)
    #endif
    #ifndef INTOSTM1_ENABLE
        #define INTOSTM1_ENABLE (1u)
    #endif

#endif /* interrupts for OSTM driver */


/* interrupts for PCMP driver */
#ifdef USE_PCMP
    #ifndef INTPCMP0FFIL_ENABLE
        #define INTPCMP0FFIL_ENABLE (1u)
    #endif
    
    #ifndef INTPCMP0FERR_ENABLE
        #define INTPCMP0FERR_ENABLE (1u)
    #endif
#endif /* interrupts for PCMP driver */

/* interrupts for DAVE2D driver */
#ifdef USE_D2_DRIVER
    #ifndef INTDRW_ENABLE
        #define INTDRW_ENABLE  (1u)
    #endif
#endif /* interrupts for DAVE2D driver */

/* interrupts for DAVE HD driver */
#ifdef USE_DAVEHD
    #ifndef INTDHD_ENABLE
        #define INTDHD_ENABLE  (1u)
    #endif
#endif /* interrupts for DAVE HD driver */

/* interrupts for LCBI driver */
#ifdef USE_LCBI
    #ifndef INTLCBI0RDY_ENABLE
        #define INTLCBI0RDY_ENABLE (1u)
    #endif
    #ifndef INTLCBI0EMPTY_ENABLE
        #define INTLCBI0EMPTY_ENABLE (1u)
    #endif
    #ifndef INTLCBI0HALF_ENABLE
        #define INTLCBI0HALF_ENABLE (1u)
    #endif
    #ifndef INTLCBI0FULL_ENABLE
        #define INTLCBI0FULL_ENABLE (1u)
    #endif    
    #ifndef INTLCBI0QTR_ENABLE
        #define INTLCBI0QTR_ENABLE (1u)
    #endif
    #ifndef INTLCBI03QTR_ENABLE
        #define INTLCBI03QTR_ENABLE (1u)
    #endif
#endif /* interrupts for LCBI driver */

/* interrupts for VDCO */
#ifdef USE_VDCO
    #ifndef INTVDCE0ERR_ENABLE
        #define INTVDCE0ERR_ENABLE        (1u)
    #endif
    #ifndef INTVDCE0GR3VBLANK_ENABLE
        #define INTVDCE0GR3VBLANK_ENABLE  (1u)
    #endif
    #ifndef INTVDCE0S0LOVSYNC_ENABLE
        #define INTVDCE0S0LOVSYNC_ENABLE  (1u)
    #endif
    #ifndef INTVDCE0GR3VLINE_ENABLE
        #define INTVDCE0GR3VLINE_ENABLE   (1u)
    #endif
    #ifndef INTVDCE0S1LOVSYNC_ENABLE
        #define INTVDCE0S1LOVSYNC_ENABLE  (1u)
    #endif
    #ifndef INTVDCE0OIRVIVSYNC_ENABLE
        #define INTVDCE0OIRVIVSYNC_ENABLE (1u)
    #endif
    #ifndef INTVDCE0OIRLOVSYNC_ENABLE
        #define INTVDCE0OIRLOVSYNC_ENABLE (1u)
    #endif
    #ifndef INTVDCE0IRVLINE_ENABLE
        #define INTVDCE0IRVLINE_ENABLE    (1u)
    #endif
    #ifndef INTVDCE0IRWLINE_ENABLE
        #define INTVDCE0IRWLINE_ENABLE    (1u)
    #endif
    #ifndef INTVDCE1ERR_ENABLE
        #define INTVDCE1ERR_ENABLE        (1u)
    #endif
    #ifndef INTVDCE1GR3VBLANK_ENABLE
        #define INTVDCE1GR3VBLANK_ENABLE  (1u)
    #endif
    #ifndef INTVDCE1S0LOVSYNC_ENABLE
        #define INTVDCE1S0LOVSYNC_ENABLE  (1u)
    #endif
    #ifndef INTVDCE1GR3VLINE_ENABLE
        #define INTVDCE1GR3VLINE_ENABLE   (1u)
    #endif
    #ifndef INTVDCE1S1LOVSYNC_ENABLE
        #define INTVDCE1S1LOVSYNC_ENABLE  (1u)
    #endif
#if 0 /* For video input driver VDCI, INTVDCE[01]ERR_ENABLE are shared with video output */
    #ifndef INTVDCE0S0VIVSYNC_ENABLE
        #define INTVDCE0S0VIVSYNC_ENABLE  (1u)
    #endif
    #ifndef INTVDCE0S0VFIELD_ENABLE
        #define INTVDCE0S0VFIELD_ENABLE   (1u)
    #endif
    #ifndef INTVDCE0S0WLINE_ENABLE
        #define INTVDCE0S0WLINE_ENABLE    (1u)
    #endif
    #ifndef INTVDCE0S1WLINE_ENABLE
        #define INTVDCE0S1WLINE_ENABLE    (1u)
    #endif
    #ifndef INTVDCE1S0VIVSYNC_ENABLE
        #define INTVDCE1S0VIVSYNC_ENABLE  (1u)
    #endif
    #ifndef INTVDCE1S0VFIELD_ENABLE
        #define INTVDCE1S0VFIELD_ENABLE   (1u)
    #endif
    #ifndef INTVDCE1S0WLINE_ENABLE
        #define INTVDCE1S0WLINE_ENABLE    (1u)
    #endif
    #ifndef INTVDCE1S1WLINE_ENABLE
        #define INTVDCE1S1WLINE_ENABLE    (1u)
    #endif
#endif

#endif /* interrupts for VDCO */

/* interrupts for VDCE */
#ifdef USE_VDCE
    #ifndef INTVDCE_ENABLE
        #define INTVDCE_ENABLE (1u)
    #endif
#endif /* interrupts for VDCE */

/* interrupts for VOWE */
#if (defined USE_VOWE) || (defined USE_VOWE_REE)
    #ifndef INTVOWE_ENABLE
        #define INTVOWE_ENABLE (1u)
    #endif
#endif /* interrupts for VOWE */

/* interrupts for MIPI */
#ifdef USE_MIPI
    #ifndef INTMIPI0OVF_ENABLE
        #define INTMIPI0OVF_ENABLE (1u)
    #endif
    #ifndef INTMIPI0CTL_ENABLE
        #define INTMIPI0CTL_ENABLE (1u)
    #endif
#endif /* interrupts for MIPI */


/* interrupts for TAUB driver */
#ifndef USE_TAUB
#define USE_TAUB  (1u)
#endif
#ifdef USE_TAUB
    
    #ifndef INTTAUB0I5_ENABLE
        #define INTTAUB0I5_ENABLE (1u)
    #endif
	
	#if 0
	
    /* TauB 0 */
    #ifndef INTTAUB0I0_ENABLE
        #define INTTAUB0I0_ENABLE  (1u)
    #endif
    #ifndef INTTAUB0I1_ENABLE
        #define INTTAUB0I1_ENABLE (1u)
    #endif
    #ifndef INTTAUB0I2_ENABLE
        #define INTTAUB0I2_ENABLE (1u)
    #endif
    #ifndef INTTAUB0I3_ENABLE
        #define INTTAUB0I3_ENABLE (1u)
    #endif
    #ifndef INTTAUB0I4_ENABLE
        #define INTTAUB0I4_ENABLE (1u)
    #endif
    #ifndef INTTAUB0I5_ENABLE
        #define INTTAUB0I5_ENABLE (1u)
    #endif
    #ifndef INTTAUB0I6_ENABLE
        #define INTTAUB0I6_ENABLE (1u)
    #endif
    #ifndef INTTAUB0I7_ENABLE
        #define INTTAUB0I7_ENABLE (1u)
    #endif
    #ifndef INTTAUB0I8_ENABLE
        #define INTTAUB0I8_ENABLE (1u)
    #endif
    #ifndef INTTAUB0I9_ENABLE
        #define INTTAUB0I9_ENABLE (1u)
    #endif
    #ifndef INTTAUB0I10_ENABLE
        #define INTTAUB0I10_ENABLE (1u)
    #endif
    #ifndef INTTAUB0I11_ENABLE
        #define INTTAUB0I11_ENABLE (1u)
    #endif
    #ifndef INTTAUB0I12_ENABLE
        #define INTTAUB0I12_ENABLE (1u)
    #endif
    #ifndef INTTAUB0I13_ENABLE
        #define INTTAUB0I13_ENABLE (1u)
    #endif
    #ifndef INTTAUB0I14_ENABLE
        #define INTTAUB0I14_ENABLE (1u)
    #endif
    #ifndef INTTAUB0I15_ENABLE
        #define INTTAUB0I15_ENABLE (1u)
    #endif

    /* TauB 1 */
    #ifndef INTTAUB1I0_ENABLE
        #define INTTAUB1I0_ENABLE (1u)
    #endif
    #ifndef INTTAUB1I1_ENABLE
        #define INTTAUB1I1_ENABLE (1u)
    #endif
    #ifndef INTTAUB1I2_ENABLE
        #define INTTAUB1I2_ENABLE (1u)
    #endif
    #ifndef INTTAUB1I3_ENABLE
        #define INTTAUB1I3_ENABLE (1u)
    #endif
    #ifndef INTTAUB1I4_ENABLE
        #define INTTAUB1I4_ENABLE (1u)
    #endif
    #ifndef INTTAUB1I5_ENABLE
        #define INTTAUB1I5_ENABLE (1u)
    #endif
    #ifndef INTTAUB1I6_ENABLE
        #define INTTAUB1I6_ENABLE (1u)
    #endif
    #ifndef INTTAUB1I7_ENABLE
        #define INTTAUB1I7_ENABLE (1u)
    #endif
    #ifndef INTTAUB1I8_ENABLE
        #define INTTAUB1I8_ENABLE (1u)
    #endif
    #ifndef INTTAUB1I9_ENABLE
        #define INTTAUB1I9_ENABLE (1u)
    #endif
    #ifndef INTTAUB1I10_ENABLE
        #define INTTAUB1I10_ENABLE (1u)
    #endif
    #ifndef INTTAUB1I11_ENABLE
        #define INTTAUB1I11_ENABLE (1u)
    #endif
    #ifndef INTTAUB1I12_ENABLE
        #define INTTAUB1I12_ENABLE (1u)
    #endif
    #ifndef INTTAUB1I13_ENABLE
        #define INTTAUB1I13_ENABLE (1u)
    #endif
    #ifndef INTTAUB1I14_ENABLE
        #define INTTAUB1I14_ENABLE (1u)
    #endif
    #ifndef INTTAUB1I15_ENABLE
        #define INTTAUB1I15_ENABLE (1u)
    #endif

    /* TauB 2 */
    #ifndef INTTAUB2I0_ENABLE
        #define INTTAUB2I0_ENABLE (1u)
    #endif
    #ifndef INTTAUB2I1_ENABLE
        #define INTTAUB2I1_ENABLE (1u)
    #endif
    #ifndef INTTAUB2I2_ENABLE
        #define INTTAUB2I2_ENABLE (1u)
    #endif
    #ifndef INTTAUB2I3_ENABLE
        #define INTTAUB2I3_ENABLE (1u)
    #endif
    #ifndef INTTAUB2I4_ENABLE
        #define INTTAUB2I4_ENABLE (1u)
    #endif
    #ifndef INTTAUB2I5_ENABLE
        #define INTTAUB2I5_ENABLE (1u)
    #endif
    #ifndef INTTAUB2I6_ENABLE
        #define INTTAUB2I6_ENABLE (1u)
    #endif
    #ifndef INTTAUB2I7_ENABLE
        #define INTTAUB2I7_ENABLE (1u)
    #endif
    #ifndef INTTAUB2I8_ENABLE
        #define INTTAUB2I8_ENABLE (1u)
    #endif
    #ifndef INTTAUB2I9_ENABLE
        #define INTTAUB2I9_ENABLE (1u)
    #endif
    #ifndef INTTAUB2I10_ENABLE
        #define INTTAUB2I10_ENABLE (1u)
    #endif
    #ifndef INTTAUB2I11_ENABLE
        #define INTTAUB2I11_ENABLE (1u)
    #endif
    #ifndef INTTAUB2I12_ENABLE
        #define INTTAUB2I12_ENABLE (1u)
    #endif
    #ifndef INTTAUB2I13_ENABLE
        #define INTTAUB2I13_ENABLE (1u)
    #endif
    #ifndef INTTAUB2I14_ENABLE
        #define INTTAUB2I14_ENABLE (1u)
    #endif
    #ifndef INTTAUB2I15_ENABLE
        #define INTTAUB2I15_ENABLE (1u)
    #endif
	
	#endif
	
#endif  /* interrupts for TAUB */

/* interrupts for TAUJ driver */
#ifdef USE_TAUJ
    /* TAUJ 0 */
    #ifndef INTTAUJ0I0_ENABLE
        #define INTTAUJ0I0_ENABLE (1u) 
    #endif
    
    #ifndef INTTAUJ0I1_ENABLE
        #define INTTAUJ0I1_ENABLE (1u) 
    #endif
    
    #ifndef INTTAUJ0I2_ENABLE
        #define INTTAUJ0I2_ENABLE (1u) 
    #endif
    
    #ifndef INTTAUJ0I3_ENABLE
        #define INTTAUJ0I3_ENABLE (1u) 
    #endif
    
    /* TAUJ 1 */
    #ifndef INTTAUJ1I0_ENABLE
        #define INTTAUJ1I0_ENABLE (1u) 
    #endif
    
    #ifndef INTTAUJ1I1_ENABLE
        #define INTTAUJ1I1_ENABLE (1u) 
    #endif
    
    #ifndef INTTAUJ1I2_ENABLE
        #define INTTAUJ1I2_ENABLE (1u) 
    #endif
    
    #ifndef INTTAUJ1I3_ENABLE
        #define INTTAUJ1I3_ENABLE (1u) 
    #endif
    
    /* TAUJ 2*/
    #ifndef INTTAUJ2I0_ENABLE
        #define INTTAUJ2I0_ENABLE (1u) 
    #endif
    
    #ifndef INTTAUJ2I1_ENABLE
        #define INTTAUJ2I1_ENABLE (1u) 
    #endif
    
    #ifndef INTTAUJ2I2_ENABLE
        #define INTTAUJ2I2_ENABLE (1u) 
    #endif
    
    #ifndef INTTAUJ2I3_ENABLE
        #define INTTAUJ2I3_ENABLE (1u) 
    #endif
#endif /* interrupts for TAUJ driver */

/* interrupts for RTCA driver */
#ifdef USE_RTCA
    #ifndef INTRTCA01S_ENABLE
        #define INTRTCA01S_ENABLE (1u) 
    #endif
    
    #ifndef INTRTCA0AL_ENABLE
        #define INTRTCA0AL_ENABLE (1u) 
    #endif  
      
    #ifndef INTRTCA0R_ENABLE
        #define INTRTCA0R_ENABLE (1u) 
    #endif    
#endif /* interrupts for RTCA driver */   


/* interrupts for SVO (PL111) driver */
#ifdef USE_PL111
    #ifndef INTSVO0FUF_ENABLE
        #define INTSVO0FUF_ENABLE (1u) 
    #endif
    
    #ifndef INTSVO0NBA_ENABLE
        #define INTSVO0NBA_ENABLE (1u) 
    #endif

    #ifndef INTSVO0VCP_ENABLE
        #define INTSVO0VCP_ENABLE (1u) 
    #endif
    
    #ifndef INTSVO0MBE_ENABLE
        #define INTSVO0MBE_ENABLE (1u)   /* master bus error is handled via FeNmi Interrupt */
        #ifndef FENMI_ENABLE
            #define FENMI_ENABLE (1u)
        #endif
    #endif
    
#endif /* interrupts for SVO (PL111) driver */


/* interrupts for VOM driver */
#ifdef USE_VOM
    #ifndef INTVOMN0_ENABLE
        #define INTVOMN0_ENABLE (1u) 
    #endif
    
    #ifndef INTVOMN1_ENABLE
        #define INTVOMN1_ENABLE (1u) 
    #endif
#endif /* interrupts for VOM driver */


/* interrupts for TCON driver */
#ifdef USE_TCON
    #ifndef INTTCON0SYNPO7_ENABLE
        #define INTTCON0SYNPO7_ENABLE (1u)
    #endif

    #ifndef INTTCON0SYNPO11_ENABLE
        #define INTTCON0SYNPO11_ENABLE (1u)
    #endif

    #ifndef INTTCON1SYNPO7_ENABLE
        #define INTTCON1SYNPO7_ENABLE (1u)
    #endif

    #ifndef INTTCON1SYNPO11_ENABLE
        #define INTTCON1SYNPO11_ENABLE (1u)
    #endif
#endif /* interrupts for TCON driver */


/* interrupts for VOCA driver */
#ifdef USE_VOCA
    #ifndef INTVOCA_VOCINT_ENABLE
        #define INTVOCA_VOCINT_ENABLE (1u)
    #endif

    #ifndef INTVOCA_ACTMONINT_ENABLE
        #define INTVOCA_ACTMONINT_ENABLE (1u)
    #endif
#endif /* interrupts for VOCA driver */


/* interrupts for DISCOM driver */
#ifdef USE_DISCOM
    #ifndef INTDISCOM_CMPI0_ENABLE
        #define INTDISCOM_CMPI0_ENABLE (1u)
    #endif

    #ifndef INTDISCOM_CMPI1_ENABLE
        #define INTDISCOM_CMPI1_ENABLE (1u)
    #endif
#endif /* interrupts for DISCOM driver */


/* interrupts for VI driver */
#ifdef USE_VI
    #ifndef INTVI0SCL_ENABLE
        #define INTVI0SCL_ENABLE (1u)
    #endif

    #ifndef INTVI0FFO_ENABLE
        #define INTVI0FFO_ENABLE (1u)
    #endif
    
    #ifndef INTVI0AHBERR_ENABLE
        #define INTVI0AHBERR_ENABLE (1u)   /* AHB error is handled via FeNmi Interrupt */
        #ifndef FENMI_ENABLE
            #define FENMI_ENABLE (1u)
        #endif
    #endif
#endif /* interrupts for VI driver */

/* interrupts for VI driver */
#ifdef USE_VI
    #ifndef INTVI0SCL_ENABLE
        #define INTVI0SCL_ENABLE (1u)
    #endif

    #ifndef INTVI0FFO_ENABLE
        #define INTVI0FFO_ENABLE (1u)
    #endif
    
    #ifndef INTVI0AHBERR_ENABLE
        #define INTVI0AHBERR_ENABLE (1u)   /* AHB error is handled via FeNmi Interrupt */
        #ifndef FENMI_ENABLE
            #define FENMI_ENABLE (1u)
        #endif
    #endif
#endif /* interrupts for VI driver */



/* interrupts for LCDBIF driver */
#ifdef USE_LCDBIF
    #ifndef INTLCBI0RDY_ENABLE
        #define INTLCBI0RDY_ENABLE (1u)
    #endif

    #ifndef INTLCBI0EMPT_ENABLE
        #define INTLCBI0EMPT_ENABLE (1u)
    #endif

    #ifndef INTLCBI0QTR_ENABLE
        #define INTLCBI0QTR_ENABLE (1u)
    #endif
    
    #ifndef INTLCBI0HALF_ENABLE
        #define INTLCBI0HALF_ENABLE (1u)
    #endif
    
    #ifndef INTLCBI03QTR_ENABLE
        #define INTLCBI03QTR_ENABLE (1u)
    #endif
    
    #ifndef INTLCBI0FULL_ENABLE
        #define INTLCBI0FULL_ENABLE (1u)
    #endif
#endif /* interrupts for LCDBIF driver */


/* interrupts for RLIN3 driver */
#ifdef USE_RLIN3
    #ifndef  INTRLIN30UR2_ENABLE
        #define INTRLIN30UR2_ENABLE (1u)
    #endif 
    #ifndef   INTRLIN30UR1_ENABLE   
        #define INTRLIN30UR1_ENABLE (1u)
    #endif  
    #ifndef   INTRLIN30UR0_ENABLE    
        #define INTRLIN30UR0_ENABLE (1u)
    #endif  
    
    #ifndef  INTRLIN31UR2_ENABLE
        #define INTRLIN31UR2_ENABLE (1u)
    #endif 
    #ifndef   INTRLIN31UR1_ENABLE   
        #define INTRLIN31UR1_ENABLE (1u)
    #endif  
    #ifndef   INTRLIN31UR0_ENABLE    
        #define INTRLIN31UR0_ENABLE (1u)
    #endif

    #ifndef  INTRLIN32UR2_ENABLE
        #define INTRLIN32UR2_ENABLE (1u)
    #endif 
    #ifndef   INTRLIN32UR1_ENABLE   
        #define INTRLIN32UR1_ENABLE (1u)
    #endif  
    #ifndef   INTRLIN32UR0_ENABLE    
        #define INTRLIN32UR0_ENABLE (1u)
    #endif
    
    #ifndef  INTRLIN33UR2_ENABLE
        #define INTRLIN33UR2_ENABLE (1u)
    #endif 
    #ifndef   INTRLIN33UR1_ENABLE   
        #define INTRLIN33UR1_ENABLE (1u)
    #endif  
    #ifndef   INTRLIN33UR0_ENABLE    
        #define INTRLIN33UR0_ENABLE (1u)
    #endif
#endif  /* USE_RLIN3 */


/* interrupts for DMA */

#ifdef USE_DMA
    #ifndef INTDMA0_ENABLE
        #define INTDMA0_ENABLE (1u)
    #endif
    #ifndef INTDMA1_ENABLE
        #define INTDMA1_ENABLE (1u)
    #endif
    #ifndef INTDMA2_ENABLE
        #define INTDMA2_ENABLE (1u)
    #endif
    #ifndef INTDMA3_ENABLE
        #define INTDMA3_ENABLE (1u)
    #endif
    #ifndef INTDMA4_ENABLE
        #define INTDMA4_ENABLE (1u)
    #endif
    #ifndef INTDMA5_ENABLE
        #define INTDMA5_ENABLE (1u)
    #endif
    #ifndef INTDMA6_ENABLE
        #define INTDMA6_ENABLE (1u)
    #endif
    #ifndef INTDMA7_ENABLE
        #define INTDMA7_ENABLE (1u)
    #endif
#endif

#ifdef USE_AFCAN
#ifndef INTFCNAWUP_ENABLE
#define INTFCNAWUP_ENABLE (1u)
#endif
#ifndef INTFCNA0ERR1_ENABLE
#define INTFCNA0ERR1_ENABLE (1u)
#endif
#ifndef INTFCNA0REC1_ENABLE
#define INTFCNA0REC1_ENABLE (1u)
#endif
#ifndef INTFCNA0TRX1_ENABLE
#define INTFCNA0TRX1_ENABLE (1u)
#endif
#ifndef INTFCNA1ERR1_ENABLE
#define INTFCNA1ERR1_ENABLE (1u)
#endif
#ifndef INTFCNA1REC1_ENABLE
#define INTFCNA1REC1_ENABLE (1u)
#endif
#ifndef INTFCNA1TRX1_ENABLE
#define INTFCNA1TRX1_ENABLE (1u)
#endif
#ifndef INTFCNA2ERR1_ENABLE
#define INTFCNA2ERR1_ENABLE (1u)
#endif
#ifndef INTFCNA2REC1_ENABLE
#define INTFCNA2REC1_ENABLE (1u)
#endif
#ifndef INTFCNA2TRX1_ENABLE
#define INTFCNA2TRX1_ENABLE (1u)
#endif
#endif


/* interrupts for Vcomp driver */
#ifdef USE_VCPC
    #ifndef INTVCMP0_ENABLE
        #define INTVCMP0_ENABLE (1u)
    #endif
    #ifndef INTVCMP1_ENABLE
        #define INTVCMP1_ENABLE (1u)
    #endif
#endif

/* interrupts for ADC driver */
#ifdef USE_ADCA
    #ifndef INTADCA0ERR_ENABLE
        #define INTADCA0ERR_ENABLE (1u)
    #endif
    #ifndef INTADCA0TG0_ENABLE
        #define INTADCA0TG0_ENABLE (1u)
    #endif
    #ifndef INTADCA0TG1_ENABLE
        #define INTADCA0TG1_ENABLE (1u)
    #endif    
    #ifndef INTADCA0TG2_ENABLE
        #define INTADCA0TG2_ENABLE (1u)
    #endif
    #ifndef INTADCA0TLLT_ENABLE
        #define INTADCA0TLLT_ENABLE (1u)
    #endif
#endif


/* interrupts for LVI driver */
#ifdef USE_LVI
    #ifndef INTLVI_ENABLE
        #define INTLVI_ENABLE (1u)
    #endif
#endif

/* interrupts for CLMA */
#ifdef USE_CLMA
    #ifndef INTCLMA0_ENABLE
        #define INTCLMA0_ENABLE (1u)
    #endif
#endif

/* interrupt for MLB */
#ifdef USE_MLB
    #ifndef INTMLB0CI_ENABLE
        #define INTMLB0CI_ENABLE (1u)
    #endif
    #ifndef INTMLB0SI_ENABLE
        #define INTMLB0SI_ENABLE (1u)
    #endif
#endif

/*Interrupts for ISM*/
#ifdef USE_ISM 

    #ifndef INTISM0REACHED_ENABLE 
        #define INTISM0REACHED_ENABLE (1u) 
    #endif
    #ifndef INTISM0DONE_ENABLE    
        #define INTISM0DONE_ENABLE (1u)    
    #endif
    #ifndef INTISM0ZPDAD_ENABLE   
        #define INTISM0ZPDAD_ENABLE (1u)   
    #endif
    #ifndef INTISM0ZPD_ENABLE     
        #define INTISM0ZPD_ENABLE (1u)     
    #endif

#endif /*Interrupts for ISM*/ 

/* Interupts for the JPG compression unit */
#if defined(USE_JCU) || defined(USE_JCUA)
    #ifndef INTJCU0DTI_ENABLE
        #define INTJCU0DTI_ENABLE (1u)
    #endif

    #ifndef INTJCU0EDI_ENABLE
        #define INTJCU0EDI_ENABLE (1u)
    #endif      
#endif

/* interrupts for VRAM */
#ifdef USE_VRAM
    #ifndef INTVRAM_ENABLE
        #define INTVRAM_ENABLE (1u)
    #endif
#endif /* interrupts for VRAM */

/* interrupts for Ethernet */
#ifdef USE_AVB
    #ifndef INTETNB0LINE0_ENABLE
        #define INTETNB0LINE0_ENABLE (1u)
    #endif
    #ifndef INTETNB0LINE1_ENABLE
        #define INTETNB0LINE1_ENABLE (1u)
    #endif
    #ifndef INTETNB0LINE2_ENABLE
        #define INTETNB0LINE2_ENABLE (1u)
    #endif
    #ifndef INTETNB0LINE3_ENABLE
        #define INTETNB0LINE3_ENABLE (1u)
    #endif
#endif

#ifdef USE_RSCAN
	#ifndef INTRCANGRECC_ENABLE
		#define INTRCANGRECC_ENABLE (1u)
	#endif
#endif

#endif /* INT_ENABLE_H_  */


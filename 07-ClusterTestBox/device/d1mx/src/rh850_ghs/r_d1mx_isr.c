/*
****************************************************************************
PROJECT : r7f701412 
FILE    : $Id: r_r7f701412_isr.c 3922 2014-09-22 08:40:10Z golczewskim $
============================================================================
DESCRIPTION
r7f701412 (D1M2(H)) interrupt service routines
============================================================================
                            C O P Y R I G H T
============================================================================
                       Copyright (c) 2013 - 2015
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
  Title: r7f701412 Interrupt Functions

  Define interrupt handling.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_dev_api.h"
#include "r_icu_feret.h"

#include "int_enabled.h"
#include"r_ism_api.h"

extern void Process_Can(void);
extern void freqcalcute2(void);
extern void freqcalcute1(void);

#if defined (R_DBG_PRINT_DEV_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#else
    #define R_DBG_PRINT(lvl, txt) 
#endif

#ifdef USE_RLIN3
#include "r_rlin3_api.h"
#endif
#ifdef USE_AWOT
#include "r_awot_api.h"
#endif
#ifdef USE_OSTM
#include "r_ostm_api.h"
#endif
#ifdef USE_GPIO
#include "r_gpio_api.h"
#endif

#ifdef USE_CSIG
 #include "r_csig_api.h"
#endif

#ifdef USE_CSIH
 #include "r_csih_api.h"
#endif

#ifdef USE_DMA
#include "r_dma_api.h"
#endif

#ifdef USE_RTCA
#include "r_rtca_api.h"
#endif

#ifdef USE_TAUB
//#include "r_taub_api.h"
#endif

#ifdef USE_WDTA
#include "r_wdta_api.h"
#endif

#ifdef USE_TAUJ
#include "r_tauj_api.h"
#endif

#ifdef USE_PCMP
#include "r_pcmp_api.h"
#endif

#ifdef USE_SG
#include "r_sg_api.h"
#endif

#ifdef USE_SSIF
#include "r_ssif_api.h"
#endif

#ifdef USE_VOCA
#include "r_ddb_api.h"      /* display database with display timings */
#include "r_voca_api.h"
#endif

#ifdef USE_DISCOM
#include "r_discom_api.h"
#endif

#ifdef USE_D2_DRIVER
#include "dave_driver.h"
#include "dave_irqfunc.h"
#endif

#ifdef USE_SFMA
#include "r_sfma_api.h"
#endif

#ifdef USE_DAVEHD
#include "davehd_kernel_os.h"
#include "davehd_os_hw.h"
#endif

#ifdef USE_ROS
#include "r_os_api.h"
#endif

#ifdef USE_JCU
    #include "r_jcu_api.h"    
#endif

#ifdef USE_LCBI
#include "r_ddb_api.h"
#include "r_lcbi_api.h"
#endif

#ifdef USE_JCUA
    #include "r_jcua_api.h"
#endif

#ifdef USE_VDCO
#include "r_ddb_api.h"
#include "r_vdco_api.h"
#endif

#ifdef USE_VDCE
#include "r_ddb_api.h"
#include "r_vdce_api.h"
#endif

#ifdef USE_MIPI
#include "r_bsp_gfx_mipi_csi2.h"
#endif

#ifdef USE_VRAM
#include "r_vram_api.h"
#endif

#ifdef USE_VOWE
#include "r_typedefs.h"
#include "r_vowe_api.h"
#endif

#ifdef USE_VOWE_REE
#include "r_typedefs.h"
#include "r_vowe_api.h"
#endif

#ifdef USE_ECM
#include "r_ecm_api.h"
#endif

#ifdef USE_AVB
#include "r_avb_api.h"
#endif

#ifdef USE_ADC7010
#include "r_adc_api.h"
#endif

#ifdef USE_ISM
#include "r_ism_api.h"
#endif

/*******************************************************************************
  Section: Local Constants 
*/


#pragma ghs startnomisra

#ifdef PRIO_0_ENABLE
#pragma ghs interrupt
void EINTPRIO_0(void) 
{
    asm("nop"); 
}
#endif

/**************************************************/
/* PIE Level "Interrupts"                             */
/**************************************************/

#ifdef PIE_ENABLE
void R_PIE_Isr( void );

FETRAP_EXCEPTION(PIE, loc_PIE);

void loc_PIE(void) {
    R_PIE_Isr();
}
#endif

/**************************************************/
/* FE Level "Interrupts"                             */
/**************************************************/

#ifdef FEINT_ENABLE

FETRAP_EXCEPTION(FEINT, loc_FEINT);

void loc_FEINT(void) {
    R_ECM_FeIntIsr(0);
}
#endif

#ifdef FENMI_ENABLE
#pragma ghs interrupt
void FENMI(void) {
    R_ECM_FeNmiIsr(0);
}
#endif


/**************************************************/
/* ADC Interrupts                                */
/**************************************************/

#ifdef INTADCE0TSN_ENABLE
#pragma ghs interrupt 
void INTADCE0TSN(void) {
     R_ADC_IsrTsn(0);
}
#endif

#ifdef INTADCE0I1_ENABLE
#pragma ghs interrupt
void INTADCE0I1(void) {
    R_ADC_Isr0(0);
}
#endif

#ifdef INTADCE0I2_ENABLE
#pragma ghs interrupt
void INTADCE0I2(void) {
    R_ADC_Isr1(0);
}
#endif

#ifdef INTADCE0I3_ENABLE
#pragma ghs interrupt
void INTADCE0I3(void) {
    R_ADC_Isr2(0);
}
#endif


/**************************************************/
/* AWOT Interrupts                                */
/**************************************************/

#ifdef INTAWOT_ENABLE
#pragma ghs interrupt
void INTAWOT(void) {
    R_AWOT_TintIsr(0);
}
#endif

/**************************************************/
/* OSTM Interrupts                                */
/**************************************************/

#ifdef INTOSTM0_ENABLE
#pragma ghs interrupt 
void INTOSTM0(void) 
{
    R_OSTM_TintIsr(0);
}
#endif

#ifdef INTOSTM1_ENABLE
#pragma ghs interrupt
void INTOSTM1(void) 
{
    R_OSTM_TintIsr(1);
}
#endif

/**************************************************/
/* RLIN3 Interrupts                               */
/**************************************************/

/*  RLIN3 unit 0 */
#ifdef INTRLIN30UR0_ENABLE
#pragma ghs interrupt
void INTRLIN30UR0(void) {
    R_RLIN3_IsrTx(0);
}
#endif

#ifdef INTRLIN30UR1_ENABLE
#pragma ghs interrupt
void INTRLIN30UR1(void) {
    R_RLIN3_IsrRx(0);
}
#endif

#ifdef INTRLIN30UR2_ENABLE
#pragma ghs interrupt
void INTRLIN30UR2(void) {
    R_RLIN3_IsrRE(0);
}
#endif

/*  RLIN3 unit 1 */
#ifdef INTRLIN31UR0_ENABLE
#pragma ghs interrupt
void INTRLIN31UR0(void) {
    R_RLIN3_IsrTx(1);
}
#endif

#ifdef INTRLIN31UR1_ENABLE
#pragma ghs interrupt
void INTRLIN31UR1(void) {
    R_RLIN3_IsrRx(1);
}
#endif

#ifdef INTRLIN31UR2_ENABLE
#pragma ghs interrupt
void INTRLIN31UR2(void) {
    R_RLIN3_IsrRE(1);
}
#endif

/*  RLIN3 unit 2 */
#ifdef INTRLIN32UR0_ENABLE
#pragma ghs interrupt
void INTRLIN32UR0(void) {
    R_RLIN3_IsrTx(2);
}
#endif

#ifdef INTRLIN32UR1_ENABLE
#pragma ghs interrupt
void INTRLIN32UR1(void) {
    R_RLIN3_IsrRx(2);
}
#endif

#ifdef INTRLIN32UR2_ENABLE
#pragma ghs interrupt
void INTRLIN32UR2(void) {
    R_RLIN3_IsrRE(2);
}
#endif

/*  RLIN3 unit 3 */
#ifdef INTRLIN33UR0_ENABLE
#pragma ghs interrupt
void INTRLIN33UR0(void) {
    R_RLIN3_IsrTx(3);
}
#endif

#ifdef INTRLIN33UR1_ENABLE
#pragma ghs interrupt
void INTRLIN33UR1(void) {
    R_RLIN3_IsrRx(3);
}
#endif

#ifdef INTRLIN33UR2_ENABLE
#pragma ghs interrupt
void INTRLIN33UR2(void) {
    R_RLIN3_IsrRE(3);
}
#endif


/**************************************************/
/* CSIG Interrupts                                */
/**************************************************/

#ifdef INTCSIG0IRE_ENABLE
#pragma ghs interrupt
void INTCSIG0IRE(void) {
    R_CSIG_IsrTire(0);
}
#endif

#ifdef INTCSIG0IR_ENABLE
#pragma ghs interrupt
void INTCSIG0IR(void) {
    R_CSIG_IsrTir(0);
}
#endif

#ifdef INTCSIG0IC_ENABLE
#pragma ghs interrupt
void INTCSIG0IC(void) {
    R_CSIG_IsrTic(0);
}
#endif

#ifdef INTCSIG1IRE_ENABLE
#pragma ghs interrupt
void INTCSIG1IRE(void) {
    R_CSIG_IsrTire(1);
}
#endif

#ifdef INTCSIG1IR_ENABLE
#pragma ghs interrupt
void INTCSIG1IR(void) {
    R_CSIG_IsrTir(1);
}
#endif

#ifdef INTCSIG1IC_ENABLE
#pragma ghs interrupt
void INTCSIG1IC(void) {
    R_CSIG_IsrTic(1);
}
#endif

#ifdef INTCSIG2IRE_ENABLE
#pragma ghs interrupt
void INTCSIG2IRE(void) {
    R_CSIG_IsrTire(2);
}
#endif

#ifdef INTCSIG2IR_ENABLE
#pragma ghs interrupt
void INTCSIG2IR(void) {
    R_CSIG_IsrTir(2);
}
#endif

#ifdef INTCSIG2IC_ENABLE
#pragma ghs interrupt
void INTCSIG2IC(void) {
    R_CSIG_IsrTic(2);
}
#endif


/**************************************************/
/* ETNB Interrupts                                */
/**************************************************/
#ifdef USE_AVB

#pragma ghs interrupt
void INTETNB0LINE0(void)
{
    R_AVB_Isr(0, R_AVB_INT_DATA);
}

#pragma ghs interrupt
void INTETNB0LINE1(void)
{
    R_AVB_Isr(0, R_AVB_INT_ERR);
}

#pragma ghs interrupt
void INTETNB0LINE2(void)
{
    R_AVB_Isr(0, R_AVB_INT_MNG);
}

#pragma ghs interrupt
void INTETNB0LINE3(void)
{
    R_AVB_Isr(0, R_AVB_INT_MAC);
}

#endif



/**************************************************/
/* CSIH Interrupts                                */
/**************************************************/

#ifdef USE_CSIH
#pragma ghs interrupt
void INTCSIH0IC (void)
{
    R_CSIH_IsrTic(0);
}

#pragma ghs interrupt
void INTCSIH0IR (void)
{
    R_CSIH_IsrTir(0);
}

#pragma ghs interrupt
void INTCSIH0IRE (void)
{
    R_CSIH_IsrTire(0);    
}

#pragma ghs interrupt
void INTCSIH0IJC (void)
{
    R_CSIH_IsrTijc(0);     
}

#pragma ghs interrupt
void INTCSIH1IC (void)
{
    R_CSIH_IsrTic(1);    
}

#pragma ghs interrupt
void INTCSIH1IR (void)
{
    R_CSIH_IsrTir(1);
}

#pragma ghs interrupt
void INTCSIH1IRE (void)
{
    R_CSIH_IsrTire(1); 
}

#pragma ghs interrupt
void INTCSIH1IJC (void)
{
    R_CSIH_IsrTijc(1); 
}
#endif  /* USE_CSIH */


/**************************************************/
/* DMA Interrupts                                 */
/**************************************************/

#ifdef INTDMA0_ENABLE
#pragma ghs interrupt
void INTDMA0(void) 
{
    R_DMA_Isr(0, 0);
}
#endif


#ifdef INTDMA1_ENABLE
#pragma ghs interrupt
void INTDMA1(void) 
{
    R_DMA_Isr(0, 1);
}
#endif


#ifdef INTDMA2_ENABLE
#pragma ghs interrupt
void INTDMA2(void) 
{
    R_DMA_Isr(0, 2);
}
#endif


#ifdef INTDMA3_ENABLE
#pragma ghs interrupt
void INTDMA3(void) 
{
    R_DMA_Isr(0, 3);
}
#endif


#ifdef INTDMA4_ENABLE
#pragma ghs interrupt
void INTDMA4(void) 
{
    R_DMA_Isr(0, 4);
}
#endif


#ifdef INTDMA5_ENABLE
#pragma ghs interrupt
void INTDMA5(void) 
{
    R_DMA_Isr(0, 5);
}
#endif


#ifdef INTDMA6_ENABLE
#pragma ghs interrupt
void INTDMA6(void) 
{
    R_DMA_Isr(0, 6);
}
#endif


#ifdef INTDMA7_ENABLE
#pragma ghs interrupt
void INTDMA7(void) 
{
    R_DMA_Isr(0, 7);
}
#endif

#ifdef INTDMA8_ENABLE
#pragma ghs interrupt
void INTDMA8(void) 
{
    R_DMA_Isr(0, 8);
}
#endif


#ifdef INTDMA9_ENABLE
#pragma ghs interrupt
void INTDMA9(void) 
{
    R_DMA_Isr(0, 9);
}
#endif


#ifdef INTDMA10_ENABLE
#pragma ghs interrupt
void INTDMA10(void) 
{
    R_DMA_Isr(0, 10);
}
#endif


#ifdef INTDMA11_ENABLE
#pragma ghs interrupt
void INTDMA11(void) 
{
    R_DMA_Isr(0, 11);
}
#endif


#ifdef INTDMA12_ENABLE
#pragma ghs interrupt
void INTDMA12(void) 
{
    R_DMA_Isr(0, 12);
}
#endif


#ifdef INTDMA13_ENABLE
#pragma ghs interrupt
void INTDMA13(void) 
{
    R_DMA_Isr(0, 13);
}
#endif


#ifdef INTDMA14_ENABLE
#pragma ghs interrupt
void INTDMA14(void) 
{
    R_DMA_Isr(0, 14);
}
#endif


#ifdef INTDMA15_ENABLE
#pragma ghs interrupt
void INTDMA15(void) 
{
    R_DMA_Isr(0, 15);
}
#endif

/**************************************************
TAUJ  interrupts 
**************************************************/

#ifdef USE_TAUJ

#pragma ghs interrupt
void INTTAUJ0I0(void) 
{
    R_TAUJ_Isr(0,(r_tauj_Interrupt_t)0); 
} 

#pragma ghs interrupt
void INTTAUJ0I1(void) 
{
    R_TAUJ_Isr(0,(r_tauj_Interrupt_t)1); 
} 
#pragma ghs interrupt
void INTTAUJ0I2(void) 
{
   R_TAUJ_Isr(0,(r_tauj_Interrupt_t)2); 
} 
#pragma ghs interrupt
void INTTAUJ0I3(void) 
{
    R_TAUJ_Isr(0,(r_tauj_Interrupt_t)3);
} 

#pragma ghs interrupt
void INTTAUJ1I0(void) 
{
    R_TAUJ_Isr(1,(r_tauj_Interrupt_t)0); 
} 

#pragma ghs interrupt
void INTTAUJ1I1(void) 
{
    R_TAUJ_Isr(1,(r_tauj_Interrupt_t)1); 
} 
#pragma ghs interrupt
void INTTAUJ1I2(void) 
{
   R_TAUJ_Isr(1,(r_tauj_Interrupt_t)2); 
} 
#pragma ghs interrupt
void INTTAUJ1I3(void) 
{
    R_TAUJ_Isr(1,(r_tauj_Interrupt_t)3);
} 

#pragma ghs interrupt
void INTTAUJ2I0(void) 
{
    R_TAUJ_Isr(2,(r_tauj_Interrupt_t)0); 
} 

#pragma ghs interrupt
void INTTAUJ2I1(void) 
{
    R_TAUJ_Isr(2,(r_tauj_Interrupt_t)1); 
} 
#pragma ghs interrupt
void INTTAUJ2I2(void) 
{
   R_TAUJ_Isr(2,(r_tauj_Interrupt_t)2); 
} 
#pragma ghs interrupt
void INTTAUJ2I3(void) 
{
    R_TAUJ_Isr(2,(r_tauj_Interrupt_t)3);
} 

#endif

/**************************************************/
/* WDTA Interrupts                                */
/**************************************************/
#ifdef INTWDTA0_ENABLE
#pragma ghs interrupt
void INTWDTA0(void) 
{
    R_WDTA_Isr(0);
}
#endif
#ifdef INTWDTA1_ENABLE
#pragma ghs interrupt
void INTWDTA1(void) 
{
    R_WDTA_Isr(1);
}
#endif

/**************************************************/
/* RTCA - Interrupts                              */
/**************************************************/
#ifdef USE_RTCA

#pragma ghs interrupt
void INTRTCA01S(void) 
{
    R_RTCA_Isr(0, R_RTCA_INT_1S);
}

#pragma ghs interrupt
void INTRTCA0AL(void)
{
    R_RTCA_Isr(0, R_RTCA_INT_AL);
}

#pragma ghs interrupt
void INTRTCA0R(void)
{
    R_RTCA_Isr(0, R_RTCA_INT_R);
}

#endif  /* RTCA */

/**************************************************
TAUB  interrupts 
**************************************************/

#ifdef USE_TAUB

#pragma ghs interrupt
void INTTAUB0I0(void) 
  {
    //R_TAUB_Isr(0,(r_taub_Interrupt_t) 0); 
  } 

#pragma ghs interrupt
void INTTAUB0I1(void) 
  {
   // R_TAUB_Isr(0,(r_taub_Interrupt_t) 1); 
  } 

#pragma ghs interrupt
void INTTAUB0I2(void) 
  {
   // R_TAUB_Isr(0,(r_taub_Interrupt_t) 2); 
  } 

#pragma ghs interrupt
void INTTAUB0I3(void) 
  {

  } 

#pragma ghs interrupt
void INTTAUB0I4(void) 
  {
    //R_TAUB_Isr(0,(r_taub_Interrupt_t) 4); 
  } 

#pragma ghs interrupt
void INTTAUB0I5(void) 
  {
     freqcalcute2();
  } 

#pragma ghs interrupt
void INTTAUB0I6(void) 
  {
   // R_TAUB_Isr(0,(r_taub_Interrupt_t) 6); 
  } 

#pragma ghs interrupt
void INTTAUB0I7(void) 
  {
      freqcalcute1();
  } 

#pragma ghs interrupt
void INTTAUB0I8(void) 
  {
   // R_TAUB_Isr(0,(r_taub_Interrupt_t) 8); 
  } 

#pragma ghs interrupt
void INTTAUB0I9(void) 
  {
    //R_TAUB_Isr(0,(r_taub_Interrupt_t) 9); 
  } 

#pragma ghs interrupt
void INTTAUB0I10(void) 
  {
   // R_TAUB_Isr(0,(r_taub_Interrupt_t) 10); 
  } 

#pragma ghs interrupt
void INTTAUB0I11(void) 
  {
   // R_TAUB_Isr(0,(r_taub_Interrupt_t) 11); 
  } 

#pragma ghs interrupt
void INTTAUB0I12(void) 
  {
   // R_TAUB_Isr(0,(r_taub_Interrupt_t) 12); 
  } 

#pragma ghs interrupt
void INTTAUB0I13(void) 
  {
    //R_TAUB_Isr(0,(r_taub_Interrupt_t) 13); 
  } 

#pragma ghs interrupt
void INTTAUB0I14(void) 
  {
    //R_TAUB_Isr(0,(r_taub_Interrupt_t) 14); 
  } 

#pragma ghs interrupt
void INTTAUB0I15(void) 
  {
   // R_TAUB_Isr(0,(r_taub_Interrupt_t) 15); 
  } 

#pragma ghs interrupt
void INTTAUB1I0(void) 
  {
   // R_TAUB_Isr(1,(r_taub_Interrupt_t) 0); 
  } 

#pragma ghs interrupt
void INTTAUB1I1(void) 
  {
   // R_TAUB_Isr(1,(r_taub_Interrupt_t) 1); 
  } 

#pragma ghs interrupt
void INTTAUB1I2(void) 
  {
  //  R_TAUB_Isr(1,(r_taub_Interrupt_t) 2); 
  } 

#pragma ghs interrupt
void INTTAUB1I3(void) 
  {
  //  R_TAUB_Isr(1,(r_taub_Interrupt_t) 3); 
  } 

#pragma ghs interrupt
void INTTAUB1I4(void) 
  {
   // R_TAUB_Isr(1,(r_taub_Interrupt_t) 4); 
  } 

#pragma ghs interrupt
void INTTAUB1I5(void) 
  {
   // R_TAUB_Isr(1,(r_taub_Interrupt_t) 5); 
  } 

#pragma ghs interrupt
void INTTAUB1I6(void) 
  {
   // R_TAUB_Isr(1,(r_taub_Interrupt_t) 6); 
  } 

#pragma ghs interrupt
void INTTAUB1I7(void) 
  {
   // R_TAUB_Isr(1,(r_taub_Interrupt_t) 7); 
  } 

#pragma ghs interrupt
void INTTAUB1I8(void) 
  {
   // R_TAUB_Isr(1,(r_taub_Interrupt_t) 8); 
  } 

#pragma ghs interrupt
void INTTAUB1I9(void) 
  {
   // R_TAUB_Isr(1,(r_taub_Interrupt_t) 9); 
  } 

#pragma ghs interrupt
void INTTAUB1I10(void) 
  {
   // R_TAUB_Isr(1,(r_taub_Interrupt_t) 10); 
  } 

#pragma ghs interrupt
void INTTAUB1I11(void) 
  {
   // R_TAUB_Isr(1,(r_taub_Interrupt_t) 11); 
  } 

#pragma ghs interrupt
void INTTAUB1I12(void) 
  {
   // R_TAUB_Isr(1,(r_taub_Interrupt_t) 12); 
  } 

#pragma ghs interrupt
void INTTAUB1I13(void) 
  {
   // R_TAUB_Isr(1,(r_taub_Interrupt_t) 13); 
  } 

#pragma ghs interrupt
void INTTAUB1I14(void) 
  {
   // R_TAUB_Isr(1,(r_taub_Interrupt_t) 14); 
  } 

#pragma ghs interrupt
void INTTAUB1I15(void) 
  {
 //   R_TAUB_Isr(1,(r_taub_Interrupt_t) 15); 
  } 

#pragma ghs interrupt
void INTTAUB2I0(void) 
  {
   // R_TAUB_Isr(2,(r_taub_Interrupt_t) 0); 
  } 

#pragma ghs interrupt
void INTTAUB2I1(void) 
  {
    //R_TAUB_Isr(2,(r_taub_Interrupt_t) 1); 
  } 

#pragma ghs interrupt
void INTTAUB2I2(void) 
  {
   // R_TAUB_Isr(2,(r_taub_Interrupt_t) 2); 
  } 

#pragma ghs interrupt
void INTTAUB2I3(void) 
  {
  //  R_TAUB_Isr(2,(r_taub_Interrupt_t) 3); 
  } 

#pragma ghs interrupt
void INTTAUB2I4(void) 
  {
  //  R_TAUB_Isr(2,(r_taub_Interrupt_t) 4); 
  } 

#pragma ghs interrupt
void INTTAUB2I5(void) 
  {
  //  R_TAUB_Isr(2,(r_taub_Interrupt_t) 5); 
  } 

#pragma ghs interrupt
void INTTAUB2I6(void) 
  {
   // R_TAUB_Isr(2,(r_taub_Interrupt_t) 6); 
  } 

#pragma ghs interrupt
void INTTAUB2I7(void) 
  {
   // R_TAUB_Isr(2,(r_taub_Interrupt_t) 7); 
  } 

#pragma ghs interrupt
void INTTAUB2I8(void) 
  {
   // R_TAUB_Isr(2,(r_taub_Interrupt_t) 8); 
  } 

#pragma ghs interrupt
void INTTAUB2I9(void) 
  {
    //R_TAUB_Isr(2,(r_taub_Interrupt_t) 9); 
  } 

#pragma ghs interrupt
void INTTAUB2I10(void) 
  {
   // R_TAUB_Isr(2,(r_taub_Interrupt_t) 10); 
  } 

#pragma ghs interrupt
void INTTAUB2I11(void) 
  {
   // R_TAUB_Isr(2,(r_taub_Interrupt_t) 11); 
  } 

#pragma ghs interrupt
void INTTAUB2I12(void) 
  {
   // R_TAUB_Isr(2,(r_taub_Interrupt_t) 12); 
  } 

#pragma ghs interrupt
void INTTAUB2I13(void) 
  {
   // R_TAUB_Isr(2,(r_taub_Interrupt_t) 13); 
  } 

#pragma ghs interrupt
void INTTAUB2I14(void) 
  {
   // R_TAUB_Isr(2,(r_taub_Interrupt_t) 14); 
  } 

#pragma ghs interrupt
void INTTAUB2I15(void) 
  {
    //R_TAUB_Isr(2,(r_taub_Interrupt_t) 15); 
  } 

#endif

/**************************************************
GPIO (external) interrupts 
**************************************************/

#ifdef USE_GPIO

#pragma ghs interrupt
void INTP0(void) 
{
    R_GPIO_IntIsr(0); 
} 

#pragma ghs interrupt
void INTP1(void) 
{
    R_GPIO_IntIsr(1); 
} 

#pragma ghs interrupt
void INTP2(void) 
{
    R_GPIO_IntIsr(2); 
} 

#pragma ghs interrupt
void INTP3(void) 
{
    R_GPIO_IntIsr(3); 
} 

#pragma ghs interrupt
void INTP4(void) 
{
    R_GPIO_IntIsr(4); 
} 

#pragma ghs interrupt
void INTP5(void) 
{
    R_GPIO_IntIsr(5); 
} 

#pragma ghs interrupt
void INTP6(void) 
{
    R_GPIO_IntIsr(6); 
} 

#pragma ghs interrupt
void INTP7(void) 
{
    R_GPIO_IntIsr(7); 
} 

#pragma ghs interrupt
void INTP8(void) 
{
    R_GPIO_IntIsr(8); 
} 

#pragma ghs interrupt
void INTP9(void) 
{
    R_GPIO_IntIsr(9); 
} 

#pragma ghs interrupt
void INTP10(void) 
{
    R_GPIO_IntIsr(10); 
} 

#endif


/**************************************************/
/* pcmp - PCM-PWM Interrupts                      */
/**************************************************/

#ifdef USE_PCMP

#pragma ghs interrupt
void INTPCMP0FFIL(void) 
{
    R_PCMP_Isr(0, R_PCMP_INT_FFIL);
}

#pragma ghs interrupt
void INTPCMP0FERR(void)
{
    R_PCMP_Isr(0, R_PCMP_INT_ERR);
}

#endif  /* PCMP */


/**************************************************/
/* sg - SG Interrupts                             */
/**************************************************/

#ifdef USE_SG

#pragma ghs interrupt
void INTSG0TI(void)
{
    R_SG_Isr(0);
}

#pragma ghs interrupt
void INTSG1TI(void)
{
    R_SG_Isr(1);
}

#pragma ghs interrupt
void INTSG2TI(void)
{
    R_SG_Isr(2);
}

#pragma ghs interrupt
void INTSG3TI(void)
{
    R_SG_Isr(3);
}

#pragma ghs interrupt
void INTSG4TI(void)
{
    R_SG_Isr(4);
}
#endif  /* SG */


/**************************************************/
/* ssif - SSIF Interrupts                         */
/**************************************************/

#ifdef USE_SSIF

#pragma ghs interrupt
void INTSSIF0(void)
{
    R_SSIF_IsrIRQ(0);
}

#pragma ghs interrupt
void INTSSIF0RX(void)
{
    R_SSIF_IsrRX(0);
}

#pragma ghs interrupt
void INTSSIF0TX(void)
{
    R_SSIF_IsrTX(0);
}

#pragma ghs interrupt
void INTSSIF1(void)
{
    R_SSIF_IsrIRQ(1);
}

#pragma ghs interrupt
void INTSSIF1RX(void)
{
    R_SSIF_IsrRX(1);
}

#pragma ghs interrupt
void INTSSIF1TX(void)
{
    R_SSIF_IsrTX(1);
}
#endif  /* SSIF */


/**************************************************/
/* gfx - LCBI Interrupts                          */
/**************************************************/

#ifdef USE_LCBI
#pragma ghs interrupt
void INTLCBI0RDY(void)
{
    R_LCBI_Isr(0, R_LCBI_INT_RDY);
}

#pragma ghs interrupt
void INTLCBI0EMPTY(void)
{
    R_LCBI_Isr(0, R_LCBI_INT_EMPT);
}

#pragma ghs interrupt
void INTLCBI0HALF(void)
{
    R_LCBI_Isr(0, R_LCBI_INT_HALF);
}

#pragma ghs interrupt
void INTLCBI0FULL(void)
{
    R_LCBI_Isr(0, R_LCBI_INT_FULL);
}

#pragma ghs interrupt
void INTLCBI0QTR(void)
{
    R_LCBI_Isr(0, R_LCBI_INT_QTR);
}

#pragma ghs interrupt
void INTLCBI03QTR(void)
{
    R_LCBI_Isr(0, R_LCBI_INT_3QTR);
}
#endif /* USE_LCBI */


/**************************************************/
/* gfx - VDCO Interrupts                          */
/**************************************************/

#if 0
/* Todo: Example of implementation with VDCI */
#ifdef INTVDCO_ENABLE || INTVDCI_ENABLE
#pragma ghs interrupt
void INTVDCO_CH0_INT0(void) {
#ifdef INTVDCO_ENABLE 
    R_VDCO_Isr0(0);
#endif
#ifdef INTVDCI_ENABLE 
    R_VDCI_Isr0(0);
#endif
}
#endif
#endif



#ifdef USE_VDCO

#pragma ghs interrupt
void INTVDCE0ERR(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCO_IsrError(0);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}


#pragma ghs interrupt
void INTVDCE0GR3VBLANK(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCO_Isr(0,R_VDCO_INTC_VBLANK_DELAY);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}


#pragma ghs interrupt
void INTVDCE0S0LOVSYNC(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCO_Isr(0,R_VDCO_INTC_VBLANK);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}


#pragma ghs interrupt
void INTVDCE0GR3VLINE(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCO_Isr(0,R_VDCO_INTC_SCANLINE);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}


#pragma ghs interrupt
void INTVDCE0S1LOVSYNC(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCO_Isr(0,R_VDCO_INTC_VBLANK_1);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}


#pragma ghs interrupt
void INTVDCE0OIRVIVSYNC(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCO_Isr(0,R_VDCO_INTC_OIR_VSCYNC_WRITE);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}


#pragma ghs interrupt
void INTVDCE0OIRLOVSYNC(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCO_Isr(0,R_VDCO_INTC_OIR_VBLANK);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}


#pragma ghs interrupt
void INTVDCE0IRVLINE(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCO_Isr(0,R_VDCO_INTC_OIR_SCANLINE);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}


#pragma ghs interrupt
void INTVDCE0IRWLINE(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCO_Isr(0,R_VDCO_INTC_OIR_SCANLINE_WRITE);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}


#pragma ghs interrupt
void INTVDCE1ERR(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCO_IsrError(1);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}


#pragma ghs interrupt
void INTVDCE1GR3VBLANK(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCO_Isr(1,R_VDCO_INTC_VBLANK_DELAY);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}


#pragma ghs interrupt
void INTVDCE1S0LOVSYNC(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCO_Isr(1,R_VDCO_INTC_VBLANK);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}


#pragma ghs interrupt
void INTVDCE1GR3VLINE(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCO_Isr(1,R_VDCO_INTC_SCANLINE);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}


#pragma ghs interrupt
void INTVDCE1S1LOVSYNC(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCO_Isr(1,R_VDCO_INTC_VBLANK_1);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}
#endif /* USE_VDCO */


/**************************************************/
/* gfx - VDCE Interrupts                          */
/**************************************************/


#ifdef INTVDCE_ENABLE  /* USE_VDCE */

#pragma ghs interrupt
void INTVDCE_CH0_ERR(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    /* R_VDCE_Isr(0, R_VDCE_INTC_ERROR); */
    R_VDCE_IsrError(0);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#pragma ghs interrupt
void INTVDCE_CH0_GR3VBLANK(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCE_Isr(0, R_VDCE_INTC_VBLANK_DELAY);    /* R_VDCE_INTC_VBLANK_DELAY ? */
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#pragma ghs interrupt
void INTVDCE_CH0_S0VIVSYNC(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCE_Isr(0, R_VDCE_INTC_CAP_VBLANK);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#pragma ghs interrupt
void INTVDCE_CH0_S0LOVSYNC(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCE_Isr(0, R_VDCE_INTC_VBLANK);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#pragma ghs interrupt
void INTVDCE_CH0_GR3VLINE(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCE_Isr(0, R_VDCE_INTC_SCANLINE);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#pragma ghs interrupt
void INTVDCE_CH0_S0VFIELD(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCE_Isr(0, R_VDCE_INTC_CAP_END_OF_FIELD);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#pragma ghs interrupt
void INTVDCE_CH0_S0WLINE(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCE_Isr(0, R_VDCE_INTC_CAP_SCANLINE_WRITE);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#pragma ghs interrupt
void INTVDCE_CH0_S1LOVSYNC(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCE_Isr(0, R_VDCE_INTC_VBLANK_1);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#pragma ghs interrupt
void INTVDCE_CH0_S1WLINE(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    /* Not supported for D1x VDCE. */
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#pragma ghs interrupt
void INTVDCE_CH0_OIRVIVSYNC(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCE_Isr(0, R_VDCE_INTC_OIR_VSCYNC_WRITE);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#pragma ghs interrupt
void INTVDCE_CH0_OIRLOVSYNC(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCE_Isr(0, R_VDCE_INTC_OIR_VBLANK);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#pragma ghs interrupt
void INTVDCE_CH0_OIRVLINE(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCE_Isr(0, R_VDCE_INTC_OIR_SCANLINE);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#pragma ghs interrupt
void INTVDCE_CH0_OIRWLINE(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCE_Isr(0, R_VDCE_INTC_OIR_SCANLINE_WRITE);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#pragma ghs interrupt
void INTVDCE_CH1_ERR(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCE_IsrError(1);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#pragma ghs interrupt
void INTVDCE_CH1_GR3VBLANK(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCE_Isr(1, R_VDCE_INTC_VBLANK_DELAY);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#pragma ghs interrupt
void INTVDCE_CH1_S0VIVSYNC(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCE_Isr(1, R_VDCE_INTC_CAP_VBLANK);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#pragma ghs interrupt
void INTVDCE_CH1_S0LOVSYNC(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCE_Isr(1, R_VDCE_INTC_VBLANK);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#pragma ghs interrupt
void INTVDCE_CH1_GR3VLINE(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCE_Isr(1, R_VDCE_INTC_SCANLINE);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#pragma ghs interrupt
void INTVDCE_CH1_S0VFIELD(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCE_Isr(1, R_VDCE_INTC_CAP_END_OF_FIELD);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#pragma ghs interrupt
void INTVDCE_CH1_S0WLINE(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCE_Isr(1, R_VDCE_INTC_CAP_SCANLINE_WRITE);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#pragma ghs interrupt
void INTVDCE_CH1_S1LOVSYNC(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VDCE_Isr(1, R_VDCE_INTC_VBLANK_1);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#pragma ghs interrupt
void INTVDCE_CH1_S1WLINE(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    /* Not supported for D1x VDCE. */
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#endif  /* INTVDCE_ENABLE / USE_VDCE */


#ifdef INTVRAM_ENABLE
#pragma ghs interrupt
void INTVRAM_CH0_1BIT(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VRAM_Isr0(0);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#pragma ghs interrupt
void INTVRAM_CH0_2BIT(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VRAM_Isr1(0);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#pragma ghs interrupt
void INTVRAM_CH0_OVF(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VRAM_Isr2(0);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#pragma ghs interrupt
void INTVRAM_CH1_1BIT(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VRAM_Isr0(1);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#pragma ghs interrupt
void INTVRAM_CH1_2BIT(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VRAM_Isr1(1);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#pragma ghs interrupt
void INTVRAM_CH1_OVF(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VRAM_Isr2(1);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}
#endif



/**************************************************/
/* gfx - VOWE Interrupts                          */
/**************************************************/

#ifdef INTVOWE_ENABLE
#pragma ghs interrupt
void INTVOWE(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VOWE_Isr0(0);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}
#endif


/**************************************************/
/* gfx - MIPI Interrupts                          */
/**************************************************/

#ifdef INTMIPI0OVF_ENABLE
#pragma ghs interrupt
void INTMIPI0OVF(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_MIPI_IsrOVF();
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}
#endif

#ifdef INTMIPI0CTL_ENABLE
#pragma ghs interrupt
void INTMIPI0CTL(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_MIPI_IsrCTL();
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}
#endif


/**************************************************/
/* gfx - VOCA Interrupts                         */
/**************************************************/

#ifdef INTVOCA_VOCINT_ENABLE
#pragma ghs interrupt
void INTVOCA_VOCINT(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VOCA_IsrVocInt(0);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}
#endif

#ifdef INTVOCA_ACTMONINT_ENABLE
#pragma ghs interrupt
void INTVOCA_ACTMONINT(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_VOCA_IsrActMonInt(0);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}
#endif


/**************************************************/
/* gfx - DISCOM Interrupts                        */
/**************************************************/

#ifdef INTDISCOM_CMPI0_ENABLE
#pragma ghs interrupt
void INTDISCOM_CMPI0(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_DISCOM_IsrCMPI(0);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}
#endif

#ifdef INTDISCOM_CMPI1_ENABLE
#pragma ghs interrupt
void INTDISCOM_CMPI1(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_DISCOM_IsrCMPI(1);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}
#endif


/**************************************************/
/* JCU Interrupts                                 */
/**************************************************/

#ifdef INTJCU0EDI_ENABLE
#pragma ghs interrupt
void INTJCU0EDI(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
#ifdef USE_JCU
    R_JCU_JediIsr();
#else
    R_JCUA_IsrStop(0);
#endif
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}
#endif

#ifdef INTJCU0DTI_ENABLE
#pragma ghs interrupt
void INTJCU0DTI(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
#ifdef USE_JCU
    R_JCU_JdtiIsr();
#else
    R_JCUA_IsrFinish(0);
#endif
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}
#endif   


/**************************************************/
/* gfx - dhd Interrupts                        */
/**************************************************/

#ifdef INTDHD_ENABLE
#pragma ghs interrupt
void INTDHD_SYNC(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    dhd_sys_IsrDave(0);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#pragma ghs interrupt
void INTDHD_PAUSE(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    dhd_sys_IsrDave(0);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#pragma ghs interrupt
void INTDHD_SPECIAL(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    dhd_sys_IsrDave(0);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}

#endif


/**************************************************/
/* ISM Interrupts                                 */
/**************************************************/

/*  ISM unit 0 */
//#ifdef INTISM0REACHED_ENABLE
#pragma ghs interrupt
void INTISM0REACHED(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_ISM_IsrReached(0);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}
//#endif

//#ifdef INTISM0DONE_ENABLE
#pragma ghs interrupt
void INTISM0DONE(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_ISM_IsrDone(0);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}
//#endif

//#ifdef INTISM0ZPDAD_ENABLE
#pragma ghs interrupt
void INTISM0ZPDAD(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_ISM_IsrZpdad(0);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}
//#endif

//#ifdef INTISM0ZPD_ENABLE
#pragma ghs interrupt
void INTISM0ZPD(void) {
#ifdef USE_ROS
    R_OS_Prv_EnterInterrupt();
#endif
    R_ISM_IsrZpd(0);
#ifdef USE_ROS
    R_OS_Prv_LeaveInterrupt();
#endif
}
//#endif

#ifdef INTRCANGRECC_ENABLE
#pragma ghs interrupt
void INTRCANGRECC70(void)
{
	Process_Can();
}
#endif


#pragma ghs endnomisra


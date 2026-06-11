/*===========================================================================*/
/* Module      = r_xbus_xcopy.s                                              */
/* Version     = V1.00                                                       */
/*===========================================================================*/
/*                                  COPYRIGHT                                */
/*===========================================================================*/
/* Copyright (c) 2014 by Renesas Electronics Europe GmbH,                    */
/*               a company of the Renesas Electronics Corporation            */
/*===========================================================================*/
/* Purpose:      Copy routine for memories on the XC Bus                     */
/*                                                                           */
/*===========================================================================*/
/*                                                                           */
/* Warranty Disclaimer                                                       */
/*                                                                           */
/* Because the Product(s) is licensed free of charge, there is no warranty   */
/* of any kind whatsoever and expressly disclaimed and excluded by Renesas,  */
/* either expressed or implied, including but not limited to those for       */
/* non-infringement of intellectual property, merchantability and/or         */
/* fitness for the particular purpose.                                       */
/* Renesas shall not have any obligation to maintain, service or provide bug */
/* fixes for the supplied Product(s) and/or the Application.                 */
/*                                                                           */
/* Each User is solely responsible for determining the appropriateness of    */
/* using the Product(s) and assumes all risks associated with its exercise   */
/* of rights under this Agreement, including, but not limited to the risks   */
/* and costs of program errors, compliance with applicable laws, damage to   */
/* or loss of data, programs or equipment, and unavailability or             */
/* interruption of operations.                                               */
/*                                                                           */
/* Limitation of Liability                                                   */
/*                                                                           */
/* In no event shall Renesas be liable to the User for any incidental,       */
/* consequential, indirect, or punitive damage (including but not limited    */
/* to lost profits) regardless of whether such liability is based on breach  */
/* of contract, tort, strict liability, breach of warranties, failure of     */
/* essential purpose or otherwise and even if advised of the possibility of  */
/* such damages. Renesas shall not be liable for any services or products    */
/* provided by third party vendors, developers or consultants identified or  */
/* referred to the User by Renesas in connection with the Product(s) and/or  */
/* the Application.                                                          */
/*                                                                           */
/*===========================================================================*/
/* Environment:                                                              */
/*         Device:         RH850G3M core devices (with FPU)                  */
/*                         RH850G3K core devices (__HAS_FPU__ is undefined)  */
/*         IDE:            GHS Multi for V800  V6.xx or later                */
/*===========================================================================*/

        .text

/****************************************************************************
  Function: XCopy

  Memcopy optimzed for AXI Cache access 

  Parameters:
  Src (r6), Dst(r7), End(r8) 
*/
    .align(8)
    .globl _R_XBUS_XCopy
_R_XBUS_XCopy:

/* Prefetch cache from Src-RAM for each cache line */
  ld.w 0x00[r6],r9
  ld.w 0x20[r6],r10
  ld.w 0x40[r6],r15
  ld.w 0x60[r6],r24
  /* It is more efficient not to reuse these single word values, 
     but to do a double word load/store from the same address. 
     For some memory targets it will have no effect (avg ~0%), 
     for some targets it will have a positive effect (avg ~+20%) */

   .copyStart:
 /* 1st line fetch from SRC-Ram */
  ld.dw 0x00[r6],r16
  ld.dw 0x08[r6],r18
  ld.dw 0x10[r6],r20
  ld.dw 0x18[r6],r22
 /* Interleaved prefetch of 1st line for next round (therefore the 5th line) */
  ld.w 0x00+0x80[r6],r9

 /* write to 1st line at Dst-RAM */
  st.dw r16,0x00[r7]
  st.dw r18,0x08[r7]
  st.dw r20,0x10[r7]
  st.dw r22,0x18[r7]

 /* 2nd line fetch from SRC-Ram */
  ld.dw 0x20[r6],r16
  ld.dw 0x28[r6],r18
  ld.dw 0x30[r6],r20
  ld.dw 0x38[r6],r22
  /* Interleaved prefetch of 2nd line for next round */
  ld.w 0x20+0x80[r6],r10
  
/* write to 2nd line at Dst-RAM */
  st.dw r16,0x20[r7]
  st.dw r18,0x28[r7]
  st.dw r20,0x30[r7]
  st.dw r22,0x38[r7]

/* 3rd line fetch from SRC-Ram */
  ld.dw 0x40[r6],r16
  ld.dw 0x48[r6],r18
  ld.dw 0x50[r6],r20
  ld.dw 0x58[r6],r22
  /* Interleaved prefetch of 3rd line for next round */
  ld.w 0x40+0x80[r6],r15

/* write to 3rd line at Dst-RAM */
  st.dw r16,0x40[r7]
  st.dw r18,0x48[r7]
  st.dw r20,0x50[r7]
  st.dw r22,0x58[r7]

/* 4th line fetch from SRC-Ram */ 
  ld.dw 0x60[r6],r16
  ld.dw 0x68[r6],r18
  ld.dw 0x70[r6],r20
  ld.dw 0x78[r6],r22
  /* Interleaved prefetch of 4th line for next round */
  ld.w 0x60+0x80[r6],r24
  
/* Execute comparison earlier, so branch prediction has some time to react */
/* The following store operation have to be adjusted due to the early addition of 0x80. */
  add 0x80,r6
  add 0x80,r7
  cmp  r6,r8

/* write to 4th line at Dst-RAM */
  st.dw r16,0x60-0x80[r7]
  st.dw r18,0x68-0x80[r7]
  st.dw r20,0x70-0x80[r7]
  st.dw r22,0x78-0x80[r7]

  bne  .copyStart
  nop
	jmp     [lp]
	
	

        
 /* EOF */
	


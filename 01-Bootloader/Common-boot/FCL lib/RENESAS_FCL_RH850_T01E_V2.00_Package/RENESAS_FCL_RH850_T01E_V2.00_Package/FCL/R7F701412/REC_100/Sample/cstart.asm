;   Copyright(c) 2013 Renesas Electronics Corporation
;   RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
;   This program must be used solely for the purpose for which
;   it was furnished by Renesas Electronics Corporation. No part of this
;   program may be reproduced or disclosed to others, in any
;   form, without the prior written permission of Renesas Electronics
;   Corporation.
;   NOTE       : THIS IS A TYPICAL EXAMPLE.

	.extern	_main
	.extern	__INITSCT_RH
	;.extern	__gp_data
	;.extern	__ep_data
	.public	__start
	.public	_hdwinit
	.public	_abort

;-----------------------------------------------------------------------------
;	system stack
;-----------------------------------------------------------------------------
STACKSIZE	.set	0x800
	.section	".stack.bss", bss
	.align	4
	.ds	(STACKSIZE)
	.align	4
_stacktop:

;-----------------------------------------------------------------------------
RAM_START  	.set  	0xFED80000
RAM_END  	.set    0xFEE00000
;-----------------------------------------------------------------------------

;-----------------------------------------------------------------------------
;	section initialize table
;-----------------------------------------------------------------------------
	.section	".INIT_DSEC.const", const
	.align	4
	.dw	#__s.data,	#__e.data,	#__s.data.R

	.section	".INIT_BSEC.const", const
	.align	4
	.dw	#__s.bss,	#__e.bss

;-----------------------------------------------------------------------------
;	startup
;-----------------------------------------------------------------------------
	.section	".text", text
	.align	2
__start:
$if 0
	mov	LOCAL_RAM_ADDR, r6
	mov	LOCAL_RAM_END, r7
	jarl	_zeroclr4, lp		;  clear local RAM area

	mov	GLOBAL_RAM_ADDR, r6
	mov	GLOBAL_RAM_END, r7
	jarl	_zeroclr4, lp		;  clear global RAM area
$endif
	mov	#_stacktop, sp		;  set sp register
	;mov	#__gp_data, gp		;  set gp register
	;mov	#__ep_data, ep		;  set ep register

	jarl	_hdwinit, lp		;  initialize hardware

	; -- Init used RAM ---
	mov RAM_START, r6
	mov RAM_END, r7

	_RAMINIT:
	st.w r0, 0[r6]
	add 4, r6
	cmp r6, r7
	bnz _RAMINIT

	mov	#__s.INIT_DSEC.const, r6
	mov	#__e.INIT_DSEC.const, r7
	mov	#__s.INIT_BSEC.const, r8
	mov	#__e.INIT_BSEC.const, r9
	jarl32	__INITSCT_RH, lp	;  initialize RAM area
	
	jarl32	_main, lp		;  call main function
_exit:
	br	_exit			;  end of program

;-----------------------------------------------------------------------------
;	zeroclr4
;-----------------------------------------------------------------------------
	.align	2
_zeroclr4:
	;shr	2, r6
	;shl	2, r6
	br	.L4
.L3:
	st.w	r0, [r6]
	add	4, r6
.L4:
	cmp	r6, r7
	bh	.L3
	jmp	[lp]

	.section	".text", text
;-----------------------------------------------------------------------------
;	hdwinit
;-----------------------------------------------------------------------------
	EIC0	.set	0xfffeea00

	.align	2
_hdwinit:
	mov	#__sVECT, r11
	ldsr	r11, 4, 1		;  set INTBP
	mov	EIC0, r11		;  get EIC0
	set1	6, 0[r11]		;  set EIINT0 as table-reference
	set1	6, 2[r11]		;  set EIINT1 as table-reference
	set1	6, 4[r11]		;  set EIINT2 as table-reference

	stsr	5, r10, 0		;  get PSW

	;movhi	0x0001, r0, r11
	;or	r11, r10		;  enable FPU
	
	;movhi	0x0002, r0, r11
	;or	r11, r10		;  enable SIMD

	;movea	0x0020, r0, r11
	;xor	r11, r10		;  enable interrupt
	
	;movhi	0x4000, r0, r11
	;or	r11, r10		;  super visor mode -> user mode

	ldsr	r10, 1, 0		;  set EIPSW
	ldsr	lp, 0, 0		;  set EIPC
	eiret				;  ret (EIPSW->PSW, EIPC->PC)

;-----------------------------------------------------------------------------
;	abort
;-----------------------------------------------------------------------------
	.align	2
_abort: 				;  abort()
	br	_abort

;-----------------------------------------------------------------------------
;	dummy section
;-----------------------------------------------------------------------------
	.section	".data", data
.L.dummy.data:
	.section	".bss", bss
.L.dummy.bss:
	.section	".const", const
.L.dummy.const:
	.section	".text", text
.L.dummy.text:
;-------------------- end of start up module -------------------;

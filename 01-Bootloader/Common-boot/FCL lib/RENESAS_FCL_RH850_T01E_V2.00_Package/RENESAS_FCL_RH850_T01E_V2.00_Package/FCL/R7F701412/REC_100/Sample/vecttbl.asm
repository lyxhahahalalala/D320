;   Copyright(c) 2012, 2013 Renesas Electronics Corporation
;   RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
;   This program must be used solely for the purpose for which
;   it was furnished by Renesas Electronics Corporation. No part of this
;   program may be reproduced or disclosed to others, in any
;   form, without the prior written permission of Renesas Electronics
;   Corporation.

	.extern	__start

	.section "RESET", text
	.align	16
	jr32	__start

	.section "VECT", text
	.align	16
	jr32	_Dummy ;EXCEPTION_SYSERR

	.align	16
	jr32	_Dummy ;EXCEPTION_HVTRAP

	.align	16
	jr32	_Dummy ;EXCEPTION_FETRAP

	.align	16
	jr32	_Dummy ;EXCEPTION_TRAP0

	.align	16
	jr32	_Dummy ;EXCEPTION_TRAP1

	.align	16
	jr32	_Dummy ;EXCEPTION_RIE

	.align	16
	jr32	_Dummy ;EXCEPTION_FPP_FPI

	.align	16
	jr32	_Dummy ;EXCEPTION_UCPOP

	.align	16
	jr32	_Dummy ;EXCEPTION_MIP_MDP

	.align	16
	jr32	_Dummy ;EXCEPTION_PIE

	.align	16
	jr32	_Dummy ;EXCEPTION_Debug

	.align	16
	jr32	_Dummy ;EXCEPTION_MAE

	.align	16
	jr32	_Dummy ;(R.F.U)

	.align	16
	jr32	_Dummy ;FENMI

	.align	16
	jr32	_Dummy ;FEINT

	.align	16
	jr32	_Dummy_EI ;EIINT_PRIORITY00

	.align	16
	jr32	_Dummy_EI ;EIINT_PRIORITY01

	.align	16
	jr32	_Dummy_EI ;EIINT_PRIORITY02

	.align	16
	jr32	_Dummy_EI ;EIINT_PRIORITY03

	.align	16
	jr32	_Dummy_EI ;EIINT_PRIORITY04

	.align	16
	jr32	_Dummy_EI ;EIINT_PRIORITY05

	.align	16
	jr32	_Dummy_EI ;EIINT_PRIORITY06

	.align	16
	jr32	_Dummy_EI ;EIINT_PRIORITY07

	.align	16
	jr32	_Dummy_EI ;EIINT_PRIORITY08

	.align	16
	jr32	_Dummy_EI ;EIINT_PRIORITY09

	.align	16
	jr32	_Dummy_EI ;EIINT_PRIORITY10

	.align	16
	jr32	_Dummy_EI ;EIINT_PRIORITY11

	.align	16
	jr32	_Dummy_EI ;EIINT_PRIORITY12

	.align	16
	jr32	_Dummy_EI ;EIINT_PRIORITY13

	.align	16
	jr32	_Dummy_EI ;EIINT_PRIORITY14

	.align	16
	jr32	_Dummy_EI ;EIINT_PRIORITY15


	.section "EIINTTBL", const
	.align	4
	.dw	#_Dummy_EI ;EIINT0
	.dw	#_Dummy_EI ;EIINT1
	.dw	#_Dummy_EI ;EIINT2
	.ds	(512-12)


	.section ".text", text
	.align	2
_Dummy:
	br	_Dummy

_Dummy_EI:
	eiret

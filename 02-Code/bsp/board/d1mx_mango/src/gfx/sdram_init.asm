;;**************************************************************
;;
;; Title  : RH850 function test
;;
;; $Id: sdram_init.asm 4988 2015-03-30 13:06:17Z golczewskim $
;; $Name:  $
;;
;; Copyright 2010 Renesas Technology Corp., All right reserved.
;;
;;**************************************************************
;;_write_sfr32 .macro sfr_addr,wdata
;;  mov  sfr_addr,r9
;;  mov  wdata,r10
;;  st.w r10,[r9]
;;.endm
;;
;;_cmp_sfr32 .macro sfr_addr,expdata,expmask
;;  mov  sfr_addr,r9
;;  ld.w [r9],r10
;;  mov  expdata,r11
;;  mov  expmask,r12
;;  and  r12,r10
;;  cmp  r10,r11
;;  bne  error_end
;;.endm
;;
;;_watch_sfr32 .macro sfr_addr,expdata,expmask
;;.local m1
;;  mov  sfr_addr,r9
;;  mov  expdata,r10
;;  mov  expmask,r11
;;m1:
;;  ld.w [r9],r12
;;  and  r11,r12
;;  cmp  r12,r10
;;  bne  m1
;;.endm

addreg_loopwait .MACRO num
.LOCAL m1
  xor           r9,r9
  mov           num,r10
m1:
  nop
  add           1,r9
  cmp           r9,r10
  bne           m1
  nop
.ENDM

 $INCLUDE (reg_acc.asm)

  CKSC_IDDR2_CTL        .set 0xFFF85140
  CKSC_IAXIAHB_CTL      .set 0xFFF85180
  MRSTC                 .set 0xFFF8F400
  PROTCMDMRST           .set 0xFFF8F480

SDRA_BASE       .set    0xf2fe0000
DBSCVCR0        .set    SDRA_BASE + 0x000
DBSCVCR1        .set    SDRA_BASE + 0x004
DBKIND          .set    SDRA_BASE + 0x008
DBSTATE         .set    SDRA_BASE + 0x00c
DBEN            .set    SDRA_BASE + 0x010
DBCMDCNT        .set    SDRA_BASE + 0x014

DBCONF          .set    SDRA_BASE + 0x020
DBDMOV          .set    SDRA_BASE + 0x024

DBTR0           .set    SDRA_BASE + 0x030
DBTR1           .set    SDRA_BASE + 0x034
DBTR2           .set    SDRA_BASE + 0x038

DBRFPDN0        .set    SDRA_BASE + 0x040
DBRFPDN1        .set    SDRA_BASE + 0x044
DBRFPDN2        .set    SDRA_BASE + 0x048
DBRFSTS         .set    SDRA_BASE + 0x04c

DBMRCNT         .set    SDRA_BASE + 0x060

DBRSET          .set    SDRA_BASE + 0x070
DBBCHOICE0      .set    SDRA_BASE + 0x074
DBBCHOICE1      .set    SDRA_BASE + 0x078
DBTTR           .set    SDRA_BASE + 0x07c
DBRSET          .set    SDRA_BASE + 0x070

DBPDCNT0        .set    SDRA_BASE + 0x108

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.section ".text", text
.public _SDR_INIT
  
_SDR_INIT:

;;clock gate open
           _wrpt    0xFFF87000, CKSC_IDDR2_CTL,   0x1             ;ddr2 gate
           _wrpt    0xFFF87000, CKSC_IAXIAHB_CTL,   0x1             ;axi gate


;;set DDRPHY offset
_write_sfr32    DBPDCNT0, 0x13000000
_write_sfr32    DBPDCNT0, 0x13000001

;;CKE set to "high"
_write_sfr32    DBCMDCNT, 0x00000003

;;wait 200usec
addreg_loopwait  0x4000

;;SDR-SDRAM of D1M1H adapter board for Micron MT48LC4M32B2P - 6A IT
;;tRCD=3(18nsec), tRP=3(18nsec), CASL=3(18nsec)
;;tWR=2(12nsec), tRAS=5(42nsec), tREF=16msec(64msec=consumer), tRP=2(18nsec),
;;tRRD=2(12nsec), tRC=tRFC=6(60nsec)

;;set SDRAM timing parameter
_write_sfr32    DBRSET, 0x00000000
_write_sfr32    DBTTR, 0x00000000
;;128Mbit memory(4Mx32bit) row=12bit/column=8bit/bank=2bit
;;SPLIT(DBCONF[24:16]) = 8'b1_0001_0001
;;BWIDTH(DBCONF[1:0]=2'b10
;;other = 0
_write_sfr32    DBCONF, 0x01110002
;;CL=3, TRAS=5, TRFC=6, TRCD=3
_write_sfr32    DBTR0, 0x03050503
;;CKEH=2 , TRP=3, TRRD=2, TWR=2
_write_sfr32    DBTR1, 0x01020101
;;TRTP=4(CL+BL-(CL-2)), TRC=6, RDWR=7, WRRD=5
_write_sfr32    DBTR2, 0x02050604
   
;;set SDR (it same as mobile-SDR)
_write_sfr32    DBKIND, 0x00000001
   
   
;;execute PALL by DBCMDCNT
_write_sfr32    DBCMDCNT, 0x00000002

;;execute REF by DBCMDCNT
_write_sfr32    DBCMDCNT, 0x00000004

;;execute REF by DBCMDCNT
_write_sfr32    DBCMDCNT, 0x00000004

;;execute MRS by DBMRCNT
_write_sfr32    DBMRCNT, 0x00000032

_write_sfr32    DBEN, 0x00000001

;;0x186=390cyc
_write_sfr32    DBRFPDN1, 0x00000186
;;390x7=2730(0xaaa), (3(tRP)+3(tRCD)+2(tWR)+3)x5=55(0x37)cyc
_write_sfr32    DBRFPDN2, 0x0aaa0037
_write_sfr32    DBRFPDN0, 0x00010000

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


jmp [lp]



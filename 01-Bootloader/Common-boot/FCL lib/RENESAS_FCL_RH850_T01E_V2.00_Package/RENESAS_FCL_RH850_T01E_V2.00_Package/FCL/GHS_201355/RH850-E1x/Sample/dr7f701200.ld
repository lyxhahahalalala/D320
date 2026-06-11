/**************************************************************/
/*
CONSTANTS
{
  zero_start = 0xffff8000
}
*/
/**************************************************************/
MEMORY
{
    iROM01          : ORIGIN  =  0x00000000,  LENGTH  =  4M             /*  Last 0x003FFFFF                           */
    iROM02          : ORIGIN  =  0x01000000,  LENGTH  =  32K            /*  Last 0x01007FFF include Reset Vector      */
//  Peripheral01    : ORIGIN  =  0x10000000,  LENGTH  =  512M           /*  Last 0x2FFFFFFF                           */

    LRAM_PE1        : ORIGIN  =  0xFEBF0000,  LENGTH  =  64K            /*  Last 0xFEBFFFFF                           */
    LRAM_self       : ORIGIN  =  0xFEDF4000,  LENGTH  =  64K            /*  Last 0xFEDFFFFF Local RAM ACCESS          */
    GRAM            : ORIGIN  =  0xFEEE0000,  LENGTH  =  256K           /*  Last 0xFEF1FFFF                           */

    Peripheral02    : ORIGIN  =  0xFF000000,  LENGTH  =  16M-128K       /*  Last 0xFFFDFFFF                           */
    Peculiarity01   : ORIGIN  =  0xFFFEE000,  LENGTH  =  8K             /*  Last 0xFFFEFFFF                           */
    Peripheral03    : ORIGIN  =  0xFFFF5000,  LENGTH  =  44K            /*  Last 0xFFFFFFFF                           */
}

DEFAULTS {
    stack_reserve  = 5k
}

/**************************************************************/
SECTIONS
{
/* Start of internal ROM area (iROM) */
    .RESET                                      :> iROM01       /*  start of RESET vector     */
    .intvect                                    :> iROM01       /*  start of interrupt vector                         */
    .itbl  0x200                                :> iROM01       /*  start of interrupt vector                         */
    .rozdata                                    :> .            /*  constant datas in ZDA area                        */
    .robase                    align(4)         :> .            /*  initialize textpointer TP for SDA addressing      */
    .rosdata_start             align(4)         :> .
    .rosdata                   align(4)         :> .            /*  constant datas in SDA16 area                      */
    .rosdata01                 align(4)         :> .            /*  constant datas in SDA23 area                      */
    .rosdata_end               align(4)         :> .
    .rodata                    align(4)         :> .            /*  constant datas in normal area                     */
    .text                      align(4)         :> .            /*  program code area                                 */
    .libtext                   align(4)         :
        {   crt0.o (*(.text))
            lib8bit.a (*(.text))
            libansi.a (*(.text))
            libarch.a (*(.text))
            libdbmem.a (*(.text))
            libece.a (*(.text))
            libecnoe.a (*(.text))
            libedge.a (*(.text))
            libedgnoe.a (*(.text))
            libeece.a (*(.text))
            libeecnoe.a (*(.text))
            libfmalloc.a (*(.text))
            libind_fp.a (*(.text))
            libind_sd.a (*(.text))
            libind_sf.a (*(.text))
            libmath_fp.a (*(.text))
            libmath_sd.a (*(.text))
            libmath_sf.a (*(.text))
            libmath_sngl.a (*(.text))
            libmulti.a (*(.text))
            libnoflt.a (*(.text))
            libsce.a (*(.text))
            libsce_fp.a (*(.text))
            libsce_sd.a (*(.text))
            libsce_sf.a (*(.text))
            libsce_sngl.a (*(.text))
            libscnoe.a (*(.text))
            libscnoe_fp.a (*(.text))
            libscnoe_sd.a (*(.text))
            libscnoe_sf.a (*(.text))
            libscnoe_sngl.a (*(.text))
            libsedge.a (*(.text))
            libsedgnoe.a (*(.text))
            libstartup.a (*(.text))
            libsys.a (*(.text))
            libutf8.a  (*(.text))
        }   > .            /*  library code area                                 */
    .startup                   align(4)         :> .            /*  startup code .text section                        */

    .fixaddr                                    :> .            /*  ghs internal (compiler)                           */
    .fixtype                                    :> .            /*  ghs internal (compiler)                           */
    .secinfo                                    :> .            /*  ghs internal (runtime library)                    */
    .syscall                                    :> .            /*  ghs internal (linker)                             */

    .romdata    ROM(.data)     align(4)         :> .            /*  constant data to initialize variables (copied to RAM at startup)              */
    .romzdata   ROM(.zdata)    align(4)         :> .            /*  constant data to initialize variables in ZDA area (copied to RAM at startup)  */
    .romsdata01 ROM(.sdata01)  align(4)         :> .            /*  constant data to initialize variables in SDA area (copied to RAM at startup)  */
    .romsdata   ROM(.sdata)    align(4)         :> .            /*  constant data to initialize variables in SDA area (copied to RAM at startup)  */
    .romtdata   ROM(.tdata)    align(4)         :> .            /*  constant data to initialize variables in TDA area (copied to RAM at startup)  */

  /* FCL Program code */
  .R_FCL_CODE_ROM       align(4)        :>.      /* FCL code in ROM */
  .R_FCL_CONST          align(4)        :>.      /* FCL constants in ROM */
  .R_FCL_CODE_USRINT    align(4)        :>.      /* Interrupt code located in RAM */
  .R_FCL_CODE_USR       align(4)        :>.      /* User code located in RAM */
  .R_FCL_CODE_RAM       align(4)        :>.      /* FCL code in RAM */
  .R_FCL_CODE_ROMRAM    align(4)        :>.      /* FCL code located in ROM or RAM (depends on the used mode) */
  .R_FCL_CODE_RAM_EX_PROT  pad(0x50) align(4):>.

/* Local RAM */
    .FCL_RESERVED          align(4)             :>LRAM_self     /* FCL copy location */
    .sdabase               align(4)             :> .            /*                                                       */
    .sdata01               align(4)             :> .            /*  initialized data in SDA16 area                       */
    .sbss01                align(4)             :> .            /*  zero initialized data in SDA16 area                  */
    .sdata                 align(4)             :> .            /*  initialized data in SDA23 area                       */
    .sbss                  align(4)             :> .            /*  zero initialized data in SDA23 area                  */

    .data                  align(4)             :> .            /*  initialized data                                     */
    .bss                   align(4)             :> .            /*  zero initialized data                                */

    .stack      align(4)   pad(stack_reserve)   :> .            /*  definition of stack size                             */

  /* Data Section for FCL */
    .R_FCL_DATA                           :>.      /* FCL data */
 
    .heapbase   align(4)                        :> .
    .heap       align(4)
                       pad(addr(LRAM_self)+sizeof(LRAM_self)-addr(.heapbase))
                       NOCLEAR                  :> .            /*  definition of heap size                              */

/* Global RAM */

    .gdata                  align(4)             :> GRAM         /*  initialized data                                     */
    .gbss                   align(4)             :> .            /*  zero initialized data                                */

    .zdata    align(4)                          :> .            /*  initialized data in ZDA area                         */
    .zbss     align(4)                          :> .            /*  zero initialized data in ZDA area                    */

    .ramtext  align(128) MAX_SIZE(0x2000)       :>.             /*  initialized and zero-initialized data in TDA area    */
    .tdata    align(4)   MAX_SIZE(0x0100)       :>.             /*  initialized and zero-initialized data in TDA area    */

//    .dummy_area02_base                          :> .
//    .dummy_area02
//                         pad(addr(GRAM)+sizeof(GRAM)-addr(.dummy_area02_base))
//                                                :> .
//  .stack    align(4)   pad(stack_reserve)     :> .            /*  definition of stack size                             */
//  .heapbase align(4)                          :> .
//  .heap     align(4)
//                     pad(addr(GRAM)+sizeof(GRAM)-addr(.heapbase))
//                     NOCLEAR                  :> .            /*  definition of heap size                              */

/* Peripheral */
    .peripheral02  pad(sizeof(Peripheral02))    :> Peripheral02
    .Peripheral03  pad(sizeof(Peripheral03))    :> Peripheral03

/* Labels for Copying Initialised Data from Flash to RAM */
__DATA_SRAM_ADDR      =  ADDR(.data);
__SDATA01_SRAM_ADDR   =  ADDR(.sdata01);
__SDATA_SRAM_ADDR     =  ADDR(.sdata);
__TDATA_SRAM_ADDR     =  ADDR(.tdata);

__DATA_SIZE           =  SIZEOF(.data);
__SDATA01_SIZE        =  SIZEOF(.sdata01);
__SDATA_SIZE          =  SIZEOF(.sdata);
__TDATA_SIZE          =  SIZEOF(.tdata);

__DATA_ROM_ADDR       =  ADDR(.romdata);
__SDATA01_ROM_ADDR    =  ADDR(.romsdata01);
__SDATA_ROM_ADDR      =  ADDR(.romsdata);
__TDATA_ROM_ADDR      =  ADDR(.romtdata);

}

/*===========================================================================*/
/*      End of File                                                          */
/*===========================================================================*/

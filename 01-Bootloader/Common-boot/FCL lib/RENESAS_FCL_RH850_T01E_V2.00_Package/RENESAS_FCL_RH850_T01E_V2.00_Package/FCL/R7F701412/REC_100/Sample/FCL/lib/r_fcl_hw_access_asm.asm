;--
;-- Library       : Code Flash Access Library for Renesas RH850 devices, based on the RV40 Flash technology
;--
;-- File Name     : $Source: r_fcl_hw_access_asm.asm $
;-- Lib. Version  : $RH850_FCL_LIB_VERSION_T01: V2.00 $
;-- Mod. Revision : $Revision: 1.8 $
;-- Mod. Date     : $Date: 2014/08/25 14:00:30MESZ $
;-- Device(s)     : RV40 Flash based RH850 microcontroller
;-- Description   : FCL hardware interface functions (assembler)
;-------------------------------------------------------------------------------------------------------------


;-------------------------------------------------------------------------------------------------------------
;--Includes   <System Includes> , "Project Includes" 
;-------------------------------------------------------------------------------------------------------------


;-------------------------------------------------------------------------------------------------------------
;--Macro definitions
;-------------------------------------------------------------------------------------------------------------
R_FPSYS_REGADD_BFASEL_U32              .SET 0xffc59008
R_FPSYS_REGADD_BWC_U32                 .SET 0xffbc0700

;-------------------------------------------------------------------------------------------------------------
;--Typedef definitions
;-------------------------------------------------------------------------------------------------------------


;-------------------------------------------------------------------------------------------------------------
;--Exported global variables (to be accessed by other files) 
;-------------------------------------------------------------------------------------------------------------


;-------------------------------------------------------------------------------------------------------------
;--Private global variables and functions 
;-------------------------------------------------------------------------------------------------------------

;-------------------------------------------------------------------------------------------------------------
;--  Dummy function declaration for creating empty sections
;-------------------------------------------------------------------------------------------------------------           
.section    "R_FCL_CODE_USRINT.text", text
.align 4
.L.dummy.R_FCL_CODE_USRINT.text:

.section    "R_FCL_CODE_USR.text", text
.align 4
.L.dummy.R_FCL_CODE_USR.text:

.section    "R_FCL_CODE_RAM.text", text
.align 4
.L.dummy.R_FCL_CODE_RAM.text:

.section    "R_FCL_CODE_ROMRAM.text", text
.align 4
.L.dummy.R_FCL_CODE_ROMRAM.text:

.section    "R_FCL_CODE_RAM_EX_PROT.text", text
.align 4
.L.dummy.R_FCL_CODE_RAM_EX_PROT.text:
.dw     0x0
.dw     0x0
.dw     0x0
.dw     0x0
.dw     0x0
.dw     0x0
.dw     0x0
.dw     0x0
.dw     0x0
.dw     0x0
.dw     0x0
.dw     0x0


;-------------------------------------------------------------------------------------------------------------
;-- Function name: R_FCL_Fct_CodeRomRam_CalcRange
;-------------------------------------------------------------------------------------------------------------
;--
;-- Function calculates start address and size of section R_FCL_CODE_ROMRAM
;--
;-- @param[out]    r6: secStart_u32   Start address of the section R_FCL_CODE_ROMRAM
;--                r7: secSize_u32    Size of the section R_FCL_CODE_ROMRAM
;-- @return        ---
;--
;-------------------------------------------------------------------------------------------------------------
R_FCL_CODE_ROM.text .CSEG TEXT
.PUBLIC _R_FCL_Fct_CodeRomRam_CalcRange

_R_FCL_Fct_CodeRomRam_CalcRange:
    ; /* calculate section start address */
    mov32   #__sR_FCL_CODE_ROMRAM.text, r10
    
    ; /* calculate section size */
    mov32   #__eR_FCL_CODE_ROMRAM.text, r11

    st.w    r10, 0[r6]
    sub     r10, r11
    st.w    r11, 0[r7]

    jmp     [lp]

;-------------------------------------------------------------------------------------------------------------
;-- Function name: R_FCL_Fct_CodeRam_CalcRange
;-------------------------------------------------------------------------------------------------------------
;--
;--  Function calculates start address and size of section R_FCL_CODE_RAM
;--
;--  @param[out]    r6: secStart_u32   Start address of the section R_FCL_CODE_RAM
;--                 r7: secSize_u32    Size of the section R_FCL_CODE_RAM
;--  @return        ---
;--
;-------------------------------------------------------------------------------------------------------------
R_FCL_CODE_ROM.text .CSEG TEXT
.PUBLIC _R_FCL_Fct_CodeRam_CalcRange

_R_FCL_Fct_CodeRam_CalcRange:
    ; /* calculate section start address */
    mov32   #__sR_FCL_CODE_RAM.text, r10

    ; /* calculate section size */
    mov32   #__eR_FCL_CODE_RAM.text, r11

    st.w    r10, 0[r6]
    sub     r10, r11
    st.w    r11, 0[r7]

    jmp     [lp]

;-------------------------------------------------------------------------------------------------------------
;-- Function name: R_FCL_Fct_CodeUsr_CalcRange
;-------------------------------------------------------------------------------------------------------------
;--
;--  Function calculates start address and size of section R_FCL_CODE_USR
;--
;--  @param[out]    r6: secStart_u32   Start address of the section R_FCL_CODE_USR
;--                 r7: secSize_u32    Size of the section R_FCL_CODE_USR
;--  @return        ---
;--
;-------------------------------------------------------------------------------------------------------------
R_FCL_CODE_ROM.text .CSEG TEXT
.PUBLIC _R_FCL_Fct_CodeUsr_CalcRange

_R_FCL_Fct_CodeUsr_CalcRange:
    ; /* calculate section start address */
    mov32   #__sR_FCL_CODE_USR.text, r10
    
    ; /* calculate section size */
    mov32   #__eR_FCL_CODE_USR.text, r11
    
    st.w    r10, 0[r6]
    sub     r10, r11
    st.w    r11, 0[r7]

    jmp     [lp]
    
;-------------------------------------------------------------------------------------------------------------
;-- Function name: R_FCL_Fct_CodeUsrInt_CalcRange
;-------------------------------------------------------------------------------------------------------------
;--
;--  Function calculates start address and size of section R_FCL_CODE_USRINT
;--
;--  @param[out]    r6: secStart_u32   Start address of the section R_FCL_CODE_USRINT
;--                 r7: secSize_u32    Size of the section R_FCL_CODE_USRINT
;--  @return        ---
;--
;-------------------------------------------------------------------------------------------------------------
R_FCL_CODE_ROM.text .CSEG TEXT
.PUBLIC _R_FCL_Fct_CodeUsrInt_CalcRange

_R_FCL_Fct_CodeUsrInt_CalcRange:
    ; /* calculate section start address */
    mov32   #__sR_FCL_CODE_USRINT.text, r10

    ; /* calculate section size */
    mov32   #__eR_FCL_CODE_USRINT.text, r11

    st.w    r10, 0[r6]
    sub     r10, r11
    st.w    r11, 0[r7]

    jmp     [lp]
    
;-------------------------------------------------------------------------------------------------------------
;-- Function name: R_FCL_Fct_CodeExProt_CalcRange
;-------------------------------------------------------------------------------------------------------------
;--
;--  Function calculates start address and size of section R_FCL_CODE_RAM_EX_PROT
;--
;--  @param[out]    r6: secStart_u32   Start address of the section R_FCL_CODE_RAM_EX_PROT
;--                 r7: secSize_u32    Size of the section R_FCL_CODE_RAM_EX_PROT
;--  @return        ---
;--
;-------------------------------------------------------------------------------------------------------------
R_FCL_CODE_ROM.text .CSEG TEXT
.PUBLIC _R_FCL_Fct_CodeExProt_CalcRange

_R_FCL_Fct_CodeExProt_CalcRange:
    ; /* calculate section start address */
    mov32   #__sR_FCL_CODE_RAM_EX_PROT.text, r10
    
    ; /* calculate section size */
    mov32   #__eR_FCL_CODE_RAM_EX_PROT.text, r11

    st.w    r10, 0[r6]
    sub     r10, r11
    st.w    r11, 0[r7]

    jmp     [lp]

;-------------------------------------------------------------------------------------------------------------
;-- Function name: R_FCL_Fct_PrgOffset
;-------------------------------------------------------------------------------------------------------------
;--
;--  Function calculates the offset between function execution address and link address
;--
;--  @param[in,out] ---
;--  @return        Execution offset in Bytes
;--
;-------------------------------------------------------------------------------------------------------------
R_FCL_CODE_ROM.text .CSEG TEXT
.PUBLIC _R_FCL_Fct_PrgOffset

_R_FCL_Fct_PrgOffset:
    jarl    L0, r10
L0: movea   loww(#L0), zero, r6
    movhi   highw(#L0), r6, r6
    sub     r6, r10

    jmp     [lp]

;-------------------------------------------------------------------------------------------------------------
;-- Function name: R_FCL_FCUFct_Switch_BFlash
;-------------------------------------------------------------------------------------------------------------
;--
;-- Function enables or disables BFlash
;--
;-- @param[in]     r6: 1: Activate BFlash
;--                r6: 0: Deactivate BFlash
;-- @return        ---
;--
;-------------------------------------------------------------------------------------------------------------
R_FCL_CODE_RAM.text .CSEG TEXT
.PUBLIC _R_FCL_FCUFct_Switch_BFlash

_R_FCL_FCUFct_Switch_BFlash:
    ; /* Switch BFlash */
    mov     R_FPSYS_REGADD_BFASEL_U32, r10
    st.b    r6, 0[r10]
    ld.b    0[r10], r10             ; /* Dummy read access to a FCU register to wait one APB access cycle */
    synci                           ; /* sync to clear the line buffer */
    
    jmp     [lp]

;-------------------------------------------------------------------------------------------------------------
;-- Function name: R_FCL_FCUFct_Clear_Cache
;-------------------------------------------------------------------------------------------------------------
;--
;-- Function clears the Flash cache
;--
;-- @param[in,out] ---
;-- @return        ---
;--
;-------------------------------------------------------------------------------------------------------------
R_FCL_CODE_RAM.text .CSEG TEXT
.PUBLIC _R_FCL_FCUFct_Clear_Cache

_R_FCL_FCUFct_Clear_Cache:
    ; /* set ICHCLR bit. */
    stsr    24, r12, 4              ; /* system register 24, 4 is ICCTRL */
    ori     0x0100, r12, r12
    ldsr    r12, 24, 4

_POLLING_SYSTEM_REG_:
    stsr    24, r12, 4              ; /* Dummy read to system register to complete the operation */
    andi    0x0100, r12, r12
    cmp     0x0000, r12
    bne     _POLLING_SYSTEM_REG_
    synci                           ; /* sync to clear the line buffer */

    ; /* BWC buffer mounted ? */
    stsr    6, r12, 1               ; /* system register 6, 1 is PID */
    andi    0x00e0, r12, r12        ; /* BWC buffer only on G3K core */
    mov     0x20, r14
    cmp     r12, r14    
    bz      _CLEAR_BWC_BUFFER_

    ; /* set CDBCLR bit */
    stsr    24, r12, 13             ; /* system register 24, 13 is CDBCR */                                                                                                                                                                                                                                
    ori     0x0002, r12, r12
    ldsr    r12, 24, 13
    stsr    24, r12, 13             ; /* Dummy read to system register to complete the operation */
    br      _CLEAR_CACHE_END_

_CLEAR_BWC_BUFFER_:
    ; /* clear BWC buffer */
    mov     R_FPSYS_REGADD_BWC_U32, r12
    mov     0x01, r14
    st.b    r0, 0[r12]              ; /* BWC buffer clear on (BWCREG = 0) */
    st.b    r14, 0[r12]             ; /* BWC buffer clear off (BWCREG = 1) */
    st.b    r0, 0[r12]              ; /* BWC buffer clear on (BWCREG = 0) */
    ld.b    0[r12], r14
    synci

_CLEAR_CACHE_END_:

    jmp     [lp]


;-------------------------------------------------------------------------------------------------------------
;-- Function name: R_FCL_Fct_Copy_Code
;-------------------------------------------------------------------------------------------------------------
;--
;-- Function calculates start address and size of section R_FCL_CODE_RAM.
;--
;-- @param[out]    r6: src_u32        start address
;--                r7: dest_u32       destination address
;--                r8: cnt_u32        size to copy (need to be 4 byte aligned)
;-- @return        last destination address + 4
;--
;-------------------------------------------------------------------------------------------------------------
R_FCL_CODE_RAM.text .CSEG TEXT
.PUBLIC _R_FCL_Fct_Copy_Code

_R_FCL_Fct_Copy_Code:
    cmp     r0, r8
    be      _COPY_LOOP_END_
_COPY_LOOP_:
    ld.w    0[r6], r10
    st.w    r10, 0[r7]
    add     4, r6
    add     4, r7
    add     -4, r8
    bnz     _COPY_LOOP_
_COPY_LOOP_END_:   
    mov     r7, r10

    jmp     [lp]

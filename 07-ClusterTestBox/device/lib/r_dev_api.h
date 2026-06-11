/*
****************************************************************************
PROJECT : Device Driver 
FILE    : $Id: r_dev_api.h 7662 2016-02-15 14:59:20Z matthias.nippert $
============================================================================ 
DESCRIPTION
Driver for CLK macro
============================================================================
                            C O P Y R I G H T                            
============================================================================
                           Copyright (c) 2015
                                  by 
                       Renesas Electronics (Europe) GmbH. 
                           Arcadiastrasse 10
                          D-40472 Duesseldorf
                               Germany
                          All rights reserved.
============================================================================
Purpose: only for testing

DISCLAIMER                                                                   
This software is supplied by Renesas Electronics Corporation and is only     
intended for use with Renesas products. No other uses are authorized. This   
software is owned by Renesas Electronics Corporation and is protected under  
all applicable laws, including copyright laws.                               
THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING  
THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT      
LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE   
AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.          
TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS       
ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE  
FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR   
ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE  
BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.                             
Renesas reserves the right, without notice, to make changes to this software 
and to discontinue the availability of this software. By using this software,
you agree to the additional terms and conditions found by accessing the      
following link:                                                              
http://www.renesas.com/disclaimer *                                          
Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.     

****************************************************************************
*/

#ifndef R_DEV_API_H_
#define R_DEV_API_H_

/* Device specific includes (typedefs) */
#include "r_dev_clk_types.h"
#include "r_dev_pin_types.h"
#include "r_dev_int_types.h"
#include "r_dev_guard_types.h"
#include "r_dev_xorchk_types.h"


/* if you don't wish to have debug output for a particular driver 
  (but still want it globally enabled) then undef the R_DBG_PRINT_DEV_REGS 
  in the corresponding file (before the inclusion of r_dev_api.h)  

** Example from r_sys_gpio.c ** 
 #ifndef R_DBG_SHOW_PIN_REGS
   #undef  R_DBG_PRINT_DEV_REGS
 #endif   
 #include "r_dev_api.h"
*******************************
*/

#if defined (R_DBG_PRINT_DEV_REGS) || defined (R_DBG_PRINT_MSG)
   #include <stdio.h> 
   #include "r_dbg_api.h"
#else
    #define R_DBG_PRINT(lvl, txt) 
#endif
         
#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: Device API

  Api for device functions.
*/

/*******************************************************************************
  Section: Global Constants
*/

/*******************************************************************************
 Constant: R_BAD_ADDRESS 
   
 Define an "illegal" address value. This can be used for validity checks.   
*/
#define R_BAD_ADDRESS       (0xFFFFFFFFu)

#if 0
#define P16                      0xff610400ul
#define PSR16                    0xff610404ul
#define PNOT16                   0xff610408ul
#define PPR16                    0xff61040cul
#define PM16                     0xff610410ul
#define PMC16                    0xff610414ul
#define PFC16                    0xff610418ul
#define PFCE16                   0xff61041cul
#define PMSR16                   0xff610420ul
#define PMCSR16                  0xff610424ul
#define PINV16                   0xff610430ul
#endif
/*******************************************************************************
  Macro: R_DEV_WRITE_REG

  Write register.
  SIZE allowed values are 8, 16, 32
*/

#ifdef R_DBG_PRINT_DEV_REGS
#define R_DEV_WRITE_REG(SIZE, ADDR, VALUE)  {R_DBG_PrintRegCmd((SIZE), (ADDR), (VALUE));  *((volatile uint##SIZE##_t*)(ADDR)) = (VALUE);}
#else
#define R_DEV_WRITE_REG(SIZE, ADDR, VALUE)  {(*((volatile uint##SIZE##_t*)(ADDR)) = (VALUE)); }
#endif



/*******************************************************************************
  Macro: R_DEV_WRITE_PROT_REG

  Write value to a protected register.
  
  Protected register are always 32bit wide 
*/
#if defined (__ghs__)
#pragma ghs startnomisra
#endif /* __ghs */

#ifdef R_DBG_PRINT_DEV_REGS
#define R_DEV_WRITE_PROT_REG(PROT_ADDR, ADDR, VALUE) {\
    uint32_t prot_stat = 0; \
    uint32_t new_val   = 0; \
  do { \
  *((volatile uint32_t*)(PROT_ADDR)) = (0x000000a5);\
  *((volatile uint32_t*)(ADDR))      = (VALUE);\
  *((volatile uint32_t*)(ADDR))      = (uint32_t)(~(uint32_t)(VALUE));\
  *((volatile uint32_t*)(ADDR))      = (VALUE);\
  prot_stat = *((volatile uint32_t*)((PROT_ADDR) + 4u)); \
  new_val   = *((volatile uint32_t*)(ADDR)); \
} while ((new_val != (VALUE)) &&  (prot_stat != 0));\
/*R_DBG_PrintRegCmd(32, (ADDR), (VALUE));*/}
#else
#define R_DEV_WRITE_PROT_REG(PROT_ADDR, ADDR, VALUE) {\
    uint32_t prot_stat = 0; \
    uint32_t new_val   = 0; \
    do { \
  *((volatile uint32_t*)(PROT_ADDR)) = (0x000000a5);\
  *((volatile uint32_t*)(ADDR))      = (VALUE);\
  *((volatile uint32_t*)(ADDR))      = (uint32_t)(~(uint32_t)(VALUE));\
  *((volatile uint32_t*)(ADDR))      = (VALUE);\
  prot_stat = *((volatile uint32_t*)((PROT_ADDR) + 4u)); \
  new_val   = *((volatile uint32_t*)(ADDR)); \
} while ((new_val != (VALUE)) &&  (prot_stat != 0));}
#endif
#if defined (__ghs__)
#pragma ghs endnomisra
#endif


/*******************************************************************************
  Macro: R_DEV_READ_REG

  Read register.
    SIZE allowed values are 8, 16, 32

*/

#ifdef R_DBG_PRINT_DEV_REGS
#define R_DEV_READ_REG(SIZE, ADDR)   (R_DBG_PrintRegRead##SIZE##((ADDR)))
#else
#define R_DEV_READ_REG(SIZE, ADDR)   ((*((volatile uint##SIZE##_t*)((ADDR)))))
#endif


/*******************************************************************************
  Macro: R_DEV_ERROR 

  Display an additional error message (in case od activeated Debug output)
  And call the device error handler  
  
 */
#define R_DEV_ERROR(Unit, DevErr, Txt)  {\
        R_DISPLAY_ERROR(Txt); R_DEV_ErrorHandler((Unit), (uint32_t)(DevErr));} 


/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Enum: r_dev_Error_t

  Device error codes.

  Values:
   R_DEV_ERR_OK              - No error
   R_DEV_ERR_NG              - Unspecified error   
   R_DEV_ERR_UNIT            - Wrong instance number
   R_DEV_ERR_INTR            - Wrong interrupt number
   R_DEV_ERR_IDX             - Wrong index
   R_DEV_ERR_REG             - bad register (address) 
   R_DEV_ERR_RANGE           - parameter limit violation  
   R_DEV_ERR_LAST            - Delimiter 
*/

typedef enum 
{
    R_DEV_ERR_OK       = 0x0000u,
    R_DEV_ERR_NG       = 0x0001u,
    R_DEV_ERR_UNIT     = 0x0002u,
    R_DEV_ERR_INTR     = 0x0004u,
    R_DEV_ERR_IDX      = 0x0008u,
    R_DEV_ERR_REG      = 0x0010u,
    R_DEV_ERR_RANGE    = 0x0020u,
    R_DEV_ERR_PARAM    = 0x0040u,
    R_DEV_ERR_LAST     
} r_dev_Error_t;



/*******************************************************************************
  Enum: r_dev_Device_t

  Device list 

  Values:
    R_DEV_R7F7014OFFS         - Start Offset of D1LM family
    R_DEV_R7F701400           - D1L1_QFP100 (bondout variant)
    R_DEV_R7F701401           - D1L1
    R_DEV_R7F701402           - D1L2
    R_DEV_R7F701403           - D1L2H
    R_DEV_R7F701404           - D1M1_3.75M
    R_DEV_R7F701405           - D1M1_5M
    R_DEV_R7F701406           - D1M1H_3.75M 
    R_DEV_R7F701407           - D1M1H_5M    
    R_DEV_R7F701408           - D1M2_3.75M 
    R_DEV_R7F701410           - D1M2_5M    
    R_DEV_R7F701411           - D1M2H_3.75M
    R_DEV_R7F701412           - D1M2H_5M   
    R_DEV_UNKNOWN_DEVICE      - Unknown device 

*/

typedef enum 
{
    R_DEV_UNKNOWN_DEVICE  ,
    R_DEV_R7F7014OFFS = 1400,
    R_DEV_R7F701400 = R_DEV_R7F7014OFFS,
    R_DEV_R7F701401       ,
    R_DEV_R7F701402       ,
    R_DEV_R7F701403       ,
    R_DEV_R7F701404       ,
    R_DEV_R7F701405       ,
    R_DEV_R7F701406       ,
    R_DEV_R7F701407       ,
    R_DEV_R7F701408       ,
    R_DEV_R7F701410 = 1410,
    R_DEV_R7F701411       ,
    R_DEV_R7F701412       ,
    R_DEV_LAST_DX_DEV
} r_dev_Device_t;


/*******************************************************************************
  Enum: r_dev_Device_Revision_t

  Device Revision number

  Values:
    R_DEV_REVISION_1V0              - V1.0
    R_DEV_REVISION_1V1              - V1.1
    R_DEV_REVISION_1V2              - V1.2
    R_DEV_REVISION_2V0              - V2.0
    R_DEV_REVISION_2V1              - V2.1
    R_DEV_REVISION_2V2              - V2.2
    R_DEV_REVISION_3V0              - V3.0
    R_DEV_UNKNOWN_DEVICE_REVISION   - Unknown device revision

    Devise revision (version)
    is given in the format 0xAABB,
    where AA is the mayor revision number
    and BB is the minor revision number.
      e.g. "0x0102" is the device revision "V1.2"
*/

typedef enum 
{
    R_DEV_UNKNOWN_DEVICE_REVISION = 0x000,
    R_DEV_REVISION_1V0 = 0x100,
    R_DEV_REVISION_1V1 = 0x101,
    R_DEV_REVISION_1V2 = 0x102, 
    R_DEV_REVISION_2V0 = 0x200,
    R_DEV_REVISION_2V1 = 0x201,
    R_DEV_REVISION_2V2 = 0x202,
    R_DEV_REVISION_3V0 = 0x300,
    R_DEV_LAST_DEV_REV
} r_dev_Device_Revision_t;


/*******************************************************************************
  Enum: r_dev_SysCall_t

  Commands that can be executed with elevated rights.

  Values:
    R_DEV_SYSCALL_SUPERVISOR_MODE   - Switch to Supervisor Mode
    R_DEV_SYSCALL_USER_MODE         - Switch to User Mode
    R_DEV_SYSCALL_CONFIGURE_PBG     - Configure PB Guard
    R_DEV_SYSCALL_LAST              - Unknown device 

*/

typedef enum 
{
    R_DEV_SYSCALL_SU,
    R_DEV_SYSCALL_SUPERVISOR_MODE = R_DEV_SYSCALL_SU,
    R_DEV_SYSCALL_USER_MODE,
    R_DEV_SYSCALL_CONFIGURE_PBG,
    R_DEV_SYSCALL_LAST  
} r_dev_SysCall_t;


/*******************************************************************************
  Section: Global API Functions
  
*/


/*******************************************************************************
                        Error handling 
 */

/*******************************************************************************
  Function: R_DEV_ErrorHandler
  
  Central device error function. 
  
  When a driver detects any error, it might call this function.
  In case there is a registered error callback user function, 
  it  will be called. If a user handler is missing an endless loop is entered. 
     
  Parameters:
  Unit       - Instance number
  Error      - Error code, see <r_dev_Error_t> for some examples 
  
  Returns:
  void
*/

void R_DEV_ErrorHandler(uint32_t Unit, uint32_t Error);  

/*******************************************************************************
  Function: R_DEV_SetErrorCallback

  Set a user function as an the error handler.

  Parameters:
  *ErrorHandler - a pointer to a user function (callback)
                  with two paramerters (unit and error number)

  Returns:
  void

*/

void R_DEV_SetErrorCallback(void(*ErrorCallback )(uint32_t Unit, uint32_t Error));


/*******************************************************************************
                        Interrupt handling 
 */

/******************************************************************************* 
  Function: R_DEV_Prepare
 
  Prepare Device  
  - Clear all device settings   
  
  Returns: 
  void
   
*/ 
void R_DEV_Prepare (void); 

/*******************************************************************************
  Function: void R_DEV_IntSetPrio

 Set the priority for the given interrupt.  

  Parameters:
  int32_t        - interrupt number, see  <r_dev_IntSel_t>

*/

void R_DEV_IntSetPrio(r_dev_IntSel_t Int, uint8_t Prio); 

/*******************************************************************************
  Function: R_DEV_IntEnable

  Enable or disable the given interrupt.  

  Parameters:
  int32_t        - interrupt number, see  <r_dev_IntSel_t>
  Enable     - enable(1) or disable(0) 

*/

void R_DEV_IntEnable(r_dev_IntSel_t Int, uint8_t Enable);

/*******************************************************************************
  Function: R_DEV_IntClearFlag

  Clear the interrupt pending flag for the given interrupt.  

  Parameters:
  int32_t        - interrupt number, see  <r_dev_IntSel_t>

  Returns:
  void
*/

void R_DEV_IntClearFlag(r_dev_IntSel_t Int); 

/*******************************************************************************
  Function: R_DEV_IntSetFlag

  Set the interrupt pending flag for the given interrupt.  

  Parameters:
  int32_t        - interrupt number, see  <r_dev_IntSel_t>

  Returns:
  void
*/

void R_DEV_IntSetFlag(r_dev_IntSel_t Int); 

/*******************************************************************************
  Function: R_DEV_IntGetFlag

  Get the interrupt pending flag value for the given interrupt.  

  Parameters:
  int32_t        - interrupt number, see  <r_dev_IntSel_t>

  Returns:
    == 1           - the pending flag is set
    != 1           - the pending flag is not set
*/

uint8_t R_DEV_IntGetFlag(r_dev_IntSel_t Int); 


/*******************************************************************************
  Function: R_DEV_IntGetMask

  Get the interrupt mask status for the given interrupt.  

  Parameters:
  int32_t        - interrupt number, see  <r_dev_IntSel_t>

  Returns:
  == 1            - Interrupt masked (disabled) 
  != 1            - Interrupt enabled 
*/

uint8_t R_DEV_IntGetMask(r_dev_IntSel_t Int); 

/*******************************************************************************
  Function: R_DEV_IntSetMode

  Set the interrupt handling mode for the given interrupt 
  
  Parameters:
  Int          - interrupt number, see  <r_dev_IntSel_t>
  Mode         - ISR handling mode, 
               1: Table look up mode, similar to previous devices 
               0: Priority group mode  
  
  Returns:
  void
*/
void R_DEV_IntSetMode(r_dev_IntSel_t Int, uint8_t Mode); 


/*******************************************************************************
  Function: R_DEV_FeNmiIsr
  
  ISR for handling the FE-NMI interrupt(s) 
  Parameters:
  void
  
  Returns:
  void 
  
    
*/
void R_DEV_FeNmiIsr(void); 

/*******************************************************************************
                        Pin handling 
*/

/*******************************************************************************
  Function: R_DEV_PinConfig

  Parameters:
  PinCfg   - pointer to pin config structure, see <r_dev_PinConfig_t>

  Returns:
   = 1   -     Success  
  != 1  -     Fail   

*/

uint8_t R_DEV_PinConfig(const r_dev_PinConfig_t * PinCfg); 


/*******************************************************************************
  Function: R_DEV_PinInit

  Performs PinConfig for each valid line of te config array 
  
  Treats the given pointer as an array and scans until the 
  the predefined delimiter value is reached 
     
  Parameters:
  PinCfg   - pointer to pin config structure, see <r_dev_PinConfig_t>

  Returns:
  void
*/
void R_DEV_PinInit(const r_dev_PinConfig_t * PinCfg); 

/*******************************************************************************
  Function: R_DEV_PinOutStateGet

  Set the global state of the device output (disabled or enabled)  
  
      
  Parameters:
  State        -    The global output state   1: Output enabled 
                                              0: Output disabled 
 
  Returns:
  void
*/
void R_DEV_PinOutStateSet(uint8_t State); 

/*******************************************************************************
  Function: R_DEV_PinOutStateGet

  Get the global state of the device output (disabled or enabled)  
  
      
  Parameters:
  void

  Returns:
  1: Output enabled 
  0: Output disabled 
  
*/
uint8_t R_DEV_PinOutStateGet(void); 


/*******************************************************************************
  Function: R_DEV_PinGetRegAddr

  Return the address of the given register for the given port     

  Parameters:
  Reg   - register name, see <r_dev_PinRegs_t>
  Port  - Port number

  Returns:
  Address of the register 
*/
uint32_t R_DEV_PinGetRegAddr(r_dev_PinRegs_t Reg, uint32_t Port);


/*******************************************************************************
  Function: R_DEV_PinGetRegAddrExt

  Return the address of the given register for the given port     

  Parameters:
  Reg   - register name, see <r_dev_PinRegs_t>
  Port  - Port number 
  Pin   - Pin number (only requrired for R_DEV_PIN_REG_PCR register address)

  Returns:
  Address of the register 
*/
uint32_t R_DEV_PinGetRegAddrExt(r_dev_PinRegs_t Reg, uint32_t Port, uint32_t Pin);


/*******************************************************************************
                        Xor check unit handling
*/

/*******************************************************************************
  Type: r_dev_xorchk_Parameter_t

  XORCHK macro configuration data

  Members:
    SampleRejectAbility         - number of samples until signal is requected (2 .. 5)
    SampleClockPrescaler        - prescaler for sampling (0 .. 7) -> equals 2^SampleClockPrescaler
*/     

typedef struct {
    uint32_t               SampleRejectAbility;
    uint32_t               SampleClockPrescaler;
} r_dev_xorchk_Parameter_t;

/*******************************************************************************
  Function: R_DEV_Xorchk_Config

  Xor check unit configuration function.

  Parameters:
  Func          - Xor check function   see <r_dev_XorchkFunc_t>
  Config        - configuration parameters
 
  Returns:
  none
*/
void R_DEV_Xorchk_Config(r_dev_XorchkFunc_t Func, r_dev_xorchk_Parameter_t *Config);

/*******************************************************************************
  Function: R_DEV_Xorchk_FunctionCheckEnable

  Enable or disables the signal check by Xor check unit.

  Parameters:
  Func          - Xor check function   see <r_dev_XorchkFunc_t>
  Enable        - R_TRUE to enable / R_FALSE to disable function checking
 
  Returns:
  none
*/
void R_DEV_Xorchk_FunctionCheckEnable(r_dev_XorchkFunc_t Func, uint8_t Enable);


/*******************************************************************************
                        Clock handling 
*/

/*******************************************************************************
  Function: R_DEV_ClkPrepare
  
  Clock driver preparation  

  Clears all internal module configuration
  
  Returns:
  none

*/
void R_DEV_ClkPrepare(void); 

/*******************************************************************************
  Function: R_DEV_ClkGenInitStore
  
  Clock genrators init function is called in the low init phase, 
  This function just is meant to store the relevant information in the device 
  clock structures. The struectures are overwritten (cleared) after the low init, 
  so this function is required to (re)store the settings  

  Parameters:
  ClkGenCfg - pointer to the Clock generator configuration, 
              see <r_dev_ClkGenConfig_t>
  
  Returns:
  none

*/
void R_DEV_ClkGenInitStore(r_dev_ClkGenConfig_t * ClkGenCfg); 

/*******************************************************************************
  Function: R_DEV_ClkGenInit
  
  Clock genrators init function  

  Parameters:
  ClkGenCfg - pointer to the Clock generator configuration, 
              see <r_dev_ClkGenConfig_t>
  ClkSelCfg - pointer to clock domains configuration,  
              see <r_dev_ClkSelConfig_t>
  
  Returns:
  none

*/
void R_DEV_ClkGenInit(r_dev_ClkGenConfig_t * ClkGenCfg, const r_dev_ClkSelConfig_t * ClkSelCfg) ; 

/*******************************************************************************
  Function: R_DEV_ClkIdSet
 
  Configure a clock domain.
  
  Set the appropriate clock selector. 
   
  Parameters:
  ClkSelCfg   - Clock selector register settings, see <r_dev_ClkSelConfig_t>
  
  Returns:
  == R_ERR_OK   -    Success  , see <r_Error_t>
  != R_ERR_OK  -     Fail     , see <r_Error_t>

*/

r_Error_t R_DEV_ClkIdSet(const r_dev_ClkSelConfig_t * ClkSelCfg);


/*******************************************************************************
  Function: R_DEV_FoutDivSet

  Set the divider for FOUT 
  
  Parameters:
  Div      - divider value (0 - disabled or 1,..,63)

  Returns:
  == R_ERR_OK   -     success 
  != R_ERR_OK   -     fail  


*/

r_Error_t R_DEV_FoutDivSet(uint16_t Div);

/*******************************************************************************
  Function: R_DEV_ClkSelection

  Clock genrators init function  
  Parses a configutaton table until the delimiter is found 
    
  Parameters:
   ClkSelCfg   - pointer Clock selector register settings, see <r_dev_ClkSelConfig_t>
 
  Returns:
  none
*/
void R_DEV_ClkSelection(const r_dev_ClkSelConfig_t * ClkSelCfg);

/*******************************************************************************
  Function: R_DEV_ClkGenActive

  Returns the status of a given clock source.

  Parameters:
  ClkGen      - Clock source, see <r_dev_ClkGen_t>

  Returns:
  == R_ERR_OK   -     The given generator is active 
  != R_ERR_OK   -     The given generator is disabled  

*/
 uint8_t R_DEV_ClkGenActive(r_dev_ClkGen_t ClkGen);

/*******************************************************************************
  Function: R_DEV_ClkGenStop

  Disable/Stop the given clock source.

  Parameters:
  ClkGen      - Clock source, see <r_dev_ClkGen_t>

  Returns:
   == R_ERR_OK   -    Success  , see <r_Error_t>
  !=  R_ERR_OK  -     Fail     , see <r_Error_t>

  
*/
r_Error_t R_DEV_ClkGenStop(r_dev_ClkGen_t ClkGen);

/*******************************************************************************
  Function: R_DEV_ClkGenStart

  Start the given clock source.

  Parameters:
  ClkGen      - Clock source, see <r_dev_ClkGen_t>

  Returns:
  == R_ERR_OK  -    Success  , see <r_Error_t>
  != R_ERR_OK  -    Fail     , see <r_Error_t>

*/
r_Error_t R_DEV_ClkGenStart(r_dev_ClkGen_t ClkGen);

/******************************************************************************* 
  Function: R_DEV_SetGenStopMask 

  Set the deep stop mask for the gicen clock generator.

  Parameters:
  ClkGen      - Clock source, see <r_dev_ClkGen_t>
  Val         - 1/0 Set or clear stop mask 

  Returns:
  == R_ERR_OK  -    Success  , see <r_Error_t>
  != R_ERR_OK  -    Fail     , see <r_Error_t>

*/ 
r_Error_t R_DEV_SetGenStopMask(r_dev_ClkGen_t ClkGen, uint8_t Val);  

/*******************************************************************************
  Function: R_DEV_ClkSelected

  Returns the numeric Id of the assigned clock input 

  Parameters:
  ClkSel      - Clock selector, see <r_dev_ClkSel_t>

  Returns:
  - The (numeric) Id of the active clock set for the given selector
  - Null in case no clock is selected or the selection is not active
    (due to a stopped clck source or invalid assignment)    
*/
 uint8_t R_DEV_ClkSelected(r_dev_ClkSel_t ClkSel);

/*******************************************************************************
  Function: R_DEV_ClkFrequencyHz

  Returns the Frequency in Hz of the assigned clock input.

  Parameters:
  ClkSel      - Clock source, see <r_dev_ClkSel_t>

  Returns:
  - The actually selected frequency in Hz
  - 0 in case no clock is selected or the selection is not active
    (due to a stopped clck source or invalid assignment)    
*/
uint32_t R_DEV_ClkFrequencyHz(r_dev_ClkSel_t ClkSel);


/*******************************************************************************
  Function: R_DEV_GetClkConfig

  Returns the Frequency in Hz of the assigned clock input together with 
  actual clock configuration.
  
  The passed structure (ClkData) needs the Cks to be set, 
  the other values are then filled in by this function.  

  Parameters:
  ClkData      - Pointer to the config structure, see <r_dev_ClkSelConfig_t>
               -  
  Returns:
  - The actually selected frequency in Hz
  - 0 in case no clock is selected or the selection is not active
    (due to a stopped clck source or invalid assignment)    
*/
uint32_t R_DEV_GetClkConfig(r_dev_ClkSelConfig_t *ClkData); 

/*******************************************************************************
  Function: R_DEV_BusFrequencyHz

  Returns the PCLK (aka APB Clock) Frequency in Hz 
  
  Parameters:
  void

  Returns:
  - Frequency in Hz

*/
uint32_t R_DEV_BusFrequencyHz(r_dev_Bus_t Bus);


/*******************************************************************************
  Function: R_DEV_EnableFlashCache
  
  Enable or disable the code cache  
  
  Parameters:
  Enable     - 0: Disable cache, !=0: enable cache 
  
  Returns:
  == R_ERR_OK  -    Success   , see <r_Error_t>
  != R_ERR_OK  -     Fail     , see <r_Error_t>
   
*/

r_Error_t R_DEV_EnableFlashCache(uint8_t Enable);


/*******************************************************************************
  Function: R_DEV_Nop
  
  A function doing exactly one nop (in assembler). 
  
  It's implemented instead of asm("nop") because some MISRA gurus think it's a 
  good idea to have a extra function declaration and an extra assembler file 
  instead of one simple instruction (Rule 2.1) 
  
  Parameters:
  void
  
  Returns:
  void   

*/

void R_DEV_Nop(void); 

/***********************************************************
  Function: _R_DEV_SyncP

  CPU pipeline sync (assembler routine)

  Parameters: none
  
*/
void R_DEV_SyncP(void);  

/***********************************************************
  Function: _R_DEV_SyncM

  CPU memory sync (assembler routine)

  Parameters: none
  
*/
void R_DEV_SyncM(void);


/*******************************************************************************
  Function: R_DEV_GetDev
  
  Function returns the device name. The regiters PRDNAME3 and PRDNAME2 are used 
  to "sortet out" the correct device  
  
  Parameters:
  void
  
  Returns:
  device name, see <r_dev_Device_t>   

*/

r_dev_Device_t R_DEV_GetDev(void); 

/*******************************************************************************
  Function: R_DEV_GetVersion 
  
  Function returns the device version. 
  
  Parameters:
  void
  
  Returns:
  device name, see <r_dev_Device_t>   

*/

uint32_t R_DEV_GetVersion(void); 


/*******************************************************************************
  Function: R_DEV_GetDevName
  
  Function returns the device name as a human readable string. 
  
  Parameters:
  void
  
  Returns:
  device name string, see <r_dev_Device_t>   

*/

const char * R_DEV_GetDevName(void); 

/*******************************************************************************
                        Syscalls 
*/

/*******************************************************************************
  Function: R_DEV_SysCall

  Switch to supervisor mode and internally call 'R_DEV_SysCallElevated'. 

  This function is usually written in assembly language.

  Parametes:
  Cmd     - System call command
  Par1    - Parameter 1
  Par2    - Parameter 2
  Par3    - Parameter 3

  Returns:
  Return value depends on command
*/

uint32_t R_DEV_SysCall(r_dev_SysCall_t Cmd, void *Par1, void *Par2, void *Par3);
uint32_t R_DEV_SysCallElevated(r_dev_SysCall_t Cmd, void *Par1, void *Par2, void *Par3);


#ifdef __cplusplus
}
#endif

#endif /* R_DEV_API_H_  */

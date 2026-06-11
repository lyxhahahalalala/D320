/*
****************************************************************************
PROJECT : device driver for V-Lib
FILE    : $Id: r_dev_pin.c 3211 2014-07-04 14:11:53Z michael.golczewski $
============================================================================
DESCRIPTION
r7f701412 interrupt service routines
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


/*******************************************************************************
  Title: upd703670 port-pin init  functions

  
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_dev_api.h"
#include "r_config_pin.h"

/* This macro defnition exist for debug purposes 
   because of its concept, MISRA always complains ...*/
#if defined (__ghs__)
#pragma ghs startnomisra
#endif /* __ghs */

#if defined (R_DBG_PRINT_DEV_REGS) 
   /* When using this, do not forget double (( ))*/
   #define LOC_DBG_PRINTF(TXT) printf TXT
#else
   #define LOC_DBG_PRINTF(TXT)
#endif

#if defined (__ghs__)
#pragma ghs endnomisra
#endif /* __ghs */

/*******************************************************************************
  Section: Local Constants 
*/

/*******************************************************************************
  PORTC Register Offsets
*/

/************************************************************************************************

Port registers
  <see r_dev_PinRegs_t> in the file "r_dev_pin_types.h".

*/

/*******************************************************************************
Array : R_GPIO_Offset_Table

The R_GPIO_Offset_Table array specifies  Register offset in a Macro
*/

static const uint32_t loc_RegOffset[R_DEV_PIN_REG_LAST] = {
    /*R_DEV_PIN_REG_PCR  */  0x2000u,
    
    /*R_DEV_PIN_REG_P    */  0x0000u,
    /*R_DEV_PIN_REG_PSR  */  0x0004u,
    /*R_DEV_PIN_REG_PPR  */  0x000Cu,
    /*R_DEV_PIN_REG_PM   */  0x0010u,
    /*R_DEV_PIN_REG_PMC  */  0x0014u,
    /*R_DEV_PIN_REG_PFC  */  0x0018u,
    /*R_DEV_PIN_REG_PFCE */  0x001Cu,
    /*R_DEV_PIN_REG_PNOT */  0x0008u,
    /*R_DEV_PIN_REG_PMSR */  0x0020u,
    /*R_DEV_PIN_REG_PMCSR*/  0x0024u,
    
    /*  OS  */
    /*R_DEV_PIN_REG_PIBC */  (R_DEV_PIN_BASEO_OFFSET + 0x4000u),
    /*R_DEV_PIN_REG_PBDC */  (R_DEV_PIN_BASEO_OFFSET + 0x4004u),
    /*R_DEV_PIN_REG_PIPC */  (R_DEV_PIN_BASEO_OFFSET + 0x4008u),
    /*R_DEV_PIN_REG_PU   */  (R_DEV_PIN_BASEO_OFFSET + 0x400Cu),
    /*R_DEV_PIN_REG_PD   */  (R_DEV_PIN_BASEO_OFFSET + 0x4010u),
    /*R_DEV_PIN_REG_PODC */  (R_DEV_PIN_BASEO_OFFSET + 0x4014u),
    /*R_DEV_PIN_REG_PDSC */  (R_DEV_PIN_BASEO_OFFSET + 0x4018u),
    /*R_DEV_PIN_REG_PIS  */  (R_DEV_PIN_BASEO_OFFSET + 0x401Cu),
    /*R_DEV_PIN_REG_PISA */  (R_DEV_PIN_BASEO_OFFSET + 0x4024u),
    /*R_DEV_PIN_REG_PPCMD*/  (R_DEV_PIN_BASEO_OFFSET + 0x402Cu),
    /*R_DEV_PIN_REG_PPROTS*/ (R_DEV_PIN_BASEO_OFFSET + 0x4034u),
};


static const r_dev_PortInputCapability_t loc_PortCapability[] = 
/*  Port    InputType11,        InputType10,        InputType01,        InputType00 */
{
    {0u,    R_DEV_PIN_TTL,      R_DEV_PIN_TTL,      R_DEV_PIN_SCHMITT4, R_DEV_PIN_SCHMITT1},
    {1u,    R_DEV_PIN_CMOS1,    R_DEV_PIN_CMOS1,    R_DEV_PIN_SCHMITT4, R_DEV_PIN_SCHMITT1},
    {2u,    R_DEV_PIN_CMOS1,    R_DEV_PIN_CMOS1,    R_DEV_PIN_SCHMITT4, R_DEV_PIN_SCHMITT1},
    {3u,    R_DEV_PIN_CMOS1,    R_DEV_PIN_CMOS1,    R_DEV_PIN_SCHMITT4, R_DEV_PIN_SCHMITT1},
    {16u,   R_DEV_PIN_CMOS1,    R_DEV_PIN_CMOS1,    R_DEV_PIN_SCHMITT4, R_DEV_PIN_SCHMITT1},
    {17u,   R_DEV_PIN_CMOS1,    R_DEV_PIN_CMOS1,    R_DEV_PIN_SCHMITT4, R_DEV_PIN_SCHMITT1},
    {21u,   R_DEV_PIN_TTL_MLB,  R_DEV_PIN_TTL_MLB,  R_DEV_PIN_CMOS1,    R_DEV_PIN_CMOS1},
    {42u,   R_DEV_PIN_CMOS1,    R_DEV_PIN_CMOS1,    R_DEV_PIN_SCHMITT4, R_DEV_PIN_SCHMITT1},
    {43u,   R_DEV_PIN_CMOS1,    R_DEV_PIN_CMOS1,    R_DEV_PIN_SCHMITT4, R_DEV_PIN_SCHMITT1},
    {46u,   R_DEV_PIN_CMOS1,    R_DEV_PIN_CMOS1,    R_DEV_PIN_SCHMITT4, R_DEV_PIN_SCHMITT1},
    {47u,   R_DEV_PIN_CMOS1,    R_DEV_PIN_CMOS1,    R_DEV_PIN_SCHMITT4, R_DEV_PIN_SCHMITT1},
    
    /* delimiter - do not remove */
    {0xFFu, R_DEV_PIN_NONE,     R_DEV_PIN_NONE,     R_DEV_PIN_NONE,     R_DEV_PIN_NONE},
};

/*******************************************************************************
  Section: Local variables 
*/
static uint8_t loc_PinEnable = 1; 


/*******************************************************************************
  Section: Global functions
*/
/*******************************************************************************
  Function: R_DEV_PinOutStateGet

  See: r_dev_api.h for details    

 */
void R_DEV_PinOutStateSet(uint8_t State)
{
    loc_PinEnable = State; 
}
/*******************************************************************************
  Function: R_DEV_PinOutStateGet

  See: r_dev_api.h for details    

*/
uint8_t R_DEV_PinOutStateGet(void) 
{
    return loc_PinEnable;
}
 

/*******************************************************************************
  Function: R_DEV_PinConfig
  
  See: r_dev_api.h for details    

*/

uint8_t R_DEV_PinConfig(const r_dev_PinConfig_t * PinCfg)
{
	uint8_t   ret_val      = 1u; 



    uint32_t  addr         = 0u; 
    uint32_t  prot_addr    = 0u; 
    uint32_t  reg_val      = 0u;
    uint16_t  pin_position = 1u;
    uint8_t   port_position = 0u;
    uint8_t   portHasPisaRegister = 0u;
    uint8_t   portHasPisRegister  = 0u;
    uint8_t   PisaRegBitValue     = 0u;
    uint8_t   PisRegBitValue      = 0u;

	// R_DBG_PRINT(R_DBG_MSG_DET, "--- Pin Config Start ---"); 
    if (0 == loc_PinEnable)
    {
		// R_DBG_PRINT(R_DBG_MSG_DET, "--- Pin output disabled - exiting  ---"); 
        return ret_val; 
    }    
    pin_position = (uint16_t)(pin_position << PinCfg->Pin);
    /* optional check if already configured */
    /* read PMC, if 1 -> already set to other alternative mode, abort */
    /* read PM, if not 1 -> already set to output, abort (this is in case of pure GPIO) */
    
    /* Input or output  */
    addr    = R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PM, PinCfg->Port);
    reg_val = R_DEV_READ_REG(16, addr);
    if (R_DEV_PIN_OUT == PinCfg->Dir)
    {
        reg_val &= (uint16_t)(~pin_position); 
        // LOC_DBG_PRINTF(("/* Set pin %d of port %d as output for ", PinCfg->Pin, PinCfg->Port)); 

    }
    else /* Set direction to input also in case of direct control */
    {
        reg_val |= pin_position;
        // LOC_DBG_PRINTF(("/* Set pin %d of port %d as input for ", PinCfg->Pin, PinCfg->Port)); 
    }
    // LOC_DBG_PRINTF(("alternative function %d */\n", PinCfg->Func )); 
    R_DEV_WRITE_REG(16, addr, reg_val);
    
    /* direct control */
    addr    = R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PIPC, PinCfg->Port);
    reg_val = R_DEV_READ_REG(16, addr);
    if (R_DEV_PIN_DIRECT == PinCfg->Dir)
    {
        reg_val |= pin_position; 
    }
    else
    {
        reg_val &= (uint16_t)(~pin_position);
    }
    R_DEV_WRITE_REG(16, addr, reg_val);
    
    
    /* activate pull-up / pull-down resistors */
    /*  set pull-up */
    addr    = R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PU, PinCfg->Port);
    reg_val = R_DEV_READ_REG(16, addr);
    if (R_DEV_PIN_PULLUP == PinCfg->Pull)
    {
        reg_val |= pin_position; 
    }
    else
    {
        reg_val &= (uint16_t)(~pin_position);
    }
    R_DEV_WRITE_REG(16, addr, reg_val);
    /*  set pull-down */
    addr    = R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PD, PinCfg->Port);
    reg_val = R_DEV_READ_REG(16, addr);
    if (R_DEV_PIN_PULLDOWN == PinCfg->Pull)
    {
        reg_val |= pin_position; 
    }
    else
    {
        reg_val &= (uint16_t)(~pin_position);
    }
    R_DEV_WRITE_REG(16, addr, reg_val);

    
    /* feedback input (bidirectional mode) */
    addr    = R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PBDC, PinCfg->Port);
    reg_val = R_DEV_READ_REG(16, addr);
    if (1 == PinCfg->Feedback)
    {
        reg_val |= pin_position; 
    }
    else
    {
        reg_val &= (uint16_t)(~pin_position);
    }
    R_DEV_WRITE_REG(16, addr, reg_val);
    
    
    /*Set mode to GPIO or alternative function */
    switch(PinCfg->Func)
    {
    case 0:  /* GPIO */                                   
        /*Set PFC and PFCE bits to 0 */     
        addr    =   R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PFC, PinCfg->Port);
        reg_val =   R_DEV_READ_REG(16, addr);
        reg_val &=  (uint16_t)(~pin_position);  
        R_DEV_WRITE_REG(16, addr, reg_val);
 
        addr    = R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PFCE, PinCfg->Port);
        reg_val =   R_DEV_READ_REG(16, addr);
        reg_val &= (uint16_t)(~pin_position);  
        R_DEV_WRITE_REG(16, addr, reg_val);
        break;
    case 1:                                     /* Alternative function 1 */
        /*Set PFC and PFCE bits to 0 */     
        addr    =   R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PFC, PinCfg->Port);
        reg_val =   R_DEV_READ_REG(16, addr);
        reg_val &= (uint16_t)(~pin_position);  
        R_DEV_WRITE_REG(16, addr, reg_val);
 
        addr    =   R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PFCE, PinCfg->Port);
        reg_val =   R_DEV_READ_REG(16, addr);
        reg_val &= (uint16_t)(~pin_position);  
        R_DEV_WRITE_REG(16, addr, reg_val);
        break;
    case 2:                                     /* Alternative function 2 */
        /*Set PFC bit to 1  and PFCE bit to 0 */     
        addr    =   R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PFC, PinCfg->Port);
        reg_val =   R_DEV_READ_REG(16, addr);
        reg_val |=  pin_position;  
        R_DEV_WRITE_REG(16, addr, reg_val);
 
        addr    =   R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PFCE, PinCfg->Port);
        reg_val =   R_DEV_READ_REG(16, addr);
        reg_val &= (uint16_t)(~pin_position);  
        R_DEV_WRITE_REG(16, addr, reg_val);
        break;
    case 3:                                     /* Alternative function 3 */
        /*Set PFC bit to 0  and PFCE bit to 1 */     
        addr    =   R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PFC, PinCfg->Port);
        reg_val =   R_DEV_READ_REG(16, addr);
        reg_val &= (uint16_t)(~pin_position);  
        R_DEV_WRITE_REG(16, addr, reg_val);
 
        addr    =   R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PFCE, PinCfg->Port);
        reg_val =   R_DEV_READ_REG(16, addr);
        reg_val |=  pin_position;  
        R_DEV_WRITE_REG(16, addr, reg_val);
        break;
    case 4:                                     /* Alternative function 4 */
        /*Set PFC and PFCE bits to 1 */     
        addr    =   R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PFC, PinCfg->Port);
        reg_val =   R_DEV_READ_REG(16, addr);
        reg_val |=  pin_position;  
        R_DEV_WRITE_REG(16, addr, reg_val);
 
        addr    =   R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PFCE, PinCfg->Port);
        reg_val =   R_DEV_READ_REG(16, addr);
        reg_val |=  pin_position;  
        R_DEV_WRITE_REG(16, addr, reg_val);
        break;
    default:
      //  R_DEV_ERROR(0u, R_DEV_ERR_NG, "We do not have what you want!"); 
        break; 
    }
    
    /* Some electrical charactersitics */
    R_DBG_PRINT(R_DBG_MSG_DET, "--- Electrical charakteristics  ---"); 
    addr    =   R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PDSC, PinCfg->Port);
    reg_val =   R_DEV_READ_REG(32, addr);
    if (0 != PinCfg->HiSpeed)
    {
        reg_val |= pin_position; 
    }
    else 
    {
        reg_val &= (uint16_t)(~pin_position); 
    }
    prot_addr =   R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PPCMD, PinCfg->Port);
    R_DEV_WRITE_PROT_REG(prot_addr ,addr, reg_val);
    
    
    /* Activate pin */
    addr    =   R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PMC, PinCfg->Port);
    reg_val =   R_DEV_READ_REG(16, addr);
    if (0 != PinCfg->Func)
    {
        reg_val |= pin_position; 
    }
    else 
    {
        reg_val &= (uint16_t)(~pin_position); 
    }
    R_DEV_WRITE_REG(16, addr, reg_val);
    
    /* Set input characteristic */    
    port_position       = 0u;
    portHasPisaRegister = 1u;
    portHasPisRegister  = 1u;
    PisaRegBitValue     = 0u;
    PisRegBitValue      = 0u;
    do {
        if (loc_PortCapability[port_position].Port == PinCfg->Port)
        {
            if (loc_PortCapability[port_position].InputType00 == PinCfg->InputType)
            {
                PisaRegBitValue = 0;
                PisRegBitValue  = 0;
            }
            if (loc_PortCapability[port_position].InputType01 == PinCfg->InputType)
            {
                PisaRegBitValue = 0;
                PisRegBitValue  = 1;
                if (loc_PortCapability[port_position].InputType01
                    == loc_PortCapability[port_position].InputType00)
                {
                    portHasPisRegister  = 0;
                }
            }
            if (loc_PortCapability[port_position].InputType10 == PinCfg->InputType)
            {
                PisaRegBitValue = 1;
                PisRegBitValue  = 0;
            }
            if (loc_PortCapability[port_position].InputType11 == PinCfg->InputType)
            {
                PisaRegBitValue = 1;
                PisRegBitValue  = 1;
                if (loc_PortCapability[port_position].InputType11
                    == loc_PortCapability[port_position].InputType10)
                {
                    portHasPisRegister  = 0;
                }
            }
            
            break;
        }
        port_position += 1;
    }
    while (0xFFu != loc_PortCapability[port_position].Port);
    
    if (0 != portHasPisaRegister)
    {
        addr    =   R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PIS, PinCfg->Port);
        reg_val =   R_DEV_READ_REG(16, addr);
        if (0 != PisaRegBitValue)
        {
            reg_val |= pin_position; 
        }
        else 
        {
            reg_val &= (uint16_t)(~pin_position); 
        }
        R_DEV_WRITE_REG(16, addr, reg_val);
    }
    
    if (0 != portHasPisRegister)
    {
        addr    =   R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PIS, PinCfg->Port);
        reg_val =   R_DEV_READ_REG(16, addr);
        if (0 != PisRegBitValue)
        {
            reg_val |= pin_position; 
        }
        else 
        {
            reg_val &= (uint16_t)(~pin_position); 
        }
        R_DEV_WRITE_REG(16, addr, reg_val);
    }
    
    
    /* Activate input buffer in case of GP input  */
    addr    =   R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PIBC, PinCfg->Port);
    reg_val =   R_DEV_READ_REG(16, addr);
    if ((0 == PinCfg->Func) && (R_DEV_PIN_IN == PinCfg->Dir))
    {
        reg_val |= pin_position; 
    }
    else 
    {
        reg_val &= (uint16_t)(~pin_position); 
    }
    R_DEV_WRITE_REG(16, addr, reg_val);

    R_DBG_PRINT(R_DBG_MSG_DET, "--- Pin Config End ---"); 
    return ret_val; 
    
}

/*******************************************************************************
  Function: R_DEV_PinInit

  See: r_dev_api.h for details    
*/
void R_DEV_PinInit(const r_dev_PinConfig_t * PinCfg)
{
    uint16_t i = 0u; 
    /* Here we can glbally disable any pion configuration */
    if (0 == loc_PinEnable)
    {
       
        return; 
    }    
    /* assuming we got a config table, we go through it until 
    we see the delimiter */   
    while (R_DEV_PIN_LAST != PinCfg[i].Pin) 
    {
        R_DEV_PinConfig(&(PinCfg[i])) ;  
        i++; 
    }
}

/*******************************************************************************
  Function: R_DEV_PinGetRegAddr

  See: r_dev_api.h for details    
*/
 uint32_t R_DEV_PinGetRegAddr(r_dev_PinRegs_t Reg, uint32_t Port)
{
    return R_DEV_PinGetRegAddrExt(Reg, Port, 0u);
}

/*******************************************************************************
  Function: R_DEV_PinGetRegAddrExt

  See: r_dev_api.h for details    
*/
 uint32_t R_DEV_PinGetRegAddrExt(r_dev_PinRegs_t Reg, uint32_t Port, uint32_t Pin)
{
    uint32_t addr = R_DEV_PIN_BASE + (R_DEV_PORT_OFFSET * Port);
    
    if (Reg < R_DEV_PIN_REG_LAST) 
    {
        if (Port == 0)
        {
            /* add offset for port n = 0 */
            addr += R_DEV_PIN_BASEOU_0_OFFSET;
        }
        if (R_DEV_PIN_REG_PCR == Reg)
        {
            /* only for PCRn register, that has pin based control */
            addr += (R_DEV_PIN_OFFSET * Pin);
        }
        addr += loc_RegOffset[Reg];
    }
    else
    {
        R_DEV_ERROR((uint32_t)Reg, R_DEV_ERR_REG, "That register does not exist!");      
    }
    return addr; 
}


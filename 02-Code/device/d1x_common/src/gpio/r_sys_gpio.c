/*
****************************************************************************
PROJECT : r7f701412
FILE    : $Id: r_sys_gpio.c 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
PORTC system functions for r7f701412
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
  Title: r7f701412 GPIO Support Functions

  Implementation of the GPIO support functions for the 
  device r7f701412.
*/

/*******************************************************************************
  Section: Includes
*/

#include "r_typedefs.h"
#include "r_gpio_api.h"
#include "r_gpio_sys.h"
#include "r_config_pin.h"
#include "r_config_gpio.h"

/* The debug output for register accesses here shall be only active 
on special request, otherwise the output might get "overloaded"  
If you want to see all register accesses, activate R_DBG_SHOW_PIN_REGS
in your application's Makefile */
#ifndef R_DBG_SHOW_PIN_REGS
  #undef  R_DBG_PRINT_DEV_REGS
#endif   
#include "r_dev_api.h"



/*******************************************************************************
  Section: Local Constants
*/

/*******************************************************************************
  Constant: LOC_FCLA0CTL_BASE
    
    Base register address for filter configuration 
*/
#define LOC_FCLA0CTL_BASE       0xffc34000u
#define LOC_FCLA1CTL_BASE       0xffc34100u

/*******************************************************************************
Array : loc_FclaCtrlRegOffset

The loc_FclaCtrlRegOffset array specifies FCLAnCTLm Register offsets
*/

static const uint32_t loc_FclaCtrlRegOffset[12] = {
    /* 0: INTP0  */             (LOC_FCLA0CTL_BASE + 0x0004u),
    /* 1: INTP1  */             (LOC_FCLA0CTL_BASE + 0x0008u),
    /* 2: INTP2  */             (LOC_FCLA0CTL_BASE + 0x000Cu),
    /* 3: INTP3  */             (LOC_FCLA0CTL_BASE + 0x0010u),
    /* 4: INTP4  */             (LOC_FCLA0CTL_BASE + 0x0014u),
    /* 5: INTP5  */             (LOC_FCLA0CTL_BASE + 0x0018u),
    /* 6: INTP6  */             (LOC_FCLA0CTL_BASE + 0x001Cu),
    /* 7: INTP7  */             (LOC_FCLA1CTL_BASE + 0x0000u),
    /* 8: INTP8  */             (LOC_FCLA1CTL_BASE + 0x0004u),
    /* 9: INTP9  */             (LOC_FCLA1CTL_BASE + 0x0008u),
    /*10: INTP10 */             (LOC_FCLA1CTL_BASE + 0x000Cu),
    /*11: NMI    */             (LOC_FCLA0CTL_BASE + 0x0000u),
};

/*******************************************************************************
  Constant: LOC_DNFA0CTL
    
    Base register address for filter control 
*/
#define LOC_DNFA0CTL            0xffc30000u

/*******************************************************************************
  Constant: LOC_DNFA0EN
    
    Base register address for filter enable 
*/
#define LOC_DNFA0EN             0xffc30004u

/*******************************************************************************
  Constant: loc_FilterSetting
    Stores required register values for the filter configuration 
*/
static const uint8_t loc_FilterSetting[R_GPIO_INT_TRIGGER_NUM] = 
{
/* R_GPIO_INT_TRIGGER_OFF         */ 0u, 
/* R_GPIO_INT_RISING_EDGE         */ 0x1u,
/* R_GPIO_INT_FALLING_EDGE        */ 0x2u, 
/* R_GPIO_INT_BOTH_EDGES          */ 0x3u,
/* R_GPIO_INT_LEVEL_HIGH          */ 0x5u,
/* R_GPIO_INT_LEVEL_LOW           */ 0x4u
};     

/*******************************************************************************
  Section: Global Functions

*/

/*******************************************************************************
  Function: R_SYS_GPIO_GetPin

  See: r_gpio_sys.h for details
 */
 uint8_t R_SYS_GPIO_GetPin(int32_t Port, uint8_t Pin)
{
    uint32_t  addr         = R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PPR, Port);  
    uint16_t  pin_position = 1u;
    uint16_t  reg_val      = R_DEV_READ_REG(16,addr);

    pin_position = (uint16_t)(pin_position << Pin);
    if (0 != (reg_val & pin_position)) 
    {
        return 1; 
    }
    else 
    {
        return 0; 
    }
}

/*******************************************************************************
  Function: R_SYS_GPIO_SetPin

 See: r_gpio_sys.h for details
*/

void R_SYS_GPIO_SetPin(int32_t Port, uint8_t Pin, uint8_t Value)
{
    uint32_t  addr         = R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PSR, Port); 
    uint16_t  pin_position = (uint16_t)((uint16_t)1u << (uint16_t)Pin);
    uint32_t  reg_val      = pin_position;

    /* we use masked port reg write 
            reg_val = Mask<<16 | Port */
    reg_val    <<= 16; 

    
    /* Set or Clear? */
    if (0 != Value) /* Set bit*/ 
    {
        reg_val |= (uint32_t)pin_position; 
    }
    else      /* Clear bit*/
    {
        /* Nothing to do */ 
    }
    R_DEV_WRITE_REG(32, addr, reg_val); 
}

/*******************************************************************************
  Function: R_SYS_GPIO_InvPin

 See: r_gpio_sys.h for details
*/

void R_SYS_GPIO_InvPin(int32_t Port, uint8_t Pin)
{
    uint32_t  addr     = R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PNOT, Port); 
    uint16_t  pin_mask = (uint16_t)((uint16_t)1u << (uint16_t)Pin);
    R_DEV_WRITE_REG(32, addr, pin_mask); 
}

/*******************************************************************************
  Function: R_SYS_GPIO_InvPort

 See: r_gpio_sys.h for details
*/

void R_SYS_GPIO_InvPort(int32_t Port)
{
    uint32_t  addr         = R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PNOT, Port); 
    R_DEV_WRITE_REG(32, addr, 0xFFFFu); 
}

/*******************************************************************************
  Function: R_SYS_GPIO_GetPort
  
 See: r_gpio_sys.h for details
*/

 uint16_t R_SYS_GPIO_GetPort(int32_t Port)
{
    uint32_t  addr = R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PPR, Port);  
    uint32_t  val = R_DEV_READ_REG(16,addr);  
    return val; 
}


/*******************************************************************************
  Function: R_SYS_GPIO_PinDirection
  
 See: r_gpio_sys.h for details

*/

void R_SYS_GPIO_PinDirection(int32_t Port, uint8_t Pin, uint8_t Dir)
{
    uint32_t  addr = R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PM, Port);  
    uint16_t  reg_val      = 0u;
    uint16_t  pin_position = 1u;

    reg_val = R_DEV_READ_REG(16, addr);
    if (0 == Dir) /*  Set direction to output */
    {
        reg_val &= (uint16_t)(~pin_position);
        R_DEV_WRITE_REG(16, addr, reg_val);
    }
    else /* Set direction to input */
    {
        reg_val |= pin_position;
        R_DEV_WRITE_REG(16, addr, reg_val);
    }
    
    /* Activate input buffer in case of GP input  */
    addr    = R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PIBC, Port);
    reg_val = R_DEV_READ_REG(16, addr);
    if (0 == Dir)
    {
        reg_val &= (uint16_t)(~pin_position); 
    }
    else 
    {
        reg_val |= pin_position; 
    }
    R_DEV_WRITE_REG(16, addr, reg_val);
    
}

/*******************************************************************************
  Function: R_SYS_GPIO_WritePort

  See: r_gpio_sys.h for details
*/

void R_SYS_GPIO_WritePort(int32_t Port, uint16_t Mask, uint16_t Value)
{
    uint32_t  addr         = R_DEV_PinGetRegAddr(R_DEV_PIN_REG_PSR, Port); 
    uint32_t  reg_val      = Mask;

    /* we use masked port reg write 
            reg_val = Mask<<16 | Port */
    reg_val    <<= 16; 
    reg_val |= (uint32_t)Value; 
    R_DEV_WRITE_REG(32, addr, reg_val); 

}
/*******************************************************************************
  Function: R_SYS_GPIO_EnableInt

  See: r_gpio_sys.h for details
*/

void R_SYS_GPIO_EnableInt(uint32_t Unit) 
{
    if (Unit < R_GPIO_EXT_INT_NUM) 

    {
        R_DEV_IntEnable((r_dev_IntSel_t)(R_DEV_INT_P0 + Unit) ,1); 
    }
    else 
    {
        R_DEV_ERROR(Unit, R_DEV_ERR_INTR, "GPIO: We do not have this Ext. Int");      
    }
}


/*******************************************************************************
  Function: R_SYS_GPIO_DisableInt

  See: r_gpio_sys.h for details
*/

void R_SYS_GPIO_DisableInt(uint32_t Unit) 
{
    if (Unit < R_GPIO_EXT_INT_NUM) 
    {
        R_DEV_IntEnable((r_dev_IntSel_t)(R_DEV_INT_P0 + Unit), 0);
    }
    else 
    {
        R_DEV_ERROR(Unit, R_DEV_ERR_INTR, "GPIO: We do not have this Ext. Int");      
    }
}

/*******************************************************************************
  Function: R_SYS_GPIO_SetIntFilter

  See: r_gpio_sys.h for details
*/
void R_SYS_GPIO_SetIntFilter(uint32_t Unit, r_gpio_IntTrigger_t Trigger)
{
    if (Trigger < R_GPIO_INT_TRIGGER_NUM)
    {
        R_DEV_WRITE_REG(8, loc_FclaCtrlRegOffset[Unit], loc_FilterSetting[Trigger]);
    }
    else 
    {
        R_DEV_ERROR(Unit, R_DEV_ERR_INTR, "GPIO: We do not have this Ext. Int"); 
    }
}

/*******************************************************************************
  Function: R_SYS_GPIO_ClearIntPendingFlag

  See: r_gpio_sys.h for details
*/
void R_SYS_GPIO_ClearIntPendingFlag(uint32_t Unit)
{
    R_DEV_IntClearFlag((r_dev_IntSel_t)(R_DEV_INT_P0 + Unit)); 
} 


/*******************************************************************************
  Function: R_SYS_GPIO_IOReset
  
  See: r_gpio_sys.h for details
*/

void R_SYS_GPIO_IOReset(uint8_t SetRelease) 
{
    /* functionality not implemented, please implement if you need */

}

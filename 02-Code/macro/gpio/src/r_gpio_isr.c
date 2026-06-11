/*
****************************************************************************
PROJECT : GPIO driver
FILE    : $Id: r_gpio_isr.c 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================
DESCRIPTION
Driver for GPIO macro
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


/***********************************************************************************************
                            Macro description
GPIO functions 
Set/reset read pin
Configure and activcate external portpin interrupts  

*/

/*******************************************************************************
  Section: Includes
*/
#include "r_typedefs.h"
#include "r_gpio_api.h"
#include "r_gpio_sys.h"
#include "r_config_gpio.h"
#include "r_dev_api.h"


/*******************************************************************************
  Section: Local Functions
*/

/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Section: Local Defines
  
*/

/*******************************************************************************
  Section: Local Types
  
*/

/*******************************************************************************
    struct R_GPIO_Int_t
    For information about configured external interrupts 
   Values:
    Port     - Port Number 
    Pin      - Pin  Number 
    Callback - Callback (notification) function 
  
*/
typedef struct {
    int32_t Port; 
    int32_t Pin;
    void (*Callback)(void);
} loc_Int_t; 

/*******************************************************************************
  Section: Local Variables
*/

/*******************************************************************************
  Variable: loc_IntConfig

  Stores the Port/Pin configuration for external Interrupts  
*/
static loc_Int_t loc_IntConfig[R_GPIO_EXT_INT_NUM]; 


/*******************************************************************************
  Section: Global Functions
*/

/*******************************************************************************
  Function: R_GPIO_SetIntCallback
  
  see <r_gpio_api.h> for details 
*/
r_Error_t R_GPIO_SetIntCallback(uint32_t Unit, void (*Isr)(void)) 
{
    if (Unit >= R_GPIO_EXT_INT_NUM) 
    {
        return R_PARAMETER_INCORRECT; 
    }
    else
    {
        loc_IntConfig[Unit].Callback = Isr;
    }
    return R_ERR_OK;
}

/*******************************************************************************
  Function: R_GPIO_IntIsr

  see <r_gpio_api.h> for details 
*/
void R_GPIO_IntIsr(uint32_t Unit) 
{
    if (Unit >= R_GPIO_EXT_INT_NUM)
    {
        /* ISR Called for non existant INT*/
        R_DEV_ErrorHandler(Unit, (uint32_t)R_DEV_ERR_UNIT); 
    }
    else
    {
        if (0u != loc_IntConfig[Unit].Callback)
        {
            loc_IntConfig[Unit].Callback();
        }
    }
}


/*******************************************************************************
  Function: R_GPIO_ReadIntPin

  see <r_gpio_api.h> for details 
*/
uint8_t R_GPIO_ReadIntPin(uint32_t Unit) 
{
    if (Unit < R_GPIO_EXT_INT_NUM) 
    {
        return R_SYS_GPIO_GetPin(loc_IntConfig[Unit].Port, loc_IntConfig[Unit].Pin);
    }
    else
    {
        /* ERROR - HALT right here */
        /*  I Don't know what to do - the unit you ask for does not exist! */
        R_DEV_ErrorHandler(Unit, R_DEV_ERR_UNIT); 
        return 0u; 
    }
}


/*******************************************************************************
  Function: R_GPIO_EnableInt

  see <r_gpio_api.h> for details 
*/
void R_GPIO_EnableInt(uint32_t Unit) 
{
    R_SYS_GPIO_EnableInt(Unit);
}


/*******************************************************************************
  Function: R_GPIO_DisableInt

  see <r_gpio_api.h> for details 
*/
void R_GPIO_DisableInt(uint32_t Unit)
{
    R_SYS_GPIO_DisableInt(Unit);
}

/*******************************************************************************
  Function: R_GPIO_InitInt
  see <r_gpio_api.h> for details 
*/
r_Error_t R_GPIO_InitInt(uint32_t Unit, r_gpio_IntConfig_t *Config) 
{
    if (Unit < R_GPIO_EXT_INT_NUM) 
    {
        /* Store configuration */
        loc_IntConfig[Unit].Port     = Config->Port; 
        loc_IntConfig[Unit].Pin      = Config->Pin; 
        loc_IntConfig[Unit].Callback = Config->Callback; 
        
        
        /* init interrupt */
        R_SYS_GPIO_SetIntFilter(Unit, Config->Trigger);   
        R_SYS_GPIO_ClearIntPendingFlag(Unit); 
        return R_ERR_OK; 
    }
    else 
    {
        return R_PARAMETER_INCORRECT;
    }
}

/*******************************************************************************
  Function: R_GPIO_DeInitInt

  see <r_gpio_api.h> for details 
*/
r_Error_t R_GPIO_DeInitInt(uint32_t Unit) 
{
    if (Unit < R_GPIO_EXT_INT_NUM) 
    {
        /* init interrupt */
        R_SYS_GPIO_ClearIntPendingFlag(Unit); 
        R_SYS_GPIO_DisableInt(Unit);
        return R_ERR_OK; 
    }
    else 
    {
        return R_PARAMETER_INCORRECT;
    }
}



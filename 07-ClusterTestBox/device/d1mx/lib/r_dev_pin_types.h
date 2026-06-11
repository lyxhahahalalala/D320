/*
****************************************************************************
PROJECT : Vlib device driver
FILE    : $Id: r_dev_pin_types.h 3010 2014-06-17 14:48:17Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Driver for CLK macro
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

#ifndef DEV_PIN_TYPES_H_
#define DEV_PIN_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: CPU API

  The interface for all CPU related accesses
*/


/*******************************************************************************
  Section: Global Constants 
*/

/*******************************************************************************
  Constant: R_DEV_PIN_LAST

  Use as delimiter for pin config arrays 
  Set pin numer to R_DEV_PIN_LAST
  
*/
#define R_DEV_PIN_LAST 0xffu
/*******************************************************************************
  Section: Global Types
*/
/*******************************************************************************
  Enum: r_dev_PinDirection_t

  Direction modes of a IO pin

  Values:
    R_DEV_PIN_IN           - Set pin as input
    R_DEV_PIN_OUT          - Set pin as output
    R_DEV_PIN_DIRECT       - give direction control 
                             to the macro that is using the pin

*/
typedef enum
{
    R_DEV_PIN_OUT = 0,
    R_DEV_PIN_IN ,
    R_DEV_PIN_DIRECT
} r_dev_PinDirection_t;

/*******************************************************************************
  Enum: r_dev_PinPull_t

  Possible built in pull resistor settings

  Values:
    R_DEV_PIN_PULLNO        - no resistor connected
    R_DEV_PIN_PULLUP        - Connect pullup resistor
    R_DEV_PIN_PULLDOWN      - Connect pulldown resistor
*/

typedef enum
{
    R_DEV_PIN_PULLNO  = 0,
    R_DEV_PIN_PULLUP,
    R_DEV_PIN_PULLDOWN
} r_dev_PinPull_t;

/*******************************************************************************
  Enum: r_dev_PinInput_t
 
   Input characterstics
   
   Do not change the order of this enum!
   Values: 
    R_DEV_PIN_NONE          - no input characteristic selectable (default)
    R_DEV_PIN_CMOS1         - see datasheet
    R_DEV_PIN_SCHMITT2      - see datasheet 
    R_DEV_PIN_SCHMITT1      - see datasheet
    R_DEV_PIN_SCHMITT4      - see datasheet
    R_DEV_PIN_TTL           - see datasheet
    R_DEV_PIN_TTL_MLB       - see datasheet
*/  
typedef enum
{
    R_DEV_PIN_NONE   = 0,
    R_DEV_PIN_CMOS1,
    R_DEV_PIN_SCHMITT2,
    R_DEV_PIN_SCHMITT1,
    R_DEV_PIN_SCHMITT4,
    R_DEV_PIN_TTL,
    R_DEV_PIN_TTL_MLB
}  r_dev_PinInput_t;

/*******************************************************************************
  Type: r_dev_PortInputCapability_t
  
  The capability of input characteristics for a device port
  
  Values:
    Port                    -  port number
    InputTypeXX             -  input characteristics, see <r_dev_PinInput_t>
                               where XX stands for 00, 01, 10 and 11
                               and mean the PISA/PIS register settings
                                o 00 -> PISAn_m = 0 / PISn_m = 0
                                o 01 -> PISAn_m = 0 / PISn_m = 1
                                o 10 -> PISAn_m = 1 / PISn_m = 0
                                o 11 -> PISAn_m = 1 / PISn_m = 1
                               Refer also to D1LM UM table
                               "ports input buffer characteristics" (UM V0.5 table 2.30)
*/

typedef struct 
{
    uint8_t                 Port; 
    r_dev_PinInput_t        InputType11;
    r_dev_PinInput_t        InputType10;
    r_dev_PinInput_t        InputType01;
    r_dev_PinInput_t        InputType00;
}r_dev_PortInputCapability_t;


/*******************************************************************************
  Type: r_dev_PinConfig_t
  
  The comlplete configuration structure for a device pin 
  
  Values:
    Port                    -  port number
    Pin                     -  pin number
    Func                    -  pin function 0: GPIO, 1-4 alternative function
    Dir                     -  pin direction, see <r_dev_PinDirection_t>
    Feedback                -  1: physical (output) pin level provided to input register, 0: disabled
    Pull                    -  internal pull up configuration, see <r_dev_PinPull_t>
    OpenDrain               -  1: open drain emulation, 0: push-pull
    HiCurr                  -  1: high level current limitation, 0: low level current limitation 
    HiSpeed                 -  1: Speed limitation disabled (drive strength control off), 0: Speed limit enabled  
    InputType               -  possible types of input characteristics, see <r_dev_PinInput_t>
  
*/  

typedef struct 
{
    uint8_t                 Port; 
    uint8_t                 Pin;
    uint8_t                 Func;
    r_dev_PinDirection_t    Dir;
    uint8_t                 Feedback; 
    r_dev_PinPull_t         Pull;
    uint8_t                 OpenDrain; 
    uint8_t                 CurrLimit; 
    uint8_t                 HiSpeed; 
    uint8_t                 InputType; 
}r_dev_PinConfig_t; 


/*******************************************************************************
  Enum:  r_dev_PinRegs_t

  This enum is used as index for R_GPIO_Register_Table array

  Values:
    R_DEV_PIN_REG_PCR          - PCR   Port control register (pin based control)     R/W     32
    R_DEV_PIN_REG_P            - P     Port register                                 R/W     16
    R_DEV_PIN_REG_PSR          - PSR   Port Set Reset register                       R/W     32
    R_DEV_PIN_REG_PNOT         - PNOT  Port NOT register                               W     16
    R_DEV_PIN_REG_PPR          - PPR   Port Pin Read register                        R       16
    R_DEV_PIN_REG_PM           - PM    Port Mode register                            R/W     16
    R_DEV_PIN_REG_PMC          - PMC   Port Function Control register                R/W     16
    R_DEV_PIN_REG_PMSR         - PMSR  Port Mode register Set Reset                  R/W     32
    R_DEV_PIN_REG_PMCSR        - PMCSR Port Function Control register                R/W     32
    R_DEV_PIN_REG_PFC          - PFC   Port Function Control Expansion register      R/W     16
    R_DEV_PIN_REG_PFCE         - PFCE  Port Function Control Expansion register      R/W     16
    R_DEV_PIN_REG_PIBC         - PIBC  Port Input Buffer Control register            R/W     16
    R_DEV_PIN_REG_PBDC         - PBDC  Port Bi-Direction Control register            R/W     16
    R_DEV_PIN_REG_PIC          - PIC   Port IP control register                      R/W     16
    R_DEV_PIN_REG_PU           - PU    Pull-up option register                       R/W     16
    R_DEV_PIN_REG_PD           - PD    Pull-down option register                     R/W     16
    R_DEV_PIN_REG_PODC         - PODC  Port Open Drain Control register              R/W     16
    R_DEV_PIN_REG_PDSC         - PDSC  Port Drive Strength Control register          R/W     16
    R_DEV_PIN_REG_PICC         - PICC  Port Input Characteristic Control register    R/W     16
    R_DEV_PIN_REG_PILC         - PILC  Port Input Level Control register             R/W     16
    R_DEV_PIN_REG_INTR         - INTR  Rising Edge Detection register                R/W     16
    R_DEV_PIN_REG_INTF         - INTF  Falling Edge Detection register               R/W     16
    R_DEV_PIN_REG_PPCMD        - PPCMD Port Protection Command register              R/W     8
*/

typedef enum
{
    R_DEV_PIN_REG_PCR    = 0,
    R_DEV_PIN_REG_P      = 1,
    R_DEV_PIN_REG_PSR    = 2,
    R_DEV_PIN_REG_PPR    ,
    R_DEV_PIN_REG_PM     ,
    R_DEV_PIN_REG_PMC    ,
    R_DEV_PIN_REG_PFC    ,
    R_DEV_PIN_REG_PFCE   ,
    R_DEV_PIN_REG_PNOT   ,
    R_DEV_PIN_REG_PMSR   ,
    R_DEV_PIN_REG_PMCSR,
    /*  OS  */
    R_DEV_PIN_REG_PIBC   ,
    R_DEV_PIN_REG_PBDC   ,
    R_DEV_PIN_REG_PIPC   ,
    R_DEV_PIN_REG_PU     ,
    R_DEV_PIN_REG_PD     ,
    R_DEV_PIN_REG_PODC   ,
    R_DEV_PIN_REG_PDSC   ,
    R_DEV_PIN_REG_PIS    ,
    R_DEV_PIN_REG_PISA   ,
    R_DEV_PIN_REG_PPCMD  ,
    R_DEV_PIN_REG_PPROTS ,
    /* delimiter - do not touch*/
    R_DEV_PIN_REG_LAST
}r_dev_PinRegs_t;

#ifdef __cplusplus
}
#endif

#endif /* DEV_PIN_TYPES_H_  */

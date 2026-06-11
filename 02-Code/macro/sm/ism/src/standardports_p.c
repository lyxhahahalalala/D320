//============================================================================
// PROJECT = Any, using standard ports set
// MODULE  = $Source$
// VERSION = $Revision: 4594 $
// DATE    = $Date: 2015-01-08 18:13:58 +0100 (Do, 08 Jan 2015) $
//============================================================================
//                                 C O P Y R I G H T                                    
//============================================================================
// Copyright (c) 2002 by NEC Electronics (Europe) GmbH. All rights reserved.
// Kanzlerstrasse 2
// D-40472 Duesseldorf
// Germany
//============================================================================
//Purpose: Standard Port Low Level Driver
//
//Warranty Disclaimer
//
//Because the Product(s) is licensed free of charge, there is no warranty 
//of any kind whatsoever and expressly disclaimed and excluded by NEC, 
//either expressed or implied, including but not limited to those for 
//non-infringement of intellectual property, merchantability and/or 
//fitness for the particular purpose. 
//NEC shall not have any obligation to maintain, service or provide bug 
//fixes for the supplied Product(s) and/or the Application.
//
//Each User is solely responsible for determining the appropriateness of 
//using the Product(s) and assumes all risks associated with its exercise 
//of rights under this Agreement, including, but not limited to the risks 
//and costs of program errors, compliance with applicable laws, damage to 
//or loss of data, programs or equipment, and unavailability or 
//interruption of operations.
//
//Limitation of Liability
//
//In no event shall NEC be liable to the User for any incidental, 
//consequential, indirect, or punitive damage (including but not limited 
//to lost profits) regardless of whether such liability is based on breach 
//of contract, tort, strict liability, breach of warranties, failure of 
//essential purpose or otherwise and even if advised of the possibility of 
//such damages. NEC shall not be liable for any services or products 
//provided by third party vendors, developers or consultants identified or
//referred to the User by NEC in connection with the Product(s) and/or the 
//Application.
//
//
//
//============================================================================
// Environment: Devices:          all featuring standard ports
//              Assembler:        GHS MULTI 2000
//              C-Compiler:       GHS MULTI 2000
//              Linker:           GHS MULTI 2000
//              Debugger:         GHS MULTI 2000
//============================================================================
//
// $Log$
// Revision 1.2  2004/08/09 08:24:28  liederr
// PORT 13 (Numeric Port) supported.
//
// Revision 1.1  2003/12/11 14:17:54  liederr
// Initial creation.
//
//
//============================================================================

#define DRIVER_LOCAL

// $$$: TODO cleanup !!!!!!
#include "r_typedefs.h"


#include "standardports_p.h"
#include "r_dev_api.h"

#include "r_config_ism.h"

 
//=============================================================================
// FunctionName: PORT_Enable
// IN  :         Port Number, Bit Spec, Port Direction, Peri. Mode, Alt. Mode
// OUT :         Error Flag
// Description : Set Port to Mode and Direction
// 
//=============================================================================

uint8_t PORT_Enable( uint16_t PortNumber_uint16_t,
                 uint16_t BitSpecification_uint16_t,
                 uint8_t PortDirection_u08,
                 uint8_t PeripheralMode_u08,
                 uint8_t AlternateFunction_u08 )
{
    r_dev_PinConfig_t pinCfg;
    pinCfg.Port = PortNumber_uint16_t;
    pinCfg.Pin  = BitSpecification_uint16_t;

    if (PORT_MODE_IO == PeripheralMode_u08)
    {
        if (PORT_DIR_INPUT == PortDirection_u08)
        {
            pinCfg.Dir = R_DEV_PIN_IN;
        }
        else {
            pinCfg.Dir = R_DEV_PIN_OUT;
        }
        pinCfg.Func = 0;
    } else {
        pinCfg.Dir = R_DEV_PIN_DIRECT;
        pinCfg.Func = AlternateFunction_u08;
    }
  
    pinCfg.Feedback  = 0;
    pinCfg.Pull      = R_DEV_PIN_PULLNO;
    pinCfg.OpenDrain = 0;
    pinCfg.CurrLimit = 1;
    pinCfg.HiSpeed   = 1;
    pinCfg.InputType = R_DEV_PIN_CMOS1;
    R_DEV_PinConfig(&pinCfg);
    
    return( R_TRUE );    
}   

//=============================================================================
// FunctionName: PORT_PortDisable
// IN  :         Port Number
// OUT :         Error Flag
// Description : Set Port to Input Mode, Keep other settings
// 
//=============================================================================

uint8_t PORT_Disable( uint16_t PortNumber_uint16_t,
                  uint16_t BitSpecification_uint16_t )
{
    return( PORT_Enable( PortNumber_uint16_t,
                         BitSpecification_uint16_t,
                         PORT_DIR_INPUT,
                         PORT_MODE_IO,
                         PORT_FUNCTION_NORMAL ) );
}    
    

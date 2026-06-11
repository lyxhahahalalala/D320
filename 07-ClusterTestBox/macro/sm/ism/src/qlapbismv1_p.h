//============================================================================
// PROJECT = ISM Type qlapbismv1
// MODULE  = $Source$
// VERSION = $Revision: 4975 $
// DATE    = $Date: 2015-03-27 17:22:34 +0100 (Fr, 27 Mrz 2015) $
//============================================================================
//                                 C O P Y R I G H T                                    
//============================================================================
// Copyright (c) 2002 by NEC Electronics (Europe) GmbH. All rights reserved.
// Kanzlerstrasse 2
// D-40472 Duesseldorf
// Germany
//============================================================================
//Purpose: ISM Driver Functions Include
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
// Environment: Devices:          All featuring qlapbismv1
//              Assembler:        GHS MULTI 2000
//              C-Compiler:       GHS MULTI 2000
//              Linker:           GHS MULTI 2000
//              Debugger:         GHS MULTI 2000
//============================================================================

#ifndef _QLAPBISMV1_P_H
#define _QLAPBISMV1_P_H

#ifdef __cplusplus
extern "C" {
#endif

#include "qlapbismv1.h"


 extern uint32_t R_ISM_PortEnable( uint8_t UnitNumber_u08,
                                         uint8_t ChannelNumber_u08 );
 extern uint32_t R_ISM_PortDisable( uint8_t UnitNumber_u08,
                                          uint8_t ChannelNumber_u08 );
 extern uint32_t R_ISM_CreateInterrupt( uint8_t  UnitNumber_u08,
                                              uint8_t  IntNumber_u08,
                                              uint16_t  SetIntLevel_u08,
                                              void ( *FunctionVector )( ) );
 extern uint32_t R_ISM_SetTimebase( uint8_t UnitNumber_u08,
                                          float PWMFrequency_flt,
                                          float CMFrequency_flt );
 extern uint32_t R_ISM_GetTimebase( uint8_t  UnitNumber_u08,
                                          uint32_t* PWMFrequency_pu32,
                                          uint32_t* CMFrequency_pu32 );
 extern uint32_t R_ISM_ReadTimebase( uint8_t  UnitNumber_u08,
                                           uint32_t* TimeBaseValue_pu32 );
 extern uint32_t R_ISM_SetChannelDelay( uint8_t UnitNumber_u08,
                                              uint8_t ChannelNumber_u08,
                                              uint8_t HorizontalDelay_u08,
                                              uint8_t VerticalDelay_u08 );
 extern uint32_t R_ISM_SetChannelType( uint8_t UnitNumber_u08,
                                             uint8_t ChannelNumber_u08,
                                             uint8_t ChannelType_u08,
                                             uint8_t ChannelPrecision_u08 );
 extern uint32_t R_ISM_GetChannelControl( uint8_t  UnitNumber_u08,
                                                uint8_t  ChannelNumber_u08,
                                                uint8_t* ChannelType_pu08,
                                                uint8_t* ChannelPrecision_pu08,
                                                uint8_t* HorizontalDelay_pu08,
                                                uint8_t* VerticalDelay_pu08 );
 extern uint32_t R_ISM_SetPWM( uint8_t UnitNumber_u08,
                                     uint8_t ChannelNumber_u08,
                                     uint16_t HorizontalDutyCycle_u16,
                                     uint16_t VerticalDutyCycle_u16,
                                     uint8_t QuadrantInfo_u08 );
 extern uint32_t R_ISM_GetPWM( uint8_t  UnitNumber_u08,
                                     uint8_t  ChannelNumber_u08,
                                     uint16_t* HorizontalDutyCycle_pu16,
                                     uint16_t* VerticalDutyCycle_pu16,
                                     uint8_t* QuadrantInfo_pu08 );
 extern uint32_t R_ISM_SetRecirculation( uint8_t UnitNumber_u08,
                                               uint8_t ChannelNumber_u08,
                                               uint8_t RecirculationEnableHorizontal_u08,
                                               uint8_t RecirculationEnableVertical_u08,
                                               uint8_t RecirculationSide_u08 );
 extern uint32_t R_ISM_GetRecirculation( uint8_t  UnitNumber_u08,
                                               uint8_t  ChannelNumber_u08,
                                               uint8_t* RecirculationEnableHorizontal_pu08,
                                               uint8_t* RecirculationEnableVertical_pu08,
                                               uint8_t* RecirculationSide_pu08 );
 extern uint32_t R_ISM_SetDirectIO( uint8_t UnitNumber_u08,
                                          uint8_t ChannelNumber_u08,
                                          uint8_t HorizontalDirect_u08,
                                          uint8_t VerticalDirect_u08 );
 extern uint32_t R_ISM_GetDirectIO( uint8_t UnitNumber_u08,
                                          uint8_t ChannelNumber_u08,
                                          uint8_t* HorizontalDirectEnable_pu08,
                                          uint8_t* HorizontalDirect_pu08,
                                          uint8_t* VerticalDirectEnable_pu08,
                                          uint8_t* VerticalDirect_pu08 );
 extern uint32_t R_ISM_ReleaseDirectIO( uint8_t UnitNumber_u08,
                                              uint8_t ChannelNumber_u08 );
 extern uint32_t R_ISM_SetCMEnable( uint8_t UnitNumber_u08,
                                          uint8_t ChannelMask_u08,
                                          uint8_t EnableFlag_u08 );
 extern uint32_t R_ISM_GetCMEnable( uint8_t  UnitNumber_u08,
                                          uint8_t* ChannelFlags_pu08,
                                          uint8_t* GlobalEnableFlag_pu08 );
 extern uint32_t R_ISM_SetInterrupt( uint8_t UnitNumber_u08,
                                           uint8_t ChannelMask_u08,
                                           uint8_t GlobalMask_u08,
                                           uint8_t ZPDADMask_u08,
                                           uint8_t EnableFlag_u08 );
 extern uint32_t R_ISM_GetInterrupt( uint8_t  UnitNumber_u08,
                                           uint8_t* ChannelFlags_pu08,
                                           uint8_t* GlobalEnableFlag_pu08,
                                           uint8_t* ZPDADEnableFlag_pu08 );
 extern uint32_t R_ISM_GetCMBusy( uint8_t  UnitNumber_u08,
                                        uint8_t* CMBusyFlag_pu08 );
 extern uint32_t R_ISM_SetCMVariables( uint8_t               UnitNumber_u08,
                                             uint8_t               ChannelNumber_u08,
                                             uint8_t               ResetAll_bit,
                                             struct rism_var_t *CMVars_p );
 extern uint32_t R_ISM_GetCMVariables( uint8_t               UnitNumber_u08,
                                             uint8_t               ChannelNumber_u08,
                                             struct rism_var_t *CMVars_p );
 extern uint32_t R_ISM_SetCMParameters( uint8_t                 UnitNumber_u08,
                                              uint8_t                 ChannelNumber_u08,
                                              struct r_ism_motor_t *ConfigSet_p );
 extern uint32_t R_ISM_SetCMTargetPos( uint8_t UnitNumber_u08,
                                             uint8_t ChannelNumber_u08,
                                             uint32_t TargetPosition_u32 );
 extern uint32_t R_ISM_GetCMTargetPos( uint8_t  UnitNumber_u08,
                                             uint8_t  ChannelNumber_u08,
                                             uint32_t* TargetPosition_pu32 );
 extern uint32_t R_ISM_SetCMPWMTable( uint8_t                        UnitNumber_u08,
                                            uint8_t                        ChannelNumber_u08,
                                            const struct rism_pwmstd_t *ConfigSetStd_p,
                                            const struct rism_pwmhi_t  *ConfigSetHi_p );
 extern uint32_t R_ISM_Reset( uint8_t UnitNumber_u08 );
 extern uint32_t R_ISM_SetZPDGlobals( uint8_t UnitNumber_u08,
                                                 float ZPDPeriod_flt,
                                                 uint8_t ZPDSuspendClocks_u08,
                                                 uint8_t ZPDFilterLength_u08,
                                                 uint8_t ZPDFilterDepth_u08,
                                                 uint8_t ZPDAnalogLevel_u08,
                                                 uint8_t ZPDAnalogSelection_u08,
                                                 uint8_t ZPDADConnect_u08 );
 extern uint32_t R_ISM_GetZPDGlobals( uint8_t  UnitNumber_u08,
                                            uint8_t  ZPDAnalogSelection_u08,
                                            uint32_t* ZPDPeriod_pu32,
                                            uint8_t* ZPDSuspendClocks_pu08,
                                            uint8_t* ZPDFilterLength_pu08,
                                            uint8_t* ZPDFilterDepth_pu08,
                                            uint8_t* ZPDAnalogLevel_pu08,
                                            uint8_t* ZPDADConnect_pu08 );
 extern uint32_t R_ISM_SetZPDChannels( uint8_t UnitNumber_u08,
                                             uint8_t ChannelNumber_u08,
                                             uint8_t AnalogLevelSelection_u08,
                                             uint16_t BlankingDelay_u16,
                                             uint8_t VibrationDamping_u08 );
 extern uint32_t R_ISM_GetZPDChannels( uint8_t  UnitNumber_u08,
                                             uint8_t  ChannelNumber_u08,
                                             uint8_t* AnalogLevelSelection_pu08,
                                             uint16_t* BlankingDelay_pu16,
                                             uint8_t* VibrationDamping_pu08 );
 extern uint32_t R_ISM_StartZPD( uint8_t UnitNumber_u08,
                                       uint8_t ChannelNumber_u08 );
 extern uint32_t R_ISM_ResetZPD( uint8_t UnitNumber_u08,
                                       uint8_t ChannelNumber_u08 );
 extern uint32_t R_ISM_SetEnableZPD( uint8_t UnitNumber_u08,
                                           uint8_t ZPDEnable_u08,
                                           uint8_t ZPDPowerSave_u08 );
 extern uint32_t R_ISM_GetEnableZPD( uint8_t  UnitNumber_u08,
                                           uint8_t* ZPDEnable_pu08,
                                           uint8_t* ZPDPowerSave_pu08 );
 extern uint32_t R_ISM_OperateZPDDirect( uint8_t UnitNumber_u08,
                                               uint8_t ChannelMask_u08,
                                               uint8_t InputSelection_u08,
                                               uint8_t StartStopFlag_u08 );
 extern uint32_t R_ISM_ReadZPDStatus( uint8_t  UnitNumber_u08,
                                            uint8_t  ChannelNumber_u08,
                                            uint8_t* ZPDActive_pu08,
                                            uint8_t* ZPDDelayState_pu08,
                                            uint8_t* ZPDTableIndex_pu08,
                                            uint8_t* ZPDMeasureActive_pu08,
                                            uint8_t* ZPDCurrentResult_pu08,
                                            uint8_t* ZPDAccumulatedResult_pu08 );
 extern uint32_t R_ISM_SetCMZPDTable( uint8_t                         UnitNumber_u08,
                                            uint8_t                         ChannelNumber_u08,
                                            const struct rism_zpdtcfg_t *ZPDCmdSet_p );

  /* Interrupt flags of driver */

  extern uint8_t *R_ISM_Interrupt_DONE_bit;  
  extern uint8_t R_ISM_Interrupt_REACHED_u08[];
  extern uint8_t R_ISM_Interrupt_REACHED_ACK_u08[];
  extern uint8_t *R_ISM_Interrupt_ZPDAD_bit;
  extern uint8_t *R_ISM_Interrupt_ZPD_bit;

#ifdef __cplusplus
}
#endif

#endif

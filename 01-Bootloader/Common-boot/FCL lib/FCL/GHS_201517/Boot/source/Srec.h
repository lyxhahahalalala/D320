/*******************************************************************************
* Freescale Semiconductor Inc.
* (c) Copyright 2004-2005 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
********************************************************************************
Services performed by FREESCALE in this matter are performed AS IS and without 
any warranty. CUSTOMER retains the final decision relative to the total design 
and functionality of the end product. FREESCALE neither guarantees nor will be 
held liable by CUSTOMER for the success of this project.
FREESCALE DISCLAIMS ALL WARRANTIES, EXPRESSED, IMPLIED OR STATUTORY INCLUDING, 
BUT NOT LIMITED TO, IMPLIED WARRANTY OF MERCHANTABILITY OR FITNESS FOR 
A PARTICULAR PURPOSE ON ANY HARDWARE, SOFTWARE ORE ADVISE SUPPLIED TO THE PROJECT
BY FREESCALE, AND OR NAY PRODUCT RESULTING FROM FREESCALE SERVICES. IN NO EVENT
SHALL FREESCALE BE LIABLE FOR INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF 
THIS AGREEMENT.

CUSTOMER agrees to hold FREESCALE harmless against any and all claims demands or 
actions by anyone on account of any damage, or injury, whether commercial, 
contractual, or tortuous, rising directly or indirectly as a result of an advise
or assistance supplied CUSTOMER in connection with product, services or goods 
supplied under this Agreement.
********************************************************************************
* File      Srec.h
* Owner     b01802
* Version   1.0
* Date      Feb-24-2010
* Classification   General Business Information
* Brief     Srecord utilities
********************************************************************************
* Detailed Description:
********************************************************************************
Revision History:
Version  Date          Author    Description of Changes
1.0      Feb-24-2010   b01802    Initial version
*******************************************************************************/
#ifndef _UTILITIES_H /* Prevent duplicated includes */
#define _UTILITIES_H

#define DataRec 1
#define EndRec 2
#define HeaderRec 3
typedef enum _ERROR_TYPE {
              noErr = 0,
              SRecRangeError = 1,      //S-Record Out Of Range
              SRecOddError = 2,        //S-Record Size Must Be Even
              FlashProgramError = 3,  //Flash Programming Error
              FlashEraseError = 4,    //Flash Erase Error
              BadHexData = 5,          //Bad Hex Data
              SRecTooLong = 6,        //S-Record Too Long
              CheckSumErr = 7,        //Checksum Error
              UnknownPartID = 8       //Unknown Part ID
            }ERROR_TYPE;

#define MaxSRecLen 1024 
//128
extern int scont2;
typedef struct {
                unsigned char RecType;
                unsigned char NumBytes;
                unsigned long LoadAddr;
                unsigned char Data[MaxSRecLen];
               } SRecDataRec;
#define RESET_VEC_SRC       (0x03FFE0ul)
#define RESET_VEC_DST       (0x03EFF8ul)

extern unsigned char RcvSRecord(SRecDataRec *SRecData);
extern unsigned char ProgramFlash(void);
#endif
/******************************************************************************/

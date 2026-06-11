/*
****************************************************************************
PROJECT :  Vlib device driver
FILE    : $Id: r_DEV_ERR_types.h 1943 2014-01-22 12:03:06Z matthias.nippert $
============================================================================ 
DESCRIPTION
Error Vector List for ECM macro
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

#ifndef DEV_ERR_TYPES_H_
#define DEV_ERR_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
  Title: Device sopecific interrupt types

*/

/*******************************************************************************
  Section: Global Types
*/

/*******************************************************************************
  Enum: r_dev_EcmErrSel_t

  All possible error control module interrupt sources .

*/
typedef enum 
{
R_DEV_ECMERR_FIRST  = 0,
R_DEV_ECMERR_WDTA0  = R_DEV_ECMERR_FIRST,
R_DEV_ECMERR_WDTA1,
R_DEV_ECMERR_NMI,
R_DEV_ECMERR_E4, /*Reserved*/
R_DEV_ECMERR_ECCDFLI,
R_DEV_ECMERR_ECCSFLI,      /*  5 */
R_DEV_ECMERR_ECCDEDEEP,
R_DEV_ECMERR_ECCSEDEEP,
R_DEV_ECMERR_ECCIC,
R_DEV_ECMERR_ECCXCC,
R_DEV_ECMERR_VOCERR,       /* 10 */
R_DEV_ECMERR_PERIGRD,
R_DEV_ECMERR_PEGRD,
R_DEV_ECMERR_VDCE0ERR,
R_DEV_ECMERR_VDCE1ERR,
R_DEV_ECMERR_ECCDLRAM,     /* 15 */
R_DEV_ECMERR_ECCSLRAM,
R_DEV_ECMERR_E17, /*Reserved*/
R_DEV_ECMERR_E18, /*Reserved*/
R_DEV_ECMERR_ECCDEDRRAM,
R_DEV_ECMERR_ECCSEDRRAM,   /* 20 */
R_DEV_ECMERR_E21,
R_DEV_ECMERR_ECCDPERIRAM,
R_DEV_ECMERR_ECCSPERIRAM,
R_DEV_ECMERR_ECCDEDVRAM,
R_DEV_ECMERR_ECCSEDVRAM,   /* 25 */
R_DEV_ECMERR_OSTM1,
R_DEV_ECMERR_E27, /*Reserved*/
R_DEV_ECMERR_CLMATI1,
R_DEV_ECMERR_CLMATI2,
R_DEV_ECMERR_CLMATI3,      /* 30 */
R_DEV_ECMERR_CLMATI4,
R_DEV_ECMERR_CLMATI5,
R_DEV_ECMERR_CLMATI6,
R_DEV_ECMERR_SG0DIAG,
R_DEV_ECMERR_PCMP0DIAG,    /* 35 */
R_DEV_ECMERR_TAUBDIAG,
R_DEV_ECMERR_ECM,
R_DEV_ECMERR_E38, /*Reserved*/
R_DEV_ECMERR_E39, /*Reserved*/
R_DEV_ECMERR_ADCEERR,      /* 40 */
R_DEV_ECMERR_ISM0ZPD,
/* delimiter, do not remove !!! */
R_DEV_ECMERR_LAST

} r_dev_EcmErrSel_t;


#ifdef __cplusplus
}
#endif

#endif /* DEV_ERR_TYPES_H_  */

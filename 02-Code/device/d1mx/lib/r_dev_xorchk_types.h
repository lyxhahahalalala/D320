/*
****************************************************************************
PROJECT :  Vlib device driver
FILE    : $Id: r_dev_xorchk_types.h 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
Driver for xor check unit functionality
============================================================================
                            C O P Y R I G H T
============================================================================
                       Copyright (c) 2014
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

#ifndef DEV_XORCHK_TYPES_H_
#define DEV_XORCHK_TYPES_H_

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
  Enum: r_dev_XorchkFunc_t

  All possible Xor unit check functions.

*/
typedef enum 
{
    /* starter, do not remove !!! */
	R_XORCHK_FUNC_FIRST         = 0,
	
	R_XORCHK_FUNC_TAUB0O1       = R_XORCHK_FUNC_FIRST,
    R_XORCHK_FUNC_TAUB0O3       ,
    R_XORCHK_FUNC_TAUB0O5       ,
    R_XORCHK_FUNC_TAUB0O7       ,
    R_XORCHK_FUNC_TAUB0O9       ,
    R_XORCHK_FUNC_TAUB0O11      ,
    R_XORCHK_FUNC_TAUB0O13      ,
    R_XORCHK_FUNC_TAUB1O1       ,
    R_XORCHK_FUNC_TAUB1O3       ,
    R_XORCHK_FUNC_TAUB1O5       ,
    R_XORCHK_FUNC_TAUB1O7       ,
    R_XORCHK_FUNC_TAUB1O9       ,	
	R_XORCHK_FUNC_PCMP0AP0      ,
	R_XORCHK_FUNC_PCMP0AN0      ,	
	R_XORCHK_FUNC_PCMP0BP0      ,
	R_XORCHK_FUNC_PCMP0BN0      ,
	R_XORCHK_FUNC_PCMP0AP1      ,
	R_XORCHK_FUNC_PCMP0AN1      ,
	R_XORCHK_FUNC_PCMP0BP1      ,
	R_XORCHK_FUNC_PCMP0BN1      ,
	R_XORCHK_FUNC_SG0AO         ,
	R_XORCHK_FUNC_SG0FAOL       ,
	R_XORCHK_FUNC_SG0FAO        ,

    /* delimiter, do not remove !!! */
    R_DEV_XORCHK_FUNC_LAST
} r_dev_XorchkFunc_t;


#ifdef __cplusplus
}
#endif

#endif /* DEV_XORCHK_TYPES_H_  */

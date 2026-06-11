/*
****************************************************************************
PROJECT : WM
FILE    : $Id: r_config_wm.h 7402 2016-01-27 15:43:39Z florian.zimmermann $
============================================================================ 
DESCRIPTION
WM configuration
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
****************************************************************************
*/

#ifndef R_CONFIG_WM_H
#define R_CONFIG_WM_H


/***********************************************************
  Title: config_wm.h
  
  D1Mx Mango board WM Configuration
*/

/***********************************************************
  Section: Global Defines
************************************************************/

/***********************************************************
  Constant: MAX_LAYER_*

  default max dimensions given by VDCE on D1Mx
  R_WM_MAX_LAYER_PITCH  - 4096
  R_WM_MAX_LAYER_WIDTH  - 2048
  R_WM_MAX_LAYER_HEIGHT - 2048
*/

#define R_WM_MAX_LAYER_PITCH   2048
#define R_WM_MAX_LAYER_WIDTH   1024
#define R_WM_MAX_LAYER_HEIGHT  1024


/***********************************************************
  Constant: R_WM_DEV_NUM

  Number of VOUT (WM) units
*/


#define R_WM_DEV_NUM         2

#endif /* R_CONFIG_WM_H_ */

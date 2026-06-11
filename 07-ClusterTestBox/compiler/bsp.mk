# ****************************************************************************
# PROJECT : VLIB BSP
# FILE    : $Id: bsp.mk 4884 2015-03-10 13:00:06Z golczewskim $
# ============================================================================ 
# DESCRIPTION
# STDIO driver makefile 
# ============================================================================
#                             C O P Y R I G H T                                    
# ============================================================================
#                            Copyright (c) 2014
#                                   by 
#                        Renesas Electronics (Europe) GmbH. 
#                            Arcadiastrasse 10
#                           D-40472 Duesseldorf
#                                Germany
#                           All rights reserved.
# ============================================================================
# Purpose: only for testing
# 
# DISCLAIMER                                                                   
# This software is supplied by Renesas Electronics Corporation and is only     
# intended for use with Renesas products. No other uses are authorized. This   
# software is owned by Renesas Electronics Corporation and is protected under  
# all applicable laws, including copyright laws.                               
# THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING  
# THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT      
# LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE   
# AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.          
# TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS       
# ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE  
# FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR   
# ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE  
# BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.                             
# Renesas reserves the right, without notice, to make changes to this software 
# and to discontinue the availability of this software. By using this software,
# you agree to the additional terms and conditions found by accessing the      
# following link:                                                              
# http://www.renesas.com/disclaimer *                                          
# Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.     
# ****************************************************************************



#
# Include path
#

VLIB_INC 		+= $(VLIB_ROOT)/bsp/$(VLIB_BSP_DRIVER_NAME)/lib
VLIB_INC 		+= $(VLIB_ROOT)/bsp/board/$(VLIB_BOARD)/config
VLIB_INC 		+= $(VLIB_ROOT)/bsp/board/$(VLIB_BOARD)/src/$(notdir $(VLIB_BSP_DRIVER_NAME))


#
# Source code search path
#

VLIB_VPATH		+= $(VLIB_ROOT)/bsp/$(VLIB_BSP_DRIVER_NAME)/src
VLIB_VPATH		+= $(VLIB_ROOT)/bsp/board/$(VLIB_BOARD)/src/$(notdir $(VLIB_BSP_DRIVER_NAME))


#
# Common BSP part 
#

VLIB_INC 		+= $(VLIB_ROOT)/bsp/board/$(VLIB_BOARD_COMMON)/config
VLIB_INC 		+= $(VLIB_ROOT)/bsp/board/$(VLIB_BOARD_COMMON)/src/$(notdir $(VLIB_BSP_DRIVER_NAME))
VLIB_VPATH		+= $(VLIB_ROOT)/bsp/board/$(VLIB_BOARD_COMMON)/src/$(notdir $(VLIB_BSP_DRIVER_NAME))

#
# Defines
#

UPPER_CASE := $(shell echo $(notdir $(VLIB_BSP_DRIVER_NAME)) | tr a-z A-Z)
VLIB_DEFINE	+= USE_BSP_$(UPPER_CASE)


# ****************************************************************************
# PROJECT : VLIB toolbox
# FILE    : $Id: drw2d.mk 4829 2015-03-02 10:10:34Z florian.zimmermann $
# AUTHOR  : Michael Golczewski $
# ============================================================================ 
# DESCRIPTION
# debug driver makefile
# ============================================================================
#                             C O P Y R I G H T                                    
# ============================================================================
#                            Copyright (c) 2012
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

VLIB_MACRO_NAME:=drw2d
VLIB_USE_CDI 	:= yes
VLIB_INC += $(VLIB_ROOT)/macro/drw2d/lib

ifeq (,$(findstring USE_DRW2D_OGLES,$(VLIB_DEFINE)))
	ifeq ($(VLIB_DEVICE), d1lx)
	    VLIB_DEFINE += R_DRW2D_SYS_CPU
		VLIB_INC += $(VLIB_ROOT)/macro/drw2d/platform/cpu
		ifeq ($(VLIB_DRIVER_CONFIG),rgl_src)
			VLIB_VPATH += $(VLIB_ROOT)/macro/drw2d/platform/cpu
		else
			VLIB_LIB_VPATH += $(VLIB_ROOT)/macro/drw2d/platform/cpu
		endif
	else
		ifeq ($(VLIB_DEVICE), d1mx)
			VLIB_DEFINE += R_DRW2D_SYS_DHD
			VLIB_INC += $(VLIB_ROOT)/macro/drw2d/platform/davehd
			ifeq ($(VLIB_DRIVER_CONFIG),rgl_src)
				VLIB_VPATH += $(VLIB_ROOT)/macro/drw2d/platform/davehd
			else
				VLIB_LIB_VPATH += $(VLIB_ROOT)/macro/drw2d/platform/davehd
			endif
		endif
	endif
endif

# OS abstraction is always outside the library
ifneq (,$(findstring USE_ROS,$(VLIB_DEFINE)))
VLIB_VPATH  += $(VLIB_ROOT)/macro/drw2d/platform/os/ros
VLIB_DEFINE += R_DRW2D_OS_RENESAS
else
VLIB_VPATH += $(VLIB_ROOT)/macro/drw2d/platform/os/no_os
VLIB_DEFINE += R_DRW2D_OS_NO_OS
endif

include $(VLIB_ROOT)/compiler/macro.mk


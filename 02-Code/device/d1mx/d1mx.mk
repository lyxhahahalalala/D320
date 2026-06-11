# ****************************************************************************
# PROJECT : VLIB d1mx
# FILE    : $Id:  $
# ============================================================================ 
# DESCRIPTION
# r7f701412 driver makefile locations
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

# What CPU is used by this device 
ifeq ($(VLIB_COMPILER),rh850_iar)
	VLIB_CPU := v850e2m
else
	VLIB_CPU := rh850g3m
#	VLIB_CPU := rh850
endif
VLIB_COMPILER_USE_SDA := yes

#dma and gfx bus have a common API for all devices, just the sources differ 
VLIB_DMA      := g3_dma
VLIB_GFX_BUS  := xbus

VLIB_DEVICE_COMMON := d1x_common

#add device code (which has no macro level implementation 
VLIB_VPATH	+= $(VLIB_ROOT)/device/$(VLIB_DEVICE)/src/dev


include $(VLIB_ROOT)/device/$(VLIB_DEVICE)/config_$(VLIB_DRIVER_CONFIG).mk



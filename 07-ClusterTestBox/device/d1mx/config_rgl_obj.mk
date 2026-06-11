# ****************************************************************************
# PROJECT : VLIB
# FILE    : $Id: config_rgl_obj.mk 3727 2014-09-03 09:56:20Z florian.zimmermann $
# ============================================================================ 
# DESCRIPTION
# V-Lib RGL driver set
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
# Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.     
# ****************************************************************************

ifeq ($(VLIB_SRC_MACROS),)
ifeq ($(VLIB_OBJ_MACROS),)


VLIB_SRC_MACROS := gpio
VLIB_SRC_MACROS += timer/ostm
VLIB_SRC_MACROS += timer/tauj
VLIB_SRC_MACROS += tick
VLIB_SRC_MACROS += csi/csisw
VLIB_SRC_MACROS += gfxbus
VLIB_SRC_MACROS += gfxbus/xbus
VLIB_SRC_MACROS += bus/pbg
VLIB_SRC_MACROS += flashc/sfcdb
VLIB_SRC_MACROS += i2c/riic
VLIB_SRC_MACROS += vo/ddb

VLIB_OBJ_MACROS := vo/vowe
VLIB_OBJ_MACROS += vo/spea
VLIB_OBJ_MACROS += vdce
VLIB_OBJ_MACROS += wm
VLIB_OBJ_MACROS += drw2d
VLIB_OBJ_MACROS += gpu/davehd
VLIB_OBJ_MACROS += gpu/jcua
VLIB_OBJ_MACROS += flashc/sfma

ifeq ($(findstring R_DBG_,$(VLIB_DEFINE)),R_DBG_)
VLIB_SRC_MACROS += dbg
endif



endif
endif

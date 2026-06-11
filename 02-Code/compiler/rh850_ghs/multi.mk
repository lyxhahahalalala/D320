# ****************************************************************************
# PROJECT : VLIB
# FILE    : $Id: multi.mk 877 2013-05-22 11:25:53Z florian.zimmermann $
# ============================================================================ 
# DESCRIPTION
# Multi project generator for V800
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

.PHONY : multi

#
# Generate relative path to VLIB root from target directory
#

TMP0 = $(subst /, /,$(word 2,$(subst vlib,vlib ,$(shell pwd))))
TMP1 = $(wordlist 2, $(words $(TMP0)),$(TMP0))
VLIB_RELPATH = $(firstword $(subst / ,/, $(foreach DIR,$(TMP1),../))..)
TMP2 = $(subst /, /,$(word 2,$(subst /app/,/app /,$(shell pwd))))
TMP3 = $(wordlist 2, $(words $(TMP2)),$(TMP2))
APP_RELPATH  = $(firstword $(subst / ,/, $(foreach DIR,$(TMP3),../))..)

#
# Multi project name
#

MULTI_NAME = $(subst .out,.gpj, $(VLIB_APP))

#
# Multi subproject name
#
MULTI_DRV = ./drivers.gpj
MULTI_APP = ./app.gpj


# For Muti.mk
SRC_PATH   := $(foreach DIR,$(sort $(VPATH)),-I$(DIR))
SRC_PATH   += $(VLIB_INCOPT)
#
# Change path in options to relative path
#

TMP4 = $(subst $(VLIB_ROOT),$(VLIB_RELPATH),$(VLIB_CFLAGS) $(sort $(SRC_PATH)) $(VLIB_ASFLAGS) $(VLIB_LDFLAGS))

#
# Remove option "stop with object" (-c), sort list and remove
# double options (caused by combining ASFLAGS and CFLAGS)
#
TMP5 = $(subst --diag_warning 1, ,$(TMP4))

MULTI_FLAGS = $(sort $(subst -c , ,$(TMP5)))


#
# Change path in files to relative path
#

TMP6 =  $(subst $(VLIB_ROOT)/app,".",$(VLIB_SRCS))
TMP7 :=
ifeq ($(VLIB_DRIVER_CONFIG),rgl_src)
  TMP7 :=  $(subst $(VLIB_ROOT)/app,".",$(VLIB_LIB_SRCS))
endif

TMP8 = $(TMP6) $(TMP7)

DRV_FILES += $(foreach FILE, $(TMP8), $(if $(findstring cygdrive,$(FILE)), $(subst $(VLIB_ROOT),".",$(FILE))))
DRV_FILES += $(subst $(VLIB_ROOT),".",$(VLIB_LIBS) $(VLIB_LDSCRIPT))
APP_FILES += $(foreach FILE, $(TMP6), $(if $(findstring cygdrive,$(FILE)), ,$(FILE)))


multi: $(LIBS)
	@echo -n "Generating driver subproject: "
	@echo "#!gbuild" > $(MULTI_DRV)
	@echo [Subproject] >> $(MULTI_DRV)
	@echo -e "\t:sourceDir=$(VLIB_RELPATH)" >> $(MULTI_DRV)
	@for i in $(DRV_FILES); do \
	  echo "$$i" >> $(MULTI_DRV); \
	done
	@echo -n "Generating app subproject: "
	@echo "#!gbuild" > $(MULTI_APP)
	@echo [Subproject] >> $(MULTI_APP)
	@echo -e "\t:sourceDir=$(APP_RELPATH)" >> $(MULTI_APP)
	@for i in $(APP_FILES); do \
	  echo "$$i" >> $(MULTI_APP); \
	done
	@echo -n "Generating MULTI project file: "
	@echo "#!gbuild" > $(MULTI_NAME)
	@echo primaryTarget=v800_standalone.tgt >> $(MULTI_NAME)
	@echo [Program] >> $(MULTI_NAME)
	@for i in $(MULTI_FLAGS); do \
	  echo -e "\t$$i" >> $(MULTI_NAME); \
	done
	@echo -e "\t--diag_warning 1" >> $(MULTI_NAME)
	@echo -e "\t-object_dir=./obj" >> $(MULTI_NAME)
	@echo -e "\t-o $(VLIB_APP)" >> $(MULTI_NAME)
	@echo $(MULTI_DRV) >> $(MULTI_NAME)
	@echo $(MULTI_APP) >> $(MULTI_NAME)
#	@for i in $(MULTI_FILES); do \
#	  echo "$$i" >> $(MULTI_NAME); \
#	done
	@echo "Done"

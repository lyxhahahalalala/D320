# ****************************************************************************
# PROJECT : VLIB
# FILE    : $Id: tag.mk 100 2012-08-21 15:33:53Z golczewskim $
# ============================================================================ 
# DESCRIPTION
# Ship project generator for V800
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


#
# $Log: tag.mk,v $
# Revision 1.5  2008/01/18 10:13:26  golczewskim
# Now the error handling works properly
#
# Revision 1.4  2008/01/18 08:23:53  golczewskim
# removed some #
#
# Revision 1.3  2008/01/18 08:21:52  golczewskim
# Added error handling for missing TAG name
#
# Revision 1.2  2007/09/17 07:02:29  golczewskim
# now replacing // by /  -> eliminate cvs error warning
#
# Revision 1.1  2007/09/14 11:41:29  lemetteilm
# Initiale release
#
#

TARGET_DIR      = $(shell pwd)
MAKE_FILES_FULL = $(subst Makefile,$(TARGET_DIR)/Makefile,$(MAKE_FILES))
MAKEFILE_DIRS   = $(foreach FILE,$(MAKE_FILES_FULL),$(dir $(FILE)))
LDSCRIPT_DIRS   = $(foreach FILE,$(LDSCRIPT),$(dir $(FILE)))
#SHIP_FILES      = $(TARGET_DIR)/$(subst .out,.gpj,$(APP))
#SHIP_FILES     += $(TARGET_DIR)/$(APP).rc


.PHONY : cvs_tag

cvs_tag :

	@echo cd $(VLIB_ROOT) > ./maketag.sh	
	@echo -n "Adding tag $(TAG) to all files in project.. "
	@echo "" >> ./maketag.sh
	
	@for i in $(INC); do \
	  echo -n "cvs tag $(TAG) " >> ./maketag.sh; \
	  echo "$$i" | sed '{s/\/\//\//;}' | sed '{s/.*vlib\///;}' >> ./maketag.sh; \
	done
	
	@for i in $(MAKEFILE_DIRS); do \
	  echo -n "cvs tag $(TAG) " >> ./maketag.sh; \
	  echo "$$i" | sed '{s/\/\//\//;}' | sed '{s/.*vlib\///;}' >> ./maketag.sh; \
	done
	
	@for i in $(LDSCRIPT_DIRS); do \
	  echo -n "cvs tag $(TAG) " >> ./maketag.sh; \
	  echo "$$i" | sed '{s/\/\//\//;}' | sed '{s/.*vlib\///;}' >> ./maketag.sh; \
	done
	
	@for i in $(LDSCRIPT); do \
	  echo -n "cvs tag $(TAG) " >> ./maketag.sh; \
	  echo "$$i" | sed '{s/\/\//\//;}' | sed '{s/.*vlib\///;}' >> ./maketag.sh; \
	done
	
# the tag name may not be empty !
ifneq ($(TAG),)
		@chmod 755 ./maketag.sh
		@./maketag.sh
		@echo "Done"	
else
		@echo "*** Missing TAG name -> nothing done! ***"
endif
	@rm ./maketag.sh
	
	

.PHONY : rem_cvs_tag

rem_cvs_tag :
	@echo cd $(VLIB_ROOT) > ./maketag.sh	
	@echo -n "Removing tag $(TAG) to all files in project.. "
	@echo "" >> ./maketag.sh
	
	@for i in $(INC); do \
	  echo -n "cvs tag -d $(TAG) " >> ./maketag.sh; \
	  echo "$$i" | sed '{s/\/\//\//;}' | sed '{s/.*vlib\///;}' >> ./maketag.sh; \
	done
	
	@for i in $(MAKEFILE_DIRS); do \
	  echo -n "cvs tag -d $(TAG) " >> ./maketag.sh; \
	  echo "$$i" | sed '{s/\/\//\//;}' | sed '{s/.*vlib\///;}' >> ./maketag.sh; \
	done
	
	@for i in $(LDSCRIPT_DIRS); do \
	  echo -n "cvs tag -d $(TAG) " >> ./maketag.sh; \
	  echo "$$i" | sed '{s/\/\//\//;}' | sed '{s/.*vlib\///;}' >> ./maketag.sh; \
	done
	
	@for i in $(LDSCRIPT); do \
	  echo -n "cvs tag -d $(TAG) " >> ./maketag.sh; \
	  echo "$$i" | sed '{s/\/\//\//;}' | sed '{s/.*vlib\///;}' >> ./maketag.sh; \
	done

# the tag name may not be empty !
ifneq ($(TAG),)
		@chmod 755 ./maketag.sh
		@./maketag.sh
		@echo "Done"	
else
		@echo "*** Missing TAG name -> nothing done! ***"
endif
	@rm ./maketag.sh

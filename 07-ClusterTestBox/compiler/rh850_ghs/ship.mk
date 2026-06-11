# ****************************************************************************
# PROJECT : VLIB
# FILE    : $Id: ship.mk 7342 2016-01-25 09:21:40Z matthias.nippert $
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


SHIP_TARGET_DIR      = $(shell pwd)
SHIP_MAKE_FILES_FULL = $(subst Makefile,$(SHIP_TARGET_DIR)/Makefile,$(VLIB_MAKE_FILES))
SHIP_MAKEFILE_DIRS   = $(foreach FILE,$(SHIP_MAKE_FILES_FULL),$(dir $(FILE)))
SHIP_LF_DIR          = $(foreach FILE,$(VLIB_LDSCRIPT),$(dir $(FILE)))
SHIP_FILES           = $(SHIP_TARGET_DIR)/$(subst .out,.gpj,$(VLIB_APP))
SHIP_FILES          += $(SHIP_TARGET_DIR)/$(VLIB_APP).rc
SHIP_FILES          += $(SHIP_TARGET_DIR)/drivers.gpj
SHIP_FILES          += $(SHIP_TARGET_DIR)/app.gpj
SHIP_LIBS_DIRS       = $(foreach FILE,$(VLIB_LIBS),$(dir $(FILE)))


MULTI_NAME_=$(subst ' ',,$(MULTI_NAME))

.PHONY : ship

ship :  multi
	@echo "Making sure all objects are up to date: "
	@make
	@echo "Done"
	@echo -n "Generating shipping directory production script: "
	@echo "#!/bin/bash" > ./makeship.sh
	@echo "shopt -s extglob" >> ./makeship.sh
	@echo "date=\$$(date -u '+%Y%m%d')" >> ./makeship.sh
	@echo "svnRevision=\$$(svn info 2>&1|grep --color=never Revision|sed 's/Revision:\ /_svnRev_/g')" >> ./makeship.sh
	@echo "svnBranch=\$$(svn info | grep --color=never Relative | grep --color=never -o 'branches/\([^/]*\)' | sed 's/branches\//_svnBranch_/g')" >> ./makeship.sh
	@for i in $(VLIB_VPATH); do \
	  if [ -e "$$i" ]; then \
	  echo -n "mkdir -p " >> ./makeship.sh; \
	  echo "$$i" | sed '{s/.*\/vlib/.\/ship\/vlib/;}' >> ./makeship.sh; \
	  echo -n "cp -p $$i/!(*.a) " >> ./makeship.sh; \
	  echo "$$i" | sed '{s/.*\/vlib/.\/ship\/vlib/;}' >> ./makeship.sh; \
	  fi \
	done
	@for i in $(VLIB_INC); do \
	  echo -n "mkdir -p " >> ./makeship.sh; \
	  echo "$$i" | sed '{s/.*\/vlib/.\/ship\/vlib/;}' >> ./makeship.sh; \
	  echo -n "cp -p $$i/!(*.a) " >> ./makeship.sh; \
	  echo "$$i" | sed '{s/.*\/vlib/.\/ship\/vlib/;}' >> ./makeship.sh; \
	done
	@for i in $(SHIP_MAKEFILE_DIRS); do \
	  echo -n "mkdir -p " >> ./makeship.sh; \
	  echo "$$i" | sed '{s/.*\/vlib/.\/ship\/vlib/;}' >> ./makeship.sh; \
	done
	@for i in $(SHIP_MAKE_FILES_FULL); do \
	  echo -n "cp -p $$i " >> ./makeship.sh; \
	  echo "$$i" | sed '{s/.*\/vlib/.\/ship\/vlib/;}' >> ./makeship.sh; \
	done
	@for i in $(SHIP_LF_DIR); do \
	  echo -n "mkdir -p " >> ./makeship.sh; \
	  echo "$$i" | sed '{s/.*\/vlib/.\/ship\/vlib/;}' >> ./makeship.sh; \
	done
	@for i in $(VLIB_LDSCRIPT); do \
	  echo -n "cp -p $$i " >> ./makeship.sh; \
	  echo "$$i" | sed '{s/.*\/vlib/.\/ship\/vlib/;}' >> ./makeship.sh; \
	done
	@for i in $(SHIP_LIBS_DIRS); do \
	  echo -n "mkdir -p " >> ./makeship.sh; \
	  echo "$$i" | sed '{s/.*\/vlib/.\/ship\/vlib/;}' >> ./makeship.sh; \
	done
	@for i in $(VLIB_LIBS); do \
	  echo -n "cp -p $$i " >> ./makeship.sh; \
	  echo "$$i" | sed '{s/.*\/vlib/.\/ship\/vlib/;}' >> ./makeship.sh; \
	done
	@for i in $(SHIP_FILES); do \
	  if [ -e "$$i" ]; then \
	  echo -n "cp -p $$i " >> ./makeship.sh; \
	  echo "$$i" | sed '{s/.*\/vlib/.\/ship\/vlib/;}' >> ./makeship.sh; \
	  fi \
	done

    # remove sources when configured as object
	@for i in $(VLIB_LIB_SRCS); do \
	  if [ -e "$$i" ]; then \
	  echo -n "rm -f  " >> ./makeship.sh; \
	  echo "$$i" | sed '{s/.*\/vlib/.\/ship\/vlib/;}' >> ./makeship.sh; \
	  fi \
	done


# add all extra files from addmakeship.txt file to shipment
	@if test -f ./addmakeship.txt; \
	  then \
	    echo "Add files from addmakeship.txt to shipment."; \
		dos2unix addmakeship.txt; \
	    for i in $$(cat addmakeship.txt); do \
          NAME=$$(dirname ./ship/vlib/$$i); \
          echo "mkdir -p  $$NAME" >> ./makeship.sh; \
          echo "cp -p $(VLIB_ROOT)/$$i $$NAME" >> ./makeship.sh; \
        done \
	  else \
	    echo -n "Omitting addmakeship.txt, because file does not exist."; \
	fi
#
# add extra directories
#
	@if test -f ./add_directory_to_ship.txt; \
	  	then \
		echo "Add directories from add_directory_to_ship.txt to shipment."; \
		dos2unix add_directory_to_ship.txt; \
	    	for i in $$(cat add_directory_to_ship.txt); do \
			NAME=./ship/vlib/$$i; \
			echo "mkdir -p  $$NAME" >> ./makeship.sh; \
			echo "cp -rp $(VLIB_ROOT)/$$i/* $$NAME" >> ./makeship.sh; \
		done \
	fi
    

	@echo "cp $(VLIB_ROOT)/compiler/$(VLIB_COMPILER)/ghs_shortcut.bat $(SHIP_TARGET_DIR)/ship/vlib/get_ghs_shortcut.bat" >> ./makeship.sh
	@echo "cd ./ship" >> ./makeship.sh
	@echo 'for i in $$(find ./vlib -name "*~"); do rm $$i; done' >> ./makeship.sh
	@echo 'for i in $$(find ./vlib -name ".svn"); do rm -rf $$i; done' >> ./makeship.sh
	@echo 'find ./vlib -iname "*.bak" -delete' >> ./makeship.sh
	@echo "chmod -R 777 ./vlib" >> ./makeship.sh
#	@echo "echo \"SVN-Revision: \$$svnRevision\"" >> ./makeship.sh
	@echo "tar czf $(basename $(notdir $(VLIB_APP)))_\$${date}\$${svnRevision}\$${svnBranch}.tar.gz vlib" >> ./makeship.sh
	@echo 'if command -v 7z; then' >> ./makeship.sh
	@echo "rm -f $(basename $(notdir $(VLIB_APP)))_\$${date}\$${svnRevision}\$${svnBranch}.7z" >> ./makeship.sh
	@echo "7z a $(basename $(notdir $(VLIB_APP)))_\$${date}\$${svnRevision}\$${svnBranch}.7z vlib | grep -v 'Compressing'" >> ./makeship.sh
	@echo 'fi' >> ./makeship.sh
	@echo "cd .." >> ./makeship.sh
	@chmod 755 ./makeship.sh
	@rm -rf ./ship
	@echo "Done"
	@echo -n "Generate shipping file: "
	@./makeship.sh
	@echo "Done"

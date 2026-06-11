# ****************************************************************************
# PROJECT : VLIB
# FILE    : $Id: svn.mk 171 2012-09-11 11:41:06Z michael.golczewski $
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

# NOTE
# This Makefile uses a paramter QUIET
# if it is set to 1, the Tourtoise-SVN window will close automatically 
# (if there were no errors) 
# If the value of QUIET is 0 (or not set while calling the make) the SVN GUI 
# window will stay open waiting for confirmation. 

# local variables  

# for SVN we only need the directories of makefiles as starting point for a 
# SVN update for all project relevant files 
MAKE_FILES_FULL = $(subst Makefile,$(TARGET_DIR)/Makefile,$(MAKE_FILES))
MAKEFILE_DIRS   = $(foreach FILE,$(MAKE_FILES_FULL),$(dir $(FILE)))

# need a storage file for sorting 
FILES_TMPFILE:="./files.temp"

.PHONY : svnupdate

svnupdate : 
	@echo -n "Collecting directories ........."
	@echo -n  >$(FILES_TMPFILE)
	@echo $(MAKEFILE_DIRS) | xargs cygpath -m | sort | uniq |  awk '{printf"%s\n",$$0}' >> $(FILES_TMPFILE)
	@echo  "Done"

	@echo -n "Generating SVN update script ..." 
	@echo "" > ./svn_update.sh
	@echo "shopt -s extglob" > ./svn_update.sh
	@echo -n "TortoiseProc.exe /command:update /path:\""  >> ./svn_update.sh; 
	@cat $(FILES_TMPFILE) | sort | uniq |awk '{printf"%s*",$$0}'  >> ./svn_update.sh;
	@echo  "\" /closeonend:0" >> ./svn_update.sh;
	@echo "Done"

	@echo -n "Executing SVN update ..........."
	@chmod 755 ./svn_update.sh
	@./svn_update.sh
	@ rm $(FILES_TMPFILE)
	@ rm ./svn_update.sh
	@echo "Done"


#
# Include path
#
# check to exclude dave driver here 
ifneq ($(VLIB_MACRO_NAME),gpu/davehd)
VLIB_INC 		+= $(VLIB_ROOT)/macro/$(VLIB_MACRO_NAME)/lib

#add some OS specific incudes 
ifneq ($(VLIB_USE_OS),)
VLIB_INC		+= $(VLIB_ROOT)/macro/$(VLIB_MACRO_NAME)/$(VLIB_USE_OS)
endif

#
# Source code search path
#

ifeq ($(VLIB_MACRO_IN_LIB), yes)
ifneq ($(VLIB_USE_OS),)
VLIB_LIB_VPATH		+= $(VLIB_ROOT)/macro/$(VLIB_MACRO_NAME)/$(VLIB_USE_OS)
VLIB_LIB_VPATH		+= $(VLIB_ROOT)/macro/$(VLIB_MACRO_NAME)/$(VLIB_USE_OS)/driver
VLIB_LIB_VPATH		+= $(VLIB_ROOT)/macro/$(VLIB_MACRO_NAME)/$(VLIB_USE_OS)/usr
#VLIB_LIB_VPATH		+= $(VLIB_ROOT)/macro/$(VLIB_MACRO_NAME)/$(VLIB_USE_OS)/server
else
VLIB_LIB_VPATH		+= $(VLIB_ROOT)/macro/$(VLIB_MACRO_NAME)/src
endif
else
ifneq ($(VLIB_USE_OS),)
VLIB_VPATH		+= $(VLIB_ROOT)/macro/$(VLIB_MACRO_NAME)/$(VLIB_USE_OS)
VLIB_VPATH		+= $(VLIB_ROOT)/macro/$(VLIB_MACRO_NAME)/$(VLIB_USE_OS)/driver
VLIB_VPATH		+= $(VLIB_ROOT)/macro/$(VLIB_MACRO_NAME)/$(VLIB_USE_OS)/usr
#VLIB_VPATH		+= $(VLIB_ROOT)/macro/$(VLIB_MACRO_NAME)/$(VLIB_USE_OS)/server
else
VLIB_VPATH		+= $(VLIB_ROOT)/macro/$(VLIB_MACRO_NAME)/src
endif
endif

SYS_DIR := $(VLIB_ROOT)/device/$(VLIB_DEVICE)/src/$(notdir $(VLIB_MACRO_NAME)) 
ifneq ($(wildcard $(SYS_DIR)),)
VLIB_VPATH		+=  $(SYS_DIR) 
endif 

SYS_DIR := $(VLIB_ROOT)/device/$(VLIB_DEVICE_COMMON)/src/$(notdir $(VLIB_MACRO_NAME))
ifneq ($(wildcard $(SYS_DIR)),)
VLIB_VPATH		+=  $(SYS_DIR)
endif


endif # check to exclude dave driver here 

ifeq ($(VLIB_COMPILER),rh850_ghs)
#
# override rules for all r_ macros: use MISRA
#
ifneq ($(VLIB_MISRA),)
MISRA_MSG :="MISRA check enforced"
else
MISRA_MSG :="MISRA check disabled"
endif
r_%.$(VLIB_OBJ_EXT) : r_%.c
	@echo "Compiling ($(MISRA_MSG))" $(notdir $<)
	$(CXX_BUILD_MISRA)
	
#
# override rules for all fw_ code: use MISRA
#
fw_%.$(VLIB_OBJ_EXT) : fw_%.c
	@echo "Compiling ($(MISRA_MSG))" $(notdir $<)
	$(CXX_BUILD_MISRA)
endif

#
# Defines
#
# First change macro name to upper case 
UPPER_CASE 		:= $(shell echo $(notdir $(VLIB_MACRO_NAME)) | tr a-z A-Z)
VLIB_DEFINE		+= USE_$(UPPER_CASE)

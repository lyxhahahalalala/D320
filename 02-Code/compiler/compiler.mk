#
# Include Compiler configuration 
#

include $(VLIB_ROOT)/compiler/$(VLIB_COMPILER)/arch.mk


#
# Add BSP source code & include path
#
# but not for integrity 
ifeq ($(findstring integrity, $(VLIB_COMPILER)),)
ifneq ($(VLIB_BOARD),)
VLIB_INC 		+= $(VLIB_ROOT)/bsp/lib
VLIB_VPATH		+= $(VLIB_ROOT)/bsp/board/$(VLIB_BOARD)/src
-include $(VLIB_ROOT)/bsp/board/$(VLIB_BOARD)/$(VLIB_BOARD).mk
endif
endif
#
# Include device makefile to get VLIB_SRC_MACROS and VLIB_OBJ_MACROS
#

ifneq ($(VLIB_DEVICE),)

#
# Device API Include path
#

VLIB_INC 		+= $(VLIB_ROOT)/device/lib

#add device configuration 
#either use the directory provided at device level (default)
# or use application's config dir (if defined) 
ifneq ($(VLIB_USE_OWN_SETUP),)
	VLIB_DEVICE_SETUP_DIR := $(VLIB_APP_DIR)/$(VLIB_USE_OWN_SETUP)
else
	VLIB_DEVICE_SETUP_DIR := $(VLIB_ROOT)/device/$(VLIB_DEVICE)/src/$(VLIB_COMPILER)
endif

VLIB_VPATH	+= $(VLIB_DEVICE_SETUP_DIR)

# Get the set of drivers to build  
ifeq ($(VLIB_DRIVER_CONFIG),)
	VLIB_DRIVER_CONFIG = default
endif

include $(VLIB_ROOT)/device/$(VLIB_DEVICE)/$(VLIB_DEVICE).mk

ifneq ($(VLIB_DEVICE_COMMON),)
VLIB_VPATH	+= $(VLIB_ROOT)/device/$(VLIB_DEVICE_COMMON)/src/dev
endif

# Add CPU name too 
# First change to upper case 
CPU_UPPER_CASE 	:= $(shell echo $(notdir $(VLIB_CPU)) | tr a-z A-Z)
VLIB_DEFINE		+= USED_CPU=$(CPU_UPPER_CASE)

endif


VLIB_VPATH		+= $(VLIB_OUT_DIR)


#
# Define make default target
#

.PHONY : all

all:	$(VLIB_APP) $(VLIB_OBJS) $(VLIB_LIBS) $(VLIB_LIB_OBJS)


#
# Include macro module makefiles (if any)
#

#
# Include bsp drivers 
#
VLIB_INC_BSP_DRIVER := $(foreach DRIVER,$(VLIB_BSP_DRIVER),$(VLIB_ROOT)/bsp/$(DRIVER)/$(notdir $(DRIVER)).mk)
-include $(VLIB_INC_BSP_DRIVER)

#
# Add driver library, if there is at least one VLIB_OBJ_MACROS module
#

#make sure to remove all duplicates from both lists 
VLIB_OBJ_MACROS     := $(sort $(VLIB_OBJ_MACROS)) 
VLIB_SRC_MACROS     := $(sort $(VLIB_SRC_MACROS)) 
VLIB_OBJ_MACROS     := $(filter-out $(VLIB_SRC_MACROS),$(VLIB_OBJ_MACROS))

VLIB_INC_SRC_MACROS := $(foreach MACRO,$(VLIB_SRC_MACROS),$(VLIB_ROOT)/macro/$(MACRO)/$(notdir $(MACRO)).mk)
VLIB_INC_OBJ_MACROS := $(foreach MACRO,$(VLIB_OBJ_MACROS),$(VLIB_ROOT)/macro/$(MACRO)/$(notdir $(MACRO)).mk)

ifneq ($(VLIB_OBJ_MACROS),)
VLIB_LIBS += $(VLIB_ROOT)/device/$(VLIB_DEVICE)/lib/lib$(VLIB_DEVICE)_$(VLIB_COMPILER).$(VLIB_LIB_EXT)
endif

VLIB_MACRO_IN_LIB	:= no
-include $(VLIB_INC_SRC_MACROS)

VLIB_MACRO_IN_LIB	:= yes
-include $(VLIB_INC_OBJ_MACROS)



#
# Add CDI source code & include path
#

ifeq ($(VLIB_USE_CDI),yes)
VLIB_DEFINE		+= USE_CDI
VLIB_INC 		+= $(VLIB_ROOT)/cdi/lib
VLIB_VPATH		+= $(VLIB_ROOT)/cdi/src
endif

#
# Add Middleware 
#

VLIB_MIDDLEWARE_DRIVER := $(foreach DRIVER,$(VLIB_MIDDLEWARE),$(VLIB_ROOT)/middleware/$(notdir $(DRIVER))/$(notdir $(DRIVER)).mk)
include $(VLIB_MIDDLEWARE_DRIVER)

#
# Generic rules
#

VPATH 		+= $(VLIB_VPATH) $(VLIB_LIB_VPATH)

VLIB_SRC_EXT	:= c cc cpp $(VLIB_AS_EXT)

VLIB_SRCS	+= $(foreach EXT, $(VLIB_SRC_EXT),$(wildcard $(foreach DIR,$(VLIB_VPATH),$(DIR)/*.$(EXT))))
VLIB_DEPS	+= $(foreach NAME,$(VLIB_SRCS),$(VLIB_OUT_DIR)$(basename $(notdir $(NAME))).d)
VLIB_OBJS	+= $(foreach NAME,$(VLIB_SRCS),$(VLIB_OUT_DIR)$(basename $(notdir $(NAME))).$(VLIB_OBJ_EXT))

VLIB_LIB_SRCS	+= $(foreach EXT, $(VLIB_SRC_EXT),$(wildcard $(foreach DIR,$(VLIB_LIB_VPATH),$(DIR)/*.$(EXT))))
VLIB_LIB_DEPS	:= $(foreach NAME,$(VLIB_LIB_SRCS),$(VLIB_OUT_DIR)$(basename $(notdir $(NAME))).d)
VLIB_LIB_OBJS  	:= $(foreach NAME,$(VLIB_LIB_SRCS),$(VLIB_OUT_DIR)$(basename $(notdir $(NAME))).$(VLIB_OBJ_EXT))

VLIB_MAKE_FILES	= $(patsubst %.d,,$(MAKEFILE_LIST))


ifeq ($(firstword $(subst _, ,$(shell uname))),CYGWIN)
VLIB_WINPATH 	:=$(shell cygpath -m $(VLIB_ROOT))
VLIB_PATH2CC	= $(subst $(VLIB_ROOT),$(VLIB_WINPATH),$(1))
else
VLIB_WINPATH 	:= $(VLIB_ROOT)
VLIB_PATH2CC	= $(1)
endif


ifeq ($(VLIB_BUILD_LOG), on)
VLIB_BUILD_LOG := 
else
VLIB_BUILD_LOG := @
endif


$(VLIB_OBJS) : $(VLIB_MAKE_FILES)

$(VLIB_LIB_OBJS) : $(VLIB_MAKE_FILES)


.PHONY : proper

proper : clean

ifneq ($(VLIB_LIB_OBJS),)
	@echo removing $(VLIB_LIBS)
	rm -f $(VLIB_LIBS)
else
	$(VLIB_BUILD_LOG)for TOKEN in $(VLIB_LIBS); do echo Cleaning external library `basename $${TOKEN%.a}`.mk in `dirname $$TOKEN`; \
		$(MAKE) VLIB_BUILD_LOG="$(VLIB_BUILD_LOG)" VLIB_DEVICE="$(VLIB_DEVICE)" VLIB_DEVICE_COMMON="$(VLIB_DEVICE_COMMON)" -C `dirname $$TOKEN` -f `basename $${TOKEN%.a}`.mk	clean ; done	
endif
ifneq ($(VLIB_KERNEL),)
	@echo removing OS kernel `basename $(VLIB_KERNEL)` in `dirname $(VLIB_KERNEL)`
	$(VLIB_BUILD_LOG)$(MAKE) VLIB_BUILD_LOG="$(VLIB_BUILD_LOG)" VLIB_OUT_DIR="$(VLIB_OUT_DIR)" VLIB_DEVICE="$(VLIB_DEVICE)" VLIB_DRIVER_CONFIG="$(VLIB_DRIVER_CONFIG)" VLIB_DEVICE_COMMON="$(VLIB_DEVICE_COMMON)" VLIB_SRC_MACROS="$(VLIB_KERNEL_SRC_MACROS)" VLIB_IGT_BSP_DIR=$(VLIB_BSP_DIR) VLIB_EXT_PATH=`readlink -m $(VLIB_KERNEL_EXT_PATH)` -C `dirname $(VLIB_KERNEL)` proper 
endif

ifneq ($(VLIB_AS_APP),)
	$(VLIB_BUILD_LOG)for TOKEN in $(VLIB_AS_APP); do echo Cleaning external project `basename $${TOKEN}` in `dirname $$TOKEN`; \
		  $(MAKE) VLIB_DEVICE="$(VLIB_DEVICE)" VLIB_DEVICE_COMMON="$(VLIB_DEVICE_COMMON)" -C `dirname $$TOKEN` clean ; done	
endif								   

.PHONY : up

up : uc

ifneq ($(VLIB_LIB_OBJS),)
	@echo removing $(VLIB_LIBS)
	rm -f $(VLIB_LIBS)
else
	$(VLIB_BUILD_LOG)for TOKEN in $(VLIB_LIBS); do echo Cleaning external library `basename $${TOKEN%.a}`.mk in `dirname $$TOKEN`; \
		$(MAKE) VLIB_BUILD_LOG="$(VLIB_BUILD_LOG)" VLIB_DEVICE="$(VLIB_DEVICE)" VLIB_DEVICE_COMMON="$(VLIB_DEVICE_COMMON)" -C `dirname $$TOKEN` -f `basename $${TOKEN%.a}`.mk	uc ; done	
endif
ifneq ($(VLIB_KERNEL),)
	@echo removing OS kernel `basename $(VLIB_KERNEL)` in `dirname $(VLIB_KERNEL)`
	$(VLIB_BUILD_LOG)$(MAKE) VLIB_BUILD_LOG="$(VLIB_BUILD_LOG)" VLIB_OUT_DIR="$(VLIB_OUT_DIR)" VLIB_DEVICE="$(VLIB_DEVICE)" VLIB_DRIVER_CONFIG="$(VLIB_DRIVER_CONFIG)" VLIB_DEVICE_COMMON="$(VLIB_DEVICE_COMMON)" VLIB_SRC_MACROS="$(VLIB_KERNEL_SRC_MACROS)" VLIB_IGT_BSP_DIR=$(VLIB_BSP_DIR) VLIB_EXT_PATH=`readlink -m $(VLIB_KERNEL_EXT_PATH)` -C `dirname $(VLIB_KERNEL)` up 
endif

ifneq ($(VLIB_AS_APP),)
	$(VLIB_BUILD_LOG)for TOKEN in $(VLIB_AS_APP); do echo Cleaning external project `basename $${TOKEN}` in `dirname $$TOKEN`; \
		  $(MAKE) VLIB_DEVICE="$(VLIB_DEVICE)" VLIB_DEVICE_COMMON="$(VLIB_DEVICE_COMMON)" -C `dirname $$TOKEN` uc ; done	
endif								   

#
# Info for debugging the make process
#

.PHONY : clean

clean :
	rm -f *.$(VLIB_APP_EXT) *.$(VLIB_LIB_EXT) *.$(VLIB_APP_EXT).hex *.$(VLIB_OBJ_EXT) *.bak *.bak *.*.bak *.lst *.lcp *.inf *.rec *.dba *.dbo *.dla *.dle *.dnm *.mono *.mono.ael *.map *.dep *.d *.i *.si *~ *.ti *.ii
#   850eserv2.log might still be in use by MULTI. Suppress this error
#   This is what we see in the console
	@echo rm -f 850eserv2.log
#   This is the effective command with suppressed error messages
	-@rm -f 850eserv2.log 2>/dev/null || true
	rm -rf ./ship ./obj

.PHONY : uc
uc :
	rm -f *.gpj *.prj *.mono.int *.$(VLIB_APP_EXT) *.$(VLIB_LIB_EXT) *.$(VLIB_APP_EXT).hex *.$(VLIB_OBJ_EXT) *.bak *.bak *.*.bak *.lst *.lcp *.inf *.rec *.dba *.dbo *.dla *.dle *.dnm *.mono *.mono.ael *.map *.dep *.d *.i *.si *~ *.ti *.ii
#   850eserv2.log might still be in use by MULTI. Suppress this error
#   This is what we see in the console
	@echo rm -f 850eserv2.log
#   This is the effective command with suppressed error messages
	-@rm -f 850eserv2.log 2>/dev/null || true
	rm -rf ./ship ./obj

.PHONY : list_all

list_all : list

ifeq ($(VLIB_LIB_OBJS),)
	$(VLIB_BUILD_LOG)for TOKEN in $(VLIB_LIBS); do echo generating project file for external library `basename $${TOKEN%.a}`.mk in `dirname $$TOKEN`; \
		$(MAKE) VLIB_OUT_DIR="$(VLIB_OUT_DIR)`basename $${TOKEN} .a`/" GPJ_DIR="$(GPJ_DIR)" VLIB_DEVICE="$(VLIB_DEVICE)" VLIB_DEVICE_COMMON="$(VLIB_DEVICE_COMMON)" VLIB_MY_DIR=`dirname $$TOKEN` -C `dirname $$TOKEN` list_all -f `basename $${TOKEN%.a}`.mk; done	
endif

ifneq ($(VLIB_KERNEL),)
	@echo generating project file for OS kernel `basename $(VLIB_KERNEL)` in `dirname $(VLIB_KERNEL)`
	$(VLIB_BUILD_LOG)$(MAKE) VLIB_DT_COMPILED_IN="$(VLIB_DT_COMPILED_IN)" GPJ_DIR="$(VLIB_APP_DIR)" VLIB_OUT_DIR="$(VLIB_OUT_DIR)kernel/" VLIB_KERNEL_NAME=`basename $(VLIB_KERNEL)` VLIB_BOARD="$(VLIB_BOARD)" VLIB_DEVICE="$(VLIB_DEVICE)" VLIB_DRIVER_CONFIG="$(VLIB_DRIVER_CONFIG)" VLIB_DEVICE_COMMON="$(VLIB_DEVICE_COMMON)" VLIB_IGT_BSP_DIR=$(VLIB_BSP_DIR) VLIB_EXT_PATH=`readlink -m $(VLIB_KERNEL_EXT_PATH)` -C `dirname $(VLIB_KERNEL)` list_all
	
endif

ifneq ($(VLIB_AS_APP),)
	$(VLIB_BUILD_LOG)for TOKEN in $(VLIB_AS_APP); do echo generating project file for external project `basename $${TOKEN}` in `dirname $$TOKEN`; \
		$(MAKE) VLIB_AS_NAME=`basename $${TOKEN}` VLIB_OUT_DIR="$(VLIB_OUT_DIR)`basename $${TOKEN} .out`/" GPJ_DIR="$(VLIB_APP_DIR)" VLIB_DEVICE="$(VLIB_DEVICE)" VLIB_DEVICE_COMMON="$(VLIB_DEVICE_COMMON)" -C `dirname $$TOKEN` list_all ; done	
endif

.PHONY : list

list:
	@echo VLIB_ROOT=$(VLIB_ROOT)
	@echo VLIB_APP_DIR=$(VLIB_APP_DIR)
	@echo VLIB_OUT_DIR=$(VLIB_OUT_DIR)
	@echo VLIB_APP=$(VLIB_APP)	
	@echo VLIB_AS_APP=$(VLIB_AS_APP)	
	@echo VLIB_COMPILER=$(VLIB_COMPILER)
	@echo VLIB_COMPILER_OPTIMIZATION=$(VLIB_COMPILER_OPTIMIZATION)
	@echo VLIB_DEVICE=$(VLIB_DEVICE)
	@echo VLIB_DRIVER_CONFIG=$(VLIB_DRIVER_CONFIG)
	@echo VLIB_BOARD=$(VLIB_BOARD)
	@echo VLIB_BSP_USE_UART=$(VLIB_BSP_USE_UART)
	@echo VLIB_SRC_MACROS=$(VLIB_SRC_MACROS)
	@echo VLIB_OBJ_MACROS=$(VLIB_OBJ_MACROS)
	@echo VLIB_KERNEL_SRC_MACROS=$(VLIB_KERNEL_SRC_MACROS)
	@echo VPATH=$(VPATH)
	@echo VLIB_VPATH=$(VLIB_VPATH)
	@echo SHIP_VPATH=$(SHIP_VPATH)
	@echo VLIB_LIB_VPATH=$(VLIB_LIB_VPATH)
	@echo SHIP_LIB_VPATH=$(SHIP_LIB_VPATH)
	@echo VLIB_OBJS=$(VLIB_OBJS)
	@echo VLIB_LIB_OBJS=$(VLIB_LIB_OBJS)
	@echo VLIB_LIB_SRCS=$(VLIB_LIB_SRCS)
	@echo VLIB_INC=$(VLIB_INC)
	@echo VLIB_INC_SRC_MACROS=$(VLIB_INC_SRC_MACROS)
	@echo VLIB_INC_OBJ_MACROS=$(VLIB_INC_OBJ_MACROS)
	@echo VLIB_COMMONFLAGS=$(VLIB_COMMONFLAGS)
	@echo VLIB_CFLAGS=$(VLIB_CFLAGS)
	@echo VLIB_DEFINE=$(VLIB_DEFINE)
	@echo VLIB_DEFOPT=$(VLIB_DEFOPT)
	@echo VLIB_LIBS=$(VLIB_LIBS)
	@echo VLIB_BSP_DRIVER=$(VLIB_BSP_DRIVER)
	@echo VLIB_INC_BSP_DRIVER=$(VLIB_INC_BSP_DRIVER)
	@echo VLIB_MAKE_FILES=$(VLIB_MAKE_FILES)
	@echo VLIB_INTEGRITY_FLAGS=$(VLIB_INTEGRITY_FLAGS) 
	@echo VLIB_ASFLAGS=$(VLIB_ASFLAGS) 
	@echo VLIB_EXTRA_ASFLAGS=$(VLIB_EXTRA_ASFLAGS)
	@echo VLIB_BUILD_LOG=$(VLIB_BUILD_LOG)
	@echo VLIB_KERNEL_TASK=$(VLIB_KERNEL_TASK)
	@echo VLIB_EXT_PATH=$(VLIB_EXT_PATH)
	@echo VLIB_KERNEL_EXT_PATH=$(VLIB_KERNEL_EXT_PATH)

#
# Ensure no object is treated as intermediate file
#
.PRECIOUS: %.$(VLIB_OBJ_EXT) %.$(VLIB_APP_EXT) %.$(VLIB_LIB_EXT) %.%.$(VLIB_APP_MONO_EXT).$(VLIB_INT_EXT)


#
# Include Compiler specific rules
#

include $(VLIB_ROOT)/compiler/$(VLIB_COMPILER)/rules.mk

#
# Dependency lists
#

# Only include DEPS if the target is not proper or clean
# This helps if a dependency is broken clean and proper can still be called
ifeq (,$(filter $(MAKECMDGOALS),proper clean))
-include $(VLIB_DEPS)
-include $(VLIB_LIB_DEPS)
endif



VLIB_INC		+= $(VLIB_ROOT)/compiler/$(VLIB_COMPILER)/inc
VLIB_INC		+= $(VLIB_ROOT)/device/$(VLIB_DEVICE)/lib

SYS_DIR := $(VLIB_ROOT)/device/$(VLIB_DEVICE)/macro_cfg
ifneq ($(wildcard $(SYS_DIR)/*.*),)
VLIB_INC		+= $(VLIB_ROOT)/device/$(VLIB_DEVICE)/macro_cfg
endif 

VLIB_INC		+= $(VLIB_ROOT)/device/$(VLIB_DEVICE_COMMON)/macro_cfg

VLIB_INCOPT     := $(foreach DIR,$(sort $(VLIB_INC)),-I$(DIR))
VLIB_DEFOPT 	:= $(foreach DEFINE,$(sort $(VLIB_DEFINE)),-D$(DEFINE))

# Debug register print 
VLIB_DEFOPT 	+= $(foreach DBG_DEFINE,$(sort $(VLIB_DBG_PRINT_REGS)),-DR_DBG_PRINT_$(DBG_DEFINE)_REGS)

# Add some device&compier specific options here 
ifeq ($(VLIB_COMPILER_USE_SDA),yes)
    VLIB_CFLAGS += -large_sda
endif
ifneq ($(VLIB_COMPILER_SDA_OPT),)
    VLIB_CFLAGS += -sda=$(VLIB_COMPILER_SDA_OPT)
endif

ifeq ($(VLIB_COMPILER_FSOFT),yes)
    VLIB_CFLAGS  += -fsoft
    VLIB_LDFLAGS += -fsoft 
endif

ifeq ($(VLIB_COMPILER_OPTIMIZATION),speed)
    VLIB_CFLAGS  += -Ospeed
endif

ifeq ($(VLIB_COMPILER_OPTIMIZATION),size)
    VLIB_CFLAGS  += -Osize
endif


VLIB_CFLAGS     += $(VLIB_COMMONFLAGS) -cpu=$(VLIB_CPU) $(VLIB_INCOPT) $(VLIB_DEFOPT) 
VLIB_ASFLAGS    += $(VLIB_COMMONFLAGS) -cpu=$(VLIB_CPU) $(VLIB_INCOPT) $(VLIB_DEFOPT)


#add the linker definition file 
# Search the linker script  
# we search in the application/target/config directory first
# if nothing is found there, we go to the vlib's device directory 
ifeq ($(findstring $(VLIB_DEVICE).ld,$(wildcard $(VLIB_APP_DIR)/config/*.ld)),$(VLIB_DEVICE).ld)
	VLIB_LDSCRIPT = $(VLIB_APP_DIR)/config/$(VLIB_DEVICE).ld
else 
	VLIB_LDSCRIPT = $(VLIB_DEVICE_SETUP_DIR)/$(VLIB_DEVICE).ld
endif

#mandatory linker setting 
ifeq ($(VLIB_ENTRY),)
   ENTRY    := _RESET
else
   ENTRY    := VLIB_ENTRY
endif

VLIB_LDFLAGS	+= -cpu=$(VLIB_CPU) -entry=$(ENTRY)



ifeq ($(firstword $(subst _, ,$(shell uname))),CYGWIN)
VLIB_DEP_BUILD_RULE = \
	 ; ESTATUS=$$? ; cat $(basename $(notdir $<)).d | sed '{s/.:[^ ]/\/cygdrive\/&\//g;s/:[^ ]//g;s/\\/\//g;s/\/$$/\\/}' > $(basename $(notdir $<)).dd; mv $(basename $(notdir $<)).dd $(basename $(notdir $<)).d ; exit $$ESTATUS
else
VLIB_DEP_BUILD_RULE =

endif

CXX_BUILD_MISRA = $(call VLIB_PATH2CC, $(VLIB_BUILD_LOG)$(CC) $(VLIB_CFLAGS) $(VLIB_MISRA) $<) $(VLIB_DEP_BUILD_RULE)
CXX_BUILD       = $(call VLIB_PATH2CC, $(VLIB_BUILD_LOG)$(CC) $(VLIB_CFLAGS) $<) $(VLIB_DEP_BUILD_RULE)
ASX_BUILD       = $(call VLIB_PATH2CC, $(VLIB_BUILD_LOG)$(AS) $(VLIB_ASFLAGS) $(VLIB_MISRA) $<) $(VLIB_DEP_BUILD_RULE)
LXX_BUILD       = $(call VLIB_PATH2CC, $(VLIB_BUILD_LOG)$(LINK) $(VLIB_MISRA) $(VLIB_OBJS) $(VLIB_LIBS) -o $@ $(VLIB_LDFLAGS) $(VLIB_LDSCRIPT))
ARXX_BUILD      = $(call VLIB_PATH2CC, $(VLIB_BUILD_LOG)$(AR) -rc $@ $(VLIB_LIB_OBJS))


%.$(VLIB_LIB_EXT) : $(VLIB_MAKE_FILES)
ifneq ($(VLIB_LIB_OBJS),)
endif


%.$(VLIB_APP_EXT) :$(VLIB_OBJS) $(VLIB_MAKE_FILES) $(VLIB_LDSCRIPT) $(VLIB_LIBS)
	@echo Linking $(notdir $@)
	$(LXX_BUILD)

%.$(VLIB_OBJ_EXT) : %.850
	@echo Assembling $(notdir $<)
	$(ASX_BUILD)

%.$(VLIB_OBJ_EXT) : %.s
	@echo Assembling $(notdir $<)
	$(ASX_BUILD)

%.$(VLIB_OBJ_EXT) : %.c
	@echo Compiling $(notdir $<)
	$(CXX_BUILD)

%.$(VLIB_OBJ_EXT) : %.cc
	@echo Compiling $(notdir $<)
	$(CXX_BUILD)

%.$(VLIB_OBJ_EXT) : %.cpp
	@echo Compiling $(notdir $<)
	$(CXX_BUILD)


include $(VLIB_ROOT)/compiler/$(VLIB_COMPILER)/ship.mk

include $(VLIB_ROOT)/compiler/svn.mk

include $(VLIB_ROOT)/compiler/$(VLIB_COMPILER)/multi.mk

include $(VLIB_ROOT)/compiler/tag.mk

include $(VLIB_ROOT)/compiler/help.mk

.PHONY : TAGS 

TAGS : GHS_PATH    := $(shell dirname $(shell which $(CC)))
TAGS :
	ctags -Re --extra=+fq --exclude='*.html' --exclude='*.htm'   $(VLIB_INC) $(VPATH) --c-kinds=+p --c++-kinds=+p \
	$(GHS_PATH)/ansi
	cp TAGS $(VLIB_ROOT)
	cp TAGS $(GHS_PATH)

.PHONY : tags

include $(VLIB_ROOT)/compiler/ultraedit.mk




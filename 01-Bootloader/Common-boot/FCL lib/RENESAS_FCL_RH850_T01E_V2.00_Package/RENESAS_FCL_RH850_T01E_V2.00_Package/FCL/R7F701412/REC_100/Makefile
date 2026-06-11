###############################################################################
# Sample application makefile                                                 #
#       adjust COMPILER_INSTALL_DIR variable to use                           #
###############################################################################
# application name and folder
CURRENT_APPL_NAME = FCLSampleApp
TARGET_OUTPUT_DIR = Sample
OBJ_OUTPUT_DIR = Sample/obj

# compiler path
COMPILER_INSTALL_DIR = c:\CubeSuite\20130820 REE RH850_P1M_pre\CubeSuite+ E2.01.00e\CubeSuite+


#compiler
CC="$(COMPILER_INSTALL_DIR)\CC-RH\V1.00.00\bin\ccrh.exe"
AS="$(COMPILER_INSTALL_DIR)\CC-RH\V1.00.00\bin\ccrh.exe"
LD="$(COMPILER_INSTALL_DIR)\CC-RH\V1.00.00\bin\rlink.exe"
 
# compiler specific defines 
DEFINES = $(patsubst %,-D%,$(PRJ_DEFINES))

# compiler specific includes
CINCLUDE   = $(patsubst %,-I%,$(INC))

# standard tool options
CFLAGS  = 
ASFLAGS = 
LDFLAGS = 
DBFLAGS = 

COMMONFLAGS  = -Xcommon=v850e3v5


CFLAGS  += $(COMMONFLAGS) -c -Xreg_mode=common -Osize -Ounroll=4 -Opipeline \
	  	   $(DEFINES) $(CINCLUDE) -Xobj_path=$(OBJ_OUTPUT_DIR)
ASFLAGS += $(COMMONFLAGS) -c -Xreg_mode=common -Onothing \
	  	   $(DEFINES) $(CINCLUDE) -Xobj_path=$(OBJ_OUTPUT_DIR)
LDFLAGS += -show=ALL -map="$(TARGET_OUTPUT_DIR)/$(CURRENT_APPL_NAME).lbp" \
	  	   -library="$(COMPILER_INSTALL_DIR)\CC-RH\V1.00.00\lib\v850e3v5\libc" \
	  	   -list="$(TARGET_OUTPUT_DIR)/$(CURRENT_APPL_NAME).map" \
	  	   -nomessage=1100,1120 -total_size -sub="$(LDSCRIPT)" \
           -output=$(TARGET_OUTPUT_DIR)/$(CURRENT_APPL_NAME).deb
 
# Tools 
ADD_TOOLS = $(LD) $(TARGET_OUTPUT_DIR)/$(CURRENT_APPL_NAME).deb -form=stype -output=$(TARGET_OUTPUT_DIR)/$(CURRENT_APPL_NAME).rec
DEL_TOOL := del /F /Q
GNU_DEL_TOOL := "C:\Program Files\GnuWin32\bin\rm"

# Source files
SRCS=main.c fcl_descriptor.c cstart.asm fcl_user.c vecttbl.asm  r_fcl_hw_access.c r_fcl_user_if.c r_fcl_hw_access_asm.asm

# linker script
LDSCRIPT = ./Sample/dr7f701412.dir

#Project defines
PRJ_DEFINES=

# Object files
OBJS := $(foreach NAME,$(SRCS),$(basename $(NAME)).obj)
OBJS := $(addprefix $(OBJ_OUTPUT_DIR)/, $(OBJS))

# search for header files in following directories
INC = ./Sample ./Sample/FCL ./Sample/FCL\lib

# search for source files in following directories
VPATH = ./Sample ./Sample/FCL ./Sample/FCL\lib

###############################################################################
# Command to link all the generated object files                              #
###############################################################################
$(CURRENT_APPL_NAME) :	$(OBJS) $(LDSCRIPT)
	@echo Check application directory ...
	@if not exist $(TARGET_OUTPUT_DIR) mkdir $(subst /,\,$(TARGET_OUTPUT_DIR))
	@echo ... Done
	@echo Building the executable
	$(LD) $(OBJS) $(LDFLAGS)
	$(ADD_TOOLS)
	@echo ... Successful

################################################################################
# Command to generate object files from all dependent files                    
################################################################################
$(OBJ_OUTPUT_DIR)/%.obj : %.c
	@echo Check output directory ...
	@if not exist $(OBJ_OUTPUT_DIR) mkdir $(subst /,\,$(OBJ_OUTPUT_DIR))
	@echo ... Done
	@echo Generating $(@F):
	@echo Compiling $<
	$(CC) $< $(CFLAGS)
	@echo ... Done
	@echo ***************************************************************************

$(OBJ_OUTPUT_DIR)/%.obj : %.asm
	@echo Check output directory ...
	@if not exist $(OBJ_OUTPUT_DIR) mkdir $(subst /,\,$(OBJ_OUTPUT_DIR))
	@echo ... Done
	@echo Generating $(@F):
	@echo Compiling $<
	$(AS) $< $(ASFLAGS)
	@echo ... Done
	@echo ***************************************************************************

$(OBJ_OUTPUT_DIR)/%.obj : %.asm
	@echo Check output directory ...
	@if not exist $(OBJ_OUTPUT_DIR) mkdir $(subst /,\,$(OBJ_OUTPUT_DIR))
	@echo ... Done
	@echo Generating $(@F):
	@echo Compiling $< 
	$(AS) $< $(ASFLAGS)
	@echo ... Done
	@echo ***************************************************************************

###############################################################################
# Command to delete all generated files                                       #
###############################################################################
clean:
	@echo Delete all object files ...
	@if exist $(subst /,\,$(OBJ_OUTPUT_DIR)) $(DEL_TOOL) $(subst /,\,$(OBJ_OUTPUT_DIR))\*.*>nul 2>nul
	@echo Delete application ...
	@if exist $(subst /,\,$(TARGET_OUTPUT_DIR)) for /f %%i in ('dir /B $(subst /,\,$(TARGET_OUTPUT_DIR))\$(CURRENT_APPL_NAME).* ^|find /V ".ld"') do $(DEL_TOOL) $(subst /,\,$(TARGET_OUTPUT_DIR))\%%i>nul 2>nul
	@if exist $(CURRENT_APPL_NAME).rec $(DEL_TOOL) $(CURRENT_APPL_NAME).rec>nul 2>nul
	@echo ... Done

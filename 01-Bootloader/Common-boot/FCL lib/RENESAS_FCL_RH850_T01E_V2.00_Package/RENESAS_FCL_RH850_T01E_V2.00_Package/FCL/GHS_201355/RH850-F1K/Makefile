###############################################################################
# Sample application makefile                                                 #
#       adjust COMPILER_INSTALL_DIR variable to use                           #
###############################################################################
# application name and folder
CURRENT_APPL_NAME = FCLSampleApp
TARGET_OUTPUT_DIR = Sample
OBJ_OUTPUT_DIR = Sample/obj

# compiler path
COMPILER_INSTALL_DIR = c:\ghs\multi_614_comp_201355


#compiler
CC="$(COMPILER_INSTALL_DIR)\ccrh850.exe"
AS="$(COMPILER_INSTALL_DIR)\ccrh850.exe"
LD="$(COMPILER_INSTALL_DIR)\ccrh850.exe"
 
# compiler specific defines 
DEFINES = $(patsubst %,-D%,$(PRJ_DEFINES))
CINCLUDE   = $(patsubst %,-I%,$(INC))

# tool options
CFLAGS  = 
ASFLAGS = 
LDFLAGS = 
DBFLAGS = 

COMMONFLAGS =   -cpu=rh850 \
                -sda=all \
                -registermode=$(register) \
                -prepare_dispose \
                -no_callt \
                -reserve_r2 \
                -inline_prologue \
                -notda \
                -fnone \
                --unknown_pragma_warnings \
                -list \
                -Ospeed \
                -object_dir=$(OBJ_OUTPUT_DIR)

COMMONFLAGS +=  -g 

CFLAGS +=       $(COMMONFLAGS) \
                $(DEFINES) \
                $(CINCLUDE) \
                -c \
                -pic
ASFLAGS +=      $(COMMONFLAGS) \
                $(DEFINES) \
                $(CINCLUDE) \
                -c \
                -passsource \
                -preprocess_assembly_files
LDFLAGS +=      $(COMMONFLAGS) \
                -map="$(TARGET_OUTPUT_DIR)/$(CURRENT_APPL_NAME).map" \
                $(LDSCRIPT) \
                -o "$(TARGET_OUTPUT_DIR)/$(CURRENT_APPL_NAME)"
 
# Tools 
ADD_TOOLS  = "$(COMPILER_INSTALL_DIR)\gsrec" $(TARGET_OUTPUT_DIR)/$(CURRENT_APPL_NAME) -o $(TARGET_OUTPUT_DIR)/$(CURRENT_APPL_NAME).rec
DEL_TOOL := del /F /Q

# Source files
SRCS=main.c fcl_descriptor.c fcl_user.c dr7f701587_Startup.850  r_fcl_hw_access.c r_fcl_user_if.c r_fcl_hw_access_asm.850

# linker script
LDSCRIPT = ./Sample/dr7f701587.ld

#Project defines
PRJ_DEFINES=

# Object files
OBJS := $(foreach NAME,$(SRCS),$(basename $(NAME)).o)
OBJS := $(addprefix $(OBJ_OUTPUT_DIR)/, $(OBJS))

# search for header files in following directories
INC = ./Sample ./../FCL ./../FCL\lib 
 
# search for source files in following directories 
VPATH = ./Sample ./../FCL ./../FCL\lib 

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
$(OBJ_OUTPUT_DIR)/%.o : %.c
	@echo Check output directory ...
	@if not exist $(OBJ_OUTPUT_DIR) mkdir $(subst /,\,$(OBJ_OUTPUT_DIR))
	@echo ... Done
	@echo Generating $(@F):
	@echo Compiling $<
	$(CC) $< $(CFLAGS)
	@echo ... Done
	@echo ***************************************************************************

$(OBJ_OUTPUT_DIR)/%.o : %.850
	@echo Check output directory ...
	@if not exist $(OBJ_OUTPUT_DIR) mkdir $(subst /,\,$(OBJ_OUTPUT_DIR))
	@echo ... Done
	@echo Generating $(@F):
	@echo Compiling $<
	$(AS) $< $(ASFLAGS)
	@echo ... Done
	@echo ***************************************************************************

$(OBJ_OUTPUT_DIR)/%.o : %.850
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

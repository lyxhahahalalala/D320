
CC   := ccv850e
CCD  := multi
CXX  := ccv850e
AR   := ax
AS   := ccv850e
LINK := ccv850e

#VLIB_CFLAGS		 +=  --prototype_errors --no_implicit_include --diag_warning 1 --no_commons -notda -allocate_ep -no_callt -registermode=22 -reserve_r2 -Zuse1bit 
VLIB_CFLAGS		 +=  --prototype_errors --no_implicit_include --no_commons -notda -allocate_ep -no_callt -registermode=22 -reserve_r2 -Zuse1bit 
VLIB_COMMONFLAGS += -c -MD -list -passsource

ifneq ($(VLIB_DEBUG_BUILD), no)
VLIB_COMMONFLAGS += -G
endif

ifeq ($(VLIB_FORCE_MISRA), yes)
VLIB_RENESAS_MISRA        := no
VLIB_RENESAS_MISRA_STRICT := yes
endif
ifeq ($(VLIB_RENESAS_MISRA), yes)
VLIB_MISRA		+= --misra_2004=all,-1.1,-8.10,-11.4,-11.5,-13.3,-14.7,-17.4,-18.4,-5.1,-5.5,-5.7,-16.2,-9.3,-19.7,-19.13,-19.12,-19.6,-11.1,-6.4,-20.9,-8.5,-5.6,-11.3,-14.3,-10.1,-10.3,-16.1
VLIB_MISRA		+= --slash_comment --no_misra_runtime --misra_req=warn
endif
ifeq ($(VLIB_RENESAS_MISRA_STRICT), yes)
VLIB_MISRA		+= --misra_2004=all,-1.1,-11.4,-11.5,-13.3,-14.7,-17.4,-18.4,-5.1,-5.7,-16.2,-9.3,-19.7,-19.13,-19.12,-19.6,-11.1,-6.4,-20.9,-8.5,-5.6,-11.3,-14.3,-10.1,-10.3,-16.1
VLIB_MISRA		+= --slash_comment --no_misra_runtime --misra_req=warn
endif

VLIB_ASFLAGS	 	+= -preprocess_assembly_files
VLIB_LDFLAGS	 	+= -map -Mn -Mx -notda -delete

VLIB_AS_EXT		:= 850 s
VLIB_LIB_EXT		:= a
VLIB_APP_EXT		:= out
VLIB_OBJ_EXT		:= o

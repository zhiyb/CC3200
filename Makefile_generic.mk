# General Makefile
# Author: Yubo Zhi (normanzyb@gmail.com)

# Root dir
SRCDIR	?= .

# Change variable type to simply-expanded
SRC	:= $(SRC)
LIBS	:= $(LIBS)
DEFS	:= $(DEFS)
SUBDIRS	:= $(SUBDIRS)
INCDIRS	:= $(INCDIRS)

# Defaults
#CROSS_COMPILE	?=
DEPDIR	?= .deps
ifndef OPTIMIZE
OPTLEVEL ?= s
endif

# Subdirectories
define inc_subdir
DIR	:= $(1)
include $(SRCDIR)/$$(DIR)/Makefile
endef
$(foreach dir,$(SUBDIRS),$(eval $(call inc_subdir,$(dir))))

# Append configuration definations
define checkconf
ifdef $1
	DEFS	+= -D$1=$$($1)
endif
endef
$(foreach conf,$(CONFS),$(eval $(call checkconf,$(conf))))
define checkconf_d
ifeq ($$($1),y)
	DEFS	+= -D$1
endif
endef
$(foreach conf,$(CONFS_D),$(eval $(call checkconf_d,$(conf))))

# Cross compile target suffix
ifndef CROSS_COMPILE
ifeq ($(OS),Windows_NT)
	SUFFIX	?= .exe
endif
else
	SUFFIX	?= .elf
endif

ELF	= $(TRG)$(SUFFIX)
OBJS	= $(subst .S,.o,$(subst .c,.o,$(SRC:.cpp=.o)))
vpath %.c $(SRCDIR)
vpath %.cpp $(SRCDIR)
vpath %.S $(SRCDIR)

AR	= $(CROSS_COMPILE)ar
AS	= $(CROSS_COMPILE)gcc
CC	= $(CROSS_COMPILE)gcc
CXX	= $(CROSS_COMPILE)g++
LD	= $(CROSS_COMPILE)gcc
SIZE	= $(CROSS_COMPILE)size

# Flags
INCS	+= $(INCDIRS:%=-I$(SRCDIR)/%)
ifdef OPTLEVEL
FLAGS	+= -O$(OPTLEVEL)
endif
FLAGS	+= $(OPTIMIZE)

# LDSCRIPT
ifdef LDSCRIPT
LDFLAG	+= -T $(LDSCRIPT)
endif

ARFLAGS	= $(ARFLG) rcs
ifdef $(OVERRIDE)
override ASFLAGS	= $(ASFLAG) $(DEFS) $(INCS) $(FLAGS)
override CFLAGS	= $(CFLAG) $(DEFS) $(INCS) $(FLAGS)
override CXXFLAGS	= $(CXXFLAG) $(DEFS) $(INCS) $(FLAGS)
override LDFLAGS	= $(LDFLAG)
else
ASFLAGS	= $(ASFLAG) $(DEFS) $(INCS) $(FLAGS)
CFLAGS	= $(CFLAG) $(DEFS) $(INCS) $(FLAGS)
CXXFLAGS	= $(CXXFLAG) $(DEFS) $(INCS) $(FLAGS)
LDFLAGS	= $(LDFLAG)
endif

# Verbose
define verbose
	@if [ 'x$(VERBOSE)' = x ]; then	\
		echo "  "$1;		\
	else				\
		echo $2;		\
	fi
	@$2
endef

# Default rule
all: $(ELF) $(EXTRA_TARGETS)
CLEAN_FILES	+= $(ELF)

CLEAN_FILES	+= $(OBJS)
$(ELF): $(OBJS) $(LIBFILES) $(LDSCRIPT)
	$(call verbose,"LD	$@",\
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBS))
	$(call verbose,"SIZE	$@",\
	$(SIZE) $@)

%.o: %.c
	@mkdir -p $(dir $(DEPDIR)/$*.d) $(dir $@)
	$(call verbose,"CC	$<",\
	$(CC) $(CFLAGS) -c -o $@ $<) -MD -MP -MF "$(DEPDIR)/$*.d"

%.o: %.cpp
	@mkdir -p $(dir $(DEPDIR)/$*.d) $(dir $@)
	$(call verbose,"CXX	$<",\
	$(CC) $(CFLAGS) -c -o $@ $<) -MD -MP -MF "$(DEPDIR)/$*.d"

%.o: %.S
	$(call verbose,"AS	$<",\
	$(AS) $(ASFLAGS) -c -o $@ $<)

%.lst: %$(SUFFIX)
	$(call verbose,"GEN	$@",\
	$(OBJDUMP) -h -S $< > $@)

# Dependency
-include $(OBJS:%.o=$(DEPDIR)/%.d)
CLEAN_DIRS	+= $(DEPDIR)

# Clean up
.PHONY: clean
clean:
	$(call verbose,"CLEAN	$(CLEAN_FILES)",\
	$(RM) $(CLEAN_FILES))
	$(call verbose,"CLEAN	$(CLEAN_DIRS)",\
	$(RM) -r $(CLEAN_DIRS))
	@rmdir -p $(dir $(CLEAN_FILES)) 2> /dev/null || true

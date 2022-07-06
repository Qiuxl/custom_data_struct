## this is a general makefile
## just
# General
CC			= g++
LD			= g++
RM			= rm -rf
RMDIR		= rmdir
INSTALL		= install
DEBUG		= -march=native -ftrapv

## CHANGE THIS ##
TARGET		= test
SRCDIR		= .
OBJDIR		= .
BINDIR		= ./
## CHANGE THIS ##

# CFLAGS, LDFLAGS, CPPFLAGS, PREFIX can be overriden on CLI
CFLAGS		:= $(DEBUG)
CPPFLAGS	:= -std=c++11 -lpthread
LDFLAGS		:= -std=c++11 -lpthread
PREFIX		:= ./
TARGET_ARCH :=
INC			:= -I.


# Compiler Flags
ALL_CFLAGS		:= $(CFLAGS)
ALL_CFLAGS		+= -Wall -Wextra -pedantic -ansi
ALL_CFLAGS		+= -fno-strict-aliasing
ALL_CFLAGS		+= -Wuninitialized -Winit-self -Wfloat-equal
ALL_CFLAGS		+= -Wundef -Wshadow -Wcast-qual -Wcast-align
ALL_CFLAGS		+= -Wconversion -Wsign-conversion
ALL_CFLAGS		+= -Wno-multichar -Wpacked -Wstrict-overflow -Wvla
ALL_CFLAGS		+= -Wformat -Wno-format-zero-length
ALL_CFLAGS      += -g
# Preprocessor Flags
ALL_CPPFLAGS	:= $(CPPFLAGS)

# Linker Flags
ALL_LDFLAGS		:= $(LDFLAGS)
ALL_LDLIBS		:= -lc


# Source, Binaries, Dependencies
SRC			:= $(shell find $(SRCDIR) -type f -name '*.cc')
OBJ			:= $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SRC:.cc=.o))
DEP			:= $(OBJ:.o=.d)
BIN			:= $(BINDIR)/$(TARGET)
-include $(DEP)


# Verbosity Control, ala automake
V 			= 0

# Verbosity for CC
REAL_CC 	:= $(CC)
CC_0 		= @echo "CC $<"; $(REAL_CC)
CC_1 		= $(REAL_CC)
CC 			= $(CC_$(V))

# Verbosity for LD
REAL_LD 	:= $(LD)
LD_0 		= @echo "LD $@"; $(REAL_LD)
LD_1 		= $(REAL_LD)
LD 			= $(LD_$(V))

# Verbosity for RM
REAL_RM 	:= $(RM)
RM_0 		= @echo "Cleaning..."; $(REAL_RM)
RM_1 		= $(REAL_RM)
RM 			= $(RM_$(V))

# Verbosity for RMDIR
REAL_RMDIR 	:= $(RMDIR)
RMDIR_0 	= @$(REAL_RMDIR)
RMDIR_1 	= $(REAL_RMDIR)
RMDIR 		= $(RMDIR_$(V))



# Build Rules
.PHONY: clean
.DEFAULT_GOAL := all

all: setup $(BIN)
setup: dir
remake: clean all

dir:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)


$(BIN): $(OBJ)
	$(LD) $(ALL_LDFLAGS) $(INC) $^ $(ALL_LDLIBS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	#$(CC) $(ALL_CFLAGS) $(INC) $(ALL_CPPFLAGS) -c -MMD -MP -o $@ $<
	$(CC) $(ALL_CFLAGS) $(INC) $(ALL_CPPFLAGS) -c -o $@ $<

install: $(BIN)
	$(INSTALL) -d $(PREFIX)/bin
	$(INSTALL) $(BIN) $(PREFIX)/bin

clean:
	$(RM) $(OBJ) $(DEP) $(BIN)
	$(RMDIR) $(OBJDIR) $(BINDIR) 2> /dev/null; true
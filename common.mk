# common.mk
#
#
#


# Makefile configuration:
SHELL := bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
.DELETE_ON_ERROR:
MAKEFLAGS += --warn-undefined-variables
# MAKEFLAGS += --no-builtin-rules
ifeq ($(origin .RECIPEPREFIX), undefined)
	$(error This Make does not support .RECIPEPREFIX. Please use GNU Make 4.0 or later)
endif
.RECIPEPREFIX = > 

# Directories:
ROOT_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
INCLUDE := $(ROOT_DIR)include
MODULES := $(ROOT_DIR)modules
BIN := $(ROOT_DIR)tests/bin

# Compiler
CC = gcc

# Compiler options:
#   -g       Produce debugging information
#   -I<dir>  Search in <dir> for include files.
#   -Wall    Enable all warnings.
#   -Werror  Make all warnings into errors.
#   -MMD     Create a dependencies files (.d), which we include in the Makefile.
#
# override   Allow additional parameters via command line: make CFLAGS=...
#
override CFLAGS += -g -Wall -Werror -MMD -I$(INCLUDE)

# Linker options:
#   -lm  Link math library.
#
LDFLAGS += -lm

# Valgrind options:
#   --error-exitcode=1
#   --leak-check=full
#   --show-error-list=yes
#   --read-var-info=yes
#   --show-leak-kinds=all
# 
VGFLAGS += --error-exitcode=1 --leak-check=full  --read-var-info=yes --show-leak-kinds=all

# Variables:
ALL_VARIABLES := $(filter %_OBJECTS,$(.VARIABLES))
ALL_EXECUTABLES := $(subst _OBJECTS,,$(ALL_VARIABLES))
PROGRAMS := $(filter-out %.a,$(ALL_EXECUTABLES))
ALL_OBJECTS := $(foreach target, $(ALL_EXECUTABLES), $($(target)_OBJECTS))
ALL_DEPENDENCIES := $(patsubst %.o, %.d, $(filter %.o, $(ALL_OBJECTS)))

RUN_TARGETS ?= $(addprefix run-, $(ALL_EXECUTABLES))
VALGRIND_TARGETS ?= $(addprefix valgrind-, $(ALL_EXECUTABLES))

# Add --time argument for each test:
$(foreach test, $(ALL_EXECUTABLES), $(eval $(test)_ARGUMENTS ?= --time))

test:
> @echo "dirs: $(ROOT_DIR) $(MODULES) $(INCLUDE) $(BIN)"
> @echo "All variables: $(ALL_VARIABLES)"
> @echo "All exectuables: $(ALL_EXECUTABLES)"
> @echo "Objects: $(ALL_OBJECTS)"
> @echo "programs: $(PROGRAMS)"
> @echo "deps: $(ALL_DEPENDENCIES)"
> @echo "targets:"
> @echo " - run: $(RUN_TARGETS)"
> @echo " - valgrind: $(VALGRIND_TARGETS)"

# Targets

all: $(PROGRAMS)

.SECONDEXPANSION:

$(PROGRAMS): $$($$@_OBJECTS)
> $(CC) $^ -o $(BIN)/$@ $(LDFLAGS)

-include $(ALL_DEPENDENCIES)

clean:
> $(RM) $(ALL_OBJECTS) $(ALL_DEPENDENCIES) $(BIN)/$(PROGRAMS)

run-%: %
> $(BIN)/$* $($*_ARGUMENTS)

run: $(RUN_TARGETS)

valgrind-%: %
> valgrind $(VGFLAGS) $(BIN)/$* $($*_ARGUMENTS)

valgrind: $(VALGRIND_TARGETS)

$(RUN_TARGETS):
$(VALGRIND_TARGETS):

.PHONY: clean run valgrind

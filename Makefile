# Makefile
#
# For more information regarding the structure of this Makefile:
# https://gist.github.com/constantinosgeorgiou/b3e3bad80aea92c8954eae9859ea300c

# Makefile configuration
SHELL := bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
.DELETE_ON_ERROR:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules
ifeq ($(origin .RECIPEPREFIX), undefined)
	$(error This Make does not support .RECIPEPREFIX. Please use GNU Make 4.0 or later)
endif
.RECIPEPREFIX = > 

# +-----------+
# |  Targets  |
# +-----------+

all: tests

tests:
> $(MAKE) -C tests all

run: run-tests

run-tests:
> $(MAKE) -C tests run

# Check for memory leaks:
valgrind-tests:
> $(MAKE) -C tests valgrind

clean:
> $(MAKE) -C tests clean

# Targets that generate no files:
.PHONY: tests run run-tests valgrind-tests clean
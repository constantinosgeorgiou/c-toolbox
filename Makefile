####################################################################################################
#
# Makefile
#
# For more information regarding the structure of this Makefile:
# https://gist.github.com/constantinosgeorgiou/b3e3bad80aea92c8954eae9859ea300c#####################
# 
####################################################################################################


####    Makefile configuration    ##################################################################
SHELL := bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
.DELETE_ON_ERROR:
MAKE += --silent
ifeq ($(origin .RECIPEPREFIX), undefined)
	$(error This Make does not support .RECIPEPREFIX. Please use GNU Make 4.0 or later)
endif
.RECIPEPREFIX = > 
####################################################################################################

# +-----------+
# |  Targets  |
# +-----------+

# Setup repository:
# -----------------

setup:
> mkdir --parents tests/bin


# Compile tests:
# --------------

all: tests

tests:
> $(MAKE) --directory=tests all


# Execute tests:
# --------------

run: run-tests

run-tests:
> $(MAKE) --directory=tests run


# Memory leaks checks:
# --------------------

valgrind: valgrind-tests

valgrind-tests:
> $(MAKE) --directory=tests valgrind


# Clean up generated files:
# -------------------------

clean:
> $(MAKE) --directory=tests clean


# Targets that generate no files:
.PHONY: setup all tests run run-tests valgrind valgrind-tests clean
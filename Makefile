####################################################################################################
#
# Makefile
#
# For more information regarding the structure of this Makefile:
# https://gist.github.com/constantinosgeorgiou/b3e3bad80aea92c8954eae9859ea300c
# 
####################################################################################################


####    Makefile configuration    ##################################################################
SHELL := bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
.DELETE_ON_ERROR:
MAKE += --silent
####################################################################################################

# +-----------+
# |  Targets  |
# +-----------+

# Setup repository:
# -----------------

setup:
	@# -p  Make parent directories
	mkdir -p tests/bin

	@# Initialize submodules
	git submodule update --init --recursive


# Compile tests:
# --------------

tests: setup
	$(MAKE) --directory=tests all


# Execute tests:
# --------------

run-tests: setup
	$(MAKE) --directory=tests run


# Memory leaks checks:
# --------------------

valgrind-tests: setup
	$(MAKE) --directory=tests valgrind


# Clean up generated files:
# -------------------------

clean: setup
	$(MAKE) --directory=tests clean


# Targets that generate no files:
.PHONY: setup all tests run run-tests valgrind valgrind-tests clean
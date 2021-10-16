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

all: run

run: run-tests

test: setup
	$(MAKE) --directory=tests all

run-tests: setup
	$(MAKE) --directory=tests run

valgrind-tests: setup
	$(MAKE) --directory=tests valgrind

clean: setup
	$(MAKE) --directory=tests clean
	$(RM) -r docs/generated/* docs/warnings.log

setup:
	@# Flags:
	@#   -p  Make parent directories
	mkdir -p tests/bin
	mkdir -p docs/generated

docs:
	doxygen docs/Doxyfile

# Targets that generate no files:
.PHONY: all run run-tests test valgrind-tests clean setup docs

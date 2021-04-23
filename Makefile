# Inspired by:
#  - Bash Strict Mode: http://redsymbol.net/articles/unofficial-bash-strict-mode/
#  - Your Makefiles are wrong:  https://tech.davis-hansson.com/p/make/

SHELL := bash
# Flags
#   -e             Exit if any command has a non-zero exit status. 
#   -u             Exit if a reference to any variable wasn't previously defined.
#   -o pipefail    Prevents errors in a pipeline from being masked.
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:         # Ran each Make recipe as one single shell session.
.DELETE_ON_ERROR:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

# Change the block character to '>' instead of tabs.
# For readability purposes and portability purposes.
ifeq ($(origin .RECIPEPREFIX), undefined)
	$(error This Make does not support .RECIPEPREFIX. Please use GNU Make 4.0 or later)
endif
.RECIPEPREFIX = > 

test:
> @echo "test"

# Test a module
# Test all modules
# Check memory leaks of a module
# Check memory leaks of all modules

# Targets that generate no files:
.PHONY: test
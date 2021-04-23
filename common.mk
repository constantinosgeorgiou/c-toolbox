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


PATH := $(dir $(lastword $(MAKEFILE_LIST)))
INCLUDE := $(PATH)include
MODULES := $(PATH)modules

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


WITH_OBJECTS := $(subst _OBJECTS,,$(filter %_OBJECTS,$(.VARIABLES)))
PROGS := $(filter-out %.a,$(WITH_OBJECTS))
LIBS := $(filter %.a,$(WITH_OBJECTS))

# Group all objects of target:
OBJECTS := $(foreach target, $(WITH_OBJECTS), $($(target)_OBJECTS))

# For each .o file, gcc generates a dependency file (.d).
# Keep only .o files and not .a files, using filter directive.
# Then, store all dependencies
DEPENDENCIES := $(patsubst %.o, %.d, $(filter %.o, $(OBJECTS)))

RUN_TARGETS ?= $(addprefix run-, $(PROGS))
VAL_TARGETS ?= $(addprefix valgrind-, $(PROGS))


# Για κάθε test (*_test) θέτουμε τις παρεμέτρους του (<test>_ARGS) από default σε --time
$(foreach test, $(filter %_test, $(PROGS)), $(eval $(test)_ARGS ?= --time))


# Default target, κάνει compile όλα τα εκτελέσιμα & τις βιβλιοθήκες
all: $(PROGS) $(LIBS)

# Αυτό χρειάζεται για να μπορούμε να χρησιμοποιήσουμε μεταβλητές στη λίστα των dependencies.
# Η χρήση αυτή απαιτεί διπλό "$$" στις μεταβλητές, πχ: $$(VAR), $$@
.SECONDEXPANSION:


# Για κάθε εκτελέσιμο <program>, δημιουργούμε έναν κανόνα που δηλώνει τα περιεχόμενα του
# <program>_OBJS ως depedencies του <program>. Το $@ περιέχει το όνομα του target,
# αλλά για να το χρησιμοποιήσουμε στη λίστα των dependencies χρειάζεται $$@ και .SECONDEXPANSION
#
$(PROGS): $$($$@_OBJECTS)
> $(CC)  $^ -o $@ $(LDFLAGS)

# Για κάθε βιβλιοθήκη <lib>, δημιουργούμε έναν κανόνα που δηλώνει τα περιεχόμενα του
# <lib>_OBJS ως depedencies του <lib>.
#
$(LIBS): $$($$@_OBJECTS)
> ar -rcs $@ $^

# Κάνουμε include τα .d αρχεία που παράγει ο gcc (το "-" αγνοεί την εντολή αν αποτύχει)
# Ενα αρχείο foo.d περιέχει όλα τα αρχεία (.c και .h) που χρειάστηκε o gcc για να κάνει compile
# το foo.o, στη μορφή του Makefile. Οπότε κάνοντας include το foo.d δηλώνουμε ότι αν οποιοδήποτε
# από τα αρχεία αυτά αλλάξει, πρέπει να ξανακάνουμε compile το foo.o.
#
-include $(DEPENDENCIES)

# Το make clean καθαρίζει οτιδήποτε φτιάχνεται από αυτό το Makefile
clean:
> @$(RM) $(PROGS) $(LIBS) $(OBJECTS) $(DEPENDENCIES) $(COV_FILES)
> @$(RM) -r coverage

# Για κάθε εκτελέσιμο <prog> φτιάχνουμε ένα target run-<prog> που το εκτελεί με παραμέτρους <prog>_ARGS
# Το run-% είναι "pattern rule", δημιουργεί έναν κανόνα για κάθε run-<foo>, θέτωντας το $* σε "foo".
run-%: %
> ./$* $($*_ARGS)

# Το make run εκτελεί όλα τα run targets
run: $(RUN_TARGETS)

# Για κάθε εκτελέσιμο <prog> φτιάχνουμε ένα target valgrind-<prog> που το εκτελεί μέσω valgrind με παραμέτρους <prog>_ARGS
valgrind-%: %
> valgrind --error-exitcode=1 --leak-check=full ./$* $($*_ARGS)

valgrind: $(VAL_TARGETS)


# Τα targets που ορίζονται από pattern rules (eg foo-%) δεν εμφανίζονται στο bash auto-complete. Τα παρακάτω κενά rules
# δεν επηρεάζουν σε τίποτα τη λειτουργία του Makefile, απλά επιτρέπουν στο auto-complete να "δει" αυτά τα targets.
$(RUN_TARGETS):
$(VAL_TARGETS):

.PHONY: clean run valgrind coverage

# all:
# > echo ".vars:$(.VARIABLES)<"
# > echo "filtred:$(filter %_OBJS,$(.VARIABLES))<"
# > echo "with0bjs:$(WITH_OBJECTS)< progs:$(PROGS)< libs:$(LIBS)<"
# > echo "objs:$(OBJECTS)<"

####################################################################################################
#
# Makefile
#
# Specifies the interface and its implementation.
#
####################################################################################################


# Interface:       ADTList
# Implementation:  LinkedList
LinkedList_ADTList_test_OBJECTS = ADTList_test.o $(MODULES)/LinkedList/ADTList.o

# Interface:       ADTMap
# Implementation:  HashTable
HashTable_ADTMap_test_OBJECTS = ADTMap_test.o $(MODULES)/HashTable/ADTMap.o $(MODULES)/LinkedList/ADTList.o


# All the tests share the following common makefile to keep this file as DRY (Don't Repeat Yourself)
# as possible.
include ../common.mk
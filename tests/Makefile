####################################################################################################
#
# Makefile
#
# Specifies the interface and its implementation.
#
####################################################################################################

# Interface:       ADTVector
# Implementation:  DynamicArray
DynamicArray_ADTVector_test_OBJECTS = ADTVector_test.o $(MODULES)/DynamicArray/ADTVector.o


# Interface:       ADTList
# Implementation:  LinkedList
LinkedList_ADTList_test_OBJECTS = ADTList_test.o $(MODULES)/LinkedList/ADTList.o

# Interface:       ADTBidirectionalList
# Implementation:  DoublyLinkedList
DoublyLinkedList_ADTBidirectionalList_test_OBJECTS = ADTBidirectionalList_test.o $(MODULES)/DoublyLinkedList/ADTBidirectionalList.o

# Interface:       ADTMap
# Implementation:  HashTable
# Dependencies:
#     | Interface | Implementation |
#     | --------- | -------------- |
#     | ADTList   | LinkedList     |
HashTable_ADTMap_test_OBJECTS = ADTMap_test.o $(MODULES)/HashTable/ADTMap.o $(MODULES)/LinkedList/ADTList.o

# Interface:       ADTOrderedSet
# Implementation:  SkipList
SkipList_ADTOrderedSet_test_OBJECTS = ADTOrderedSet_test.o $(MODULES)/SkipList/ADTOrderedSet.o
SkipList_ADTOrderedSet_test_ARGUMENTS = oset_create oset_insert

# All the tests share the following common makefile to keep this file as DRY (Don't Repeat Yourself)
# as possible.
include ../common.mk
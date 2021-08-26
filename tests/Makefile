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
# Dependencies:    ADTVector pcg_basic
SkipList_ADTOrderedSet_test_OBJECTS = ADTOrderedSet_test.o $(MODULES)/SkipList/ADTOrderedSet.o $(MODULES)/DynamicArray/ADTVector.o $(MODULES)/pcg-c-basic/pcg_basic.o

# Interface:       stack
# Implementation:  List
# Dependencies:    List
List_Stack_test_OBJECTS = stack_test.o $(MODULES)/List/stack.o $(MODULES)/LinkedList/ADTList.o

# Interface:       queue
# Implementation:  DoublyLinkedList
# Dependencies:    DoublyLinkedList
DoublyLinkedList_Queue_test_OBJECTS = queue_test.o $(MODULES)/DoublyLinkedList/queue.o $(MODULES)/DoublyLinkedList/ADTBidirectionalList.o

# Interface:       pqueue
# Implementation:  Heap
# Dependencies:    Vector
Heap_PQueue_test_OBJECTS = pqueue_test.o $(MODULES)/Heap/pqueue.o $(MODULES)/DynamicArray/ADTVector.o

# All the tests share the following common makefile to keep this file as DRY (Don't Repeat Yourself)
# as possible.
include ../common.mk
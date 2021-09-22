####################################################################################################
#
# Makefile
#
# Specifies the interface and its implementation.
#
####################################################################################################

# Interface:       vector
# Implementation:  DynamicArray
DynamicArray_Vector_test_OBJECTS = vector_test.o $(MODULES)/DynamicArray/vector.o

# Interface:       slist
# Implementation:  LinkedList
LinkedList_SList_test_OBJECTS = slist_test.o $(MODULES)/LinkedList/slist.o

# Interface:       list
# Implementation:  DoublyLinkedList
DoublyLinkedList_List_test_OBJECTS = list_test.o $(MODULES)/DoublyLinkedList/list.o

# Interface:       map
# Implementation:  HashTable
# Dependencies:    slist
HashTable_Map_test_OBJECTS = map_test.o $(MODULES)/HashTable/map.o $(MODULES)/LinkedList/slist.o

# Interface:       oset
# Implementation:  SkipList
# Dependencies:    vector pcg_basic
SkipList_OrderedSet_test_OBJECTS = oset_test.o $(MODULES)/SkipList/oset.o $(MODULES)/DynamicArray/vector.o $(MODULES)/pcg-c-basic/pcg_basic.o

# Interface:       stack
# Implementation:  SList
# Dependencies:    SList
SList_Stack_test_OBJECTS = stack_test.o $(MODULES)/LinkedList/stack.o $(MODULES)/LinkedList/slist.o

# Interface:       queue
# Implementation:  List
# Dependencies:    List
List_Queue_test_OBJECTS = queue_test.o $(MODULES)/DoublyLinkedList/queue.o $(MODULES)/DoublyLinkedList/list.o

# Interface:       pqueue
# Implementation:  Heap
# Dependencies:    vector
Heap_PQueue_test_OBJECTS = pqueue_test.o $(MODULES)/Heap/pqueue.o $(MODULES)/DynamicArray/vector.o

# All the tests share the following common makefile to keep this file as DRY (Don't Repeat Yourself)
# as possible.
include ../common.mk
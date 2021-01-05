//
//  BinList.c
//  lianxi
//
//  Created by bigbing on 2020/09/13.
//  Copyright © 2020 axe. All rights reserved.
//



#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "BinList.h"
#include "aeHashMap.h"


#pragma mark - type declarations
struct _BinListNode {
    void *element;
    BinListNode next;
};

struct _BinList {
    size_t length;
    BinListNode head, end;
};

#pragma mark - binlist node methods
void*
BinListNodeValue(BinListNode node) {
    return node->element;
}

void
BinListNodeNum(BinListNode node) {
    printf("%zu => ", (size_t)node->element);
    printf("\n");
}


BinListNode
BinListNext(BinListNode n) {
    return n->next;
}



#pragma mark - binlist methods

BinList
BinListNew() {
    BinList list = malloc(sizeof(struct _BinList));
    list->head = NULL;
    list->end = NULL;
    list->length = 0;
    return list;
}


void
BinListFree(BinList list) {
    BinListNode n = list->head;
    while (n != NULL) {
        BinListNode before = n;
        n = n->next;
        free(before);
    }
    list->head = NULL;
    list->end = NULL;
    list->length = 0;
    list = NULL;
}


size_t
BinListLength(BinList list){
    return list->length;
}


void
BinListAppend(BinList list, void *element) {
//    printf("aaa1\n");
    
    BinListNode n = malloc(sizeof(struct _BinListNode));
    n->element = element;
    n->next = NULL;
    
    if(list->head == NULL) {
        list->head = n;
    } else {
        list->end->next = n;
    }
//    printf("aaa2\n");
    
    list->end = n;
//    printf("aaa3\n");
    list->length += 1;
//    printf("aaa4\n");
}


void*
BinListElementByIndex(BinList list, size_t index) {
    size_t i = 0;
    BinListNode n = list->head;
    while (n != NULL) {
        if(index == i) {
            return n->element;
        }
        n = n->next;
        i += 1;
    }
    return NULL;
}

void*
BinListGetLastElement(BinList list) {
    size_t len = BinListLength(list);
    void* lastone = BinListElementByIndex(list, len - 1);
    return lastone;
}


void
BinListSetElementByIndex(BinList list, void* element, size_t index) {
    size_t i = 0;
    BinListNode n = list->head;
    while (n != NULL) {
        if(index == i) {
            n->element = element;
        }
        n = n->next;
        i += 1;
    }
}


size_t
BinListGetIndexByNumElement(BinList list, size_t value) {
    size_t i = 0;
    BinListNode n = list->head;
    while (n != NULL) {
        size_t e = (size_t)n->element;
        if(e == value) {
            return i;
        }
        n = n->next;
        i += 1;
    }
    return -1;
}



bool
BinListContains(BinList list, void* element) {
    BinListNode n = list->head;
    while (n != NULL) {
        if(element == n->element) {
            return true;
        }
        n = n->next;
    }
    return false;
}

void
BinListPrepend(BinList list, void* element) {
    BinListNode n = malloc(sizeof(struct _BinListNode));
    n->element = element;
    n->next = list->head;
    list->head = n;
    if(list->head == NULL) {
        list->end = n;
    }
    list->length += 1;
}

BinListNode
BinListPrepop(BinList list) {
    BinListNode res = list->head;
    
    if(list->head != NULL) {
        list->head = list->head->next;
    }
    if(list->end == res) {
        list->end = NULL;
    }
    
    list->length -= 1;
    return res;
}

void
BinListInsertElementAtIndex(BinList list, void* element, size_t index) {
    BinListNode n = list->head;
    BinListNode e = malloc(sizeof(struct _BinListNode));
    e->element = element;
    
    if(index == 0) {
        e->next = list->head;
        list->head = e;
        if(n == NULL) {
            list->end = e;
        }
    } else {
        BinListNode p = NULL;
        size_t i = 0;
        while (n != NULL) {
            if(index == i) {
              break;
            }
            p = n;
            n = n->next;
            i += 1;
        }
        e->next = p->next;
        p->next = e;
    }
    
    list->length += 1;
}

void
BinListLogString(char* tag, BinList list) {
    BinListNode n = list->head;
    printf("%s values : ", tag);
    while (n != NULL) {
        printf("%s => ", (char*)n->element);
        fflush(stdout);
        n = n->next;
    }
    printf("\n");
}

void
BinListLogMap(char* tag, BinList list) {
    BinListNode n = list->head;
    size_t index = 0;
    while (n != NULL) {
        printf("%s 第 %zu 个 map  : \n", tag, index);
        aeHashMapLog((aeHashMap)n->element);
        fflush(stdout);
        n = n->next;
        index += 1;
    }
    printf("\n");
}



void
BinListLogNum(char* tag, BinList list) {
    BinListNode n = list->head;
//    printf("keys : ");
    printf("%s nums : ", tag);
    while (n != NULL) {
        printf("%zu => ", (size_t)n->element);
        fflush(stdout);
        n = n->next;
    }
//    printf("BinListLogNum : ");
    fflush(stdout);
    
    printf("\n");
}

BinListNode
BinListHead(BinList list) {
    return list->head;
}

BinList
BinListSubList(BinList list, size_t start, size_t end){
    BinList new_list = BinListNew();
    BinListNode n = list->head;
    
    size_t index = 0;
    while (n != NULL) {
        if(start <= index && index < end ) {
            BinListAppend(new_list, n->element);
        }
        n = n->next;
        index += 1;
    }
    return new_list;
}

BinList
BinListExtend(BinList baseList, BinList toBeAddedList) {
    BinList list = BinListNew();
    for (size_t i = 0; i < BinListLength(baseList); i++) {
        void* one = BinListElementByIndex(baseList, i);
        BinListAppend(list, one);
    }
    
    for (size_t i = 0; i < BinListLength(toBeAddedList); i++) {
        void* one = BinListElementByIndex(toBeAddedList, i);
        BinListAppend(list, one);
    }
    
    return list;
}

BinList
BinListReverse(BinList orgList) {
    BinList list = BinListNew();
    size_t len = BinListLength(orgList);
    size_t i = len - 1;
    while ((int)i >= 0) {
        void* one = BinListElementByIndex(orgList, i);
        BinListAppend(list, one);
        i = i - 1;
    }
    return list;
}

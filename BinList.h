//
//  BinList.h
//
//  Created by bigbing on 2020/09/13.
//  Copyright © 2020 axe. All rights reserved.
//

#ifndef BinList_h
#define BinList_h

#include <stdio.h>

#pragma mark - defination

typedef struct _BinListNode *BinListNode;
typedef struct _BinList *BinList;

#pragma mark - binlist node methods


void*
BinListNodeValue(BinListNode node);

void
BinListNodeNum(BinListNode node);


#pragma mark - binlist methods


BinListNode
BinListNext(BinListNode n);


BinList
BinListNew(void);

size_t
BinListLength(BinList list);

void
BinListAppend(BinList list, void *element);

void*
BinListElementByIndex(BinList list, size_t index);

void
BinListFree(BinList list);

void
BinListPrepend(BinList list, void* element);

BinListNode
BinListPrepop(BinList list);

void
BinListInsertElementAtIndex(BinList list, void* element, size_t index);

void
BinListLogString(char* tag, BinList list);

void
BinListLogNum(char* tag, BinList list);


BinListNode
BinListHead(BinList list);

BinList
BinListSubList(BinList list, size_t start, size_t end);

size_t
BinListGetIndexByNumElement(BinList list, size_t value);

void
BinListSetElementByIndex(BinList list, void* element, size_t index);

BinList
BinListExtend(BinList baseList, BinList toBeAddedList);

BinList
BinListReverse(BinList parameters);

void*
BinListGetLastElement(BinList list);

void
BinListLogMap(char* tag, BinList list);

#endif /* BinList_h */

//
//  aeHashMap.h
//  Homework
//
//  Created by 范敏 on 2020/09/09.
//  Copyright © 2020 axe. All rights reserved.
//

#ifndef aeHashMap_h
#define aeHashMap_h

#include <stdio.h>
#include "aeString.h"
#include "BinList.h"

#pragma mark - defination
typedef struct _aeHashMap *aeHashMap;
typedef struct _aeHashMapNode *aeHashMapNode;
typedef struct _aeHashMapNodeList *aeHashMapNodeList;


#pragma mark - methods

aeHashMap
aeHashMapNew(void);

void
aeHashMapFree(aeHashMap map);

void
aeHashMapSet(aeHashMap map, aeString key, size_t value);

void
aeHashMapLog(aeHashMap map);

size_t
aeHashMapGet(aeHashMap map, aeString key);

bool
aeHashMapContains(aeHashMap map, aeString key);


BinList
aeHashMapGetKeylist(aeHashMap map);

#endif /* aeHashMap_h */

//
//  aeHashMap.c
//  Homework
//
//  Created by 大饼 on 2020/09/09.
//  Copyright © 2020 axe. All rights reserved.
//

#include "aeHashMap.h"
#include "aeString.h"
#include "BinList.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#pragma mark - defination

struct _aeHashMapNode {
    aeString key;
    size_t value;
    struct _aeHashMapNode* next;
};

struct _aeHashMapNodeList {
    struct _aeHashMapNode* head;
};

struct _aeHashMap {
    size_t table_size;
    aeHashMapNodeList table[17];
};


#pragma mark - map node list methods


aeHashMapNodeList
aeHashMapNodeListNew(void) {
    aeHashMapNodeList list = malloc(sizeof(struct _aeHashMapNodeList));
    list->head = malloc(sizeof(struct _aeHashMapNode));
    list->head->next = NULL;
    return list;
}

void
aeHashMapNodeListFree(aeHashMapNodeList list) {
    aeHashMapNode n = list->head->next;
       while (n != NULL) {
           aeHashMapNode before = n;
           n = n->next;
//           aeStringFree(before->key);
           free(before);
           before = NULL;
       }
       free(list->head);
        list->head = NULL;
       free(list);
        list = NULL;
}

void
aeHashMapNodeListAppend(aeHashMapNodeList l, aeHashMapNode an) {
    aeHashMapNode n = l->head;
    while (n->next != NULL) {
        n = n->next;
    }
    n->next = an;
    an->next = NULL;
}


size_t
aeHashMapNodeListLength(aeHashMapNodeList l) {
    size_t len = 0;
    aeHashMapNode n = l->head;
    while (n->next != NULL) {
        n = n->next;
        len += 1;
    }
    return len;
}


#pragma mark - map methods


aeHashMap
aeHashMapNew(void){
    aeHashMap map = malloc(sizeof(struct _aeHashMap));
    map->table_size = 17;
    for (size_t i=0; i < 17; i++) {
        aeHashMapNodeList list = aeHashMapNodeListNew();
        map->table[i] = list;
    }
    return map;
}

void
aeHashMapFree(aeHashMap map) {
    for (size_t i=0; i < 17; i++) {
        aeHashMapNodeList list = map->table[i];
        aeHashMapNodeListFree(list);
    }
    free(map);
    map = NULL;
}


size_t
aeHashMapHash(aeString key) {
//    printf("aeHashMapHash %p \n", key);
//    fflush(stdout);
    
    size_t total = 0;
    size_t factor = 1;
    size_t len = aeStringLength(key);
    for (size_t i = 0; i < len; i++) {
        char c = aeStringGetCharByIndex(key, i);
        total += c * factor;
        factor *= 10;
    }
    return total;
}

void
aeHashMapInsertOrUpdate(aeHashMap map, aeString key, size_t value, size_t index) {
    //添加或者更新 O(1)
    //虽然有循环 但是相对于整个 hash table 来说，这个操作是 O(1)

    size_t v = aeHashMapGet(map, key);
    if(v == -1){
        aeHashMapNode n = malloc(sizeof(struct _aeHashMapNode));
           n->key = key;
           n->value = value;
           // 在下标处插入元素
        aeHashMapNodeListAppend((map->table)[index], n);
    } else {
        aeHashMapNodeList l = (map->table)[index];
        aeHashMapNode n = l->head;
        while (n->next != NULL) {
            n = n->next;
            if(aeStringEqual(n->key, key)) {
                n->value = value;
                break;
            }
        }
    }

}


void
aeHashMapSet(aeHashMap map, aeString key, size_t value) {
    // 先计算出下标
    size_t index = aeHashMapHash(key) % map->table_size;
    aeHashMapInsertOrUpdate(map, key, value, index);
}


void
aeHashMapNodeListShow(aeHashMapNodeList l) {
    aeHashMapNode n = l->head->next;
    while (n != NULL) {
        printf("%s, %zu", aeStringData(n->key), n->value);
        n = n->next;
    }
    
    n = l->head->next;
    while (n != NULL) {
        n = n->next;
    }
    printf("\n");
}


void
aeHashMapLog(aeHashMap map) {
    size_t size = map->table_size;
    for(size_t i= 0; i < size; i++){
        aeHashMapNodeList list = (map->table)[i];
        if(list->head->next != NULL) {
            printf("第%zu个 aeHashMapLog: ", i);
            aeHashMapNodeListShow(list);
        }
    }
}


size_t
aeHashMapGet(aeHashMap map, aeString key) {
//    printf("aeHashMapGet %p \n", key);
//    fflush(stdout);
    
    // 先计算出下标
    size_t index = aeHashMapHash(key) % map->table_size;

//    printf("get index %d  aeHashMapHash(key) %zu\n", index, aeHashMapHash(key));
//    fflush(stdout);
    

    
    // 找到元素
    aeHashMapNodeList list = (map->table)[index];
    aeHashMapNode n = list->head->next;
    while (n != NULL) {
        if(aeStringEqual(n->key, key)) {
            return n->value;
        }
        n = n->next;
    }
    return -1;
}


bool
aeHashMapContains(aeHashMap map, aeString key) {
    // 先计算出下标
    size_t index = aeHashMapHash(key) % map->table_size;
    // 找到元素
    aeHashMapNodeList list = (map->table)[index];
    aeHashMapNode n = list->head->next;
    while (n != NULL) {
        if(aeStringEqual(n->key, key)) {
            return true;
        }
        n = n->next;
    }
    return false;
}


BinList
aeHashMapGetKeylist(aeHashMap map) {
    BinList keyList = BinListNew();
    
    for(size_t i = 0; i < 17; i++) {
        aeHashMapNodeList list = (map->table)[i];
        aeHashMapNode n = list->head->next;
        
        while (n != NULL) {
            BinListAppend(keyList, n->key);
            n = n->next;
        }
        
    }
    return keyList;
}

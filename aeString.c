//
//  aeString.c
//  Homework
//
//  Created by 范敏 on 2020/09/02.
//  Copyright © 2020 axe. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "aeString.h"


#pragma mark - type defination

struct _aeString {
    char *data;
    size_t length;
};


struct _aeStringListNode {
    aeString as;
    struct _aeStringListNode* next;
};

struct _aeStringList {
    struct _aeStringListNode* head;
};

#pragma mark - aeString

aeString
aeStringNewWithChars(char *s) {
    size_t len = strlen(s);
    char *s1 = malloc(len + 1);
    strncpy(s1, s, len);
    s1[len] = '\0';
    
    aeString as = malloc(sizeof(struct _aeString));
    as->data = s1;
    as->length = len;
    return as;
}

void
aeStringFree(aeString as) {
    free(as->data);
    as->data = NULL;
    free(as);
    as = NULL;
}

size_t
aeStringLength(aeString a) {
//    printf("aeStringLength %p \n", a);
//    fflush(stdout);
    
    return a->length;
}

char *
aeStringData(aeString a) {
    return a->data;
}


aeString
aeStringConcat(aeString as1,aeString as2){
    
    size_t len1 = aeStringLength(as1);
    size_t len2 = aeStringLength(as2);
    size_t len3 = len1 + len2;
    
    char *s3 = malloc(len3 + 1);
    strcat(s3, aeStringData(as1));
    strcat(s3, aeStringData(as2));
    s3[len3] = '\0';
    
    aeString as3 = malloc(sizeof(struct _aeString));
    as3->data = s3;
    as3->length = len3;    
    return as3;
}

void
aeStringLog(char* tag, aeString as) {
    printf("%s : %s\n", tag, as->data);
}

aeString
aeStringCopy(aeString s) {
    aeString new_str = aeStringNewWithChars(s->data);
    return new_str;
}


aeString
aeStringAddChar(aeString s, char c){
    size_t len = strlen(s->data);
    char *s1 = malloc(len + 1 + 1);
    strncpy(s1, s->data, len);
    s1[len] = c;
    s1[len + 1] = '\0';
    
    aeString as = malloc(sizeof(struct _aeString));
    as->data = s1;
    as->length = len + 1;
    
    aeString new_str = aeStringNewWithChars(s1);
    return new_str;
}

char
aeStringGetCharByIndex(aeString s, size_t index) {

    char result = s->data[index];
    
    return result;
}


aeString
aeStringSubstring(aeString s, size_t start, size_t end){
    size_t len = s->length;

    if(start < 0) {
        aeString as = aeStringNewWithChars("");
        return as;
    } else if(end > len) {
        aeString as = aeStringSubstring(s, start, len);
        return as;
    } else {
        size_t index = 0;
        char* new_str = malloc(end - start + 1);
        
        for (size_t i = 0; i < len; i++) {
            if(i >= start && i < end) {
                new_str[index] = s->data[i];
                index += 1;
            }
        }
        new_str[end - start] = '\0';
        aeString as = aeStringNewWithChars(new_str);
        return as;
    }
    
}


aeString
aeStringAppend(aeString s1, aeString s2) {
    char* cs = s2->data;
    aeString str = aeStringNewWithChars(s1->data);
    for(size_t i = 0; i < s2->length ; i++) {
        char c = cs[i];
        str = aeStringAddChar(str, c);
    }
    return str;
}

bool
aeStringEqual(aeString s1, aeString s2) {
    return strcmp(s1->data, s2->data) == 0;
}

bool
aeStringEqualWithRawString(aeString s1, char* s2) {
    return strcmp(s1->data, s2) == 0;
}


size_t
aeStringToInt(aeString s1) {
    return atoi(s1->data);
}


#pragma mark - aeString List methods
aeStringList
aeStringListNew(void) {
    aeStringList list = malloc(sizeof(struct _aeStringList));
    list->head = malloc(sizeof(struct _aeStringList));
    list->head->next = NULL;
    return list;
}


void
aeStringListFree(aeStringList list) {
    aeStringListNode n = list->head->next;
    while (n != NULL) {
       aeStringListNode before = n;
       aeStringFree(before->as);
       n = n->next;
       free(before);
    }
    free(list->head);
    list->head = NULL;
    free(list);
    list = NULL;
}


aeStringList
aeStringSplit(aeString s, aeString delimeter) {
    aeStringList list = aeStringListNew();
    size_t i = 0;
    aeString toAdd = aeStringNewWithChars("");
    while (i < s->length + 1) {
        size_t start = i; //0
        size_t end = i + delimeter->length; //0+1 = 1
        aeString substr = aeStringSubstring(s, start, end); //s
        if(!aeStringEqual(delimeter, substr)) {
            toAdd = aeStringAddChar(toAdd, s->data[i]);
            if(end == s->length + 1) {
                aeStringListAppend(list, toAdd);
            }
        } else {
            aeStringListAppend(list, toAdd);
            toAdd = aeStringNewWithChars("");
            i += delimeter->length - 1;
        }
        i += 1;
    }
    return list;
}


aeString
aeStringReplace(aeString s, aeString from, aeString to) {
    aeStringList list = aeStringSplit(s, from);
    size_t len = aeStringListLength(list);
    aeString new_str = aeStringNewWithChars("");
    for(size_t i = 0; i < len;i++) {
        aeString one = aeStringListGetByIndex(list, i);
        new_str = aeStringAppend(new_str, one);
        if(i < len - 1) {
            new_str = aeStringAppend(new_str, to);
        }
    }
    aeStringLog("new_str", new_str);
    return new_str;
}




void
aeStringListAppend(aeStringList l, aeString as) {
    aeStringListNode n = l->head;
    while (n->next != NULL) {
        n = n->next;
    }
    
    aeStringListNode new_node = malloc(sizeof(struct _aeStringListNode));
    new_node->as = as;
    n->next = new_node;
    n->next->next = NULL;
}

void
aeStringListShow(aeStringList l) {
    aeStringListNode n = l->head->next;
    printf("队列里面的内容是：");
    while (n != NULL) {
        printf("%s,", n->as->data);
        n = n->next;
    }
    printf("\n");
    printf("队列长度是 %zu \n", aeStringListLength(l));
}

size_t
aeStringListLength(aeStringList aslist) {
    size_t size = 0;
    aeStringListNode n = aslist->head->next;
    while (n != NULL) {
        n = n->next;
        size += 1;
    }
    return size;
}

aeString
aeStringListGetByIndex(aeStringList aslist, size_t index) {
       size_t size = 0;
       aeStringListNode n = aslist->head->next;
       while (n != NULL) {
           if(index == size) {
               return n->as;
           }
           n = n->next;
           size += 1;
       }
       return NULL;
}

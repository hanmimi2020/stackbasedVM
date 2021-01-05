//
//  aeString.h
//  Homework
//
//  Created by 范敏 on 2020/09/02.
//  Copyright © 2020 axe. All rights reserved.
//

#ifndef aeString_h
#define aeString_h

#include <stdio.h>
#include <stdbool.h>

#pragma mark - type defination
typedef struct _aeString *aeString;
typedef struct _aeStringListNode *aeStringListNode;
typedef struct _aeStringList *aeStringList;


#pragma mark - aeStrinn methods

aeString
aeStringNewWithChars(char *s);

size_t
aeStringLength(aeString a);

char*
aeStringData(aeString a);

aeString
aeStringConcat(aeString s1,aeString s2);

void
aeStringLog(char* tag, aeString as);

aeString
aeStringReplace(aeString as, aeString from, aeString to);

aeString
aeStringCopy(aeString s);

aeString
aeStringAddChar(aeString s, char c);

char
aeStringGetCharByIndex(aeString s, size_t index);

aeString
aeStringSubstring(aeString s, size_t start, size_t end);

aeString
aeStringAppend(aeString s1, aeString s2);

bool
aeStringEqual(aeString s1, aeString s2);

size_t
aeStringToInt(aeString s1);

void
aeStringFree(aeString as);

#pragma mark - aeStringList methods

aeStringList
aeStringSplit(aeString s, aeString delimeter);


bool
aeStringEqualWithRawString(aeString s1, char* s2);



aeStringList
aeStringListNew(void);

void
aeStringListAppend(aeStringList l, aeString as);


void
aeStringListShow(aeStringList aslist);

size_t
aeStringListLength(aeStringList aslist);

aeString
aeStringListGetByIndex(aeStringList aslist, size_t index);

void
aeStringListFree(aeStringList list);

#endif /* aeString_h */

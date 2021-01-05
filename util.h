//
//  util.h
//  Homework
//
//  Created by 范敏 on 2020/09/10.
//  Copyright © 2020 axe. All rights reserved.
//

#ifndef util_h
#define util_h

#include <stdio.h>
#include <stdbool.h>

char*
replaceStringWithChar(char* str, char c1, char c2);

bool
isDigit(char* str);

char*
removeStringWithChar(char* str, char c);

char*
intToString(size_t n);

#endif /* util_h */

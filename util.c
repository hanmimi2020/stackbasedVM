//
//  util.c
//  Homework
//
//  Created by 范敏 on 2020/09/10.
//  Copyright © 2020 axe. All rights reserved.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "util.h"


char*
replaceStringWithChar(char* str, char c1, char c2) {
    char* new_str = malloc(300);
    strcpy(new_str, str);
    size_t len = strlen(str);
    
    for (size_t i=0; i<len; i++) {
        if(new_str[i] == c1) {
            new_str[i] = c2;
        }
    }
    return new_str;
}

//char*
//replaceStringWithChar(char* str, char c1, char c2) {
//    size_t len = strlen(str);
////    printf("lenlen %zu \n", len);
////    fflush(stdout);
//    char *new_str= malloc(len+1);
//    new_str[len] = '\0';
//    for (size_t i=0; i< len; i++) {
//        if(str[i] == c1) {
//            new_str[i] = c2;
//        } else {
//            new_str[i] = str[i];
//        }
//    }
//    return new_str;
//}


bool
isDigit(char* str) {
    char* nums = "0123456789";
    bool flg = true;
    for(size_t i = 0 ; i < strlen(str); i++) {
        size_t index = 0;
        for(size_t j = 0 ; j < strlen(nums); j++) {
            index += 1;
            if(str[i] == nums[j]) {
                break;
            }
        }
//        printf("index %zu \n", index);
        if(index >= strlen(nums) && str[i] != '9') {
            flg = false;
            break;
        }
    }
//    printf("flg %d str %s\n", flg, str);
    return flg;
}


char*
removeStringWithChar(char* str, char c){
    char* new_str = malloc(300);
    size_t len = strlen(str);
    
    int index = 0;
    for (size_t i = 0; i<len; i++) {
        if(str[i] != c) {
//            printf("str[i] <%c>, <%c> \n", str[i], c);
            new_str[index] = str[i];
            index += 1;
        }
    }
    new_str[len] = '\0';
    return new_str;
}


char*
intToString(size_t n) {
    int dividend = 10;
    int remainder = n % dividend;
    int length = 1;

    while (remainder != n) {
        dividend = dividend * 10;
        remainder = n % dividend;
        length = length + 1;
    }
    char *nStr = malloc(sizeof(char) * (length + 1));
    sprintf(nStr, "%ld", n);
    // return nStr;
    return nStr;
}

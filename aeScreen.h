//
//  aeScreen.h
//  Homwork5
//
//  Created by 范敏 on 2020/09/21.
//

#ifndef aeScreen_h
#define aeScreen_h

#include <stdio.h>
#include "raylib.h"
#include "BinList.h"


#pragma mark - type defination

typedef struct _aeScreen *aeScreen;
typedef struct _aeRect *aeRect;



#pragma mark - methods


//aeScreen
//aeScreenNew(size_t pixel_width, size_t pixel_height, BinList memory, size_t start_index);
aeScreen
aeScreenNew(size_t pixel_width, size_t pixel_height, BinList memory, size_t start_index);
    
void
aeScreenDrawRect(aeRect rect);

aeRect
aeRectNew(size_t positionX, size_t positionY, size_t width, size_t height, Color color);


void
aeScreenFree(aeScreen s);

void
aeScreenRun(aeScreen s);

void
aeScreenUpdate(aeScreen s, BinList memory);

#endif /* aeScreen_h */

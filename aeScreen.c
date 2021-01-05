//
//  aeScreen.c
//  Homwork5
//
//  Created by 范敏 on 2020/09/21.
//
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "aeScreen.h"
#include "raylib.h"
#include "BinList.h"


#pragma mark - type defination

struct _aeScreen {
    size_t screenWidth;
    size_t screenHeight;
    size_t fps;
    BinList memory;
    size_t start_index;
    size_t pixel_gain;
};

struct _aeRect {
    size_t positionX;
    size_t positionY;
    size_t width;
    size_t height;
    Color color;
};


#pragma mark - methods

aeScreen
aeScreenNew(size_t pixel_width, size_t pixel_height, BinList memory, size_t start_index) {
    aeScreen s = malloc(sizeof(struct _aeScreen));
    s->fps = 60;
    s->screenWidth = pixel_width;
    s->screenHeight = pixel_height;
    s->memory = memory;
    InitWindow((int)(s->screenWidth), (int)(s->screenHeight), "AxeWriting");
    SetTargetFPS((int)(s->fps));
    s->start_index = start_index;
    s->pixel_gain = 20;
    return s;
}

void
aeScreenFree(aeScreen s) {
    CloseWindow();
    s->fps = 0;
    s->screenWidth = 0;
    s->screenHeight = 0;
    BinListFree(s->memory);
    s->start_index = 0;
    s->pixel_gain = 0;
    free(s);
    s = NULL;
}

aeRect
aeRectNew(size_t positionX, size_t positionY, size_t width, size_t height, Color color){
    aeRect r = malloc(sizeof(struct _aeRect));
    r->color = color;
    r->positionX = positionX;
    r->positionY = positionY;
    r->width = width;
    r->height = height;
    return r;
}


void
aeRectFree(aeRect r) {
    r->positionX = 0;
    r->positionY = 0;
    r->width = 0;
    r->height = 0;
    free(r);
    r = NULL;
}

void
aeScreenDrawRect(aeRect rect){
    DrawRectangle((int)rect->positionX, (int)rect->positionY, (int)rect->width, (int)rect->height, rect->color);
}

void
printTime() {
    time_t timer;
    char buffer[26];
    struct tm* tm_info;

    timer = time(NULL);
    tm_info = localtime(&timer);

    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    puts(buffer);

}

void
aeScreenRun(aeScreen s) {
    BinList m = s->memory;
    size_t len = BinListLength(m);
    
//    start = self.start_index
//    w = self.pixel_width
//    h = self.pixel_height
//    g = self.pixel_gain
//
//    for y in range(h):
//        for x in range(w):
//            if self.memory[start + y * w + x] != 0:
//                byte_value = self.memory[start + y * w + x]
//                color = color_from_byte_value(byte_value)
//                pygame.draw.rect(screen, color, ((x * g, y * g), (g, g)), 0)
    
    size_t start = 51200;
    size_t w = 10;
    size_t h = 10;
    size_t g = s->pixel_gain;
    
    while (!WindowShouldClose()) {
        printf("aeScreenRun\n");
        printTime();
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        for (size_t y = 0; y < h; y++) {
            for (size_t x = 0; x < w; x ++) {
                size_t displayValue = (int)BinListElementByIndex(m, start + y * w + x);
                if(displayValue != 0) {
                    Color c = {
                        displayValue,
                        0,
                        0,
                        200
                    };
                    aeRect rect = aeRectNew(g * x, g * y, g, g, c);
                    aeScreenDrawRect(rect);
                    aeRectFree(rect);
                }
            }
        }
        EndDrawing();
    }
}

void
aeScreenUpdate(aeScreen s, BinList memory) {
    s->memory = memory;
}

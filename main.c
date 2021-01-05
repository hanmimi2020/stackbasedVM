//
//  main.c
//  StackVm
//
//  Created by bigBing on 2020/09/26.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "aeString.h"
#include "util.h"
#include "BinList.h"
#include "stackVM.h"
#include "aeScreen.h"
#include "Assembler.h"



void
test_stack_machine() {
    aeString filename = aeStringNewWithChars("./programs/while.stack");
    FILE *fp;
    char* path = aeStringData(filename);
    if((fp = fopen(path,"r")) == NULL) {
        printf("Open Falied!");
        return;
    }
    
    const size_t line_size = 300;
    char* line = malloc(line_size);
    BinList program = BinListNew();
    
    while (fgets(line, line_size, fp) != NULL)  {
        char* str_line = malloc(300);
        strcpy(str_line, line);
        BinListAppend(program, str_line);
    }
    free(line);

    BinList memory = assemblerCompile(program);
    BinListFree(program);
    
    
    stackVirtualMachine vm = stackVirtualMachineNew(memory);
    stackVMRun(vm);

////    pthread_t tid;
////    pthread_create(&tid, NULL, stackVMRun, (void*)vm);
//
    //run虚拟机
        aeScreen s = aeScreenNew(200, 200, memory, 0);
        aeScreenRun(s);
//
    BinListFree(memory);
    fclose(fp);
}

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    test_stack_machine();
    return 0;
}

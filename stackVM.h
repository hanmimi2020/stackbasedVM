//
//  stackVM.h
//  StackVm
//
//  Created by 范敏 on 2020/09/30.
//

#ifndef stackVM_h
#define stackVM_h

#include <stdio.h>
#include "aeString.h"
#include "BinList.h"


#include <stdio.h>

struct _stackVirtualMachine {
    size_t pa;
    BinList memory;
    BinList operand_stack;
    BinList return_stack;
    BinList parameter_stack;
    BinList return_value_stack;
    BinList variable_stack;
};
typedef struct _stackVirtualMachine *stackVirtualMachine;

void *
stackVMRun(void* vmachine);

stackVirtualMachine
stackVirtualMachineNew(BinList memory);

void
stackVirtualMachineFree(stackVirtualMachine vm);


#endif /* stackVM_h */

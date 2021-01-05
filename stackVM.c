//
//  stackVM.c
//  StackVm
//
//  Created by 范敏 on 2020/09/30.
//

#include "stackVM.h"
#include "aeString.h"
#include "BinList.h"
#include "Assembler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

stackVirtualMachine
stackVirtualMachineNew(BinList memory) {
    stackVirtualMachine vm = malloc(sizeof(struct _stackVirtualMachine));
    vm->memory = memory;
    vm->operand_stack = BinListNew();
    //存函数调用后的返回地址
    vm->return_stack = BinListNew();
    vm->parameter_stack = BinListNew();
    vm->return_value_stack = BinListNew();
    vm->variable_stack = BinListNew();
    vm->pa = 0;
    return vm;
}

void
stackVirtualMachineFree(stackVirtualMachine vm) {
    BinListFree(vm->memory);
    BinListFree(vm->operand_stack);
    vm->pa = 0;
    free(vm);
    vm = NULL;
}

void*
stackVmOpStackPop(stackVirtualMachine vm){
    BinList list = vm->operand_stack;
    void* lastone = BinListElementByIndex(list, BinListLength(list) - 1);
    list = BinListSubList(list, 0, BinListLength(list) - 1);
    vm->operand_stack = list;
    return lastone;
}

void*
stackVmReturnStackPop(stackVirtualMachine vm){
    BinList list = vm->return_stack;
    void* lastone = BinListElementByIndex(list, BinListLength(list) - 1);
    list = BinListSubList(list, 0, BinListLength(list) - 1);
    vm->return_stack = list;
    return lastone;
}

void*
stackVmParamStackPop(stackVirtualMachine vm){
    BinList list = vm->parameter_stack;
    void* lastone = BinListElementByIndex(list, BinListLength(list) - 1);
    list = BinListSubList(list, 0, BinListLength(list) - 1);
    vm->parameter_stack = list;
    return lastone;
}

void*
stackVmReturnValueStackPop(stackVirtualMachine vm){
    BinList list = vm->return_value_stack;
    void* lastone = BinListElementByIndex(list, BinListLength(list) - 1);
    list = BinListSubList(list, 0, BinListLength(list) - 1);
    vm->return_value_stack = list;
    return lastone;
}


void*
stackVMMemoryPopReturnStack(stackVirtualMachine vm){
    BinList list = vm->return_stack;
    void* lastone = BinListElementByIndex(list, BinListLength(list) - 1);
    list = BinListSubList(list, 0, BinListLength(list) - 1);
    vm->return_stack = list;
    return lastone;
}

void*
stackVMMemoryPopVariableStack(stackVirtualMachine vm){
    BinList list = vm->variable_stack;
    void* lastone = BinListElementByIndex(list, BinListLength(list) - 1);
    list = BinListSubList(list, 0, BinListLength(list) - 1);
    vm->variable_stack = list;
    return lastone;
}



void
do_push(stackVirtualMachine vm) {
    size_t element =  (int)BinListElementByIndex(vm->memory, vm->pa + 1);
    BinListAppend(vm->operand_stack, (void *)element);
    vm->pa += 2;
}

void
do_pop(stackVirtualMachine vm) {
    stackVmOpStackPop(vm);
    vm->pa += 1;
}


void
do_save_to_memory(stackVirtualMachine vm) {
    void* value = stackVmOpStackPop(vm);
    void* address = stackVmOpStackPop(vm);
    printf("value %ld, address %ld \n", (size_t)value, (size_t)address);
    vm->pa += 1;
    BinListSetElementByIndex(vm->memory,(void*)value, (size_t)address);
}


void
do_add(stackVirtualMachine vm) {
    size_t value_b = (size_t)stackVmOpStackPop(vm);
    size_t value_a = (size_t)stackVmOpStackPop(vm);
    size_t _sum = value_a + value_b;
    printf("do_add value_a %zu value_b %zu value_sums %zu\n", value_a, value_b, _sum);
    
    BinListAppend(vm->operand_stack, (void *)_sum);
    
    vm->pa += 1;
}

void
do_compare(stackVirtualMachine vm) {
    size_t b = (size_t)stackVmOpStackPop(vm);
    size_t a = (size_t)stackVmOpStackPop(vm);

    
    if(a > b) {
        BinListAppend(vm->operand_stack, (void *)2);
    } else if( a == b) {
        BinListAppend(vm->operand_stack, (void *)1);
    } else {
        BinListAppend(vm->operand_stack, (void *)0);
    }
    
    vm->pa += 1;

}


void
do_jump_if_great(stackVirtualMachine vm) {
    size_t address =  (size_t)stackVmOpStackPop(vm);
    size_t condition =  (size_t)stackVmOpStackPop(vm);

    if(condition == 2) {
        vm->pa = address;
    } else{
        vm->pa += 1;

    }
}


void
do_jump_if_less(stackVirtualMachine vm) {
    size_t address =  (size_t)stackVmOpStackPop(vm);
    size_t condition =  (size_t)stackVmOpStackPop(vm);

    if(condition == 0) {
        vm->pa = address;
    } else{
        vm->pa += 1;
    }
}

void
do_jump(stackVirtualMachine vm) {
    size_t address =  (size_t)stackVmOpStackPop(vm);
    vm->pa = address;
}

void
do_exit(stackVirtualMachine vm) {
//    BinListLogNum(vm->operand_stack);
    size_t return_address =  (size_t)stackVmReturnStackPop(vm);
//    printf("return_address %zu \n", return_address);
    
    vm->pa = return_address;
}

void
do_push_parameter_stack(stackVirtualMachine vm) {
//    BinListLogNum(vm->operand_stack);
    void* p = stackVmOpStackPop(vm);
    BinListAppend(vm->parameter_stack, p);
    vm->pa += 1;
}

void
do_pop_parameter_stack(stackVirtualMachine vm) {
    void* p = stackVmParamStackPop(vm);
    BinListAppend(vm->operand_stack, p);
    vm->pa += 1;
}


void
do_subroutine_call(stackVirtualMachine vm) {
    size_t subroutine_address = (size_t)stackVmOpStackPop(vm);
    size_t return_address = vm->pa + 1;
    BinListAppend(vm->return_stack, (void *)return_address);
    vm->pa = subroutine_address;
}

void
do_push_return_value_stack(stackVirtualMachine vm) {
//    BinListLogNum(vm->operand_stack);
    void* p = stackVmOpStackPop(vm);
    BinListAppend(vm->return_value_stack, p);
    vm->pa += 1;
}

void
do_pop_return_value_stack(stackVirtualMachine vm) {
    BinListLogNum("do_pop_return_value_stack", vm->return_value_stack);
    void* p = stackVmReturnValueStackPop(vm);
    BinListAppend(vm->operand_stack, p);
    vm->pa += 1;
}


void
do_push_variable_stack(stackVirtualMachine vm) {
    void* p = stackVmOpStackPop(vm);
    printf("do_push_variable_stack pointer %p \n", p);
    printf("do_push_variable_stack %lu \n", (size_t)p);
    BinListAppend(vm->variable_stack, p);
    vm->pa += 1;
}

void
do_pop_variable_stack(stackVirtualMachine vm) {
//    BinListLogNum(vm->return_value_stack);
    void* p = stackVMMemoryPopVariableStack(vm);
    BinListAppend(vm->operand_stack, p);
    vm->pa += 1;
}

void*
stackVmVariableStackTop(stackVirtualMachine vm, size_t offset) {
    size_t len = BinListLength(vm->variable_stack);
    void* value = BinListElementByIndex(vm->variable_stack, len - offset - 1);
    return value;
}

void
stackVmVariableStackReplace(stackVirtualMachine vm, size_t offset, size_t value) {
    size_t len = BinListLength(vm->variable_stack);
//    void* value = BinListElementByIndex(vm->variable_stack, len - offset - 1);
    BinListSetElementByIndex(vm->variable_stack, (void*)value, len - offset - 1);
}

void
do_load_from_variable_stack(stackVirtualMachine vm) {
    size_t offset = (size_t)stackVmOpStackPop(vm);
    void* value = stackVmVariableStackTop(vm, offset);
//    printf("load_from_variable_stack offset %zu  value %zu", (size_t)offset, (size_t)value);
    BinListAppend(vm->operand_stack, value);
    vm->pa += 1;
}

void
do_save_to_variable_stack(stackVirtualMachine vm) {
    size_t offset = (size_t)stackVmOpStackPop(vm);
    size_t value = (size_t)stackVmOpStackPop(vm);
    stackVmVariableStackReplace(vm, offset, value);
//    printf("do_save_to_variable_stack offset %zu  value %zu", (size_t)offset, (size_t)value);
    vm->pa += 1;
}





void *
stackVMRun(void* vmachine) {
    stackVirtualMachine vm = (stackVirtualMachine)vmachine;
    size_t count = 0;
    while(true) {
//        if(count >= 260) {
//            break;
//        }
        count += 1;
        size_t instruction = (size_t)BinListElementByIndex(vm->memory, vm->pa);
        printf("instruction %zu  pa %zu \n", instruction, vm->pa);
        if(instruction == halt) {
            break;
        } else if(instruction == push) {
            do_push(vm);
        } else if(instruction == pop) {
            do_pop(vm);
        } else if(instruction == save_to_memory) {
            do_save_to_memory(vm);
        } else if(instruction == add) {
            do_add(vm);
        } else if(instruction == compare) {
            do_compare(vm);
        } else if(instruction == jump_if_great) {
            do_jump_if_great(vm);
        } else if(instruction == jump_if_less) {
            do_jump_if_less(vm);
        } else if(instruction == jump) {
            do_jump(vm);
        } else if(instruction == __exit) {
            do_exit(vm);
        } else if(instruction == subroutine_call) {
            do_subroutine_call(vm);
            
        } else if(instruction == push_parameter_stack) {
            do_push_parameter_stack(vm);
            
        } else if(instruction == pop_parameter_stack) {
            do_pop_parameter_stack(vm);
        } else if(instruction == pop_return_value_stack) {
            do_pop_return_value_stack(vm);
        } else if(instruction == push_return_value_stack) {
            do_push_return_value_stack(vm);
        } else if(instruction == push_variable_stack) {
            do_push_variable_stack(vm);
        } else if(instruction == pop_variable_stack) {
            do_pop_variable_stack(vm);
        } else if(instruction == load_from_variable_stack) {
            do_load_from_variable_stack(vm);
        } else if(instruction == save_to_variable_stack) {
            do_save_to_variable_stack(vm);
        }
    }
            
    return NULL;
}

//
//  Assembler.h
//  StackVm
//
//  Created by 范敏 on 2020/10/08.
//

#ifndef Assembler_h
#define Assembler_h

#include <stdio.h>
#include "BinList.h"
#include "aeHashMap.h"

#pragma mark - defination of fake instrctions
#define _var ".var"
#define _add ".add"
#define _var_to_operand_stack ".var_to_operand_stack"
#define _function ".function"
#define _call ".call"
#define _return ".return"
#define _function_end ".function_end"
#define _if ".if"
#define _if_end ".if_end"
#define _while ".while"
#define _while_end ".while_end"

typedef struct _assembler *assembler;

#pragma mark - defination of registers and instrctions

enum registers {
    pa = 0b00000000,
};

enum instructions {
    push = 0b00000000, //0
    pop = 0b00000001, //1
    add = 0b00000010, //2
    save_to_memory = 0b00000011,//3
    halt = 0b00000100, //4
    
    
    jump = 0b00000111, //7
    jump_if_great = 0b00001000, //8
    jump_if_less = 0b00011000, //24
    
    compare = 0b00001001, //9
    subroutine_call = 0b00001010, //10
    __exit = 0b00001011, //11
    push_parameter_stack = 0b00001100, //12
    pop_parameter_stack = 0b00001101, //13
    pop_return_value_stack = 0b00001110, //14
    push_return_value_stack = 0b00001111, //15
    
    push_variable_stack = 0b00010000,//16
    pop_variable_stack = 0b00010001,//17
    load_from_variable_stack = 0b00010010,//18
    save_to_variable_stack = 0b00010011,//19
};




#pragma mark - defination of methods
BinList
assemblerCompile(BinList program);

BinList
assemblerExtendFakeInstructions(BinList instructions, assembler assem);

#endif /* Assembler_h */

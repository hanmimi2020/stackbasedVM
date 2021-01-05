//
//  Assembler.c
//  StackVm
//
//  Created by 范敏 on 2020/10/08.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "Assembler.h"
#include "stackVM.h"
#include "aeString.h"
#include "BinList.h"
#include "util.h"
#include "aeHashMap.h"
#define STR(var) #var
#define atoa(x) #x




struct _assemblerFakeRes {
    BinList extended; //伪指令展开的原生汇编指令
    size_t end_index; //这一次处理伪指令扫过的所有指令的最后一行的位置
};
typedef struct _assemblerFakeRes *assemblerFakeRes;



struct _assembler {
    BinList symbol_stack;
    size_t index;
    BinList scopes;
    aeHashMap scopeCounterMap;
};


assembler
assemblerNew() {
    assembler a = malloc(sizeof(struct  _assembler));
    a->index = 0;
    a->symbol_stack = BinListNew();
    a->scopes = BinListNew();
    a->scopeCounterMap = aeHashMapNew();
    BinListAppend(a->scopes, "global");
//    aeHashMapSet(a->scopeCounterMap, "global", <#size_t value#>)
    return a;
}



aeString
getScope(assembler assem) {
    aeString new_label = aeStringNewWithChars("");
    for (size_t i=0; i<BinListLength(assem->scopes); i++) {
        char* s = (char*)BinListElementByIndex(assem->scopes, i);
        new_label = aeStringConcat(new_label, aeStringNewWithChars(s));
        new_label = aeStringConcat(new_label, aeStringNewWithChars("->"));
    }
    return new_label;
}

BinList
assemblerToMachineCodes(BinList instructions) {
    BinList res = BinListNew();
    
    for (size_t i = 0; i < BinListLength(instructions); i++) {
        char* instruction = (char*)BinListElementByIndex(instructions, i);
//        printf("assemblerToMachineCodes <%s> length %lu \n", instruction, strlen(instruction));
        if(isDigit(instruction)) {
            size_t instr = aeStringToInt(aeStringNewWithChars(instruction));
            BinListAppend(res, (void*)instr);

        } else{
            if(strcmp(STR(push), instruction) == 0) {
                BinListAppend(res, (void*)push);
                
            } else if(strcmp(STR(pop), instruction) == 0) {
                BinListAppend(res, (void*)pop);
                
            } else if(strcmp(STR(save_to_memory), instruction) == 0) {
                BinListAppend(res, (void*)save_to_memory);
                
            } else if(strcmp(STR(halt), instruction) == 0) {
                BinListAppend(res, (void*)halt);
                
            } else if(strcmp(STR(add), instruction) == 0) {
                BinListAppend(res, (void*)add);
                
            } else if(strcmp(STR(compare), instruction) == 0) {
                BinListAppend(res, (void*)compare);
                
            } else if(strcmp(STR(jump_if_great), instruction) == 0) {
                BinListAppend(res, (void*)jump_if_great);
                
            } else if(strcmp(STR(jump), instruction) == 0) {
                BinListAppend(res, (void*)jump);
                
            } else if(strcmp(STR(subroutine_call), instruction) == 0) {
                BinListAppend(res, (void*)subroutine_call);
                
            } else if(strcmp(STR(__exit), instruction) == 0) {
                BinListAppend(res, (void*)__exit);
                
            } else if(strcmp(STR(push_parameter_stack), instruction) == 0) {
                BinListAppend(res, (void*)push_parameter_stack);
                
            } else if(strcmp(STR(pop_parameter_stack), instruction) == 0) {
                BinListAppend(res, (void*)pop_parameter_stack);
                
            } else if(strcmp(STR(push_return_value_stack), instruction) == 0) {
                BinListAppend(res, (void*)push_return_value_stack);
                
            } else if(strcmp(STR(pop_return_value_stack), instruction) == 0) {
                BinListAppend(res, (void*)pop_return_value_stack);
                
            }  else if(strcmp(STR(push_variable_stack), instruction) == 0) {
                BinListAppend(res, (void*)push_variable_stack);
                
            }  else if(strcmp(STR(pop_variable_stack), instruction) == 0) {
                BinListAppend(res, (void*)pop_variable_stack);
                
            }  else if(strcmp(STR(load_from_variable_stack), instruction) == 0) {
                BinListAppend(res, (void*)load_from_variable_stack);
                
            }  else if(strcmp(STR(save_to_variable_stack), instruction) == 0) {
                BinListAppend(res, (void*)save_to_variable_stack);
                
            } else if(strcmp(STR(jump_if_less), instruction) == 0) {
                BinListAppend(res, (void*)jump_if_less);
    
            } else {
                printf("没有这个！！！ <%s> %d \n", instruction, strcmp("push", instruction) == 0);
            }
        }
    }
    BinListLogNum("assemblerToMachineCodes", res);
    return res;
}

int
find(char* str, char c) {
    size_t len = strlen(str);
    for (int i = 0; i < len; i++) {
        if(c == str[i]) {
            return i;
        }
    }
    return -1;
}

char*
removeEveryThing(char* str) {
    char* new_str = malloc(200);
    char* letters = "abcdefghijklmnopqrstuvwxyz_@#. >";
    char* nums = "01234567890";
    size_t len = strlen(str);
    size_t index = 0;
    for (int i = 0; i < len; i++) {
        char c = str[i];
        if(find(letters, c) != -1 || find(nums, c) != -1) {
            new_str[index] = c;
            index += 1;
        }
    }
    new_str[index + 1] = 0;

    return new_str;
}

char*
trimSpace(char* str) {
//    printf("trimSpace\n");
    size_t len = strlen(str);
    size_t start_index = 0;
    size_t end_index = 0;
    for (size_t i = 0; i < len; i++) {
        char c = str[i];
        if(c != ' ') {
            start_index = i;
            break;
        }
    }
//    printf("trimSpace2\n");
    for (size_t i = len - 1; i >= 0; i--) {
//        printf("loop in len -1  %zu %zu\n", len, i);
        char c = str[i];
        if(c != ' ') {
            end_index = i;
            break;
        }
    }
//    printf("trimSpace3\n");
    aeString new_str = aeStringSubstring(aeStringNewWithChars(str), start_index, end_index + 1);

    
//    aeStringLog("trimtrim", new_str);
    return aeStringData(new_str);
}


char*
stringSubstring(char* s, size_t start, size_t end){
    size_t len = strlen(s);

    if(start < 0) {
        return "";
    } else if(end > len) {
        char* as = stringSubstring(s, start, len);
        return as;
    } else {
        size_t index = 0;
        char* new_str = malloc(end - start + 1);
        for (size_t i = 0; i < len; i++) {
            if(i >= start && i < end) {
                new_str[index] = s[i];
                index += 1;
            }
        }
        new_str[end - start] = '\0';
        return new_str;
    }
}

BinList
stringSplit(char* s, char* delimeter) {
    BinList list = BinListNew();
    size_t i = 0;
    
    char* toAdd = malloc(100);
    size_t index_add = 0;
    while (i < strlen(s) + 1) {
        size_t start = i; //0
        size_t end = i + strlen(delimeter); //0+1 = 1
        char* substr = stringSubstring(s, start, end); //s
//        printf("substr %s \n", substr);
        if(strcmp(delimeter, substr) != 0) {
            toAdd[index_add] = s[i];
            index_add += 1;
            if(end == strlen(s) + 1) {
                toAdd[index_add] = '\0';
                char* bb = malloc(strlen(toAdd) + 1);
                strcpy(bb, toAdd);
//                printf("substr end2 %s \n", bb);
                BinListAppend(list, bb);
            }
        } else {
            toAdd[index_add] = '\0';
            char* aa = malloc(strlen(toAdd) + 1);
            strcpy(aa, toAdd);
            BinListAppend(list, aa);
//            printf("substr end1 %s \n", aa);
            free(toAdd);
            toAdd = malloc(100);
            index_add = 0;
            i += strlen(delimeter) - 1;
        }
        i += 1;
    }
    return list;
}


BinList
assemblerPreproccess(BinList program) {
    BinList instructions = BinListNew();
    for (size_t i = 0; i < BinListLength(program); i++) {
        char* rawStr = (char*)BinListElementByIndex(program, i);
        BinList strlist = stringSplit(rawStr, "//");
        char* splited = (char*)BinListElementByIndex(strlist, 0);
    
        rawStr = removeEveryThing(splited);
        rawStr = trimSpace(rawStr);
        
        aeString new_line = aeStringNewWithChars(rawStr);
        
        if(aeStringLength(new_line) == 0) {
            continue;
        }
        BinListAppend(instructions, aeStringData(new_line));
    }
    


    
    return instructions;
}


struct _assemblerLabelSet {
    aeHashMap definition;
    BinList new_structions;
};

typedef struct _assemblerLabelSet *assemblerLabelSet;


assemblerLabelSet
assemblerLabelDefinition(BinList instructions){
    assemblerLabelSet ls = malloc(sizeof(struct _assemblerLabelSet));
    ls->definition = aeHashMapNew();
    ls->new_structions = BinListNew();
    
    for (size_t i = 0; i < BinListLength(instructions); i ++) {
        char* s = (char*)BinListElementByIndex(instructions, i);
        if(s[0] != '#') {
            BinListAppend(ls->new_structions, (void*)s);
        } else {
            aeString as = aeStringNewWithChars(s);
            aeString label_name = aeStringSubstring(as, 1, aeStringLength(as));
            size_t address = BinListLength(ls->new_structions);
//            aeStringLog("label_name", label_name);
            aeHashMapSet(ls->definition, label_name, address);
        }
    }
    
//    aeHashMapLog(ls->definition);
    return ls;
}


BinList
assemblerReplaceLabelsUsage(assemblerLabelSet labelSet) {
    BinList new_structions = BinListNew();
    for (size_t i = 0; i < BinListLength(labelSet->new_structions); i ++) {
        char* s = (char*)BinListElementByIndex(labelSet->new_structions, i);
        if(s[0] == '@') {
            aeString as = aeStringNewWithChars(s);
            aeString label_name = aeStringSubstring(as, 1, aeStringLength(as));
            size_t address = aeHashMapGet(labelSet->definition, label_name);
//            printf("address address %zu \n", address);
            char* str_address = intToString(address);
            BinListAppend(new_structions, (void*)str_address);
        } else {
            BinListAppend(new_structions, (void*)s);
        }
    }

//    BinListLogString(new_structions);
//    fflush(stdout);
    return new_structions;
}

void
push_variable(char *var_name, BinList symbol_stack) {
    aeString ae_name = aeStringNewWithChars(var_name);
    aeHashMap nowVarsMap = (aeHashMap)BinListGetLastElement(symbol_stack);
    if(aeHashMapGet(nowVarsMap, ae_name) != -1) {
        printf("⭐️⭐️⭐️!!!!!!重复定义!!!\n");
        exit(-99);
    }
    
    for (size_t i = 0; i < BinListLength(symbol_stack); i++) {
        aeHashMap vmap = (aeHashMap)BinListElementByIndex(symbol_stack, i);
        BinList keyList = aeHashMapGetKeylist(vmap);
        
        //原 variable stack 里的变量的偏移 +1
        for (size_t j = 0; j < BinListLength(keyList); j++) {
            aeString k = (aeString)BinListElementByIndex(keyList, j);
            size_t v = aeHashMapGet(vmap, k);
            aeHashMapSet(vmap, k, v + 1);
        }
    }
    aeHashMapSet(nowVarsMap, ae_name, 0);
    
    
//    BinList keyList = aeHashMapGetKeylist(varsMap);
////    BinListLogString(keyList);
//    for(size_t i = 0; i < BinListLength(keyList); i++) {
//        aeString k = (aeString)BinListElementByIndex(keyList, i);
//        size_t now_offset = (size_t)aeHashMapGet(varsMap, k);
//        now_offset += 1;
//        aeHashMapSet(varsMap, k, now_offset);
////        printf("aeK %s now_offset %ld \n", aeStringData(k), now_offset);
//    }
//    aeHashMapSet(varsMap, ae_name, 0);
//    printf("name %s \n", name);
//    aaeHashMapLog(varsMap);
}

assemblerFakeRes
assemblerFakeResNew(){
    assemblerFakeRes res = malloc(sizeof(struct _assemblerFakeRes));
    res->extended = BinListNew();
    res->end_index = 0;
    return res;
}

char*
get_scoped_name(assembler assem, char* name) {
    char* scope_now = (char*)BinListGetLastElement(assem->scopes);
    aeString scoped_name = aeStringAppend(aeStringNewWithChars(scope_now), aeStringNewWithChars("_"));
    scoped_name = aeStringAppend(scoped_name, aeStringNewWithChars(name));
    return aeStringData(scoped_name);
}



assemblerFakeRes
fake_instruction_var(BinList instructions, size_t index, assembler assem) {
    assemblerFakeRes res = assemblerFakeResNew();
    char* fake = (char*)BinListElementByIndex(instructions, index);
    
    BinList fakeList = stringSplit(fake, " ");
    void* name = BinListElementByIndex(fakeList, 1);
    void* value = BinListElementByIndex(fakeList, 2);
    
    BinListLogMap("before var push", assem->symbol_stack);
    push_variable(name, assem->symbol_stack);
    BinListLogMap("after var push", assem->symbol_stack);
//    printf("after");
//    aeHashMapLog(varsMap);
    BinListAppend(res->extended, "push");
    BinListAppend(res->extended, value);
    BinListAppend(res->extended, "push_variable_stack");
    res->end_index = index;
    return res;
}

size_t
offset_by_name(assembler assem, char* var_name) {
//    size_t offset = aeHashMapGet(varsMap, aeStringNewWithChars(var_name));
//    return offset;

    for (size_t i = 0; i < BinListLength(assem->symbol_stack); i++) {
        aeHashMap vmap = (aeHashMap)BinListElementByIndex(assem->symbol_stack, i);
        BinList keyList = aeHashMapGetKeylist(vmap);
        
        for (size_t j = 0; j < BinListLength(keyList); j++) {
            aeString k = (aeString)BinListElementByIndex(keyList, j);
            if(aeStringEqual(k, aeStringNewWithChars(var_name))) {
                size_t v = aeHashMapGet(vmap, k);
                printf("offset_by_name %zu\n", v);
                return v;
            }
        }
    }
    return -1;
}

assemblerFakeRes
fake_instruction_add(BinList instructions, size_t index,assembler assem) {
    BinListLogMap("before add", assem->symbol_stack);
    assemblerFakeRes res = assemblerFakeResNew();
//    aeHashMap varsMap = BinListGetLastElement(assem->symbol_stack);
    
    char* fake = (char*)BinListElementByIndex(instructions, index);
    BinList fakeList = stringSplit(fake, " ");
    char* var_name_1 = (char*)BinListElementByIndex(fakeList, 1);
    char* var_name_2 = (char*)BinListElementByIndex(fakeList, 2);
    char* var_name_sum = (char*)BinListElementByIndex(fakeList, 3);
    
    //根据变量名，利用 vars 知道变量在 variable stack 里的偏移
    size_t offset_1 = offset_by_name(assem, var_name_1);
    size_t offset_2 = offset_by_name(assem, var_name_2);
    size_t offset_sum = offset_by_name(assem, var_name_sum);
    printf("fake_instruction_add");
//    aeHashMapLog(varsMap);
    printf("offset_1 %zu offset_2 %zu offset_3 %zu \n", offset_1, offset_2, offset_sum);

    BinListAppend(res->extended, "push");
    BinListAppend(res->extended, (void*)intToString(offset_1));
    BinListAppend(res->extended, "load_from_variable_stack");
    BinListAppend(res->extended, "push");
    BinListAppend(res->extended, (void*)intToString(offset_2));
    BinListAppend(res->extended, "load_from_variable_stack");
    BinListAppend(res->extended, "add");

    BinListAppend(res->extended, "push");
    BinListAppend(res->extended, (void*)intToString(offset_sum));
    BinListAppend(res->extended, "save_to_variable_stack");
    res->end_index = index;
    
//    BinListLogString(res->extended);
    
    return res;
}


assemblerFakeRes
fake_instruction_var_to_operand_stack(BinList instructions, size_t index, assembler assem) {
    assemblerFakeRes res = assemblerFakeResNew();
    // vars 更新变量名在 variable stack 里的偏移
    // 新变量的偏移为 0（表示在栈顶），旧变量的偏移全部 +1
    char* fake = (char*)BinListElementByIndex(instructions, index);
    BinList fakeList = stringSplit(fake, " ");
    char* name = (char*)BinListElementByIndex(fakeList, 1);
    

    size_t offset = offset_by_name(assem, name);

    BinListAppend(res->extended, "push");
    BinListAppend(res->extended, (void*)intToString(offset));
    BinListAppend(res->extended, "load_from_variable_stack");
    
    res->end_index = index;
    return res;
}


void
block_enter(assembler assem, char* scope_name){
    puts("before ************assem->scopeCounterMap********");
    aeHashMapLog(assem->scopeCounterMap);

    aeHashMap map = aeHashMapNew();
    BinListAppend(assem->symbol_stack, (void*)map);
    BinListAppend(assem->scopes, (void*)scope_name);
    
    //维护一个字典，字典里面是至今为止的作用域，然后进入一次就 +1，出来一次就 -1
    aeString s = getScope(assem);
//    printf("至今为止的作用域 %s\n" , aeStringData(s));
    size_t value = aeHashMapGet(assem->scopeCounterMap, s);
    
    if(value == -1){
        aeHashMapSet(assem->scopeCounterMap, s, 1);
    } else {
        aeHashMapSet(assem->scopeCounterMap, s, value + 1);
    }
    
    puts("after ************assem->scopeCounterMap********");
    aeHashMapLog(assem->scopeCounterMap);
    
}

void
block_leave(assembler assem){
    puts("block_leave start");
    aeHashMap to_pop_map = (aeHashMap)BinListGetLastElement(assem->symbol_stack);
    BinListLogString("to_pop_list", aeHashMapGetKeylist(to_pop_map));
    BinList to_pop_list = aeHashMapGetKeylist(to_pop_map);
    size_t num_to_pop = 0;
    for (size_t i = 0 ; i< 17; i++) {
        char* s = BinListElementByIndex(to_pop_list, i);
        if(s != NULL) {
            num_to_pop += 1;
        }
    }
    
    //获得将要退出的作用域里的变量数 num_to_pop
//    size_t num_to_pop = BinListLength(to_pop_list);
    printf("num_to_pop %zu \n", num_to_pop);
    
    //删掉当前作用域
    assem->symbol_stack = BinListSubList(assem->symbol_stack, 0, BinListLength(assem->symbol_stack) - 1);
    assem->scopes = BinListSubList(assem->scopes, 0, BinListLength(assem->scopes) - 1);
    
//    BinListLogMap("mid in leave", symbol_stack);
    // 原 variable stack 里的变量的偏移都朝栈顶方向移动，
    // 变化量是 num_to_pop
    for (size_t i = 0; i < BinListLength(assem->symbol_stack); i++) {
        aeHashMap vmap = (aeHashMap)BinListElementByIndex(assem->symbol_stack, i);
        BinList keyList = aeHashMapGetKeylist(vmap);
        
        //原 variable stack 里的变量的偏移 +1
        for (size_t j = 0; j < BinListLength(keyList); j++) {
            aeString k = (aeString)BinListElementByIndex(keyList, j);
            printf("kkkk %s\n", aeStringData(k));
            size_t v = aeHashMapGet(vmap, k);
            aeHashMapSet(vmap, k, v - num_to_pop);
        }
    }
    
    puts("block_leave end");
}

assemblerFakeRes
fake_instruction_function_definition(BinList instructions, size_t index, assembler assem) {
    
    
    
    assemblerFakeRes res = assemblerFakeResNew();
    
    char* fake = (char*)BinListElementByIndex(instructions, index);
    BinList fakeList = stringSplit(fake, " ");
    
    //函数名
    char* func_name = (char*)BinListElementByIndex(fakeList, 1);
    
//    BinListLogMap("vars before enter", assem->symbol_stack);
    block_enter(assem, func_name);
//    BinListLogMap("vars after enter", assem->symbol_stack);
    //函数名作为标签
    aeString label = aeStringConcat(aeStringNewWithChars("#"), aeStringNewWithChars(func_name));
    BinListAppend(res->extended, (void*)aeStringData(label));
    
    //把函数参数都从 parameter stack 中 pop 出来，push 到 variable stack
    //同时 vars 要更新
    BinList parameters = BinListNew();
    if(BinListLength(fakeList) > 2) {
        BinList sublist = BinListSubList(fakeList, 2, BinListLength(fakeList) + 1);
//        BinListLogString(sublist);
        parameters = BinListExtend(parameters, sublist);
//        BinListLogString(parameters);
    }
    
    for (size_t i = 0; i < BinListLength(parameters); i++) {
        char* p = (char*)BinListElementByIndex(parameters, i);
//        printf("pppppp %s \n", p);
        push_variable(p, assem->symbol_stack);
        BinListAppend(res->extended, "pop_parameter_stack");
        BinListAppend(res->extended, "push_variable_stack");
    }
    
    //跳过 [.function 函数名 参数1 参数2 ...] 这一行
    index += 1;
    
    //收集函数体里的命令，
    //'.function_end' 作为函数体结束的标志
    BinList func_body_instructions = BinListNew();
    char* next_instruction = (char*)BinListElementByIndex(instructions, index);
    
    //如果有嵌套的函数定义，
    //要防止内部的 ".function_end" 被误认为整个函数体的结束标志
    //所以加一个计数来正确配对 .function 和 .function_end
    int nested_count = 0;
    while (true) {
        if(strcmp(next_instruction, _function_end) == 0 && nested_count == 0) {
            break;
        } else if(strcmp(next_instruction, _function_end) == 0 && nested_count > 0) {
            nested_count -= 1;
        }
        
        BinList tmpList = stringSplit(next_instruction, " ");
        
        //函数名
        char* n = (char*)BinListElementByIndex(tmpList, 0);
        if(strcmp(n, _function) == 0) {
            nested_count += 1;
        }
        
        
        BinListAppend(func_body_instructions, (void*)next_instruction);
        index += 1;
        next_instruction = (char*)BinListElementByIndex(instructions, index);
    }

    //因为函数体内部的指令也可以是伪指令，所以也做伪指令展开
    BinListLogString("func_body_instructions", func_body_instructions);
    func_body_instructions = assemblerPreproccess(func_body_instructions);
    func_body_instructions = assemblerExtendFakeInstructions(func_body_instructions, assem);
    
    res->extended = BinListExtend(res->extended, func_body_instructions);
    
    
//    BinListLogMap("vars before leave", assem->symbol_stack);
    block_leave(assem);
//    BinListLogMap("vars after leave", assem->symbol_stack);
    
    res->end_index = index;
    return res;
}



aeString
createStartLabelWithScope(assembler assem) {
    aeString new_label = getScope(assem);
    size_t v = aeHashMapGet(assem->scopeCounterMap, new_label);
    
    new_label = aeStringConcat(new_label, aeStringNewWithChars("start"));
    new_label = aeStringConcat(new_label, aeStringNewWithChars("->"));
    new_label = aeStringConcat(new_label, aeStringNewWithChars(intToString(v)));
    
    return new_label;
}

aeString
createEndLabelWithScope(assembler assem) {
    aeString new_label = getScope(assem);
    size_t v = aeHashMapGet(assem->scopeCounterMap, new_label);
    new_label = aeStringConcat(new_label, aeStringNewWithChars("end"));
    new_label = aeStringConcat(new_label, aeStringNewWithChars("->"));
    new_label = aeStringConcat(new_label, aeStringNewWithChars(intToString(v)));
    return new_label;
}

assemblerFakeRes
fake_instruction_while(BinList instructions, size_t index, assembler assem) {
    assemblerFakeRes res = assemblerFakeResNew();
    
//    BinListLogMap("while before enter", assem->symbol_stack);
    block_enter(assem,"while");
//    BinListLogMap("while after enter", assem->symbol_stack);
    
    char* fake = (char*)BinListElementByIndex(instructions, index);
    BinList fakeList = stringSplit(fake, " ");
    // todo 先定死 operand_1 是变量，operand_2 是数字
    // todo 先定死是 while (变量 < 数字)
    // todo while 的作用域
    char* operand_1 = (char*)BinListElementByIndex(fakeList, 1);
    char* operand_2 = (char*)BinListElementByIndex(fakeList, 3);
    size_t operand_1_offset = offset_by_name(assem, operand_1);
    printf("while operand_1_offset %zu operand_1 %s\n", operand_1_offset, operand_1);

    aeString while_label_start = createStartLabelWithScope(assem);
    aeString while_label_end = createEndLabelWithScope(assem);

    
    aeString sharp_while_label_start = aeStringConcat(aeStringNewWithChars("#"),while_label_start);
    BinListAppend(res->extended, aeStringData(sharp_while_label_start));
    BinListAppend(res->extended, "push");
    BinListAppend(res->extended, (void*)intToString(operand_1_offset));
    BinListAppend(res->extended, "load_from_variable_stack");
    BinListAppend(res->extended, "push");
    BinListAppend(res->extended, operand_2);
    BinListAppend(res->extended, "compare");
    
    BinListAppend(res->extended, "push");
    BinListAppend(res->extended, "@do");
    BinListAppend(res->extended, "jump_if_less");
    BinListAppend(res->extended, "push");
    aeString at_while_label_end = aeStringConcat(aeStringNewWithChars("@"), while_label_end);
    BinListAppend(res->extended, aeStringData(at_while_label_end));
//    BinListAppend(res->extended, "@while_end");
    BinListAppend(res->extended, "jump");
    
    
    BinList while_body_instructions = BinListNew();
    BinListAppend(while_body_instructions, "#do");
    size_t body_index = index + 1;
    char* next = (char*)BinListElementByIndex(instructions, body_index);
    while (strcmp(next, _while_end) != 0) {
        BinListAppend(while_body_instructions, (void*)next);
        body_index += 1;
        next = (char*)BinListElementByIndex(instructions, body_index);
    }
    BinListLogString("while_body_instructions", while_body_instructions);

    //块里的代码也可以有伪指令，展开
    while_body_instructions = assemblerExtendFakeInstructions(while_body_instructions, assem);
    while_body_instructions = assemblerPreproccess(while_body_instructions);
    res->extended = BinListExtend(res->extended, while_body_instructions);
    
    //清空在 while 块里新定义的变量
    BinList pop_instructions = BinListNew();
    aeHashMap nowVarsMap = (aeHashMap)BinListGetLastElement(assem->symbol_stack);
    BinList keyList = aeHashMapGetKeylist(nowVarsMap);
//    BinListLogString("⭐️⭐️keyList", keyList);
    for (size_t i = 0; i < BinListLength(keyList); i++) {
        BinListAppend(pop_instructions, "pop_variable_stack");
        BinListAppend(pop_instructions, "pop");
    }
    res->extended = BinListExtend(res->extended, pop_instructions);
    
//    BinListLogMap("while before leave", assem->symbol_stack);
    block_leave(assem);
//    BinListLogMap("while after leave", assem->symbol_stack);
    BinListAppend(res->extended, "push");
    aeString at_while_label_start = aeStringConcat(aeStringNewWithChars("@"), while_label_start);
    BinListAppend(res->extended, aeStringData(at_while_label_start));
//    BinListAppend(res->extended, "@while_start");
    BinListAppend(res->extended, "jump");
    aeString shart_while_label_end = aeStringConcat(aeStringNewWithChars("#"), while_label_end);
    BinListAppend(res->extended, aeStringData(shart_while_label_end));

//    BinListAppend(res->extended, "#while_end");
    
    res->end_index = body_index;
    
    
    return res;
}



assemblerFakeRes
fake_instruction_if(BinList instructions, size_t index, assembler assem) {
    assemblerFakeRes res = assemblerFakeResNew();
    
    BinListLogMap("if before enter", assem->symbol_stack);
    block_enter(assem, "if");
    BinListLogMap("if after enter", assem->symbol_stack);
    
    char* fake = (char*)BinListElementByIndex(instructions, index);
    BinList fakeList = stringSplit(fake, " ");
    // todo  先定死 operand_1 是变量，operand_2 是数字
    // todo  先定死是 if (变量 > 数字)
    // todo  if 作用域
    char* operand_1 = (char*)BinListElementByIndex(fakeList, 1);
    char* operand_2 = (char*)BinListElementByIndex(fakeList, 3);
    
    aeHashMap nowVarsMap = (aeHashMap)BinListGetLastElement(assem->symbol_stack);
    size_t operand_1_offset = offset_by_name(assem, operand_1);
    aeHashMapLog(nowVarsMap);
    printf("operand_1_offset %zu operand_1 %s\n", operand_1_offset, operand_1);

    aeString if_label_start = createStartLabelWithScope(assem);
    aeString if_label_end = createEndLabelWithScope(assem);
    
//    BinListAppend(res->extended, "#if");
    aeString sharp_if_label_start = aeStringConcat(aeStringNewWithChars("#"),if_label_start);
    BinListAppend(res->extended, aeStringData(sharp_if_label_start));
    BinListAppend(res->extended, "push");
    BinListAppend(res->extended, (void*)intToString(operand_1_offset));
    BinListAppend(res->extended, "load_from_variable_stack");
    BinListAppend(res->extended, "push");
    BinListAppend(res->extended, operand_2);
    BinListAppend(res->extended, "compare");
    BinListAppend(res->extended, "push");
    BinListAppend(res->extended, "@then");
    BinListAppend(res->extended, "jump_if_great");
    BinListAppend(res->extended, "push");
    aeString at_if_label_end = aeStringConcat(aeStringNewWithChars("@"),if_label_end);
    BinListAppend(res->extended, aeStringData(at_if_label_end));
//    BinListAppend(res->extended, "@endif");
    BinListAppend(res->extended, "jump");

    
    
    BinList if_body_instructions = BinListNew();
    BinListAppend(if_body_instructions, "#then");
    size_t if_index = index + 1;
    char* next = (char*)BinListElementByIndex(instructions, if_index);
    while (strcmp(next, _if_end) != 0) {
        BinListAppend(if_body_instructions, (void*)next);
        if_index += 1;
        next = (char*)BinListElementByIndex(instructions, if_index);
    }
    BinListLogString("if_body_instructions", if_body_instructions);
//    //块里的代码也可以有伪指令，展开
    if_body_instructions = assemblerExtendFakeInstructions(if_body_instructions, assem);
    if_body_instructions = assemblerPreproccess(if_body_instructions);
    res->extended = BinListExtend(res->extended, if_body_instructions);
//
//
//    //清空在 if 块里新定义的变量
    BinList pop_instructions = BinListNew();
    BinList keyList = aeHashMapGetKeylist(nowVarsMap);
    for (size_t i = 0; i < BinListLength(keyList); i++) {
        BinListAppend(pop_instructions, "pop_variable_stack");
        BinListAppend(pop_instructions, "pop");
    }
    
    BinListLogMap("if before leave", assem->symbol_stack);
    block_leave(assem);
    BinListLogMap("if after leave", assem->symbol_stack);
    aeString sharp_if_label_end = aeStringConcat(aeStringNewWithChars("#"),if_label_end);
    BinListAppend(res->extended, aeStringData(sharp_if_label_end));
//    BinListAppend(res->extended, "#endif");
    
    res->end_index = if_index;
    return res;
    
}

assemblerFakeRes
fake_instruction_call(BinList instructions, size_t index, assembler assem) {
    assemblerFakeRes res = assemblerFakeResNew();
    
    char* fake = (char*)BinListElementByIndex(instructions, index);
    BinList fakeList = stringSplit(fake, " ");
    
    //函数名
    char* func_name = (char*)BinListElementByIndex(fakeList, 1);
//    printf("fake_instruction_call func_name %s \n", func_name);
    
    
    BinList parameters = BinListNew();
    if(BinListLength(fakeList) > 2) {
        BinList sublist = BinListSubList(fakeList, 2, BinListLength(fakeList) + 1);
        parameters = BinListExtend(parameters, sublist);
    }
    BinListLogString("fake_instruction_call", parameters);
    //跳转到函数之前，把参数 push 到 parameter stack
    //把参数按反序 push 到 parameter stack，
    BinList reversed_parameters = BinListReverse(parameters);
    BinListLogString("fake_instruction_call reversed_parameters", reversed_parameters);
     
    for (size_t i = 0; i < BinListLength(reversed_parameters); i++) {
        char* p = (char*)BinListElementByIndex(parameters, i);
        size_t offset = offset_by_name(assem, p);
        printf("offsetoffset %s %zu \n", p, offset);
        BinListAppend(res->extended, "push");
        BinListAppend(res->extended, (void*)intToString(offset));
        BinListAppend(res->extended, "load_from_variable_stack");
        BinListAppend(res->extended, "push_parameter_stack");
    }
    
    //调用子过程
    BinListAppend(res->extended, "push");
    //函数名作为标签
    aeString label = aeStringConcat(aeStringNewWithChars("@"), aeStringNewWithChars(func_name));
    BinListAppend(res->extended, (void*)aeStringData(label));
    BinListAppend(res->extended, "subroutine_call");
    
    res->end_index = index;
    return res;
}

assemblerFakeRes
fake_instruction_return(BinList instructions, size_t index, assembler assem) {
    assemblerFakeRes res = assemblerFakeResNew();
    
    char* fake = (char*)BinListElementByIndex(instructions, index);
    BinList fakeList = stringSplit(fake, " ");
    aeHashMap nowVarsMap = BinListGetLastElement(assem->symbol_stack);
    
    
    //展开有返回值的情况
    if(BinListLength(fakeList) > 1) {
        //todo 先不管多返回值，只有一个返回值
        char* return_var_name = (char*)BinListElementByIndex(fakeList, 1);
        size_t offset = offset_by_name(assem, return_var_name);
        printf("fake_instruction_return offset %zu \n", offset);
        BinListAppend(res->extended, "push");
        BinListAppend(res->extended, (void*)intToString(offset));
        BinListAppend(res->extended, "load_from_variable_stack");
        BinListAppend(res->extended, "push_return_value_stack");
    }
    
    //删除在函数里新增的所有变量
    //过 vars 里的记录可得到
    BinList plist = aeHashMapGetKeylist(nowVarsMap);
//    printf("  BinListLength(plist) %zu \n", BinListLength(plist));
//    BinListLogString("fake_instruction_return plist", plist);
    for (size_t i = 0; i < BinListLength(plist); i++) {
        BinListAppend(res->extended, "pop_variable_stack");
        BinListAppend(res->extended, "pop");
    }
    
    BinListAppend(res->extended, "__exit");
    
    res->end_index = index;
    return res;
}




BinList
assemblerExtendFakeInstructions(BinList instructions, assembler assem){
    BinList res = BinListNew();
    size_t i = 0;
    size_t len = BinListLength(instructions);
//    block_enter(assem, "global");
    while (i < len) {
        char* ins = (char*)BinListElementByIndex(instructions, i);
        //如果指令以 '.' 开头，就是伪指令
        if(ins[0] == '.') {
            BinList strList = stringSplit(ins, " ");
            char* signature = (char*)BinListElementByIndex(strList, 0);
//            printf("signature %s \n", signature);
            if(strcmp(signature, _var) == 0) {
                assemblerFakeRes fakeRes = fake_instruction_var(instructions, i, assem);
                res = BinListExtend(res, fakeRes->extended);
                i = fakeRes->end_index + 1;
            } else if(strcmp(signature, _add) == 0) {
                assemblerFakeRes fakeRes = fake_instruction_add(instructions, i, assem);
                res = BinListExtend(res, fakeRes->extended);
                i = fakeRes->end_index + 1;
            } else if(strcmp(signature, _var_to_operand_stack) == 0) {
                assemblerFakeRes fakeRes = fake_instruction_var_to_operand_stack(instructions, i, assem);
                res = BinListExtend(res, fakeRes->extended);
                i = fakeRes->end_index + 1;
            } else if(strcmp(signature, _return) == 0) {
                assemblerFakeRes fakeRes = fake_instruction_return(instructions, i, assem);
                res = BinListExtend(res, fakeRes->extended);
                i = fakeRes->end_index + 1;
            } else if(strcmp(signature, _function) == 0) {
                assemblerFakeRes fakeRes = fake_instruction_function_definition(instructions, i, assem);
                res = BinListExtend(res, fakeRes->extended);
                i = fakeRes->end_index + 1;
            } else if(strcmp(signature, _call) == 0) {
                assemblerFakeRes fakeRes = fake_instruction_call(instructions, i, assem);
                res = BinListExtend(res, fakeRes->extended);
                i = fakeRes->end_index + 1;
            }else if(strcmp(signature, _if) == 0) {
                fflush(stdout);
                assemblerFakeRes fakeRes = fake_instruction_if(instructions, i, assem);
                res = BinListExtend(res, fakeRes->extended);
                i = fakeRes->end_index + 1;
            }else if(strcmp(signature, _while) == 0) {
                fflush(stdout);
                assemblerFakeRes fakeRes = fake_instruction_while(instructions, i, assem);
                res = BinListExtend(res, fakeRes->extended);
                i = fakeRes->end_index + 1;
            }
        } else {
            BinListAppend(res, ins);
            i += 1;
        }
    }
//    block_leave(assem);
//    aeHashMapLog(varsMap);
    BinListLogString("assemblerExtendFakeInstructions", res);
    
    return res;
}

BinList
assemblerCompile(BinList program) {
    //预处理，去除注释，空白，不需要的回车等
    BinList instructions = assemblerPreproccess(program);
    //作用域栈
    assembler a = assemblerNew();
//    BinList symbol_stack = BinListNew();
    aeHashMap globalMap =  aeHashMapNew();
    BinListAppend(a->symbol_stack, globalMap);
    
   instructions = assemblerExtendFakeInstructions(instructions,a);
    
    //记录标签定义，标签名 -> 地址
    assemblerLabelSet labelSet = assemblerLabelDefinition(instructions);
    
    //标签替换为地址
    instructions = assemblerReplaceLabelsUsage(labelSet);
              
    BinListLogString("instructionsinstructions", instructions);
    //从汇编变成机器语言
    BinList machineCodes = assemblerToMachineCodes(instructions);
    
    //往后补到 64k
    for(size_t i = BinListLength(machineCodes); i< 64 * 1024; i++) {
        BinListAppend(machineCodes, 0);
    }
    
    return machineCodes;
}


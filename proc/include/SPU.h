#ifndef SPU_H
#define SPU_H

#include "../../common.h"
#include "stack.h"
#include "stack_funcs.h"
#include "verify_funcs.h"


/*const char *all_commands[] = {[PUSH] = "push",
                              [ADD] = "add",
                              [SUB] = "sub",
                              [MUL] = "mul",
                              [DIV] = "div",
                              [OUT] = "out",
                              [IN] = "in",
                              [SQRT] = "sqrt",
                              [SIN] = "sin",
                              [COS] = "cos",
                              [DUMP] = "dump",
                              [HLT] = "hlt"};*/

const int NUM_OF_REGISTERS = 16;

typedef struct
{
    int *code;
    size_t code_size;
    size_t ip;
    Stack_t stack;
    int registers[NUM_OF_REGISTERS];
} SPU_t;

#endif

#ifndef COMMON_H
#define COMMON_H

typedef enum
{
    SUCCESS                = 0,
    FILE_NULL_PTR          = 1,
    STACK_ERROR            = 2,
    NULL_PTR               = 3,
    ARG_NULL_PTR           = 4,
    WRONG_ARG              = 5,
    CAPACITY_LIMIT_REACHED = 6,
    UNKNOWN_ERROR          = 7
} errors_t;

const int CODE_POISON = -1;

typedef enum
{
   NOT_COMMAND = 0,
    PUSH = 1,
    POP,
    ADD,
    SUB,
    MUL,
    DIV,
    OUT,
    IN,
    SQRT,
    SIN,
    COS,
    DUMP,
    HLT,
    JMP,
    JA,
    JAE,
    JB,
    JBE,
    JE,
    JNE
} commands_t;

const int CODE_RECALLOC_COEF = 2;

void *Recalloc(void *data, size_t data_size, size_t number, size_t size);

#endif

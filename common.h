#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>

typedef enum
{
    SUCCESS                = 0,
    FILE_NULL_PTR          = 1,
    STACK_ERROR            = 2,
    NULL_PTR               = 3,
    ARG_NULL_PTR           = 4,
    WRONG_ARG              = 5,
    CAPACITY_LIMIT_REACHED = 6,
    SCAN_ERROR             = 7,
    UNKNOWN_ERROR          = 8
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
    JNE,
    CALL,
    RET,
    CHR
} commands_t;

typedef enum
{
    NUMBER = 1,
    REGISTER = 2,
    RAM      = 4
} arg_t;

const int PRECISION = 100;

const int RECALLOC_COEF = 2;
const int STACK_RECALLOC_COEF = 2;
const int CODE_RECALLOC_COEF = 2;

void *Recalloc(void *data, size_t prev_size, size_t new_size, size_t elem_size);
void PrintErr(errors_t res);

#define MY_ASSERT(CHECKED_ARG, MESSAGE, ACTION) do                                         \
    {                                                                                      \
        if (!(CHECKED_ARG))                                                                \
        {                                                                                  \
            printf("Assertion failed: " #CHECKED_ARG "\nMessage: " MESSAGE                 \
                   "\nFile: " __FILE__ "\nFunction: %s\nLine: %d\n\n", __func__, __LINE__);\
            ACTION;                                                                        \
        }                                                                                  \
    } while(0)

#endif

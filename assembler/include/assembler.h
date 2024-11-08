#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>

#include "../../common.h"

//#define ASM_DEBUG

#ifdef ASM_DEBUG
    #define ON_ASM_DBG(...) __VA_ARGS__
#else
    #define ON_ASM_DBG(...)
#endif

const int MAX_CMD_LEN = 60;

const int INIT_LBLS_SIZE = 20;
typedef struct
{
    size_t adres;
    char name[MAX_CMD_LEN];
} Label_t;

typedef struct
{
    int *code;
    FILE *output;
    char *buffer;
    Label_t *labels;
    size_t code_size, ip, buf_size, labels_num;
    size_t labels_size;
    //
} Compiler_t;

typedef enum
{
    NO_COMMENTS = 0,
    STARTS_WITH_COMMENT,
    HAS_COMMENT
} comment_t;

#endif

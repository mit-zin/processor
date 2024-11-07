#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>

#include "../../common.h"

const int MAX_CMD_LEN = 20;

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

#endif

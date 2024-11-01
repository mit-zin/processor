#ifndef COMPILER_H
#define COMPILER_H

#include <stdio.h>

#include "../common.h"

const int MAX_CMD_LEN = 20;
const size_t MIN_SIZE = 50;
const int MAX_STRING_LEN = MAX_CMD_LEN * 3;

typedef struct
{
    size_t adres;
    char name[MAX_CMD_LEN];
} Label_t;

typedef struct
{
    char (*strings)[MAX_CMD_LEN];
    size_t size_of_arr, num_of_strings;
} Str_arr_struct;

typedef struct
{
    int *code;
    FILE *output;
    size_t size_of_code, ip;
    Str_arr_struct str_arr;
} Compiler_t;

void CreateCompiler(Compiler_t *compiler);
void DestroyCompiler(Compiler_t *compiler);
void ReadAsmFile(Str_arr_struct *str_arr);
void Compile(Compiler_t *compiler);
commands_t ReadCommand(const char command[MAX_CMD_LEN]);
void ReadPushArg(Compiler_t *compiler, size_t *str_ind);
void ReadPopArg(Compiler_t *compiler, size_t *str_ind);
int JumpArg(const char *string, int num_of_labels, const Label_t *labels);

#endif

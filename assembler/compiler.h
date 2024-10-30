#ifndef COMPILER_H
#define COMPILER_H

#include <stdio.h>

#include "../common.h"

const int MAX_CMD_LEN = 20;
const size_t MIN_SIZE = 50;

typedef struct
{
    size_t adres;
    char name[MAX_CMD_LEN];
} Label_t;

typedef struct
{
    int *code;
    FILE *input, *output;
    size_t size, ip;
} Compiler_t;

void CreateCompiler(Compiler_t *compiler);
void DestroyCompiler(Compiler_t *compiler);
void Compile(Compiler_t *compiler);
commands_t ReadCommand(const char command[MAX_CMD_LEN]);
int JumpArg(FILE *input, int num_of_labels, const Label_t *labels);

#endif

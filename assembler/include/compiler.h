#ifndef COMPILER_H
#define COMPILER_H

#include "assembler.h"
#include "read_funcs.h"

const size_t MIN_SIZE = 50;
const size_t NUM_OF_ITERATIONS = 2;
const int MIN_DIFFERENCE = 3;

void CreateCompiler (Compiler_t *compiler);
void DestroyCompiler(Compiler_t *compiler);

void ReadAsmFile(Compiler_t *compiler);

void WriteInFile(Compiler_t *compiler);

#endif

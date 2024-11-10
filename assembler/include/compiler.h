#ifndef COMPILER_H
#define COMPILER_H

#include "assembler.h"
#include "read_funcs.h"

const size_t MIN_SIZE = 50;
const size_t NUM_OF_ITERATIONS = 2;
const int MIN_DIFFERENCE = 3;

errors_t CreateCompiler (Compiler_t *compiler, const char *out_file_name);
errors_t DestroyCompiler(Compiler_t *compiler);

errors_t ReadAsmFile(Compiler_t *compiler, const char *file_name);

errors_t WriteInFile(Compiler_t *compiler);

#endif

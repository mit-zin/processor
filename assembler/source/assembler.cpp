#include <assert.h>
#include <stdio.h>

#include "assembler.h"
#include "compiler.h"
#include "read_funcs.h"

int main()
{
    //fope
    Compiler_t compiler = {.code = 0, .output = 0, .buffer = 0, .labels = 0, .code_size = 0,
                           .ip = 0, .buf_size = 0, .labels_num = 0, .labels_size = 0};
    CreateCompiler(&compiler);

    ReadAsmFile(&compiler);

    WriteInFile(&compiler);

    DestroyCompiler(&compiler);

    return 0;
}

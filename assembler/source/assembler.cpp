#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "assembler.h"
#include "compiler.h"
#include "read_funcs.h"



int main(int argc, char *argv[])
{
    char *input = "../program.asm";
    char *output = "../program_code.bin";
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-input") && argc - i >= 2)
            input = argv[++i];
        else if (!strcmp(argv[i], "-output") && argc - i >= 2)
            output = argv[++i];
    }

    Compiler_t compiler = {.code = 0, .output = 0, .buffer = 0, .labels = 0, .code_size = 0,
                           .ip = 0, .buf_size = 0, .labels_num = 0, .labels_size = 0};
    unsigned int res = 0;
    res |= CreateCompiler(&compiler, output);

    if (!res)
    {
        res |= ReadAsmFile(&compiler, input);

        if (!res)
            res |= WriteInFile(&compiler);

        res |= DestroyCompiler(&compiler);
    }

    if (res)
        PrintErr(res);

    printf("End of program.\n");

    return 0;
}

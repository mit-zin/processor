#include <assert.h>
#include <stdio.h>

#include "compiler.h"

int main()
{
    //fopen
    Compiler_t compiler = {};
    CreateCompiler(&compiler);

    Compile(&compiler);

    DestroyCompiler(&compiler);

    return 0;
}

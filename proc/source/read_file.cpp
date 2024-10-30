#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "read_file.h"

void ReadFile(const char *file_name, SPU_t *SPU)
{
    assert(file_name);
    assert(SPU);

    FILE *input = fopen(file_name, "r");
    assert(input);

    fscanf(input, "%u", &SPU->code_size);

    SPU->code = (int *) calloc(SPU->code_size, sizeof(int));
    assert(SPU->code);

    for (size_t i = 0; i < SPU->code_size; i++)
        SPU->code[i] = CODE_POISON;

    for (size_t i = 0; i < SPU->code_size; i++)
        fscanf(input, "%d", SPU->code + i);

    fclose(input);
}

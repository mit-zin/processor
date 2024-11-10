#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

void *Recalloc(void *data, size_t prev_size, size_t new_size, size_t elem_size)
{
    assert(data);

    data = realloc(data, new_size * elem_size);

    for (size_t i = prev_size * elem_size; i < new_size * elem_size; i++)
        *(char *) (data + i) = 0;

    return data;
}

void PrintErr(unsigned int res)
{
    printf("Errors:\n");
    if (res & FILE_NULL_PTR)
        printf("File null pointer.\n");
    if (res & STACK_ERROR)
        printf("Stack error.\n");
    if (res & NULL_PTR)
        printf("Null pointer.\n");
    if (res & ARG_NULL_PTR)
        printf("Null pointer was given as argument.\n");
    if (res & WRONG_ARG)
        printf("Wrong argument.\n");
    if (res & CAPACITY_LIMIT_REACHED)
        printf("Capacity limit reached.\n");
    if (res & SCAN_ERROR)
        printf("Wrong input.\n");
    if (res & UNKNOWN_ERROR)
        printf("Unknown error.\n");
    printf("\n");
}


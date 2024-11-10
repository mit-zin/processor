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

void PrintErr(errors_t res)
{
    switch (res)
    {
        case SUCCESS :
            break;
        case UNKNOWN_ERROR :
            printf("Unknown error.\n");
            break;
        case FILE_NULL_PTR :
            printf("File null pointer.\n");
            break;
        case STACK_ERROR :
            printf("Stack error.\n");
            break;
        case NULL_PTR :
            printf("Null pointer.\n");
            break;
        case ARG_NULL_PTR :
            printf("Null pointer was given as argument.\n");
            break;
        case WRONG_ARG :
            printf("Wrong argument.");
            break;
        case CAPACITY_LIMIT_REACHED :
            printf("Capacity limit reached.");
            break;
        case SCAN_ERROR :
            printf("Wrong input.");
            break;
        default :
            printf("Unknown error.\n");
    }
}


#include <assert.h>
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

#include <assert.h>
#include <stdlib.h>

#include "common.h"

void *Recalloc(void *data, size_t data_size_bytes, size_t number, size_t size)
{
    assert(data);

    data = realloc(data, number * size);

    for (size_t i = data_size_bytes; i < number * size; i++)
        *(char *) (data + i) = 0;

    return data;
}

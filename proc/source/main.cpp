//#include <TXLib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "stack_funcs.h"
#include "stack.h"
#include "verify_funcs.h"

int main(void)
{
    Stack_t stack = {};

    CreateStack(&stack, 10);
    for (int i = 0; i < 12; i++)
    {
        StackPush(&stack, 1);
    }

    stack_elem_t elem = 0;

    for (int i = 0; i < 10; i++)
    {
        StackPop(&stack, &elem);
    }

    DestroyStack(&stack);

    printf("End of program.\n");

    return 0;
}

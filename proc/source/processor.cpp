#include <stdio.h>

#include "proc_funcs.h"
#include "SPU.h"

int main()
{
    SPU_t SPU = {};
    CreateSPU(&SPU);

    ReadFile("../program_code.txt", &SPU);

    //SPU_Dump(&SPU);
    //printf("%d\n", SPU.RAM[0]);
    Run(&SPU);
    DUMP(&SPU.stack, 0);

    /*bool hlt_flag = false;
    while (SPU.ip < SPU.code_size && !hlt_flag)
    {
        switch (SPU.code[SPU.ip])
        {
            case PUSH :
                StackPush(&SPU.stack, SPU.code[++SPU.ip]);
                break;
            case ADD :
            {
                stack_elem_t popped_elem1 = 0, popped_elem2 = 0;
                StackPop(&SPU.stack, &popped_elem1);
                StackPop(&SPU.stack, &popped_elem2);
                StackPush(&SPU.stack, popped_elem1 + popped_elem2);
                break;
            }
            case SUB :
            {
                stack_elem_t popped_elem1 = 0, popped_elem2 = 0;
                StackPop(&SPU.stack, &popped_elem1);
                StackPop(&SPU.stack, &popped_elem2);
                StackPush(&SPU.stack, popped_elem2 - popped_elem1);
                break;
            }
            case MUL :
            {
                stack_elem_t popped_elem1 = 0, popped_elem2 = 0;
                StackPop(&SPU.stack, &popped_elem1);
                StackPop(&SPU.stack, &popped_elem2);
                StackPush(&SPU.stack, popped_elem1 * popped_elem2);
                break;
            }
            case DIV :
            {
                stack_elem_t popped_elem1 = 0, popped_elem2 = 0;
                StackPop(&SPU.stack, &popped_elem1);
                StackPop(&SPU.stack, &popped_elem2);
                StackPush(&SPU.stack, popped_elem2 / popped_elem1);
                break;
            }
            case OUT :
            {
                stack_elem_t popped_elem = 0;
                StackPop(&SPU.stack, &popped_elem);
                printf("%f\n", popped_elem);
                break;
            }
            case IN :
            {
                stack_elem_t pushed_elem = 0;
                scanf("%lf", &pushed_elem);
                StackPush(&SPU.stack, pushed_elem);
                break;
            }
            case SQRT :
            {
                stack_elem_t popped_elem = 0;
                StackPop(&SPU.stack, &popped_elem);
                StackPush(&SPU.stack, sqrt(popped_elem));
                break;
            }
            case SIN :
            {
                stack_elem_t popped_elem = 0;
                StackPop(&SPU.stack, &popped_elem);
                StackPush(&SPU.stack, sin(popped_elem));
                break;
            }
            case COS :
            {
                stack_elem_t popped_elem = 0;
                StackPop(&SPU.stack, &popped_elem);
                StackPush(&SPU.stack, cos(popped_elem));
                break;
            }
            case DUMP :
                SPU_Dump(&SPU);
                break;
            case HLT :
                hlt_flag = true;
                break;
            default :
                printf("Error: default case.\n");
        }

        SPU.ip++;
    }*/

    DestroySPU(&SPU);

    printf("End of program.\n");

    return 0;
}

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "proc_funcs.h"


void CreateSPU(SPU_t *SPU)
{
    assert(SPU);

    CreateStack(&SPU->stack, INITIAL_CAPACITY);
}

void ReadFile(const char *file_name, SPU_t *SPU)
{
    assert(file_name);
    assert(SPU);

    FILE *input = fopen(file_name, "rb");
    assert(input);

    //fscanf(input, "%zu", &SPU->code_size)
    int size = 0;
    fread(&size, sizeof(int), 1, input);
    SPU->code_size = (size_t) size;

    SPU->code = (int *) calloc(SPU->code_size, sizeof(int));
    assert(SPU->code);

    fread(SPU->code, sizeof(int), SPU->code_size, input);

    /*for (size_t i = 0; i < SPU->code_size; i++)
        SPU->code[i] = CODE_POISON;

    for (size_t i = 0; i < SPU->code_size; i++)
        fscanf(input, "%d", SPU->code + i);*/

    fclose(input);
}

errors_t SPU_Dump(SPU_t *SPU)
{
    assert(SPU);

    FILE *dump_out = 0;
    static bool first_file_opening = true;
    if (first_file_opening)
    {
        dump_out = fopen("./SPU_dump.txt", "w");
        first_file_opening = false;
    }
    else
        dump_out = fopen("./SPU_dump.txt", "a");
    if (!dump_out)
        return FILE_NULL_PTR;

    if (!SPU->code)
        fprintf(dump_out, "Error: code null ptr.\n");
    else
    {
        fprintf(dump_out, "code:\n");
        for (size_t i = 0; i < SPU->code_size; i++)
            fprintf(dump_out, "%02zX ", i);
        fprintf(dump_out, "\n");
        for (size_t i = 0; i < SPU->code_size; i++)
            fprintf(dump_out, "%02X ", SPU->code[i]);
        fprintf(dump_out, "\n");
        for (size_t i = 0; i < SPU->ip; i++)
            fprintf(dump_out, "   ");
        fprintf(dump_out, "^\n");
        for (size_t i = 0; i < SPU->ip; i++)
            fprintf(dump_out, "   ");
        fprintf(dump_out, "ip=%02zX\n\n", SPU->ip);
    }

    fprintf(dump_out, "registers:\n");
    for (size_t i = 0; i < NUM_OF_REGISTERS; i++)
        fprintf(dump_out, "r%zu=%d, ", i, SPU->registers[i]);

    fprintf(dump_out, "\n\nStack:\n");
    for (size_t i = 0; i < SPU->stack.size; i++)
        fprintf(dump_out, "%02.f, ", SPU->stack.data[i]);

    fprintf(dump_out, "\n\n");
    for (size_t i = 0; i < 100; i++)
        fprintf(dump_out, "-");
    fprintf(dump_out, "\n\n");

    fclose(dump_out);

    return SUCCESS;
}

void Run(SPU_t *SPU)
{
    assert(SPU);

    bool hlt_flag = false;

    while (SPU->ip < SPU->code_size && !hlt_flag)
    {
        switch (SPU->code[SPU->ip++])
        {
            case PUSH :
                if (SPU->code[SPU->ip++] == 1)
                    assert(!StackPush(&SPU->stack, (stack_elem_t) SPU->registers[SPU->code[SPU->ip++]]));
                else
                    assert(!StackPush(&SPU->stack, (stack_elem_t) SPU->code[SPU->ip++]));
                break;
            case POP :
            {
                stack_elem_t popped_elem = 0;
                assert(!StackPop(&SPU->stack, &popped_elem));
                SPU->registers[SPU->code[SPU->ip++]] = (int) popped_elem;
                break;
            }
            case ADD :
            {
                stack_elem_t popped_elem1 = 0, popped_elem2 = 0;
                StackPop(&SPU->stack, &popped_elem1);
                StackPop(&SPU->stack, &popped_elem2);
                StackPush(&SPU->stack, popped_elem1 + popped_elem2);
                break;
            }
            case SUB :
            {
                stack_elem_t popped_elem1 = 0, popped_elem2 = 0;
                StackPop(&SPU->stack, &popped_elem1);
                StackPop(&SPU->stack, &popped_elem2);
                StackPush(&SPU->stack, popped_elem2 - popped_elem1);
                break;
            }
            case MUL :
            {
                stack_elem_t popped_elem1 = 0, popped_elem2 = 0;
                StackPop(&SPU->stack, &popped_elem1);
                StackPop(&SPU->stack, &popped_elem2);
                StackPush(&SPU->stack, popped_elem1 * popped_elem2);
                break;
            }
            case DIV :
            {
                stack_elem_t popped_elem1 = 0, popped_elem2 = 0;
                StackPop(&SPU->stack, &popped_elem1);
                StackPop(&SPU->stack, &popped_elem2);
                StackPush(&SPU->stack, popped_elem2 / popped_elem1);
                break;
            }
            case OUT :
            {
                stack_elem_t popped_elem = 0;
                StackPop(&SPU->stack, &popped_elem);
                printf("%f\n", popped_elem);
                break;
            }
            case IN :
            {
                stack_elem_t pushed_elem = 0;
                scanf("%lf", &pushed_elem);
                StackPush(&SPU->stack, pushed_elem);
                break;
            }
            case SQRT :
            {
                stack_elem_t popped_elem = 0;
                StackPop(&SPU->stack, &popped_elem);
                StackPush(&SPU->stack, sqrt(popped_elem));
                break;
            }
            case SIN :
            {
                stack_elem_t popped_elem = 0;
                StackPop(&SPU->stack, &popped_elem);
                StackPush(&SPU->stack, sin(popped_elem));
                break;
            }
            case COS :
            {
                stack_elem_t popped_elem = 0;
                StackPop(&SPU->stack, &popped_elem);
                StackPush(&SPU->stack, cos(popped_elem));
                break;
            }
            case DUMP :
                SPU_Dump(SPU);
                break;
            case HLT :
                hlt_flag = true;
                break;
            case JMP :
                SPU->ip = SPU->code[SPU->ip];
                break;
            case JA :
            {
                stack_elem_t b_elem = 0, a_elem = 0;
                assert(!StackPop(&SPU->stack, &b_elem));
                assert(!StackPop(&SPU->stack, &a_elem));
                if (a_elem > b_elem)
                    SPU->ip = SPU->code[SPU->ip];
                else
                    SPU->ip++;
                break;
            }
            case JAE :
            {
                stack_elem_t b_elem = 0, a_elem = 0;
                assert(!StackPop(&SPU->stack, &b_elem));
                assert(!StackPop(&SPU->stack, &a_elem));
                if (a_elem >= b_elem)
                    SPU->ip = SPU->code[SPU->ip];
                else
                    SPU->ip++;
                break;
            }
            case JB :
            {
                stack_elem_t b_elem = 0, a_elem = 0;
                assert(!StackPop(&SPU->stack, &b_elem));
                assert(!StackPop(&SPU->stack, &a_elem));
                if (a_elem < b_elem)
                    SPU->ip = SPU->code[SPU->ip];
                else
                    SPU->ip++;
                break;
            }
            case JBE :
            {
                stack_elem_t b_elem = 0, a_elem = 0;
                assert(!StackPop(&SPU->stack, &b_elem));
                assert(!StackPop(&SPU->stack, &a_elem));
                if (a_elem <= b_elem)
                    SPU->ip = SPU->code[SPU->ip];
                else
                    SPU->ip++;
                break;
            }
            case JE :
            {
                stack_elem_t b_elem = 0, a_elem = 0;
                assert(!StackPop(&SPU->stack, &b_elem));
                assert(!StackPop(&SPU->stack, &a_elem));
                if (a_elem == b_elem)
                    SPU->ip = SPU->code[SPU->ip];
                else
                    SPU->ip++;
                break;
            }
            case JNE :
            {
                stack_elem_t b_elem = 0, a_elem = 0;
                assert(!StackPop(&SPU->stack, &b_elem));
                assert(!StackPop(&SPU->stack, &a_elem));
                if (a_elem != b_elem)
                    SPU->ip = SPU->code[SPU->ip];
                else
                    SPU->ip++;
                break;
            }
            default :
                printf("Error: default case. ip=%d, code[ip]=%d\n", SPU->ip - 1, SPU->code[SPU->ip-1]);
        }
    }
}

void DestroySPU(SPU_t *SPU)
{
    assert(SPU);

    free(SPU->code);
    SPU->code_size = 0;
    SPU->ip = 0;
    for (size_t i = 0; i < NUM_OF_REGISTERS; i++)
        SPU->registers[i] = 0;
    DestroyStack(&SPU->stack);
}

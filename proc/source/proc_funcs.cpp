#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "proc_funcs.h"

errors_t CreateSPU(SPU_t *SPU)
{
    MY_ASSERT(SPU, "Null pointer given as argument.", return ARG_NULL_PTR);


    SPU->RAM = (int *) calloc(SIZE_OF_RAM, sizeof(int));
    MY_ASSERT(SPU->RAM, "Allocation failed.", return NULL_PTR);

    for (int i = 0; i < SIZE_OF_RAM; i++)
        SPU->RAM[i] = 0;

    errors_t res = CreateStack(&SPU->stack, INITIAL_CAPACITY);
    if (res) return res;

    return SUCCESS;
}

errors_t ReadFile(const char *file_name, SPU_t *SPU)
{
    MY_ASSERT(file_name, "Null pointer given as argument.", return ARG_NULL_PTR);
    MY_ASSERT(SPU, "Null pointer given as argument.", return ARG_NULL_PTR);

    FILE *input = fopen(file_name, "rb");
    MY_ASSERT(input, "File opening error.", return FILE_NULL_PTR);

    int size = 0;
    fread(&size, sizeof(int), 1, input);
    SPU->code_size = (size_t) size;

    SPU->code = (int *) calloc(SPU->code_size, sizeof(int));
    MY_ASSERT(SPU->code, "Allocation failed.", fclose(input); return NULL_PTR);

    fread(SPU->code, sizeof(int), SPU->code_size, input);

    fclose(input);

    return SUCCESS;
}

errors_t SPU_Dump(SPU_t *SPU)
{
    MY_ASSERT(SPU, "Null pointer given as argument.", return ARG_NULL_PTR);

    FILE *dump_out = 0;
    static bool first_file_opening = true;
    if (first_file_opening)
    {
        dump_out = fopen(SPU_DUMP_FILE, "w");
        first_file_opening = false;
    }
    else
        dump_out = fopen(SPU_DUMP_FILE, "a");
    MY_ASSERT(dump_out, "File opening error.", return FILE_NULL_PTR);

    if (!SPU->code)
        fprintf(dump_out, "Error: code null ptr.\n");
    else
    {
        fprintf(dump_out, "code:\n");
        for (size_t i = 0; i < SPU->code_size; i++)
            fprintf(dump_out, "%8zX ", i);
        fprintf(dump_out, "\n");
        for (size_t i = 0; i < SPU->code_size; i++)
            fprintf(dump_out, "%8X ", SPU->code[i]);
        fprintf(dump_out, "\n");
        for (size_t i = 0; i < SPU->ip; i++)
            fprintf(dump_out, "         ");
        fprintf(dump_out, "^\n");
        for (size_t i = 0; i < SPU->ip; i++)
            fprintf(dump_out, "         ");
        fprintf(dump_out, "ip=%02zX\n\n", SPU->ip);
    }

    fprintf(dump_out, "registers:\n");
    for (size_t i = 0; i < NUM_OF_REGISTERS; i++)
        fprintf(dump_out, "r%zu=%g, ", i, ((double) SPU->registers[i]) / PRECISION);

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

errors_t Run(SPU_t *SPU)
{
    MY_ASSERT(SPU, "Null pointer given as argument.", return ARG_NULL_PTR);

    Stack_t call_stack = {};
    errors_t res = CreateStack(&call_stack, INITIAL_CAPACITY);
    if (res)
        return res;

    bool hlt_flag = false;
    while (SPU->ip < SPU->code_size && !hlt_flag)
    {
        switch (SPU->code[SPU->ip++])
        {
            case PUSH :
                res = StackPush(&SPU->stack, GetArg(SPU));
                if (res) return res;
                break;
            case POP :
            {
                stack_elem_t popped_elem = 0;
                res = StackPop(&SPU->stack, &popped_elem);
                if (res) return res;
                int arg_type = SPU->code[SPU->ip++];

                if (arg_type & RAM)
                {
                    if (arg_type & NUMBER) //TODO:  MAGIC
                        SPU->RAM[SPU->code[SPU->ip++]] = (int) (PRECISION * popped_elem);
                    else if (arg_type & REGISTER)
                        SPU->RAM[SPU->registers[SPU->code[SPU->ip++]] / PRECISION] =
                            (int) (PRECISION * popped_elem);
                }
                else
                    SPU->registers[SPU->code[SPU->ip++]] = (int) (PRECISION * popped_elem);

                break;
            }
            case ADD :
            {
                stack_elem_t popped_elem1 = 0, popped_elem2 = 0;
                res = StackPop(&SPU->stack, &popped_elem1);
                if (res) return res;
                res = StackPop(&SPU->stack, &popped_elem2);
                if (res) return res;
                res = StackPush(&SPU->stack, popped_elem1 + popped_elem2);
                if (res) return res;
                break;
            }
            case SUB :
            {
                stack_elem_t popped_elem1 = 0, popped_elem2 = 0;
                res = StackPop(&SPU->stack, &popped_elem1);
                if (res) return res;
                res = StackPop(&SPU->stack, &popped_elem2);
                if (res) return res;
                res = StackPush(&SPU->stack, popped_elem2 - popped_elem1);
                if (res) return res;
                break;
            }
            case MUL :
            {
                stack_elem_t popped_elem1 = 0, popped_elem2 = 0;
                res = StackPop(&SPU->stack, &popped_elem1);
                if (res) return res;
                res = StackPop(&SPU->stack, &popped_elem2);
                if (res) return res;
                res = StackPush(&SPU->stack, popped_elem1 * popped_elem2);
                if (res) return res;
                break;
            }
            case DIV :
            {
                stack_elem_t popped_elem1 = 0, popped_elem2 = 0;
                res = StackPop(&SPU->stack, &popped_elem1);
                if (res) return res;
                res = StackPop(&SPU->stack, &popped_elem2);
                if (res) return res;
                res = StackPush(&SPU->stack, popped_elem2 / popped_elem1);
                if (res) return res;
                break;
            }
            case OUT :
            {
                stack_elem_t popped_elem = 0;
                res = StackPop(&SPU->stack, &popped_elem);
                if (res) return res;
                printf("%g\n", popped_elem);
                break;
            }
            case IN :
            {
                stack_elem_t pushed_elem = 0;
                if (!scanf("%lf", &pushed_elem))
                    return SCAN_ERROR;
                res = StackPush(&SPU->stack, pushed_elem);
                if (res) return res;
                break;
            }
            case SQRT :
            {
                stack_elem_t popped_elem = 0;
                res = StackPop(&SPU->stack, &popped_elem);
                if (res) return res;
                res = StackPush(&SPU->stack, sqrt(popped_elem));
                if (res) return res;
                break;
            }
            case SIN :
            {
                stack_elem_t popped_elem = 0;
                res = StackPop(&SPU->stack, &popped_elem);
                if (res) return res;
                res = StackPush(&SPU->stack, sin(popped_elem));
                if (res) return res;
                break;
            }
            case COS :
            {
                stack_elem_t popped_elem = 0;
                res = StackPop(&SPU->stack, &popped_elem);
                if (res) return res;
                res = StackPush(&SPU->stack, cos(popped_elem));
                if (res) return res;
                break;
            }
            case DUMP :
                res = SPU_Dump(SPU);
                if (res) return res;
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
                res = StackPop(&SPU->stack, &b_elem);
                if (res) return res;
                res = StackPop(&SPU->stack, &a_elem);
                if (res) return res;

                if (a_elem > b_elem)
                    SPU->ip = SPU->code[SPU->ip];
                else
                    SPU->ip++;
                break;
            }
            case JAE :
            {
                stack_elem_t b_elem = 0, a_elem = 0;
                res = StackPop(&SPU->stack, &b_elem);
                if (res) return res;
                res = StackPop(&SPU->stack, &a_elem);
                if (res) return res;

                if (a_elem >= b_elem)
                    SPU->ip = SPU->code[SPU->ip];
                else
                    SPU->ip++;
                break;
            }
            case JB :
            {
                stack_elem_t b_elem = 0, a_elem = 0;
                res = StackPop(&SPU->stack, &b_elem);
                if (res) return res;
                res = StackPop(&SPU->stack, &a_elem);
                if (res) return res;

                if (a_elem < b_elem)
                    SPU->ip = SPU->code[SPU->ip];
                else
                    SPU->ip++;
                break;
            }
            case JBE :
            {
                stack_elem_t b_elem = 0, a_elem = 0;
                res = StackPop(&SPU->stack, &b_elem);
                if (res) return res;
                res = StackPop(&SPU->stack, &a_elem);
                if (res) return res;

                if (a_elem <= b_elem)
                    SPU->ip = SPU->code[SPU->ip];
                else
                    SPU->ip++;
                break;
            }
            case JE :
            {
                stack_elem_t b_elem = 0, a_elem = 0;
                res = StackPop(&SPU->stack, &b_elem);
                if (res) return res;
                res = StackPop(&SPU->stack, &a_elem);
                if (res) return res;
                if (a_elem == b_elem)
                    SPU->ip = SPU->code[SPU->ip];
                else
                    SPU->ip++;
                break;
            }
            case JNE :
            {
                stack_elem_t b_elem = 0, a_elem = 0;
                res = StackPop(&SPU->stack, &b_elem);
                if (res) return res;
                res = StackPop(&SPU->stack, &a_elem);
                if (res) return res;
                if (a_elem != b_elem)
                    SPU->ip = SPU->code[SPU->ip];
                else
                    SPU->ip++;
                break;
            }
            case CALL :
                res = StackPush(&call_stack, (stack_elem_t)(SPU->ip + 1));
                if (res) return res;
                SPU->ip = SPU->code[SPU->ip];
                break;
            case RET :
            {
                stack_elem_t popped_elem = 0;
                res = StackPop(&call_stack, &popped_elem);
                if (res) return res;
                SPU->ip = (size_t) popped_elem;
                break;
            }
            case CHR :
            {
                stack_elem_t popped_elem = 0;
                res = StackPop(&SPU->stack, &popped_elem);
                if (res) return res;
                printf("%c", (char) popped_elem);
                break;
            }
            default :
                printf("Error: default case. ip=%d, code[ip]=%d\n", SPU->ip - 1, SPU->code[SPU->ip-1]);
        }
    }

    DestroyStack(&call_stack);

    return SUCCESS;
}


stack_elem_t GetArg(SPU_t *SPU)
{
    MY_ASSERT(SPU, "Null pointer given as argument.", exit(EXIT_FAILURE));

    int arg_type = SPU->code[SPU->ip++];
    int res = SPU->code[SPU->ip++];

    if (arg_type & REGISTER)//TODO: MASK_MEM MASK_ARg
        res = SPU->registers[res];
    if (arg_type & RAM)
    {
        res = SPU->RAM[res / PRECISION];
    }

    return ((stack_elem_t) res) / PRECISION;
}

errors_t DestroySPU(SPU_t *SPU)
{
    MY_ASSERT(SPU, "Null poiner given as argument.", return ARG_NULL_PTR);

    free(SPU->code);
    SPU->code_size = 0;
    SPU->ip = 0;
    for (size_t i = 0; i < NUM_OF_REGISTERS; i++)
        SPU->registers[i] = 0;
    for (size_t i = 0; i < SIZE_OF_RAM; i++)
        SPU->RAM[i] = 0;

    free(SPU->RAM);

    DestroyStack(&SPU->stack);

    return SUCCESS;
}

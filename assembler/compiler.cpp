#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compiler.h"

void CreateCompiler(Compiler_t *compiler)
{
    assert(compiler);

    compiler->output = fopen("../program_code.txt", "wb");

    compiler->size_of_code = MIN_SIZE;

    compiler->code = (int *) calloc(MIN_SIZE, sizeof(int));
    assert(compiler->code);

    compiler->str_arr.size_of_arr = MIN_SIZE;

    compiler->str_arr.strings = (char (*)[MAX_CMD_LEN]) calloc(MIN_SIZE, sizeof(char) * MAX_CMD_LEN);
    assert(compiler->str_arr.strings);

    for (size_t i = 0; i < MIN_SIZE; i++)
        compiler->code[i] = CODE_POISON;
}

void DestroyCompiler(Compiler_t *compiler)
{
    assert(compiler);

    free(compiler->code);
    free(compiler->str_arr.strings);

    fclose(compiler->output);
}

void ReadAsmFile(Str_arr_struct *str_arr)
{
    assert(str_arr);

    FILE *input = fopen("../program.asm", "rb");
    assert(input);

    size_t i = 0;
    while (fscanf(input, "%s", str_arr->strings[i++]) != EOF)
    {
        if (str_arr->size_of_arr == i + 1)
        {
            str_arr->strings = (char (*)[MAX_CMD_LEN]) Recalloc(str_arr->strings,
                sizeof(char) * MAX_CMD_LEN * str_arr->size_of_arr,
                str_arr->size_of_arr * RECALLOC_COEF, sizeof(char) * MAX_CMD_LEN);
            str_arr->size_of_arr *= RECALLOC_COEF;
            assert(str_arr->strings);
        }
    }
    str_arr->num_of_strings = i - 1;

    fclose(input);
}

void Compile(Compiler_t *compiler)
{
    assert(compiler);

    ReadAsmFile(&compiler->str_arr);

    compiler->ip = 1;

    const int LABELS_SIZE = 20;
    Label_t *labels = (Label_t *) calloc(LABELS_SIZE, sizeof(Label_t));
    assert(labels);
    int num_of_labels = 0;

    for (size_t str_ind = 0,  i = 0; i < 2; i++, str_ind = 0)
    {
        while (str_ind < compiler->str_arr.num_of_strings)
        {
            if (compiler->size_of_code - compiler->ip < 3)
            {
                compiler->code =(int *) Recalloc(compiler->code, sizeof(int) * compiler->size_of_code,
                    compiler->size_of_code * CODE_RECALLOC_COEF, sizeof(int));
                compiler->size_of_code *= CODE_RECALLOC_COEF;
                assert(compiler->code);
            }

            switch(ReadCommand(compiler->str_arr.strings[str_ind++]))
            {
            case PUSH :
            {
                compiler->code[compiler->ip++] = PUSH;

                if (tolower((int) compiler->str_arr.strings[str_ind][0]) == 'r')
                {
                    compiler->code[compiler->ip++] = 1;
                    compiler->code[compiler->ip++] = atoi(compiler->str_arr.strings[str_ind++] + 1);
                }
                else
                {
                    compiler->code[compiler->ip++] = 0;
                    compiler->code[compiler->ip++] = (int) (PRECISION * atof(compiler->str_arr.strings[str_ind++]));
                }

                break;
            }
            case POP :
            {
                compiler->code[compiler->ip++] = POP;

                compiler->code[compiler->ip++] = atoi(compiler->str_arr.strings[str_ind++] + 1);
                break;
            }
            case ADD :
                compiler->code[compiler->ip++] = ADD;
                break;
            case SUB :
                compiler->code[compiler->ip++] = SUB;
                break;
            case MUL :
                compiler->code[compiler->ip++] = MUL;
                break;
            case DIV :
                compiler->code[compiler->ip++] = DIV;
                break;
            case OUT :
                compiler->code[compiler->ip++] = OUT;
                break;
            case IN :
                compiler->code[compiler->ip++] = IN;
                break;
            case SQRT :
                compiler->code[compiler->ip++] = SQRT;
                break;
            case SIN :
                compiler->code[compiler->ip++] = SIN;
                break;
            case COS :
                compiler->code[compiler->ip++] = COS;
                break;
            case DUMP :
                compiler->code[compiler->ip++] = DUMP;
                break;
            case HLT :
                compiler->code[compiler->ip++] = HLT;
                break;
            case JMP :
                compiler->code[compiler->ip++] = JMP;
                compiler->code[compiler->ip++] = JumpArg(compiler->str_arr.strings[str_ind++], num_of_labels, labels);
                break;
            case JA :
                compiler->code[compiler->ip++] = JA;
                compiler->code[compiler->ip++] = JumpArg(compiler->str_arr.strings[str_ind++], num_of_labels, labels);
                break;
            case JAE :
                compiler->code[compiler->ip++] = JAE;
                compiler->code[compiler->ip++] = JumpArg(compiler->str_arr.strings[str_ind++], num_of_labels, labels);
                break;
            case JB :
                compiler->code[compiler->ip++] = JB;
                compiler->code[compiler->ip++] = JumpArg(compiler->str_arr.strings[str_ind++], num_of_labels, labels);
                break;
            case JBE :
                compiler->code[compiler->ip++] = JBE;
                compiler->code[compiler->ip++] = JumpArg(compiler->str_arr.strings[str_ind++], num_of_labels, labels);
                break;
            case JE :
                compiler->code[compiler->ip++] = JE;
                compiler->code[compiler->ip++] = JumpArg(compiler->str_arr.strings[str_ind++], num_of_labels, labels);
                break;
            case JNE :
                compiler->code[compiler->ip++] = JNE;
                compiler->code[compiler->ip++] = JumpArg(compiler->str_arr.strings[str_ind++], num_of_labels, labels);
                break;
            default :
                strcpy(labels[num_of_labels].name, compiler->str_arr.strings[str_ind - 1]);
                labels[num_of_labels++].adres = compiler->ip - 1;
            }
        }

        compiler->ip = (i == 0) ? 1 : compiler->ip;
    }

    compiler->code[0] = compiler->ip - 1;

    fwrite(compiler->code, sizeof(int), compiler->ip, compiler->output);

    free(labels);
}

commands_t ReadCommand(const char command[MAX_CMD_LEN])
{
    assert(command);

    if (!strcmp(command, "push"))
        return PUSH;
    if (!strcmp(command, "pop"))
        return POP;
    if (!strcmp(command, "add"))
        return ADD;
    if (!strcmp(command, "sub"))
        return SUB;
    if (!strcmp(command, "mul"))
        return MUL;
     if (!strcmp(command, "div"))
        return DIV;
     if (!strcmp(command, "out"))
        return OUT;
     if (!strcmp(command, "in"))
        return IN;
     if (!strcmp(command, "sqrt"))
        return SQRT;
     if (!strcmp(command, "sin"))
        return SIN;
     if (!strcmp(command, "cos"))
        return COS;
     if (!strcmp(command, "dump"))
        return DUMP;
     if (!strcmp(command, "hlt"))
        return HLT;
     if (!strcmp(command, "jmp"))
        return JMP;
     if (!strcmp(command, "ja"))
        return JA;
     if (!strcmp(command, "jae"))
        return JAE;
     if (!strcmp(command, "jb"))
        return JB;
     if (!strcmp(command, "jbe"))
        return JBE;
     if (!strcmp(command, "je"))
        return JE;
     if (!strcmp(command, "jne"))
        return JNE;

    return NOT_COMMAND;
}

int JumpArg(const char *string, int num_of_labels, const Label_t *labels)
{
    assert(labels);

    int arg = 0;
    if (!sscanf(string, "%d", &arg))
    {

        for (int i = 0; i < num_of_labels; i++)
        {
            if (!strcmp(string, labels[i].name))
                arg = labels[i].adres;
        }
    }

    return arg;
}

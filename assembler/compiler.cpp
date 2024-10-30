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

    compiler->input = fopen("../program.asm", "r");
    compiler->output = fopen("../program_code.txt", "wb");
    assert(compiler->input);
    assert(compiler->output);

    compiler->size = MIN_SIZE;

    compiler->code = (int *) calloc(MIN_SIZE, sizeof(int));
    assert(compiler->code);

    for (size_t i = 0; i < MIN_SIZE; i++)
        compiler->code[i] = CODE_POISON;
}

void DestroyCompiler(Compiler_t *compiler)
{
    assert(compiler);

    free(compiler->code);

    fclose(compiler->input);
    fclose(compiler->output);
}

void Compile(Compiler_t *compiler)
{
    assert(compiler);


    char command[MAX_CMD_LEN] = {};

    compiler->ip = 1;

    const int LABELS_SIZE = 20;
    Label_t *labels = (Label_t *) calloc(LABELS_SIZE, sizeof(Label_t));
    assert(labels);
    int num_of_labels = 0;

    for (int i = 0; i < 2; i++)
    {
        while (fscanf(compiler->input, "%s", command) != EOF)
        {
            if (compiler->size - compiler->ip < 3)
            {
                compiler->code =(int *) Recalloc(compiler->code, sizeof(int) * compiler->size,
                    compiler->size * CODE_RECALLOC_COEF, sizeof(int));
                assert(compiler->code);
            }

            switch(ReadCommand(command))
            {
            case PUSH :
            {
                compiler->code[compiler->ip++] = PUSH;

                char arg[10] = "";
                assert(fscanf(compiler->input, "%s", arg) != EOF);

                if (tolower((int) arg[0]) == 'r')
                {
                    compiler->code[compiler->ip++] = 1;
                    compiler->code[compiler->ip++] = atoi(arg + 1);
                }
                else
                {
                    compiler->code[compiler->ip++] = 0;
                    compiler->code[compiler->ip++] = atoi(arg);
                }

                continue;
            }
            case POP :
            {
                compiler->code[compiler->ip++] = POP;

                char arg[4] = "";
                assert(fscanf(compiler->input, "%s", arg) != EOF);
                compiler->code[compiler->ip++] = atoi(arg + 1);

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
                compiler->code[compiler->ip++] = JumpArg(compiler->input, num_of_labels, labels);
                break;
            case JA :
                compiler->code[compiler->ip++] = JA;
                compiler->code[compiler->ip++] = JumpArg(compiler->input, num_of_labels, labels);
                break;
            case JAE :
                compiler->code[compiler->ip++] = JAE;
                compiler->code[compiler->ip++] = JumpArg(compiler->input, num_of_labels, labels);
                break;
            case JB :
                compiler->code[compiler->ip++] = JB;
                compiler->code[compiler->ip++] = JumpArg(compiler->input, num_of_labels, labels);
                break;
            case JBE :
                compiler->code[compiler->ip++] = JBE;
                compiler->code[compiler->ip++] = JumpArg(compiler->input, num_of_labels, labels);
                break;
            case JE :
                compiler->code[compiler->ip++] = JE;
                compiler->code[compiler->ip++] = JumpArg(compiler->input, num_of_labels, labels);
                break;
            case JNE :
                compiler->code[compiler->ip++] = JNE;
                compiler->code[compiler->ip++] = JumpArg(compiler->input, num_of_labels, labels);
                break;
            default :
                strcpy(labels[num_of_labels].name, command);
                labels[num_of_labels++].adres = compiler->ip - 1;
            }
    }
    printf("%u\n", labels[0].adres);
    fseek(compiler->input, 0, SEEK_SET);
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

int JumpArg(FILE *input, int num_of_labels, const Label_t *labels)
{
    assert(input);
    assert(labels);

    int arg = 0;
    if (!fscanf(input, "%d", &arg))
    {
        char lbl[MAX_CMD_LEN] = {};
        fscanf(input, "%s", lbl);
        for (int i = 0; i < num_of_labels; i++)
        {
            if (!strcmp(lbl, labels[i].name))
                arg = labels[i].adres;
        }
    }

    return arg;
}

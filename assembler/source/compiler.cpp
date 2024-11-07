#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdint.h>

#include "compiler.h"

void CreateCompiler(Compiler_t *compiler)
{
    assert(compiler);

    compiler->output = fopen("../program_code.txt", "wb");

    compiler->code_size = MIN_SIZE;

    compiler->code = (int *) calloc(MIN_SIZE, sizeof(int));
    assert(compiler->code);

    for (size_t i = 0; i < MIN_SIZE; i++)
        compiler->code[i] = CODE_POISON;

    compiler->labels = (Label_t *) calloc(INIT_LBLS_SIZE, sizeof(Label_t));
    assert(compiler->labels);
    compiler->labels_num = 0;
    compiler->labels_size = INIT_LBLS_SIZE;
}

void DestroyCompiler(Compiler_t *compiler)
{
    assert(compiler);

    free(compiler->code);
    free(compiler->buffer);
    free(compiler->labels);

    fclose(compiler->output);
}

void ReadAsmFile(Compiler_t *compiler)
{
    assert(compiler);

    FILE *input = fopen("../program.asm", "rb");
    assert(input);

    struct stat stbuf = {};
    stat("../program.asm", &stbuf);

    compiler->buf_size = stbuf.st_size + 1;

    compiler->buffer = (char *) calloc(compiler->buf_size, sizeof(char));
    assert(compiler->buffer);

    fread(compiler->buffer, sizeof(char), compiler->buf_size - 1, input);

    compiler->buffer[compiler->buf_size - 1] = '\0';

    fclose(input);
}

void WriteInFile(Compiler_t *compiler)
{
    assert(compiler);

    compiler->ip = 1;

    for (size_t iteration = 0, offset = 0; iteration < NUM_OF_ITERATIONS; iteration++, offset = 0)
    {
        char command[MAX_CMD_LEN] = {};
        int chrs_scanned = 0;

        while (sscanf(compiler->buffer + offset, "%s%n", command, &chrs_scanned) > 0)
        {
            offset += chrs_scanned;

            if (compiler->code_size - compiler->ip < MIN_DIFFERENCE)
            {
                compiler->code = (int *) Recalloc(compiler->code, compiler->code_size,
                    compiler->code_size * CODE_RECALLOC_COEF, sizeof(int));
                compiler->code_size *= CODE_RECALLOC_COEF;
                assert(compiler->code);
            }

            if (compiler->labels_size - compiler->labels_num < 1)
            {
                compiler->labels = (Label_t *) Recalloc(compiler->labels, compiler->labels_size,
                    compiler->labels_size * RECALLOC_COEF, sizeof(Label_t));
                compiler->labels_size *= RECALLOC_COEF;
                assert(compiler->labels);
            }

            switch(ReadCommand(command))
            {
            case PUSH:
                compiler->code[compiler->ip++] = PUSH;
                ReadPushArg(compiler, &offset);
                break;
            case POP :
            {
                compiler->code[compiler->ip++] = POP;
                ReadPopArg(compiler, &offset);
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
                compiler->code[compiler->ip++] = ReadJumpArg(compiler, &offset);
                break;
            case JA :
                compiler->code[compiler->ip++] = JA;
                compiler->code[compiler->ip++] = ReadJumpArg(compiler, &offset);
                break;
            case JAE :
                compiler->code[compiler->ip++] = JAE;
                compiler->code[compiler->ip++] = ReadJumpArg(compiler, &offset);
                break;
            case JB :
                compiler->code[compiler->ip++] = JB;
                compiler->code[compiler->ip++] = ReadJumpArg(compiler, &offset);
                break;
            case JBE :
                compiler->code[compiler->ip++] = JBE;
                compiler->code[compiler->ip++] = ReadJumpArg(compiler, &offset);
                break;
            case JE :
                compiler->code[compiler->ip++] = JE;
                compiler->code[compiler->ip++] = ReadJumpArg(compiler, &offset);
                break;
            case JNE :
                compiler->code[compiler->ip++] = JNE;
                compiler->code[compiler->ip++] = ReadJumpArg(compiler, &offset);
                break;
            default :
                strcpy(compiler->labels[compiler->labels_num].name, command);
                compiler->labels[compiler->labels_num++].adres = compiler->ip - 1;
            }

            memset(command, '\0', MAX_CMD_LEN);
        }

        compiler->ip = (iteration == 0) ? 1 : compiler->ip;
    }

    compiler->code[0] = compiler->ip - 1;

    fwrite(compiler->code, sizeof(int), compiler->ip, compiler->output);
}

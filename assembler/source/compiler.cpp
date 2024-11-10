#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdint.h>

#include "compiler.h"

errors_t CreateCompiler(Compiler_t *compiler, const char *out_file_name)
{
    MY_ASSERT(compiler, "Null pointer given as argument.", return ARG_NULL_PTR);
    MY_ASSERT(out_file_name, "Null pointer given as argument.", return ARG_NULL_PTR);

    compiler->output = fopen(out_file_name, "wb");
    MY_ASSERT(compiler->output, "File opening error.", return FILE_NULL_PTR);

    compiler->code_size = MIN_SIZE;

    compiler->code = (int *) calloc(MIN_SIZE, sizeof(int));
    MY_ASSERT(compiler->code, "Allocation failed.", return NULL_PTR);

    for (size_t i = 0; i < MIN_SIZE; i++)
        compiler->code[i] = CODE_POISON;

    compiler->labels = (Label_t *) calloc(INIT_LBLS_SIZE, sizeof(Label_t));
    MY_ASSERT(compiler->labels, "Allocation failed.", return NULL_PTR);
    compiler->labels_num = 0;
    compiler->labels_size = INIT_LBLS_SIZE;

    return SUCCESS;
}

errors_t DestroyCompiler(Compiler_t *compiler)
{
    MY_ASSERT(compiler, "Null pointer given as argument.", return ARG_NULL_PTR);

    free(compiler->code);
    free(compiler->buffer);
    free(compiler->labels);

    fclose(compiler->output);

    return SUCCESS;
}

errors_t ReadAsmFile(Compiler_t *compiler, const char *file_name)
{
    MY_ASSERT(compiler, "Null pointer given as argument.", return ARG_NULL_PTR);
    MY_ASSERT(file_name, "Null pointer given as argument.", return ARG_NULL_PTR);

    FILE *input = fopen(file_name, "rb");
    MY_ASSERT(input, "File opening error.", return FILE_NULL_PTR);

    struct stat stbuf = {};
    stat(file_name, &stbuf);

    compiler->buf_size = stbuf.st_size + 1;

    compiler->buffer = (char *) calloc(compiler->buf_size, sizeof(char));
    MY_ASSERT(compiler->buffer, "Allocation failed.", fclose(input); return NULL_PTR);

    fread(compiler->buffer, sizeof(char), compiler->buf_size - 1, input);

    compiler->buffer[compiler->buf_size - 1] = '\0';

    fclose(input);

    return SUCCESS;
}

errors_t WriteInFile(Compiler_t *compiler)
{
    MY_ASSERT(compiler, "Null pointer given as argument.", return ARG_NULL_PTR);

    compiler->ip = 1;

    for (size_t iteration = 0, offset = 0; iteration < NUM_OF_ITERATIONS; iteration++, offset = 0)
    {
        char command[MAX_CMD_LEN] = {};
        int chrs_scanned = 0;

        while (sscanf(compiler->buffer + offset, "%s%n", command, &chrs_scanned) > 0)
        {
            comment_t res = SkipCommentAftCmd(compiler, &offset, command);
            if (res == STARTS_WITH_COMMENT)
                continue;
            else if (res == NO_COMMENTS)
                offset += chrs_scanned;

            ON_ASM_DBG(printf("%s\n", command);)

            if (compiler->code_size - compiler->ip < MIN_DIFFERENCE)
            {
                compiler->code = (int *) Recalloc(compiler->code, compiler->code_size,
                    compiler->code_size * CODE_RECALLOC_COEF, sizeof(int));
                compiler->code_size *= CODE_RECALLOC_COEF;
                if (!compiler->code)
                    return NULL_PTR;
            }

            if (compiler->labels_size - compiler->labels_num < 1)
            {
                compiler->labels = (Label_t *) Recalloc(compiler->labels, compiler->labels_size,
                    compiler->labels_size * RECALLOC_COEF, sizeof(Label_t));
                compiler->labels_size *= RECALLOC_COEF;
                if (!compiler->labels)
                    return NULL_PTR;
            }

            switch(ReadCommand(command))
            {
                case PUSH:
                {
                    compiler->code[compiler->ip++] = PUSH;
                    errors_t result = ReadPushArg(compiler, &offset);
                    if (result) return result;
                    break;
                }
                case POP :
                {
                    compiler->code[compiler->ip++] = POP;
                    errors_t result = ReadPopArg(compiler, &offset);
                    if (result) return result;
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
                case CALL :
                    compiler->code[compiler->ip++] = CALL;
                    compiler->code[compiler->ip++] = ReadJumpArg(compiler, &offset);
                    break;
                case RET :
                    compiler->code[compiler->ip++] = RET;
                    break;
                case CHR :
                    compiler->code[compiler->ip++] = CHR;
                    break;
                default :
                    if (strchr(command, ':') != NULL)
                        *strchr(command, ':') = '\0';
                    strcpy(compiler->labels[compiler->labels_num].name, command);
                    compiler->labels[compiler->labels_num++].adres = compiler->ip - 1;
            }

            memset(command, '\0', MAX_CMD_LEN);
        }

        compiler->ip = (iteration == 0) ? 1 : compiler->ip;
    }

    compiler->code[0] = compiler->ip - 1;

    fwrite(compiler->code, sizeof(int), compiler->ip, compiler->output);

    return SUCCESS;
}



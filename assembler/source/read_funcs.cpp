#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "read_funcs.h"

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
    if (!strcmp(command, "call"))
        return CALL;
    if (!strcmp(command, "ret"))
        return RET;

    return NOT_COMMAND;
}

void ReadPopArg(Compiler_t *compiler, size_t *offset)
{
    assert(compiler);
    assert(offset);

    char arg[MAX_CMD_LEN] = {};
    int chrs_scanned = 0;
    assert(sscanf(compiler->buffer + *offset, "%s%n", arg, &chrs_scanned));

    if (SkipCommentAftArg(compiler, offset, arg) == NO_COMMENTS)
        *offset += chrs_scanned;

    if (arg[0] == '[')
    {
        compiler->code[compiler->ip] = RAM;
        if (tolower((int) arg[1]) == 'r')
        {
            compiler->code[compiler->ip++] |= REGISTER;
            sscanf(arg + 2, "%d]", &compiler->code[compiler->ip++]);
        }
        else
        {
            compiler->code[compiler->ip++] |= NUMBER;
            sscanf(arg + 1, "%d]", &compiler->code[compiler->ip++]);
        }
    }
    else
    {
        compiler->code[compiler->ip++] = REGISTER;
        sscanf(arg + 1, "%d", &compiler->code[compiler->ip++]);
    }
}

void ReadPushArg(Compiler_t *compiler, size_t *offset)
{
    assert(compiler);
    assert(offset);

    char arg[MAX_CMD_LEN] = {};
    int chrs_scanned = 0;
    assert(sscanf(compiler->buffer + *offset, "%s%n", arg, &chrs_scanned));

    if (SkipCommentAftArg(compiler, offset, arg) == NO_COMMENTS)
        *offset += chrs_scanned;

    if (arg[0] == '[')
    {
        compiler->code[compiler->ip] = RAM;
        if (tolower((int) arg[1]) == 'r')
        {
            compiler->code[compiler->ip++] |= REGISTER;
            sscanf(arg + 2, "%d]", &compiler->code[compiler->ip++]);
        }
        else
        {
            compiler->code[compiler->ip++] |= NUMBER;
            sscanf(arg + 1, "%d]", &compiler->code[compiler->ip]);
            compiler->code[compiler->ip++] *= PRECISION;
        }
    }
    else if (tolower((int) arg[0]) == 'r')
    {
        compiler->code[compiler->ip++] = REGISTER;
        sscanf(arg + 1, "%d", &compiler->code[compiler->ip++]);
    }
    else
    {
        compiler->code[compiler->ip++] = NUMBER;
        compiler->code[compiler->ip++] =
            (int) (PRECISION * atof(arg));
    }
}

int ReadJumpArg(Compiler_t *compiler, size_t *offset)
{
    assert(compiler);
    assert(offset);

    char arg[MAX_CMD_LEN] = {};
    int chrs_scanned = 0;
    assert(sscanf(compiler->buffer + *offset, "%s%n", arg, &chrs_scanned));

    if (SkipCommentAftArg(compiler, offset, arg) == NO_COMMENTS)
        *offset += chrs_scanned;

    int adres = 0;
    if (!sscanf(arg, "%d", &adres))
    {
        if (strchr(arg, ':') != NULL)
            *strchr(arg, ':') = '\0';
        for (size_t i = 0; i < compiler->labels_num; i++)
        {
            if (!strcmp(arg, compiler->labels[i].name))
                adres = compiler->labels[i].adres;
        }
    }

    return adres;
}

comment_t SkipCommentAftArg(Compiler_t *compiler, size_t *offset, char *arg)
{
    if (strchr(arg, ';') != NULL)
    {
        *strchr(arg, ';') = '\0';
        (*offset) = strchr(compiler->buffer + *offset, ';') - compiler->buffer;
        return HAS_COMMENT;
    }
    else if (strchr(arg, '*') != NULL)
    {
        *strchr(arg, '*') = '\0';
        (*offset) = strchr(compiler->buffer + *offset, '*') - compiler->buffer;
        return HAS_COMMENT;
    }

    return NO_COMMENTS;
}

comment_t SkipCommentAftCmd(Compiler_t *compiler, size_t *offset, char *command)
{
    if (strchr(command, ';') != NULL && command[0] != ';')
    {
        *strchr(command, ';') = '\0';
        *offset = strchr(compiler->buffer + *offset, ';') - compiler->buffer;
        return HAS_COMMENT;
    }
    else if (command[0] == ';')
    {
        *offset = strchr(compiler->buffer + *offset, '\n') + 1 - compiler->buffer;
        memset(command, '\0', MAX_CMD_LEN);
        return STARTS_WITH_COMMENT;
    }
    else if (strchr(command, '*') != NULL && command[0] != '*')
    {
        *strchr(command, '*') = '\0';
        *offset = strchr(compiler->buffer + *offset, '*') - compiler->buffer;
        return HAS_COMMENT;
    }
    else if (command[0] == '*')
    {
        *offset = strchr(strchr(compiler->buffer + *offset, '*') + 1, '*') + 1 - compiler->buffer;
        memset(command, '\0', MAX_CMD_LEN);
        return STARTS_WITH_COMMENT;
    }

    return NO_COMMENTS;
}


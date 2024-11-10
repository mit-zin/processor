#ifndef READ_FUNCS_H
#define READ_FUNCS_H

#include "assembler.h"

commands_t ReadCommand(const char command[MAX_CMD_LEN]);

errors_t ReadPushArg(Compiler_t *compiler, size_t *offset);
errors_t ReadPopArg (Compiler_t *compiler, size_t *offset);
int  ReadJumpArg(Compiler_t *compiler, size_t *offset);

comment_t SkipCommentAftArg(Compiler_t *compiler, size_t *offset, char *arg);
comment_t SkipCommentAftCmd(Compiler_t *compiler, size_t *offset, char *command);

#endif

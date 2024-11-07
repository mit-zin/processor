#ifndef READ_FUNCS_H
#define READ_FUNCS_H

#include "assembler.h"

commands_t ReadCommand(const char command[MAX_CMD_LEN]);

void ReadPushArg(Compiler_t *compiler, size_t *offset);
void ReadPopArg (Compiler_t *compiler, size_t *offset);
int  ReadJumpArg(Compiler_t *compiler, size_t *offset);

#endif
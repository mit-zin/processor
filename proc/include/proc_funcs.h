#ifndef PROC_FUNCS_H
#define PROC_FUNKS_H

#include "SPU.h"
#include "../../common.h"

const size_t INITIAL_CAPACITY = 30;

const char *SPU_DUMP_FILE = "./SPU_dump.txt";

void     CreateSPU (SPU_t *SPU);
void     DestroySPU(SPU_t *SPU);
void     ReadFile  (const char *filename, SPU_t *SPU);
errors_t SPU_Dump  (SPU_t *SPU);
void     Run       (SPU_t *SPU);
stack_elem_t GetArg(SPU_t *SPU);

#endif

#ifndef PROC_FUNCS_H
#define PROC_FUNKS_H

#include "SPU.h"
#include "../../common.h"

static const char *SPU_DUMP_FILE = "./SPU_dump.txt";

const size_t INITIAL_CAPACITY = 30;

errors_t CreateSPU (SPU_t *SPU);
errors_t DestroySPU(SPU_t *SPU);
errors_t ReadFile  (const char *filename, SPU_t *SPU);
errors_t SPU_Dump  (SPU_t *SPU);
errors_t Run       (SPU_t *SPU);
stack_elem_t GetArg(SPU_t *SPU);

#endif

#include <stdio.h>
#include <stdlib.h>

#include "proc_funcs.h"
#include "SPU.h"

int main()
{
    SPU_t SPU = {};
    unsigned int res = 0;
    res |= CreateSPU(&SPU);

    if (!res)
    {
        res |= ReadFile("../program_code.bin", &SPU);

        if (!res)
            res |= Run(&SPU);

        res |= DestroySPU(&SPU);
    }

    if (res)
        PrintErr(res);

    printf("End of program.\n");

    return 0;
}

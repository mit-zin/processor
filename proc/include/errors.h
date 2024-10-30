#ifndef ERRORS_H
#define ERRORS_H

typedef enum
{
    SUCCESS                = 0,
    FILE_NULL_PTR          = 1,
    STACK_ERROR            = 2,
    NULL_PTR               = 3,
    ARG_NULL_PTR           = 4,
    WRONG_ARG              = 5,
    CAPACITY_LIMIT_REACHED = 6,
    UNKNOWN_ERROR          = 7
} errors_t;

#endif

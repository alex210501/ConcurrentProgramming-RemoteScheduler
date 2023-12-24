#ifndef __DEFS_H__
#define __DEFS_H__

#include <stdlib.h>
#include <stdio.h>

typedef enum {
    ACTIVATION = 0,
    DEACTIVATION,
    EXIT,
} Actions_t;

typedef enum {
    SUCCESS = 0,
    TASK_DOES_NOT_EXIST,
    NOT_SCHEDULABLE,
} Error_t;

void actionToString(Actions_t action, char* dest, size_t dest_size) {
    switch (action) {
    case ACTIVATION:
        snprintf(dest, dest_size, "Activation");
        break;
    case DEACTIVATION:
        snprintf(dest, dest_size, "Deactivation");
        break;
    case EXIT:
        snprintf(dest, dest_size, "Exit");
        break;
    default:
        break;
    }
}

void errorToString(Error_t error, char* dest, size_t dest_size) {
    switch (error) {
    case SUCCESS:
        snprintf(dest, dest_size, "SUCCESS");
        break;
    case TASK_DOES_NOT_EXIST:
        snprintf(dest, dest_size, "TASK DOES NOT EXIST");
        break;
    case NOT_SCHEDULABLE:
        snprintf(dest, dest_size, "NOT SCHEDULABLE");
        break;
    default:
        break;
    }
}

#endif /* __DEFS_H__ */

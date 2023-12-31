#ifndef __DEFS_H__
#define __DEFS_H__

#include <stdlib.h>
#include <stdio.h>

#define PORT (8080)
#define ACTION_VALID(action) (action >= 0 && action < ACTIONS_MAX)
#define RELY_ON_TASK(action) (action == ACTIVATION || action == DEACTIVATION)

typedef enum {
    ACTIVATION = 0,
    DEACTIVATION,
    EXIT,
    SHOW_STATUS,
    ACTIONS_MAX,
} Actions_t;

typedef enum {
    SUCCESS = 0,
    TASK_DOES_NOT_EXIST,
    NOT_SCHEDULABLE,
    INVALID_ACTION,
    TASK_NOT_ACTIVATED,
    ERRORS_MAX,
} Error_t;

void actionToString(Actions_t action, char* dest, size_t dest_size) {
    switch (action) {
    case ACTIVATION:
        snprintf(dest, dest_size, "Activation");
        break;
    case DEACTIVATION:
        snprintf(dest, dest_size, "Deactivation");
        break;
    case SHOW_STATUS:
        snprintf(dest, dest_size, "Show status");
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
    case INVALID_ACTION:
        snprintf(dest, dest_size, "INVALID ACTION");
        break;
    case TASK_NOT_ACTIVATED:
        snprintf(dest, dest_size, "TASK NOT ACTIVATED");
        break;
    default:
        break;
    }
}

#endif /* __DEFS_H__ */

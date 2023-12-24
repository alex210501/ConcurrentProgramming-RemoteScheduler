#ifndef __DEFS_H__
#define __DEFS_H__

#include <stdlib.h>
#include <stdio.h>

typedef enum {
    ACTIVATION = 0,
    DEACTIVATION,
} Actions_t;

void actionToString(Actions_t action, char* dest, size_t dest_size) {
    switch (action)
    {
    case ACTIVATION:
        snprintf(dest, dest_size, "Activation");
        break;
    case DEACTIVATION:
        snprintf(dest, dest_size, "Deactivation");
        break;
    default:
        break;
    }
}

#endif /* __DEFS_H__ */

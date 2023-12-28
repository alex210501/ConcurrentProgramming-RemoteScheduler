#ifndef __TASK_H__
#define __TASK_H__

#include <unistd.h>

// Macro used to create tasks
#define CONCAT_IMPL( x, y ) x##y
#define CONCAT( x, y ) CONCAT_IMPL( x, y )
#define CREATE_TASK(timeout) \
    void CONCAT( task_, __COUNTER__ )(void) { \
        usleep(timeout); \ 
    } \


typedef void (*task_callback_t)(void);

void task_handler(task_callback_t callback) {
    // TODO - Add task to scheduler

    callback();

    // TODO - Remove task from scheduler
}

#endif /* __TASK_H__ */
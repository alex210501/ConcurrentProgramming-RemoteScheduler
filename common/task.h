#ifndef __TASK_H__
#define __TASK_H__

#include <pthread.h>
#include <time.h>
#include <unistd.h>

#include "queue.h"

#define TASKS_NUMBER       (4)
#define MAX_TASKS          (20)
#define MEASURE_ITERATIONS (5)
#define MAX(a, b)          (a >= b ? a : b)


// Macro used to create tasks
#define CONCAT_IMPL( x, y ) x##y
#define CONCAT( x, y ) CONCAT_IMPL( x, y )
#define CREATE_TASK(count) \
    void CONCAT( task_, __COUNTER__ )(void) { \
        for(volatile int i = 0; i < count; i++); \
    } \


typedef double task_time_t;
typedef double task_cpu_usage_t;
typedef double task_period_t;
typedef void (*task_callback_t)(void);

typedef struct {
    double cpu_usage;
    int num_tasks;
    struct {
        int task_id;
        int task_count;
    } tasks[MAX_TASKS]; 
} task_status_t;

typedef struct {
    task_callback_t callback;
    task_time_t execution_time;
    task_cpu_usage_t cpu_usage;
    task_period_t period;
} task_info_t;

typedef struct {
    task_cpu_usage_t cpu_usage;
    queue_t tasks_running[TASKS_NUMBER];
    pthread_mutex_t lock;
} scheduler_info_t;

typedef struct {
    int task;
    int running;
    pthread_t thread_id;
    task_info_t* task_info;
    scheduler_info_t* scheduler_info;
} task_handler_arg_t;

void measure_time(task_info_t tasks[], size_t size) {
    for (int i = 0; i < size; i++) {
        tasks[i].execution_time = 0.0;
        
        for (int j = 0; j < MEASURE_ITERATIONS; j++) {
            struct timespec start, end;
            clock_gettime(CLOCK_MONOTONIC, &start);

            tasks[i].callback();
            
            clock_gettime(CLOCK_MONOTONIC, &end);

            // Measure the maximum execution time
            double execution_time = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_nsec - start.tv_nsec) / 1000000.0;
            tasks[i].execution_time = MAX(tasks[i].execution_time, execution_time);
            tasks[i].cpu_usage = (double)tasks[i].execution_time / tasks[i].period;
        }
    }
}

int is_schedulable(double cpu_usage, double task_cpu_usage) {
    return (cpu_usage + task_cpu_usage) <= 1.0;
}

#endif /* __TASK_H__ */
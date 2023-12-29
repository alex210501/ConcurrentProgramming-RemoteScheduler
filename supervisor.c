#include <pthread.h>

#include "common/defs.h"
#include "common/task.h"
#include "tcp/tcp-server.h"

CREATE_TASK(100000)
CREATE_TASK(200000)
CREATE_TASK(300000)
CREATE_TASK(400000)

task_info_t tasks[] = {
    { .callback = &task_0, .period = 10, },
    { .callback = &task_1, .period = 10, },
    { .callback = &task_2, .period = 20, },
    { .callback = &task_3, .period = 20, },
};

scheduler_info_t scheduler_info;

void* task_handler(void* arg) {
    task_handler_arg_t* handler_arg = (task_handler_arg_t *)arg;

    if (handler_arg == NULL || handler_arg->task_info == NULL)
        return NULL;

    rounded_queue_t* q = &(handler_arg->scheduler_info->tasks_running[handler_arg->task]);
    running_task_arg_t running_task_arg = { .running = 1, .thread_id = *(handler_arg->thread_id) };

    handler_arg->scheduler_info->cpu_usage += handler_arg->task_info->cpu_usage;
    enqueue(q, (void*)&running_task_arg);

    while (running_task_arg.running) {
        handler_arg->task_info->callback();
        // sleep(1);
    }

    dequeue(q);
    handler_arg->scheduler_info->cpu_usage -= handler_arg->task_info->cpu_usage;
    free(handler_arg);
}

void print_status(scheduler_info_t* info) {
    printf("--- STATUS ---\n");
    printf("CPU usage - %f\n", info->cpu_usage);

    for (int i = 0; i < TASKS_NUMBER; i++) {
        printf("Task %d - %d\n", i, info->tasks_running[i].counter);
    }
}

void deinit_tasks(scheduler_info_t* info) {
    if (info == NULL) return;

    for (int i = 0; i < TASKS_NUMBER; i++) {
        rounded_queue_t* q = &(info->tasks_running[i]);

        while (q->counter) {
            running_task_arg_t* arg = get_top(q);

            arg->running = 0;
            pthread_join(arg->thread_id, NULL);
            dequeue(q);
        }
    }
}

void tcp_server_callback(int connfd) {
    for (;;) {
        Request_t req;
        Answer_t ans = { .error = SUCCESS };

        // Read data received by the client
        read(connfd, (void*)&req, sizeof(Request_t));

        // TODO - Debug print
        printf("Data received: %d - %d\n", req.action, req.task);

        // Check action
        if (!ACTION_VALID(req.action)) {
            printf("Invalid action %d!\n", req.action);
            ans.error = INVALID_ACTION;
            goto ERR;
        } 
        
        if (RELY_ON_TASK(req.action) && (req.task < 0 || req.task >= TASKS_NUMBER)) {
            printf("Task %d invalid!\n", req.task);
            ans.error = TASK_DOES_NOT_EXIST;
            goto ERR;
        }

        // Exit if the client requested it
        if (req.action == EXIT) break;

        switch (req.action) {
        case ACTIVATION:
            // Check if it's schedulable
            if (scheduler_info.cpu_usage + tasks[req.task].cpu_usage > 1.0) {
                ans.error = NOT_SCHEDULABLE;
                goto ERR;
            }

            pthread_t thread;
            task_handler_arg_t* arg = malloc(sizeof(task_handler_arg_t));
            
            arg->scheduler_info = &scheduler_info;
            arg->task_info = &tasks[req.task];
            arg->task = req.task;
            arg->thread_id = &thread;

            pthread_create(&thread, NULL, &task_handler, arg);
            break;
        case DEACTIVATION:
            rounded_queue_t* q = &scheduler_info.tasks_running[req.task];
            // TODO - Manage if deactivate an unactivate task

            if (!is_empty(q)){
                running_task_arg_t* arg = (running_task_arg_t*)get_top(q);
                arg->running = 0;
                pthread_join(arg->thread_id, NULL);
            }

            break;
        case SHOW_STATUS:
            print_status(&scheduler_info);
            break;
        default:
            break;
        }

ERR:

        write(connfd, (void*)&ans, sizeof(Answer_t));
    }
}

int main() {
    measure_time(tasks, TASKS_NUMBER);

    for (int i = 0; i < TASKS_NUMBER; i++) {
        printf("Task %d - %lf - usage: %lf\n", i, tasks[i].time, tasks[i].cpu_usage);
    }

    init_tcp_server(tcp_server_callback, PORT);

    deinit_tasks(&scheduler_info);

    return 0;
}
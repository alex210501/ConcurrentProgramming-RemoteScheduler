#include <pthread.h>
#include <semaphore.h>

#include "common/defs.h"
#include "common/task.h"
#include "tcp/tcp-server.h"

// #define SHOW_PRINT_TASK  // Define if we print something in the task
#define MAX_TASK_TO_DELETE (10)

CREATE_TASK(1000000)
CREATE_TASK(1000000)
CREATE_TASK(1000000)
CREATE_TASK(1000000)

struct {
    task_handler_arg_t* buf[MAX_TASK_TO_DELETE];
    pthread_t thread_id;
    sem_t empty;
    sem_t full;
    pthread_mutex_t lock;
    queue_t queue;
} task_to_delete;

task_info_t tasks[] = {
    { .callback = &task_0, .period = 10.0, },
    { .callback = &task_1, .period = 20.0, },
    { .callback = &task_2, .period = 30.0, },
    { .callback = &task_3, .period = 40.0, },
};

scheduler_info_t scheduler_info;

void get_tasks_status(scheduler_info_t* info, task_status_t* tasks_status) {
    if (tasks_status == NULL) return;

    tasks_status->cpu_usage = info->cpu_usage;
    tasks_status->num_tasks = TASKS_NUMBER;

    for (int i = 0; i < TASKS_NUMBER; i++) {
        tasks_status->tasks[i].task_id = i;
        tasks_status->tasks[i].task_count = info->tasks_running[i].counter;
    }
}

void* delete_task_thread(void* arg) {
    for (;;) {
        sem_wait(&task_to_delete.full);

        // Put thread ID instead
        if (pthread_mutex_lock(&task_to_delete.lock) != 0) {
            printf("Error while getting the lock!");
            return NULL;
        }

        task_handler_arg_t* arg = dequeue(&task_to_delete.queue);

        pthread_mutex_unlock(&task_to_delete.lock);
        sem_post(&task_to_delete.empty);

        // Stop the running task
        arg->running = 0;
        pthread_join(arg->thread_id, NULL);

        // Free the args resources
        free(arg);
    }
}

void* task_handler(void* arg) {
    task_handler_arg_t* handler_arg = (task_handler_arg_t *)arg;

    if (handler_arg == NULL || handler_arg->task_info == NULL)
        return NULL;

    queue_t* q = &(handler_arg->scheduler_info->tasks_running[handler_arg->task]);
    double time_to_sleep_ms = handler_arg->task_info->period - handler_arg->task_info->execution_time;
    struct timespec request = { 0, time_to_sleep_ms * 1000000 };

    // Update CPU usage
    if (pthread_mutex_lock(&handler_arg->scheduler_info->lock) == 0) {
        enqueue(q, (void*)handler_arg);
        handler_arg->scheduler_info->cpu_usage += handler_arg->task_info->cpu_usage;
        pthread_mutex_unlock(&handler_arg->scheduler_info->lock);   
    }

    // Run periodically
    while (handler_arg->running) {
        handler_arg->task_info->callback();
        nanosleep(&request, NULL);
    
#ifdef SHOW_PRINT_TASK
        printf("Task %d\n", handler_arg->task);
        sleep(1);
#endif
    }

    // Update CPU usage
    if (pthread_mutex_lock(&handler_arg->scheduler_info->lock) == 0) {
        dequeue(q);
        handler_arg->scheduler_info->cpu_usage -= handler_arg->task_info->cpu_usage;
        pthread_mutex_unlock(&handler_arg->scheduler_info->lock);
    }
}

void print_status(scheduler_info_t* info) {
    printf("\n--- STATUS ---\n");
    printf("CPU usage - %f\n", info->cpu_usage);

    for (int i = 0; i < TASKS_NUMBER; i++) {
        printf("Task %d - %d\n", i, info->tasks_running[i].counter);
    }
}

void deinit_tasks(scheduler_info_t* info) {
    if (info == NULL) return;

    for (int i = 0; i < TASKS_NUMBER; i++) {
        queue_t* q = &(info->tasks_running[i]);

        while (q->counter) {
            task_handler_arg_t* arg = get_top(q);

            arg->running = 0;
            pthread_join(arg->thread_id, NULL);
            dequeue(q);
            free(arg);
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

        switch (req.action) {
        case ACTIVATION:
            // Check if it's schedulable
            if (!is_schedulable(scheduler_info.cpu_usage, tasks[req.task].cpu_usage)) {
                ans.error = NOT_SCHEDULABLE;
                goto ERR;
            }

            // Freed in the delete task thread
            task_handler_arg_t* arg = malloc(sizeof(task_handler_arg_t));
            
            // Initialize parameters
            arg->running = 1;
            arg->scheduler_info = &scheduler_info;
            arg->task_info = &tasks[req.task];
            arg->task = req.task;

            pthread_create(&arg->thread_id, NULL, &task_handler, arg);
            break;
        case DEACTIVATION:
            queue_t* q = &scheduler_info.tasks_running[req.task];

            // If the queue is empty, then no such task is activated right now
            if (is_empty(q)) {
                ans.error = TASK_NOT_ACTIVATED;
                goto ERR;
            }

            sem_wait(&task_to_delete.empty);

            if (pthread_mutex_lock(&task_to_delete.lock) == 0) {
                enqueue(&task_to_delete.queue, get_top(q));
                pthread_mutex_unlock(&task_to_delete.lock);   
                sem_post(&task_to_delete.full);
            }

            break;
        case SHOW_STATUS:
            task_status_t status;

            print_status(&scheduler_info);
            get_tasks_status(&scheduler_info, &status);

            memcpy(ans.frame, &status, sizeof(task_status_t));

            break;
        default:
            break;
        }

ERR:
        write(connfd, (void*)&ans, sizeof(Answer_t));

        // Exit if the client requested it
        if (req.action == EXIT) break;
    }
}

int main() {
    // Initialise the thread to delete the deactivated tasks
    pthread_create(&task_to_delete.thread_id, NULL, &delete_task_thread, NULL);

    // Initialise lock
    pthread_mutex_init(&scheduler_info.lock, NULL);
    pthread_mutex_init(&task_to_delete.lock, NULL);

    // Initialise task to delete
    sem_init(&task_to_delete.empty, 0, MAX_TASK_TO_DELETE);
    sem_init(&task_to_delete.full, 0, 0);

    // Measure the execution time of each task
    measure_time(tasks, TASKS_NUMBER);

    // DEBUG - Print execution time and CPU usage of each task
    for (int i = 0; i < TASKS_NUMBER; i++) {
        printf("Task %d - %lf - usage: %lf\n", i, tasks[i].execution_time, tasks[i].cpu_usage);
    }

    // Initialize the TCP server
    init_tcp_server(tcp_server_callback, PORT);

    // Deinit all tasks and destroy lokcs, and semaphore
    deinit_tasks(&scheduler_info);
    pthread_mutex_destroy(&scheduler_info.lock);
    pthread_mutex_destroy(&task_to_delete.lock);
    sem_destroy(&task_to_delete.empty);
    sem_destroy(&task_to_delete.full);

    return 0;
}
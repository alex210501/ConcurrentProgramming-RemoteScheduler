#include "common/defs.h"
#include "common/task.h"
#include "tcp/tcp-server.h"

#define TASKS_NUMBER (4)

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

void tcp_server_callback(int connfd) {
    for (;;) {
        Request_t req;
        Answer_t ans = { .error = TASK_DOES_NOT_EXIST };

        // Read data received by the client
        read(connfd, (void*)&req, sizeof(Request_t));

        // TODO - Debug print
        printf("Data received: %d - %d\n", req.action, req.task);

        write(connfd, (void*)&ans, sizeof(Answer_t));
    }
}

int main() {
    measure_time(tasks, TASKS_NUMBER);

    for (int i = 0; i < TASKS_NUMBER; i++) {
        printf("Task %d - %lld - usage: %lf\n", i, tasks[i].time, tasks[i].cpu_usage);
    }

    init_tcp_server(tcp_server_callback, PORT);

    return 0;
}
#include "common/defs.h"
#include "common/task.h"
#include "tcp/tcp-server.h"

CREATE_TASK(100);
CREATE_TASK(200);
CREATE_TASK(300);

task_callback_t tasks[] = {
    &task_0,
    &task_1,
    &task_2,
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
    init_tcp_server(tcp_server_callback, PORT);

    return 0;
}
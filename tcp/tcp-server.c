#include "tcp-server.h"

#define MAX_BUF_SIZE (100)

static void default_callback(int connfd) {
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
    printf("Starting demo for TCP server...\n");
    init_tcp_server(default_callback, 8080);

    return 0;
}
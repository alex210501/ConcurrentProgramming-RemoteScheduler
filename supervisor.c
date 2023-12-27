#include "common/defs.h"
#include "tcp/tcp-server.h"

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
#include "tcp-client.h"

#define MAX_BUF_SIZE (100)

static void default_callback(int sockfd) {
    for (int i = 0; i < 10; i++) {
        Request_t req = { .action = DEACTIVATION, .task = 10 };
        Answer_t ans;

        write(sockfd, (void*)&req, sizeof(Request_t));
        read(sockfd, (void*)&ans, sizeof(Answer_t));
        printf("Received from server : %d\n", ans.error);
    }
}

int main() {
    init_tcp_client(default_callback, 8080);
}
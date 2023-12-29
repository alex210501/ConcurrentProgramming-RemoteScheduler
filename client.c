#include "common/defs.h"
#include "tcp/tcp-defs.h"
#include "tcp/tcp-client.h"

Actions_t actions[] = {
    ACTIVATION, DEACTIVATION, EXIT, SHOW_STATUS,
};

void tcp_client_callback(int sockfd) {
    for (;;) {
        int action, task;

        // Get action
        printf("\nChoose an action:\n");
        for (int i = 0; i < (sizeof(actions) / sizeof(Actions_t)); i++) {
            char dest[15];
            actionToString(actions[i], dest, sizeof(dest));
            printf("[%d] - %s\n", i, dest);
        }

        printf("Choose an action - ");
        scanf("%d", &action);

        if (!ACTION_VALID(action)) {
            printf("Action %d is not valid!\n", action);
            continue;
        }

        if (RELY_ON_TASK(action)) {
            printf("Choose an task - ");
            scanf("%d", &task);
        }

        Request_t req = { .action = (Actions_t)action, .task = task };
        Answer_t ans;

        write(sockfd, (void*)&req, sizeof(Request_t));
        read(sockfd, (void*)&ans, sizeof(Answer_t));

        // Print the return status
        char result[25];
        errorToString(ans.error, result, sizeof(result));
        printf("Received from server : %s\n", result);

        // If asked to exit, close the client
        if (req.action == EXIT) break;
    }
}

int main() {
    init_tcp_client(tcp_client_callback, PORT);

    return 0;
}
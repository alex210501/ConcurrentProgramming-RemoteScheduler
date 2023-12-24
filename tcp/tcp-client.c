#include "tcp-client.h"

#define MAX_BUF_SIZE (100)

static void default_callback(int sockfd) {
    char buf[MAX_BUF_SIZE];
    int n;

    for (int i = 0; i < 10; i++) {
        snprintf(buf, MAX_BUF_SIZE, "Hello ");

        write(sockfd, buf, strlen(buf));
        memset(buf, 0, MAX_BUF_SIZE);
        read(sockfd, buf, MAX_BUF_SIZE);
        printf("Received from server : %s\n", buf);
    }
}

void init_tcp_client(tcp_client_callback_t callback, int port) {
    int sockfd, connfd;
    sockaddr_in_t servaddr, cli;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("Socket creation failed...\n");
        exit(0);
    }

    printf("Socket successfully created..\n");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(port);

    if (connect(sockfd, (sockaddr_t*)&servaddr, sizeof(servaddr)) != 0) {
        printf("Connection with the server failed...\n");
        exit(0);
    }

    printf("Connected to the server..\n");
    
    // Handler to manage the server communication
    callback(sockfd);

    // close the socket
    close(sockfd);
}

int main() {
    init_tcp_client(default_callback, 8080);
}
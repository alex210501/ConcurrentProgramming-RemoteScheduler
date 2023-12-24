#include "tcp-defs.h"

void init_tcp_server(int port)  {
    int sockfd, connfd;
    socklen_t len;
    sockaddr_in_t servaddr, cli;
    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("Socket creation failed!\n");
        exit(0);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(port); 

    printf("Socket created on port %d!\n", port);

    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (sockaddr_t*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("Socket bind failed...\n"); 
        exit(0); 
    }

    printf("Socket successfully binded..\n"); 

    // Now server is ready to listen and verification
    // TODO - Put 5 in define
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 

    printf("Server listening..\n"); 

    if ((connfd = accept(sockfd, (sockaddr_t*)&cli, &len)) < 0) { 
        printf("server accept failed...\n"); 
        exit(0); 
    }

    // After chatting close the socket 
    close(sockfd); 
}

int main() {
    printf("Starting demo for TCP server...\n");
    init_tcp_server(8080);

    return 0;
}
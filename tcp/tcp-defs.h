#ifndef __TCP_DEFS_H__
#define __TCP_DEFS_H__

#include <arpa/inet.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h>

#include "../common/defs.h"

typedef struct sockaddr sockaddr_t;
typedef struct sockaddr_in sockaddr_in_t;
typedef void (*tcp_server_callback_t)(int);
typedef void (*tcp_client_callback_t)(int);

typedef union {
    struct {
        Actions_t action:3;
        int task:5;
    };
    uint8_t frame;
} Request_t;

typedef union {
    struct {
        Error_t error;
    };
    uint8_t frame;
} Answer_t;

#endif /* __TCP_DEFS_H__ */
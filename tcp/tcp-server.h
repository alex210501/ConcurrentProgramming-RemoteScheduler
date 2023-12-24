#ifndef  __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include "tcp-defs.h"

void init_tcp_server(tcp_server_callback_t callback, int port);

#endif /* __TCP_SERVER_H__ */
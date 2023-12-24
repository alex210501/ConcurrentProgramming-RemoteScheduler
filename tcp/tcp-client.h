#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include "tcp-defs.h"

void init_tcp_client(tcp_client_callback_t callback, int port);

#endif /* __TCP_CLIENT_H__ */
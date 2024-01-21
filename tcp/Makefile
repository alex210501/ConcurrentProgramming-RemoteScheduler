CC=gcc
CFLAGS=-I.
SERVER_OBJ = tcp-server.o
CLIENT_OBJ = tcp-client.o
DEPS = tcp-defs.h ../common/defs.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

server: $(SERVER_OBJ)
	$(CC) -o tcp-server $^ $(CFLAGS)

client: $(CLIENT_OBJ)
	$(CC) -o tcp-client $^ $(CFLAGS)
	
clean:
	rm *.o
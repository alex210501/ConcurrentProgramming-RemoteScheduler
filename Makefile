CC=gcc
CFLAGS=-I.
CLIENT_OBJ = client.o 
DEPS = ./common/defs.h ./tcp/tcp-defs.h ./tcp/tcp-client.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

client: $(CLIENT_OBJ)
	$(CC) -o client $^ $(CFLAGS)

all: $(modules)
	$(CC) -o program $^ $(CFLAGS)

clean:
	rm *.o
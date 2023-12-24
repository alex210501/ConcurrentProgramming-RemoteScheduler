CC=gcc
CFLAGS=-I.
OBJ = tcp-server.o
DEPS = tcp-defs.h ../common/defs.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

server: $(OBJ)
	$(CC) -o tcp-server $^ $(CFLAGS)

clean:
	rm *.o
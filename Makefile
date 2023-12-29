CC=gcc
CFLAGS=-I.
CLIENT_OBJ = client.o 
SUPERVISOR_OBJ = supervisor.o
DEPS = ./common/defs.h ./common/task.h ./common/rounded_queue.h ./tcp/tcp-defs.h ./tcp/tcp-client.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

client: $(CLIENT_OBJ)
	$(CC) -o client $^ $(CFLAGS)

supervisor: $(SUPERVISOR_OBJ)
	$(CC) -o supervisor $^ $(CFLAGS)	

clean:
	rm *.o
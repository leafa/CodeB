OBJS = client.o actions.o
CC = g++
DEBUG = -g
CFLAGS = -c $(DEBUG)
LFLAGS = $(DEBUG)

client : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o client

client.o : 
	$(CC) $(CFLAGS) client.cpp

actions.o : 
	$(CC) $(CFLAGS) actions.cpp

clean:
	\rm *.o client

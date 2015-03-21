OBJS = client.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

client : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o client

client.o : 
	$(CC) $(CFLAGS) client.cpp

clean:
	\rm *.o client

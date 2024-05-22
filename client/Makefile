# Makefile for client.c
# @SebastianFrazier26

L = ../libcs50


OBJS = client.o ../support/message.o ../support/log.o
LLIBS = $L/libcs50-given.a
LIBS = -lm


# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

client: $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o $@

test: 
	./client plank 54812

client.o: client.h
../support/message.o: ../support/message.h
../support/log.o: ../support/log.h

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core

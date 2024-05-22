# Makefile for 'bag' module
#
# David Kotz, April 2016, 2017, 2019, 2021
# updated by Xia Zhou, July 2016, 2018, January 2019

L = ../libcs50


OBJS = server.o
LLIBS = ../libcs50/libcs50-given.a ../support/support.a ../map/map.a 
LIBS = -lm


# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L
CC = gcc
MAKE = make

# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

server: $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o $@


server.o: server.h ../libcs50/file.h ../libcs50/hashtable.h 

test: server
	./server ../maps/small.txt

valgrind: server
	$(VALGRIND) ./server ../maps/small.txt


.PHONY: test valgrind clean

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f hashtest
	rm -f core
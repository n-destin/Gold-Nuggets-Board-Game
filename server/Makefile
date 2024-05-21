# Makefile for 'bag' module
#
# David Kotz, April 2016, 2017, 2019, 2021
# updated by Xia Zhou, July 2016, 2018, January 2019

L = ../libcs50


OBJS = server.o
LLIBS = $L/libcs50-given.a ../map/map.a
LIBS = -lm


# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L
CC = gcc
MAKE = make
LIB = map.a

# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

server: $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o $@


server.o: server.h

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
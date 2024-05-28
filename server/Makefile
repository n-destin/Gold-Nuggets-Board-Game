OBJS = server.o
LLIBS = ../support/support.a ../map/map.a 
LIBS = -lm -L../libcs50 -lcs50-given

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I. -I../libcs50
CC = gcc
MAKE = make

# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

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
		rm -f *~ *.o *.d *.ds_store
		rm -f hashtest
		rm -f core

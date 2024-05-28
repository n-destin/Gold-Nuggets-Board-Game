# Makefile for client.c
# @SebastianFrazier26/Izzy Axinn

SUPPORT_DIR = ../support
L = $(SUPPORT_DIR)/support.a
OBJS = client.o
LIBS = -lm -lcurses $(L)

# uncomment the following to turn on verbose memory logging
# TESTING = -DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$(SUPPORT_DIR)
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

client: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $@

client.o: client.h

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core

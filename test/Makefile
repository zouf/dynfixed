
CC=gcc
CFLAGS=-Wall -g -O3   -I../src/

all: test.exe 

test.exe: test.o	
	$(CC) test.o -o $@ -L.. -ldynfixed -lm

test.o:	test.c
	$(CC) -c $^ $(CFLAGS) -o $@

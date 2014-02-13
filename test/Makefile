
SOURCE=dynfixed.c 
CC=gcc#$(CS_BASE)/llvm-install/bin/clang
CFLAGS=-Wall -g -O3   -I../src/

all:  test.exe pytest.exe

test.exe: test.o	
	$(CC) test.o -o $@ ../libdynfixed.a -lm

test.o:	test.c
	$(CC) -c $^ $(CFLAGS) -o $@

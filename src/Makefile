
SOURCE=dynfixed.c 
CC=gcc#$(CS_BASE)/llvm-install/bin/clang

PY_CFLAGS=-I/usr/include/python2.7 -fno-strict-aliasing -DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes
CFLAGS=$(PY_CFLAGS) -fPIC 

PY_LFLAGS=-L/usr/lib/python2.7/config -lpthread -ldl -lutil -lm -lpython2.7 
LFLAGS=-shared -Wl,-soname,libdynfixedpoint.so -lelf -lrt -lpthread -Wall -fPIC  $(PY_LFLAGS)

all: libdynfixed.so pytest.exe

pytest.exe: pytest.o
	$(CC)  -o $@ $^ $(PY_LFLAGS)

pytest.o:	pytest.c
	$(CC) $(PY_CFLAGS) -c $^ 

clean:
	rm -f *.a *.o *.exe *.so *.pyc ../libdynfixed.so

libdynfixed.so: dynfixed.o
	g++ -fPIC $(LFLAGS) -o $@ $^
	ln -s --force $(PWD)/$@ ../

dynfixed.o: $(SOURCE)
	$(CC) $(CFLAGS) -c  $^ -o $@ 

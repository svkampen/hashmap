CC=gcc
CFLAGS=-std=gnu11 -O2 -I../include -ggdb -g3
SHARED_CFLAGS=-shared -fpic -o ../bin/libhashmap.so
SHARED_FILES=hash.c list.c hashfn.c
STATIC_CFLAGS=-c

static:
	mkdir -p bin
	cd src; $(CC) $(CFLAGS) $(STATIC_CFLAGS) $(SHARED_FILES); ar rcs ../bin/libhashmap.a *.o
	rm src/*.o

shared:
	mkdir -p bin
	cd src; $(CC) $(CFLAGS) $(SHARED_CFLAGS) $(SHARED_FILES)

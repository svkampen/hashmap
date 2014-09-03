CC=gcc
CFLAGS=-std=c99 -O2 -I../include
SHARED_CFLAGS=-shared -fpic -o ../bin/libhashmap.so
SHARED_FILES=hash.c list.c hashfn.c
STATIC_CFLAGS=-c

static:
	mkdir -p bin
	cd src; $(CC) $(CFLAGS) $(STATIC_CFLAGS) $(SHARED_FILES); ar rcs ../bin/libhashmap.a *.o

shared:
	mkdir -p bin
	cd src; $(CC) $(CFLAGS) $(SHARED_CFLAGS) $(SHARED_FILES)

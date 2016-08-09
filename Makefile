EMACS_ROOT ?= ../..
EMACS ?= emacs

CC      = gcc
LD      = gcc
CPPFLAGS = -I$(EMACS_ROOT)/src -I.
CFLAGS = -std=gnu99 -ggdb3 -Wall -fPIC $(CPPFLAGS)

.PHONY : test

all: mt-rand-core.so

mt-rand-core.so: mt-rand-core.o mt19937ar.o
	$(LD) -shared $(LDFLAGS) -o $@ $^

mt-rand-core.o: mt-rand-core.c
	$(CC) $(CFLAGS) -c -o $@ $<

mt19937ar.o: mt19937ar.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm -f mt-rand-core.so mt-rand-core.o mt19937ar.o

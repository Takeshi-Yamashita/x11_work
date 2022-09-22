# Makefile for test
CC = gcc
CFLAGS = -O2 -I/usr/include
LIB = -L/usr/lib/x86_64-linux-gnu -lX11 -lm
TARGETS = xlib
SRC = $(TARGETS).c

$(TARGETS): $(SRC)
	$(CC) -o $@ $^ $(CFLAGS) $(LIB)

clean: 
	rm -f *.o $(TARGETS)


CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -Wno-unused-parameter
LDFLAGS = -lncurses

all: pixel

pixel: pixel.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<


CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -Wno-unused-parameter
LDFLAGS = -lncurses

check: Dockerfile all
	docker build -t pixel .
	docker run -it pixel

all: pixel

pixel: pixel.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f pixel *.o

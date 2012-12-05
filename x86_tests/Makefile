CC      = gcc
CFLAGS  = -g -O2 -Wall -Werror

objects = bubblesort.o

default: bubblesort

.PHONY: default clean clobber

bubblesort: $(objects)
	$(CC) -g -o $@ $^

bubblesort.o: bubblesort.c bubblesort.h config.h types.h

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(objects)

clobber: clean
	rm -f bubblesort

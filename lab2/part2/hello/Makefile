CC      = arm-linux-gcc
OBJCOPY = arm-linux-objcopy
CFLAGS  = -O2 -ffreestanding -Wall -Werror -nostdinc \
          -I ../uboot/include -I ../../part1/libc/include
LDFLAGS = -nostdlib -Wl,-Ttext,$(LOAD_ADDR)

LOAD_ADDR    = 0xa2000000

STUBS = ../uboot/stubs.o
CRT0    = ../../part1/libc/crt0.o
LDLIBS  = ../../part1/libc/libc.a

objects = hello.o

default: hello.bin

.PHONY: default clean clobber

hello.bin: hello
	$(OBJCOPY) -O binary $< $@

hello: $(objects)
	$(CC) $(LDFLAGS) -o $@ $(CRT0) $^ $(STUBS) $(LDLIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f hello $(objects)

clobber: clean
	rm -f hello.bin

CC      = arm-linux-gcc
OBJCOPY = arm-linux-objcopy
CFLAGS  = -O2 -ffixed-r8 -ffreestanding -Wall -Werror -nostdinc \
          -I ../uboot/include -I include
ASFLAGS = -nostdinc -I ../uboot/include -I include
LDFLAGS = -nostdlib -Wl,-N,-Ttext,$(LOAD_ADDR)

LOAD_ADDR = 0xa3000000

STUBS = ../uboot/stubs.o

objects = kernel.o C_SWI_handler.o swi_handler.o

default: kernel.bin

.PHONY: default clean clobber

kernel.bin: kernel
	$(OBJCOPY) -O binary $< $@

kernel: start.o $(objects)
	$(CC) $(LDFLAGS) -o $@ $^ $(STUBS)

kernel.o: kernel.c
C_SWI_handler.o: C_SWI_handler.c C_SWI_handler.h
swi_handler.o: swi_handler.S

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.o: %.S
	$(CC) -c $(ASFLAGS) -o $@ $<

clean:
	rm -f kernel start.o $(objects)

clobber: clean
	rm -f kernel.bin

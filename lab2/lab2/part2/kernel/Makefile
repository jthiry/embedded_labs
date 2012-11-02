CC      = arm-linux-gcc
OBJCOPY = arm-linux-objcopy
CFLAGS  = -O2 -ffixed-r8 -ffreestanding -Wall -Werror -nostdinc \
          -I ../uboot/include -I include
ASFLAGS = -nostdinc -I ../uboot/include -I include
LDFLAGS = -nostdlib -Wl,-N,-Ttext,$(LOAD_ADDR)

LOAD_ADDR = 0xa3000000

STUBS = ../uboot/stubs.o

objects = kernel.o c_swi_handler.o swi_handler.o exit.o kernel_util.o

default: kernel.bin

.PHONY: default clean clobber dump

kernel.bin: kernel
	$(OBJCOPY) -O binary $< $@

kernel: start.o $(objects)
	$(CC) $(LDFLAGS) -o $@ $^ $(STUBS)

kernel.o: kernel.c
c_swi_handler.o: c_swi_handler.c c_swi_handler.h
swi_handler.o: swi_handler.S swi_handler.h
exit.o: exit.S exit.h
kernel_util.o: kernel_util.S kernel_util.h


%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.o: %.S
	$(CC) -c $(ASFLAGS) -o $@ $<

clean:
	rm -f kernel start.o $(objects)

clobber: clean
	rm -f kernel.bin
dump:
	arm-linux-objdump -D kernel > d.txt

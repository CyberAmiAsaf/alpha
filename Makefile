C_SOURCES = $(wildcard kernel/*.c cpu/**/*.c drivers/**/*.c lib/**/*.c)
C_HEADERS = $(wildcard kernel/*.h cpu/**/*.h drivers/**/*.h lib/**/*.h)
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o}

ASM = /usr/bin/nasm
CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
LD = /usr/local/i386elfgcc/bin/i386-elf-ld
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb

INCLUDE_PATHS = -I $(shell pwd)/kernel/ -I $(shell pwd)/cpu/ -I $(shell pwd)/drivers/ -I $(shell pwd)/lib/

ASM_BOOT_FLAGS = -f bin
ASM_KERNEL_FLAGS = -f elf
CC_FLAGS = $(INCLUDE_PATHS) -ffreestanding -c -g -m32 # -nostdinc # -fno-builtin -fno-stack-protector # -nostartfiles -Wall -Wextra -Werror # -nodefaultlibs -nostdlib

$(info $(C_SOURCES))

all: run

# Build kernel
bin/kernel.bin: kernel/kernel_entry.o ${OBJ}
	${LD} -o $@ -Ttext 0x1000 $^ --oformat binary

kernel/kernel_entry.o: kernel/kernel_entry.asm
	${ASM} -f elf $< -o $@

%.o: %.asm
	${ASM} -f elf $< -o $@

%.o: %.c ${HEADERS}
	${CC} ${CC_FLAGS} $< -o $@

# Build bootsector
bin/bootsect.bin: boot/bootsect.asm
	${ASM} -f bin $< -o $@

# Concat bootsector + kernel
bin/alpha.bin: bin/bootsect.bin bin/kernel.bin
	cat $^ > bin/alpha.bin

# hard disk image
bin/hard_disk.img:
	qemu-img create -f raw -o size=512M bin/hard_disk.img

# Run qEmulator
run: bin/alpha.bin bin/hard_disk.img
	qemu-system-i386 -fda $< -boot a -m 2G -drive file=bin/hard_disk.img,index=1,media=disk,format=raw

bin/kernel.elf: kernel/kernel_entry.o ${OBJ}
	${LD} -o $@ -Ttext 0x1000 $^

debug: bin/alpha.bin bin/kernel.elf
	qemu-system-i386 -s -fda bin/alpha.bin  &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file bin/kernel.elf"

clean:
	rm -rf $(wildcard bin/* kernel/*.o lib/**/*.o drivers/**/*.o)

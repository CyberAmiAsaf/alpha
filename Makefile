C_SOURCES = $(wildcard kernel/*.c drivers/**/*.c lib/**/*.c)
C_HEADERS = $(wildcard kernel/*.h drivers/**/*.h lib/**/*.h)
OBJ = ${C_SOURCES:.c=.o}

ASM = /usr/bin/nasm
CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
LD = /usr/local/i386elfgcc/bin/i386-elf-ld

ASM_BOOT_FLAGS = -f bin
ASM_KERNEL_FLAGS = -f elf
CC_FLAGS = -ffreestanding -c -g

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

# Run qEmulator
run: bin/alpha.bin
	qemu-system-i386 -drive file=$<,format=raw

clean:
	rm -rf $(wildcard bin/*.bin kernel/*.o lib/**/*.o drivers/**/*.o)
pwd := $(shell pwd)
export PATH := /usr/local/i386elfgcc/bin:$(PATH)
SHELL := /bin/bash

# Run qEMU
all: run

# Build kernel
bin/kernel.bin: obj/kernel_entry.o obj/kernel.o
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

obj/kernel_entry.o: kernel/kernel_entry.asm
	nasm $< -f elf -o $@

obj/kernel.o: kernel/kernel.c
	i386-elf-gcc -ffreestanding -c $< -o $@

# Build bootsector
bin/bootsect.bin: boot/bootsect.asm
	nasm $< -f bin -o $@

# Concat bootsector + kernel
bin/alpha.bin: bin/bootsect.bin bin/kernel.bin
	cat $^ > bin/alpha.bin

# Run qEmu
run: bin/alpha.bin
	qemu-system-i386 -fda $<

build-docker-dev:
	docker build . -t "alpha-dev"

run-docker-dev:
	docker run -v $(pwd):/root/alpha -i -t "alpha-dev" /bin/bash

print-path:
	@echo $$PATH

clean:
	rm bin/*.bin obj/*.o dis/*.dis

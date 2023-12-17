C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.h)

OBJ = ${C_SOURCES:.c=.o}

all: os-image

run: all
	/mnt/c/Program\ Files/Bochs-2.7/bochs.exe -q -f bochsrc.bxrc

os-image: boot/boot_sect.bin kernel.bin
	cat $^ > os-image

%.bin: %.asm
	nasm $< -f bin -I './boot' -o $@

# Build the kernel binary
kernel.bin: kernel/kernel_entry.o ${OBJ}
	ld \
		-m elf_i386 \
		-o kernel.bin \
		-Ttext 0x1000 \
		-static \
		$^ \
		--oformat binary

# Build the kernel entry object file .
%.o: %.asm
	nasm $< -f elf -o $@

# Generic rule for building 'somefile .o' from 'somefile .c'
# -m32: compile for 32-bit
# -ffreestanding: Don't include the standard library.
# -fno-pic: Don't use position-independent addressing.
# -c: compile to object code.
%.o : %.c ${HEADERS}
	gcc \
		-x c \
		-m32 \
		-march=x86-64 \
		-ffreestanding \
		-nostdlib \
		-nostartfiles \
		-fno-pic \
		-fno-builtin \
		-O0 \
		-Wall \
		-Wextra \
		-Werror \
		-c $< \
		-o $@


floppy.img: os-image
	dd if=/dev/zero of=$@ bs=1024 count=1440
	dd if=os-image of=$@ conv=notrunc

deploy-floppy: floppy.img
	cp $< /mnt/c/Users/vcana/github/vcanaa/vinix

.PHONY: clean
clean:
	rm -fr *.bin *.o os-image floppy.img
	rm -fr kernel/*.o boot/*.bin drivers/*.o
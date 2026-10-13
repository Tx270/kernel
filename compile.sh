set -e

CFLAGS=(
    -m32
    -march=i386
    -ffreestanding
    -fno-pie
    -fno-pic
    -fno-stack-protector
    -fno-asynchronous-unwind-tables
    -fno-unwind-tables
    -mno-sse
    -mno-mmx
    -O2
    -Wall
    -Wextra
    -Ikernel/include
)

nasm -f bin boot.asm -o boot.bin
nasm -f elf32 kernel_entry.asm -o kernel_entry.o

gcc "${CFLAGS[@]}" -c kernel/kernel.c -o kernel.o
gcc "${CFLAGS[@]}" -c kernel/drivers/terminal.c -o terminal.o
gcc "${CFLAGS[@]}" -c kernel/drivers/keyboard.c -o keyboard.o
gcc "${CFLAGS[@]}" -c kernel/lib/string.c -o string.o

ld -m elf_i386 \
    -T linker.ld \
    -o kernel.elf \
    kernel_entry.o \
    kernel.o \
    terminal.o \
    keyboard.o \
    string.o

objcopy -O binary kernel.elf kernel.bin

test "$(stat -c %s boot.bin)" -eq 512
test "$(stat -c %s kernel.bin)" -le 16384

truncate -s 1440K system.flp
dd if=boot.bin of=system.flp conv=notrunc status=none
dd if=kernel.bin of=system.flp bs=512 seek=1 conv=notrunc status=none

qemu-system-i386 \
    -drive file=system.flp,format=raw,if=floppy \
    -boot a \
    -display gtk
cmake_minimum_required(VERSION 3.20)

project(MyOS LANGUAGES C ASM_NASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32)
set(CMAKE_ASM_NASM_FLAGS "-f elf32")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

set(KERNEL_CFLAGS
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
)

include_directories(kernel/include)

set(KERNEL_SOURCES
    kernel/kernel.c
    kernel/drivers/terminal.c
    kernel/drivers/keyboard.c
    kernel/lib/string.c
    kernel/shell/shell.c
    kernel_entry.asm
)

add_executable(kernel_elf ${KERNEL_SOURCES})

set_target_properties(kernel_elf PROPERTIES
    OUTPUT_NAME "kernel"
    SUFFIX ".elf"
)

target_compile_options(kernel_elf PRIVATE
    $<$<COMPILE_LANGUAGE:C>:${KERNEL_CFLAGS}>
)

target_link_options(kernel_elf PRIVATE
    -m32
    -nostdlib
    -no-pie
    -Wl,-m,elf_i386
    "-Wl,-T,${CMAKE_SOURCE_DIR}/linker.ld"
    -Wl,--build-id=none
)

set(BOOT_BIN "${CMAKE_BINARY_DIR}/boot.bin")
set(KERNEL_BIN "${CMAKE_BINARY_DIR}/kernel.bin")
set(FLOPPY_IMG "${CMAKE_BINARY_DIR}/system.flp")

add_custom_command(
    OUTPUT "${BOOT_BIN}"
    COMMAND nasm -f bin
            "${CMAKE_SOURCE_DIR}/boot.asm"
            -o "${BOOT_BIN}"
    DEPENDS "${CMAKE_SOURCE_DIR}/boot.asm"
    VERBATIM
)

add_custom_command(
    OUTPUT "${KERNEL_BIN}"
    COMMAND objcopy -O binary
            "$<TARGET_FILE:kernel_elf>"
            "${KERNEL_BIN}"
    DEPENDS kernel_elf
    VERBATIM
)

add_custom_target(kernel_binary DEPENDS "${KERNEL_BIN}")

add_custom_command(
    OUTPUT "${FLOPPY_IMG}"
    COMMAND bash
            "${CMAKE_SOURCE_DIR}/make_image.sh"
            "${BOOT_BIN}"
            "${KERNEL_BIN}"
            "${FLOPPY_IMG}"
    DEPENDS
        "${BOOT_BIN}"
        "${KERNEL_BIN}"
        "${CMAKE_SOURCE_DIR}/make_image.sh"
    VERBATIM
)

add_custom_target(image ALL DEPENDS "${FLOPPY_IMG}")

add_custom_target(run
    COMMAND qemu-system-i386
            -drive "file=${FLOPPY_IMG},format=raw,if=floppy"
            -boot a
            -display gtk
    DEPENDS image
    USES_TERMINAL
)
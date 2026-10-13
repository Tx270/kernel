
BITS 16
ORG 0x7C00

start:
    jmp 0x0000:main

main:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    mov [boot_drive], dl

    mov ax, 0x1000
    mov es, ax
    xor bx, bx

    mov byte [current_sector], 2
    mov byte [current_head], 0
    mov byte [current_cylinder], 0
    mov byte [sectors_left], 32

.read_sector:
    mov ah, 0x02
    mov al, 1
    mov ch, [current_cylinder]
    mov cl, [current_sector]
    mov dh, [current_head]
    mov dl, [boot_drive]

    push bx
    int 0x13
    pop bx
    jc disk_error

    add bx, 512

    inc byte [current_sector]
    cmp byte [current_sector], 19
    jb .next

    mov byte [current_sector], 1
    inc byte [current_head]
    cmp byte [current_head], 2
    jb .next

    mov byte [current_head], 0
    inc byte [current_cylinder]

.next:
    dec byte [sectors_left]
    jnz .read_sector

    cli
    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp dword 0x08:protected_mode

disk_error:
    mov si, error_message
    mov ah, 0x0E
    mov bx, 0x0007

.print_error:
    lodsb
    test al, al
    jz .halt
    int 0x10
    jmp .print_error

.halt:
    cli
    hlt
    jmp .halt

boot_drive db 0
current_sector db 2
current_head db 0
current_cylinder db 0
sectors_left db 32
error_message db 'Disk read error!', 0

align 8
gdt_start:
    dq 0

gdt_code:
    dw 0xFFFF
    dw 0
    db 0
    db 10011010b
    db 11001111b
    db 0

gdt_data:
    dw 0xFFFF
    dw 0
    db 0
    db 10010010b
    db 11001111b
    db 0

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

BITS 32

protected_mode:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    jmp 0x08:0x10000

BITS 16

times 510 - ($ - $$) db 0
dw 0xAA55

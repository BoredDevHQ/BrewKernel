global long_mode_start
extern kernel_main
extern keyboard_handler

section .text
bits 64

long_mode_start:

    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call kernel_main
    hlt

keyboard_isr:
   
    call keyboard_handler
 
    mov al, 0x20
    out 0x20, al
    iretq

;
; Brew Kernel
; Copyright (C) 2024-2025 boreddevhq
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program. If not, see <https://www.gnu.org/licenses/>.
;

;
; Interrupt Descriptor Table (IDT)
; This file implements the x86_64 interrupt handling infrastructure
; It sets up the IDT and provides basic interrupt service routines (ISRs)
;

bits 64

; Constants for IDT structure
%define IDT_ENTRIES 256        ; Total number of possible interrupt vectors
%define IDT_ENTRY_SIZE 16      ; Size of each IDT entry in bytes

section .data
    ; IDT pointer structure (used by LIDT instruction)
    idt_ptr:
        dq IDT_ENTRIES * IDT_ENTRY_SIZE - 1  ; Size of IDT - 1
        dq idt                               ; Base address of IDT

section .bss
    ; Reserve space for the IDT
    idt resb IDT_ENTRIES * IDT_ENTRY_SIZE    ; Uninitialized IDT array

section .text
global load_idt

; Function: load_idt
; Loads the IDT pointer into the CPU's IDTR register
load_idt:
    lidt [idt_ptr]                 ; Load IDT pointer
    ret

; Function: set_idt_entry
; Sets up an IDT entry
; Input:
;   RAX = Address of ISR
;   RBX = Interrupt vector number
;   RCX = Segment selector
set_idt_entry:
    mov rdx, rax                           ; Save ISR address
    
    shl rbx, 4                             ; Multiply vector by 16 (entry size)
    lea rsi, [idt + rbx]                   ; Calculate entry address
    
    mov [rsi], rdx                         ; Store low 16 bits of ISR address
    shr rdx, 16                            ; Get high bits
    mov [rsi + 8], rdx                     ; Store high bits of ISR address
    mov [rsi + 2], rcx                     ; Set segment selector
    mov byte [rsi + 5], 0x8E               ; P=1, DPL=0, Type=0xE (64-bit interrupt gate)
    ret

section .text

; Interrupt Service Routines

; ISR 0: Division by Zero Exception Handler
isr_divide_by_zero:
    cli                                    ; Disable interrupts
    hlt                                    ; Halt the CPU
    iretq                                  ; Return from interrupt (64-bit)

; ISR 1: Debug Exception Handler
isr_debug:
    cli                                    ; Disable interrupts
    hlt                                    ; Halt the CPU
    iretq                                  ; Return from interrupt

; ISR 14: Page Fault Exception Handler
isr_page_fault:
    cli                                    ; Disable interrupts
    hlt                                    ; Halt the CPU
    iretq                                  ; Return from interrupt

; Function: init_idt
; Initializes the IDT with basic exception handlers
init_idt:
    ; Set up Division by Zero handler (Vector 0)
    mov rax, isr_divide_by_zero
    mov rcx, 0                             ; Vector number
    call set_idt_entry

    ; Set up Debug Exception handler (Vector 1)
    mov rax, isr_debug
    mov rcx, 1                             ; Vector number
    call set_idt_entry

    ; Set up Page Fault handler (Vector 14)
    mov rax, isr_page_fault
    mov rcx, 14                            ; Vector number
    call set_idt_entry

    call load_idt                          ; Load the IDT
    ret

section .text
global main

; Main entry point
main:
    call init_idt                          ; Initialize interrupt handling

    cli                                    ; Disable interrupts
    hlt                                    ; Halt the CPU

section .data
idt_end:
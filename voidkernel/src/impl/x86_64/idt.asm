bits 64

%define IDT_ENTRIES 256
%define IDT_ENTRY_SIZE 16

section .data
    idt_ptr:
        dq IDT_ENTRIES * IDT_ENTRY_SIZE - 1 
        dq idt                                

section .bss
    idt resb IDT_ENTRIES * IDT_ENTRY_SIZE     

section .text
global load_idt


load_idt:
    lidt [idt_ptr]                 
    ret


set_idt_entry:
    mov rdx, rax                           
    
    shl rbx, 4                             
    lea rsi, [idt + rbx]                  
    
    mov [rsi], rdx                       
    shr rdx, 16                        
    mov [rsi + 8], rdx               
    mov [rsi + 2], rcx                 
    mov byte [rsi + 5], 0x8E              
    ret

section .text

isr_divide_by_zero:
    cli                                    
    hlt                                 
    iretq                                    

isr_debug:
    cli                                  
    hlt                                      
    iretq                                   

isr_page_fault:
    cli                                       
    hlt                                        
    iretq                                   

init_idt:
    mov rax, isr_divide_by_zero
    mov rcx, 0                            
    call set_idt_entry

    mov rax, isr_debug
    mov rcx, 1                        
    call set_idt_entry

    mov rax, isr_page_fault
    mov rcx, 14                                
    call set_idt_entry

    call load_idt
    ret

section .text
global main

main:
    call init_idt                        

    cli                                       
    hlt                                       

section .data
idt_end:                              
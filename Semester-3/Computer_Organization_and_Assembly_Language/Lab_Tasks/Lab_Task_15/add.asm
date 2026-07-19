; add.asm (64-bit)
global add

section .text
add:
    mov rax, rcx     ; rax = first parameter
    add rax, rdx     ; rax = rax + second parameter
    ret

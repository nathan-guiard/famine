bits 64

section .text
	global	_start

_start:
	push	rdx
	push	rcx
	push	rbx
	push	rax

	mov rax, 1
	mov rdi, 1
	mov rsi, rsp
	mov rdx, 32
	syscall

	mov rax, 60
	mov rsi, 0
	syscall


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_macros.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 13:38:07 by nguiard           #+#    #+#             */
/*   Updated: 2024/11/06 12:55:26 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASM_MACROS_H
#define ASM_MACROS_H

//	Syscall numbers
#define SYS_READ		0
#define SYS_WRITE		1
#define SYS_OPEN		2
#define SYS_CLOSE		3
#define SYS_FSTAT		5
#define SYS_MUNMAP		11
#define SYS_MREMAP		25
#define SYS_EXIT		60
#define SYS_FTRUNCATE	77
#define SYS_GETDENTS	78
#define SYS_PTRACE		101

#define get_rip(instruction_pointer)	\
	asm(								\
		".intel_syntax noprefix\n"		\
		"lea rsi, [rip]\n"				\
		"mov %0, rsi\n"					\
		".att_syntax prefix\n"			\
		:	"=r" (instruction_pointer)	\
		:								\
		: "rax", "rdi", "rsi", "rdx"	\
	);
//	Raw syscalls
#define sys1_noret(sysnum, first)	\
	asm(							\
		".intel_syntax noprefix\n"	\
		"mov rdi, %[a]\n"			\
		"mov rax, %[num]\n"			\
		"syscall\n"					\
		".att_syntax prefix\n"		\
		:							\
		: [a] "r" (first), 			\
			[num] "r" (sysnum)		\
		: "rdi", "rax"				\
	)

#define sys2(ret, sysnum, first, second) \
	asm(										\
		".intel_syntax noprefix\n"				\
		"mov rsi, %[b]\n"						\
		"mov rdi, %[a]\n"						\
		"mov rax, %[num]\n"						\
		"syscall\n"								\
		"mov %0, rax\n"							\
		".att_syntax prefix\n"					\
		: "=r" (ret)							\
		: [a] "r" (first),						\
			[b] "r" (second),					\
			[num] "r" (sysnum)					\
		: "rdi", "rsi", "rax"					\
	)

#define sys3(ret, sysnum, first, second, third) \
	asm(										\
		".intel_syntax noprefix\n"				\
		"mov rdx, %[c]\n"						\
		"mov rsi, %[b]\n"						\
		"mov rdi, %[a]\n"						\
		"mov rax, %[num]\n"						\
		"syscall\n"								\
		"mov %0, rax\n"							\
		".att_syntax prefix\n"					\
		: "=r" (ret)							\
		: [a] "r" (first),						\
			[b] "r" (second),					\
			[c] "r" (third),					\
			[num] "r" (sysnum)					\
		: "rdi", "rsi", "rdx", "rax"			\
	)

#define sys4(ret, sysnum, first, second, third, fourth) \
	asm(												\
		".intel_syntax noprefix\n"						\
		"mov r10, %[d]\n"								\
		"mov rdx, %[c]\n"								\
		"mov rsi, %[b]\n"								\
		"mov rdi, %[a]\n"								\
		"mov rax, %[num]\n"								\
		"syscall\n"										\
		"mov %0, rax\n"									\
		".att_syntax prefix\n"							\
		: "=r" (ret)									\
		: [a] "r" (first),								\
			[b] "r" (second),							\
			[c] "r" (third),							\
			[d] "r" (fourth),							\
			[num] "r" (sysnum)							\
		: "rdi", "rsi", "rdx", "rax", "r10"				\
	)
//	Syscall functions

#define exit(exit_status) \
		sys1_noret((uint64_t)SYS_EXIT, (uint64_t)exit_status)

#define write(fd, buff, len) \
		sys3(ret, (uint64_t)SYS_WRITE, (uint64_t)fd, (void *)buff, (uint64_t)len)

#define close(fd) \
		sys1_noret((uint64_t)SYS_CLOSE, (uint64_t)fd)

#define read(ret, fd, buff, len) \
		sys3(ret, (uint64_t)SYS_READ, (uint64_t)fd, (void *)buff, (uint64_t)len)

#define open(ret, path, mode) \
		sys2(ret, (uint64_t)SYS_OPEN, (void *)path, (uint64_t)mode)

#define fstat(ret, path, statbuf) \
		sys2(ret, (uint64_t)SYS_FSTAT, (void *)path, (uint64_t)statbuf)

#define getdents(ret, fd, buff, size) \
		sys3(ret, (uint64_t)SYS_GETDENTS, (uint64_t)fd, (void *)buff, (uint64_t)size)

#define munmap(ret, addr, len) \
		sys2(ret, (uint64_t)SYS_MUNMAP, (void *)addr, (uint64_t)len)

#define ftruncate(ret, fd, size) \
		sys2(ret, (uint64_t)SYS_FTRUNCATE, (void *)fd, (uint64_t)size)

#define ptrace(ret, request, pid, addr, data) \
    	sys4(ret, (uint64_t)SYS_PTRACE, (uint64_t)request, (uint64_t)pid, (uint64_t)addr, (uint64_t)data)

// See mmap below
#define mmap_real(ret, addr, len, prot, flags, fd, offset)	\
	asm(													\
		".intel_syntax noprefix\n"							\
		"mov r9, %[f]\n"									\
		"mov r8, %[e]\n"									\
		"mov r10, %[d]\n"									\
		"mov rdx, %[c]\n"									\
		"mov rsi, %[b]\n"									\
		"mov rdi, %[a]\n"									\
		"mov rax, 9\n"										\
		"syscall\n"											\
		"mov %0, rax\n"										\
		".att_syntax prefix\n"								\
		: "=r" (ret)										\
		: [a] "r" (addr),									\
			[b] "r" (len),									\
			[c] "r" (prot),									\
			[d] "r" (flags),								\
			[e] "r" (fd),									\
			[f] "r" (offset)								\
		: "rdi", "rsi", "rdx",								\
			"r10", "r9", "r8", "rax"						\
	)

#define mmap(ret, addr, len, prot, flags, fd, offset)	\
		mmap_real(										\
			ret,										\
			(void *)addr,								\
			(uint64_t)len,								\
			(uint64_t)prot,								\
			(uint64_t)flags,							\
			(uint64_t)fd,								\
			(uint64_t)offset							\
		)

#define mremap(ret, old_addr, old_size, new_size, flags)	\
		sys4(ret, (uint64_t)SYS_MREMAP,						\
			(void *)old_addr,								\
			(uint64_t)old_size,								\
			(uint64_t)new_size, (uint64_t)flags);

#endif

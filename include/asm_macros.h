/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_macros.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nguiard <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 13:38:07 by nguiard           #+#    #+#             */
/*   Updated: 2024/10/30 15:09:14 by nguiard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASM_MACROS_H
#define ASM_MACROS_H

//	Syscall numbers
#define SYS_WRITE	1	
#define SYS_MMAP	1	
#define SYS_EXIT	60	


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

//	Syscall functions

// uintXX_t	exit_status
#define exit(exit_status) \
		sys1_noret((uint64_t)SYS_EXIT, (uint64_t)exit_status)

// uint64_t	ret	-> return value placeholder
// uintXX_t	fd
// void *	data
// uintXX_t	len
#define write(ret, fd, data, len) \
		sys3(ret, (uint64_t)SYS_WRITE, (uint64_t)fd, (void *)data, (uint64_t)len)

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

#endif

fin = int("25df", 16) + 2



original_jmp = fin - (int("24e7", 16) + 1)

ptrace_start = fin - (int("251d", 16))
ptrace_end = fin - (int("253f", 16))
decrypt_start = fin - (int("2546", 16))
key = fin - (int("2554", 16) + 2)

final_jmp = fin - (int("25d0", 16) + 1)

print("final_jmp: ", final_jmp)
print("original_jmp: ", original_jmp)

print("key: ", key)

print("ptrace_start: ", ptrace_start)
print("ptrace_end: ", ptrace_end)
print("decrypt_start: ", decrypt_start)

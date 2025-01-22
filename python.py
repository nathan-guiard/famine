fin = int("25dd", 16) + 2



original_jmp = fin - (int("24e7", 16) + 1)

ptrace_start = fin - (int("2521", 16))
ptrace_end = fin - (int("2543", 16))
decrypt_start = fin - (int("2544", 16))
key = fin - (int("2552", 16) + 2)

final_jmp = fin - (int("25ce", 16) + 1)

print("final_jmp: ", final_jmp)
print("original_jmp: ", original_jmp)

print("key: ", key)

print("ptrace_start: ", ptrace_start)
print("ptrace_end: ", ptrace_end)
print("decrypt_start: ", decrypt_start)

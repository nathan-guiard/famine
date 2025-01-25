fin = int("25a9", 16) + 2



original_jmp = fin - (int("2407", 16) + 1)

ptrace_start = fin - (int("2463", 16))
ptrace_end = fin - (int("24b2", 16))
decrypt_start = fin - (int("24ea", 16))
key = fin - (int("24f8", 16) + 2)

final_jmp = fin - (int("259a", 16) + 1)

print("final_jmp: ", final_jmp)
print("original_jmp: ", original_jmp)

print("key: ", key)

print("ptrace_start: ", ptrace_start)
print("ptrace_end: ", ptrace_end)
print("decrypt_start: ", decrypt_start)

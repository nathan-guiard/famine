fin = int("252b", 16) + 2



original_jmp = fin - (int("2417", 16) + 1)

ptrace_start = fin - (int("243a", 16))
ptrace_end = fin - (int("2455", 16))
decrypt_start = fin - (int("248d", 16))
key = fin - (int("249b", 16) + 2)

final_jmp = fin - (int("251c", 16) + 1)

print("final_jmp: ", final_jmp)
print("original_jmp: ", original_jmp)

print("key: ", key)

print("ptrace_start: ", ptrace_start)
print("ptrace_end: ", ptrace_end)
print("decrypt_start: ", decrypt_start)

all:
	mkdir -p /tmp/test /tmp/test2
	rm -f /tmp/test/* /tmp/test2/*
	nasm -f elf64 register.s -g -o /tmp/test/register.o
	ld /tmp/test/register.o -o /tmp/test/registers
	gcc -g -o /tmp/test/hello_world hello_world.c
	chmod 777 /tmp/test/hello_world


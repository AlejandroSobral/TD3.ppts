all:
	nasm -g -f elf64 -F DWARF -osleep.o sleep.asm
	nasm -g -f elf64 -F DWARF -outils.o utils.asm
	ld -g -oadormir sleep.o utils.o
clean:
	rm -f *.o adormir

MBALIGN equ 1 << 0;
MEMINFO equ 1 << 1;
FLAGS 	equ MBALIGN | MEMINFO
MAGIC  	equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM



section .bss
align 16
stack_bottom:
resb 16384
stack_top:



section .heap
global __BEG_OF_HEAP
__BEG_OF_HEAP:
resb 1


section .text
global _start
_start:

	extern kernel_main
	call kernel_main
	cli
.hang	hlt
	jmp .hang

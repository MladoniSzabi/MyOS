#include <sys/tty.h>
#include <sys/memorymap.h>

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "ix86-elf compiler needed"
#endif

void kernel_main(void)
{
	terminal_initialise();
	initialise_memory_manager();
}

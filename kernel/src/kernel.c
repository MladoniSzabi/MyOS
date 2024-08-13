#include <sys/tty.h>
#include <sys/memorymap.h>
#include <sys/interrupts.h>
#include <sys/acpi.h>
#include <sys/ps2.h>
#include <sys/keyboard.h>

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "ix86-elf compiler needed"
#endif

#include "sys/debug.h"

void kernel_main(void)
{
	terminal_initialise();
	initialise_memory_manager();
	findRSDT();
	enable_interrupts();
	init_ps2();
	while (1)
	{
		uint16_t keycode = process_keyboard();
		if (keycode == 0)
		{
			continue;
		}

		if (!(keycode & 0x100))
		{
			char c = keycode_to_ascii(keycode);
			if (c)
				terminal_putchar(c);
		}
	}
}

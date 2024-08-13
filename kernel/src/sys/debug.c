#include <sys/debug.h>
#include <sys/tty.h>

void printhex(uint64_t num) { printhex_digits(num, 32); }

void printhex_digits(uint64_t num, unsigned int digits)
{
	int shift = digits - 4;
	terminal_write("0x", 2);
	while (shift >= 0)
	{
		unsigned int dig = (num >> shift) % 16;
		if (dig >= 10)
		{
			dig -= 10;
			terminal_putchar('A' + dig);
		}
		else
		{
			terminal_putchar('0' + dig);
		}
		shift -= 4;
	}
}

void println()
{
	terminal_putchar('\n');
}

void printspc()
{
	terminal_putchar(' ');
}

void kernel_panic(const char *msg)
{
	terminal_writestring(msg);
	while (1)
	{
		__asm__("hlt");
	}
}
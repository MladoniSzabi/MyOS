Bootloader 1 is the first bootloader that runs when the disk is booted.
It is limited to 512 bytes and it runs in 16bit real mode.
It's purpose is to enable the A20 line, to read the memory map and to read and run the second bootloader.

The second bootloader run in 16bit real mode, just like the first one, however it is not limited to a size of 512 bytes.
It's purpose is to read the OS and the third bootloader from the disk (since we loose access to the bios once we enter protected mode), set up the global descriptor table (GDT) (which tells the CPU about memory segments) and run the third bootloader.

The third bootloader run in 32bit protected mode
The purpose of the third bootloader is to parse the ELF file that the kernel is stored in, place all segments into the correct place in memory, and start the kernel
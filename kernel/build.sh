export TARGET="/home/cinker/programming/else/assembly/newOS/cross-compiler/bin/i686-elf"
nasm -felf32 boot.s -o obj/boot.o
${TARGET}-gcc -c src/kernel.c -o obj/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
${TARGET}-gcc -T linker.ld -o bin/kernel.bin -ffreestanding -O2 -nostdlib obj/boot.o obj/kernel.o -lgcc
if grub-file --is-x86-multiboot bin/kernel.bin; then
  echo multiboot confirmed
else
  echo the file is not multiboot
fi

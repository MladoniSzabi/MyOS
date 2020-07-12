  [BITS 32]
  [ORG 0x800]

  counter equ 0x7e00
  file_offset equ 0x7e02
  kernel_entry_point equ 0x7e06
  program_segment_count equ 0x7e0a
  program_segment_data equ 0x7e10

bootloader2_main:

  cli
  call terminal_initialise
  call init_pic
  call populate_idt
  lidt [idt_descriptor]
  sti

  call read_from_disk

  jmp $

bootloader_msg: db "Hello, World!", 0

read_from_disk:
  cli
  mov byte [ata_flag], 0x0
  ;; select device
  mov eax, 0                    ;slave bit (not sure what this means)
  shr eax, 4
  mov dx, 0x1f6
  out dx, al

  ;; enable dma
  mov eax, 1
  mov dx, 0x1f1
  out dx, al

  ;; set byte count
  mov dx, 0x1f3
  mov al, 0xa
  out dx, al

  ;; send packet command
  mov eax, 0xa0
  mov dx, 0x1f7
  out dx, al

  ;; wait till controller is ready (busy flag)
  mov dx, 0x1f7
read_from_disk_loopdi_loop1:
  in al, dx
  and al, 0b10000000
  cmp al, 0b10000000
  je read_from_disk_loopdi_loop1

  ;; wait till controller is ready (DRQ flag)
  mov dx, 0x1f7
read_from_disk_loopdi_loop2:
  in al, dx
  and al, 0b00001000
  cmp al, 0b00001000
  jne read_from_disk_loopdi_loop2

  ;; check for errors
  in al, dx
  and al, 1
  cmp al, 1
  jne read_from_disk_no_error
  mov al, 0x45
  call putchar
  ret

read_from_disk_no_error:

  sti

  mov al, 'a'
  call putchar

  jmp $

  ;; send command
  mov dx, 0x1f0
  mov ebx, ata_read_cmd
  mov ecx, 0
read_from_disk_loopdi_loop3:
  mov ax, [ebx]
  ror ax, 8
  out dx, ax
  add ebx, 2
  inc ecx
  cmp ecx, 6
  jne read_from_disk_loopdi_loop3

  mov al, 'b'
  call putchar

  ;; wait for cmd to arrive (signalled with an interrupt)
read_from_disk_loopdi_loop4:
  cmp byte [ata_flag], 0x1
  jne read_from_disk_loopdi_loop4

 	mov al, 'c'
  call putchar

  mov dx, 0x1f7
  in al, dx
  and al, 1
  add al, 0x41
  call putchar

  ;; read actual data sent
  mov dx, 0x1f5
  in al, dx
  mov cl, al
  mov dx, 0x1f4
  in al, dx
  mov ch, al

  cmp ecx, 0
  jne read_from_disk_jmp1
  mov ecx, 0x20
  jmp read_from_disk_loopdi_loop5

read_from_disk_jmp1:
  cmp ecx, 0xa
  jle read_from_disk_loopdi_loop5
  mov ecx, 0x20

  mov al, cl
  add al, 0x41
  call putchar

  ;; display data
read_from_disk_loopdi_loop5:
  mov dx, 0x1f0
  in ax, dx
  ror ax, 8
  call putchar
  ror ax, 8
  call putchar

  dec ecx
  cmp ecx, 0x0
  jne read_from_disk_loopdi_loop5

  mov al, 'd'
  call putchar

  ret


ata_read_cmd: db 0xa8, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0
disk_buffer:  times 10 db 0

  %include "string.asm"
  %include "tty.asm"
  %include "stdio.asm"
  %include "isr.asm"

  	times 0xa00-($-$$)  db 0x0

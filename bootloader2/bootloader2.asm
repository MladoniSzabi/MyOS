[ORG 0x1000]

bootloader_main:

 	call read_os
  call read_ternary_bootloader
  call setup_gdt

	; load global descriptor table
  cli
	lgdt [gdt_descriptor]
	mov eax, cr0
	or al, 1
	mov cr0, eax

	jmp clear_prefetch_queue
	nop
	nop


clear_prefetch_queue:
; set up the segment registers
	mov ax, 0x08
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov esp, 0x6fff

; jump to the ternary bootloader
	db 0x66
	db 0xea
	dd 0x800
	dw 0x10

  jmp $

read_ternary_bootloader:
  mov bx, 0x800
  mov ah, 0x02
  mov al, [num_sectors]
  mov cl, 0x04
  mov ch, 0x00
  mov dh, 0x0
  mov dl, [0x500]
  int 0x13
  jc error
	cmp al, [num_sectors]
	jne error

  ret

; read the ELF binary with the OS from disk
read_os:
	mov ax, 0x1000
	mov es, ax
	mov bx, 0x0
	mov ah, 0x02
	mov al, [num_os_sectors]
	mov cl, 0x04
	add cl, [num_sectors]
	mov ch, 0x00
  mov dh, 0x0
	mov dl, [0x500]
	int 0x13
	jc error
	cmp al, [num_os_sectors]
	jne error

	xor ax,ax
	mov es, ax

	ret

; print '?' to the screen
error:
	mov al, '?'
	mov ah, 0x0e
	int 0x10
	jmp $

setup_gdt:
	mov ax, 0x7ff0
	mov es, ax
	mov cx, gdt_end
	sub cx, gdt_start
	mov ax, 0
setup_gdt_loopdi_loop:
	mov bx, gdt_start
	add bx, ax
	mov dx, [bx]
	mov bx, ax
	mov [es:bx], dx
	add ax, 1
	cmp ax, cx
	jne	setup_gdt_loopdi_loop

	ret

num_os_sectors:           db 16
num_sectors:  db 3
gdt_start:
	;; Null descriptor
	times 8 db 0
	;; data segment
	dw 0xffff
	dw 0x0000
	db 0x00
	db 0x92
	db 0xcf
	db 0x00
	;; Code segment
	dw 0x0c00
	dw 0x0000
	db 0x00
	db 0x9a
	db 0xc0
	db 0x00
gdt_end:

gdt_descriptor:
	dw 40
	dd 0x0007ff00

  times 0x400-($-$$) db 0

  [ORG 0x7c00]      ; add to offsets

  jmp bootloader_main

  times 3-($-$$) DB 0x90   ; Support 2 or 3 byte encoded JMPs before BPB.

	; Dos 4.0 EBPB 1.44MB floppy
	OEMname:           db    "mkfs.fat"  ; mkfs.fat is what OEMname mkdosfs uses
	bytesPerSector:    dw    512
	sectPerCluster:    db    1
	reservedSectors:   dw    1
	numFAT:            db    2
	numRootDirEntries: dw    224
	numSectors:        dw    2880
	mediaType:         db    240
	numFATsectors:     dw    9
	sectorsPerTrack:   dw    18
	numHeads:          dw    2
	numHiddenSectors:  dd    0
	numSectorsHuge:    dd    0
	driveNum:          db    0
	reserved:          db    0
	signature:         db    0x29
	volumeID:          dd    0x2d7e5a1a
	volumeLabel:       db    "NO NAME    "
	fileSysType:       db    "FAT12   "

bootloader_wait_8042_data:
  in al, 0x64
  test al, 0x1
  jz bootloader_wait_8042_data
  ret

bootloader_enable_a20_line:

 	call bootloader_check_a20_line
	cmp ax, 0x0
	jne enable_a20_exit

	;; method 1
  mov ax, 0x2401
  int 0x15

  call bootloader_check_a20_line
  cmp ax, 0x0
  jne enable_a20_exit

  ;; method 2
  cli

  call bootloader_wait_8042_data
  mov al, 0xad
  out 0x64, al

  call bootloader_wait_8042_data
  mov al, 0xd0
  out 0x64, al

  call bootloader_wait_8042_data
  mov al, 0x60
  push eax

  call bootloader_wait_8042_data
  mov al, 0xd1
  out 0x64, al

  call bootloader_wait_8042_data
  pop eax
  or al, 2
  out 0x60, al

  call bootloader_wait_8042_data
  mov al, 0xae
  out 0x64, al

  call bootloader_wait_8042_data

  sti

  call bootloader_check_a20_line
  cmp ax, 0x0
  jne enable_a20_exit

  mov al, 0x50
  mov ah, 0x0e
  int 0x10

  jmp $

  ;; method 3
  in al, 0x92
  or al, 2
  out 0x92, al


enable_a20_exit:
  ret

bootloader_check_a20_line:

  push es
  push fs
  push si
  push di

  xor ax, ax
  mov es, ax
  mov di, 0x0500

  mov ax,0xffff
  mov fs,ax,
  mov si, 0x0510

  mov al, [es:di]
  push ax

  mov al, [fs:si]
  push ax

  mov byte [es:di], 0x00
  mov byte [fs:si], 0xff

  cmp byte [es:di], 0xff

  pop ax
  mov [fs:si], ax

  pop ax
  mov [es:di], ax

  mov ax, 0

  je a20_exit
  mov ax, 1

a20_exit:
  pop di
  pop si
  pop fs
  pop es

  ret

bootloader_do_nothing:
  jmp $

bootloader_print_char:
  mov ah, 0x0e
  int 0x10
  ret


bootloader_error_2:
  mov al, ah
  add al, '0'
  call bootloader_error
bootloader_error_1:
  add al, 'a'
  call bootloader_error

bootloader_error:
  call bootloader_print_char
  int 0x18
  jmp $

bootloader_setup_gdt:
  mov ax, 0x7ff0
  mov es, ax
  mov cx, gdt_end
  sub cx, gdt_start
  mov ax, 0
loopdi_loop:
  mov bx, gdt_start
  add bx, ax
  mov dx, [bx]
  mov bx, ax
  mov [es:bx], dx
  add ax, 1
  cmp ax, cx
  jne loopdi_loop

  ret

bootloader_main:
  cli
  xor ax, ax
	mov ds, ax
	mov ss, ax
  mov es, ax
	mov sp, 0x6fff

  sti
  cld

  mov [0x500], dl

  xor ax, ax
  int 0x13


  mov ax, 0x0
	mov ds, ax
  mov es, ax

	mov bx, 0x800
  mov ah, 0x02
	mov al, [num_sectors]
	mov cl, 0x02
	mov ch, 0x00
	int 0x13
	jc bootloader_error_2
	cmp al, [num_sectors]
	jne bootloader_error_1

  call bootloader_setup_gdt

  mov ax, 0x0
  mov es, ax
  mov ax, 0
  mov ds, ax
  mov bx, 0

	call bootloader_enable_a20_line

  cli
  lgdt [gdt_descriptor]
  mov eax, cr0
  or al, 1
  mov cr0, eax

  jmp clear_prefetch_queue
  nop
  nop

clear_prefetch_queue:
  mov ax, 0x18
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  mov esp, 0x6fff

  db 0x66
  db 0xea
  dd 0x800
  dw 0x20

disk_label:               db 0
num_sectors:              db 5
start_printing_from:      dw 0x0
counter:                  db 0

gdt_descriptor:
  dw 40
  dd 0x0007ff00

gdt_start:
  ;; Null descriptor
  times 8 db 0
  ;; 16 bit code segment
	dw 0x0200
	dw 0x0c00
	db 0x00
	db 0x9a
	db 0xc0
	db 0x00
  ;; 16 bit data segment
  dw 0x0f00
  dw 0x0000
  db 0x00
  db 0x92
  db 0x00
  db 0x00
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

  times 510-($-$$)        db 0
                          db 0x55
                          db 0xAA

times 0x168000-($-$$)     db 0

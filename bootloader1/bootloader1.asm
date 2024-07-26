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

wait_8042_data:
  in al, 0x64
  test al, 0x1
  jz wait_8042_data
  ret

; the a20 line allows the computer to address up to 16MB of memory instead of 1MB
; this is disabled by default to remain compatible with older processors and we need to enable it
enable_a20_line:

 	call check_a20_line
	cmp ax, 0x0
	jne enable_a20_exit

	;; method 1
  mov ax, 0x2401
  int 0x15

  call check_a20_line
  cmp ax, 0x0
  jne enable_a20_exit

  ;; method 2
  cli

  call wait_8042_data
  mov al, 0xad
  out 0x64, al

  call wait_8042_data
  mov al, 0xd0
  out 0x64, al

  call wait_8042_data
  mov al, 0x60
  push eax

  call wait_8042_data
  mov al, 0xd1
  out 0x64, al

  call wait_8042_data
  pop eax
  or al, 2
  out 0x60, al

  call wait_8042_data
  mov al, 0xae
  out 0x64, al

  call wait_8042_data

  sti

  call check_a20_line
  cmp ax, 0x0
  jne enable_a20_exit

  ;; method 3
  in al, 0x92
  or al, 2
  out 0x92, al


enable_a20_exit:
  ret

; to check whether the a20 line is enabled, we compare the value at 0x500
; with the value 1MB above it. If the A20 line is disabled, the value 1MB
; above, will wrap back around to 0x500.
; returns 0 in ax if the line is disabled and 1 if it is enabled
check_a20_line:

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

  mov al, [es:di]           ; es:di -> 0x0000:0x0500 -> 0x00000500
  push ax

  mov al, [fs:si]           ; fs:si -> 0xffff:0x0510 -> 0x00100500
  push ax

  mov byte [es:di], 0x00    ; write different values to the two memory addresses
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

; writes an excalmation mark to the screen
bootloader_error:
  mov al, '!'
  mov ah, 0x0e
  int 0x10
  jmp $

read_second_bootloader:
  mov bx, 0x1000          ; destination of the second bootloader
	mov ah, 0x02            ; sector on disk from which to start reading
                          ; 0x01 is this bootloader and 0x02 is he second one
	mov al, [num_sectors]   ; number of sectors to read
	mov cl, 0x02            
	mov ch, 0x00
	int 0x13
	jc bootloader_error     ; cf set if read failed
	cmp al, [num_sectors]   ; al contains actual number of sectors read
	jne bootloader_error

  ret

; 0xe820 is a bios function that returns a memory map of the computer
; this function asks the bios for a memory map and places it at adress 0x50d
read_memory_map:
  mov di, 0x50d   ; the address to store the memory map
	xor ebx, ebx		; ebx must be 0 to start
	xor bp, bp		; keep an entry count in bp
	mov edx, 0x0534D4150	; Place "SMAP" into edx
	mov eax, 0xe820
	mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
	mov ecx, 24		; ask for 24 bytes
	int 0x15
	jc short .failed	; carry set on first call means "unsupported function"
	mov edx, 0x0534D4150	; Some BIOSes apparently trash this register?
	cmp eax, edx		; on success, eax must have been reset to "SMAP"
	jne short .failed
	test ebx, ebx		; ebx = 0 implies list is only 1 entry long (worthless)
	je short .failed
	jmp short .jmpin
.e820lp:
	mov eax, 0xe820		; eax, ecx get trashed on every int 0x15 call
	mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
	mov ecx, 24		; ask for 24 bytes again
	int 0x15
	jc short .e820f		; carry set means "end of list already reached"
	mov edx, 0x0534D4150	; repair potentially trashed register
.jmpin:
	jcxz .skipent		; skip any 0 length entries
	cmp cl, 20		; got a 24 byte ACPI 3.X response?
	jbe short .notext
	test byte [es:di + 20], 1	; if so: is the "ignore this data" bit clear?
	je short .skipent
.notext:
	mov ecx, [es:di + 8]	; get lower uint32_t of memory region length
	or ecx, [es:di + 12]	; "or" it with upper uint32_t to test for zero
	jz .skipent		; if length uint64_t is 0, skip entry
	inc bp			; got a good entry: ++count, move to next storage spot
	add di, 24
.skipent:
	test ebx, ebx		; if ebx resets to 0, list is complete
	jne short .e820lp
.e820f:
	mov [0x505], bp	; store the entry count
	clc			; there is "jc" on end of list to this point, so the carry must be cleared
	ret
.failed:
	stc			; "function unsupported" error exit
	ret

bootloader_main:
  cli
  xor ax, ax      ; set ax to 0
	mov ds, ax      ; zero out all the segment registers
	mov ss, ax
  mov es, ax
	mov sp, 0x6fff  ; set the stack pointer

  sti
  cld

  mov [0x504], dl
  int 0x13

  call read_second_bootloader

	call enable_a20_line
  call read_memory_map

to_bootloader_2:
  jmp 0x1000

num_sectors:              db 2
  times 510-($-$$)        db 0
                          db 0x55
                          db 0xAA

times 0x168000-($-$$)     db 0

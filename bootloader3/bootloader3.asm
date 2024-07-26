  [BITS 32]
  [ORG 0x800]

bootloader2_main:
  ; put each part of the OS in the correct place in memory
  call bootloader2_parse_elf
  ; start the kernel
  mov ebx, [bootloader2_entry_point]
  jmp ebx

  ;; eax: source
	;; ebx: destination
	;; ecx: count
memcpy:
	push eax
	push ebx
	push ecx
	push edx

memcpy_loopdi_loop:
	mov dl, [eax]
	mov [ebx], dl
	inc eax
	inc ebx
	dec ecx
	cmp ecx, 0
	jne memcpy_loopdi_loop

	pop edx
	pop ecx
	pop ebx
	pop eax
	ret

bootloader2_parse_elf:
  mov eax, 0
  mov ebx, 0x10000
  add ebx, 24
  mov eax, [ebx]
  mov [bootloader2_entry_point], eax

  add ebx, 4
  mov eax, 0
  mov eax, [ebx]
  mov [bootloader2_program_header_offset], eax

  add ebx, 16
  mov eax, 0
  mov ax, [ebx]
  mov [bootloader2_program_header_count], ax

  mov eax, bootloader2_program_header_info
  mov [bootloader2_array_pointer], eax

  mov cx, 0
  mov ebx, [bootloader2_program_header_offset]
  add ebx, 0x10000

bootloader2_parse_ph:
bootloader2_parse_ph_loopdi_loop:
  ;; P_OFFSET
  add ebx, 4
  mov eax, [ebx]
  mov edx, ebx
  mov ebx, [bootloader2_array_pointer]
  mov [ebx], eax
  add ebx, 4
  mov [bootloader2_array_pointer], ebx
  mov ebx, edx

  ;; P_VADDR
  add ebx, 4
  mov eax, [ebx]
  mov edx, ebx
  mov ebx, [bootloader2_array_pointer]
  mov [ebx], eax
  add ebx, 4
  mov [bootloader2_array_pointer], ebx
  mov ebx, edx

  ;; P_PADDR
  add ebx, 4
	mov eax, [ebx]
	mov edx, ebx
	mov ebx, [bootloader2_array_pointer]
	mov [ebx], eax
	add ebx, 4
	mov [bootloader2_array_pointer], ebx
	mov ebx, edx

  ;; P_FILESZ
  add ebx, 4
	mov eax, [ebx]
	mov edx, ebx
	mov ebx, [bootloader2_array_pointer]
	mov [ebx], eax
	add ebx, 4
	mov [bootloader2_array_pointer], ebx
	mov ebx, edx

  ;; P_MEMSZ
  add ebx, 4
  mov eax, [ebx]
	mov edx, ebx
	mov ebx, [bootloader2_array_pointer]
	mov [ebx], eax
	add ebx, 4
	mov [bootloader2_array_pointer], ebx
	mov ebx, edx

  add ebx, 12

  inc cx
  cmp cx, [bootloader2_program_header_count]
  jne bootloader2_parse_ph

  mov cx, 0
bootloader2_create_program_sections:
  mov ebx, bootloader2_program_header_info
bootloader2_create_program_sections_loopdi_loop:
  push ecx
  mov ecx, [ebx+12]
  cmp ecx, 0
  je bootloader2_create_program_sections_next
  mov eax, 0x10000
  add eax, [ebx]
  mov edx, ebx
  mov ebx, [ebx+8]
  mov [0x500], ebx
  call memcpy
  mov ebx, edx

bootloader2_create_program_sections_next:
  pop ecx
  add ebx, 20
  inc cx

  cmp cx, [bootloader2_program_header_count]
  jne bootloader2_create_program_sections_loopdi_loop

  ret


bootloader2_x: dd 0
bootloader2_y: dd 0
bootloader2_entry_point:  dd 0
bootloader2_program_header_offset:  dd 0
bootloader2_program_header_count: dw 0
bootloader2_counter:  db 0
bootloader2_array_pointer:  dd 0
bootloader2_program_header_info:

  	times 0xa00-($-$$)  db 0x0

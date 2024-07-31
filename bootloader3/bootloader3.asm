  [BITS 32]
  [ORG 0x7C00]

bootloader3_main:
  ; put each part of the OS in the correct place in memory
  call kernel_parse_elf
  ; start the kernel
  mov eax, 0xabcd
  call printhex
  mov ebx, [kernel_entry_point]
  jmp ebx

println:
    mov byte [col], 0
    inc byte [row]
    ret

printhex:
    push eax
    push ebx
    push ecx
    push edx

    xor ecx, ecx

    mov ebx, eax

    xor eax, eax
    mov al, [row]
    mov ecx, 160
    mul ecx
    add eax, 0xB8000
    xor ecx, ecx
    mov cl, [col]
    shl ecx, 1
    add eax, ecx

printex_loopdi_loop:
    mov ecx, 0x0f
    and ecx, ebx
    add cl, 48
    cmp cl, 57
    jle printex_lessthanten
    add cl, 7

printex_lessthanten:
    mov ch, [color]
    mov [eax], cx
    add eax, 2
    shr ebx, 4
    cmp ebx, 0
    jne printex_loopdi_loop

    pop edx
    pop ecx
    pop ebx
    pop eax
    ret

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

kernel_parse_elf:
  mov eax, 0

  mov ebx, 0x10000
  add ebx, 24
  mov eax, [ebx]
  mov [kernel_entry_point], eax

  add ebx, 4
  mov eax, 0
  mov eax, [ebx]
  mov [kernel_program_header_offset], eax

  add ebx, 16
  mov eax, 0
  mov ax, [ebx]
  mov [kernel_program_header_count], ax

  mov eax, kernel_program_header_info
  mov [kernel_array_pointer], eax

  mov cx, 0
  mov ebx, [kernel_program_header_offset]
  add ebx, 0x10000

kernel_parse_ph:
kernel_parse_ph_loopdi_loop:
  ;; P_OFFSET
  add ebx, 4
  mov eax, [ebx]
  mov edx, ebx
  mov ebx, [kernel_array_pointer]
  mov [ebx], eax
  add ebx, 4
  mov [kernel_array_pointer], ebx
  mov ebx, edx

  ;; P_VADDR
  add ebx, 4
  mov eax, [ebx]
  mov edx, ebx
  mov ebx, [kernel_array_pointer]
  mov [ebx], eax
  add ebx, 4
  mov [kernel_array_pointer], ebx
  mov ebx, edx

  ;; P_PADDR
  add ebx, 4
	mov eax, [ebx]
	mov edx, ebx
	mov ebx, [kernel_array_pointer]
	mov [ebx], eax
	add ebx, 4
	mov [kernel_array_pointer], ebx
	mov ebx, edx

  ;; P_FILESZ
  add ebx, 4
	mov eax, [ebx]
	mov edx, ebx
	mov ebx, [kernel_array_pointer]
	mov [ebx], eax
	add ebx, 4
	mov [kernel_array_pointer], ebx
	mov ebx, edx

  ;; P_MEMSZ
  add ebx, 4
  mov eax, [ebx]
	mov edx, ebx
	mov ebx, [kernel_array_pointer]
	mov [ebx], eax
	add ebx, 4
	mov [kernel_array_pointer], ebx
	mov ebx, edx

  add ebx, 12

  inc cx
  cmp cx, [kernel_program_header_count]
  jne kernel_parse_ph

  push eax
  mov eax, 0x1234
  call printhex
  pop eax


  mov cx, 0
kernel_create_program_sections:
  mov ebx, kernel_program_header_info
kernel_create_program_sections_loopdi_loop:
  push ecx
  mov ecx, [ebx+12]
  cmp ecx, 0
  je kernel_create_program_sections_next
  mov eax, 0x10000
  add eax, [ebx]
  mov edx, ebx
  mov ebx, [ebx+8]
  mov [0x500], ebx
  call memcpy
  mov ebx, edx

kernel_create_program_sections_next:
  pop ecx
  add ebx, 20
  inc cx

  push eax
  xor eax, eax
  mov ax, cx
  call printhex
  pop eax

  cmp cx, [kernel_program_header_count]
  jne kernel_create_program_sections_loopdi_loop

  

  ret


row: db 0
col: db 0
color: db 0x07
kernel_x: dd 0
kernel_y: dd 0
kernel_entry_point:  dd 0
kernel_program_header_offset:  dd 0
kernel_program_header_count: dw 0
kernel_counter:  db 0
kernel_array_pointer:  dd 0
kernel_program_header_info:

  	times 0xa00-($-$$)  db 0x0

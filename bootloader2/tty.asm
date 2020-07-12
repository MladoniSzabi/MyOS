	VGA_COLOR_BLACK equ 0x0,
	VGA_COLOR_BLUE equ 0x1,
	VGA_COLOR_GREEN equ 0x2,
	VGA_COLOR_CYAN equ 0x3,
	VGA_COLOR_RED equ 0x4,
	VGA_COLOR_MAGENTA equ 0x5,
	VGA_COLOR_BROWN equ 0x6,
	VGA_COLOR_LIGHT_GREY equ 0x7,
	VGA_COLOR_DARK_GREY equ 0x8,
	VGA_COLOR_LIGHT_BLUE equ 0x9,
	VGA_COLOR_LIGHT_GREEN equ 0xa,
	VGA_COLOR_LIGHT_CYAN equ 0xb,
	VGA_COLOR_LIGHT_RED equ 0xc,
	VGA_COLOR_LIGHT_MAGENTA equ 0xd,
	VGA_COLOR_LIGHT_BROWN equ 0xe,
	VGA_COLOR_WHITE equ 0xf,

  VGA_WIDTH equ 80
  VGA_HEIGHT equ 25
  VGA_MEMORY equ 0xb8000

terminal_row: db 0
terminal_column:  db 0
terminal_color: db 0
terminal_buffer:  dw 0, 0

terminal_initialise:
  push ebx
  push cx
  mov byte [terminal_row], 0
  mov byte [terminal_column], 0
  mov byte [terminal_color], 0x07
  mov ebx, 0xb8000
  mov [terminal_buffer], eax

  mov cx, 0
terminal_initialise_loopdi_loop:
  mov ax, 0x0720
  mov [ebx], ax
  inc ebx
  inc ebx
  inc cx
  cmp cx, 80*25
  jne terminal_initialise_loopdi_loop

  pop cx
  pop ebx
  ret

  ;; al: color
terminal_setcolor:
  mov [terminal_color], al
  ret

  ;; bl: char
  ;; bh: color
  ;; cl: x
  ;; ch: y
terminal_putentryat:
  push eax
  push ebx
  push ecx
  mov al, ch
  mov ah, VGA_WIDTH
  mul ah
  add al, cl
  add eax, VGA_MEMORY
  mov ecx, eax
  mov al, bl
  mov ah, bh
  call vga_entry
  mov [ecx], ax
  pop ecx
  pop ebx
  pop eax
  ret

  ;; al: char
terminal_putchar:
  push eax
  push ebx
  push ecx

  cmp al, 0xa
  je terminal_putchar_process_newline
  cmp al, 0x0
  je terminal_putchar_end

  mov bl, al
  mov bh, [terminal_color]
  mov al, [terminal_row]
  mov cl, 2
  mul cl
  mov cl, al
  mov al, [terminal_column]
  mov ch, 2
  mul ch
  mov ch, al
  call terminal_putentryat
  call terminal_nextchar

terminal_putchar_end:
  pop ecx
  pop ebx
  pop eax
  ret


terminal_putchar_process_newline:
  call terminal_nextline
  jmp terminal_putchar_end

terminal_nextline:
  push ecx
  mov ch, [terminal_column]
	inc ch
	mov [terminal_column], ch
	cmp ch, VGA_HEIGHT
	jne terminal_nextline_end
	mov byte [terminal_column], 0

terminal_nextline_end:
  pop ecx
  ret


terminal_nextchar:
  push ecx
	mov cl, [terminal_row]
  inc cl
  mov [terminal_row], cl
  cmp cl, VGA_WIDTH
  jne terminal_nextchar_end
  mov byte [terminal_row], 0
  call terminal_nextline

terminal_nextchar_end:
  pop ecx
  ret

  ;; ebx: pointer
  ;; ecx: count
terminal_write:
  push eax
  push ebx
  push ecx

terminal_write_loopdi_loop:
  mov al, [ebx]
  call terminal_putchar
  inc ebx
  dec ecx
  cmp ecx, 0
  jne terminal_write_loopdi_loop

  pop ecx
  pop ebx
  pop eax
  ret

  ;; eax: pointer
terminal_writestring:
  push ebx
  push eax
  mov ebx, eax
  call strlen
  mov ecx, eax
  pop eax
  call terminal_write
  pop ebx
  ret

  ;; al: character color
  ;; ah: background color
  ;; al: return val
vga_entry_color:
  shl ah, 4
  or al, ah
  ret

  ;; al: character
  ;; ah: color
  ;; ax: retval
vga_entry:
  ;; empty
  ret

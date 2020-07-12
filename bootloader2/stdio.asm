;; al: character
putchar:
  push ebx
  push ecx
  push ax
  mov ebx, esp
  mov ecx, 1
  call terminal_write
  pop ax
  pop ecx
  pop ebx
  ret

	  ;; eax: address to pointer
puts:
  call terminal_writestring
  mov al, 0xa
  call putchar
  ret

	  ;; eax: address to pointer
printf:
  call terminal_writestring
  ret

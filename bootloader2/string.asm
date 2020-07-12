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

  ;; eax: source
  ;; ebx: destination
  ;; ecx
  ;; TODO:
memcmp:
  ret

  ;; TODO: wtf even is this??
memmove:
  ret

  ;; al: char
  ;; ebx: poiner
  ;; ecx: count
memset:
  push ebx
  push ecx

memset_loopdi_loop:
  mov [ebx], al
  inc ebx
  dec ecx
  cmp ecx, 0
  jne memset_loopdi_loop

  pop ecx
  pop ebx
  ret

  ;; eax: buffer
  ;; eax: retval
strlen:
  push ebx
  mov ebx, 0
strlen_loopdi_loop:
  cmp byte [eax], 0
  je strlen_end
  inc eax
  inc ebx
  jmp strlen_loopdi_loop
strlen_end:
  mov eax, ebx
  pop ebx
  ret

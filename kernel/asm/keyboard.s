global keyboard_interrupt
global keyboard_buffer_end

extern KEYBOARD_BUFFER_SIZE
extern keyboard_buffer
extern printhex

extern c_keyboard_interrupt

keyboard_buffer_end: db 0

keyboard_interrupt:
    pushad
    push es
    push ds

    mov eax, 0
    in al, 0x60
    push ax
    call c_keyboard_interrupt
    pop ax
    ; mov ebx, keyboard_buffer
    ; add ebx, [keyboard_buffer_end]
    ; mov [ebx], al
    ; mov edx, 0
    ; mov eax, 0
    ; push eax
    ; mov eax, [keyboard_buffer_end]
    ; push eax
    ; call printhex
    ; pop eax
    ; pop eax
    ; mov ebx, 0
    ; mov bl, [KEYBOARD_BUFFER_SIZE]
    ; div ebx
    ; mov [keyboard_buffer_end], dl

    mov al, 0x20
    out 0x20, al

    pop ds
    pop es
    popad
    iret
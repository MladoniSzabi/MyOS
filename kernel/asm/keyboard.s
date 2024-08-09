global keyboard_interrupt
global keyboard_buffer
global keyboard_buffer_end

keyboard_buffer: times 32 db 0
keyboard_buffer_end: db 0

extern c_keyboard_interrupt

keyboard_interrupt:
    pushad
    push es
    push ds
    mov eax, 0
    in al, 0x60
    push ax
    call c_keyboard_interrupt
    pop ax
    mov al, 0x20
    out 0x20, al
    pop ds
    pop es
    popad
    sti
    iret
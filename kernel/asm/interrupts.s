
row: db 0
col: db 0
color: db 0x07

; println:
;     mov byte [col], 0
;     inc byte [row]
;     ret

; printhex:
;     push eax
;     push ebx
;     push ecx
;     push edx

;     mov ebx, eax
;     xor ecx, ecx
;     xor eax, eax

;     mov al, [row]
;     mov ecx, 160
;     mul ecx
;     add eax, 0xB8000
;     xor ecx, ecx
;     mov cl, [col]
;     shl ecx, 1
;     add eax, ecx

; printex_loopdi_loop:
;     mov ecx, 0x0f
;     and ecx, ebx
;     add cl, 48
;     cmp cl, 57
;     jle printex_lessthanten
;     add cl, 7

; printex_lessthanten:
;     mov ch, [color]
;     mov word [eax], cx
;     add eax, 2
;     shr ebx, 4
;     cmp ebx, 0
;     jne printex_loopdi_loop

;     pop edx
;     pop ecx
;     pop ebx
;     pop eax
;     ret

global load_idt
global empty_irq_pic_1
global empty_irq_pic_2
extern isr_callbacks
extern exception_handler
extern exception_handler_argument

pic1_command equ 0x20
pic1_data equ 0x21
pic2_command equ 0xa0
pic2_data equ 0xa1

; pic commands
ICW1_ICW4 equ 0x01
ICW1_SINGLE equ 0x02
ICW1_INTERVAL4 equ 0x04
ICW1_LEVEL equ 0x08
ICW1_INIT equ 0x10

ICW4_8086 equ 0x01
ICW4_AUTO equ 0x02
ICW4_BUF_SLAVE equ 0x08
ICW4_BUF_MASTER equ 0x0c
ICW4_SFNM 0x10


mask_pic1: db 0
mask_pic2: db 0

; offset of 1st pic is going to be 0x20
; offset of 1st pic is going to be 0x28
reprogram_pic:
    push eax
    in al, pic1_data
    mov [mask_pic1], al
    in al, pic2_data
    mov [mask_pic2], al

    mov al, ICW1_INIT | ICW1_ICW4
    out pic1_command, al
    out 0x80, al
    out pic2_command, al
    out 0x80, al

    mov al, 0x20
    out pic1_data, al
    out 0x80, al
    mov al, 0x28
    out pic2_data, al
    out 0x80, al

    mov al, 4
    out pic1_data, al
    out 0x80, al
    mov al, 2
    out pic2_data, al
    out 0x80, al

    mov al, ICW4_8086
    out pic1_data, al
    out 0x80, al
    out pic2_data, al
    out 0x80, al

    mov al, [mask_pic1]
    out pic1_data, al
    mov al, [mask_pic2]
    out pic2_data, al

    ret

load_idt:
    call reprogram_pic
    push eax
    mov eax, [esp+8]
    lidt [eax]
    pop eax
    sti
    ret

empty_irq_pic_1:
    push eax
    mov al,20h
    out pic1_command,al
    pop eax
    iret

empty_irq_pic_2:
    push eax
    mov al,20h
    out pic1_command,al
    out pic2_command,al
    pop eax
    iret
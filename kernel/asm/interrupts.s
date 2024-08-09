global load_idt
global empty_irq_pic_1
global empty_irq_pic_2
global wait_io
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
ICW4_SFNM equ 0x10


mask_pic1: db 0
mask_pic2: db 0

wait_io:
    out 0x80, al
    ret

; offset of 1st pic is going to be 0x20
; offset of 2nd pic is going to be 0x28
reprogram_pic:
    push eax
    in al, pic1_data
    mov [mask_pic1], al
    in al, pic2_data
    mov [mask_pic2], al

    mov al, ICW1_INIT | ICW1_ICW4
    out pic1_command, al
    call wait_io
    out pic2_command, al
    call wait_io

    mov al, 0x20
    out pic1_data, al
    call wait_io
    mov al, 0x28
    out pic2_data, al
    call wait_io

    mov al, 4
    out pic1_data, al
    call wait_io
    mov al, 2
    out pic2_data, al
    call wait_io

    mov al, ICW4_8086
    out pic1_data, al
    call wait_io
    out pic2_data, al
    call wait_io

    mov al, [mask_pic1]
    out pic1_data, al
    mov al, [mask_pic2]
    out pic2_data, al

    pop eax

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
    mov al,0x20
    out pic1_command,al
    pop eax
    iret

empty_irq_pic_2:
    push eax
    mov al,0x20
    out pic1_command,al
    out pic2_command,al
    pop eax
    iret
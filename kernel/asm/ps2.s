PS2_DATA equ 0x60
PS2_STATUS equ 0x64     ; when read from
PS2_COMMAND equ 0x64    ; when writen to

extern terminal_writestring

extern wait_io
extern is_ps2_present
global init_ps2
global ps2_status_byte

; bit 0: 1 if ps2 controller is working, 0 otherwise
; bit 1: 1 if port 1 is working, 0 otherwise
; bit 2: 1 if port 2 si working, 0 otherwise
; bit 3: 1 if there is a device on port 1, 0 otherwise
; bit 4: 1 if there is a device on port 2, 0 otherwise
ps2_status_byte: db 0

; bit 0 of the status register must be set before attempting read
wait_for_read:
    push eax
wait_for_read_loop:
    in al, PS2_STATUS
    test al, 00000001b
    jz wait_for_read_loop
    pop eax
    ret

; bit 1 of the status register must be clear before attempting read
wait_for_write:
    push eax
wait_for_write_loop:
    in al, PS2_STATUS
    test al, 00000010b
    jnz wait_for_write_loop
    pop eax
    ret

init_device:
    mov ah, 0xff
init_device_loop_1:
    in al, PS2_STATUS
    test al, 00000010b
    jz init_device_send_command
    dec ah
    test ah, ah
    jnz init_device_loop_1
    jmp init_device_failed

init_device_send_command:
    mov al, 0xff
    out PS2_DATA, al
    call wait_for_read
    in al, PS2_DATA
    cmp al, 0xfc
    je init_device_failed
    push ebx
    mov bh, al
    call wait_for_read
    in al, PS2_DATA
    mov bl, al
init_device_loop_2:
    in al, PS2_STATUS
    test al, 00000010b
    jz init_device_type_continue
    dec ah
    test ah, ah
    jnz init_device_loop_2
    mov eax, 0xfe
init_device_type_continue:
    cmp bx, 0xfaaa
    je init_device_success
    cmp bx, 0xaafa
    je init_device_success
    pop ebx
    jmp init_device_failed
init_device_success:
    pop ebx
    ret
init_device_failed:
    mov eax, 0
    ret

init_ps2:
    cli
    push eax
    mov eax, 0
    ; init usb controller (absolutely not)
    ; check if ps2 controller exists
    call is_ps2_present
    test eax, 1
    jz init_ps2_end
    ; disable devices
    mov al, 0xad
    out PS2_COMMAND, al
    call wait_io
    mov al, 0xa7
    out PS2_COMMAND, al
    call wait_io
    ; flush output buffer
    in al, PS2_DATA
    ; set controller config byte
    mov al, 0x20
    out PS2_COMMAND, al
    call wait_for_read
    in al, PS2_DATA
    and al, 10001100b
    call wait_for_write
    mov ah, al
    out PS2_DATA, al
    call wait_io
    mov al, 0x60
    out PS2_COMMAND, al
    call wait_io
    ; perform controller self test
    mov al, 0xaa
    out PS2_COMMAND, al
    call wait_for_read
    in al, PS2_DATA
    cmp al, 0x55
    jne init_ps2_end
    mov al, ah
    call wait_for_write
    out PS2_DATA, al
    call wait_io
    mov al, 0x60
    out PS2_COMMAND, al
    call wait_io
    ; check if there are 2 channels
    mov al, 0xab
    out PS2_COMMAND, al
    call wait_io
    mov al, 0x20
    out PS2_COMMAND, al
    call wait_for_read
    in al, PS2_DATA
    test al, 100000b
    jz init_ps2_check_continue
    mov al, 101b
    mov [ps2_status_byte], al
init_ps2_check_continue:
    ; perform interfaces tests
    mov al, 0xab
    out PS2_COMMAND, al
    call wait_for_read
    in al, PS2_DATA
    cmp al, 0
    jne init_ps2_no_port_1
    or byte [ps2_status_byte], 010b
init_ps2_no_port_1:
    mov al, 0xa9
    out PS2_COMMAND, al
    call wait_for_read
    in al, PS2_DATA
    cmp al, 0
    je init_ps2_port_2_check_end
    and byte [ps2_status_byte], 011b
init_ps2_port_2_check_end:
    ; enable devices
    
    mov al, 0xae
    out PS2_COMMAND, al
    call wait_io
    mov al, 0xa8
    out PS2_COMMAND, al
    call wait_io
    mov al, 0x20
    out PS2_COMMAND, al
    call wait_for_read
    in al, PS2_DATA
    or al, 11b
    call wait_for_write
    out PS2_DATA, al
    call wait_io
    mov al, 0x60
    out PS2_COMMAND, al
    call wait_io
    ; reset devices
    test byte [ps2_status_byte], 10b
    jz init_ps2_no_device_1
    mov al, 0xa7
    out PS2_COMMAND, al
    call wait_io
    mov al, 0xae
    out PS2_COMMAND, al
    call wait_io
    ;call init_device
    ;test eax, eax
    ;jz init_ps2_no_device_1
    or byte [ps2_status_byte], 1000b
init_ps2_no_device_1:
    ;reset device 2
    test byte [ps2_status_byte], 100b
    jz init_ps2_end
    mov al, 0xa8
    out PS2_COMMAND, al
    call wait_io
    mov al, 0xad
    out PS2_COMMAND, al
    call wait_io
    ;call init_device
    ;test eax, eax
    ;jz init_ps2_no_device_2
    or byte [ps2_status_byte], 10000b
init_ps2_no_device_2:
    test byte [ps2_status_byte], 10b
    jz init_ps2_end
    mov al, 0xae
    out PS2_COMMAND, al
init_ps2_end:
    pop eax
    sti
    ret

debug_string: db "Debugging...", 0
  idt_address equ 0x7e000

  pic1_cmd equ 0x20
  pic1_data equ 0x21
  pic2_cmd equ 0xa0
  pic2_data equ 0xa1

  pic_eoi equ 0x20

io_wait:
  push ax
  push dx
  mov ax, 0
  mov dx, 0x80
  out dx, al
  pop dx
  pop ax
  ret

  ;; al: interrupt code
PIC_sendEOI:
  push ax
  push bx
  push dx
  mov bl, al
  mov dx, pic1_cmd
  mov al, pic_eoi
  out dx, al
  cmp bl, 0x8
  jl PIC_sendEOI_end
  mov dx, pic2_cmd
  out dx, al

PIC_sendEOI_end:
  pop dx
  pop bx
  pop ax
  ret


populate_idt:
  mov edx, idt_callbacks
  mov ebx, idt_address
populate_idt_loopdi_loop:
 	mov eax, placeholder_idt_entry
	mov ecx, 8
	call memcpy

  mov eax, [edx]
  mov [ebx], ax
  add ebx, 6
  shl eax, 16
  mov [ebx], ax
  add ebx, 2
  add edx, 4

	inc word [idt_counter]
	cmp word [idt_counter], 256
	jne populate_idt_loopdi_loop

	ret

init_pic:
  ;; save pic masks
  mov dx, pic1_data
  in al, dx
  mov [pic1_mask], al
  mov dx, pic2_data
  in al, dx
  mov [pic2_mask], al

  ;; send initialise cmd
  mov dx, pic1_cmd
  mov al, 0x11
  out dx, al
  call io_wait
  mov dx, pic2_cmd
  out dx, al
  call io_wait

  ;; send irq offsets
  mov al, 0x20
  mov dx, pic1_data
  out dx, al
  call io_wait
  mov al, 0x28
  mov dx, pic2_data
  out dx, al
  call io_wait

  ;; set up master/slave relation between chip 1 and 2
  mov al, 0x4
  mov dx, pic1_data
  out dx, al
  call io_wait
  mov al, 0x2
  mov dx, pic2_data
  out dx, al
  call io_wait

  ;; set chip mode (not sure what this means)
  mov al, 0x1
  mov dx, pic1_data
  out dx, al
  call io_wait
  mov dx, pic2_data
  out dx, al
  call io_wait

  ;; restore pic masks
  mov al, [pic1_mask]
  mov dx, pic1_data
  out dx, al
  mov al, [pic2_mask]
  mov dx, pic2_data
  out dx, al

  ret

placeholder_isr:
  push eax
  mov al, '0'
  call putchar
  pop eax
  iret

placeholder_idt_entry:
  dw placeholder_isr
	dw 0x20
	db 00000000b                  ; bits 0..2 holds Interrupt Stack Table offset, rest of bits zero. 
	db 10001110b                  ;	type and attributes
	dw 0x0
	dd 0x0
	dd 0x0

placeholder_irq_isr:
  push eax
	mov al, '1'
	call putchar
  pop eax
  call PIC_sendEOI
  iret

clock_irq:
  call PIC_sendEOI
  iret

ata_irq:
  mov byte [ata_flag], 0x1
  call PIC_sendEOI
  iret

idt_descriptor:
  dw 256*16
  dd idt_address
idt_counter:  dw 0x0
pic1_mask:  db 0x0
pic2_mask:  db 0x0
ata_flag: db 0x0

idt_callbacks: times 32  dd placeholder_isr
  dd clock_irq
  dd placeholder_irq_isr
  dd placeholder_irq_isr
  dd placeholder_irq_isr
  dd placeholder_irq_isr
  dd placeholder_irq_isr
  dd placeholder_irq_isr
  dd placeholder_irq_isr
  dd placeholder_irq_isr
  dd placeholder_irq_isr
  dd placeholder_irq_isr
  dd placeholder_irq_isr
  dd placeholder_irq_isr
  dd placeholder_irq_isr
  dd ata_irq
  dd placeholder_irq_isr
  ;; 	times 16 dd placeholder_irq_isr
  times 208 dd placeholder_isr

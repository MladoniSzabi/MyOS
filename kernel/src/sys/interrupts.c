#include "sys/interrupts.h"
#include "sys/debug.h"

#define IDT_MAX_DESCRIPTORS 256

#define PIC1 0x20 /* IO base address for master PIC */
#define PIC2 0xA0 /* IO base address for slave PIC */
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)

extern void __attribute__((cdecl)) load_idt(idtr_t *);
extern void __attribute__((cdecl)) load_isr(void *);
extern void __attribute__((cdecl)) stub_isr();
extern void __attribute__((cdecl)) stub_isr_argument();
extern void __attribute__((cdecl)) empty_irq_pic_1();
extern void __attribute__((cdecl)) empty_irq_pic_2();
extern void *isr_stub_table[];

__attribute__((aligned(0x10))) static idt_entry_t idt[IDT_MAX_DESCRIPTORS];
static idtr_t idtr;
volatile void (*isr_callbacks[IDT_MAX_DESCRIPTORS])();

void __attribute__((cdecl)) exception_handler()
{
    kernel_panic("An exception has occured.");
    __asm__("iret");
}

void __attribute__((cdecl)) exception_handler_argument(uint32_t err_code)
{
    printhex(err_code);
    println();
    kernel_panic("An exception has occured.");
    __asm__("iret");
}

void idt_set_descriptor(int vector, void *isr, uint8_t flags)
{
    idt_entry_t *descriptor = &idt[vector];

    descriptor->isr_low = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs = 0x10; // this value can be whatever offset your kernel code selector is in your GDT
    descriptor->attributes = flags;
    descriptor->isr_high = (uint32_t)isr >> 16;
    descriptor->reserved = 0;
}

void setup_idt()
{
    idtr.base = (uint32_t)&idt[0];
    idtr.limit = (uint16_t)(sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1);

    load_idt(&idtr);
}

void set_isr(unsigned int interrupt_id, void *callback)
{
    isr_callbacks[interrupt_id] = callback;
}

void enable_interrupts()
{
    // Mark all interrupts as unused and only enable the ones we plan to use
    for (int i = 0; i < IDT_MAX_DESCRIPTORS; i++)
    {
        idt_set_descriptor(i, exception_handler, 0x0E);
    }

    idt_set_descriptor(0, exception_handler, 0x8E);
    idt_set_descriptor(1, exception_handler, 0x8E);
    idt_set_descriptor(2, exception_handler, 0x8E);
    idt_set_descriptor(3, exception_handler, 0x8E);
    idt_set_descriptor(4, exception_handler, 0x8E);
    idt_set_descriptor(5, exception_handler, 0x8E);
    idt_set_descriptor(6, exception_handler, 0x8E);
    idt_set_descriptor(7, exception_handler, 0x8E);
    idt_set_descriptor(8, exception_handler_argument, 0x8E);
    idt_set_descriptor(9, exception_handler, 0x8E);
    idt_set_descriptor(10, exception_handler_argument, 0x8E);
    idt_set_descriptor(11, exception_handler_argument, 0x8E);
    idt_set_descriptor(12, exception_handler_argument, 0x8E);
    idt_set_descriptor(13, exception_handler_argument, 0x8E);
    idt_set_descriptor(14, exception_handler_argument, 0x8E);
    idt_set_descriptor(15, exception_handler, 0x8E);
    idt_set_descriptor(16, exception_handler, 0x8E);
    idt_set_descriptor(17, exception_handler_argument, 0x8E);
    idt_set_descriptor(18, exception_handler, 0x8E);
    idt_set_descriptor(19, exception_handler, 0x8E);
    idt_set_descriptor(20, exception_handler, 0x8E);
    idt_set_descriptor(21, exception_handler, 0x8E);
    idt_set_descriptor(22, exception_handler, 0x8E);
    idt_set_descriptor(23, exception_handler, 0x8E);
    idt_set_descriptor(24, exception_handler, 0x8E);
    idt_set_descriptor(25, exception_handler, 0x8E);
    idt_set_descriptor(26, exception_handler, 0x8E);
    idt_set_descriptor(27, exception_handler, 0x8E);
    idt_set_descriptor(28, exception_handler, 0x8E);
    idt_set_descriptor(29, exception_handler, 0x8E);
    idt_set_descriptor(30, exception_handler_argument, 0x8E);
    idt_set_descriptor(31, exception_handler, 0x8E);

    for (int i = 32; i < 32 + 8; i++)
        idt_set_descriptor(i, empty_irq_pic_1, 0x8E);

    for (int i = 32 + 8; i < 32 + 16; i++)
        idt_set_descriptor(i, empty_irq_pic_2, 0x8E);

    setup_idt();
}
#include "sys/interrupts.h"
#include "sys/debug.h"
#include "sys/keyboard.h"
#include "sys/cpu_exceptions.h"

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
}

void __attribute__((cdecl)) exception_handler_argument(uint32_t err_code)
{
    printhex(err_code);
    println();
    kernel_panic("An exception has occured.");
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

    idt_set_descriptor(0, cpu_exception_division_error, 0x8E);
    idt_set_descriptor(1, cpu_exception_debug, 0x8E);
    idt_set_descriptor(2, cpu_exception_non_maskable_interrupt, 0x8E);
    idt_set_descriptor(3, cpu_exception_breakpoint, 0x8E);
    idt_set_descriptor(4, cpu_exception_overflow, 0x8E);
    idt_set_descriptor(5, cpu_exception_bound_range_exceeded, 0x8E);
    idt_set_descriptor(6, cpu_exception_invalid_opcode, 0x8E);
    idt_set_descriptor(7, cpu_exception_device_not_available, 0x8E);
    idt_set_descriptor(8, cpu_exception_double_fault, 0x8E);
    idt_set_descriptor(9, cpu_exception_coprocessor_segment_overrun, 0x8E);
    idt_set_descriptor(10, cpu_exception_invalid_tss, 0x8E);
    idt_set_descriptor(11, cpu_exception_segment_not_present, 0x8E);
    idt_set_descriptor(12, cpu_exception_stack_segment_fault, 0x8E);
    idt_set_descriptor(13, cpu_exception_general_protection_fault, 0x8E);
    idt_set_descriptor(14, cpu_exception_page_fault, 0x8E);
    idt_set_descriptor(15, cpu_exception_reserved, 0x8E);
    idt_set_descriptor(16, cpu_exception_x87_floating_point_exception, 0x8E);
    idt_set_descriptor(17, cpu_exception_alignment_check, 0x8E);
    idt_set_descriptor(18, cpu_exception_machine_check, 0x8E);
    idt_set_descriptor(19, cpu_exception_simd_floating_point_exception, 0x8E);
    idt_set_descriptor(20, cpu_exception_virtualization_exception, 0x8E);
    idt_set_descriptor(21, cpu_exception_control_protection_exception, 0x8E);
    idt_set_descriptor(22, cpu_exception_reserved, 0x8E);
    idt_set_descriptor(23, cpu_exception_reserved, 0x8E);
    idt_set_descriptor(24, cpu_exception_reserved, 0x8E);
    idt_set_descriptor(25, cpu_exception_reserved, 0x8E);
    idt_set_descriptor(26, cpu_exception_reserved, 0x8E);
    idt_set_descriptor(27, cpu_exception_reserved, 0x8E);
    idt_set_descriptor(28, cpu_exception_hypervisor_injection_exception, 0x8E);
    idt_set_descriptor(29, cpu_exception_vmm_communication_exception, 0x8E);
    idt_set_descriptor(30, cpu_exception_security_exception, 0x8E);
    idt_set_descriptor(31, cpu_exception_reserved, 0x8E);

    for (int i = 32; i < 32 + 8; i++)
        idt_set_descriptor(i, empty_irq_pic_1, 0x8E);

    for (int i = 32 + 8; i < 32 + 16; i++)
        idt_set_descriptor(i, empty_irq_pic_2, 0x8E);

    idt_set_descriptor(33, keyboard_interrupt, 0x8E);
    idt_set_descriptor(34, exception_handler, 0x8E);

    setup_idt();
}
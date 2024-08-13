#include "sys/cpu_exceptions.h"

void __attribute__((cdecl)) cpu_exception_division_error()
{
    kernel_panic("There was a Division Error");
}

void __attribute__((cdecl)) cpu_exception_debug()
{
    kernel_panic("There was a Debug");
}

void __attribute__((cdecl)) cpu_exception_non_maskable_interrupt()
{
    kernel_panic("There was a Non-maskable Interrupt");
}

void __attribute__((cdecl)) cpu_exception_breakpoint()
{
    kernel_panic("There was a Breakpoint");
}

void __attribute__((cdecl)) cpu_exception_overflow()
{
    kernel_panic("There was a Overflow");
}

void __attribute__((cdecl)) cpu_exception_bound_range_exceeded()
{
    kernel_panic("There was a Bound Range Exceeded");
}

void __attribute__((cdecl)) cpu_exception_invalid_opcode()
{
    kernel_panic("There was a Invalid Opcode");
}

void __attribute__((cdecl)) cpu_exception_device_not_available()
{
    kernel_panic("There was a Device Not Available");
}

void __attribute__((cdecl)) cpu_exception_double_fault(uint32_t code)
{
    kernel_panic("There was a Double Fault");
}

void __attribute__((cdecl)) cpu_exception_coprocessor_segment_overrun()
{
    kernel_panic("There was a Coprocessor Segment Overrun");
}

void __attribute__((cdecl)) cpu_exception_invalid_tss(uint32_t code)
{
    kernel_panic("There was a Invalid TSS");
}

void __attribute__((cdecl)) cpu_exception_segment_not_present(uint32_t code)
{
    kernel_panic("There was a Segment Not Present");
}

void __attribute__((cdecl)) cpu_exception_stack_segment_fault(uint32_t code)
{
    kernel_panic("There was a Stack-Segment Fault");
}

void __attribute__((cdecl)) cpu_exception_general_protection_fault(uint32_t code)
{
    kernel_panic("There was a General Protection Fault");
}

void __attribute__((cdecl)) cpu_exception_page_fault(uint32_t code)
{
    kernel_panic("There was a Page Fault");
}

void __attribute__((cdecl)) cpu_exception_reserved()
{
    kernel_panic("There was a Reserved");
}

void __attribute__((cdecl)) cpu_exception_x87_floating_point_exception()
{
    kernel_panic("There was a x87 Floating-Point Exception");
}

void __attribute__((cdecl)) cpu_exception_alignment_check(uint32_t code)
{
    kernel_panic("There was a Alignment Check");
}

void __attribute__((cdecl)) cpu_exception_machine_check()
{
    kernel_panic("There was a Machine Check");
}

void __attribute__((cdecl)) cpu_exception_simd_floating_point_exception()
{
    kernel_panic("There was a SIMD Floating-Point Exception");
}

void __attribute__((cdecl)) cpu_exception_virtualization_exception()
{
    kernel_panic("There was a  Virtualization Exception");
}

void __attribute__((cdecl)) cpu_exception_control_protection_exception(uint32_t code)
{
    kernel_panic("There was a Control Protection Exception");
}

void __attribute__((cdecl)) cpu_exception_hypervisor_injection_exception()
{
    kernel_panic("There was a Hypervisor Injection Exception");
}

void __attribute__((cdecl)) cpu_exception_vmm_communication_exception(uint32_t code)
{
    kernel_panic("There was a VMM Communication Exception");
}

void __attribute__((cdecl)) cpu_exception_security_exception(uint32_t code)
{
    kernel_panic("There was a Security Exception");
}
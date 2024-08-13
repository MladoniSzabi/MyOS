#pragma once

#include "stdint.h"

#include "sys/debug.h"

void __attribute__((cdecl)) cpu_exception_division_error();
void __attribute__((cdecl)) cpu_exception_debug();
void __attribute__((cdecl)) cpu_exception_non_maskable_interrupt();
void __attribute__((cdecl)) cpu_exception_breakpoint();
void __attribute__((cdecl)) cpu_exception_overflow();
void __attribute__((cdecl)) cpu_exception_bound_range_exceeded();
void __attribute__((cdecl)) cpu_exception_invalid_opcode();
void __attribute__((cdecl)) cpu_exception_device_not_available();
void __attribute__((cdecl)) cpu_exception_double_fault(uint32_t code);
void __attribute__((cdecl)) cpu_exception_coprocessor_segment_overrun();
void __attribute__((cdecl)) cpu_exception_invalid_tss(uint32_t code);
void __attribute__((cdecl)) cpu_exception_segment_not_present(uint32_t code);
void __attribute__((cdecl)) cpu_exception_stack_segment_fault(uint32_t code);
void __attribute__((cdecl)) cpu_exception_general_protection_fault(uint32_t code);
void __attribute__((cdecl)) cpu_exception_page_fault(uint32_t code);
void __attribute__((cdecl)) cpu_exception_reserved();
void __attribute__((cdecl)) cpu_exception_x87_floating_point_exception();
void __attribute__((cdecl)) cpu_exception_alignment_check(uint32_t code);
void __attribute__((cdecl)) cpu_exception_machine_check();
void __attribute__((cdecl)) cpu_exception_simd_floating_point_exception();
void __attribute__((cdecl)) cpu_exception_virtualization_exception();
void __attribute__((cdecl)) cpu_exception_control_protection_exception(uint32_t code);
void __attribute__((cdecl)) cpu_exception_hypervisor_injection_exception();
void __attribute__((cdecl)) cpu_exception_vmm_communication_exception(uint32_t code);
void __attribute__((cdecl)) cpu_exception_security_exception(uint32_t code);

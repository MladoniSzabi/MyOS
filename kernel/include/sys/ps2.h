#pragma once

#include <stdint.h>
#include <stdbool.h>

extern uint8_t ps2_status_byte;

extern void __attribute__((cdecl)) init_ps2();
bool __attribute__((cdecl)) is_ps2_present();
#include <stdint.h>

#include "sys/debug.h"

extern uint8_t keyboard_buffer_end;
extern uint8_t keyboard_buffer[32];
uint8_t keyboard_buffer_start = 0;

void process_keyboard()
{
    if (keyboard_buffer_start == keyboard_buffer_end)
        return;
}

void __attribute__((cdecl)) c_keyboard_interrupt(uint16_t keycode)
{
    printhex_digits(keycode, 16);
    printspc();
}
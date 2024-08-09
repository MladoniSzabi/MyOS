#pragma once

void __attribute__((cdecl)) keyboard_interrupt();
void __attribute__((cdecl)) c_keyboard_interrupt(uint16_t keycode);
void process_keyboard();
#pragma once

#include <stdint.h>

void printhex(uint64_t num);
void printhex_digits(uint64_t num, unsigned int digits);
void printspc();
void println();
void kernel_panic(const char *msg);
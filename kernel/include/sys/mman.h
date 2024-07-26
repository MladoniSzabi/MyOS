#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void* mmap(void *addr, size_t length, int prot, int flags, int fd, unsigned int offset);
int munmap(void *addr, size_t length);

#ifdef __cplusplus
}
#endif
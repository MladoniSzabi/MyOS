#include <string.h>

void* memcpy(void* destination, const void* source, size_t num) {
	for(size_t i = 0; i < num; i++)
		((char*)destination)[i] = ((char*)source)[i];
	return destination;
}

void* memset(void* ptr, int value, size_t num) {
	for(size_t i = 0; i < num; i++)
		((char*)ptr)[i] = (char)value;
	return ptr;
}
char* strcpy(char* destination, const char* source) {return 0;}

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len] != 0)
		len++;
	return len;
}

#include <stdio.h>
#include<stdarg.h>

int fclose(FILE* file) {return 0;}
int fflush(FILE* file) {return 0;}
FILE* fopen(const char* name, const char* params) {return 0;}
int fprintf(FILE* file, const char* format, ...) {return 0;}
size_t fread(void* buffer, size_t size, size_t count, FILE* file) {return 0;}
int fseek(FILE* file, long offset, int origin) {return 0;}
long ftell(FILE* file) {return 0;}
size_t fwrite(const void* buffer, size_t size, size_t count, FILE* file) {return 0;}
void setbuf(FILE* file, char* buffer) {}
int vfprintf(FILE* file, const char* format, va_list args) {return 0;}

int printf(const char* format, ...) {
	va_list ap;
	
}
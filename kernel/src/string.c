#include <string.h>

void *memcpy(void *destination, const void *source, size_t num)
{
	for (size_t i = 0; i < num; i++)
		((char *)destination)[i] = ((char *)source)[i];
	return destination;
}

void *memset(void *ptr, int value, size_t num)
{
	for (size_t i = 0; i < num; i++)
		((char *)ptr)[i] = (char)value;
	return ptr;
}
char *strcpy(char *destination, const char *source)
{
	char *sdest = destination;
	while (*source != 0)
	{
		*destination = *source;
		destination++;
		source++;
	}
	return sdest;
}

size_t strlen(const char *str)
{
	size_t len = 0;
	while (str[len] != 0)
		len++;
	return len;
}

int strcmp(const char *lhs, const char *rhs)
{
	while (*lhs != 0)
	{
		if (*rhs == 0)
			return -1;

		if (*lhs < *rhs)
			return -1;

		if (*lhs > *rhs)
			return 1;

		lhs++;
		rhs++;
	}

	if (*rhs == 0)
		return 0;

	return 1;
}

int strncmp(const char *lhs, const char *rhs, size_t size)
{
	while (size > 0)
	{
		if (*rhs == 0 && *lhs == 0)
			return 0;

		if (*lhs < *rhs)
			return -1;

		if (*lhs > *rhs)
			return 1;

		lhs++;
		rhs++;
		size--;
	}

	return 0;
}
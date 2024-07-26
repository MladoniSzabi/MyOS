#include <stdlib.h>
#include <stdbool.h>

void abort(void) {}
int atexit(void (*cb)(void)) { return 0; }

int atoi(const char *str)
{
    int retval;
    bool isNegative = false;
    size_t index = 0;

    if (str[0] == '-')
    {
        isNegative = true;
        index += 1;
    }

    while (1)
    {
        char digit = str[index] - '0';
        if (digit < 0 || digit > 9)
            break;
        retval = retval * 10 + digit;
    }

    if (isNegative)
        retval = -retval;

    return retval;
}

void *malloc(size_t count)
{
}

void free(void *ptr) {}
char *getenv(const char *name) { return 0; }
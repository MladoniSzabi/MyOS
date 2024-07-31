#include <unistd.h>

int execv(const char *path, char *const args[]) { return 0; }
int execve(const char *path, char *const args[], char *const env[]) { return 0; }
int execvp(const char *path, char *const args[]) { return 0; }
pid_t fork(void) { return 0; }
long syscall(long number, ...)
{
    __asm__(
        "syscall");
}
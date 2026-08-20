/*
 * Minimal newlib syscall stubs.
 *
 * Defining these here (rather than relying on --specs=nosys.specs) keeps
 * newlib-nano's own reentrant stub objects out of the link, which avoids
 * the .gnu.warning-triggered "not implemented and will always fail"
 * link-time warnings that newer newlib versions bake into those objects.
 * -Wl,-fatal-warnings then stays effective for catching real problems.
 */

#include <errno.h>
#include <sys/stat.h>

int _close(int file)
{
    (void)file;
    errno = ENOSYS;
    return -1;
}

int _fstat(int file, struct stat *st)
{
    (void)file;
    st->st_mode = S_IFCHR;
    return 0;
}

int _getpid(void)
{
    return 1;
}

int _isatty(int file)
{
    (void)file;
    return 1;
}

int _kill(int pid, int sig)
{
    (void)pid;
    (void)sig;
    errno = ENOSYS;
    return -1;
}

int _lseek(int file, int ptr, int dir)
{
    (void)file;
    (void)ptr;
    (void)dir;
    return 0;
}

int _read(int file, char *ptr, int len)
{
    (void)file;
    (void)ptr;
    (void)len;
    return 0;
}

int _write(int file, char *ptr, int len)
{
    (void)file;
    (void)ptr;
    return len;
}

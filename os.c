#if __APPLE__
#define daemon daemondeprecatedhack
#endif

#include <unistd.h>

#if __APPLE__
#undef daemon
extern int daemon(int, int);
#endif

int os_daemon(int nochdir, int noclose)
{
    return daemon(nochdir, noclose);
}




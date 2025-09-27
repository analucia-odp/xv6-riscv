#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char *argv[]) {
    int syscall = atoi(argv[1]);
    int calls = getcnt(syscall);
    printf("syscall %d has been called %d times\n", syscall, calls);
    return 0;
}
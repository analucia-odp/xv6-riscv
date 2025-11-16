#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char *argv[]) {
    if (argc < 2){
        printf("[getcnt] Error - provided syscall number\n");
        exit(1);
    }

    int syscall = atoi(argv[1]);
    if (syscall <= 0){
        printf("[getcnt] Error - syscall number must be greater than 0\n");
        exit(1);
    }
    int calls = getcnt(syscall);
    printf("syscall %d has been called %d times\n", syscall, calls);
    return 0;
}
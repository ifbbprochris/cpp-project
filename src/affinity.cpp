#include <iostream>
#include <unistd.h>
#include <sys/syscall.h>

#include <sched.h>

#define __GNU_SOURCE

int process_affinity(int num) {
    pid_t self_id = syscall(__NR_gettid);

    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(self_id % num, &mask);

    sched_setaffinity(self_id, sizeof(mask), &mask);

    while (1)
        ;

    return 0;
}

int main() {
    int num = sysconf(_SC_NPROCESSORS_CONF);

    printf("num: %d\n", num);

    pid_t pid = 0;

    for (int i = 0; i < 2; i++) {
        pid = fork();

        if (pid <= 0) { break; }
    }

    if (pid == 0) { process_affinity(num); }

    printf("affinity.cpp  %d\n", pid);

    while (true) { usleep(1); }

    return 0;
}
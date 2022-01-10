#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

int main() {
    struct timeval tv;
    int half_sec = 1;
    while (1) {
        while (gettimeofday(&tv, NULL) == 0 && tv.tv_usec / 500000 < half_sec) ;
        printf("Now, time is %d * half_sec\n", half_sec);
        half_sec ++;
    }

    return 0;
}

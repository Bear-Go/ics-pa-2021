#include <stdio.h>
#include <NDL.h>

int main() {
    int sec = 1;
    NDL_Init(0);
    while (1) {
        while (NDL_GetTicks() / 1000 < sec) ;
        printf("Now, time is %d * half_sec\n", sec);
        sec ++;
    }

    return 0;
}

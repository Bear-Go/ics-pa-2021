#include <stdio.h>
#include <NDL.h>

int main() {
    int half_sec = 1;
    while (1) {
        while (NDL_GetTicks() / 500000 < half_sec) ;
        printf("Now, time is %d * half_sec\n", half_sec);
        half_sec ++;
    }

    return 0;
}

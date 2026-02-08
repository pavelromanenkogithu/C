#include <stdio.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

#define IN(v2, v1, mul1, mul2, div) \
    (((double)(v1)*(mul1) + (v2)*(mul2)) / (div))

long now_us() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000L + ts.tv_nsec / 1000;
}

int main() {
    long start = now_us();
    long duration = 5 * 1000000L; // 5 секунд

    int xs = 300, xe = 750;
    int ys = 400, ye = 300;
    int sxs = 100, sxe = 300;
    int sys = 100, sye = 300;

    while (1) {
        long t = now_us() - start;
        if (t > duration) break;

        int mul1 = t >> 8;
        int mul2 = (duration - t) >> 8;
        int div  = duration >> 8;

        double cx = IN(xs, xe, mul1, mul2, div)
                  + sin(t / 300000.0) * 40;

        double cy = IN(ys, ye, mul1, mul2, div)
                  + cos(t / 500000.0) * 40;

        double sx = IN(sxs, sxe, mul1, mul2, div)
                  + sin(t / 520000.0) * 70;

        double sy = IN(sys, sye, mul1, mul2, div)
                  + cos(t / 700000.0) * 70;

        printf(
            "\rcx=%6.1f cy=%6.1f | sx=%6.1f sy=%6.1f",
            cx, cy, sx, sy
        );
        fflush(stdout);

        usleep(16000); // ~60 FPS
    }

    printf("\nDone.\n");
    return 0;
}

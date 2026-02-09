#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 40
#define MAX_ITER 100

// Julia parameters
const double complex c = -0.7 + 0.27015*I;

void draw_julia() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            // Map coordinates to complex numbers
            double real = 3.0 * (x - WIDTH/2) / (WIDTH/2);
            double imag = 2.0 * (y - HEIGHT/2) / (HEIGHT/2);
            double complex z = real + imag*I;

            int iter = 0;
            while (cabs(z) < 2.0 && iter < MAX_ITER) {
                z = z*z + c;
                iter++;
            }

            // Choose character based on iteration count
            char ch;
            if (iter == MAX_ITER) ch = '#';
            else if (iter > MAX_ITER/2) ch = '*';
            else if (iter > MAX_ITER/4) ch = '.';
            else ch = ' ';

            putchar(ch);
        }
        putchar('\n');
    }
}

int main() {
    while (1) {
        system("clear"); // Clear screen for animation
        draw_julia();
        usleep(100000); // Delay 0.1 seconds
    }
    return 0;
}

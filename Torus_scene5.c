#include <stdio.h>
#include <math.h>
#include <unistd.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24

int main() {
    float A = 0, B = 0;
    float i, j;
    float z[SCREEN_WIDTH * SCREEN_HEIGHT];
    char b[SCREEN_WIDTH * SCREEN_HEIGHT];
    float theta_spacing = 0.07;
    float phi_spacing = 0.02;
    float R1 = 1, R2 = 2, K2 = 5;
    float K1 = SCREEN_WIDTH * K2 * 3 / (8 * (R1 + R2));

    while (1) {
        for (int k = 0; k < SCREEN_WIDTH * SCREEN_HEIGHT; k++) {
            b[k] = ' ';
            z[k] = 0;
        }

        for (j = 0; j < 2 * M_PI; j += phi_spacing) {
            for (i = 0; i < 2 * M_PI; i += theta_spacing) {
                float cosA = cos(A), sinA = sin(A);
                float cosB = cos(B), sinB = sin(B);
                float cosi = cos(i), sini = sin(i);
                float cosj = cos(j), sinj = sin(j);

                float circlex = R2 + R1 * cosi;
                float circley = R1 * sini;

                float x = circlex * (cosB * cosj + sinA * sinB * sinj) - circley * cosA * sinB;
                float y = circlex * (sinB * cosj - sinA * cosB * sinj) + circley * cosA * cosB;
                float zval = K2 + cosA * circlex * sinj + circley * sinA;
                float ooz = 1 / zval;

                int xp = (int)(SCREEN_WIDTH / 2 + K1 * ooz * x);
                int yp = (int)(SCREEN_HEIGHT / 2 - K1 * ooz * y);
                int idx = xp + yp * SCREEN_WIDTH;

                float L = cosj * cosi * sinB - cosA * cosi * sinj - sinA * sini + cosB * (cosA * sini - cosi * sinA * sinj);
                if (L > 0 && idx >= 0 && idx < SCREEN_WIDTH * SCREEN_HEIGHT) {
                    if (ooz > z[idx]) {
                        z[idx] = ooz;
                        int lumin = (int)(L * 8);
                        const char *chars = ".,-~:;=!*#$@";
                        b[idx] = chars[lumin > 11 ? 11 : lumin];
                    }
                }
            }
        }

        printf("\033[H");
        for (int k = 0; k < SCREEN_HEIGHT; k++) {
            for (int l = 0; l < SCREEN_WIDTH; l++) {
                putchar(b[l + k * SCREEN_WIDTH]);
            }
            putchar('\n');
        }

        A += 0.04;
        B += 0.02;

        usleep(30000);
    }

    return 0;
}

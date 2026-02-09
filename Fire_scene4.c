#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define W 80
#define H 25

unsigned char fire[H][W];

void init_fire() {
    for (int x = 0; x < W; x++)
        fire[H - 1][x] = rand() % 256;
}

void update_fire() {
    for (int y = 0; y < H - 1; y++) {
        for (int x = 1; x < W - 1; x++) {
            int v =
                fire[y + 1][x - 1] +
                fire[y + 1][x] +
                fire[y + 1][x + 1] +
                fire[y + 2 < H ? y + 2 : H - 1][x];

            fire[y][x] = v / 4 > 0 ? v / 4 - 1 : 0;
        }
    }
}

char pixel(unsigned char v) {
    const char *ramp = " .:-=+*#%@";
    return ramp[v / 26];
}

void draw_fire() {
    printf("\033[H");
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++)
            putchar(pixel(fire[y][x]));
        putchar('\n');
    }
}

int main() {
    srand(time(NULL));
    printf("\033[2J\033[?25l");

    while (1) {
        init_fire();
        update_fire();
        draw_fire();
        usleep(30000);
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>  // for usleep
#include <time.h>    // for srand

#define DWIDTH 80    // terminal width (characters)
#define DHEIGHT 24   // terminal height (lines)
#define ZOOMSTEP 0.01
#define MINZOOM 0.8
#define MAXZOOM 2.7
#define ZTABSIZE ((int)((MAXZOOM-MINZOOM)/ZOOMSTEP))
#define RAD(angle) ((double) angle * M_PI / 180)
#define RANDOM(x) (rand()%x)
#define N_FRAMERATE 15  // FPS
#define POW4(x) ((x)*(x)*(x)*(x))

static char *PlasmaTbl;
static unsigned char pos1, pos2, pos3, pos4;
static unsigned int pnum;
static int dir = 1;

// function to convert intensity to ASCII character
char ascii_color(int val) {
    const char* chars = " .:-=+*#%@";
    int idx = val * 10 / 256;
    if(idx < 0) idx = 0;
    if(idx > 9) idx = 9;
    return chars[idx];
}

// initialize plasma table
void init_scene(void) {
    int i;
    float czoom;
    for(czoom = MINZOOM, pnum = 0; pnum < ZTABSIZE; pnum++, czoom += ZOOMSTEP) {
        for(i = 0; i < 256; i++) {
            PlasmaTbl[(pnum << 8) + i] = (char)( ((float)cos(RAD(i*256/180))) * 256 / POW4(czoom) );
        }
    }
    pnum = 0;
}

// draw ASCII plasma frame
void draw_plasma(void) {
    int i, j, color;
    unsigned char p1, p2, p3 = pos3, p4 = pos4;
    for(i = 0; i < DWIDTH; i++) {
        p1 = pos1;
        p2 = pos2;
        for(j = 0; j < DHEIGHT; j++) {
            color = PlasmaTbl[(pnum << 8) + p1] + PlasmaTbl[(pnum << 8) + p2] +
                    PlasmaTbl[(pnum << 8) + p3] + PlasmaTbl[(pnum << 8) + p4] +
                    PlasmaTbl[(pnum << 8) + ((unsigned char)i)] +
                    PlasmaTbl[(pnum << 8) + ((unsigned char)j)];
            printf("%c", ascii_color(color));
            p1 += 3;
            p2 += 1;
        }
        printf("\n");
        p3 += 2;
        p4 += 3;
    }
}

// move plasma offsets
void move_plasma(void) {
    pos1 -= 4 + RANDOM(2);
    pos3 += 4 + RANDOM(1);
    pos2 -= RANDOM(2);
    pos4 -= RANDOM(2);

    pnum += dir;
    if(pnum > (ZTABSIZE - 2)) dir = -1;
    else if(pnum <= 0) dir = 1;
}

// main plasma loop
void scene(void) {
    pos1 = pos2 = pos3 = pos4 = pnum = dir = 0;
    PlasmaTbl = (char*)malloc(ZTABSIZE*256);
    if(!PlasmaTbl) { printf("Memory allocation error!\n"); return; }

    init_scene();

    for(int frame=0; frame<500; frame++) {  // number of frames
        printf("\033[H\033[J");  // ANSI escape to clear screen
        draw_plasma();
        move_plasma();
        usleep(1000000 / N_FRAMERATE);  // frame delay
    }

    free(PlasmaTbl);
}

int main(void) {
    srand(time(NULL));
    scene();
    return 0;
}

#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define FRAME_DELAY 30000   // microseconds
#define STROBE_FRAMES 20

static int screen_w, screen_h;

/* ---------- UTIL ---------- */

void center_print(int y, const char *text) {
    int x = (screen_w - strlen(text)) / 2;
    mvprintw(y, x, "%s", text);
}

char random_hex() {
    return (rand() % 2)
        ? 'A' + rand() % 6
        : '0' + rand() % 10;
}

/* ---------- EFFECTS ---------- */

void fake_loading() {
    clear();
    const char *msg = "Please wait. Precalculating data";
    for (int i = 0; i < screen_h * screen_w / 4; i++) {
        int x = rand() % screen_w;
        int y = rand() % screen_h;
        mvaddch(y, x, random_hex());
        center_print(screen_h / 2, msg);
        refresh();
        usleep(1500);
    }
}

void noise_effect(int frames) {
    for (int f = 0; f < frames; f++) {
        clear();
        for (int y = 0; y < screen_h; y++) {
            for (int x = 0; x < screen_w; x++) {
                if (rand() % 6)
                    mvaddch(y, x, random_hex());
            }
        }
        refresh();
        usleep(FRAME_DELAY);
    }
}

void strobe_flash() {
    for (int i = 0; i < STROBE_FRAMES; i++) {
        if (i % 2)
            attron(A_REVERSE);
        else
            attroff(A_REVERSE);

        clear();
        refresh();
        usleep(40000);
    }
    attroff(A_REVERSE);
}

void bouncing_letters() {
    float pos = screen_h * 2;
    float vel = -10.0f;

    for (int i = 0; i < 120; i++) {
        vel += (screen_h / 2 - pos) / 30.0f;
        vel *= 0.94f;
        pos += vel;

        clear();
        mvprintw((int)pos, screen_w / 4, "B");
        mvprintw(screen_h - (int)pos, 3 * screen_w / 4, "B");
        refresh();
        usleep(FRAME_DELAY);
    }
}

void presentation_text() {
    const char *text[] = {
        "THE",
        "100 %",
        "ANSI C",
        "PORTABLE",
        "ASCII",
        "DEMO",
        "TEXT MODE",
        "LINUX",
        "ANDROID",
        "TERMUX"
    };

    for (int i = 0; i < 10; i++) {
        strobe_flash();
        clear();
        center_print(screen_h / 2, text[i]);
        refresh();
        usleep(600000);
    }
}

/* ---------- MAIN ---------- */

int main() {
    srand(time(NULL));

    initscr();
    noecho();
    curs_set(FALSE);
    nodelay(stdscr, TRUE);

    getmaxyx(stdscr, screen_h, screen_w);

    fake_loading();
    noise_effect(40);
    strobe_flash();
    bouncing_letters();
    presentation_text();

    nodelay(stdscr, FALSE);
    mvprintw(screen_h - 1, 0, "Press any key to exit...");
    getch();

    endwin();
    return 0;
}

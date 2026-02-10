#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#define ETIME 50    // delay between frames in milliseconds
#define MAXPOS 20   // maximum text position

// Function to clear the screen and print text in the center
void centerprint(int row, const char *text) {
    int col = (COLS - strlen(text)) / 2;
    mvprintw(row, col, "%s", text);
}

// Moving text in "double pass" effect
void doublepass(int step, const char *text1, const char *text2) {
    int pos1 = step % (COLS + strlen(text1));
    int pos2 = step % (COLS + strlen(text2));

    mvprintw(LINES / 3, COLS - pos1, "%s", text1);
    mvprintw(2 * LINES / 3, -strlen(text2) + pos2, "%s", text2);
}

// Zoom effect
void drawzoomer(const char *text, int step) {
    int row = LINES / 2;
    int start_col = (COLS - strlen(text)) / 2;
    if (step > 0)
        mvprintw(row, start_col, "%s", text);
}

// Main scene
void scene2() {
    const char *messages[] = {
        "Future", "Crew", "Triton", "Cascada",
        "Complex", "Pascal", "Titans", "Xography",
        "Sonic PC", "Scrymag", "...", "Microsoft", "", "!?!"
    };

    // Display messages with zoom effect
    for (int i = 0; i < sizeof(messages)/sizeof(char*); i++) {
        clear();
        drawzoomer(messages[i], i);
        refresh();
        usleep(ETIME * 1000);
    }

    // Display "double pass" effect
    for (int step = 0; step < COLS; step++) {
        clear();
        doublepass(step, "Hello", "World");
        refresh();
        usleep(ETIME * 1000);
    }
}

int main() {
    initscr();       // start ncurses
    noecho();        // disable character echo
    curs_set(FALSE); // hide cursor

    scene2();

    endwin();        // exit ncurses
    return 0;
}

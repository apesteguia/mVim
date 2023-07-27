#include "window.h"
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

WINDOW *
new_window (char *file)
{
    WINDOW *win;
    int start_x, start_y;

    getmaxyx (stdscr, start_y, start_x);
    win = newwin (MAX_HEIGHT, start_x - 2, 0, 1);
    box (win, 0, 0);
    mvwprintw (win, 0, 5, file);
    scrollok (win, TRUE);
    wrefresh (win);
    refresh ();
    return win;
}
void
init_window ()
{
    initscr ();
    cbreak ();
    noecho ();
    start_color ();
    curs_set (0);
    init_pair (1, COLOR_WHITE, COLOR_BLACK);
    refresh ();
}
void
end_window (WINDOW *win)
{
    delwin (win);
    endwin ();
}
// "█"
//

void
load_file (WINDOW *win, char *path, int max_x)
{
    FILE *f;
    char c;
    int x = 1;
    int y = 2;

    f = fopen (path, "r");

    if (f == NULL)
        {
            mvwprintw (win, 0, 0,
                       "Error: File not found or cannot be opened.");
            wrefresh (win);
            return;
        }

    while ((c = fgetc (f)) != EOF)
        {

            if (c == '\n')
                {
                    y++;
                    x = 1;
                }
            else if (x > max_x - 5)
                {
                    x = 1;
                    y++;
                }

            mvwprintw (win, y, x, "%c", c);
            x++;
        }

    fclose (f);
    wrefresh (win);
}

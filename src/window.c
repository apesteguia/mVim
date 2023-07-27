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
    win = newwin (start_y, start_x - 2, 1, 1);
    mvprintw (1, 3, "%s", file);
    scrollok (win, TRUE);
    wsetscrreg (win, 0, start_y);
    box (win, 0, 0);
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
                    x = 0;
                }
            else if (x > max_x - 5)
                {
                    x = 1;
                    y++;
                    mvwprintw (win, y, x, "%c", c);
                }
            else if (c != '\n')
                {

                    mvwprintw (win, y, x, "%c", c);
                }

            x++;
        }

    fclose (f);
    wrefresh (win);
}

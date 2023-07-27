#include "window.h"
#include <curses.h>
#include <ncurses.h>
#include <unistd.h>

int
main (int argc, char **argv)
{
    WINDOW *win;
    int max_y, max_x, scrollPos = 0;
    char ch;

    init_window ();
    win = new_window (argv[1]);
    getmaxyx (win, max_y, max_x);

    load_file (win, argv[1], max_x);

    while ((ch = wgetch (win)) != 'q')
        {
            if (ch == 'j')
                {
                    wscrl (win, 1);
                }
            if (ch == 'k')
                {
                    wscrl (win, -1);
                }
        }

    end_window (win);
    return 0;
}

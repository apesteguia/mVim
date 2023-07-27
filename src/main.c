#include "window.h"
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

    sleep (2);

    end_window (win);
    return 0;
}

#include "window.h"
#include <curses.h>
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

int
main (int argc, char **argv)
{
    if (argc > 1 && argv[1] != NULL)
        main_loop (argc, argv[1]);
    return 0;
}

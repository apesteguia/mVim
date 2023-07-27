// window.h (No changes needed)

// window.c (No changes needed)

// main.c
#include "window.h"
#include <curses.h>
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

int
main (int argc, char **argv)
{
    WINDOW *win;
    Buffer *buf;
    int max_x, max_y, scroll_pos;

    max_x = max_y = scroll_pos = 0;

    if (argc == 1)
        {
            fputs ("less: No file to open\n", stderr);
            return 1;
        }

    init_window ();
    getmaxyx (stdscr, max_y, max_x); // Get terminal dimensions
    win = new_window (argv[1]);

    buf = load_file (win, argv[1], max_x);

    draw (win, buf, max_x, 0);
    int ch;

    while ((ch = getch ()) != 'q')
        {
            switch (ch)
                {
                case KEY_UP:
                    if (scroll_pos > 0)
                        {
                            scroll_pos--;
                            draw (win, buf, max_x, scroll_pos);
                        }
                    break;
                case KEY_DOWN:
                    if (scroll_pos + max_y < buf->n)
                        {
                            scroll_pos++;
                            draw (win, buf, max_x, scroll_pos);
                        }
                    break;
                default:
                    break;
                }
            getmaxyx (stdscr, max_y,
                      max_x); // Update terminal dimensions in case of resize
        }

    free_buffer (buf);
    end_window (win);
    return 0;
}

#include "window.h"
#include <curses.h>
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

int
main (int argc, char **argv)
{
    WINDOW *win, *explorer;
    Buffer *buf;
    int max_x, max_y, scroll_pos;

    max_x = max_y = scroll_pos = 0;

    if (argc == 1)
        {
            fputs ("less: No file to open\n", stderr);
            return 1;
        }

    init_window ();
    getmaxyx (stdscr, max_y, max_x);
    win = new_window (argv[1]);
    buf = load_file (win, argv[1], max_x);
    draw (win, buf, getmaxx (win), 0);
    print_keys (win);
    print_title (win, argv[1]);

    int ch = 'j';
    int ch2;
    while ((ch = getch ()) != 'q')
        {
            switch (ch)
                {
                case ' ':
                    scroll_pos = 0;
                    explorer = create_explorer (win);
                case 'k':
                    if (scroll_pos > 0)
                        {
                            // delete_explorer (explorer);
                            scroll_pos -= 1;
                            draw (win, buf, getmaxx (win), scroll_pos);
                        }
                    break;
                case 'K':
                    if (scroll_pos > 0)
                        {
                            // delete_explorer (explorer);
                            scroll_pos -= 10;
                            draw (win, buf, getmaxx (win), scroll_pos);
                        }
                    break;

                case 'j':
                    if (scroll_pos + getmaxy (win) - 1 < buf->n)
                        {
                            // delete_explorer (explorer);
                            scroll_pos += 1;
                            draw (win, buf, getmaxx (win), scroll_pos);
                        }
                    break;
                case 'J':
                    if (scroll_pos + getmaxy (win) - 1 < buf->n > 0)
                        {
                            // delete_explorer (explorer);
                            scroll_pos += 10;
                            draw (win, buf, getmaxx (win), scroll_pos);
                        }
                    break;

                case 'r':
                    delete_explorer (explorer);
                    endwin ();
                    refresh ();
                    win = new_window (argv[1]);
                    print_keys (win);
                    draw (win, buf, getmaxx (win), scroll_pos);
                    print_title (win, argv[1]);
                    break;
                default:
                    print_title (win, argv[1]);
                    print_keys (win);
                    break;
                }
        }

    free_buffer (buf);
    end_window (win);
    return 0;
}

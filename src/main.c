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
    print_title (win, argv[1]);

    buf = load_file (win, argv[1], max_x);

    draw (win, buf, max_x, 0);
    int ch;

    while ((ch = getch ()) != 'q')
        {
            if (ch == KEY_RESIZE)
                {
                    endwin ();
                    refresh ();
                    win = new_window (argv[1]);
                    print_title (win, argv[1]);
                    draw (win, buf, max_x,
                          scroll_pos); // Redraw the content after resizing
                }
            else
                {
                    switch (ch)
                        {
                        case KEY_UP:
                            if (scroll_pos > 0)
                                {
                                    scroll_pos -= 10;
                                    wscrl (win, -10); // Scroll the content up
                                    wrefresh (win);
                                }
                            break;
                        case KEY_DOWN:
                            if (scroll_pos + getmaxy (win) - 1 < buf->n)
                                {
                                    scroll_pos += 10;
                                    wscrl (win, 10); // Scroll the content down
                                    wrefresh (win);
                                }
                            break;
                        default:
                            print_title (win, argv[1]);
                            break;
                        }
                }
        }

    free_buffer (buf);
    end_window (win);
    return 0;
}

/*

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
    print_title (win, argv[1]);

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
                            scroll_pos -= 10;
                        }
                    break;
                case KEY_DOWN:
                    if (scroll_pos + max_y < buf->n)
                        {
                            scroll_pos += 10;
                        }
                    break;
                default:
                    print_title (win, argv[1]);
                    break;
                }

            if (scroll_pos < 0)
                {
                    scroll_pos = 0;
                }
            if (scroll_pos + max_y >= buf->n)
                {
                    scroll_pos = buf->n - max_y;
                }

            draw (win, buf, max_x, scroll_pos);
            getmaxyx (stdscr, max_y,
                      max_x); // Update terminal dimensions in case of resize
        }

    free_buffer (buf);
    end_window (win);
    return 0;
} */

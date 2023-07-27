#include "window.h"
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

int
main (int argc, char **argv)
{
    WINDOW *win;
    Buffer *buf;
    int max_x, max_y;

    max_x = max_y = 0;
    if (argc == 1)
        {
            fputs ("less: No file to open\n", stderr);
            return 1;
        }

    init_window ();
    win = new_window (argv[1]);

    load_file (win, buf, argv[1], max_x);

    draw (win, buf, max_x);

    sleep (2);
    int ch;
    /*while ((ch = getch ()) != EOF && ch != 'q')
        {
            if (ch == KEY_UP && buf->curr->prev != NULL)
                {
                    buf->curr_line--;
                    buf->curr = buf->curr->prev;
                    draw (win, buf, max_x);
                }
            else if (ch == KEY_DOWN && buf->curr->next != NULL
                     && buf->curr_line + max_x < buf->n)
                {
                    buf->curr_line++;
                    buf->curr = buf->curr->next;
                    draw (win, buf, max_x);
                }
        }
    */
    free_buffer (buf);
    endwin ();
    return 0;
}

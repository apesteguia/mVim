#include "window.h"
#include <curses.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

WINDOW *
new_window (char *file)
{
    WINDOW *win;
    int start_x, start_y;

    start_x = start_y = 0;

    getmaxyx (stdscr, start_y, start_x);
    win = newwin (start_y, start_x - 2, 1, 1);
    mvprintw (1, 3, "%s", file);
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
    curs_set (1);
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

// Modify the load_file function to return a Buffer*
Buffer *
load_file (WINDOW *win, char *path, int max_x)
{
    FILE *f;
    Line *l;
    Buffer *buf;
    char c;

    f = fopen (path, "r");
    if (f == NULL)
        {
            mvwprintw (win, 0, 0,
                       "Error: File not found or cannot be opened.");
            wrefresh (win);
            return NULL;
        }

    buf = malloc (sizeof (Buffer));
    buf->n = 0;
    buf->curr_line = 0;
    buf->head = malloc (sizeof (Line));
    buf->head->prev = buf->head->next = NULL;
    buf->curr = buf->head;

    while (fgets (buf->curr->content, MAXLEN, f))
        {
            l = malloc (sizeof (Line));
            l->prev = buf->curr;
            buf->curr->next = l;
            buf->curr = l;
            buf->n++;
        }
    buf->curr->next = NULL;

    fclose (f);
    return buf; // Return the created Buffer*
}

void
draw (WINDOW *win, Buffer *buf, int max_x, int start_line)
{
    werase (win);
    wmove (win, 0, 0);

    Line *current_line = buf->head;
    for (int i = 0; i < start_line && current_line != NULL; i++)
        {
            current_line = current_line->next;
        }

    for (int i = 0; i < max_x && current_line != NULL; i++)
        {
            waddstr (win, current_line->content);
            current_line = current_line->next;
        }

    wrefresh (win);
}

void
free_buffer (Buffer *buf)
{
}

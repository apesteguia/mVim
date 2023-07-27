#include "window.h"
#include <curses.h>
#include <menu.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

WINDOW *
create_explorer (WINDOW *w)
{
    WINDOW *win;
    int start_x, start_y;
    start_x = start_y = 0;

    getmaxyx (w, start_y, start_x);

    win = newwin (start_y - 10, start_x / 2, 5, (start_x) / 4);

    box (win, 0, 0);
    wattron (win, COLOR_PAIR (3) | A_BOLD);

    mvwprintw (win, 0, (getmaxx (win) - strlen ("Explorer")) / 2, "Explorer");
    wattroff (win, COLOR_PAIR (3) | A_BOLD);

    wrefresh (win);
    return win;
}

void
delete_explorer (WINDOW *win)
{
    delwin (win);
}

WINDOW *
new_window (char *file)
{
    WINDOW *win;
    int start_x, start_y;

    start_x = start_y = 0;

    getmaxyx (stdscr, start_y, start_x);
    win = newwin (start_y - 5, start_x - 2, 3, 1);
    box (win, 0, 0);
    scrollok (win, TRUE);
    wrefresh (win);
    refresh ();
    return win;
}

void
print_title (WINDOW *win, char *file)
{
    init_pair (3, COLOR_BLUE, COLOR_BLACK);
    wattron (stdscr, COLOR_PAIR (3) | A_BOLD);
    mvprintw (1, 1, "%s", file);
    wattroff (stdscr, COLOR_PAIR (3) | A_BOLD);
}

void
print_keys (WINDOW *win)
{
    int x, y;
    x = y = 0;

    init_pair (3, COLOR_BLUE, COLOR_BLACK);
    wattron (stdscr, COLOR_PAIR (3) | A_BOLD);
    getmaxyx (stdscr, y, x);
    mvprintw (y - 2, 1, "%s",
              "r: refresh | j,J: 1up/10 | k,K: 1down/10 | q: quit");
    wattroff (stdscr, COLOR_PAIR (3) | A_BOLD);
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

    int y = 1;
    int content_height = getmaxy (win) - 1;
    int line_number = start_line + 1;

    while (y <= content_height && current_line != NULL)
        {
            wmove (win, y, 0);

            // Print line number and content
            mvwprintw (win, y, 0, "%3d  %s", line_number,
                       current_line->content);

            current_line = current_line->next;
            y++;
            line_number++;
        }

    for (; y <= content_height; y++)
        {
            wmove (win, y, 0);
            wclrtoeol (win);
        }

    scrollok (win, TRUE);

    wrefresh (win);
}

void
free_buffer (Buffer *buf)
{
    Line *aux = buf->head;
    while (aux != NULL)
        {
            Line *next_line = aux->next;

            free (aux);
            aux = next_line;
        }

    free (buf);
}

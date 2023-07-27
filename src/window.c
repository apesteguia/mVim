#include "window.h"
#include <curses.h>
#include <dirent.h>
#include <menu.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **
getAllFilesInCurrentDirectory ()
{
    char **fileList = NULL;
    int fileCount = 0;

    DIR *dir = opendir (".");
    if (dir == NULL)
        {
            perror ("Unable to open directory");
            return NULL;
        }

    struct dirent *entry;
    while ((entry = readdir (dir)) != NULL)
        {
            if (entry->d_type == DT_REG)
                {
                    fileCount++;
                }
        }

    fileList = (char **)malloc ((fileCount + 1) * sizeof (char *));
    if (fileList == NULL)
        {
            closedir (dir);
            perror ("Memory allocation error");
            return NULL;
        }

    rewinddir (dir);

    int i = 0;
    while ((entry = readdir (dir)) != NULL)
        {
            if (entry->d_type == DT_REG)
                {
                    fileList[i] = strdup (entry->d_name);
                    i++;
                }
        }

    fileList[fileCount] = NULL;

    closedir (dir);

    return fileList;
}

void
create_explorer (WINDOW *w, Buffer *buf, int max_x, int start_line)
{
    WINDOW *win;
    MENU *menu;
    ITEM **items;
    int start_x, start_y, n;
    start_x = start_y = 0;
    char **dirs, ch, help[] = "w: quit | d: up | f: down";

    getmaxyx (w, start_y, start_x);

    win = newwin (start_y - 10, start_x / 2, 5, (start_x) / 4);

    box (win, 0, 0);
    wattron (win, COLOR_PAIR (3) | A_BOLD);

    mvwprintw (win, 0, (getmaxx (win) - strlen ("Explorer")) / 2, "Explorer");
    mvwprintw (win, getmaxy (win) - 1, (getmaxx (win) - strlen (help)) / 2,
               "%s", help);
    wattroff (win, COLOR_PAIR (3) | A_BOLD);

    dirs = getAllFilesInCurrentDirectory ();
    n = 0;
    while (dirs[n] != NULL)
        {
            n++;
        }

    items = (ITEM **)malloc (n * sizeof (ITEM) + 1);
    for (int i = 0; i < n; i++)
        {
            items[i] = new_item (dirs[i], NULL);
        }
    items[n] = NULL;

    menu = new_menu (items);
    set_menu_win (menu, win);
    set_menu_sub (menu, derwin (win, n, start_x / 3, 1, 1));
    set_menu_format (menu, n, 1);
    set_menu_mark (menu, "->");
    post_menu (menu);

    wrefresh (win);
    nodelay (win, FALSE);

    while ((ch = getch ()) != 'w')
        {
            switch (ch)
                {
                case 'd':
                    menu_driver (menu, REQ_UP_ITEM);
                    break;
                case 'f':
                    menu_driver (menu, REQ_DOWN_ITEM);
                    break;
                case 'w':
                    unpost_menu (menu);
                    free_menu (menu);
                    for (int i = 0; i < n; i++)
                        {
                            free_item (items[i]);
                            free (dirs[i]);
                        }
                    free (items);
                    free (dirs);
                    delwin (win);

                    break;
                }
            wrefresh (win);
        }

    unpost_menu (menu);
    free_menu (menu);
    for (int i = 0; i < n; i++)
        {
            free_item (items[i]);
            free (dirs[i]);
        }
    free (items);
    free (dirs);
    wrefresh (win);
    delwin (win);
    draw (w, buf, max_x, start_line);
    wrefresh (stdscr);
    wrefresh (w);
    refresh ();
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
    keypad (stdscr, TRUE);
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

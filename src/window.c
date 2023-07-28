#include "window.h"
#include <curses.h>
#include <dirent.h>
#include <menu.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void
write_file (Buffer *buf, char *path)
{
    FILE *f;
    Line *aux = buf->head;
    f = fopen (path, "w");

    while (aux != NULL)
        {
            fprintf (f, "%s", aux->content);
            aux = aux->next;
        }

    fclose (f);
}

bool
is_line_spaces (char *line)
{
    int i = 0;
    int len = strlen (line);
    while (line[i] == ' ')
        {
            i++;
        }
    return i == len;
}

void
remove_char (Buffer **buf, Mouse *m)
{
    Line *current_line = (*buf)->head;
    bool b;

    for (int i = 0; i < m->col && current_line; i++)
        {
            current_line = current_line->next;
        }

    if (current_line)
        {
            int len = strlen (current_line->content);

            b = is_line_spaces (current_line->content);

            if (b)
                return;

            if (m->row >= 0 && m->row < len)
                {
                    for (int i = m->row; i < len; i++)
                        {
                            current_line->content[i]
                                = current_line->content[i + 1];
                        }
                }
        }
}

void
main_loop (int argc, char *argv)
{
    WINDOW *win, *explorer;
    Mouse *m;
    Buffer *buf;
    int max_x, max_y, scroll_pos;
    char *res;

    max_x = max_y = scroll_pos = 0;
    m = malloc (sizeof (Mouse));
    m->x = 5;
    m->y = 1;
    m->col = 0;
    m->row = 0;

    if (argc == 1)
        {
            fputs ("less: No file to open\n", stderr);
            return;
        }

    init_window ();
    getmaxyx (stdscr, max_y, max_x);
    win = new_window (argv);
    buf = load_file (win, argv, max_x);
    draw (win, buf, getmaxx (win), 0);
    print_keys (win);
    print_title (win, argv);

    int ch = 'j';
    while ((ch = getch ()) != 'q' || res != NULL)
        {
            switch (ch)
                {
                case ' ':
                    m->x = 4;
                    m->y = 1;
                    scroll_pos = 0;
                    res = create_explorer (win, buf, getmaxx (win),
                                           scroll_pos);
                    if (res != NULL)
                        {
                            free_buffer (buf); // Free the current buffer
                            end_window (win);
                            win = new_window (res); // Open the selected file
                            buf = load_file (
                                win, res,
                                getmaxx (win)); // Load the selected file
                            draw (win, buf, getmaxx (win), scroll_pos);
                            clear ();
                            print_keys (win);
                            strcpy (argv, res);
                            print_title (win, argv);
                            free (res);

                            wclear (win);
                            refresh ();
                            wrefresh (win);
                            draw (win, buf, getmaxx (win), scroll_pos);
                        }
                    else
                        {
                            delete_explorer (explorer);
                        }
                    /*scroll_pos = 0;
                    res = create_explorer (win, buf, getmaxx (win),
                                           scroll_pos); */
                case 'c':
                    write_file (buf, argv);
                case KEY_BACKSPACE:
                    remove_char (&buf, m);
                    draw (win, buf, getmaxx (win), scroll_pos);
                    // wmove (win, m->y, m->x--);
                    break;
                case 'w':
                    if (m->y > 1)
                        {
                            m->y--;
                            wmove (win, m->y, m->x);
                            wrefresh (win);
                        }
                    if (m->col < 0)
                        m->col = 0;
                    else
                        m->col--;
                    if (m->y < 1)
                        {
                            m->y = 1;
                        }
                    break;
                case 's':
                    if (m->y < max_y - 2)
                        {
                            m->y++;
                            wmove (win, m->y, m->x);
                            wrefresh (win);
                        }
                    if (m->col > max_y)
                        m->col = max_y;
                    else
                        m->col++;
                    break;
                case 'a':
                    if (m->x < 6)
                        m->x = 6;
                    m->x--;
                    wmove (win, m->y, m->x);
                    wrefresh (win);
                    if (m->row < 0)
                        m->row = 0;
                    else
                        m->row--;
                    break;
                case 'd':
                    if (m->x > max_x)
                        m->x -= 2;
                    m->x++;
                    wmove (win, m->y, m->x);
                    wrefresh (win);
                    if (m->row > max_x - 2)
                        m->row = max_x - 3;
                    else
                        m->row++;
                    break;
                case 'k':
                    if (scroll_pos > 0)
                        {
                            delete_explorer (explorer);
                            scroll_pos -= 1;
                            draw (win, buf, getmaxx (win), scroll_pos);
                        }
                    break;
                case 'K':
                    if (scroll_pos > 0)
                        {
                            delete_explorer (explorer);
                            scroll_pos -= 10;
                            draw (win, buf, getmaxx (win), scroll_pos);
                        }
                    break;

                case 'j':
                    if (scroll_pos + getmaxy (win) - 1 < buf->n)
                        {
                            delete_explorer (explorer);
                            scroll_pos += 1;
                            draw (win, buf, getmaxx (win), scroll_pos);
                        }
                    break;
                case 'J':
                    if (scroll_pos + getmaxy (win) - 1 < buf->n > 0)
                        {
                            delete_explorer (explorer);
                            scroll_pos += 10;
                            draw (win, buf, getmaxx (win), scroll_pos);
                        }
                    break;

                case '+':
                    delete_explorer (explorer);
                    endwin ();
                    refresh ();
                    win = new_window (argv);
                    print_keys (win);
                    draw (win, buf, getmaxx (win), scroll_pos);
                    print_title (win, argv);
                    break;
                case '-':
                    delete_explorer (explorer);
                    endwin ();
                    refresh ();
                    win = new_window (argv);
                    print_keys (win);
                    draw (win, buf, getmaxx (win), scroll_pos);
                    print_title (win, argv);
                    break;

                case 'r':
                    delete_explorer (explorer);
                    endwin ();
                    refresh ();
                    win = new_window (argv);
                    print_keys (win);
                    draw (win, buf, getmaxx (win), scroll_pos);
                    print_title (win, argv);
                    break;
                default:
                    print_title (win, argv);
                    print_keys (win);
                    wrefresh (win);

                    break;
                }
        }

    free_buffer (buf);
    end_window (win);
    refresh ();
    wrefresh (stdscr);
    if (res != NULL)
        main_loop (argc, res);
    endwin ();
}
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

char *
create_explorer (WINDOW *w, Buffer *buf, int max_x, int start_line)
{
    WINDOW *win;
    MENU *menu;
    ITEM **items;
    int start_x, start_y, n, dir_index;
    start_x = start_y = 0;
    char **dirs, *res, ch, help[] = "w: quit | d: up | f: down";

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

    dir_index = 0;

    while ((ch = getch ()) != 'w')
        {
            switch (ch)
                {
                case 10:
                    menu_driver (menu, REQ_UP_ITEM);
                    res = (char *)malloc (sizeof (char)
                                          * strlen (dirs[dir_index]));
                    strcpy (res, dirs[dir_index]);
                    return res;
                    break;
                case 'g':
                    menu_driver (menu, REQ_UP_ITEM);
                    if (dir_index > 0)
                        dir_index--;
                    break;
                case 'f':
                    if (dir_index < n)
                        dir_index++;
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
    return NULL;
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

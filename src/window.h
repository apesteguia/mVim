#ifndef MAG_EDITOR_H
#define MAG_EDITOR_H

#include <ncurses.h>

#define MAXLEN 256

typedef struct line
{
    char content[MAXLEN];
    int n;
    struct line *prev;
    struct line *next;
} Line;

typedef struct buffer
{
    Line *head;
    Line *curr;
    int n;
    int curr_line;
} Buffer;

WINDOW *new_window (char *);
void init_window ();
void print_title (WINDOW *win, char *file);
void print_keys (WINDOW *win);
void end_window (WINDOW *);
Buffer *load_file (WINDOW *, char *, int);
void scrollWindow (WINDOW *, int *);
void draw (WINDOW *, Buffer *, int, int);
void free_buffer (Buffer *);

#endif // MAG_EDITOR_H

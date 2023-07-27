#ifndef MAG_EDITOR_H
#define MAG_EDITOR_H

#include <ncurses.h>

#define MAX_HEIGHT 2000
#define N 1000
#define MAXLEN 256

typedef struct line
{
    char content[MAXLEN];
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
void end_window (WINDOW *);
void load_file (WINDOW *, Buffer *, char *, int);
void scrollWindow (WINDOW *, int *);
void draw (WINDOW *, Buffer *, int);
void free_buffer (Buffer *);

#endif // MAG_EDITOR_H

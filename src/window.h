#ifndef MAG_EDITOR_H
#define MAG_EDITOR_H

#include <ncurses.h>

#define MAX_HEIGHT 2000
#define N 1000
#define MAX_LINE_LENGTH 256

WINDOW *new_window (char *);
void init_window ();
void end_window (WINDOW *);
void load_file (WINDOW *, char *, int);
void scrollWindow (WINDOW *, int *);

#endif // MAG_EDITOR_H

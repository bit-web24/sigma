#ifndef MAIN_H
#define MAIN_H

struct node *head = NULL;
struct node *tail = NULL;

unsigned int input;
int Xmax,
    Ymax,
    Xwin,
    Ywin,
    x, y;
bool atlast = true;
bool zoomed_io = false;

extern void Hscroll(WINDOW *window, struct node *head, int Xmax, int x);
extern void lrefresh(WINDOW *window, struct node *head);
extern void set_statusbar(int Xmax, int Ymax, int x, int y);

#endif

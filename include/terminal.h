#ifndef TERMINAL_H
#define TERMINAL_H

#include <cstdio>
#include <unistd.h>

#define clear_screen() write(1, "\x1B[2J", 4)
#define mvcurs(y, x) printf("\x1B[%d;%dH", y, x)
#define clear_line() printf("\x1B[2K")
#define save_cur() printf("\x1B%d", 7)
#define restore_cur() printf("\x1B%d", 8)

void gety(int *y);
void getx(int *x);
void terminit();
void termexit();
#endif

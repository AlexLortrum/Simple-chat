#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include "../include/terminal.h"

void gety(int *y)
{
	struct winsize size;
	ioctl(1, TIOCGWINSZ, &size);
	*y = size.ws_row;
}

void getx(int *x)
{
	struct winsize size;
	ioctl(1, TIOCGWINSZ, &size);
	*x = size.ws_col;
}

void terminit()
{
	clear_screen();
	struct termios term;
	tcgetattr(0, &term);
	term.c_lflag &= ~ICANON;
	term.c_lflag &= ~ECHO;
	tcsetattr(0, TCSANOW, &term);
}

void termexit()
{
	struct termios term;
	tcgetattr(0, &term);
	term.c_lflag |= ICANON;
	term.c_lflag |= ECHO;
	tcsetattr(0, TCSANOW, &term);
}

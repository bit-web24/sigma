#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifndef BIT_TYPES

#ifndef ENTER
	#define ENTER 10
#endif

#ifndef BACKSPACE
	#define BACKSPACE 127
#endif

#endif

int input,
    Xmax,
    Ymax,
    Xwin,
    Ywin,
    x, y;

void set_statusbar(int Xmax, int Ymax, int x, int y){
	attron(A_REVERSE);
	for(int i = 0; i < Xmax; i++){
		mvprintw(Ymax-1, i, " ");
	};

	mvprintw(Ymax-1, Xmax-6, "(%d,%d)", y, x);
	attroff(A_REVERSE);
	refresh();
};

void set_topbar(int Xmax, char **CLI_argument){
	attron(A_REVERSE);
	for(int i = 0; i < Xmax; i++){
		mvprintw(0, i, " ");
	};

	mvprintw(0, (Xmax-4)/2, "nano");
	if(CLI_argument[1]){
		mvprintw(0, 1, "%s", CLI_argument[1]);
	} else{
		mvprintw(0, 1, "New");
	};
	attroff(A_REVERSE);
	refresh();
};

int main(int argc, char *argv[]){
	initscr();
	noecho();
	start_color();
	keypad(stdscr, true);

	Ymax = 0;
	Xmax = 0;
	Xwin = 0;
	Ywin = 0;
	x = 0;
	y = 0;

	getmaxyx(stdscr, Ymax, Xmax);
	WINDOW *window = newwin(Ymax-2, Xmax, 1, 0);
	getmaxyx(window, Ywin, Xwin);
	
	set_topbar(Xmax, argv);
	set_statusbar(Xmax, Ymax, x, y);
	
	wrefresh(window);
	wmove(window, y, x);

	while((input = getch()) != KEY_CANCEL){
		switch(input){
			case ENTER:
				y += 1;
				if(y > Ywin-1){
					y = Ywin-1;
					x = 0;
					set_statusbar(Xmax, Ymax, x, y);
				}
				x = 0;
				wmove(window, y, x);
				set_statusbar(Xmax, Ymax, x, y);
				break;
			case BACKSPACE:
				x -= 1;
				if(x < 0){
					y -= 1;
					if(y < 0){
						y = 0;
						x = 0;
					} else{
						x = Xmax-1;
					};
					wmove(window, y, x);
				}
				mvwdelch(window, y, x);
				set_statusbar(Xmax, Ymax, x, y);
				break;
			case KEY_DOWN:
				y += 1;
				if(y > Ywin-1){
					y = Ywin-1;
					wmove(window, y, x);
				}
				wmove(window, y, x);
				set_statusbar(Xmax, Ymax, x, y);
				break;
			case KEY_UP:
				y -= 1;
				if(y < 0){
					y = 0;
					wmove(window, y, x);
				}
				wmove(window, y, x);
				set_statusbar(Xmax, Ymax, x, y);
				break;
			case KEY_LEFT:
				x -= 1;
				if(x < 0){
					y -= 1;
					if(y < 0){
						x = 0;
						y = 0;
						wmove(window, y, x);
					} else{
					x = Xwin-1;
					wmove(window, y, x);
					set_statusbar(Xmax, Ymax, x, y);
					};
				}
				wmove(window, y, x);
				set_statusbar(Xmax, Ymax, x, y);
				break;
			case KEY_RIGHT:
				x += 1;
				if(x > Xwin-1){
					y += 1;
					if(y > Ywin-1){
						x = Xwin-1;
						y = Ywin-1;
						wmove(window, y, x);
					}
					x = 0;
					wmove(window, y, x);
					set_statusbar(Xmax, Ymax, x, y);
				}
				wmove(window, y, x);
				set_statusbar(Xmax, Ymax, x, y);
				break;
			default:
				if(x > Xmax-2){
					mvwprintw(window, y, x, "%c", ENTER);
					x  = 0;
					y += 1;
					mvwprintw(window, y, x, "%c", (char) input);
				}
				mvwprintw(window, y, x, "%c", (char) input);
				x += 1;
				set_statusbar(Xmax, Ymax, x, y);
				break;
		};

		refresh();
		wrefresh(window);
	};

	endwin();
};

#include <ncurses.h>

#include "ckeys.h"

void set_statusbar(int Xmax, int Ymax, int x, int y){
	attron(A_REVERSE);
	for(int i = 0; i < Xmax; i++){
		mvprintw(Ymax-1, i, " ");
	};

	mvprintw(Ymax-1, (Xmax)/2, "(%d,%d)", y, x);
	attroff(A_REVERSE);
	refresh();
};

void lrefresh(WINDOW *window, struct node *head){
	struct node *temp = head;
	unsigned int X = 0;
	unsigned int Y = 0;

if(temp != NULL){
	while(temp->next != NULL){
		if(temp->data == ENTER){
			mvwprintw(window, Y, X, "%c", (char) temp->data);
			X = 0;
			Y += 1;
			
			temp->x = X;
			temp->y = Y;
		} else {	
			mvwprintw(window, Y, X, "%c", (char) temp->data);
			temp->x = X;
			temp->y = Y;
			X += 1;
		};

		temp = temp->next;
	};
		
	mvwprintw(window, Y, X, "%c", (char) temp->data);

	X = 0;
	Y = 0;

	wrefresh(window);
	refresh();
}
};

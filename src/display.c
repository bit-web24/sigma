/** This file is part of Sigma.

   * Sigma is free software: you can redistribute it and/or modify
   * it under the terms of the GNU General Public License as published by
   * the Free Software Foundation, either version 3 of the License, or
   * (at your option) any later version.

   * Sigma is distributed in the hope that it will be useful,
   * but WITHOUT ANY WARRANTY; without even the implied warranty of
   * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   * GNU General Public License for more details.

   * You should have received a copy of the GNU General Public License
   * along with Sigma.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <ncurses.h>

#include "global.h"

extern int ARGC;
extern char **ARGV;
extern int TOTAL_LINE_WRITTEN;

void set_statusbar(int Xmax, int Ymax, int x, int y);
void lrefresh(WINDOW *window, struct node *head);

void set_statusbar(int Xmax, int Ymax, int x, int y){
	attron(A_REVERSE);
	for(int i = 0; i < Xmax; i++){
		mvprintw(Ymax-1, i, " ");
	};

	if(ARGV[1] != NULL) {
		mvprintw(Ymax-1, 1, "\"%s\"", ARGV[1]);
	} else {
		mvprintw(Ymax-1, 1, "\"NEW\"");
	};

	mvprintw(Ymax-1, (Xmax)/2, "(%d,%d)", y, x);
	mvprintw(Ymax-1, (Xmax-4), "[%d]", TOTAL_LINE_WRITTEN);
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

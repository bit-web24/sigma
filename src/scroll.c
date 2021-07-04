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

void Hscroll(WINDOW *window, struct node *head, int Xmax, int x){
	unsigned int X, Y, cntr;
	unsigned int abs_X;
	bool is_enter = false;
	bool is_last = false;
	
	X = 0;
	Y = 0;
	cntr = x-(Xmax-2);
	abs_X= 0;
	
	struct node *scrollh = head;
	for(int i = 0; i < cntr; i++){
		scrollh = scrollh->next;
		if(scrollh->data == ENTER){
			break;
		}
		continue;
	};
	
	while(scrollh->next != NULL){
		if(scrollh->data != ENTER){
			mvwprintw(window, Y, X, "%c", (char) scrollh->data);
			X += 1;
			abs_X += 1;
		} else {
			mvwprintw(window, Y, X, "%c", (char) scrollh->data);
			X = 0;
			Y += 1;
			abs_X = 0;
			for(int i = 0; i < cntr; i++){
				/**
				 * the condition solves the problem of over iteration of
				 * scrollh = scrollh->next;
				 * when it actually overflows it set the stat to true,
				 * to break the 'while' iteration after breaking the 'for loop'
				 */
				if(scrollh->next != NULL){
					scrollh = scrollh->next;
					if(scrollh->data == ENTER){
						is_enter = true;
						break;
					}
					abs_X += 1; 
					continue;
				} else {
					is_last = true;
					break;
				};
			};

			if(is_last){
				break;
			} else if (is_enter == true){
				is_enter = false;
				continue;
			} else{
				wrefresh(window);
			};
		};

		if(scrollh->next != NULL){
			scrollh = scrollh->next;
			abs_X += 1;
		} else {
			break;
		};
		//don't iterate
	};

	if(is_last == true){
		is_last = false;
	} else {
		if(x-(Xmax-2) < abs_X)
			mvwprintw(window, Y, X, "%c", (char) scrollh->data);
	};

	X = 0;
	Y = 0;

	wrefresh(window);
	refresh();
};

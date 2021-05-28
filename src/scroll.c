#include <ncurses.h>

#include "ckeys.h"

void Hscroll(WINDOW *window, struct node *head, int Xmax, int x){
	unsigned int X, Y, cntr;
	X = 0;
	Y = 0;
	bool is_enter = false;
	cntr = x-(Xmax-2);
	uint8_t cntr_2= 0;
	
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
		} else {
			mvwprintw(window, Y, X, "%c", (char) scrollh->data);
			X = 0;
			Y += 1;
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
					continue;
				} else {
					cntr_2 = 1;
					break;
				};
			};
			if(cntr_2 == 1){
				break;
			} else {
				if(is_enter == true){
					is_enter = false;
					continue;
				} else{
				wrefresh(window);
				};
			};
		};
		if(scrollh->next != NULL){
			scrollh = scrollh->next;
		}
		// don't iterate
	};
	if(cntr_2 == 1){
		cntr_2 = 0;
	} else {
		mvwprintw(window, Y, X, "%c", (char) scrollh->data);
	};
	X = 0;
	Y = 0;

	wrefresh(window);
	refresh();
};

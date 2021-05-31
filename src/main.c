/**
 * Sigma - A Lightweight Text Editor
 * /

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

#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "ckeys.h"
#include "main.h"

void add_new_node(unsigned int input);

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

	head = NULL;
	tail = NULL;
	zoomed_io = false;
ASCII_RELOAD:
	getmaxyx(stdscr, Ymax, Xmax);
	WINDOW *window = newwin(Ymax-1, Xmax, 0, 0);
	getmaxyx(window, Ywin, Xwin);
	
	set_statusbar(Xmax, Ymax, x, y);
	
	if(zoomed_io == true){
		if(x-(Xmax-2) > 0){
			Hscroll(window, head, Xmax, x);
			wmove(window, y, Xmax-1);
		}
		lrefresh(window, head);
		wmove(window, y, x);
		wrefresh(window);
	} else{
		wrefresh(window);
		wmove(window, y, x);
	};

	while((input = getch()) != KEY_CANCEL){
		switch(input){
			case ZOOM_IO:
				zoomed_io = true;
				goto ASCII_RELOAD;
				break;
		case ENTER:
		;
			struct node *new_node = (struct node *) malloc(sizeof(struct node));
				if(new_node == NULL){
					clear();
					endwin();
				}
				new_node->data = ENTER;
				
				if(tail != NULL){
					if(tail->prev == NULL){
						if(atlast == true){
							new_node->next = NULL;
							new_node->prev = tail;
							tail->next = new_node;
							tail = new_node;
						} else{
						new_node->prev = NULL;
						new_node->next = tail;
						tail->prev = new_node; 
						head = new_node;
						};
					} else if(tail->next != NULL){
						new_node->next = tail;
						new_node->prev = tail->prev;
						
						(tail->prev)->next = new_node;
						tail->prev = new_node;
						tail = tail;
					} else{
						if(atlast == false){
						new_node->next = tail;
						new_node->prev = tail->prev;
						
						(tail->prev)->next = new_node;
						tail->prev = new_node;
						} else{
						new_node->next = NULL;
						new_node->prev = tail;
						tail->next = new_node;
						tail = new_node;
						};
					};
				
				} else{
					new_node->next = NULL;
					new_node->prev = NULL;
					head = new_node;
					tail = new_node;
				};
				
				new_node->x = x;
				new_node->y = y;
				x = 0;
				y += 1;
				
				lrefresh(window, head);
				wmove(window, y, x);
				set_statusbar(Xmax, Ymax, x, y);
				break;
			case KEY_BACKSPACE:
				goto rmchar;
				break;
			case BACKSPACE:
rmchar:
				if(tail == NULL){
					break;
				} else if(tail->prev != NULL){
					if(atlast == true){
					if(tail->data == ENTER){
						struct node *xprev = tail->prev;
						xprev->next = NULL;

						free(tail);
						tail = xprev;
						
						y -= 1;
						if(tail->data != ENTER){
							x = (tail->x)+1;
						} else{
							x = (tail->x);
						};
						
						if(x-(Xmax-2) > 0){
							x -= 1;
							wclear(window);
							Hscroll(window, head, Xmax, x);
							x += 1;
							wmove(window, y, Xmax-1);
						} else if(x-(Xmax-2) == 0){
							mvwdelch(window, y, x);
							lrefresh(window, head);
							wmove(window, y, x);
						} else {
							wmove(window, y, x);
						};

						set_statusbar(Xmax, Ymax, x, y);
					} else{
						struct node *xprev = tail->prev;
						xprev->next = NULL;

						free(tail);
						tail = xprev;
					
						x -= 1;
						if(x-(Xmax-2) > 0){
							mvwdelch(window, y, x);
							x -= 1;
							Hscroll(window, head, Xmax, x);
							x += 1;
							wmove(window, y, Xmax-1);
						} else if(x-(Xmax-2) == 0){
							mvwdelch(window, y, x);
							lrefresh(window, head);
							wmove(window, y, x);
						} else {
							mvwdelch(window, y, x);
							lrefresh(window, head);
							wmove(window, y, x);
						};

						set_statusbar(Xmax, Ymax, x, y);
					};

				} else{
					if((tail->prev)->prev == NULL){
						free(tail->prev);
						tail->prev = NULL;
						head = tail;
						x = 0;
						y = 0;
						mvwdelch(window, y, x);
						wclear(window);
					lrefresh(window, head);
					wmove(window, y, x);
					set_statusbar(Xmax, Ymax, x, y);
					} else {
						if((tail->prev)->data == ENTER){
							struct node *tmp = tail->prev;
							tail->prev = (tail->prev)->prev;
							(tail->prev)->next = tail;

							free(tmp);
						
							y -= 1;
							if((tail->prev)->data == ENTER){
								x = 0;
								/**
								 * the following code will clear the previous stuffs and list the all chars accordingly
								 */
								wclear(window);
								lrefresh(window, head);
								wmove(window, y, x);
							} else{
								x = (tail->prev)->x+1;
							if(x-(Xmax-2) > 0){
								x -= 1;
								wclear(window);
								Hscroll(window, head, Xmax, x);
								x += 1;
								wmove(window, y, Xmax-1);
							} else if(x-(Xmax-2) == 0){
								mvwdelch(window, y, x);
								lrefresh(window, head);
								wmove(window, y, x);
							} else {
								wclear(window);
								lrefresh(window, head);
								wmove(window, y, x);
							};
						};

					set_statusbar(Xmax, Ymax, x, y);
						}  else{
							struct node *tmp = tail->prev;
							tail->prev = (tail->prev)->prev;
							(tail->prev)->next = tail;

							free(tmp);
				
							x -= 1;
							if(x-(Xmax-2) > 0){
								mvwdelch(window, y, x);
								x -= 1;
								Hscroll(window, head, Xmax, x);
								x += 1;
								wmove(window, y, Xmax-1);
							} else if(x-(Xmax-2) == 0){
								mvwdelch(window, y, x);
								lrefresh(window, head);
								wmove(window, y, x);
							} else {
								mvwdelch(window, y, x);
								lrefresh(window, head);
								wmove(window, y, x);
							};
						set_statusbar(Xmax, Ymax, x, y);
						};
					};
				};
					
				} else{
					if(atlast == true){
						free(tail);
						tail = NULL;
						head = NULL;

						x = 0;
						y = 0;
						mvwdelch(window, y, x);
						wmove(window, y, x);
						set_statusbar(Xmax, Ymax, x, y);
					} else{
						break;
					};
				};

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
				if(tail == NULL){
					break;
				} else if(atlast == true){
					if(tail->data == ENTER){
						if((tail->prev)->data == ENTER){
							x = tail->x;
						} else {
							x = (tail->prev)->x+1;
						};

						y -= 1;
						if(x-(Xmax-2) > 0){
							x -= 1;
							Hscroll(window, head, Xmax, x);
							x += 1;
							wmove(window, y, Xmax-1);
						} else {
							wrefresh(window);
							wmove(window, y, x);
						};
					} else{
						if(x-(Xmax-2) > 0){
							x -= 2;
							Hscroll(window, head, Xmax, x);
							x += 1;
							wmove(window, y, Xmax-1);
						} else {
							x -= 1;
							wmove(window, y, x);
						};

					};
					
					atlast = false;
					set_statusbar(Xmax, Ymax, x, y);
				} else{
					if(tail->prev != NULL){
						if((tail->prev)->data == ENTER){
							tail = tail->prev;
							if(tail->prev != NULL){
								if((tail->prev)->data != ENTER){
									x = (tail->prev)->x+1;
								} else{
									 x = (tail->prev)->x;
								};
								y -= 1;
							if(x-(Xmax-2) > 0){
								x -= 1;
								wclear(window);
								Hscroll(window, head, Xmax, x);
								x += 1;
								wmove(window, y, Xmax-1);
							} else {
								wmove(window, y, x);
							};
							} else{
								x = 0;
								y = 0;
								wmove(window, y, x);
							};
							
							set_statusbar(Xmax, Ymax, x, y);
						} else {
							tail = tail->prev;
							x -= 1;
							if(x-(Xmax-2) > 0){
								x -= 1;
								Hscroll(window, head, Xmax, x);
								x += 1;
								wmove(window, y, Xmax-1);
							} else {
								wrefresh(window);
								wmove(window, y, x);
							};
							set_statusbar(Xmax, Ymax, x, y);
						};
					} 
					 else{
						continue;
					};
				};
				break;
			case KEY_RIGHT:
				if(tail == NULL){
					break;
				} else if(tail->next != NULL){
					if((tail->next)->data == ENTER){
						if(tail->data == ENTER){
							tail = tail->next;
							y += 1;
							x = 0;

							wclear(window);
							lrefresh(window, head);
							wrefresh(window);
							wmove(window, y, x);
						} else{
							tail = tail->next;
							x += 1;
							
						if(x-(Xmax-2) > 0){
							x -= 1;
							wclear(window);
							Hscroll(window, head, Xmax, x);
							x += 1;
							wmove(window, y, Xmax-1);
						} else {
							wrefresh(window);
							wmove(window, y, x);
						};
						
						};
						
					} else if(tail->data == ENTER){
						tail = tail->next;
						x = 0;
						y += 1;
						
						lrefresh(window, head);
						wmove(window, y, x);
					} else{
						tail = tail->next;
						x += 1;

						if(x-(Xmax-2) > 0){
							x -= 1;
							wclear(window);
							Hscroll(window, head, Xmax, x);
							x += 1;
							wmove(window, y, Xmax-1);
						} else {
							wrefresh(window);
							wmove(window, y, x);
						};
					};
				} else{
					if(atlast != true){
						if(tail->data != ENTER){
							atlast = true;
							x += 1;

							if(x-(Xmax-2) > 0){
								wclear(window);
								x -= 1;
								Hscroll(window, head, Xmax, x);
								x += 1;
								wmove(window, y, Xmax-1);
							} else{
								wmove(window, y, x);
							};
						} else{
							y += 1;
							x = 0;
							atlast = true;
							lrefresh(window, head);
							wmove(window, y, x);
						};
					} else{
						continue;
					};
				};
				
				set_statusbar(Xmax, Ymax, x, y);
				break;
			default:
				if(x > Xmax-2){
					add_new_node(input);
					Hscroll(window, head, Xmax, x);
					x += 1;
					wmove(window, y, Xmax-1);
				} else {
					add_new_node(input);
					x += 1;
	
					lrefresh(window, head);
					wmove(window, y, x);
				};
				set_statusbar(Xmax, Ymax, x, y);
				break;
		};

		refresh();
		wrefresh(window);
	};

	endwin();
	return 0;
};



void add_new_node(unsigned int input){				
		// Adding a new_node to the buffer
		struct node *new_node = (struct node *) malloc(sizeof(struct node));
				if(new_node == NULL){
					clear();
					endwin();
				}
				new_node->data = input;
				new_node->x = x;
				new_node->y = y;

				if(tail == NULL){
					new_node->next = NULL;
					new_node->prev = NULL;
					
					tail = new_node;
					head = new_node;
				}else if(tail->next == NULL){
				
					if (atlast == true){
						new_node->prev = tail;
						new_node->next = NULL;
					
						tail->next = new_node;
						tail = new_node;
					} else{
						new_node->next = tail;
						
						if(tail->prev != NULL){
							new_node->prev = tail->prev;
							(tail->prev)->next = new_node;
							tail->prev = new_node;
							tail = tail;
						} else{
							new_node->prev = NULL;
							tail->prev = new_node;
							head = new_node;
						};
					};
				} else{
					if(tail->prev != NULL){
						new_node->next = tail;
						new_node->prev = tail->prev;
					
						(tail->prev)->next = new_node;
						tail->prev = new_node;
					} else{
						head = new_node;
						new_node->next = tail;
						new_node->prev = NULL;
						tail->prev = new_node;
					};				
				};
				// Adding a new_node to the buffer
};



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

#ifndef ZOOM_IO
#define ZOOM_IO 410
#endif

#endif

struct node {
	struct node *prev;
	unsigned int data;
	int x;
	int y;
	struct node *next;
};

struct node *head = NULL;
struct node *tail = NULL;

unsigned int input;
int Xmax,
    Ymax,
    Xwin,
    Ywin,
    x, y;
bool atlast = true;

void Hscroll(WINDOW *window);
void Vscroll(WINDOW *window);
void lrefresh(WINDOW *window);
void add_new_node(unsigned int input);

void set_statusbar(int Xmax, int Ymax, int x, int y){
	attron(A_REVERSE);
	for(int i = 0; i < Xmax; i++){
		mvprintw(Ymax-1, i, " ");
	};

	mvprintw(Ymax-1, Xmax-8, "(%d,%d)", y, x);
	attroff(A_REVERSE);
	refresh();
};

void set_topbar(int Xmax, char **CLI_argument){
	attron(A_REVERSE);
	for(int i = 0; i < Xmax; i++){
		mvprintw(0, i, " ");
	};

	mvprintw(0, (Xmax-4)/2, "sigma");
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

	head = NULL;
	tail = NULL;
ASCII_RELOAD:
	getmaxyx(stdscr, Ymax, Xmax);
	WINDOW *window = newwin(Ymax-2, Xmax, 1, 0);
	getmaxyx(window, Ywin, Xwin);
	
	set_topbar(Xmax, argv);
	set_statusbar(Xmax, Ymax, x, y);
	
	wrefresh(window);
	wmove(window, y, x);

	while((input = getch()) != KEY_CANCEL){
		switch(input){
			case ZOOM_IO:
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
				
				lrefresh(window);
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
							Hscroll(window);
							x += 1;
							wmove(window, y, Xmax-1);
						} else if(x-(Xmax-2) == 0){
							mvwdelch(window, y, x);
							lrefresh(window);
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
							Hscroll(window);
							x += 1;
							wmove(window, y, Xmax-1);
						} else if(x-(Xmax-2) == 0){
							mvwdelch(window, y, x);
							lrefresh(window);
							wmove(window, y, x);
						} else {
							mvwdelch(window, y, x);
							lrefresh(window);
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
					lrefresh(window);
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
								lrefresh(window);
								wmove(window, y, x);
							} else{
								x = (tail->prev)->x+1;
							if(x-(Xmax-2) > 0){
								x -= 1;
								wclear(window);
								Hscroll(window);
								x += 1;
								wmove(window, y, Xmax-1);
							} else if(x-(Xmax-2) == 0){
								mvwdelch(window, y, x);
								lrefresh(window);
								wmove(window, y, x);
							} else {
								wclear(window);
								lrefresh(window);
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
								Hscroll(window);
								x += 1;
								wmove(window, y, Xmax-1);
							} else if(x-(Xmax-2) == 0){
								mvwdelch(window, y, x);
								lrefresh(window);
								wmove(window, y, x);
							} else {
								mvwdelch(window, y, x);
								lrefresh(window);
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
						x = tail->x;
						y = tail->y;
						wrefresh(window);
						wmove(window, y, x);
					} else{
						if(x-(Xmax-2) > 0){
							x -= 2;
							Hscroll(window);
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
								Hscroll(window);
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
								Hscroll(window);
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
							
							wrefresh(window);
							wmove(window, y, x);
						} else{
							tail = tail->next;
							x += 1;
							
						if(x-(Xmax-2) > 0){
							x -= 1;
							wclear(window);
							Hscroll(window);
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
						
						lrefresh(window);
						wmove(window, y, x);
					} else{
						tail = tail->next;
						x += 1;

						if(x-(Xmax-2) > 0){
							x -= 1;
							wclear(window);
							Hscroll(window);
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
								Hscroll(window);
								x += 1;
								wmove(window, y, Xmax-1);
							} else{
								wmove(window, y, x);
							};
						} else{
							y += 1;
							x = 0;
							atlast = true;
							lrefresh(window);
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
					Hscroll(window);
					x += 1;
					wmove(window, y, Xmax-1);
				} else {
					add_new_node(input);
					x += 1;
	
					lrefresh(window);
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

void Hscroll(WINDOW *window){
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
				 * when it actually overflows it set the stat to true, to break the 'while' iteration after breaking the 'for loop'
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
				continue;
			} else {
				if(is_enter == true){
					continue;
				}
				wrefresh(window);
			};
		};
		if(scrollh->next != NULL){
			scrollh = scrollh->next;
		}
		// don't iterate
	};
	if(cntr_2 == 1){
		// don't print any character
	} else {
		mvwprintw(window, Y, X, "%c", (char) scrollh->data);
	};
	X = 0;
	Y = 0;

	wrefresh(window);
	refresh();
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

void lrefresh(WINDOW *window){
	struct node *temp = head;
	unsigned int X = 0;
	unsigned int Y = 0;

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
};

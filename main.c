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

	head = NULL;
	tail = NULL;

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
		;
			struct node *new_node = (struct node *) malloc(sizeof(struct node));
				if(new_node == NULL){
					clear();
					endwin();
				}
				new_node->data = ENTER;
				
				if(tail != NULL){
					if(tail->prev == NULL){
						new_node->prev = NULL;
						new_node->next = tail;
						tail->prev = new_node; 
						head = new_node;
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
				
				refresh();
				wrefresh(window);
				lrefresh(window);
				wmove(window, y, x);
				set_statusbar(Xmax, Ymax, x, y);
				break;
			case BACKSPACE:
				if(tail == NULL){
					break;
				} else if(tail->prev != NULL){
				
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
						
						wmove(window, y, x);
					} else{
						struct node *xprev = tail->prev;
						xprev->next = NULL;

						free(tail);
						tail = xprev;
					
						x -= 1;
						mvwdelch(window, y, x);
					};
					lrefresh(window);
					set_statusbar(Xmax, Ymax, x, y);
				} else{
					free(tail);
					if(tail->data == ENTER){
						head = NULL;
						tail = NULL;
						x = 0;
						y = 0;
						wmove(window, y, x);
						wrefresh(window);
						set_statusbar(Xmax, Ymax, x, y);
					}
					tail = NULL;
					head = NULL;
					
					x = 0;
					mvwdelch(window, y, x);
					set_statusbar(Xmax, Ymax, x, y);
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
				if(atlast == true){
					if(tail->data == ENTER){
						x = tail->x;
						y = tail->y;
					} else{
						x -= 1;
					};
					
					atlast = false;
					wrefresh(window);
					wmove(window, y, x);
					set_statusbar(Xmax, Ymax, x, y);
				} else{
					if(tail->prev != NULL){
						if((tail->prev)->data == ENTER){
							tail = tail->prev;
							if((tail->prev)->data != ENTER){
								x = (tail->prev)->x+1;
							} else{
								 x = (tail->prev)->x;
							};
							
							y -= 1;
							wmove(window, y, x);
							set_statusbar(Xmax, Ymax, x, y);
						} else{
						tail = tail->prev;
						x -= 1;
						wrefresh(window);
						wmove(window, y, x);
						set_statusbar(Xmax, Ymax, x, y);
						};
					} 
					 else{
						continue;
					};
				};
				break;
			case KEY_RIGHT:
				if(tail->next != NULL){
					if((tail->next)->data == ENTER){
						if(tail->data == ENTER){
							tail = tail->next;
							y += 1;
							x = 0;
						} else{
							tail = tail->next;
							x += 1;
						};
					} else if(tail->data == ENTER){
						tail = tail->next;
						x = 0;
						y += 1;
					} else{
						tail = tail->next;
						x += 1;
					};
				} else{
					if(atlast != true){
						if(tail->data != ENTER){
						atlast = true;
						x += 1;
						} else{
							continue;
						};
					} else{
						continue;
					};
				};
				
				wrefresh(window);
				wmove(window, y, x);
				set_statusbar(Xmax, Ymax, x, y);
				break;
			default:
				if(x > Xmax-2){
					add_new_node(ENTER);
					x  = 0;
					y += 1;
					add_new_node(input);
				} else{
					add_new_node(input);
					x += 1;
				};

				lrefresh(window);
				wmove(window, y, x);
				set_statusbar(Xmax, Ymax, x, y);
				break;
		};

		refresh();
		wrefresh(window);
	};

	endwin();
};

void add_new_node(unsigned int input){				
		// Adding a new_node to the buffer
		struct node *new_node = (struct node *) malloc(sizeof(struct node));
				if(new_node == NULL){
					clear();
					endwin();
				}
				new_node->data = input;
				if(tail == NULL){
					new_node->next = NULL;
					new_node->prev = NULL;
					new_node->x = x;
					new_node->y = y;
					
					tail = new_node;
					head = new_node;
				}else if(tail->next == NULL){
				
					if (atlast == true){
						new_node->prev = tail;
						new_node->next = NULL;
						new_node->x = x;
						new_node->y = y;
					
						tail->next = new_node;
						tail = new_node;
					} else{
						new_node->next = tail;
						new_node->prev = tail->prev;
						new_node->x = x;
						new_node->y = y;
						
						(tail->prev)->next = new_node;
						tail->prev = new_node;
						tail = tail;
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

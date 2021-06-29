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
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ncurses.h>

#include "global.h"

extern struct FILE_INFO fstatus;

extern struct node *head;
extern FILE        *TARGET;
extern char        INPUT_FILE[250];
extern int         Xmax;
extern int         Ymax;
extern int	   x;
extern int	   y;
extern WINDOW      *window;
extern void 	   set_statusbar(int Xmax, int Ymax, int x, int y);

int  save_to_file();
int  perform_required_action(WINDOW *window);
void invoke_actions(char action[2], WINDOW *window);
void get_filename();
void clear_statusbar();
char action[2];

void clear_statusbar(){
	attron(A_REVERSE);
	for(int z = 0; z < Xmax-1; z++){
		mvprintw(Ymax-1, z, " ");
	};
	attroff(A_REVERSE);
}

void get_filename(){
	uint32_t input;
	uint32_t cntr;
	
	cntr = 0;
	for(int i = 0; i < Xmax; i++){
		input = getch();
		if(input == ENTER || input == KEY_ENTER){
			INPUT_FILE[cntr] = '\0';
			set_statusbar(Xmax, Ymax, x, y);
			break;
		} else if(input == BACKSPACE || input == KEY_BACKSPACE){
			cntr -= 1;
			INPUT_FILE[cntr] = '\0';
			mvdelch(Ymax, cntr);
			move(Ymax, cntr);
		} else if(input == KEY_UP   || input == KEY_DOWN){
			// DO NOTHING!
		} else {
			INPUT_FILE[cntr] = (char) input;

			attron(A_REVERSE);
			mvprintw(Ymax-1, cntr, "%c", (char) input);
			attron(A_REVERSE);
			
			cntr += 1;
			move(Ymax, cntr);
		}
	};
}

int save_to_file(){
	uint32_t input;
	uint32_t cntr;	

	if(fstatus.FSPEC == NO){
		clear_statusbar();
		move(Ymax-1, 0);
		get_filename();
		TARGET = fopen(INPUT_FILE, "w+");
	} else {
		TARGET = fopen(INPUT_FILE, "w+");
	};

	if(head == NULL){
		fprintf(TARGET, "%c", (char) EOF);
		return 1;
	}
	struct node *tmp = head;
	while(tmp->next != NULL){
		fprintf(TARGET, "%c", (char) tmp->data);
		tmp = tmp->next;
	};
	
	fprintf(TARGET, "%c\n%c", (char) tmp->data, (char) EOF);
	fclose(TARGET);
	return 1;
}


int perform_required_action(WINDOW *window){
	uint32_t input;
	int      startpos;
take_cmd_again:
	action[0] = '-';
	action[1] = '-';
	action[2] = '-';
	startpos  = 10;

	for(int x = 0; x < 3; x++){
		input = getch();
		if(input == ENTER || input == KEY_ENTER){
			invoke_actions(action, window);
			return 1;
		} else if(input == BACKSPACE || input == KEY_BACKSPACE){
			x = -1;
			startpos = 10;
			action[0] = '-';
			action[1] = '-';
			action[2] = '-';

			attron(A_REVERSE);
			mvprintw(Ymax-1, startpos, "---");
			attroff(A_REVERSE);

			move(Ymax-1, startpos);
		} else {
			action[x] = (char) input;
			mvprintw(Ymax-1, startpos, "%c", action[x]);
			startpos++;
		}
	};

complete_action:
	input = getch();
	if(input == ENTER){
		invoke_actions(action, window);
	} else if(input == BACKSPACE || input == KEY_BACKSPACE){
		startpos = 10;
		action[0] = '-';
		action[1] = '-';
		action[2] = '-';

		attron(A_REVERSE);
		mvprintw(Ymax-1, startpos, "---");
		attroff(A_REVERSE);

		move(Ymax-1, startpos);
		goto take_cmd_again;
	} else {
		goto complete_action;
	}

	return 1;
}

void invoke_actions(char action[2], WINDOW *window){
	if(action[0] == '-' && action[1] == '-' && action[2] == '-'){
		refresh();
		wrefresh(window);
		set_statusbar(Xmax, Ymax, x, y);
	} else if(action[0] == 'w' && action[1] == '-' && action[2] == '-'){
		int saved = save_to_file();
		if(!saved){
			exit(EXIT_FAILURE);
			fstatus.SAVING = NSAVED;
		}
		fstatus.SAVING = SAVED;
		fstatus.FSPEC  = YES; 
		mvprintw(Ymax-1, 10, "[FILE SAVED]");
	} else if(action[0] == 'q' && action[1] == '-' && action[2] == '-'){
		if(fstatus.SAVING == SAVED || fstatus.SAVING == NONE){
			endwin();
			clear();
			exit(EXIT_SUCCESS);
		}
		mvprintw(Ymax-1, 10, "[FILE NOT SAVED]");
	} else if(action[0] == 'w' && action[1] == 'q' && action[2] == '-'){
		int saved = save_to_file();
		if(saved){
			fstatus.SAVING = SAVED;
			endwin();
			clear();
			exit(EXIT_FAILURE);
		}
		fstatus.SAVING = NSAVED;
	} else if(action[0] == 'q' && action[1] == '!' && action[2] == '-'){
		endwin();
		clear();
		exit(EXIT_SUCCESS);
	} else {
		mvprintw(Ymax-1, 10, "[CMD NOT FOUND]");
	};
}

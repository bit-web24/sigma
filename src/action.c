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
extern WINDOW      *window;

int  save_to_file();
int  perform_required_action(WINDOW *window);
void display_required_status(WINDOW *window, int Xmax, int Ymax);
void invoke_actions(char action[2], WINDOW *window);

char action[2];

int save_to_file(){
	if(strcmp(INPUT_FILE, "NEW") == 0){
		TARGET = fopen("/home/bittu/new.txt", "w+");
	} else {
		TARGET = fopen(INPUT_FILE, "w+");
	};

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

	action[0] = '-';
	action[1] = '-';
	action[2] = '-';
	startpos  = 10;

	for(int x = 0; x < 3; x++){
		input = getch();
		if(input == ENTER || input == KEY_ENTER){
			invoke_actions(action, window);
			return 1;
		}
		action[x] = (char) input;
		mvprintw(Ymax-1, startpos, "%c", action[x]);
		startpos++;
	};

complete_action:
	input = getch();
	if(input != ENTER){
		goto complete_action;
	}

	return 1;
}


void display_required_status(WINDOW *window, int Xmax, int Ymax){

}

void invoke_actions(char action[2], WINDOW *window){
	if(action[0] == '-' && action[1] == '-' && action[2] == '-'){
		refresh();
		wrefresh(window);
	} else if(action[0] == 'w' && action[1] == '-' && action[2] == '-'){
		int saved = save_to_file();
		if(!saved){
			exit(EXIT_FAILURE);
			fstatus.SAVING = NSAVED;
		}
		fstatus.SAVING = SAVED;
	} else if(action[0] == 'q' && action[1] == '-' && action[2] == '-'){
		if(fstatus.SAVING == SAVED){
			endwin();
			clear();
			exit(EXIT_SUCCESS);
		}
		fstatus.SAVING = NSAVED;
		mvprintw(Ymax-1, 15, "[FILE NOT SAVED]");
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
	};
}

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

extern struct node *head;
extern FILE        *TARGET;
extern char        INPUT_FILE[250];

int  save_to_file();
int  perform_required_action();
void display_required_status(WINDOW *window, int Xmax, int Ymax);
void invoke_actions(char action[2]);

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
	
	fprintf(TARGET, "%c", (char) tmp->data);
	fclose(TARGET);
	return 1;
}


int perform_required_action(){
	uint32_t input;
	char     action[2];

	action[0] = '-';
	action[1] = '-';
	action[2] = '-';

	for(int x = 0; x < 3; x++){
		input = getch();
		if(input == 0){
			return 0;
		}
		if(input == ENTER || input == KEY_ENTER){
			// verify and invoke actions
			invoke_actions(action);
		}
		action[x] = (char) input;
	};

	return 1;
}


void display_required_status(WINDOW *window, int Xmax, int Ymax){

}

void invoke_actions(char action[2]){
	if(strncmp(action, "w--", 3) == 0){
		int saved = save_to_file();
		if(saved){
			//do nothing...
		}
	} else if(strncmp(action, "wq-", 3) == 0){
		int saved = save_to_file();
		if(saved){
			endwin();
			clear();
		}
	} else{
	}
}

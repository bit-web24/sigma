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

extern char **ARGV;
extern int  x;
extern int  y;
extern int  Xmax;
extern int  Ymax;
extern int  lrefresh(WINDOW *window, struct node *head);
extern void add_new_node(unsigned int input);
extern int  TOTAL_LINE_WRITTEN; 
extern struct node *tail;

int         load_buffer();
static int  _load_file_();
static bool INPUT_FILE_PATH();
FILE        *TARGET;
char        INPUT_FILE[250] = "NEW"; 

bool INPUT_FILE_PATH(){
	system("pwd > ~/pwd.txt");
	
	TARGET = fopen("../pwd.txt", "r");
	fscanf(TARGET, "%s", INPUT_FILE);
	fclose(TARGET);

	strcat(INPUT_FILE, "/");
	// do here
	strcat(INPUT_FILE, ARGV[1]);

	char check_input_file[258] = "test -e ";
	strcat(check_input_file, INPUT_FILE);

	int exist = system(check_input_file);
	if(exist == 0){
		return true;
	} else{
		return false;
	}
}

int _load_file_(){
	TARGET = fopen(INPUT_FILE, "r+");
	TOTAL_LINE_WRITTEN = 0;
	int ch;

	while((ch = fgetc(TARGET)) != EOF){
		if(ch == ENTER){
			add_new_node((unsigned int)ch);
			TOTAL_LINE_WRITTEN += 1;
			y++;
			x = 0;
		} else {
			add_new_node((unsigned int)ch);
			x++;
		}
	};

	if(tail->prev == NULL){
		tail = NULL;
		y = 0;
		x = 0;
		TOTAL_LINE_WRITTEN = 0;
	} else {
		tail = tail->prev;
		tail->next = NULL;
	
		x = tail->x;
		y = tail->y;
		if(tail->data == ENTER){
			y++;
			x = 0;
		} else {
			x++;
		};
		TOTAL_LINE_WRITTEN -= 1;
	};

	fclose(TARGET);
	return 0;
};

int load_buffer(){
	bool does_exist = INPUT_FILE_PATH();
	if(does_exist == true){
		int loaded = _load_file_();
		if(loaded == 0){
			return 0;
		}
		exit(EXIT_FAILURE);
	}
	TARGET = fopen(INPUT_FILE, "w");
	if(TARGET != NULL){
		fclose(TARGET);
		return -1;
	}
	exit(EXIT_FAILURE);
};

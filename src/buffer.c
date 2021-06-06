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

int  load_buffer(struct node *head, WINDOW *window);
bool INPUT_FILE_PATH();

bool INPUT_FILE_PATH(){
	FILE *TARGET;
	static char INPUT_FILE[250]; 
	
	system("pwd > ../pwd.txt");
	
	TARGET = fopen("../pwd.txt", "r");
	fscanf(TARGET, "%s", INPUT_FILE);
	fclose(TARGET);

	strcat(INPUT_FILE, "/");
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

int load_buffer(struct node *head, WINDOW *window){
	bool does_exist = INPUT_FILE_PATH();
	if(does_exist == false){
		return 0;
	}
	return 1;
};

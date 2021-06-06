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

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ncurses.h>

#include "global.h"

extern char **ARGV;
static FILE *TARGET;

void load_buffer(struct node *head, WINDOW *window);

void load_buffer(struct node *head, WINDOW *window){
	char INPUT_FILE[250]; 
	uint32_t i = 0;
	char CHAR;

	system("pwd > ~/.local/pwd.txt");
	TARGET = fopen("~/.local/pwd.txt", "r");
	
	while((CHAR = fgetc(TARGET)) != EOF){
		INPUT_FILE[i] = CHAR;
		i++;
	};
	fclose(TARGET);
	strcat(INPUT_FILE, "/");
	strcat(INPUT_FILE, ARGV[1]);
	
	TARGET = fopen(INPUT_FILE, "w");
	fprintf(TARGET, "Absolute Path : %s", INPUT_FILE);
	printf("%s\n", INPUT_FILE);
	fclose(TARGET);
};

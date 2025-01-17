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

#ifndef MAIN_H
#define MAIN_H

struct node *head = NULL;
struct node *tail = NULL;
struct FILE_INFO  fstatus;

unsigned \
int    input;
int    Xmax,
       Ymax,
       Xwin,
       Ywin,
       x, y;
bool   atlast = true;
bool   zoomed_io = false;
int    ARGC;
char   **ARGV;
int    TOTAL_LINE_WRITTEN;

extern int INPUT_FILE[250];

extern void Hscroll(WINDOW *window, struct node *head, int Xmax, int x);
extern void lrefresh(WINDOW *window, struct node *head);
extern void set_statusbar(int Xmax, int Ymax, int x, int y);
extern void clear_statusbar();
extern int  load_buffer();
extern int  save_to_file();
extern int  perform_required_action(WINDOW *window);
#endif

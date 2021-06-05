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

#ifndef C_KEYS
#define C_KEYS

#ifndef ENTER
#define ENTER 10
#endif

#ifndef BACKSPACE
#define BACKSPACE 127
#endif

#ifndef ZOOM_IO
#define ZOOM_IO 410
#endif

struct node {
	struct node *prev;
	unsigned int data;
	int x;
	int y;
	struct node *next;
};

#endif

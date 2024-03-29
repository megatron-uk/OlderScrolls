/* utils_ql.H, prototypes for utility functions.
 Copyright (C) 2021  John Snowdon
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _UTILS_QL_H

unsigned char check_Files(void);
unsigned char word_length(char *c, unsigned short pos, unsigned short string_size);
void * get_FreeBlock(unsigned int *size, unsigned int base, unsigned short increment);

#define _UTILS_QL_H
#endif
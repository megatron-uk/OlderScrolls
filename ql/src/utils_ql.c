/* utils_ql.c, Miscellaneous functions and utilties for Sinclair QL.
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

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef _CONFIG_H
#include "../common/config.h"
#define _CONFIG_H
#endif
#ifndef _UTIL_H
#include "../common/utils.h"
#define _UTIL_H
#endif

unsigned char check_Files(){

	int f;
	unsigned char errors = 0;
		
	// Font file(s)
	f = open(FONT_8X8, O_RDONLY);
	if (f < 0){
		printf("- Error: Unable to find 8x8 font %s\n", FONT_8X8);
		errors++;
	} else {
		//printf("Found %s\n", FONT_8X8);
		close(f);
	}
	
	// ==============================
	// Data files
	// ==============================
	
	// Weapon data
	f = open(WEAPON_DAT, O_RDONLY);
	if (f < 0){
		printf("- Error: Unable to find game data %s\n", WEAPON_DAT);
		errors++;
	} else {
		//printf("Found %s\n", WEAPON_DAT);
		close(f);
	}
	
	// Monster data
	f = open(MONSTER_DAT, O_RDONLY);
	if (f < 0){
		printf("- Error: Unable to find game data %s\n", MONSTER_DAT);
		errors++;
	} else {
		//printf("Found %s\n", MONSTER_DAT);
		close(f);
	}
	
	// Map data
	f = open(ITEM_DAT, O_RDONLY);
	if (f < 0){
		printf("- Error: Unable to find game data %s\n", ITEM_DAT);
		errors++;
	} else {
		//printf("Found %s\n", ITEM_DAT);
		close(f);
	}
	
	// Map data
	f = open(MAP_DAT, O_RDONLY);
	if (f < 0){
		printf("- Error: Unable to find game data %s\n", MAP_DAT);
		errors++;
	} else {
		//printf("Found %s\n", MAP_DAT);
		close(f);
	}
	
	// Story/text data
	f = open(STORY_DAT, O_RDONLY);
	if (f < 0){
		printf("- Error: Unable to find game data %s\n", STORY_DAT);
		errors++;
	} else {
		//printf("Found %s\n", STORY_DAT);
		close(f);
	}
	
	if (errors == 0){
		printf("- All game datafiles present\n");
	}
	return errors;
}

unsigned char word_length(char *c, unsigned short pos){
	// Count from the current position to the next space/newline/carriage return.
	//
	// Returns the number of characters so that a text printing routine
	// can work out if there is enough space to print this word.
	
	unsigned short i;
	unsigned short count;
	
	count = 0;
	for (i = pos; i <strlen(c); i++){
		if ((c[i] == 0x20) || (c[i] == 0x0A) || (c[i] == 0x0D)){
			// Newline/space found at this position
			// Return the number of characters that are needed to print
			// this section of text
			return count;	
		}
		count++;
	}
	// No more spaces found
	return count;
}

void * get_FreeBlock(unsigned int *size, unsigned int base, unsigned char increment){
	// Returns the biggest free block of memory than can be allocated
	
	unsigned char *mem;
	
	mem = malloc(base);
	if (mem != NULL){
		
		// Allocate until we are no longer able to
		while (mem != NULL){
			free(mem);
			base += increment;
			mem = malloc(base);
		}
		
		// Wind back until we get a free block increment
		while (mem == NULL){
			base -= 1;
			mem = malloc(base);
		}
		
		*size = base;
		return mem;
	} 
	*size = 0;
	return NULL;
}
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

#include <string.h>
#include <stdio.h>

#ifndef _CONFIG_H
#include "../common/config.h"
#define _CONFIG_H
#endif
#ifndef _UTIL_H
#include "../common/utils.h"
#define _UTIL_H
#endif

unsigned char check_Files(){

	FILE *f;
	unsigned char errors = 0;
	
	printf("Checking game datafiles...\n");
	
	// Font file(s)
	f = fopen(FONT_8X8, "r");
	if (f == NULL){
		printf("Error: Unable to find 8x8 font %s\n", FONT_8X8);
		errors++;
	} else {
		printf("Found %s\n", FONT_8X8);
		fclose(f);
	}
	
	// ==============================
	// Data files
	// ==============================
	
	// Weapon data
	f = fopen(WEAPON_DAT, "r");
	if (f == NULL){
		printf("Error: Unable to find game data %s\n", WEAPON_DAT);
		errors++;
	} else {
		printf("Found %s\n", WEAPON_DAT);
		fclose(f);
	}
	
	// Monster data
	f = fopen(MONSTER_DAT, "r");
	if (f == NULL){
		printf("Error: Unable to find game data %s\n", MONSTER_DAT);
		errors++;
	} else {
		printf("Found %s\n", MONSTER_DAT);
		fclose(f);
	}
	
	// Map data
	f = fopen(ITEM_DAT, "r");
	if (f == NULL){
		printf("Error: Unable to find game data %s\n", ITEM_DAT);
		errors++;
	} else {
		printf("Found %s\n", ITEM_DAT);
		fclose(f);
	}
	
	// Map data
	f = fopen(MAP_DAT, "r");
	if (f == NULL){
		printf("Error: Unable to find game data %s\n", MAP_DAT);
		errors++;
	} else {
		printf("Found %s\n", MAP_DAT);
		fclose(f);
	}
	
	if (errors == 0){
		printf("All game datafiles present\n");
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
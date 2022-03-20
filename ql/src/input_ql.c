/* input_ql.h. Functions for the Sinclair QL input routines in OlderScrolls-Engine.
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
#include <qdos.h>

#ifndef _DRAW_H
#include "../common/draw.h"
#endif
#ifndef _INPUT_H
#include "../common/input.h"
#endif

unsigned char input_allowed[MAX_ALLOWED_INPUTS];

unsigned char input_Get(Screen_t *screen){
	// Listens for input and returns it, if it is present as
	// one of the allowed types for this game area.
	//
	// Screen should already have been initialised before calling.
	
	unsigned char c;
	unsigned i;
	short v;
	
	v = io_fbyte(screen->win, 0, (char *) &c);
	if (v != ERR_OK){
		return 0;	
	}
	for (i = 0; i < MAX_ALLOWED_INPUTS; i++){
		if (c == input_allowed[i]){
			return c;
		}
	}
	return 0;
}

void input_Set(unsigned char key){
	// Sets a new allowed input key in the list of allowed types
	// for this specific game area.
	
	unsigned char i;
	
	for (i = 0; i < MAX_ALLOWED_INPUTS; i++){
		
		// Don't allow duplicate inputs
		if (input_allowed[i] == key){
			return;
		}
		
		// Set the new input
		if (input_allowed[i] == 0){
			input_allowed[i] = key;
			return;
		}	
	}
}

void input_Clear(void){
	// Clears the list of allowed input types, ready for a new
	// game area to be loaded.
	unsigned char i;
	
	for (i = 0; i < MAX_ALLOWED_INPUTS; i++){
		input_allowed[i] = 0;
	}
}

void input_Wait(Screen_t *screen, unsigned char key){
	// Quickly set a single key type and wait for it
	
	input_Clear();
	input_Set(key);
	while (input_Get(screen) == 0){
	}
	input_Clear();
}

unsigned short input_WaitTimer(Screen_t *screen, unsigned char key){
	// Quickly set a single key type and wait for it
	// Returns the number of iterations until the key is pressed
	
	unsigned int i = 0;
	
	input_Clear();
	input_Set(key);
	while (input_Get(screen) == 0){
		i++;
	}
	input_Clear();
	return i;
}

void input_WaitAndReturn(Screen_t *screen){
	// Wait for any current key (or escape) to be pressed
	
	input_Set(INPUT_CANCEL);
	while (input_Get(screen) == 0){
	}
}
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
#define _DRAW_H
#endif
#ifndef _INPUT_H
#include "../common/input.h"
#define _INPUT_H
#endif

unsigned char input_allowed[MAX_ALLOWED_INPUTS];

unsigned char input_Get(void){
	// Listens for input and returns it, if it is present as
	// one of the allowed types for this game area.
	//
	// Screen should already have been initialised before calling.
	
	unsigned char c;
	unsigned i;
	short v;
	
	v = io_fbyte(screen.win, 0, (char *) &c);
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

void input_Wait(unsigned char key){
	// Quickly set a single key type and wait for it
	
	input_Clear();
	input_Set(key);
	while (input_Get() == 0){
	}
	input_Clear();
}
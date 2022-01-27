/* main_ql.c, Runtime for the Sinclair QL version of OlderScrolls-Engine.
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

#include <stdio.h>
#include <stdlib.h>
#include <qdos.h>

// Platform agnostic functions
#ifndef _MAIN_H
#include "../common/main.h"
#define _MAIN_H
#endif
#ifndef _CONFIG_H
#include "../common/config.h"
#define _CONFIG_H
#endif
#ifndef _UI_H
#include "../common/ui.h"
#define _UI_H
#endif
#ifndef _DRAW_H
#include "../common/draw.h"
#define _DRAW_H
#endif
#ifndef _UTIL_H
#include "../common/utils.h"
#define _UTIL_H
#endif
#ifndef _INPUT_H
#include "../common/input.h"
#define _INPUT_H
#endif
#ifndef _DATA_H
#include "../common/data.h"
#define _DATA_H
#endif
#ifndef _GAME_H
#include "../common/game.h"
#define _GAME_H
#endif

// Options to the C68 runtime environment
long _stack = 1 * 1024L; 		// Set size of stack, in kb. Defaults to 4kb.
long _mneed = 4 * 1024L; 		// Minimum heap size allowed
long _memincr = 2 * 1024L; 		// When heap exhausted, requests to QDOS are in this size
long _memqdos = 10 * 1024L; 	// Minimum amount of memory that is allowed to remain for QDOS ...
								// ... this defaults to 20kb, so reducing it to 10kb gives us another
								// 10kb of heap to play with.

int main(void){
	
	char c;
	unsigned char main_exit = 0;
	
	GameState_t *gamestate = NULL;
	LevelState_t *levelstate = NULL;
	gamestate = (GameState_t *) calloc(sizeof(GameState_t), 1);
	levelstate = (LevelState_t *) malloc(sizeof(LevelState_t));
	
	printf("%s starting...\n", ENGINE_NAME);
	
	// Check that all game objects are present
	c = check_Files();
	c = 0;
	if (c != 0){
		printf("- Error: One or more game datafiles are missing! [Err:%d]\n", c);
		return(-1);
	}
	
	// Try to initialise screen/video memory interface
	c = screen_Init();
	if (c != 0){
		printf("- Error: Screen could not be initialised! [Err:%d]\n", c);
		return(-1);
	} else {
		printf("- Screen structures initialised\n");	
	}
	
	printf("\nPress any key to begin full screen mode...\n");
	input_Wait(INPUT_CONFIRM);
	
	// Initialise game data and open datafiles
	game_Init(gamestate, levelstate);
	
	// Show the adventure-specific splash screen
	game_Splash(gamestate, levelstate);
	
	// Main game loop
	while(main_exit == 0){
		
		switch(gamestate->gamemode){
		
			case GAME_MODE_MAP:
				game_Map(gamestate, levelstate);
				break;
			
			case GAME_MODE_COMBAT:
				break;
				
			case GAME_MODE_SHOP:
				break;
				
			case GAME_MODE_EXIT:
				main_exit = 1;
				break;
				
			default:
				break;
		}
	}
	
	screen_Exit();
	game_Exit();
	return(0);
}

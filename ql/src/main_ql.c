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
#ifndef _ERROR_H
#include "../common/error.h"
#define _ERROR_H
#endif

// Options to the C68 runtime environment
long _stack = 1 * 4024L; 		// Set size of stack, in kb. Defaults to 4kb.
long _mneed = 4 * 1024L; 		// Minimum heap size allowed
long _memincr = 2 * 4024L; 		// When heap exhausted, requests to QDOS are in this size
long _memqdos = 10 * 1024L; 		// Minimum amount of memory that is allowed to remain for QDOS ...
								// ... this defaults to 20kb, so reducing it to 10kb gives us another
								// 10kb of heap to play with - very useful for an unexpanded 128kb machine.

int main(void){
	
	char c = 0;
	unsigned char main_exit = 0;
	
	GameState_t *gamestate = NULL;
	LevelState_t *levelstate = NULL;
	Screen_t *screen = NULL;
	
	printf("%s starting...\n", ENGINE_NAME);
	
	// Allocate memory for basic data structures
	gamestate = (GameState_t *) calloc(sizeof(GameState_t), 1);
	levelstate = (LevelState_t *) calloc(sizeof(LevelState_t), 1);
	screen = (Screen_t *) calloc(sizeof(Screen_t), 1);
	if ((gamestate == NULL) || (levelstate == NULL) || (screen == NULL)){
		printf("- Error: Unable to allocate memory for essential data!");
		printf("- Error: This Sinclair QL target requires 256KB in order");
		printf("- Error: to run the OlderScrolls RPG engine!");
		return(-1);
	}
	
	// Check that all game objects are present
	c = check_Files();
	c = 0;
	if (c != 0){
		printf("- Error: One or more game datafiles are missing! [Err:%d]\n", c);
		return(MAIN_DATAFILES_MISSING);
	}
	
	// Try to initialise screen/video memory interface
	c = screen_Init(screen);
	if (c != 0){
		printf("- Error: Screen could not be initialised! [Err:%d]\n", c);
		return(MAIN_SCREEN_FAILURE);
	}
	
	printf("\nPress return to begin full screen mode...\n");
	// The keyboard input wait also initialises random seed #1
	gamestate->seed1 = input_WaitTimer(screen, INPUT_CONFIRM); 
		
	// Initialise game data and open any initial datafiles 
	// (splash text, first level location)
	game_Init(screen, gamestate, levelstate);
	
	// Show the adventure-specific splash screen
	// ... and initialise random seed #2
	game_Splash(screen, gamestate, levelstate);
	
	while(main_exit == 0){
		
		switch(gamestate->gamemode){
		
			case GAME_MODE_COMBAT:
				//game_Combat();
				break;
				
			case GAME_MODE_SHOP:
				//game_Shop();
				break;
				
			case GAME_MODE_EXIT:
				main_exit = 1;
				break;
			
			case GAME_MODE_MAP:
				game_Map(screen, gamestate, levelstate);
				break;
				
			default:
				break;
		}
	}
	
	screen_Exit(screen);
	game_Exit(screen);
	return(OK);
}

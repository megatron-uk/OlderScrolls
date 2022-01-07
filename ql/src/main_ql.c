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
#ifndef _GAME_H
#include "../common/game.h"
#define _GAME_H
#endif

int main(void){
	
	unsigned char c;
	unsigned char exit = 0;
	
	GameState_t *gamestate = NULL;
	LevelState_t *levelstate = NULL;
	gamestate = (GameState_t *) malloc(sizeof(GameState_t));
	levelstate = (LevelState_t *) malloc(sizeof(LevelState_t));
	
	printf("%s %s starting...\n", ENGINE_NAME, ENGINE_TARGET_NAME);
	printf("- Game state: %d bytes\n", sizeof(GameState_t));
	printf("- Level state: %d bytes\n", sizeof(LevelState_t));
	
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
	while(exit == 0){
		
		switch(gamestate->gamemode){
		
			case GAME_MODE_MAP:
				game_Map(gamestate, levelstate);
				break;
			
			case GAME_MODE_COMBAT:
				break;
				
			case GAME_MODE_SHOP:
				break;
				
			case GAME_MODE_EXIT:
				exit = 1;
				break;
				
			default:
				break;
		}
	}
	
	screen_Exit();
	game_Exit();
	return(0);
}

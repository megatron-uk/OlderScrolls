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
	
	printf("Engine starting up...\n");
	
	// Check that all game objects are present
	check_Files();
	/*
	if (check_Files() != 0){
		printf("One or more game datafiles are missing!\n");
		return -1;
	};
	*/
	
	// Try to initialise screen/video memory interface
	c = screen_Init();
	if (c != 0){
		printf("Screen could not be initialised! [Err:%d]\n", c);
		return -1;
	} else {
		printf("Screen structures initialised\n");	
	}
	
	
	printf("Press any key to begin full screen mode...\n");
	input_Wait(INPUT_RETURN);
	
	// Initialise game data and open datafiles
	game_Init(gamestate, levelstate);
	
	// Draw the main interface
	ui_Draw(gamestate, levelstate);
	draw_Flip();
	
	// Show the adventure-specific splash screen
	game_Splash(gamestate, levelstate);
	
	// Main game loop
	//input_Wait(INPUT_RETURN);
	//while(exit == 0){
		
		// Run main game logic
		// game_Main();
		
		// Flip screen buffer if marked as dirty
		//draw_Flip();
	//}
	
	screen_Exit();
	game_Exit();
	return(0);
}

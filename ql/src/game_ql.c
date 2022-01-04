/* game_ql.c, Main game logic for the Sinclair QL version of OlderScrolls-Engine.
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
#include <string.h>

#ifndef _CONFIG_H
#include "../common/config.h"
#define _CONFIG_H
#endif
#ifndef _GAME_H
#include "../common/game.h"
#define _GAME_H
#endif
#ifndef _DATA_H
#include "../common/data.h"
#define _DATA_H
#endif
#ifndef _INPUT_H
#include "../common/input.h"
#define _INPUT_H
#endif

FILE *story_file;
FILE *map_file;
FILE *weapon_file;
FILE *item_file;
FILE *monster_file;
FILE *index_file;

void game_Init(){
	// Load initial data for the currently selected game
	//
	// This does such things as:
	// - Load story text for the splash screen
	// - Populate Player character details
	// - Set initial map location
	
	unsigned short i;
	
	//story_file = fopen(STORY_DAT, "r");
	//weapon_file = fopen(WEAPON_DAT, "r");
	//monster_file = fopen(MONSTER_DAT, "r");
	//item_file = fopen(ITEM_DAT, "r");
	map_file = fopen(MAP_DAT, "r");
	
	// Open the story data file and load entry 0 - this has the splash screen data
	//data_Load(DATA_TYPE_STORY, 0);
	
	// Open the Map data file and load entry 1 - this will be our starting location
	data_Load(DATA_TYPE_MAP, 3);
	
	// Initialise a new player character
	
	// Initialise game state
	//gamestate.level = 1;
	//gamestate.level_previous = 1;
	//memset(gamestate.level_visits, 0, MAX_LOCATIONS); 
	//memset(gamestate.level_defeated_primary, 0, MAX_LOCATIONS); 
	//memset(gamestate.level_defeated_secondary, 0, MAX_LOCATIONS); 
	//gamestate.gold = 0;
}

void game_Exit(){
	// Close any open data files
	
	if (story_file != NULL){
		fclose(story_file);
	}
	
	if (map_file != NULL){
		fclose(map_file);
	}
	
	if (weapon_file != NULL){
		fclose(weapon_file);
	}
	
	if (item_file != NULL){
		fclose(item_file);
	}
	
	if (monster_file != NULL){
		fclose(monster_file);
	}
}

void game_Splash(){
	// Shows a splash screen before the main game itself
	//
	// Shows basic information about OlderScrolls engine,
	// as well as title and introductory text from the 
	// location 0 entry of the currently loaded map file.
	//
	// Shows the screen, prompts for a basic set of options:
	// Start, Exit, <TO DO>, <TO DO>
	// ... then starts the game engine proper
	
	//input_Clear();
	//input_Set(INPUT_RETURN);
	
	ui_DrawSplashText();
	
	// Wait for user input
}
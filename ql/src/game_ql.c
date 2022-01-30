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
#include <stdlib.h>

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
#ifndef _UI_H
#include "../common/ui.h"
#define _UI_H
#endif
#ifndef _CONDITIONS_H
#include "../common/conditions.h"
#define _CONDITIONS_H
#endif

FILE *story_file;
FILE *map_file;
FILE *weapon_file;
FILE *item_file;
FILE *monster_file;
FILE *index_file;

void game_Init(GameState_t *gamestate, LevelState_t *levelstate){
	// Load initial data for the currently selected game
	//
	// This does such things as:
	// - Load story text for the splash screen
	// - Populate Player character details
	// - Set initial map location
	
	unsigned short i;
	
	// Initialise game state
	gamestate->gamemode = GAME_MODE_MAP;
	gamestate->level = 1;
	gamestate->level_previous = 1;
	gamestate->gold = 0;
	gamestate->counter = 0;
	gamestate->npcs = NULL;
	gamestate->players = (PartyState_t *) calloc(sizeof(PlayerState_t), 1);
	for (i = 0; i < MAX_PLAYERS; i++){
		gamestate->players->player[i] = (PlayerState_t *) calloc(sizeof(PlayerState_t), 1);
	}
	gamestate->enemies = (EnemyState_t *) calloc(sizeof(EnemyState_t), 1);
	for (i = 0; i < MAX_MONSTER_TYPES; i++){
		gamestate->enemies->enemy[i] = (PlayerState_t *) calloc(sizeof(PlayerState_t), 1);
	}
	
	// Open the story data file and load entry 0 - this has the adventure name
	data_LoadStory(gamestate, levelstate, 0);
	strncpy((char *) gamestate->name, (char *) gamestate->text_buffer, MAX_LEVEL_NAME_SIZE);
	
	// Open the story data file and load entry 1 - this has the splash screen data
	data_LoadStory(gamestate, levelstate, 1);
	sprintf(gamestate->text_buffer, gamestate->buf);
	
	// Open the Map data file and load entry 1 - this will be our starting location
	data_LoadMap(gamestate, levelstate, 1);
	gamestate->level_visits[1] = 1;
	
	// Initialise a new player character and their sprites
	data_CreateCharacter(gamestate->players->player[0], screen.players[0]);
}

void game_Exit(){
	// Close any open data files
	
}

void game_Splash(GameState_t *gamestate, LevelState_t *levelstate){
	// Shows a splash screen before the main game itself
	//
	// Shows basic information about OlderScrolls engine,
	// as well as title and introductory text from the 
	// location 0 entry of the currently loaded map file.
	//
	// Shows the screen, prompts for a basic set of options:
	// Start, Exit, <TO DO>, <TO DO>
	// ... then starts the game engine proper
	unsigned char i;
	
	draw_Clear();
	ui_Draw(gamestate, levelstate);
	ui_DrawSplashText(gamestate, levelstate);
	draw_Flip();
	
	// Wait for user input
	input_Wait(INPUT_CONFIRM);
}

void game_Map(GameState_t *gamestate, LevelState_t *levelstate){
	// The main game mode - navigating the map and reading text/viewing story locations
	// with options for navigation, talking etc.
	
	unsigned char c = 0;
	unsigned char e = 0;
	unsigned short remain = 0;
	
	// Clear input
	input_Clear();
	input_Set(INPUT_QUIT);
	input_Set(INPUT_QUIT_);
	input_Set(INPUT_DEBUG);
	
	// Redraw the main screen
	ui_Draw(gamestate, levelstate);
	
	// Open the Map data file and load current level
	if (gamestate->level != gamestate->level_previous){
		data_LoadMap(gamestate, levelstate, gamestate->level);
		
		// Record a visit to this location
		if (gamestate->level_visits[gamestate->level] < 255){
			gamestate->level_visits[gamestate->level]++;
		}
	}
	ui_DrawLocationName(gamestate, levelstate);
	
	// Show default map location text
	data_LoadStory(gamestate, levelstate, levelstate->text);
	sprintf(gamestate->text_buffer, gamestate->buf);
	
	// Have primary monsters spawned here previously?
	// Display additional after_spawn text
	
	// Have secondary monsters spawned here previously?
	// Display additional after_respawn text
	
	// Apply any ongoing status effects (bleeding, poison, etc)
	// Display any status effect text
	
	// Display party sidebar
	ui_DrawSideBar(gamestate, levelstate);
	
	// If monsters are spawned - go to combat, but print before_spawn or before_respawn text
	c = game_CheckMonsterSpawn(gamestate, levelstate, 0, 1);
	if(c){
		
		// Add fight and withdraw options
		input_Set(INPUT_FIGHT);
		input_Set(INPUT_FIGHT_);
		input_Set(INPUT_W);
		input_Set(INPUT_W_);
		
		if (c == 1){
			// Show primary spawn text
			
		}
		
		if (c == 2){
			// Show secondary spawn text
			
		}
	
		// Add 'fight' or 'retreat' options
		// input_Set(INPUT_FIGHT);
		// input_Set(INPUT_FIGHT_);
		// input_Set(INPUT_W);
		// input_Set(INPUT_W_);
			
	// else if not spawned
	} else {
		
		data_AddNPC(gamestate, levelstate, 1);
		
		// Are any NPC's active?
		// Add 'talk' options
		//if (game_CheckTalk(gamestate, levelstate, 0, (char *) input_allowed)){
		//	input_Set(INPUT_TALK);
		//	input_Set(INPUT_TALK_);
		//}
		
		// Are there any shops in this location?
		// Add 'barter' options
		//if (game_CheckShop(gamestate, levelstate, 0, (char *) input_allowed)){
		//	input_Set(INPUT_BARTER);
		//	input_Set(INPUT_BARTER_);
		//}
		
		// Are there any rest spots (inn/temple/friendly tribe) here?
		// Add 'rest' options
		//if (game_CheckRest(gamestate, levelstate, 0, (char *) input_allowed)){
		//	input_Set(INPUT_REST);
		//	input_Set(INPUT_REST_);
		//}
		
		// Are any exits active? - draw the text and make movement active
		if (game_CheckMovement(gamestate, levelstate, 0, 1)){
			input_Set(INPUT_MOVE);
			input_Set(INPUT_MOVE_);
		}
	}
	
	// Draw the main window text now
	remain = ui_DrawMainWindowText(gamestate, levelstate, 0, (char *) gamestate->text_buffer);
	if (remain > 0){
		// while (remain){
			// Show 'press button to view next page'
			// Show next page
		//}
	}
	
	// Draw the available options in the status bar	
	ui_DrawStatusBar(gamestate, levelstate, 1, 1);
	draw_Flip();
	
	// Wait for user input
	while(!e){
		c = input_Get();
		switch(c){
			case INPUT_DEBUG:
				// ======================================
				// Show the memory/used game/progress screen
				// ======================================
				input_Clear();
				input_Set(INPUT_CANCEL);
				ui_DebugScreen(gamestate, levelstate);
				draw_Flip();
				while(!e){
					c = input_Get();
					switch(c){
						case INPUT_CANCEL:
							e = 1;
							break;
						default:
							break;
					}
				}
			case INPUT_MOVE:
			case INPUT_MOVE_:
				// ======================================
				// Allow player to move to another location
				// ======================================
				game_CheckMovement(gamestate, levelstate, 1, 0);
				while(!e){
					c = input_Get();
					switch(c){
						case INPUT_N:
						case INPUT_N_:
							gamestate->level_previous = gamestate->level;
							gamestate->level = levelstate->north;
							e = 1;
							break;
						case INPUT_S:
						case INPUT_S_:
							gamestate->level_previous = gamestate->level;
							gamestate->level = levelstate->south;
							e = 1;
							break;
						case INPUT_E:
						case INPUT_E_:
							gamestate->level_previous = gamestate->level;
							gamestate->level = levelstate->east;
							e = 1;
							break;
						case INPUT_W:
						case INPUT_W_:
							gamestate->level_previous = gamestate->level;
							gamestate->level = levelstate->west;
							e = 1;
							break;
						case INPUT_CANCEL:
							e = 1;
							break;
						default:
							break;
					}
				}
				break;
			case INPUT_FIGHT:
			case INPUT_FIGHT_:
				// ======================================
				// Set combat game mode
				// ======================================
				//game_Combat();
				// if alive
				// 		Redraw the ui
				// else
				//		show game over screen
				break;
			case INPUT_TALK:
			case INPUT_TALK_:
				// ======================================
				// Set talk game mode
				// ======================================
				//game_Talk();
				// e = 1;
				break;	
			//case INPUT_BARTER:
			//case INPUT_BARTER_:
				// ======================================
				// Set shop game mode
				// ======================================
				//game_Shop();
				// e = 1;
				//break;
			//case INPUT_REST:
			//case INPUT_REST_:
				// ======================================
				// Set rest game mode
				// ======================================
				//game_Rest();
				// e = 1;
				//break;	
			case INPUT_QUIT:
			case INPUT_QUIT_:
				// ======================================
				// Set exit game mode
				// ======================================
				game_Quit(gamestate, levelstate);
				e = 1;
				break;
			default:
				break;
		}
	}
	return;
}

void game_Combat(GameState_t *gamestate, LevelState_t *levelstate){
	// Combat mode - cannot exit this until the combat is resolved
}

void game_Shop(GameState_t *gamestate, LevelState_t *levelstate){
	// At a shop
}

void game_Quit(GameState_t *gamestate, LevelState_t *levelstate){
	// Display the 'Do you want to quit?' popup and
	// wait for user input
	
	unsigned char c;
	unsigned char e = 0;
	
	// Set all the inputs we allow
	input_Clear();
	input_Set(INPUT_Y);
	input_Set(INPUT_Y_);
	input_Set(INPUT_N);
	input_Set(INPUT_N_);
	input_Set(INPUT_CANCEL);
	
	// Draw a yes/no dialogue box
	ui_DrawYesNo((unsigned char *) "Really Quit?");
	draw_Flip();
	
	// Wait for user input
	while(!e){
		c = input_Get();
		switch(c){
			case INPUT_Y:
			case INPUT_Y_:
				gamestate->gamemode = GAME_MODE_EXIT;
				e = 1;
				break;
			case INPUT_N:
			case INPUT_N_:
			case INPUT_CANCEL:
				e = 1;
				break;
			default:
				break;
		}
	}
	return;
}

unsigned char game_CheckMonsterSpawn(GameState_t *gamestate, LevelState_t *levelstate, unsigned char add_inputs, unsigned char add_text){
	// Returns a flag indicating if combat is going to happen
	// Checks both primary and secondary spawning rules
	
	unsigned char can_fight = 0;
	
	// Are there any monster ID's listed as primary spawn?
	if (levelstate->spawn_number){
		if (check_Cond(gamestate, levelstate, levelstate->spawn_require, levelstate->spawn_require_number, levelstate->spawn_eval_type)){
			can_fight = 1;
		}
	}
	
	// Only if the primary spawn is false do we check secondary spawn
	if (!can_fight){
		if (levelstate->respawn_number){
			if (check_Cond(gamestate, levelstate, levelstate->respawn_require, levelstate->respawn_require_number, levelstate->respawn_eval_type)){
				can_fight = 2;
			}
		}
	}
	return can_fight;
	
}

unsigned char game_CheckMovement(GameState_t *gamestate, LevelState_t *levelstate, unsigned char add_inputs, unsigned char add_text){
	// Returns a flag indicating if movement is possible.
	// Adds compass points to input options if set
	// Prints direction text to main ui if set

	unsigned char add_it = 0;
	unsigned char can_move = 0;
	char first = 0x0A;
	
	if (add_inputs){
		input_Clear();
	}
	if (levelstate->north || levelstate->south || levelstate->east || levelstate->west){
		
		// Test individual compass points
		if (levelstate->north){
			add_it = 1;
			// Check conditions
			if (levelstate->north_require_number > 0){
				add_it = 0;
				if (check_Cond(gamestate, levelstate, levelstate->north_require, levelstate->north_require_number, levelstate->north_eval_type)){
					// Add option
					add_it = 1;
				}
			}
			if (add_it){
				if (add_inputs){
					input_Set(INPUT_N);
					input_Set(INPUT_N_);
				}
				can_move = 1;
				if (add_text){
					data_LoadStory(gamestate, levelstate, levelstate->north_text);
					sprintf(gamestate->text_buffer + strlen(gamestate->text_buffer), "%c\n%s\n", first, gamestate->buf);
					first = 0;
				}
			}
		}
		if (levelstate->south){
			add_it = 1;
			// Check conditions
			if (levelstate->south_require_number > 0){
				add_it = 0;
				if (check_Cond(gamestate, levelstate, levelstate->south_require, levelstate->south_require_number, levelstate->south_eval_type)){
					// Add option
					add_it = 1;
				}
			}
			if (add_it){
				if (add_inputs){
					input_Set(INPUT_S);
					input_Set(INPUT_S_);
				}
				can_move = 1;
				if (add_text){
					data_LoadStory(gamestate, levelstate, levelstate->south_text);
					sprintf(gamestate->text_buffer + strlen(gamestate->text_buffer), "%c\n%s\n", first, gamestate->buf);
					first = 0;
				}
			}
		}
		if (levelstate->east){
			add_it = 1;
			// Check conditions
			if (levelstate->east_require_number > 0){
				add_it = 0;
				if (check_Cond(gamestate, levelstate, levelstate->east_require, levelstate->east_require_number, levelstate->east_eval_type)){
					// Add option
					add_it = 1;
				}
			}
			if (add_it){
				if (add_inputs){
					input_Set(INPUT_E);
					input_Set(INPUT_E_);
				}
				can_move = 1;
				if (add_text){
					data_LoadStory(gamestate, levelstate, levelstate->east_text);
					sprintf(gamestate->text_buffer + strlen(gamestate->text_buffer), "%c\n%s\n", first, gamestate->buf);
					first = 0;
				}
			}
		}
		if (levelstate->west){
			add_it = 1;
			// Check conditions
			if (levelstate->west_require_number > 0){
				add_it = 0;
				if (check_Cond(gamestate, levelstate, levelstate->west_require, levelstate->west_require_number, levelstate->west_eval_type)){
					// Add option
					add_it = 1;
				}
			}
			if (add_it){
				if (add_inputs){
					input_Set(INPUT_W);
					input_Set(INPUT_W_);
				}
				can_move = 1;
				if (add_text){
					data_LoadStory(gamestate, levelstate, levelstate->west_text);
					sprintf(gamestate->text_buffer + strlen(gamestate->text_buffer), "%c\n%s\n", first, gamestate->buf);
					first = 0;
				}
			}
		}
	}
	if (add_inputs){
		input_Set(INPUT_CANCEL);
		if (can_move){
			ui_DrawNavigation(gamestate, levelstate);
			draw_Flip();	
		}
	}
	return can_move;
}
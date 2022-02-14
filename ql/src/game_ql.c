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

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef _CONFIG_H
#include "../common/config.h"
#endif
#ifndef _GAME_H
#include "../common/game.h"
#endif
#ifndef _DATA_H
#include "../common/data.h"
#endif
#ifndef _INPUT_H
#include "../common/input.h"
#endif
#ifndef _UI_H
#include "../common/ui.h"
#endif
#ifndef _CONDITIONS_H
#include "../common/conditions.h"
#endif

FILE *story_file;
FILE *map_file;
FILE *weapon_file;
FILE *item_file;
FILE *monster_file;
FILE *index_file;

void game_Init(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate){
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
	gamestate->npcs = (struct NPCList *) calloc(sizeof(struct NPCList), 1);
	gamestate->players = (PartyState_t *) calloc(sizeof(PlayerState_t), 1);
	for (i = 0; i < MAX_PLAYERS; i++){
		gamestate->players->player[i] = (PlayerState_t *) calloc(sizeof(PlayerState_t), 1);
	}
	gamestate->enemies = (EnemyState_t *) calloc(sizeof(EnemyState_t), 1);
	for (i = 0; i < MAX_MONSTER_TYPES; i++){
		gamestate->enemies->enemy[i] = (PlayerState_t *) calloc(sizeof(PlayerState_t), 1);
	}
	
	// Open the story data file and load entry 0 - this has the adventure name
	data_LoadStory(screen, gamestate, levelstate, 0);
	strncpy((char *)gamestate->name, (char *)gamestate->buf, MAX_LEVEL_NAME_SIZE);
	
	// Open the story data file and load entry 1 - this has the splash screen data
	data_LoadStory(screen, gamestate, levelstate, 1);
	sprintf(gamestate->text_buffer, gamestate->buf);
	
	// Open the Map data file and load entry 1 - this will be our starting location
	data_LoadMap(screen, gamestate, levelstate, 1);
	gamestate->level_visits[1] = 1;
	
	// Initialise a new player character and their sprites
	data_CreateCharacter(screen, gamestate->players->player[0], screen->players[0], NULL, CHARACTER_TYPE_MONSTER, 1);
	for (i = 1; i < MAX_PLAYERS; i++){
		data_CreateCharacter(screen, gamestate->players->player[i], screen->players[i], NULL, CHARACTER_TYPE_MONSTER, 0);
	}
}

void game_Exit(Screen_t *screen){
	// Close any open data files
	// Clear screen
	// Return to previous screen mode
	
	draw_Clear(screen);
}

void game_Splash(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate){
	// Shows a splash screen before the main game itself
	//
	// Shows basic information about OlderScrolls engine,
	// as well as title and introductory text from the 
	// location 0 entry of the currently loaded map file.
	//
	// Shows the screen, prompts for a basic set of options:
	// Start, Exit, <TO DO>, <TO DO>
	// ... then starts the game engine proper
	
	draw_Clear(screen);
	ui_Draw(screen, gamestate, levelstate);
	ui_DrawSplashText(screen, gamestate, levelstate);
	draw_Flip(screen);
	
	// Wait for user input
	input_Wait(screen, INPUT_CONFIRM);
}

void game_Map(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate){
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
	ui_Draw(screen, gamestate, levelstate);
	
	// Open the Map data file and load current level
	if (gamestate->level != gamestate->level_previous){
		data_LoadMap(screen, gamestate, levelstate, gamestate->level);
		
		// Record a visit to this location
		if (gamestate->level_visits[gamestate->level] < 255){
			gamestate->level_visits[gamestate->level]++;
		}
	}
	ui_DrawLocationName(screen, gamestate, levelstate);
	
	// Load default map location text - but don't display yet, 
	// as we may need to append additional text to it (exits, monster details, etc)
	data_LoadStory(screen, gamestate, levelstate, levelstate->text);
	sprintf(gamestate->text_buffer, gamestate->buf);
	
	// Have primary monsters spawned here previously?
	// Display additional after_spawn text
	
	// Have secondary monsters spawned here previously?
	// Display additional after_respawn text
	
	// Apply any ongoing status effects (bleeding, poison, etc)
	// Display any status effect text
	
	// Display party sidebar
	ui_DrawSideBar(screen, gamestate, levelstate);
	
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
		
		//data_AddNPC(screen, gamestate, levelstate, 1);
		
		// Are any NPC's active?
		// Add 'talk' options
		if (game_CheckTalk(screen, gamestate, levelstate, 0, 1)){
			input_Set(INPUT_TALK);
			input_Set(INPUT_TALK_);
		}
		
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
		if (game_CheckMovement(screen, gamestate, levelstate, 0, 1)){
			input_Set(INPUT_MOVE);
			input_Set(INPUT_MOVE_);
		}
		
		// Are any items available to take?
		if (game_CheckLoot(screen, gamestate, levelstate, 0, 1)){
			input_Set(INPUT_LOOT);
			input_Set(INPUT_LOOT_);
		}
	}
	
	// Draw the main window text now
	remain = ui_DrawMainWindowText(screen, gamestate, levelstate, 0, (char *) gamestate->text_buffer);
	if (remain > 0){
		// More text than can be shown on one page...
		input_Set(INPUT_N);
		input_Set(INPUT_N_);
	}
	
	// Draw the available options in the status bar	
	ui_DrawStatusBar(screen, gamestate, levelstate, 1, 1);
	
	draw_Flip(screen);
	
	// Wait for user input
	while(!e){
		c = input_Get(screen);
		switch(c){
			case INPUT_DEBUG:
				// ======================================
				// Show the memory/used game/progress screen
				// ======================================
				input_Clear();
				input_Set(INPUT_CANCEL);
				ui_DebugScreen(screen, gamestate, levelstate);
				draw_Flip(screen);
				while(!e){
					c = input_Get(screen);
					switch(c){
						case INPUT_CANCEL:
							e = 1;
							break;
						default:
							break;
					}
				}
			case INPUT_N:
			case INPUT_N_:
				// ======================================
				// Allow player to page through to next page of dialogue
				// ======================================
				remain = ui_DrawMainWindowText(screen, gamestate, levelstate, remain, (char *) gamestate->text_buffer);
				break;
			case INPUT_MOVE:
			case INPUT_MOVE_:
				// ======================================
				// Allow player to move to another location
				// ======================================
				game_CheckMovement(screen, gamestate, levelstate, 1, 0);
				e = 1;
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
				game_CheckTalk(screen, gamestate, levelstate, 1, 0);
				e = 1;
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
				game_Quit(screen, gamestate, levelstate);
				e = 1;
				break;
			default:
				break;
		}
	}
	return;
}

void game_Combat(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate){
	// Combat mode - cannot exit this until the combat is resolved
}

void game_Shop(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate){
	// At a shop
}

void game_Quit(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate){
	// Display the 'Do you want to quit?' popup and
	// wait for user input
	
	unsigned char c;
	
	// Draw a yes/no dialogue box
	c = ui_DrawYesNo(screen, gamestate, levelstate, (char *) "Really Quit?");
	if (c){
		gamestate->gamemode = GAME_MODE_EXIT;
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

unsigned char game_CheckTalk(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned char add_inputs, unsigned char add_text){
	// Returns a flag indicating if there is an NPC to talk to.
	// Prints 'you can talk to <character_name> to the main ui if set
	
	unsigned char add_it = 0;
	unsigned char can_talk = 0;
	unsigned char first = 0x0A;
	unsigned char added_1 = 0;
	unsigned char added_2 = 0;
	unsigned char added_3 = 0;
	
	if (add_inputs){
		input_Clear();
	}
	levelstate->has_npc1 = 0;
	levelstate->has_npc2 = 0;
	
	if (levelstate->npc1 || levelstate->npc2){
		// Check conditions
		if (levelstate->npc1 > 0){
			add_it = 0;
			if (check_Cond(gamestate, levelstate, levelstate->npc1_require, levelstate->npc1_require_number, levelstate->npc1_eval_type)){
				// Add option
				add_it = 1;
				levelstate->has_npc1 = 1;
				// Load NPC into enemy slot 1
				data_CreateCharacter(screen, gamestate->enemies->enemy[1], screen->enemies[1], NULL, CHARACTER_TYPE_NPC, levelstate->npc1);
			}
			if (add_it){
				can_talk = 1;
				if (add_inputs){
					input_Set(INPUT_1);
					added_1 = 1;
				}
			}
		}

		// Check conditions
		if (levelstate->npc2 > 0){
			add_it = 0;
			if (check_Cond(gamestate, levelstate, levelstate->npc2_require, levelstate->npc2_require_number, levelstate->npc2_eval_type)){
				// Add option
				add_it = 1;
				levelstate->has_npc2 = 1;
				// Load NPC into enemy slot 2
				data_CreateCharacter(screen, gamestate->enemies->enemy[2], screen->enemies[2], NULL, CHARACTER_TYPE_NPC, levelstate->npc2);
			}
			if (add_it){
				can_talk = 1;
				if (add_inputs){
					input_Set(INPUT_2);
					added_2 = 1;
				}
			}
		}
		
		// Check conditions
		if (levelstate->npc3 > 0){
			add_it = 0;
			if (check_Cond(gamestate, levelstate, levelstate->npc3_require, levelstate->npc3_require_number, levelstate->npc3_eval_type)){
				// Add option
				add_it = 1;
				levelstate->has_npc3 = 1;
				// Load NPC into enemy slot 3
				data_CreateCharacter(screen, gamestate->enemies->enemy[3], screen->enemies[3], NULL, CHARACTER_TYPE_NPC, levelstate->npc3);
			}
			if (add_it){
				can_talk = 1;
				if (add_inputs){
					input_Set(INPUT_3);
					added_1 = 1;						
				}
			}
		}
		
	}
	if (add_inputs){
		input_Set(INPUT_CANCEL);
		if (can_talk){
			ui_DrawTalkChoice(screen, gamestate, levelstate);
			draw_Flip(screen);	
		}
	}
	return can_talk;
}

unsigned char game_CheckLoot(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned char add_inputs, unsigned char add_text){
	// Returns a flag indicating if items/loot are available.
	// Adds items to input options 
	// Adds loot text to main ui if set
	
	unsigned char add_it = 0;
	unsigned char can_loot = 0;
	
	if ((levelstate->items_number > 0) || (levelstate->weapons_number > 0)){
		if (check_Cond(gamestate, levelstate, levelstate->items_require, levelstate->items_require_number, levelstate->items_eval_type)){
			add_it = 1;
		}
		if (add_it){
			can_loot = 1;
			if (add_inputs){
				input_Set(INPUT_LOOT);
				input_Set(INPUT_LOOT_);
			}
		}
	}
	
	//sprintf(gamestate->buf, "can loot: %d\nitems_number: %d\nweapons_number: %d\n", can_loot, levelstate->items_number, levelstate->weapons_number);
	//ui_DrawError(screen, "Debug: CheckLoot", gamestate->buf, 0);
	
	if (add_inputs){
		input_Set(INPUT_CANCEL);
		//if (can_loot){
			//ui_DrawLootChoice(screen, gamestate, levelstate);
			//draw_Flip(screen);	
		//}
	}
	return can_loot;
}

unsigned char game_CheckMovement(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned char add_inputs, unsigned char add_text){
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
					if (levelstate->north_text > 0){
						data_LoadStory(screen, gamestate, levelstate, levelstate->north_text);
					} else {
						sprintf(gamestate->buf, "You can <r>M<C>ove <g>north<C>.");
					}
					sprintf(gamestate->text_buffer + strlen(gamestate->text_buffer), "%c\n%s\n", first, gamestate->buf);
					first = 0x20;
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
					if (levelstate->south_text > 0){
						data_LoadStory(screen, gamestate, levelstate, levelstate->south_text);
					} else {
						sprintf(gamestate->buf, "You can <r>M<C>ove <g>south<C>.");
					}
					sprintf(gamestate->text_buffer + strlen(gamestate->text_buffer), "%c\n%s\n", first, gamestate->buf);
					first = 0x20;
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
					if (levelstate->east_text > 0){
						data_LoadStory(screen, gamestate, levelstate, levelstate->east_text);
					} else {
						sprintf(gamestate->buf, "You can <r>M<C>ove <g>east<C>.");
					}
					sprintf(gamestate->text_buffer + strlen(gamestate->text_buffer), "%c\n%s\n", first, gamestate->buf);
					first = 0x20;
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
					if (levelstate->west_text > 0){
						data_LoadStory(screen, gamestate, levelstate, levelstate->west_text);
					} else {
						sprintf(gamestate->buf, "You can <r>M<C>ove <g>west<C>.");
					}
					sprintf(gamestate->text_buffer + strlen(gamestate->text_buffer), "%c\n%s\n", first, gamestate->buf);
					first = 0x20;
				}
			}
		}
	}
	if (add_inputs){
		input_Set(INPUT_CANCEL);
		if (can_move){
			ui_DrawNavigationChoice(screen, gamestate, levelstate);
		}
	}
	return can_move;
}
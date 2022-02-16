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

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

// Platform agnostic functions
#ifndef _CONFIG_H
#include "../common/config.h"
#endif
#ifndef _GAME_H
#include "../common/game.h"
#endif
#ifndef _UI_H
#include "../common/ui.h"
#endif
#ifndef _DATA_H
#include "../common/data.h"
#endif
#ifndef _DRAW_H
#include "../common/draw.h"
#endif
#ifndef _INPUT_H
#include "../common/input.h"
#endif
#ifndef _UTILS_H
#include "../common/utils.h"
#endif
#ifndef _ERROR_H
#include "../common/error.h"
#endif
#ifndef _MONSTERS_H
#include "../common/monsters.h"
#endif
#ifndef _ENGINE_H
#include "../common/engine.h"
#endif

void ui_Draw(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate){
	// Draws the main user interface - top bar with game name and turn counter
	// Main story/combat window
	// Side bar with party details
	// Bottom bar with status messages/current options
	
	draw_Clear(screen);
	
	// Main outer edges of screen
	draw_Box(screen, UI_OUTER_BORDER_START_X, UI_OUTER_BORDER_START_Y, SCREEN_WIDTH, SCREEN_HEIGHT - 1, UI_OUTER_BORDER_PX, UI_OUTER_BORDER_COLOUR, PIXEL_CLEAR, MODE_PIXEL_OR);
	
	// Game title header bar
	draw_HLine(screen, UI_OUTER_BORDER_START_X, UI_OUTER_BORDER_START_Y + 11, SCREEN_WIDTH, UI_OUTER_BORDER_COLOUR, 0, MODE_PIXEL_SET);
	draw_HLine(screen, UI_OUTER_BORDER_START_X, UI_OUTER_BORDER_START_Y + 12, SCREEN_WIDTH, UI_OUTER_BORDER_COLOUR, 1, MODE_PIXEL_SET);
	
	// Bottom status box
	draw_HLine(screen, UI_STATUSBAR_START_X, UI_STATUSBAR_START_Y, UI_STATUSBAR_LENGTH, UI_OUTER_BORDER_COLOUR, 0, MODE_PIXEL_SET);
	draw_HLine(screen, UI_STATUSBAR_START_X, UI_STATUSBAR_START_Y + 1, UI_STATUSBAR_LENGTH, UI_OUTER_BORDER_COLOUR, 1, MODE_PIXEL_SET);
	
	// Sidebar
	draw_VLine(screen, UI_SIDEBAR_START_X, UI_SIDEBAR_START_Y, UI_SIDEBAR_HEIGHT, UI_OUTER_BORDER_COLOUR, MODE_PIXEL_OR);
	draw_VLine(screen, UI_SIDEBAR_START_X + 1, UI_SIDEBAR_START_Y + 1, UI_SIDEBAR_HEIGHT - 1, UI_OUTER_BORDER_COLOUR, MODE_PIXEL_OR);

	// Game Engine name in title bar
	draw_String(screen, UI_TITLEBAR_TEXT_X, UI_TITLEBAR_TEXT_Y, 24, 1, 0, screen->font_8x8, PIXEL_GREEN, ENGINE_TARGET_NAME, MODE_PIXEL_OR);
	
	screen->dirty = 1;

}

void ui_DrawCombat(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate){
	
	screen->dirty = 1;
}

void ui_DrawCharacterScreen_Overview(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate){
	// Draw the overview character screen
	
	PlayerState_t *pc;
	unsigned char i;
	char pc_id = gamestate->players->current - 1;
	unsigned char dice_quantity = 0;
	unsigned char dice_type = 0;
	unsigned char proficiency = 0;
	char constitution_modifier = 0;
	
	// Load the player character
	pc = gamestate->players->player[pc_id];
	
	// ==========================================
	// Left hand column
	// ==========================================
	
	// Character portrait
	draw_Box(screen, 8, 16, DRAW_PORTRAIT_WIDTH + 1, DRAW_PORTRAIT_HEIGHT + 1, 1, PIXEL_RED, PIXEL_CLEAR, MODE_PIXEL_SET);
	draw_Sprite(screen, 9, 17, screen->players[pc_id], 1);
	
	// Name, Class, Race
	sprintf(gamestate->buf, "<r>Character<C>\nName : %s\nClass: %s\nRace : %s", pc->name, player_classes[pc->player_class], player_races[pc->player_race]);
	draw_String(screen, 6, 18, 24, 4, 0, screen->font_8x8, PIXEL_WHITE, gamestate->buf, MODE_PIXEL_SET);
	
	// ==========================================
	// Central divider
	// ==========================================
	draw_VLine(screen, 255, 20, 200, PIXEL_RED, MODE_PIXEL_SET);
	
	// ==========================================
	// Right hand column
	// ==========================================
	
	// Attribute values and their respective modifiers
	sprintf(gamestate->buf, "<r>Attribute<C>\nStrength\nDexterity\nConstitution\nWisdom\nIntelligence\nCharisma");
	draw_String(screen, 33, 18, 12, 13, 0, screen->font_8x8, PIXEL_WHITE, gamestate->buf, MODE_PIXEL_SET);
	sprintf(gamestate->buf, "<r>Value<C>\n%d\n%d\n%d\n%d\n%d\n%d", pc->str, pc->dex, pc->con, pc->wis, pc->intl, pc->chr);
	draw_String(screen, 47, 18, 12, 13, 0, screen->font_8x8, PIXEL_WHITE, gamestate->buf, MODE_PIXEL_SET);
	sprintf(gamestate->buf, "<r>Modifier<C>\n%d\n%d\n%d\n%d\n%d\n%d", ability_Modifier(pc->str), ability_Modifier(pc->dex), ability_Modifier(pc->con), ability_Modifier(pc->wis), ability_Modifier(pc->intl), ability_Modifier(pc->chr));
	draw_String(screen, 54, 18, 12, 13, 0, screen->font_8x8, PIXEL_WHITE, gamestate->buf, MODE_PIXEL_SET);
	
	// Print out the HP increase per level
	constitution_modifier = ability_Modifier(pc->con);
	hit_Dice(pc, &dice_quantity, &dice_type, &constitution_modifier);
	sprintf(gamestate->buf, "HP per level: %dx D%d +%d", dice_quantity, dice_type, constitution_modifier);
	draw_String(screen, 33, 100, 30, 1, 0, screen->font_8x8, PIXEL_WHITE, gamestate->buf, MODE_PIXEL_SET);
	
	// Print out skills we are proficient in
	sprintf(gamestate->buf, "<r>Skills<C>\n");
	for (i = 0; i < MAX_PROFICIENCIES; i++){
		proficiency = player_class_proficiencies[pc->player_class][i];
		if (proficiency != 0){
			sprintf(gamestate->buf + strlen(gamestate->buf), "+%d %s\n", is_proficient(pc, proficiency), proficiencies[proficiency]);
		}
	}
	draw_String(screen, 33, 120, 30, 11, 0, screen->font_8x8, PIXEL_WHITE, gamestate->buf, MODE_PIXEL_SET);
}

void ui_DrawCharacterScreen_Status(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate){
	// Draw the status screen of all active effects on the character
	
	PlayerState_t *pc;
	unsigned char i;
	char pc_id = gamestate->players->current - 1;
		
	// Load the player character
	pc = gamestate->players->player[pc_id];
	
	// Column 1 of effects
	for (i = 0; i < 16; i++){
		draw_Box(screen, 30, 18 + (i * 12), 8, 8, 1, PIXEL_RED, PIXEL_CLEAR, MODE_PIXEL_SET);	
		draw_String(screen, 6, 19 + (i * 12), 16, 1, 0, screen->font_8x8, PIXEL_WHITE, status_effects[i], MODE_PIXEL_SET); 
	}
	
	// Column 2 of effects
	for (i = 0; i < 16; i++){
		draw_Box(screen, 300, 18 + (i * 12), 8, 8, 1, PIXEL_RED, PIXEL_CLEAR, MODE_PIXEL_SET);	
		draw_String(screen, 40, 19 + (i * 12), 16, 1, 0, screen->font_8x8, PIXEL_WHITE, status_effects[(16 + i)], MODE_PIXEL_SET);
	}
	
	
}

char ui_DrawCharacterScreen(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned char tab_id){
	// Draw the player character detail screen to check their status, inventory etc

	char modestring[16];
	char new_mode = 0;
	unsigned char c;
	unsigned char e = 0;
	
	input_Clear();
	input_Set(INPUT_CANCEL);
	input_Set(INPUT_OVERVIEW);
	input_Set(INPUT_OVERVIEW_);
	input_Set(INPUT_ITEMS);
	input_Set(INPUT_ITEMS_);
	input_Set(INPUT_WEAPONS);
	input_Set(INPUT_WEAPONS_);
	input_Set(INPUT_MAGIC);
	input_Set(INPUT_MAGIC_);
	input_Set(INPUT_STATUS);
	input_Set(INPUT_STATUS_);
	
	// Basic screen clear and outline
	draw_Clear(screen);
	draw_Box(screen, 0, 10, SCREEN_WIDTH, SCREEN_HEIGHT - 11, 1, PIXEL_RED, PIXEL_CLEAR, MODE_PIXEL_SET);
	
	// Draw all tabs
	// Overview
	draw_Box(screen, 0, 0, 101, 10, 1, PIXEL_RED, PIXEL_CLEAR, MODE_PIXEL_OR);
	draw_String(screen, 0 + 2, 2, 12, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>O<C>verview", MODE_PIXEL_OR);
	
	// Inventory
	draw_Box(screen, 102, 0, 101, 10, 1, PIXEL_RED, PIXEL_CLEAR, MODE_PIXEL_OR);
	draw_String(screen, 102 / 8 + 2, 2, 12, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>I<C>tems", MODE_PIXEL_OR);
	
	// Equipment
	draw_Box(screen, 204, 0, 101, 10, 1, PIXEL_RED, PIXEL_CLEAR, MODE_PIXEL_OR);
	draw_String(screen, 204 / 8 + 2, 2, 12, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>W<C>eapons", MODE_PIXEL_OR);
	
	// Magic
	draw_Box(screen, 306, 0, 101, 10, 1, PIXEL_RED, PIXEL_CLEAR, MODE_PIXEL_OR);
	draw_String(screen, 306 / 8 + 2, 2, 12, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>M<C>agic", MODE_PIXEL_OR);
	
	// Status
	draw_Box(screen, 408, 0, 104, 10, 1, PIXEL_RED, PIXEL_CLEAR, MODE_PIXEL_OR);
	draw_String(screen, 408 / 8 + 2, 2, 12, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>S<C>tatus", MODE_PIXEL_OR);
	
	// Draw the selected tab
	switch(tab_id){
		case 0:
			strcpy(modestring, "Overview");
			break;
		case 1:
			strcpy(modestring, "Items");
			break;
		case 2:
			strcpy(modestring, "Weapons");
			break;
		case 3:
			strcpy(modestring, "Magic");
			break;
		case 4:
			strcpy(modestring, "Status");
			break;
	}
	draw_Box(screen, tab_id * 102, 0, 101, 10, 1, PIXEL_RED, PIXEL_RED, MODE_PIXEL_OR);
	draw_String(screen, (tab_id * 102) / 8 + 2, 2, strlen(modestring), 1, 0, screen->font_8x8, PIXEL_WHITE, modestring, MODE_PIXEL_OR);
	
	//=======================================
	// Main character overview display 
	//=======================================
	switch(tab_id){
		case 0:
			ui_DrawCharacterScreen_Overview(screen, gamestate, levelstate);
			break;
		case 4:
			ui_DrawCharacterScreen_Status(screen, gamestate, levelstate);
			break;
	}
	
	// Draw overview UI
	// Draw 
	
	//draw_Flip(screen);
	
	while(!e){
		c = input_Get(screen);
		switch(c){
			case INPUT_OVERVIEW:
			case INPUT_OVERVIEW_:
				new_mode = 0;
				e = 1;
				break;
			case INPUT_ITEMS:
			case INPUT_ITEMS_:
				new_mode = 1;
				e = 1;
				break;
			case INPUT_WEAPONS:
			case INPUT_WEAPONS_:
				new_mode = 2;
				e = 1;
				break;
			case INPUT_MAGIC:
			case INPUT_MAGIC_:
				new_mode = 3;
				e = 1;
				break;
			case INPUT_STATUS:
			case INPUT_STATUS_:
				new_mode = 4;
				e = 1;
				break;
			case INPUT_CANCEL:
				new_mode = -1;
				e = 1;
				break;
			default:
				break;
		}
	}	
	return new_mode;
}

void ui_DrawSideBar(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate){
	// Draws the sidebar with the player party portraits, names, basic status, etc
	
	unsigned char i;		// Loop counter
	char buf[20];			// Temporary string buffer
	
	// Draw player stats
	for (i = 0; i <= 3; i++){
		
		// Only draw stats of existing part
		// characters
		if (gamestate->players->player[i]->level){

			// Draw field titles
			draw_String(screen, UI_SIDEBAR_STAT_TEXT_X, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i) + 1, 3, 4, 0, screen->font_8x8, PIXEL_WHITE, "HP:\nLv:\nFm:\nSt:", MODE_PIXEL_SET);
			
			// ===================================================
			// Hitpoints
			// Shows a colour-coded display of the current players
			// health:
			// - Green if HP over 50%
			// - Red if HP under 50%
			// ===================================================
			
			sprintf(buf, "%03d", gamestate->players->player[i]->hp);
			if (((gamestate->players->player[i]->hp * 100) / gamestate->players->player[i]->hp_reset) > UI_HP_WARN_LEVEL){
				draw_String(screen, UI_SIDEBAR_STAT_VALUES_X, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i) + 1, 3, 1, 0, screen->font_8x8, PIXEL_GREEN, buf, MODE_PIXEL_SET);
			} else {
				draw_String(screen, UI_SIDEBAR_STAT_VALUES_X, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i) + 1, 3, 1, 0, screen->font_8x8, PIXEL_RED, buf, MODE_PIXEL_SET);
			}
			
			// ===================================================
			// Level
			// Just shows the level number of the current player
			// ===================================================
			sprintf(buf, "%03d", gamestate->players->player[i]->level);
			draw_String(screen, UI_SIDEBAR_STAT_VALUES_X, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i) + 9, 3, 1, 0, screen->font_8x8, PIXEL_WHITE, buf, MODE_PIXEL_SET);
		
			// ===================================================
			// Combat formation
			// Can display:
			// - Front
			// - Mid
			// - Rear
			// ===================================================
			if (gamestate->players->player[i]->formation == FORMATION_FRONT){
				sprintf(buf, "Front");
			}
			if (gamestate->players->player[i]->formation == FORMATION_MID){
				sprintf(buf, "Mid");
			}
			if (gamestate->players->player[i]->formation == FORMATION_REAR){
				sprintf(buf, "Rear");
			}
			draw_String(screen, UI_SIDEBAR_STAT_VALUES_X, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i) + 17, 5, 1, 0, screen->font_8x8, PIXEL_WHITE, buf, MODE_PIXEL_SET);
			
			// ===================================================
			// Status
			// Green if ok
			// Red if any status effects
			// ===================================================
			if (gamestate->players->player[i]->status == STATUS_OK){
				sprintf(buf, "<g>Good");
			} else {
				sprintf(buf, "<r>Check");
			}
			draw_String(screen, UI_SIDEBAR_STAT_VALUES_X, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i) + 25, 8, 1, 0, screen->font_8x8, PIXEL_WHITE, buf, MODE_PIXEL_SET);
		
			// Name
			sprintf(buf, "%d: %s", (i + 1), gamestate->players->player[i]->short_name);
			draw_String(screen, UI_SIDEBAR_PC_NAME_X, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i) + 36, 12, 1, 0, screen->font_8x8, PIXEL_WHITE, buf, MODE_PIXEL_SET);
		} else {
			// Name
			sprintf(buf, "%d: ----", (i + 1));
			draw_String(screen, UI_SIDEBAR_PC_NAME_X, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i) + 36, 12, 1, 0, screen->font_8x8, PIXEL_WHITE, buf, MODE_PIXEL_SET);
		}
	}
	
	// Draw 4 player head boxes and 4 player portrait sprites
	for (i = 0; i <= 3; i++){
		if (i > 0){
			draw_HLine(screen, UI_SIDEBAR_START_X, UI_SIDEBAR_START_Y + (UI_SIDEBAR_BLOCK * i), SCREEN_WIDTH - UI_SIDEBAR_START_X, PIXEL_GREEN, 0, MODE_PIXEL_OR);	
		}
		
		draw_Box(screen, UI_SIDEBAR_PORTRAIT_X, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i), DRAW_PORTRAIT_WIDTH + 1, DRAW_PORTRAIT_HEIGHT + 1, 1, PIXEL_RED, PIXEL_BLACK, MODE_PIXEL_OR);
		
		if (gamestate->players->player[i]->level > 0){
			// Only draw portraits of existing party characters
			draw_Sprite(screen, UI_SIDEBAR_PORTRAIT_X, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i) + 1, screen->players[i], 1);
		} else {
			// Otherwise draw a placeholder portrait for a default character
			draw_Sprite(screen, UI_SIDEBAR_PORTRAIT_X, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i) + 1, screen->players[i], 1);
		}
	}
	
	screen->dirty = 1;
}

void ui_DrawStatusBar(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned char buttons, unsigned char labels){
	// Draws the available options in the bottom status bar - these are the keys we
	// can press or the options we can take
	
	unsigned char i;
	
	// Clear the status bar
	draw_Box(screen, 8, UI_STATUSBAR_START_Y + 4, SCREEN_WIDTH - 16, (SCREEN_HEIGHT - UI_STATUSBAR_START_Y) - 8, 0, PIXEL_CLEAR, PIXEL_BLACK, MODE_PIXEL_SET);
	
	// Draw blank buttons
	if (buttons){
		draw_Box(screen, 6, 236, 60, 14, 2, PIXEL_RED_STIPPLED, PIXEL_CLEAR, MODE_PIXEL_SET);
		draw_Box(screen, 70, 236, 60, 14, 2, PIXEL_RED_STIPPLED, PIXEL_CLEAR, MODE_PIXEL_SET);
		draw_Box(screen, 134, 236, 60, 14, 2, PIXEL_RED_STIPPLED, PIXEL_CLEAR, MODE_PIXEL_SET);
		draw_Box(screen, 198, 236, 82, 14, 2, PIXEL_RED_STIPPLED, PIXEL_CLEAR, MODE_PIXEL_SET);
		draw_Box(screen, 284, 236, 56, 14, 2, PIXEL_RED_STIPPLED, PIXEL_CLEAR, MODE_PIXEL_SET);
		draw_Box(screen, 344, 236, 62, 14, 2, PIXEL_RED_STIPPLED, PIXEL_CLEAR, MODE_PIXEL_SET);
		draw_Box(screen, 410, 236, 66, 14, 2, PIXEL_RED_STIPPLED, PIXEL_CLEAR, MODE_PIXEL_SET);
		draw_Box(screen, 480, 236, 24, 14, 2, PIXEL_RED_STIPPLED, PIXEL_CLEAR, MODE_PIXEL_SET);
	}
	
	// Draw labels on the buttons
	if (labels){
		for (i = 0; i < MAX_ALLOWED_INPUTS; i++){
			if ((input_allowed[i] == INPUT_MOVE) || (input_allowed[i] == INPUT_MOVE_)){
				draw_String(screen, 2, 240, 8, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>M<C>ove", MODE_PIXEL_SET);
			}
				
			if ((input_allowed[i] == INPUT_TALK) || (input_allowed[i] == INPUT_TALK_)){
				draw_String(screen, 10, 240, 8, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>T<C>alk", MODE_PIXEL_SET);			
			}
			
			// FIGHT and LOOT are mutually exclusive - so they display in the same option box
			if ((input_allowed[i] == INPUT_FIGHT) || (input_allowed[i] == INPUT_FIGHT_)){
				draw_String(screen, 18, 240, 8, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>F<C>ight", MODE_PIXEL_SET);
			}
			if ((input_allowed[i] == INPUT_LOOT) || (input_allowed[i] == INPUT_LOOT_)){
				draw_String(screen, 18, 240, 8, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>L<C>oot", MODE_PIXEL_SET);
			}
			
			if ((input_allowed[i] == INPUT_WITHDRAW) || (input_allowed[i] == INPUT_WITHDRAW_)){
				draw_String(screen, 26, 240, 12, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>W<C>ithdraw", MODE_PIXEL_SET);
			}
			
			if ((input_allowed[i] == INPUT_REST) || (input_allowed[i] == INPUT_REST_)){
				draw_String(screen, 37, 240, 12, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>R<C>est", MODE_PIXEL_SET);
			}
			
			if ((input_allowed[i] == INPUT_BARTER) || (input_allowed[i] == INPUT_BARTER_)){
				draw_String(screen, 44, 240, 12, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>B<C>arter", MODE_PIXEL_SET);
			}
			
			if ((input_allowed[i] == INPUT_N) || (input_allowed[i] == INPUT_N_)){
				draw_String(screen, 52, 240, 12, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>N<C>ext", MODE_PIXEL_SET);
			}
			
			if ((input_allowed[i] == INPUT_QUIT) || (input_allowed[i] == INPUT_QUIT_)){
				draw_String(screen, 61, 240, 12, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>Q", MODE_PIXEL_SET);
			}	
		}
	}
	screen->dirty = 1;
}

void ui_DrawNavigationChoice(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate){
	// Overlays a navigation window with the available exits for a given level
	
	
	unsigned char i;
	unsigned char c;
	unsigned char e = 0;		// Exit
	unsigned char row = 2;		// Start exit labels offset below the window title
	unsigned char have_n = 0;
	unsigned char have_s = 0;
	unsigned char have_e = 0;
	unsigned char have_w = 0;
	
	ui_DrawPopup(screen, UI_NAVBOX_START_X, UI_NAVBOX_START_Y, UI_NAVBOX_WIDTH, UI_NAVBOX_HEIGHT, "Destination", 1);
	
	// Only print out the current allowed navigation options
	for (i = 0; i < MAX_ALLOWED_INPUTS; i++){
		if ((input_allowed[i] == INPUT_N) || (input_allowed[i] == INPUT_N_)){
			if (!have_n){
				draw_String(screen, UI_NAVBOX_TEXT_X, UI_NAVBOX_TEXT_Y + (10), 8, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>N<C>orth", MODE_PIXEL_SET);
				have_n = 1;
			}
		}
		if ((input_allowed[i] == INPUT_S) || (input_allowed[i] == INPUT_S_)){
			if (!have_s){
				draw_String(screen, UI_NAVBOX_TEXT_X, UI_NAVBOX_TEXT_Y + (18), 8, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>S<C>outh", MODE_PIXEL_SET);
				row += 2;
				have_s = 1;
			}
		}
		if ((input_allowed[i] == INPUT_E) || (input_allowed[i] == INPUT_E_)){
			if (!have_e){
				draw_String(screen, UI_NAVBOX_TEXT_X, UI_NAVBOX_TEXT_Y + (26), 8, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>E<C>ast", MODE_PIXEL_SET);
				row += 2;
				have_e = 1;
			}
		}
		if ((input_allowed[i] == INPUT_W) || (input_allowed[i] == INPUT_W_)){
			if (!have_w){
				draw_String(screen, UI_NAVBOX_TEXT_X, UI_NAVBOX_TEXT_Y + (34), 8, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>W<C>est", MODE_PIXEL_SET);
				row += 2;
				have_w = 1;
			}
		}
	}
		
	screen->dirty = 1;
	draw_Flip(screen);
	
	while(!e){
		c = input_Get(screen);
		switch(c){
			case INPUT_N:
			case INPUT_N_:
				// Highlight option
				draw_SelectedString(screen, UI_NAVBOX_TEXT_X, UI_NAVBOX_TEXT_Y + (10), 5, PIXEL_WHITE, "North");				
				gamestate->level_previous = gamestate->level;
				gamestate->level = levelstate->north;
				e = 1;
				break;
			case INPUT_S:
			case INPUT_S_:
				draw_SelectedString(screen, UI_NAVBOX_TEXT_X, UI_NAVBOX_TEXT_Y + (18), 5, PIXEL_WHITE, "South");
				gamestate->level_previous = gamestate->level;
				gamestate->level = levelstate->south;
				e = 1;
				break;
			case INPUT_E:
			case INPUT_E_:
				draw_SelectedString(screen, UI_NAVBOX_TEXT_X, UI_NAVBOX_TEXT_Y + (26), 5, PIXEL_WHITE, "East");
				gamestate->level_previous = gamestate->level;
				gamestate->level = levelstate->east;
				e = 1;
				break;
			case INPUT_W:
			case INPUT_W_:
				draw_SelectedString(screen, UI_NAVBOX_TEXT_X, UI_NAVBOX_TEXT_Y + (34), 5, PIXEL_WHITE, "West");
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
	
}

void ui_DrawTalkChoice(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate){
	// Draws a dialogue box into which the available NPC characters we can talk to are placed
	unsigned char row = 2;		// Start character choice labels offset below the window title
	unsigned char i;
	unsigned char e = 0;
	unsigned char c;
	unsigned short remain = 0;
		
	unsigned char input1_y = 0;
	unsigned char input2_y = 0;
	unsigned char input3_y = 0;
	unsigned char flash_y = 0;
	
	ui_DrawPopup(screen, UI_TALKCHOICE_START_X, UI_TALKCHOICE_START_Y, UI_TALKCHOICE_WIDTH, UI_TALKCHOICE_HEIGHT, "Select Character", 1);
	
	if (levelstate->has_npc1){
		sprintf(gamestate->buf, "<r>1<C>. %s", gamestate->enemies->enemy[1]->name);
		input1_y = UI_TALKCHOICE_TEXT_Y + (row * 8);
		draw_String(screen, UI_TALKCHOICE_TEXT_X, input1_y, 28, 1, 0, screen->font_8x8, PIXEL_WHITE, gamestate->buf, MODE_PIXEL_SET);
	} else {
		draw_String(screen, UI_TALKCHOICE_TEXT_X, input1_y, 28, 1, 0, screen->font_8x8, PIXEL_WHITE, "1. ---", MODE_PIXEL_SET);
	}
	if (levelstate->has_npc2){
		sprintf(gamestate->buf, "<r>2<C>- %s", gamestate->enemies->enemy[2]->name);
		input2_y = input1_y + (2 * 8);
		draw_String(screen, UI_TALKCHOICE_TEXT_X, input2_y, 28, 1, 0, screen->font_8x8, PIXEL_WHITE, gamestate->buf, MODE_PIXEL_SET);
	} else {
		draw_String(screen, UI_TALKCHOICE_TEXT_X, input2_y, 28, 1, 0, screen->font_8x8, PIXEL_WHITE, "2. ---", MODE_PIXEL_SET);
	}
	if (levelstate->has_npc3){
		sprintf(gamestate->buf, "<r>3<C>- %s", gamestate->enemies->enemy[3]->name);
		input3_y = input1_y + (3 * 8);
		draw_String(screen, UI_TALKCHOICE_TEXT_X, input3_y, 28, 1, 0, screen->font_8x8, PIXEL_WHITE, gamestate->buf, MODE_PIXEL_SET);
	} else {
		draw_String(screen, UI_TALKCHOICE_TEXT_X, input3_y, 28, 1, 0, screen->font_8x8, PIXEL_WHITE, "3. ---", MODE_PIXEL_SET);
	}
		
	draw_Flip(screen);
	
	levelstate->selected_npc = 0;
	while(!e){
		c = input_Get(screen);
		switch(c){
			case INPUT_1:
				levelstate->selected_npc = 1;;
				flash_y = input1_y;
				break;
			case INPUT_2:
				levelstate->selected_npc = 2;
				flash_y = input2_y;
				break;
			case INPUT_3:
				levelstate->selected_npc = 3;
				flash_y = input3_y;
				break;
			case INPUT_CANCEL:
				e = 1;
				break;
			default:
				if (levelstate->selected_npc){
					// Flash selected option - we backspace 6 characters, as there are 6 control codes in 
					// the string which displays the selected NPC name.
					draw_SelectedString(screen, UI_TALKCHOICE_TEXT_X, flash_y, strlen(gamestate->buf) - 6, PIXEL_WHITE, gamestate->buf);
					remain = ui_NPCDialogue(screen, gamestate, levelstate, 0, 1);
					if (remain > 0){
						// More text than can be shown on one page...
						input_Set(INPUT_N);
						input_Set(INPUT_N_);
					}
					while(!e){
						c = input_Get(screen);
						switch(c){
							case INPUT_N:
							case INPUT_N_:
								// Keep scrolling through dialogue
								remain = ui_NPCDialogue(screen, gamestate, levelstate, remain, 0);
								break;
							case INPUT_CANCEL:
								e = 1;
								break;
							default:
								break;
						}
					}
				}
				break;
		}
	}
	
	screen->dirty = 1;
}

void ui_DrawPopup(Screen_t *screen, unsigned short x, unsigned short y, unsigned short w, unsigned short h, char *title, unsigned char animate){
	// Draws a 'popup' box with a solid title bar and a title string
	// The box is animated by appearing to roll down from the title bar.
	
	unsigned char line = 0;
	unsigned char pc = (h / screen->popup_steps);
	unsigned char new_y = y + 10;
	unsigned char new_h = h - 10;

	// Draw solid title bar and popup title
	draw_Box(screen, x, y, w, 10, 1, PIXEL_RED, PIXEL_RED, MODE_PIXEL_SET);
	draw_String(screen, (x / 8) + 1, y + 2, (w / 8), 1, 0, screen->font_8x8, PIXEL_WHITE, title, MODE_PIXEL_OR);
	
	if (animate){
		// Animate the box getting bigger
		for (line = 0; line < new_h; line++){
			if (line % pc == 0){
				draw_Box(screen, x, new_y, w, line + 1, 1, PIXEL_RED, PIXEL_BLACK, MODE_PIXEL_SET);
				screen_Vsync(screen, 2);
				draw_Flip(screen);
			}
		}
	}
	draw_Box(screen, x, new_y, w, new_h, 1, PIXEL_RED, PIXEL_BLACK, MODE_PIXEL_SET);	
	
}

void ui_DrawLocationName(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate){
	// Draws the location name in the title bar
	
	draw_String(screen, UI_TITLEBAR_MAX_CHARS - (strlen((char *)levelstate->name)), UI_TITLEBAR_TEXT_Y, MAX_LEVEL_NAME_SIZE, 1, 0, screen->font_8x8, PIXEL_WHITE, (char *)levelstate->name, MODE_PIXEL_SET);
	screen->dirty = 1;
}

unsigned short ui_NPCDialogue(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned short remain, unsigned char animate){
	// Draws the dialogue window, places portrait images and shows the text from the selected npc (npc_choice), 
	// which can be either levelstate->npc1 or leveltstate->npc2
	
	unsigned short next_remain = 0;
	unsigned char npc = 0;
	unsigned short npc_id = 0;
	unsigned short npc_text_id = 0;
	unsigned char npc_unique_dialogue_id = 0;
	
	npc = levelstate->selected_npc;
	
	if (npc == 1){
		npc_text_id = levelstate->npc1_text;
		npc_unique_dialogue_id = levelstate->npc1_text_unique_id;
	}
	if (npc == 2){		
		npc_text_id = levelstate->npc2_text;
		npc_unique_dialogue_id = levelstate->npc2_text_unique_id;
	}
	if (npc == 3){		
		npc_text_id = levelstate->npc3_text;
		npc_unique_dialogue_id = levelstate->npc3_text_unique_id;
	} 
	npc_id = gamestate->enemies->enemy[npc]->id;
	
	// We don't need to load data on every page turn of this dialogue box...
	if (remain == 0){
		// Load the dialogue entry for this NPC
		data_LoadStory(screen, gamestate, levelstate, npc_text_id);
		
		// Register this NPC
		data_AddNPC(screen, gamestate, levelstate, npc_id);
		
		// Register this unique dialogue for this NPC
		// ... in case there are any condition checks on
		// this conversation having taken place
		data_IncrementNPCTalk(screen, gamestate, levelstate, npc_id, npc_unique_dialogue_id);
	}
	
	// Animate a popup window to hold the NPC details and dialogue text
	ui_DrawPopup(screen, UI_NPCDIALOGUE_START_X, UI_NPCDIALOGUE_START_Y, UI_NPCDIALOGUE_WIDTH + 4, UI_NPCDIALOGUE_HEIGHT, gamestate->enemies->enemy[npc]->name, animate);
	
	// Display the portrait for this NPC
	draw_Box(screen, UI_NPCDIALOGUE_START_X + 2, UI_NPCDIALOGUE_START_Y + 12, DRAW_PORTRAIT_WIDTH + 1, DRAW_PORTRAIT_HEIGHT + 1, 1, PIXEL_RED, PIXEL_CLEAR, MODE_PIXEL_OR);
	draw_Sprite(screen, UI_NPCDIALOGUE_START_X + 3, UI_NPCDIALOGUE_START_Y + 13, screen->enemies[npc], 1);
	
	next_remain = draw_String(screen, (UI_NPCDIALOGUE_START_X / 8) + 6, UI_NPCDIALOGUE_START_Y + 13, ((UI_NPCDIALOGUE_WIDTH - DRAW_PORTRAIT_WIDTH)/ 8) - 1, 8, remain, screen->font_8x8, UI_MAIN_WINDOW_COLOUR, gamestate->buf, MODE_PIXEL_SET);
	if (next_remain > 0){
		// Right justify the text, minus the 4 displayable characters
		draw_String(screen, ((UI_NPCDIALOGUE_START_X + UI_NPCDIALOGUE_WIDTH) / 8) - 4, UI_NPCDIALOGUE_START_Y + UI_NPCDIALOGUE_HEIGHT - 10, 16, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>N<C>ext", MODE_PIXEL_SET);
	} else {
		// Right justify the text, minus the 6 displayable characters
		draw_String(screen, ((UI_NPCDIALOGUE_START_X + UI_NPCDIALOGUE_WIDTH) / 8) - 6, UI_NPCDIALOGUE_START_Y + UI_NPCDIALOGUE_HEIGHT - 10, 24, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>Esc<C>ape", MODE_PIXEL_SET);
	}
	return next_remain;
}

unsigned short ui_DrawMainWindowText(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned short remain, char *c){
	// Draws text into the main window with the correct colours,
	// bounding box sizes and other main-window specific settings.
	//
	// This is ALL it does. If all the text cannot be drawn, it
	// returns the number of characters (of the original string) it
	// has printed, otherwise.
	//
	// A subsequent call with the same string, and a non-zero 'remain'
	// parameter will redraw the text from that offset.
	
	// Clear main text window
	draw_Box(screen, UI_MAIN_WINDOW_X, UI_MAIN_WINDOW_Y, UI_MAIN_WINDOW_WIDTH, UI_MAIN_WINDOW_HEIGHT, 0, PIXEL_CLEAR, PIXEL_BLACK, MODE_PIXEL_SET);
	
	remain = draw_String(screen, UI_MAIN_WINDOW_TEXT_X, UI_MAIN_WINDOW_TEXT_Y, UI_MAIN_WINDOW_MAX_CHARS,	UI_MAIN_WINDOW_MAX_ROWS, remain, screen->font_8x8, UI_MAIN_WINDOW_COLOUR, c, MODE_PIXEL_SET);
	
	// Mark screen as dirty, ready to be repainted
	screen->dirty = 1;
	
	// Return position in buffer
	return remain;
}

void ui_DrawSplashText(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate){
	// Draws the game splash text in the centre of the main UI window
	//
	// Shows:
	// - engine name
	// - engine (git) version
	// - current datafile adventure name (story text ID 0)
	// - current adventure intro (story text ID 1)
	
	// Adventure Name	
	draw_String(screen, UI_MAIN_WINDOW_TEXT_X + 12, UI_MAIN_WINDOW_TEXT_Y + (3 * 8), UI_MAIN_WINDOW_MAX_CHARS, 1, 0, screen->font_8x8, PIXEL_GREEN, (char *) gamestate->name, MODE_PIXEL_SET);
	// Engine Name	
	draw_String(screen, UI_MAIN_WINDOW_TEXT_X + 12, UI_MAIN_WINDOW_TEXT_Y + (5 * 8), UI_MAIN_WINDOW_MAX_CHARS, 1, 0,	screen->font_8x8, PIXEL_GREEN, ENGINE_TARGET_NAME, MODE_PIXEL_SET);
	// Print the introductory text
	draw_String(screen, UI_MAIN_WINDOW_TEXT_X, UI_MAIN_WINDOW_TEXT_Y + (9 * 8), UI_MAIN_WINDOW_MAX_CHARS, 14, 0,	screen->font_8x8, PIXEL_WHITE, (char *) gamestate->text_buffer, MODE_PIXEL_SET);
	// Key to continue message
	draw_String(screen, UI_MAIN_WINDOW_TEXT_X, UI_MAIN_WINDOW_TEXT_Y + (23 * 8), UI_MAIN_WINDOW_MAX_CHARS, 1, 0,	screen->font_8x8, PIXEL_RED, "... Press return to start this adventure!", MODE_PIXEL_SET);
	
	// Mark screen as dirty, ready to be repainted
	screen->dirty = 1;
}

unsigned char ui_DrawYesNo(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, char *title){
	// Draw a box with a title of 'title'
	// Print 'Yes' and 'No' in the box
	unsigned char c;
	unsigned char e = 0;
	unsigned char ret = 0;
	
	// Set all the inputs we allow
	input_Clear();
	input_Set(INPUT_Y);
	input_Set(INPUT_Y_);
	input_Set(INPUT_N);
	input_Set(INPUT_N_);
	input_Set(INPUT_CANCEL);
	
	ui_DrawPopup(screen, UI_YESNO_START_X, UI_YESNO_START_Y, (strlen((char *)title) + 8) * 8, 38, (char *)title, 1);
	draw_String(screen, UI_YESNO_START_X / 8 + 1, UI_YESNO_START_Y + 16, (strlen((char *)title) + 8), 3, 0, screen->font_8x8, PIXEL_WHITE, "<r>Y<C>es", MODE_PIXEL_SET);
	draw_String(screen, UI_YESNO_START_X / 8 + 1, UI_YESNO_START_Y + 24, (strlen((char *)title) + 8), 3, 0, screen->font_8x8, PIXEL_WHITE, "<r>N<C>o", MODE_PIXEL_SET);
	draw_Flip(screen);
	
	// Wait for user input
	while(!e){
		c = input_Get(screen);
		switch(c){
			case INPUT_Y:
			case INPUT_Y_:
				draw_SelectedString(screen, UI_YESNO_START_X / 8 + 1, UI_YESNO_START_Y + 16, 3, PIXEL_WHITE, "Yes");	
				e = 1;
				ret = 1;
				break;
			case INPUT_N:
			case INPUT_N_:
			case INPUT_CANCEL:
				draw_SelectedString(screen, UI_YESNO_START_X / 8 + 1, UI_YESNO_START_Y + 24, 2, PIXEL_WHITE, "No");
				e = 1;
				ret = 0;
				break;
			default:
				break;
		}
	}
	return ret;
}

void ui_DrawError(Screen_t *screen, char *title, char *text, short errorcode){
	// Draw 'an error has occurred' box, with a title, body text and an error code
	
	char buf[32];
	
	// Popup, including title
	ui_DrawPopup(screen, UI_ERROR_START_X, UI_ERROR_START_Y, (30 * 8), (7 * 8), (char *)title, 1);
	
	// Main body
	draw_String(screen, UI_ERROR_START_X / 8 + 1, UI_ERROR_START_Y + 12, 28, 3, 0,	screen->font_8x8, PIXEL_RED, (char *)text, MODE_PIXEL_SET);
	
	// Error code in footer
	sprintf(buf, "Error code: <w>[%d]\n", errorcode);
	draw_String(screen, UI_ERROR_START_X / 8 + 1, UI_ERROR_START_Y + 48, 28, 1, 0,	screen->font_8x8, PIXEL_RED, (char *)buf, MODE_PIXEL_SET);
		
	// Flip the screen buffer - all errors are fatal -  don't wait for redraw.
	draw_Flip(screen);
	input_WaitAndReturn(screen);
}

void ui_DebugScreen(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate){

	unsigned char c;
	unsigned char e = 0;
	unsigned short i;
	unsigned short locations = 0;
	unsigned short primary = 0;
	unsigned short secondary = 0;
	unsigned char players = 1;
	unsigned char npcs = 0;
	unsigned char *mem;
	unsigned char *mem2;
	unsigned char *mem3;
	unsigned char *mem4;
	unsigned int size_bytes = 0;
	unsigned int total_bytes = 0;
	unsigned int base1 = 2048;
	unsigned int base2 = 1024;
	unsigned int base3 = 256;
	unsigned int base4 = 8;
	
	draw_Clear(screen);
	
	// NPCs encountered
	npcs = data_CountNPC(gamestate->npcs);
	
	// Locations visited
	for (i = 0; i < MAX_LOCATIONS; i++){
		if (gamestate->level_visits[i] > 0){
			locations++;
		}
	}
	
	// Primary spawns defeated
	for (i = 0; i < MAX_LOCATIONS; i++){
		primary += gamestate->level_defeated_primary[i];
	}
	
	// Secondary spawns defeated
	for (i = 0; i < MAX_LOCATIONS; i++){
		secondary += gamestate->level_defeated_secondary[i];
	}
	
	draw_String(screen, UI_TITLEBAR_MAX_CHARS - (strlen("DEBUG SCREEN")), UI_TITLEBAR_TEXT_Y, MAX_LEVEL_NAME_SIZE, 1, 0, screen->font_8x8, PIXEL_RED, "DEBUG SCREEN", MODE_PIXEL_SET);
	
	// Print data structure sizes	
	sprintf((char *)gamestate->text_buffer, "<g>Data Structures<C>\n- <r>%6d<C> BMP buffers\n- <r>%6d<C> Gamestate (inc text buffers)\n- <r>%6d<C> Levelstate\n- <r>%6d<C> per NPC state, (total: <r>%db<C>)\n- <r>%6d<C> Partystate (total: <r>%dx<C>)\n- <r>%6d<C> Enemystate (total: <r>%dx<C>)\n", (sizeof(bmpdata_t) + sizeof(bmpstate_t)), sizeof(GameState_t), sizeof(LevelState_t), sizeof(struct NPCList), (npcs * sizeof(struct NPCList)), (sizeof(PartyState_t) + (MAX_PLAYERS * sizeof(PlayerState_t))), MAX_PLAYERS, (sizeof(EnemyState_t) + (MAX_MONSTER_TYPES * sizeof(PlayerState_t))), MAX_MONSTER_TYPES);
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> per Weapon\n", sizeof(WeaponState_t));
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> per Spell\n", sizeof(SpellState_t));
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> per Item\n", sizeof(ItemState_t));
	draw_String(screen, 1, 15, 48, 11, 0, screen->font_8x8, PIXEL_WHITE, (char *)gamestate->text_buffer, MODE_PIXEL_SET);
	
	sprintf((char *)gamestate->text_buffer, "<g>Graphics Data<C>\n");
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> Double buffering?\n- <r>%6d<C> Screen state\n- <r>%6d<C> Bitmap font\n- <r>%6d<C> PC/Enemy GFX\n- <r>%6d<C> Boss GFX\n- <r>%6d<C> Sprite size\n- <r>%6d<C> Boss size", screen->indirect, (screen->indirect * SCREEN_BYTES) + sizeof(Screen_t), sizeof(fontdata_t), sizeof(ssprite_t) * (MAX_PLAYERS + MAX_MONSTER_TYPES), sizeof(lsprite_t), SPRITE_NORMAL_BYTES, SPRITE_BOSS_BYTES);
	draw_String(screen, 36, 96, 48, 11, 0, screen->font_8x8, PIXEL_WHITE, (char *)gamestate->text_buffer, MODE_PIXEL_SET);
		
	// Calculate largest free blocks of memory that remain
	sprintf((char *)gamestate->text_buffer, "<g>Memory Free<C> wait...\n");
	draw_String(screen, 1, 104, 48, 6, 0, screen->font_8x8, PIXEL_WHITE, (char *)gamestate->text_buffer, MODE_PIXEL_SET);
	sprintf((char *)gamestate->text_buffer, "<g>Memory Free<C>        \n");
	mem = get_FreeBlock(&size_bytes, base1, base1);
	total_bytes += size_bytes;
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> 1st (%db chunks)\n", size_bytes, base1);
	
	mem2 = get_FreeBlock(&size_bytes, base2, base2);
	total_bytes += size_bytes;
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> 2nd (%db)\n", size_bytes, base2);
	
	mem3 = get_FreeBlock(&size_bytes, base3, base3);
	total_bytes += size_bytes;
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> 3rd (%db)\n", size_bytes, base3);
	
	mem4 = get_FreeBlock(&size_bytes, base4, base4);
	total_bytes += size_bytes;
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> 4th (%db)\n- <r>%6d<C> Total Bytes Free\n", size_bytes, base4, total_bytes);
	draw_String(screen, 1, 104, 48, 6, 0, screen->font_8x8, PIXEL_WHITE, (char *)gamestate->text_buffer, MODE_PIXEL_SET);
	
	// Free any memory allocated!
	if (mem){
		free(mem);
	}
	if (mem2){
		free(mem2);
	}
	if (mem3){
		free(mem3);
	}
	if (mem4){
		free(mem4);
	}

	// Game progress details
	sprintf((char *)gamestate->text_buffer, "<g>Game Progress\n<C>\n- <r>%6d<C> PC in player party\n- <r>%6d<C> NPCs met\n- <r>%6d<C> Locations discovered\n- <r>%6d<C> Primary spawns\n- <r>%6d<C> Secondary spawns\n", players, npcs, locations, primary, secondary);
	draw_String(screen, 1, 160, 48, 6, 0, screen->font_8x8, PIXEL_WHITE, (char *)gamestate->text_buffer, MODE_PIXEL_SET);
	draw_String(screen, 1, SCREEN_HEIGHT - 10, 32, 1, 0, screen->font_8x8, PIXEL_RED, "Press [ESC] to return to game", MODE_PIXEL_SET);
	
	screen->dirty = 1;
	draw_Flip(screen);
	
	input_Clear();
	input_Set(INPUT_CANCEL);
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
}
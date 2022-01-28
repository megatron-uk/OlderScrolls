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

#include <string.h>
#include <stdlib.h>

// Platform agnostic functions
#ifndef _CONFIG_H
#include "../common/config.h"
#define _CONFIG_H
#endif
#ifndef _GAME_H
#include "../common/game.h"
#define _GAME_H
#endif
#ifndef _UI_H
#include "../common/ui.h"
#define _UI_H
#endif
#ifndef _DATA_H
#include "../common/data.h"
#define _DATA_H
#endif
#ifndef _DRAW_H
#include "../common/draw.h"
#define _DRAW_H
#endif
#ifndef _INPUT_H
#include "../common/input.h"
#define _INPUT_H
#endif
#ifndef _UTILS_H
#include "../common/utils.h"
#define _UTILS_H
#endif

void ui_Draw(GameState_t *gamestate, LevelState_t *levelstate){
	// Draws the main user interface - top bar with game name and turn counter
	// Main story/combat window
	// Side bar with party details
	// Bottom bar with status messages/current options
	
	draw_Clear();
	
	// Main outer edges of screen
	draw_Box(UI_OUTER_BORDER_START_X, UI_OUTER_BORDER_START_Y, SCREEN_WIDTH, SCREEN_HEIGHT - 1, UI_OUTER_BORDER_PX, UI_OUTER_BORDER_COLOUR, PIXEL_CLEAR, MODE_PIXEL_OR);
	
	// Game title header bar
	draw_HLine(UI_OUTER_BORDER_START_X, UI_OUTER_BORDER_START_Y + 11, SCREEN_WIDTH, UI_OUTER_BORDER_COLOUR, 0, MODE_PIXEL_SET);
	draw_HLine(UI_OUTER_BORDER_START_X, UI_OUTER_BORDER_START_Y + 12, SCREEN_WIDTH, UI_OUTER_BORDER_COLOUR, 1, MODE_PIXEL_SET);
	
	// Bottom status box
	draw_HLine(UI_STATUSBAR_START_X, UI_STATUSBAR_START_Y, UI_STATUSBAR_LENGTH, UI_OUTER_BORDER_COLOUR, 0, MODE_PIXEL_SET);
	draw_HLine(UI_STATUSBAR_START_X, UI_STATUSBAR_START_Y + 1, UI_STATUSBAR_LENGTH, UI_OUTER_BORDER_COLOUR, 1, MODE_PIXEL_SET);
	
	// Sidebar
	draw_VLine(UI_SIDEBAR_START_X, UI_SIDEBAR_START_Y, UI_SIDEBAR_HEIGHT, UI_OUTER_BORDER_COLOUR, MODE_PIXEL_OR);
	draw_VLine(UI_SIDEBAR_START_X + 1, UI_SIDEBAR_START_Y + 1, UI_SIDEBAR_HEIGHT - 1, UI_OUTER_BORDER_COLOUR, MODE_PIXEL_OR);

	// Game Engine name in title bar
	draw_String(UI_TITLEBAR_TEXT_X, UI_TITLEBAR_TEXT_Y, 24, 1, 0, screen.font_8x8, PIXEL_GREEN, ENGINE_TARGET_NAME);
	
	screen.dirty = 1;

}

void ui_DrawCombat(GameState_t *gamestate, LevelState_t *levelstate){
	
}

void ui_DrawImage(GameState_t *gamestate, LevelState_t *levelstate){
	
}

void ui_DrawText(GameState_t *gamestate, LevelState_t *levelstate){
	
}

void ui_DrawSideBar(GameState_t *gamestate, LevelState_t *levelstate){
	// Draws the sidebar with the player party portraits, names, basic status, etc
	
	unsigned char i;		// Loop counter
	unsigned char n;		// Integer calculation on player stats
	PlayerState_t *pc;		// Pointer to current player
	char buf[16];			// Temporary string buffer
	
	// Draw 3 horizontal bars between the 4 player info boxes
	for (i = 1; i <= 3; i++){
		draw_HLine(UI_SIDEBAR_START_X, UI_SIDEBAR_START_Y + (UI_SIDEBAR_BLOCK * i), SCREEN_WIDTH - UI_SIDEBAR_START_X, PIXEL_GREEN, 0, MODE_PIXEL_OR);
	}
	
	// Draw 4 player head boxes and 4 player portrait sprites
	for (i = 0; i <= 3; i++){
		draw_Box(UI_SIDEBAR_PORTRAIT_X, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i), DRAW_PORTRAIT_WIDTH + 1, DRAW_PORTRAIT_HEIGHT + 1, 1, PIXEL_GREEN, PIXEL_CLEAR, MODE_PIXEL_OR);
		
		if (pc->level){
			
		}
	}	
	
	// Draw player stats
	for (i = 0; i <= 3; i++){
		
		// Only draw
		pc = gamestate->players->player[i];
		if (pc->level){

			// Draw field titles
			draw_String(55, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i) + 1, 3, 4, 0, screen.font_8x8, PIXEL_WHITE, "HP:\nLv:\nFm:\nSt:");
			
			// ===================================================
			// Hitpoints
			// Shows a colour-coded display of the current players
			// health:
			// - Green if HP over 50%
			// - Red if HP under 50%
			// ===================================================
			
			n = (pc->hp * 100) / pc->hp_reset;
			sprintf(buf, "%03d", pc->hp);
			if (n > 50){
				draw_String(58, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i) + 1, 3, 1, 0, screen.font_8x8, PIXEL_GREEN, buf);
			} else {
				draw_String(58, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i) + 1, 3, 1, 0, screen.font_8x8, PIXEL_RED, buf);
			}
			
			// ===================================================
			// Level
			// Just shows the level number of the current player
			// ===================================================
			sprintf(buf, "%03d", pc->level);
			draw_String(58, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i) + 9, 3, 1, 0, screen.font_8x8, PIXEL_WHITE, buf);
		
			// ===================================================
			// Combat formation
			// Can display:
			// - Front
			// - Mid
			// - Rear
			// ===================================================
			if (pc->formation == FORMATION_FRONT){
				sprintf(buf, "Front");
			}
			if (pc->formation == FORMATION_MID){
				sprintf(buf, "Mid");
			}
			if (pc->formation == FORMATION_REAR){
				sprintf(buf, "Rear");
			}
			draw_String(58, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i) + 17, 5, 1, 0, screen.font_8x8, PIXEL_WHITE, buf);
			
			// ===================================================
			// Status
			// Green if ok
			// Red if any status effects
			// ===================================================
			if (pc->status == STATUS_OK){
				sprintf(buf, "<g>Okay");
			} else {
				sprintf(buf, "<r>Check");
			}
			draw_String(58, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i) + 25, 8, 1, 0, screen.font_8x8, PIXEL_WHITE, buf);
		
			// Name
			sprintf(buf, "%d: %s", (i + 1), pc->short_name);
			draw_String(51, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i) + 36, 12, 1, 0, screen.font_8x8, PIXEL_WHITE, buf);
		}
	}
	
	screen.dirty = 1;
}

void ui_DrawStatusBar(GameState_t *gamestate, LevelState_t *levelstate, unsigned char buttons, unsigned char labels){
	// Draws the available options in the bottom status bar - these are the keys we
	// can press or the options we can take
	
	unsigned char i;
	
	// Clear the status bar
	draw_Box(4, UI_STATUSBAR_START_Y + 2, SCREEN_WIDTH - 6, (SCREEN_HEIGHT - UI_STATUSBAR_START_Y) - 6, 2, PIXEL_BLACK, PIXEL_BLACK, MODE_PIXEL_OR);
	
	// Draw blank buttons
	if (buttons){
		draw_Box(6, 236, 60, 14, 2, PIXEL_RED_STIPPLED, PIXEL_BLACK, MODE_PIXEL_OR);
		draw_Box(70, 236, 60, 14, 2, PIXEL_RED_STIPPLED, PIXEL_BLACK, MODE_PIXEL_OR);
		draw_Box(134, 236, 60, 14, 2, PIXEL_RED_STIPPLED, PIXEL_BLACK, MODE_PIXEL_OR);
		draw_Box(198, 236, 82, 14, 2, PIXEL_RED_STIPPLED, PIXEL_BLACK, MODE_PIXEL_OR);
		draw_Box(284, 236, 56, 14, 2, PIXEL_RED_STIPPLED, PIXEL_BLACK, MODE_PIXEL_OR);
		draw_Box(344, 236, 62, 14, 2, PIXEL_RED_STIPPLED, PIXEL_BLACK, MODE_PIXEL_OR);
		draw_Box(410, 236, 66, 14, 2, PIXEL_RED_STIPPLED, PIXEL_BLACK, MODE_PIXEL_OR);
		draw_Box(480, 236, 24, 14, 2, PIXEL_RED_STIPPLED, PIXEL_BLACK, MODE_PIXEL_OR);
	}
	
	// Draw labels on the buttons
	if (labels){
		for (i = 0; i < MAX_ALLOWED_INPUTS; i++){
			if ((input_allowed[i] == INPUT_MOVE) || (input_allowed[i] == INPUT_MOVE_)){
				draw_String(2, 240, 8, 1, 0, screen.font_8x8, PIXEL_WHITE, "<r>M<C>ove");
			}
				
			if ((input_allowed[i] == INPUT_TALK) || (input_allowed[i] == INPUT_TALK_)){
				draw_String(10, 240, 8, 1, 0, screen.font_8x8, PIXEL_WHITE, "<r>T<C>alk");			
			}
			
			if ((input_allowed[i] == INPUT_FIGHT) || (input_allowed[i] == INPUT_FIGHT_)){
				draw_String(18, 240, 8, 1, 0, screen.font_8x8, PIXEL_WHITE, "<r>F<C>ight");
			}
			
			if ((input_allowed[i] == INPUT_WITHDRAW) || (input_allowed[i] == INPUT_WITHDRAW_)){
				draw_String(26, 240, 12, 1, 0, screen.font_8x8, PIXEL_WHITE, "<r>W<C>ithdraw");
			}
			
			if ((input_allowed[i] == INPUT_REST) || (input_allowed[i] == INPUT_REST_)){
				draw_String(37, 240, 12, 1, 0, screen.font_8x8, PIXEL_WHITE, "<r>R<C>est");
			}
			
			if ((input_allowed[i] == INPUT_BARTER) || (input_allowed[i] == INPUT_BARTER_)){
				draw_String(44, 240, 12, 1, 0, screen.font_8x8, PIXEL_WHITE, "<r>B<C>arter");
			}
			
			if ((input_allowed[i] == INPUT_PLAYER) || (input_allowed[i] == INPUT_PLAYER_)){
				draw_String(52, 240, 12, 1, 0, screen.font_8x8, PIXEL_WHITE, "<r>P<C>layer");
			}
			
			if ((input_allowed[i] == INPUT_QUIT) || (input_allowed[i] == INPUT_QUIT_)){
				draw_String(61, 240, 12, 1, 0, screen.font_8x8, PIXEL_WHITE, "<r>Q");
			}	
		}
	}
	screen.dirty = 1;
}

void ui_DrawNavigation(GameState_t *gamestate, LevelState_t *levelstate){
	// Overlays a navigation window with the available exits for a given level
	
	unsigned char i;
	unsigned char row = 3;		// Start exit labels offset below the window title
	unsigned char have_n = 0;
	unsigned char have_s = 0;
	unsigned char have_e = 0;
	unsigned char have_w = 0;
	
	draw_Box(UI_NAVBOX_START_X, UI_NAVBOX_START_Y, UI_NAVBOX_WIDTH, UI_NAVBOX_HEIGHT, 2, PIXEL_RED_STIPPLED, PIXEL_BLACK, MODE_PIXEL_SET);
	draw_String(UI_NAVBOX_TEXT_X, UI_NAVBOX_TEXT_Y, 11, 2, 0, screen.font_8x8, PIXEL_GREEN, "Select Destination");
	
	// Only print out the current allowed navigation options
	for (i = 0; i < MAX_ALLOWED_INPUTS; i++){
		if ((input_allowed[i] == INPUT_N) || (input_allowed[i] == INPUT_N_)){
			if (!have_n){
				draw_String(UI_NAVBOX_TEXT_X, UI_NAVBOX_TEXT_Y + (row * 8), 8, 1, 0, screen.font_8x8, PIXEL_WHITE, "<r>N<C>orth");
				row += 2;
				have_n = 1;
			}
		}
		if ((input_allowed[i] == INPUT_S) || (input_allowed[i] == INPUT_S_)){
			if (!have_s){
				draw_String(UI_NAVBOX_TEXT_X, UI_NAVBOX_TEXT_Y + (row * 8), 8, 1, 0, screen.font_8x8, PIXEL_WHITE, "<r>S<C>outh");
				row += 2;
				have_s = 1;
			}
		}
		if ((input_allowed[i] == INPUT_E) || (input_allowed[i] == INPUT_E_)){
			if (!have_e){
				draw_String(UI_NAVBOX_TEXT_X, UI_NAVBOX_TEXT_Y + (row * 8), 8, 1, 0, screen.font_8x8, PIXEL_WHITE, "<r>E<C>ast");
				row += 2;
				have_e = 1;
			}
		}
		if ((input_allowed[i] == INPUT_W) || (input_allowed[i] == INPUT_W_)){
			if (!have_w){
				draw_String(UI_NAVBOX_TEXT_X, UI_NAVBOX_TEXT_Y + (row * 8), 8, 1, 0, screen.font_8x8, PIXEL_WHITE, "<r>W<C>est");
				row += 2;
				have_w = 1;
			}
		}
	}
	
	draw_String(UI_NAVBOX_TEXT_X, UI_NAVBOX_TEXT_Y+ (11 * 8), 14, 2, 0, screen.font_8x8, PIXEL_GREEN, "...press a key");
	
	screen.dirty = 1;
}

void ui_DrawLocationName(GameState_t *gamestate, LevelState_t *levelstate){
	// Draws the location name in the title bar
	
	draw_String(UI_TITLEBAR_MAX_CHARS - (strlen((char *)levelstate->name)), UI_TITLEBAR_TEXT_Y, MAX_LEVEL_NAME_SIZE, 1, 0, screen.font_8x8, PIXEL_WHITE, (char *)levelstate->name);
	screen.dirty = 1;
}

unsigned short ui_DrawMainWindowText(GameState_t *gamestate, LevelState_t *levelstate, unsigned short remain, char *c){
	// Draws text into the main window with the correct colours,
	// bounding box sizes and other main-window specific settings.
	//
	// This is ALL it does. If all the text cannot be drawn, it
	// returns the number of characters (of the original string) it
	// has printed, otherwise.
	//
	// A subsequent call with the same string, and a non-zero 'remain'
	// parameter will redraw the text from that offset.
	
	remain = draw_String(UI_MAIN_WINDOW_TEXT_X, 
		UI_MAIN_WINDOW_TEXT_Y, 
		UI_MAIN_WINDOW_MAX_CHARS, 
		UI_MAIN_WINDOW_MAX_ROWS, 
		remain, 
		screen.font_8x8, 
		UI_MAIN_WINDOW_COLOUR, c);
	
	// Mark screen as dirty, ready to be repainted
	screen.dirty = 1;
	
	return remain;
}

void ui_DrawSplashText(GameState_t *gamestate, LevelState_t *levelstate){
	// Draws the game splash text in the centre of the main UI window
	//
	// Shows:
	// - engine name
	// - engine (git) version
	// - current datafile adventure name (story text ID 0)
	// - current adventure intro (story text ID 1)
	
	// Engine Name	
	draw_String(UI_MAIN_WINDOW_TEXT_X + 12,
		UI_MAIN_WINDOW_TEXT_Y + (3 * 8),
		UI_MAIN_WINDOW_MAX_CHARS,
		1,
		0,
		screen.font_8x8,
		PIXEL_GREEN, (char *) gamestate->name);
	
	draw_String(UI_MAIN_WINDOW_TEXT_X + 12,
		UI_MAIN_WINDOW_TEXT_Y + (5 * 8),
		UI_MAIN_WINDOW_MAX_CHARS,
		1,
		0,
		screen.font_8x8,
		PIXEL_GREEN, ENGINE_TARGET_NAME);
	
	// Print the introductory text
	draw_String(UI_MAIN_WINDOW_TEXT_X,
		UI_MAIN_WINDOW_TEXT_Y + (9 * 8),
		UI_MAIN_WINDOW_MAX_CHARS,
		14,
		0,
		screen.font_8x8,
		PIXEL_WHITE, (char *) gamestate->text_buffer);
	
	draw_String(UI_MAIN_WINDOW_TEXT_X,
		UI_MAIN_WINDOW_TEXT_Y + (23 * 8),
		UI_MAIN_WINDOW_MAX_CHARS,
		1,
		0,
		screen.font_8x8,
		PIXEL_RED, "... Press return to start this adventure!");
	
	// Mark screen as dirty, ready to be repainted
	screen.dirty = 1;
}

void ui_DrawYesNo(unsigned char *title){
	// Draw a box with a title of 'title'
	// Print 'Yes' and 'No' in the box
	
	// 
	draw_Box(UI_YESNO_START_X, UI_YESNO_START_Y, (strlen((char *)title) + 8) * 8, 42, 2, PIXEL_RED_STIPPLED, PIXEL_BLACK, MODE_PIXEL_SET);
	draw_String(UI_YESNO_START_X / 8 + 1,
		UI_YESNO_START_Y + 4,
		strlen((char *)title),
		1,
		0,
		screen.font_8x8,
		PIXEL_GREEN, (char *)title);	
	
	draw_String(UI_YESNO_START_X / 8 + 1,
		UI_YESNO_START_Y + 16,
		14,
		3,
		0,
		screen.font_8x8,
		PIXEL_WHITE, "<r>Y<C>es\n<r>N<C>o\n<g>...press a key");
	screen.dirty = 1;
}

void ui_DebugScreen(GameState_t *gamestate, LevelState_t *levelstate){

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
	unsigned int base1 = 512;
	unsigned int base2 = 256;
	unsigned int base3 = 128;
	unsigned int base4 = 8;
	
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
	
	// Draw the basic game UI
	ui_Draw(gamestate, levelstate);
	draw_String(UI_TITLEBAR_MAX_CHARS - (strlen("DEBUG SCREEN")), UI_TITLEBAR_TEXT_Y, MAX_LEVEL_NAME_SIZE, 1, 0, screen.font_8x8, PIXEL_RED, "DEBUG SCREEN");
	
	// Show size of current game data structures
	sprintf((char *)gamestate->text_buffer, "<g>Memory Used<C>\n");
	
	// This line adds the following
	// + 1x offscreen buffer (if in use)
	// + 1x bmpdata structure, used to load any new bitmaps
	// + 1x bmpstate structure, used to progressively load bitmaps to screen
	// + 4x player sprite/portrait structures
	// + MAX_MONSTER_TYPES x enemy sprite/portrait structures
	// + 1x boss sprite/portrait structures
	
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> Screen, Player Sprites, BMP buffers\n", (screen.indirect * SCREEN_BYTES) + sizeof(screen) + sizeof(bmpdata_t) + sizeof(bmpstate_t) + (4 * sizeof(ssprite_t)) + (MAX_MONSTER_TYPES * sizeof(ssprite_t)) + sizeof(lsprite_t));
	
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> Game, Party, Enemies\n", sizeof(GameState_t));//, sizeof(EnemyState_t));
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> Level data\n", sizeof(LevelState_t));
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> Font data\n", sizeof(fontdata_t));
	//sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> per Player, (total: <r>%d<C>)\n", sizeof(PlayerState_t), (players * sizeof(PlayerState_t)));
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> per NPC, (total: <r>%d<C>)\n", sizeof(struct NPCList), (npcs * sizeof(struct NPCList)));
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> per Weapon\n", sizeof(WeaponState_t));
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> per Spell\n", sizeof(SpellState_t));
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> per Item\n", sizeof(ItemState_t));
	draw_String(1, 15, 48, 11, 0, screen.font_8x8, PIXEL_WHITE, (char *)gamestate->text_buffer);
	
	// Calculate largest free blocks of memory that remain
	sprintf((char *)gamestate->text_buffer, "<g>Memory Free<C>\n");
	mem = get_FreeBlock(&size_bytes, base1, 72);
	total_bytes += size_bytes;
	if (mem){
		sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> 1st Free block\n", size_bytes);
	} else {
		sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- 1st free block is <r>less<C> than <r>%d<C> bytes\n", base1);
	}
	
	mem2 = get_FreeBlock(&size_bytes, base2, 8);
	total_bytes += size_bytes;
	if (mem2){
		sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> 2nd\n", size_bytes);
	}
	
	mem3 = get_FreeBlock(&size_bytes, base3, 2);
	total_bytes += size_bytes;
	if (mem3){
		sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> 3rd\n", size_bytes);
	}
	
	mem4 = get_FreeBlock(&size_bytes, base4, 1);
	total_bytes += size_bytes;
	if (mem4){
		sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> 4th\n", size_bytes);
	}
	
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> Total Bytes Free\n", total_bytes);
	draw_String(1, 96, 48, 6, 0, screen.font_8x8, PIXEL_WHITE, (char *)gamestate->text_buffer);
	
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
	sprintf((char *)gamestate->text_buffer, "<g>Game Progress<C>\n");
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> PC in player party\n", players);
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> NPCs met\n", npcs);
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> Locations discovered\n", locations);
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> Primary spawns\n", primary);
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> Secondary spawns\n", secondary);
	draw_String(1, 156, 48, 6, 0, screen.font_8x8, PIXEL_WHITE, (char *)gamestate->text_buffer);
	
	screen.dirty = 1;
}
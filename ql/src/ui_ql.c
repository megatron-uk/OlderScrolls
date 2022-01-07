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
#ifndef _DRAW_H
#include "../common/draw.h"
#define _DRAW_H
#endif
#ifndef _INPUT_H
#include "../common/input.h"
#define _INPUT_H
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
	//draw_String(UI_TITLEBAR_TEXT_X + 13, UI_TITLEBAR_TEXT_Y, 24, 1, 0, screen.font_8x8, PIXEL_RED, ENGINE_TARGET_NAME);
	
	screen.dirty = 1;

}

void ui_DrawCombat(GameState_t *gamestate, LevelState_t *levelstate){
	
}

void ui_DrawImage(GameState_t *gamestate, LevelState_t *levelstate){
	
}

void ui_DrawText(GameState_t *gamestate, LevelState_t *levelstate){
	
}

void ui_DrawSideBar(GameState_t *gamestate, LevelState_t *levelstate){
	
}

void ui_DrawStatusBar(GameState_t *gamestate, LevelState_t *levelstate, unsigned char buttons, unsigned char labels, char *allowed_inputs){
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
			if ((allowed_inputs[i] == INPUT_MOVE) || (allowed_inputs[i] == INPUT_MOVE_)){
				draw_String(2, 240, 8, 1, 0, screen.font_8x8, PIXEL_WHITE, "<r>M<C>ove");
			}
				
			if ((allowed_inputs[i] == INPUT_TALK) || (allowed_inputs[i] == INPUT_TALK_)){
				draw_String(10, 240, 8, 1, 0, screen.font_8x8, PIXEL_WHITE, "<r>T<C>alk");			
			}
			
			if ((allowed_inputs[i] == INPUT_FIGHT) || (allowed_inputs[i] == INPUT_FIGHT_)){
				draw_String(18, 240, 8, 1, 0, screen.font_8x8, PIXEL_WHITE, "<r>F<C>ight");
			}
			
			if ((allowed_inputs[i] == INPUT_WITHDRAW) || (allowed_inputs[i] == INPUT_WITHDRAW_)){
				draw_String(26, 240, 12, 1, 0, screen.font_8x8, PIXEL_WHITE, "<r>W<C>ithdraw");
			}
			
			if ((allowed_inputs[i] == INPUT_REST) || (allowed_inputs[i] == INPUT_REST_)){
				draw_String(37, 240, 12, 1, 0, screen.font_8x8, PIXEL_WHITE, "<r>R<C>est");
			}
			
			if ((allowed_inputs[i] == INPUT_BARTER) || (allowed_inputs[i] == INPUT_BARTER_)){
				draw_String(44, 240, 12, 1, 0, screen.font_8x8, PIXEL_WHITE, "<r>B<C>arter");
			}
			
			if ((allowed_inputs[i] == INPUT_PLAYER) || (allowed_inputs[i] == INPUT_PLAYER_)){
				draw_String(52, 240, 12, 1, 0, screen.font_8x8, PIXEL_WHITE, "<r>P<C>layer");
			}
			
			if ((allowed_inputs[i] == INPUT_QUIT) || (allowed_inputs[i] == INPUT_QUIT_)){
				draw_String(61, 240, 12, 1, 0, screen.font_8x8, PIXEL_WHITE, "<r>Q");
			}	
		}
	}
	screen.dirty = 1;
}

void ui_DrawNavigation(GameState_t *gamestate, LevelState_t *levelstate, char *input_allowed){
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
	
	draw_String(UI_TITLEBAR_MAX_CHARS - (strlen(levelstate->name)), UI_TITLEBAR_TEXT_Y, MAX_LEVEL_NAME_SIZE, 1, 0, screen.font_8x8, PIXEL_WHITE, (char *) levelstate->name);
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
	
	draw_String(UI_MAIN_WINDOW_TEXT_X + 12,
		UI_MAIN_WINDOW_TEXT_Y + (6 * 8),
		UI_MAIN_WINDOW_MAX_CHARS,
		1,
		0,
		screen.font_8x8,
		PIXEL_GREEN, "ver.abc123");
	
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
	draw_Box(UI_YESNO_START_X, UI_YESNO_START_Y, (strlen(title) + 8) * 8, 52, 2, PIXEL_RED_STIPPLED, PIXEL_BLACK, MODE_PIXEL_SET);
	draw_String(UI_YESNO_START_X / 8 + 1,
		UI_YESNO_START_Y + 4,
		strlen(title),
		1,
		0,
		screen.font_8x8,
		PIXEL_GREEN, (char *) title);	
	
	draw_String(UI_YESNO_START_X / 8 + 1,
		UI_YESNO_START_Y + 16,
		8,
		1,
		0,
		screen.font_8x8,
		PIXEL_WHITE, "<r>Y<C>es");
	draw_String(UI_YESNO_START_X / 8 + 1,
		UI_YESNO_START_Y + 28,
		8,
		1,
		0,
		screen.font_8x8,
		PIXEL_WHITE, "<r>N<C>o");
	draw_String(UI_YESNO_START_X / 8 + 1,
		UI_YESNO_START_Y + 40,
		14,
		1,
		0,
		screen.font_8x8,
		PIXEL_GREEN, "...press a key");
	screen.dirty = 1;
}
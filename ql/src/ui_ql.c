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

void ui_Draw(GameState_t *gamestate, LevelState_t *levelstate){
	// Draws the main user interface - top bar with game name and turn counter
	// Main story/combat window
	// Side bar with party details
	// Bottom bar with status messages/current options
	
	draw_Clear();
	
	// Main outer edges of screen
	draw_Box(UI_OUTER_BORDER_START_X, UI_OUTER_BORDER_START_Y, SCREEN_WIDTH, SCREEN_HEIGHT - 1, UI_OUTER_BORDER_PX, UI_OUTER_BORDER_COLOUR, PIXEL_CLEAR);
	
	// Game title/turn number header
	draw_HLine(UI_OUTER_BORDER_START_X, UI_OUTER_BORDER_START_Y + 11, SCREEN_WIDTH, UI_OUTER_BORDER_COLOUR, 0);
	draw_HLine(UI_OUTER_BORDER_START_X, UI_OUTER_BORDER_START_Y + 12, SCREEN_WIDTH, UI_OUTER_BORDER_COLOUR, 1);
	
	// Bottom status box
	draw_HLine(UI_STATUSBAR_START_X, UI_STATUSBAR_START_Y, UI_STATUSBAR_LENGTH, UI_OUTER_BORDER_COLOUR, 0);
	draw_HLine(UI_STATUSBAR_START_X, UI_STATUSBAR_START_Y + 1, UI_STATUSBAR_LENGTH, UI_OUTER_BORDER_COLOUR, 1);
	
	// Sidebar
	draw_VLine(UI_SIDEBAR_START_X, UI_SIDEBAR_START_Y, UI_SIDEBAR_HEIGHT, UI_OUTER_BORDER_COLOUR);
	draw_VLine(UI_SIDEBAR_START_X + 1, UI_SIDEBAR_START_Y + 1, UI_SIDEBAR_HEIGHT - 1, UI_OUTER_BORDER_COLOUR);

	// Game Engine name in title bar
	draw_String(UI_TITLEBAR_TEXT_X, UI_TITLEBAR_TEXT_Y, 24, 1, 0, screen.font_8x8, PIXEL_GREEN, ENGINE_NAME);
	draw_String(UI_TITLEBAR_TEXT_X + 13, UI_TITLEBAR_TEXT_Y, 24, 1, 0, screen.font_8x8, PIXEL_RED, ENGINE_TARGET_NAME);
	
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

void ui_DrawStatusBar(GameState_t *gamestate, LevelState_t *levelstate){
	
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
	// - current datafile adventure name
	// - current adventure story text ID 0
	
	// Engine Name	
	draw_String(UI_MAIN_WINDOW_TEXT_X + 12,
		UI_MAIN_WINDOW_TEXT_Y + (3 * 8),
		UI_MAIN_WINDOW_MAX_CHARS,
		1,
		0,
		screen.font_8x8,
		PIXEL_GREEN, gamestate->name);
	
	draw_String(UI_MAIN_WINDOW_TEXT_X + 12,
		UI_MAIN_WINDOW_TEXT_Y + (5 * 8),
		UI_MAIN_WINDOW_MAX_CHARS,
		1,
		0,
		screen.font_8x8,
		PIXEL_GREEN, "OlderScrolls");
	
	draw_String(UI_MAIN_WINDOW_TEXT_X + 25,
		UI_MAIN_WINDOW_TEXT_Y + (5 * 8),
		UI_MAIN_WINDOW_MAX_CHARS,
		1,
		0,
		screen.font_8x8,
		PIXEL_RED, ENGINE_TARGET_NAME);
	
	draw_String(UI_MAIN_WINDOW_TEXT_X + 12,
		UI_MAIN_WINDOW_TEXT_Y + (6 * 8),
		UI_MAIN_WINDOW_MAX_CHARS,
		1,
		0,
		screen.font_8x8,
		PIXEL_GREEN, "abc123");
	
	// Print the introductory text
	draw_String(UI_MAIN_WINDOW_TEXT_X,
		UI_MAIN_WINDOW_TEXT_Y + (9 * 8),
		UI_MAIN_WINDOW_MAX_CHARS,
		14,
		0,
		screen.font_8x8,
		PIXEL_WHITE, gamestate->text_buffer);
	
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
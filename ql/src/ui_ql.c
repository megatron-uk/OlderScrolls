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

// Platform agnostic functions
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

// Any UI display routine needs to print, at most, all rows and all
// columns of the main text display window.
char ui_text_buffer[UI_MAIN_WINDOW_MAX_CHARS * UI_MAIN_WINDOW_MAX_ROWS];

void ui_Draw(){
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

void ui_DrawCombat(){
	
}

void ui_DrawImage(){
	
}

void ui_DrawText(){
	
}

void ui_DrawSideBar(){
	
}

void ui_DrawStatusBar(){
	
}

unsigned short ui_DrawMainWindowText(unsigned short remain){
	// Draws text into the main window with the correct colours,
	// bounding box sizes and other main-window specific settings.
	
	remain = draw_String(UI_MAIN_WINDOW_TEXT_X, 
		UI_MAIN_WINDOW_TEXT_Y, 
		UI_MAIN_WINDOW_MAX_CHARS, 
		UI_MAIN_WINDOW_MAX_ROWS, 
		remain, 
		screen.font_8x8, 
		UI_MAIN_WINDOW_COLOUR, c);
	
	screen.dirty = 1;
}

void ui_DrawSplash(){
	// Draws the game splash text in the centre of the main UI window
	//
	// Shows:
	// - engine name
	// - engine (git) version
	// - current datafile adventure name
	// - current adventure story text ID 0
	
}
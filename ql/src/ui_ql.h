/* ui_ql.h, Prototypes for the Sinclair QL ui functions.
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

#ifndef _DRAW_H
#include "../common/draw.h"
#define _DRAW_H
#endif

// Main windows is the full screen
#define UI_OUTER_BORDER_START_X		0
#define UI_OUTER_BORDER_START_Y		0
#define UI_OUTER_BORDER_PX			2
#define UI_OUTER_BORDER_COLOUR		PIXEL_GREEN_STIPPLED

// Bottom status bar is 5 rows of text + the UI border size
#define UI_STATUSBAR_START_X		0
#define UI_STATUSBAR_START_Y		207 // 256 - (5x8px characters, plus 2x border px)
#define UI_STATUSBAR_LENGTH			SCREEN_WIDTH

// Sidebar
#define UI_SIDEBAR_START_X			400
#define UI_SIDEBAR_START_Y			12
#define UI_SIDEBAR_HEIGHT			195

// Text areas
#define UI_MAIN_WINDOW_TEXT_X		1
#define UI_MAIN_WINDOW_TEXT_Y		14
#define UI_MAIN_WINDOW_MAX_CHARS	48
#define UI_MAIN_WINDOW_MAX_ROWS		24
#define UI_MAIN_WINDOW_COLOUR		PIXEL_WHITE

#define UI_TITLEBAR_TEXT_X			1
#define UI_TITLEBAR_TEXT_Y			3


void ui_Draw();
void ui_DrawCombat();
void ui_DrawImage();
void ui_DrawText();
void ui_DrawSideBar();
void ui_DrawStatusBar();
void ui_DrawSplashText();

// Area-specific text display routines
unsigned short ui_DrawMainWindowText(unsigned short remain);

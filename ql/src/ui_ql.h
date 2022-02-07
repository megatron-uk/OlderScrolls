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

#ifndef _UI_QL_DEF_H
#define _UI_QL_DEF_H

#ifndef _GAME_H
#include "../common/game.h"
#endif
#ifndef _DRAW_H
#include "../common/draw.h"
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
#define UI_SIDEBAR_BLOCK			49
#define UI_SIDEBAR_PORTRAIT_X		UI_SIDEBAR_START_X + 4 
#define UI_SIDEBAR_PORTRAIT_Y		UI_SIDEBAR_START_Y + 3
#define UI_SIDEBAR_PC_NAME_X		51
#define UI_SIDEBAR_STAT_TEXT_X		55
#define UI_SIDEBAR_STAT_VALUES_X	58
#define UI_HP_WARN_LEVEL			50 // HP under this percentage will show in a different colour

// Text areas
#define UI_MAIN_WINDOW_X			8
#define UI_MAIN_WINDOW_Y			13
#define UI_MAIN_WINDOW_WIDTH		392
#define UI_MAIN_WINDOW_HEIGHT		193
#define UI_MAIN_WINDOW_TEXT_X		1
#define UI_MAIN_WINDOW_TEXT_Y		14
#define UI_MAIN_WINDOW_MAX_CHARS	48
#define UI_MAIN_WINDOW_MAX_ROWS		24
#define UI_MAIN_WINDOW_COLOUR		PIXEL_WHITE

// Title bar
#define UI_TITLEBAR_TEXT_X			1
#define UI_TITLEBAR_TEXT_Y			3
#define UI_TITLEBAR_MAX_CHARS		63

// Navigation window popup
#define UI_NAVBOX_START_X			170
#define UI_NAVBOX_START_Y			80
#define UI_NAVBOX_WIDTH				124
#define UI_NAVBOX_HEIGHT			102
#define UI_NAVBOX_TEXT_X			22
#define UI_NAVBOX_TEXT_Y			UI_NAVBOX_START_Y + 4

// Yes/No popup boxes
#define UI_YESNO_START_X			160
#define UI_YESNO_START_Y			170

// Error popup boxes
#define UI_ERROR_START_X			110
#define UI_ERROR_START_Y			150

#endif

// Protos

#ifndef _UI_QL_PROTO_H
#define _UI_QL_PROTO_H

void ui_Draw(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate);
void ui_DrawCombat(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate);
void ui_DrawImage(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate);
void ui_DrawText(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate);
void ui_DrawSideBar(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate);
void ui_DrawStatusBar(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned char buttons, unsigned char labels);
void ui_DrawSplashText(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate);
void ui_DrawLocationName(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate);
void ui_DrawNavigation(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate);
void ui_DrawYesNo(Screen_t *screen, char *title);
void ui_DrawError(Screen_t *screen, char *title, char *text, short errorcode);
void ui_DebugScreen(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate);

// Area-specific text display routines
unsigned short ui_DrawMainWindowText(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned short remain, char *c);

#endif
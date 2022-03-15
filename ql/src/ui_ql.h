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
#define UI_NAVBOX_START_X			190
#define UI_NAVBOX_START_Y			130
#define UI_NAVBOX_WIDTH				128
#define UI_NAVBOX_HEIGHT			50
#define UI_NAVBOX_TEXT_X			25
#define UI_NAVBOX_TEXT_Y			UI_NAVBOX_START_Y + 4

// Talk choice window popup
#define UI_TALKCHOICE_START_X			190
#define UI_TALKCHOICE_START_Y			140
#define UI_TALKCHOICE_WIDTH				164
#define UI_TALKCHOICE_HEIGHT			52
#define UI_TALKCHOICE_TEXT_X			25
#define UI_TALKCHOICE_TEXT_Y			UI_TALKCHOICE_START_Y + 2

// NPC dialogue window popup
#define UI_NPCDIALOGUE_START_X		30
#define UI_NPCDIALOGUE_START_Y		70
#define UI_NPCDIALOGUE_WIDTH		304
#define UI_NPCDIALOGUE_HEIGHT		90

// Yes/No popup boxes
#define UI_YESNO_START_X			160
#define UI_YESNO_START_Y			140

// Error popup boxes
#define UI_ERROR_START_X			140
#define UI_ERROR_START_Y			20

// Loot popup boxes
#define UI_LOOT_START_X				14
#define UI_LOOT_START_Y				40
#define UI_LOOT_WIDTH				180

#endif

// Protos

#ifndef _UI_QL_PROTO_H
#define _UI_QL_PROTO_H

void ui_Draw(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate);
void ui_DrawCombat(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate);
char ui_DrawCharacterScreen(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned char tab_id);
char ui_DrawCharacterScreen_Overview(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate);
void ui_DrawSideBar(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate);
void ui_DrawStatusBar(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned char buttons, unsigned char labels);
void ui_DrawSplashText(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate);
void ui_DrawLocationName(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate);
void ui_DrawNavigationChoice(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate);
void ui_DrawTalkChoice(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate);
void ui_DrawError(Screen_t *screen, char *title, char *text, short errorcode);
void ui_DebugScreen(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate);
void ui_DrawPopup(Screen_t *screen, unsigned short x, unsigned short y, unsigned short w, unsigned short h, char *title, unsigned char animate);
void ui_DrawCharacterScreen_WeaponsToString(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, WeaponState_t *weapon);
void ui_DrawCharacterScreen_WeaponDamageToString(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, WeaponState_t *weapon, unsigned char dmg_type, unsigned char dice_qty, unsigned char dice_type);
char ui_DrawLootDestination(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, WeaponState_t *weapon, ItemState_t *item);
char ui_DrawLootChoice(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned char location_loot, unsigned char enemy_loot);
void ui_DrawLootChoiceRedraw(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned char location_loot, unsigned char enemy_loot, char selected_id, unsigned char selected);
void ui_DrawEquipError(Screen_t *screen, char *error);
char ui_DrawBooleanChoice(Screen_t *screen, char *title, char *choice1, char *choice2);

// Area-specific text display routines
unsigned short ui_DrawMainWindowText(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned short remain, char *c);
unsigned short ui_NPCDialogue(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned short remain, unsigned char animate);

#endif
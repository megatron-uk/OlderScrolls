/* game_ql.h, Sinclair QL specific game functions
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

// Defines

#ifndef _GAME_QL_DEF_H
#define _GAME_QL_DEF_H

#ifndef _DRAW_H
#include "../common/draw.h"
#endif

#endif

// Prototypes

#ifndef _GAME_QL_PROTO_H
#define _GAME_QL_PROTO_H

void game_Init(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate); 	// Init game data
void game_Exit(Screen_t *screen); 														// De-init game data
void game_Splash(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate); 	// Show a splash screen on game start
void game_Map(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate); 
void game_Combat(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate);
void game_Quit(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate);

unsigned char game_CheckMovement(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned char add_inputs, unsigned char add_text);
unsigned char game_CheckMonsterSpawn(GameState_t *gamestate, LevelState_t *levelstate, unsigned char add_inputs, unsigned char add_text);

#endif
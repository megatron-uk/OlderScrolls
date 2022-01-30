/* data_ql.h, Prototypes for Sinclair QL specific data file search/loading functions.
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

#ifndef _GAME_H
#include "../common/game.h"
#define _GAME_H
#endif
#ifndef _DRAW_H
#include "../common/draw.h"
#define _DRAW_H
#endif

// This is defined here and not in data.h as not all targets support bitmap sprites
// as part of the player creation routine (e.g. text mode targets)
unsigned char data_CreateCharacter(PlayerState_t *playerstate, ssprite_t *playersprite);

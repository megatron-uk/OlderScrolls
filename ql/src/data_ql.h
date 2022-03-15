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

// Defines

#ifndef _DATA_QL_DEFS_H
#define _DATA_QL_DEFS_H
#endif

// Protos

#ifndef _DATA_QL_PROTO_H
#define _DATA_QL_PROTO_H

#ifndef _GAME_H
#include "../common/game.h"
#endif
#ifndef _ENGINE_H
#include "../common/engine.h"
#endif
#ifndef _DRAW_H
#include "../common/draw.h"
#endif

int data_LoadStory(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned short id);
int data_LoadMap(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned short id);
int data_LoadSprite(Screen_t *screen, ssprite_t *sprite, unsigned short id);
int data_LoadPortrait(Screen_t *screen, ssprite_t *sprite, unsigned short id);
int data_LoadBoss(Screen_t *screen, lsprite_t *lsprite, unsigned short id);
int data_LoadWeapon(Screen_t *screen, WeaponState_t *weaponstate, unsigned char id);
int data_LoadItem(Screen_t *screen, ItemState_t *itemstate, unsigned char id);
char data_AddNPC(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned char id);
char data_SetNPCDead(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned char id, unsigned char dead);
char data_IncrementNPCTalk(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned char id, unsigned char unique_dialogue_id);

struct NPCList * data_FindNPC(struct NPCList *npclist, unsigned char id);
struct NPCList * data_LastNPC(struct NPCList *npclist);
unsigned char data_CountNPC(struct NPCList *npclist);

// This is defined here and not in data.h as not all targets support bitmap sprites
// as part of the player creation routine (e.g. text mode targets)
int data_CreateCharacter(Screen_t *screen, PlayerState_t *playerstate, ssprite_t *playersprite, lsprite_t *bosssprite, unsigned char character_type, short character_id);

#endif
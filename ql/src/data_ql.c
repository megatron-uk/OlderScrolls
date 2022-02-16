/* data_ql.c, Loading functions for external game data of the OlderScrolls-Engine.
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

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef _INPUT_H
#include "../common/input.h"
#endif
#ifndef _GAME_H
#include "../common/game.h"
#endif
#ifndef _DATA_H
#include "../common/data.h"
#endif
#ifndef _CONFIG_H
#include "../common/config.h"
#endif
#ifndef _UI_H
#include "../common/ui.h"
#endif
//#ifndef _BMP_H
//#include "bmp_ql.h"
//#endif
#ifndef _DRAW_H
#include "../common/draw.h"
#endif
#ifndef _ERROR_H
#include "../common/error.h"
#endif
#include "../common/conditions.h"


int data_LoadMap(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned short id){
	// Load a gameworld map from disk, parsing it and inserting
	// the data into the global 'levelstate' struct.
	
	unsigned short record_size = 0;
	unsigned long record_offset = 0;
	unsigned char i;
	unsigned char total_items = 0;
	unsigned char item_type;
	unsigned char item_id;
	unsigned char w_count = 0;
	unsigned char i_count = 0;	
	int f;
	
	f = open(MAP_IDX, O_RDONLY);
	if (f < 0){
		ui_DrawError(screen, DATA_LOAD_ERROR_MSG, DATA_LOAD_MAP_INDEX_MSG, f);
		return DATA_LOAD_MAP_INDEXFILE;	
	}
	
	// Seek to the right ID in the header
	lseek(f, ((id - 1) * DATA_HEADER_ENTRY_SIZE), SEEK_SET);
	
	// Read the header entry for this record
	read(f, &record_size, DATA_HEADER_RECORD_SIZE);		// This is the size of the record, in bytes
	read(f, &record_offset, DATA_HEADER_OFFSET_SIZE);		// This is the offset of the record, in bytes from 0
	close(f);
	
	f = open(MAP_DAT, O_RDONLY);
	if (f < 0){
		ui_DrawError(screen, DATA_LOAD_ERROR_MSG, DATA_LOAD_MAP_DAT_MSG, f);
		return DATA_LOAD_MAP_DATFILE;	
	}
	
	// Seek to the data record itself
	lseek(f, record_offset, SEEK_SET);
	
	// (2 bytes) Level ID
	read(f, &levelstate->id, 2);
	if (levelstate->id != id){
		ui_DrawError(screen, DATA_LOAD_ERROR_MSG, DATA_LOAD_MAP_MISMATCH_MSG, 0);
		return DATA_LOAD_MAP_MISMATCH;	
	}
	
	// (2 bytes) Primary text ID
	read(f, &levelstate->text, 2);

	// (32 bytes) Level name 
	read(f, &levelstate->name, MAX_LEVEL_NAME_SIZE);
	
	// =====================================
	// North exit
	// =====================================
	
	// (2 bytes) North exit ID
	read(f, &levelstate->north, 2);
	
	// (2 bytes) North exit text label ID
	read(f, &levelstate->north_text, 2);
	
	// North condition (min 2 bytes, possibly 7+)
	read(f, &levelstate->north_eval_type, 1);
	read(f, &levelstate->north_require_number, 1,);
	if (levelstate->north_require_number != 0){
		read(f, &levelstate->north_require, (COND_LENGTH * levelstate->north_require_number));
	}
	
	// =====================================
	// South exit
	// =====================================	
	
	// (2 bytes) South exit ID
	read(f, &levelstate->south, 2);
	
	// (2 bytes) South exit text label ID
	read(f, &levelstate->south_text, 2);
	
	// South condition (min 2 bytes, possibly 7+)
	read(f, &levelstate->south_eval_type, 1);
	read(f, &levelstate->south_require_number, 1);
	if (levelstate->south_require_number != 0){
		read(f, &levelstate->south_require, (COND_LENGTH * levelstate->south_require_number));
	}

	// =====================================
	// East exit
	// =====================================	
	
	// (2 bytes) East exit ID
	read(f, &levelstate->east, 2);
	
	// (2 bytes) East exit text label ID
	read(f, &levelstate->east_text, 2);
	
	// East condition (min 2 bytes, possibly 7+)
	read(f, &levelstate->east_eval_type, 1);
	read(f, &levelstate->east_require_number, 1);
	if (levelstate->east_require_number != 0){
		read(f, &levelstate->east_require, (COND_LENGTH * levelstate->east_require_number));
	}

	// =====================================
	// West exit
	// =====================================	
	
	// (2 bytes) West exit ID
	read(f, &levelstate->west, 2);
	
	// (2 bytes) West exit text label ID
	read(f, &levelstate->west_text, 2);
	
	// West condition (min 2 bytes, possibly 7+)
	read(f, &levelstate->west_eval_type, 1);
	read(f, &levelstate->west_require_number, 1);
	if (levelstate->west_eval_type != 0){
		read(f, &levelstate->west_require, (COND_LENGTH * levelstate->west_require_number));
	}
	
	// =====================================
	// Primary monsters
	// =====================================
	
	// (1 bytes) primary monster spawn chance
	read(f, &levelstate->spawn_chance, 1);
	
	// (1 byte) number of monster ID's that follow
	read(f, &levelstate->spawn_number, 1);
	if (levelstate->spawn_number > 0){
		read(f, &levelstate->spawn_list, levelstate->spawn_number);	
	}
	
	// Spawn condition (min 2 bytes, possibly 7+)
	read(f, &levelstate->spawn_eval_type, 1);
	read(f, &levelstate->spawn_require_number, 1);
	if (levelstate->spawn_require_number != 0){
		read(f, &levelstate->spawn_require, (COND_LENGTH * levelstate->spawn_require_number));
	}
	
	// =====================================
	// Secondary monsters
	// =====================================
	
	// (1 bytes) secondary monster spawn chance
	read(f, &levelstate->respawn_chance, 1);
	
	// (1 byte) number of monster ID's that follow
	read(f, &levelstate->respawn_number, 1);
	if (levelstate->respawn_number > 0){
		read(f, &levelstate->respawn_list, levelstate->respawn_number);	
	}
	
	// Spawn condition (min 2 bytes, possibly 7+)
	read(f, &levelstate->respawn_eval_type, 1);
	read(f, &levelstate->respawn_require_number, 1);
	if (levelstate->respawn_require_number != 0){
		read(f, &levelstate->respawn_require, (COND_LENGTH * levelstate->respawn_require_number));
	}

	// ====================================
	// Items/Treasure
	// ====================================
	
	// (1 bytes) item spawn chance
	read(f, &levelstate->items_chance, 1);
	
	// (1 byte) number of item ID's that follow
	read(f, &total_items, 1);
	
	// Empty the list of weapons and items
	levelstate->weapons_number = 0;
	levelstate->items_number = 0;
	
	// If we set a number of items, proceed to read each pair of 
	// bytes (item type + item id)
	if (total_items > 0){
		// Extract items and weapons and put them in the correct array
		i_count = 0;
		w_count = 0;
		
		for (i = 0; i < total_items; i++){
			read(f, &item_type, 1);
			read(f, &item_id, 1);
			
			// Check for 'w' or 'i'
			switch(item_type){
				case 119:
					levelstate->weapons_list[w_count] = item_id;
					levelstate->weapons_number++;
					w_count++;
					break;
				case 105:
					levelstate->items_list[i_count] = item_id;
					levelstate->items_number++;
					i_count++;
					break;
				default:
					break;
			}
		}			
	}
	
	// Item spawn condition (min 2 bytes, possibly 7+)
	read(f, &levelstate->items_eval_type, 1);
	read(f, &levelstate->items_require_number, 1);
	if (levelstate->items_require_number != 0){
		read(f, &levelstate->items_require, (COND_LENGTH * levelstate->items_require_number));
	}
	
	
	// ==================================================
	// (2 bytes) Text shown when primary monsters spawn
	// ==================================================
	read(f, &levelstate->text_spawn, 2);
	
	// ==================================================
	// (2 bytes) Text shown when primary monsters spawn
	// ==================================================
	read(f, &levelstate->text_after_spawn, 2);

	// ==================================================
	// (2 bytes) Text shown when primary monsters spawn
	// ==================================================
	read(f, &levelstate->text_respawn, 2);
	
	// ==================================================
	// (2 bytes) Text shown when primary monsters spawn
	// ==================================================
	read(f, &levelstate->text_after_respawn, 2);
		
	// ==================================================
	// NPC 1
	// ==================================================
	
	// (1 byte) NPC 1 ID
	read(f, &levelstate->npc1, 1);
	
	// NPC 1 spawn condition (min 2 bytes, possibly 7+)
	read(f, &levelstate->npc1_eval_type, 1);
	read(f, &levelstate->npc1_require_number, 1);
	if (levelstate->npc1_require_number != 0){
		read(f, &levelstate->npc1_require, (COND_LENGTH * levelstate->npc1_require_number));
	}
	
	// (1 byte) NPC 1 unique dialogue ID
	read(f, &levelstate->npc1_text_unique_id, 1);
	
	// (2 byte) NPC 1 text ID
	read(f, &levelstate->npc1_text, 2);
		
	// ==================================================
	// NPC 2
	// ==================================================
	
	// (1 byte) NPC 2 ID
	read(f, &levelstate->npc2, 1);
	
	// NPC 2 spawn condition (min 2 bytes, possibly 7+)
	read(f, &levelstate->npc2_eval_type, 1);
	read(f, &levelstate->npc2_require_number, 1);
	if (levelstate->npc2_require_number != 0){
		read(f, &levelstate->npc2_require, (COND_LENGTH * levelstate->npc2_require_number));
	}
	
	// (1 byte) NPC 2 unique dialogue ID
	read(f, &levelstate->npc2_text_unique_id, 1);
	// (2 byte) NPC 2 text ID
	read(f, &levelstate->npc2_text, 2);
	
	// ==================================================
	// NPC 3
	// ==================================================
	
	// (1 byte) NPC 3 ID
	read(f, &levelstate->npc3, 1);
	
	// NPC 3 spawn condition (min 2 bytes, possibly 7+)
	read(f, &levelstate->npc3_eval_type, 1);
	read(f, &levelstate->npc3_require_number, 1);
	if (levelstate->npc3_require_number != 0){
		read(f, &levelstate->npc3_require, (COND_LENGTH * levelstate->npc3_require_number));
	}
	
	// (1 byte) NPC 3 unique dialogue ID
	read(f, &levelstate->npc3_text_unique_id, 1);
	// (2 byte) NPC 3 text ID
	read(f, &levelstate->npc3_text, 2);
	
	levelstate->spawned = 0;	// Monsters have not spawned yet
	levelstate->has_npc1 = 0;	// NPC 1 ise not available until their condition requirements are evaluated
	levelstate->has_npc2 = 0;	// NPC 2
	levelstate->has_npc3 = 0;	// NPC 3
	
	close(f);
	return DATA_LOAD_OK;
}

int data_LoadStory(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned short id){
	// Load a story text fragment into the global ui text buffer
	
	unsigned short record_size = 0;
	unsigned long record_offset = 0;
	unsigned short i = 0;
	int f;
	
	f = open(STORY_IDX, O_RDONLY);
	if (f < 0){
		ui_DrawError(screen, DATA_LOAD_ERROR_MSG, DATA_LOAD_STORY_INDEX_MSG, f);
		return DATA_LOAD_STORY_INDEXFILE;	
	}
	
	// Seek to the right ID in the header
	lseek(f, (id * DATA_HEADER_ENTRY_SIZE), SEEK_SET);
	
	// Read the header entry for this record
	read(f, &record_size, DATA_HEADER_RECORD_SIZE);		// This is the size of the record, in bytes
	read(f, &record_offset, DATA_HEADER_OFFSET_SIZE);	// This is the offset of the record, in bytes from 0
	close(f);
	
	
	f = open(STORY_DAT, O_RDONLY);
	if (f < 0){
		ui_DrawError(screen, DATA_LOAD_ERROR_MSG, DATA_LOAD_STORY_DAT_MSG, f);
		return DATA_LOAD_STORY_DATFILE;	
	}
	// Seek to the data record itself
	lseek(f, record_offset, SEEK_SET);
		
	// Read DATA_HEADER_RECORD_SIZE worth of bytes
	memset(gamestate->buf, '\0', record_size + 1);
	read(f, gamestate->buf, record_size);
	
	close(f);
	return DATA_LOAD_OK;
}

/*
int data_LoadItem(Screen_t *screen, ItemState_t *itemstate, unsigned char id){
	// Load a single item definition from disk
	
	// Check for item in cache
	// 		return cached entry
	// Else
	// 		Open file and load record
	
	return DATA_LOAD_OK;
}
*/

int data_LoadWeapon(Screen_t *screen, WeaponState_t *weaponstate, unsigned char id){
	// Load a single weapon definition from disk
	
	return DATA_LOAD_OK;
}

int data_LoadSprite(Screen_t *screen, ssprite_t *sprite, unsigned short id){
	// Load a single (non boss) sprite into a ssprite_t structure
	
	int status;
	int f;
	
	f = open(SPRITE_DAT, O_RDONLY);
	if (f < 0){
		ui_DrawError(screen, DATA_LOAD_ERROR_MSG, DATA_LOAD_SPRITE_DAT_MSG, f);
		return DATA_LOAD_SPRITEFILE;	
	}
	
	// The position in the file is the storage size of a sprite * sprite_ID
	lseek(f, SPRITE_NORMAL_BYTES * id, SEEK_SET);
	status = read(f, sprite->pixels, SPRITE_DAT_SIZE);
	close(f);
	if (status < SPRITE_DAT_SIZE){
		ui_DrawError(screen, DATA_LOAD_ERROR_MSG, DATA_LOAD_SPRITE_DAT_READ, status);
		return DATA_LOAD_SPRITEFILE;
	}
	
	// Sprites are always a fixed size
	sprite->width = DRAW_PC_WIDTH;
	sprite->height = DRAW_PC_HEIGHT;
	sprite->bpp = 0;
	
	return DATA_LOAD_OK;
	
}

int data_LoadPortrait(Screen_t *screen, ssprite_t *sprite, unsigned short id){
	// Load a single portrait sprite into a ssprite_t structure
	
	int f;
	int status;
	
	f = open(PORTRAIT_DAT, O_RDONLY);
	if (f < 0){
		ui_DrawError(screen, DATA_LOAD_ERROR_MSG, DATA_LOAD_PORTRAIT_DAT_MSG, f);
		return DATA_LOAD_PORTRAITFILE;	
	}
	
	// The position in the file is the storage size of a sprite * sprite_ID
	lseek(f, PORTRAIT_DAT_SIZE * id, SEEK_SET);
	status = read(f, sprite->portrait, PORTRAIT_DAT_SIZE);
	close(f);
	if (status < PORTRAIT_DAT_SIZE){
		ui_DrawError(screen, DATA_LOAD_ERROR_MSG, DATA_LOAD_PORTRAIT_DAT_READ, status);
		return DATA_LOAD_BOSSFILE;
	}
	
	// Sprites are always a fixed size
	sprite->width = DRAW_PORTRAIT_WIDTH;
	sprite->height = DRAW_PORTRAIT_HEIGHT;
	sprite->bpp = 0;
	
	return DATA_LOAD_OK;
	
}

int data_LoadBoss(Screen_t *screen, lsprite_t *lsprite, unsigned short id){
	// Load single boss sprite into a lsprite_t structure
	
	int f;
	int status;
	
	f = open(BOSS_DAT, O_RDONLY);
	if (f < 0){
		ui_DrawError(screen, DATA_LOAD_ERROR_MSG, DATA_LOAD_BOSS_DAT_MSG, f);
		return DATA_LOAD_BOSSFILE;	
	}
	
	// The position in the file is the storage size of a sprite * sprite_ID
	lseek(f, BOSS_DAT_SIZE * id, SEEK_SET);
	status = read(f, lsprite->pixels, BOSS_DAT_SIZE);
	close(f);
	if (status < BOSS_DAT_SIZE){
		ui_DrawError(screen, DATA_LOAD_ERROR_MSG, DATA_LOAD_BOSS_DAT_READ, status);
		return DATA_LOAD_BOSSFILE;
	}
	
	// Sprites are always a fixed size
	lsprite->width = DRAW_BOSS_WIDTH;
	lsprite->height = DRAW_BOSS_HEIGHT;
	lsprite->bpp = 0;
	
	return DATA_LOAD_OK;
}

int data_CreateCharacter(Screen_t *screen, PlayerState_t *playerstate, ssprite_t *playersprite, lsprite_t *bosssprite, unsigned char character_type, short character_id){
	// Create a new player, party or enemy character
	// and load their sprite/portrait data
	
	int f;
	int status;
	unsigned short sprite_id, portrait_id;
	unsigned char i;
	unsigned char w = 0;
	unsigned char b;
	int seek_offset = MONSTER_ENTRY_SIZE * character_id;
	
	//printf("searching id: %d\n", character_id);
	
	// character_type NPC
	// Load from the NPC.DAT file
	if (character_type == CHARACTER_TYPE_NPC){
		f = open(NPC_DAT, O_RDONLY);
		if (f < 0){
			ui_DrawError(screen, DATA_LOAD_ERROR_MSG, DATA_LOAD_NPC_DAT_MSG, status);
			return DATA_LOAD_NPCFILE;	
		}
	} else {
		// character_type MONSTER / BOSS
		// Load data from the MONSTER.DAT file
		f = open(MONSTER_DAT, O_RDONLY);
		if (f < 0){
			ui_DrawError(screen, DATA_LOAD_ERROR_MSG, DATA_LOAD_MONSTER_DAT_MSG, status);
			return DATA_LOAD_MONSTERFILE;	
		}
	}
	
	// Seek to correct monster entry location
	status = lseek(f, seek_offset, SEEK_SET);
	if (status != seek_offset){
		ui_DrawError(screen, DATA_LOAD_ERROR_MSG, DATA_LOAD_MONSTER_DAT_SEEK, DATA_LOAD_MONSTERFILE_SEEK);
		return DATA_LOAD_MONSTERFILE;
	}
	
	// 1. (2 bytes) character ID
	read(f, &playerstate->id, 2);
	if (playerstate->id != character_id){
		ui_DrawError(screen, DATA_LOAD_ERROR_MSG, DATA_LOAD_MONSTER_MISMATCH_MSG, DATA_LOAD_MONSTER_MISMATCH);
		return DATA_LOAD_MONSTER_MISMATCH;	
	}
	
	// 2. (18 bytes) character name
	status = read(f, playerstate->name, MAX_PLAYER_NAME);
	strncpy(playerstate->short_name, playerstate->name, MAX_SHORT_NAME);
	
	// 3. (1 byte) character type (boss, enemy, npc)
	read(f, &playerstate->type, 1);
	
	// 4. (1 byte) character sprite type (boss, normal monster)
	read(f, &playerstate->sprite_type, 1);
	
	// 5a. (2 bytes) initial sprite ID 
	read(f, &sprite_id, 2);
	// 5b. (38 bytes) all other sprite IDs (not supported yet on QL)
	lseek(f, 38, SEEK_CUR);
	
	// 6. (2 bytes) portrait sprite ID 
	read(f, &portrait_id, 2);
	
	// 7. (1 byte) character class	
	read(f, &b, 1);
	playerstate->player_class = b << 4;	// Class is the lower 4 bits
	playerstate->player_race = b >> 4;		// Race is the upper 4 bits
	
	// 8. (1 byte) character level
	read(f, &playerstate->level, 1);
	
	// 9. (2 bytes) attack profile / aggression profile
	read(f, &playerstate->profile, 2);
	
	// 10. (1 byte) str
	read(f, &playerstate->str, 1);
	
	// 11. (1 byte) dex
	read(f, &playerstate->dex, 1);
	
	// 12. (1 byte) con
	read(f, &playerstate->con, 1);
	
	// 13. (1 byte) wis
	read(f, &playerstate->wis, 1);
	
	// 14. (1 byte) intl
	read(f, &playerstate->intl, 1);
	
	// 15. (1 byte) chr
	read(f, &playerstate->chr, 1);
	
	// 16. (2 bytes) hp
	read(f, &playerstate->hp, 2);
	playerstate->hp_reset = playerstate->hp; // Copy HP to hp_reset
	
	// 17. (4 bytes) status effects bitfield
	read(f, &playerstate->status, 4);
		
	// Equipped items
	
	playerstate->head = 0;
	playerstate->body = 0;
	playerstate->option = 0;
	
	playerstate->formation = FORMATION_FRONT;
	
	playerstate->kills = 0;
	playerstate->spells_cast = 0;
	playerstate->hits_taken = 0;
	playerstate->hits_caused = 0;
	
	close(f);
	
	// Load weapon one
	w = 0;
	if (w){
		data_LoadWeapon(screen, playerstate->weapon_r, w);	
	}
	
	// Load weapon two
	w = 0;
	if (w){
		data_LoadWeapon(screen, playerstate->weapon_l, w);
	}
	
	// Set initial items to empty
	for (i = 0; i < MAX_ITEMS; i++){
		playerstate->items[i] = 0;	
	}
	
	if (playerstate->type == CHARACTER_TYPE_BOSS){
		status = data_LoadBoss(screen, bosssprite, sprite_id); 		// Load the large, 96x96 boss sprite
	} else {
		status = data_LoadSprite(screen, playersprite, sprite_id); // Load a regular 32x32 player/enemy sprite
	}
	if (status != DATA_LOAD_OK){
		return status;
	}
	
	status = data_LoadPortrait(screen, playersprite, portrait_id); 	// Load the portrait/headshot
	if (status != DATA_LOAD_OK){
		return status;
	}
	
	return DATA_LOAD_OK;
}

char data_AddNPC(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned char id){
	// Adds a record of an NPC to the game list, if it does not already exist
	
	struct NPCList *npc;
	
	npc = data_FindNPC(gamestate->npcs, id);
	if (npc){
		// Found, no need to create
		return DATA_LOAD_OK;
	}
	
	// NPC does not exist
	// Find last record
	if (npc->id != 0){
		npc = data_LastNPC(gamestate->npcs);
	
		// Add another record for this NPC
		npc->next = (struct NPCList *) calloc(sizeof(struct NPCList), 1);
		if (npc->next == NULL){
			// Error allocating memory
			ui_DrawError(screen, GENERIC_MEMORY_MSG, DATA_LOAD_NPCMEMORY_MSG, 0);
			return DATA_LOAD_NPCMEMORY;
		}
	
		// Add this npc into the next slot
		npc->next->id = id;
		npc->next->talked_count = 0;
		npc->next->talked_time = 0;
		npc->next->death_time = 0;
		npc->next->next = NULL;	
	} else {
		
		// This is the first npc
		npc->id = id;
		npc->talked_count = 0;
		npc->talked_time = 0;
		npc->death_time = 0;
		npc->next = NULL;	
	}
	return DATA_LOAD_OK;
}

char data_SetNPCDead(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned char id, unsigned char dead){
	// Set the death time for this NPC to the current turn number
	
	struct NPCList *npc;
	
	npc = data_FindNPC(gamestate->npcs, id);
	if (npc == NULL){
		// Not found
		ui_DrawError(screen, DATA_LOAD_NPC_MISSING, DATA_LOAD_NPC_MISSING_DEATH, 0);
		return DATA_LOAD_NO_NPC;
	}
	
	if (dead){
		npc->death_time = gamestate->counter;
	} else {
		npc->death_time = 0;	
	}
	return OK;
}

char data_IncrementNPCTalk(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned char id, unsigned char unique_dialogue_id){
	// Increment the NPC talked_count figure and update the talked_time to current turn number
	
	struct NPCList *npc;
	
	npc = data_FindNPC(gamestate->npcs, id);
	if (npc == NULL){
		// Not found
		ui_DrawError(screen, DATA_LOAD_NPC_MISSING, DATA_LOAD_NPC_MISSING_TALK, 0);
		return DATA_LOAD_NO_NPC;
	}
	
	if (npc->talked_count < 255){
		npc->talked_count++;	
	}
	npc->talked_time = gamestate->counter;
	return OK;
}

struct NPCList * data_FindNPC(struct NPCList *npclist, unsigned char id){
	// Find an NPC in the linked list of visited NPCs
	while(npclist != NULL){
		if (npclist->id == id){
			return npclist;
		}
		npclist = npclist->next;
	}
	return NULL;
}

struct NPCList * data_LastNPC(struct NPCList *npclist){
	// Get the last NPC record
	while(npclist->next != NULL){
		npclist =  npclist->next;	
	}
	return npclist;
}

unsigned char data_CountNPC(struct NPCList *npclist){
	// Return count of encountered NPCs
	unsigned char i = 0;
	
	if (npclist->id){
		i++;
		//printf("NPC %d\n", npclist->id);
	}
	while(npclist->next != NULL){
		npclist = npclist->next;
		//printf("NPC %d\n", npclist->id);
		if (npclist->next != NULL){
			i++;
		}
	}
	return i;
}

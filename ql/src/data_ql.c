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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef _INPUT_H
#include "../common/input.h"
#define _INPUT_H
#endif
#ifndef _GAME_H
#include "../common/game.h"
#define _GAME_H
#endif
#ifndef _DATA_H
#include "../common/data.h"
#define _DATA_H
#endif
#ifndef _CONFIG_H
#include "../common/config.h"
#define _CONFIG_H
#endif
#ifndef _UI_H
#include "../common/ui.h"
#define _UI_H
#endif
#include "../common/conditions.h"

unsigned char data_LoadMap(GameState_t *gamestate, LevelState_t *levelstate, unsigned short id){
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
	
	index_file = fopen(MAP_IDX, "r");
	if (index_file == NULL){
		return DATA_LOAD_ERR;	
	}
	
	// Seek to the right ID in the header
	i = fseek(index_file, ((id - 1) * DATA_HEADER_ENTRY_SIZE), SEEK_SET);
	
	// Read the header entry for this record
	i = fread(&record_size, 1, DATA_HEADER_RECORD_SIZE, index_file);		// This is the size of the record, in bytes
	i = fread(&record_offset, 1, DATA_HEADER_OFFSET_SIZE, index_file);		// This is the offset of the record, in bytes from 0
	
	// Seek to the data record itself
	i = fseek(map_file, record_offset, SEEK_SET);
	
	// (2 bytes) Level ID
	i = fread(&levelstate->id, 1, sizeof(unsigned short), map_file);
	if (levelstate->id != id){
		return DATA_LOAD_ERR;	
	}
	
	// (2 bytes) Primary text ID
	i = fread(&levelstate->text, 1, sizeof(unsigned short), map_file);

	// (32 bytes) Level name
	i = fread(&levelstate->name, 1, MAX_LEVEL_NAME_SIZE, map_file);
	
	// =====================================
	// North exit
	// =====================================
	
	// (2 bytes) North exit ID
	i = fread(&levelstate->north, 1, sizeof(unsigned short), map_file);
	
	// (2 bytes) North exit text label ID
	i = fread(&levelstate->north_text, 1, sizeof(unsigned short), map_file);
	
	// North condition (min 2 bytes, possibly 7+)
	i = fread(&levelstate->north_eval_type, 1, 1, map_file);
	i = fread(&levelstate->north_require_number, 1, 1, map_file);
	if (levelstate->north_require_number != 0){
		i = fread(&levelstate->north_require, COND_LENGTH, levelstate->north_require_number, map_file);
	}
	
	// =====================================
	// South exit
	// =====================================	
	
	// (2 bytes) South exit ID
	i = fread(&levelstate->south, 1, sizeof(unsigned short), map_file);
	
	// (2 bytes) South exit text label ID
	i = fread(&levelstate->south_text, 1, sizeof(unsigned short), map_file);
	
	// South condition (min 2 bytes, possibly 7+)
	i = fread(&levelstate->south_eval_type, 1, 1, map_file);
	i = fread(&levelstate->south_require_number, 1, 1, map_file);
	if (levelstate->south_require_number != 0){
		i = fread(&levelstate->south_require, COND_LENGTH, levelstate->south_require_number, map_file);
	}

	// =====================================
	// East exit
	// =====================================	
	
	// (2 bytes) East exit ID
	i = fread(&levelstate->east, 1, sizeof(unsigned short), map_file);
	
	// (2 bytes) East exit text label ID
	i = fread(&levelstate->east_text, 1, sizeof(unsigned short), map_file);
	
	// East condition (min 2 bytes, possibly 7+)
	i = fread(&levelstate->east_eval_type, 1, 1, map_file);
	i = fread(&levelstate->east_require_number, 1, 1, map_file);
	if (levelstate->east_require_number != 0){
		i = fread(&levelstate->east_require, COND_LENGTH, levelstate->east_require_number, map_file);
	}

	// =====================================
	// West exit
	// =====================================	
	
	// (2 bytes) West exit ID
	i = fread(&levelstate->west, 1, sizeof(unsigned short), map_file);
	
	// (2 bytes) West exit text label ID
	i = fread(&levelstate->west_text, 1, sizeof(unsigned short), map_file);
	
	// West condition (min 2 bytes, possibly 7+)
	i = fread(&levelstate->west_eval_type, 1, 1, map_file);
	i = fread(&levelstate->west_require_number, 1, 1, map_file);
	if (levelstate->west_eval_type != 0){
		i = fread(&levelstate->west_require, COND_LENGTH, levelstate->west_require_number, map_file);
	}
	
	// =====================================
	// Primary monsters
	// =====================================
	
	// (1 bytes) primary monster spawn chance
	i = fread(&levelstate->spawn_chance, 1, 1, map_file);
	
	// (1 byte) number of monster ID's that follow
	i = fread(&levelstate->spawn_number, 1, 1, map_file);
	if (levelstate->spawn_number > 0){
		i = fread(&levelstate->spawn_list, levelstate->spawn_number, 1, map_file);	
	}
	
	// Spawn condition (min 2 bytes, possibly 7+)
	i = fread(&levelstate->spawn_eval_type, 1, 1, map_file);
	i = fread(&levelstate->spawn_require_number, 1, 1, map_file);
	if (levelstate->spawn_require_number != 0){
		i = fread(&levelstate->spawn_require, COND_LENGTH, levelstate->spawn_require_number, map_file);
	}
	
	// =====================================
	// Secondary monsters
	// =====================================
	
	// (1 bytes) secondary monster spawn chance
	i = fread(&levelstate->respawn_chance, 1, 1, map_file);
	
	// (1 byte) number of monster ID's that follow
	i = fread(&levelstate->respawn_number, 1, 1, map_file);
	if (levelstate->respawn_number > 0){
		i = fread(&levelstate->respawn_list, levelstate->respawn_number, 1, map_file);	
	}
	
	// Spawn condition (min 2 bytes, possibly 7+)
	i = fread(&levelstate->respawn_eval_type, 1, 1, map_file);
	i = fread(&levelstate->respawn_require_number, 1, 1, map_file);
	if (levelstate->respawn_require_number != 0){
		i = fread(&levelstate->respawn_require, COND_LENGTH, levelstate->respawn_require_number, map_file);
	}

	// ====================================
	// Items/Treasure
	// ====================================
	
	// (1 bytes) item spawn chance
	i = fread(&levelstate->items_chance, 1, 1, map_file);
	
	// (1 byte) number of item ID's that follow
	i = fread(&total_items, 1, 1, map_file);
	
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
			i = fread(&item_type, 1, 1, map_file);
			i = fread(&item_id, 1, 1, map_file);
			
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
	i = fread(&levelstate->items_eval_type, 1, 1, map_file);
	i = fread(&levelstate->items_require_number, 1, 1, map_file);
	if (levelstate->items_require_number != 0){
		i = fread(&levelstate->items_require, COND_LENGTH, levelstate->items_require_number, map_file);
	}
	
	
	// ==================================================
	// (2 bytes) Text shown when primary monsters spawn
	// ==================================================
	i = fread(&levelstate->text_spawn, 1, sizeof(unsigned short), map_file);
	
	// ==================================================
	// (2 bytes) Text shown when primary monsters spawn
	// ==================================================
	i = fread(&levelstate->text_after_spawn, 1, sizeof(unsigned short), map_file);

	// ==================================================
	// (2 bytes) Text shown when primary monsters spawn
	// ==================================================
	i = fread(&levelstate->text_respawn, 1, sizeof(unsigned short), map_file);
	
	// ==================================================
	// (2 bytes) Text shown when primary monsters spawn
	// ==================================================
	i = fread(&levelstate->text_after_respawn, 1, sizeof(unsigned short), map_file);
		
	// ==================================================
	// NPC 1
	// ==================================================
	
	// (1 byte) NPC 1 ID
	i = fread(&levelstate->npc1, 1, 1, map_file);
	
	// NPC 1 spawn condition (min 2 bytes, possibly 7+)
	i = fread(&levelstate->npc1_eval_type, 1, 1, map_file);
	i = fread(&levelstate->npc1_require_number, 1, 1, map_file);
	if (levelstate->npc1_require_number != 0){
		i = fread(&levelstate->npc1_require, COND_LENGTH, levelstate->npc1_require_number, map_file);
	}
	
	// (2 byte) NPC 1 text ID
	i = fread(&levelstate->npc1_text, 1, sizeof(unsigned short), map_file);
		
	// ==================================================
	// NPC 2
	// ==================================================
	
	// (1 byte) NPC 1 ID
	i = fread(&levelstate->npc2, 1, 1, map_file);
	
	// NPC 1 spawn condition (min 2 bytes, possibly 7+)
	i = fread(&levelstate->npc2_eval_type, 1, 1, map_file);
	i = fread(&levelstate->npc2_require_number, 1, 1, map_file);
	if (levelstate->npc2_require_number != 0){
		i = fread(&levelstate->npc2_require, COND_LENGTH, levelstate->npc2_require_number, map_file);
	}
	
	// (2 byte) NPC 1 text ID
	i = fread(&levelstate->npc2_text, 1, sizeof(unsigned short), map_file);
	
	// Monsters have not spawned yet
	levelstate->spawned = 0;
	
	i = fclose(index_file);
	return DATA_LOAD_OK;
}

unsigned char data_LoadStory(GameState_t *gamestate, LevelState_t *levelstate, unsigned short id){
	// Load a story text fragment into the global ui text buffer
	
	unsigned short record_size = 0;
	unsigned long record_offset = 0;
	unsigned short i = 0;
	
	index_file = fopen(STORY_IDX, "r");
	if (index_file == NULL){
		return DATA_LOAD_ERR;	
	}
	
	// Seek to the right ID in the header
	i = fseek(index_file, (id * DATA_HEADER_ENTRY_SIZE), SEEK_SET);
	
	// Read the header entry for this record
	i = fread(&record_size, 1, DATA_HEADER_RECORD_SIZE, index_file);		// This is the size of the record, in bytes
	i = fread(&record_offset, 1, DATA_HEADER_OFFSET_SIZE, index_file);	// This is the offset of the record, in bytes from 0
	
	// Seek to the data record itself
	i = fseek(story_file, record_offset, SEEK_SET);
		
	// Read DATA_HEADER_RECORD_SIZE worth of bytes
	memset(gamestate->buf, '\0', record_size + 1);
	i = fread(gamestate->buf, 1, record_size, story_file);
	
	i = fclose(index_file);
	return DATA_LOAD_OK;
}

/*
unsigned char data_LoadItem(ItemState_t *itemstate, unsigned char id){
	// Load a single item definition from disk
	
	// Check for item in cache
	// 		return cached entry
	// Else
	// 		Open file and load record
	
	return DATA_LOAD_OK;
}
*/

unsigned char data_LoadWeapon(WeaponState_t *weaponstate, unsigned char id){
	// Load a single weapon definition from disk
	
	return DATA_LOAD_OK;
}

unsigned char data_CreateCharacter(PlayerState_t *playerstate, ssprite_t *playersprite){
	// Create a new player, party or enemy character
	// and load their sprite/portrait data
	
	unsigned char i;
	unsigned char w = 0;
	
	// Option 1 - we create a character based on set attributes of a monster ID 
	
	// Option 2 - we create a character based on 
	
	// Option 3
	
	strncpy(playerstate->name, "Bob the dog", MAX_PLAYER_NAME);
	strncpy(playerstate->short_name, "Bob", MAX_SHORT_NAME);
	for (i = 0; i < MAX_ITEMS; i++){
		playerstate->items[i] = 0;	
	}
	//memcpy(playerstate->items, 0, MAX_ITEMS);
	
	playerstate->player_class = HUMAN_UNTRAINED;
	playerstate->level = 1;
	
	// Core stats
	playerstate->str = 10;
	playerstate->dex = 10;
	playerstate->con = 10;
	playerstate->wis = 10;
	playerstate->intl = 10;
	playerstate->chr = 10;
	playerstate->profile = 0;
	playerstate->hp = 10;
	playerstate->status = 0x00000000;
	
	// Equipped items
	playerstate->head = 0;
	playerstate->body = 0;
	playerstate->option = 0;
	
	playerstate->formation = FORMATION_FRONT;
	
	playerstate->kills = 0;
	playerstate->spells_cast = 0;
	playerstate->hits_taken = 0;
	playerstate->hits_caused = 0;
	
	// Load weapon one
	w = 0;
	if (w){
		data_LoadWeapon(playerstate->weapon_r, w);	
	}
	
	// Load weapon two
	w = 0;
	if (w){
		data_LoadWeapon(playerstate->weapon_l, w);
	}
	
	return DATA_LOAD_OK;
}

unsigned char data_AddNPC(GameState_t *gamestate, LevelState_t *levelstate, unsigned char id){
	// Adds a record of an NPC to the game list, if it does not already exist
	
	struct NPCList *npc;
	
	npc = data_FindNPC(gamestate->npcs, id);
	if (npc){
		// Found, no need to create
		return 1;
	}
	
	// NPC does not exist
	// Find last record
	if (npc->id != 0){
		npc = data_LastNPC(gamestate->npcs);
	}
	// Add another record for this NPC
	npc->next = (struct NPCList *) calloc(sizeof(struct NPCList), 1);
	if (npc->next == NULL){
		// Error allocating memory
		return 0;
	}
		
	npc->next->id = id;
	npc->next->talked_count = 0;
	npc->next->talked_time = 0;
	npc->next->death_time = 0;
	npc->next->next = NULL;
	return 1;
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
		printf("NPC %d\n", npclist->id);
	}
	while(npclist->next != NULL){
		npclist = npclist->next;
		printf("NPC %d\n", npclist->id);
		if (npclist->next != NULL){
			i++;
		}
	}
	return i;
}

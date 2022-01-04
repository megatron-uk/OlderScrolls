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
#include "../common/conditions.h"

unsigned char data_Load(unsigned char data_type, unsigned short id){
	
	switch(data_type){
		case DATA_TYPE_STORY:
			return data_LoadStory(id);
			break;
		case DATA_TYPE_MAP:
			return data_LoadMap(id);
			break;
		default:
			break;
	}
}

unsigned char data_LoadMap(unsigned short id){
	// Load a gameworld map from disk, parsing it and inserting
	// the data into the global 'levelstate' struct.
	
	unsigned short record_size = 0;
	unsigned long record_offset = 0;
	unsigned char cond_type = 0;
	unsigned char cond_number = 0;
	unsigned char i;
	unsigned char total_items = 0;
	unsigned char item_type;
	unsigned char item_id;
	unsigned char w_count = 0;
	unsigned char i_count = 0;
	unsigned char buf[MAX_LEVEL_NAME_SIZE + 1];
	
	
	index_file = fopen(MAP_IDX, "r");
	if (index_file == NULL){
		return DATA_LOAD_ERR;	
	}
	
	// Seek to the right ID in the header
	fseek(index_file, ((id - 1) * DATA_HEADER_ENTRY_SIZE), SEEK_SET);
	
	// Read the header entry for this record
	i = fread(&record_size, 1, DATA_HEADER_RECORD_SIZE, index_file);		// This is the size of the record, in bytes
	i = fread(&record_offset, 1, DATA_HEADER_OFFSET_SIZE, index_file);		// This is the offset of the record, in bytes from 0
	
	// Seek to the data record itself
	fseek(map_file, record_offset, SEEK_SET);
	
	// (2 bytes) Level ID
	i = fread(&levelstate.id, 1, sizeof(unsigned short), map_file);
	if (levelstate.id != id){
		return DATA_LOAD_ERR;	
	}
	
	// (2 bytes) Primary text ID
	i = fread(&levelstate.text, 1, sizeof(unsigned short), map_file);

	// (32 bytes) Level name
	memset(&levelstate.name, 0, MAX_LEVEL_NAME_SIZE + 1);
	i = fread(&levelstate.name, 1, MAX_LEVEL_NAME_SIZE, map_file);
	
	// =====================================
	// North exit
	// =====================================
	
	// (2 bytes) North exit ID
	fread(&levelstate.north, 1, sizeof(unsigned short), map_file);
	
	// (2 bytes) North exit text label ID
	fread(&levelstate.north_text, 1, sizeof(unsigned short), map_file);
	
	// North condition (min 2 bytes, possibly 7+)
	fread(&levelstate.north_eval_type, 1, 1, map_file);
	fread(&levelstate.north_require_number, 1, 1, map_file);
	//memset(levelstate.north_require, '\0', MAX_REQUIREMENTS * REQUIREMENT_BYTES);
	if (levelstate.north_require_number != 0){
		fread(&levelstate.north_require, COND_LENGTH, levelstate.north_require_number, map_file);
	}
	//printf("North  ID: %2d, Text: %2d, Eval: %2d, Length: %2d\n", levelstate.north, levelstate.north_text, levelstate.north_eval_type, levelstate.north_require_number);
	
	// =====================================
	// South exit
	// =====================================	
	
	// (2 bytes) South exit ID
	fread(&levelstate.south, 1, sizeof(unsigned short), map_file);
	
	// (2 bytes) South exit text label ID
	fread(&levelstate.south_text, 1, sizeof(unsigned short), map_file);
	
	// South condition (min 2 bytes, possibly 7+)
	fread(&levelstate.south_eval_type, 1, 1, map_file);
	fread(&levelstate.south_require_number, 1, 1, map_file);
	//memset(levelstate.south_require, '\0', MAX_REQUIREMENTS * REQUIREMENT_BYTES);
	if (levelstate.south_require_number != 0){
		fread(&levelstate.south_require, COND_LENGTH, levelstate.south_require_number, map_file);
	}
	//printf("South  ID: %2d, Text: %2d, Eval: %2d, Length: %2d\n", levelstate.south, levelstate.south_text, levelstate.south_eval_type, levelstate.south_require_number);

	// =====================================
	// East exit
	// =====================================	
	
	// (2 bytes) East exit ID
	fread(&levelstate.east, 1, sizeof(unsigned short), map_file);
	
	// (2 bytes) East exit text label ID
	fread(&levelstate.east_text, 1, sizeof(unsigned short), map_file);
	
	// East condition (min 2 bytes, possibly 7+)
	fread(&levelstate.east_eval_type, 1, 1, map_file);
	fread(&levelstate.east_require_number, 1, 1, map_file);
	//memset(levelstate.east_require, '\0', MAX_REQUIREMENTS * REQUIREMENT_BYTES);
	if (levelstate.east_require_number != 0){
		fread(&levelstate.east_require, COND_LENGTH, levelstate.east_require_number, map_file);
	}
	//printf("East   ID: %2d, Text: %2d, Eval: %2d, Length: %2d\n", levelstate.east, levelstate.east_text, levelstate.east_eval_type, levelstate.east_require_number);

	// =====================================
	// West exit
	// =====================================	
	
	// (2 bytes) West exit ID
	fread(&levelstate.west, 1, sizeof(unsigned short), map_file);
	
	// (2 bytes) West exit text label ID
	fread(&levelstate.west_text, 1, sizeof(unsigned short), map_file);
	
	// West condition (min 2 bytes, possibly 7+)
	fread(&levelstate.west_eval_type, 1, 1, map_file);
	fread(&levelstate.west_require_number, 1, 1, map_file);
	//memset(levelstate.west_require, '\0', MAX_REQUIREMENTS * REQUIREMENT_BYTES);
	if (levelstate.west_eval_type != 0){
		fread(&levelstate.west_require, COND_LENGTH, levelstate.west_require_number, map_file);
	}
	//printf("West   ID: %2d, Text: %2d, Eval: %2d, Length: %2d\n", levelstate.west, levelstate.west_text, levelstate.west_eval_type, levelstate.west_require_number);
	
	// =====================================
	// Primary monsters
	// =====================================
	
	// (1 bytes) primary monster spawn chance
	fread(&levelstate.spawn_chance, 1, 1, map_file);
	
	// (1 byte) number of monster ID's that follow
	fread(&levelstate.spawn_number, 1, 1, map_file);
	//memset(levelstate.spawn_require, '\0', MAX_REQUIREMENTS * REQUIREMENT_BYTES);
	if (levelstate.spawn_number > 0){
		fread(&levelstate.spawn_list, levelstate.spawn_number, 1, map_file);	
	}
	
	// Spawn condition (min 2 bytes, possibly 7+)
	fread(&levelstate.spawn_eval_type, 1, 1, map_file);
	fread(&levelstate.spawn_require_number, 1, 1, map_file);
	if (levelstate.spawn_require_number != 0){
		fread(&levelstate.spawn_require, COND_LENGTH, levelstate.spawn_require_number, map_file);
	}
	//printf("Spawn 1: %3d%, Monsters: %2d, Eval: %2d, Length: %2d\n", levelstate.spawn_chance, levelstate.spawn_number, levelstate.spawn_eval_type, levelstate.spawn_require_number);
	
	// =====================================
	// Secondary monsters
	// =====================================
	
	// (1 bytes) secondary monster spawn chance
	fread(&levelstate.respawn_chance, 1, 1, map_file);
	
	// (1 byte) number of monster ID's that follow
	fread(&levelstate.respawn_number, 1, 1, map_file);
	//memset(levelstate.respawn_require, '\0', MAX_REQUIREMENTS * REQUIREMENT_BYTES);
	if (levelstate.respawn_number > 0){
		fread(&levelstate.respawn_list, levelstate.respawn_number, 1, map_file);	
	}
	
	// Spawn condition (min 2 bytes, possibly 7+)
	fread(&levelstate.respawn_eval_type, 1, 1, map_file);
	fread(&levelstate.respawn_require_number, 1, 1, map_file);
	if (levelstate.respawn_require_number != 0){
		fread(&levelstate.respawn_require, COND_LENGTH, levelstate.respawn_require_number, map_file);
	}
	//printf("Spawn 2: %3d%, Monsters: %2d, Eval: %2d, Length: %2d\n", levelstate.respawn_chance, levelstate.respawn_number, levelstate.respawn_eval_type, levelstate.respawn_require_number);

	// ====================================
	// Items/Treasure
	// ====================================
	
	// (1 bytes) item spawn chance
	fread(&levelstate.items_chance, 1, 1, map_file);
	
	// (1 byte) number of item ID's that follow
	fread(&total_items, 1, 1, map_file);
	
	// Empty the list of weapons and items
	//memset(levelstate.weapons_list, '\0', MAX_REWARD_ITEMS);
	//memset(levelstate.items_list, '\0', MAX_REWARD_ITEMS);
	levelstate.weapons_number = 0;
	levelstate.items_number = 0;
	
	// If we set a number of items, proceed to read each pair of 
	// bytes (item type + item id)
	if (total_items > 0){
		// Extract items and weapons and put them in the correct array
		i_count = 0;
		w_count = 0;
		
		for (i = 0; i < total_items; i++){
			fread(&item_type, 1, 1, map_file);
			fread(&item_id, 1, 1, map_file);
			
			// Check for 'w' or 'i'
			switch(item_type){
				case 119:
					levelstate.weapons_list[w_count] = item_id;
					levelstate.weapons_number++;
					w_count++;
					break;
				case 105:
					levelstate.items_list[i_count] = item_id;
					levelstate.items_number++;
					i_count++;
					break;
				default:
					break;
			}
		}			
	}
	
	// Item spawn condition (min 2 bytes, possibly 7+)
	fread(&levelstate.items_eval_type, 1, 1, map_file);
	fread(&levelstate.items_require_number, 1, 1, map_file);
	//memset(levelstate.items_require, '\0', MAX_REQUIREMENTS * REQUIREMENT_BYTES);
	if (levelstate.items_require_number != 0){
		fread(&levelstate.items_require, COND_LENGTH, levelstate.items_require_number, map_file);
	}
	//printf("Items Total: %2d, Items: %2d, Weapons: %2d, Eval: %2d, Length: %2d\n", total_items, levelstate.items_number, levelstate.weapons_number, levelstate.items_eval_type, levelstate.items_require_number);
	
	
	// ==================================================
	// (2 bytes) Text shown when primary monsters spawn
	// ==================================================
	fread(&levelstate.text_spawn, 1, sizeof(unsigned short), map_file);
	
	// ==================================================
	// (2 bytes) Text shown when primary monsters spawn
	// ==================================================
	fread(&levelstate.text_after_spawn, 1, sizeof(unsigned short), map_file);

	// ==================================================
	// (2 bytes) Text shown when primary monsters spawn
	// ==================================================
	fread(&levelstate.text_respawn, 1, sizeof(unsigned short), map_file);
	
	// ==================================================
	// (2 bytes) Text shown when primary monsters spawn
	// ==================================================
	fread(&levelstate.text_after_respawn, 1, sizeof(unsigned short), map_file);
		
	//printf("Spawn Text  : Pre: %2d, Post: %2d\n", levelstate.text_spawn, levelstate.text_after_spawn);
	//printf("Respawn Text: Pre: %2d, Post: %2d\n", levelstate.text_respawn, levelstate.text_after_respawn);

		
	// ==================================================
	// NPC 1
	// ==================================================
	
	// (1 byte) NPC 1 ID
	fread(&levelstate.npc1, 1, 1, map_file);
	
	// NPC 1 spawn condition (min 2 bytes, possibly 7+)
	fread(&levelstate.npc1_eval_type, 1, 1, map_file);
	fread(&levelstate.npc1_require_number, 1, 1, map_file);
	//memset(levelstate.npc1_require, '\0', MAX_REQUIREMENTS * REQUIREMENT_BYTES);
	if (levelstate.npc1_require_number != 0){
		fread(&levelstate.npc1_require, COND_LENGTH, levelstate.npc1_require_number, map_file);
	}
	
	// (2 byte) NPC 1 text ID
	fread(&levelstate.npc1_text, 1, sizeof(unsigned short), map_file);
	
	//printf("NPC 1 ID: %2d, Text: %2d, Eval: %2d, Length %2d\n", levelstate.npc1, levelstate.npc1_text, levelstate.npc1_eval_type, levelstate.npc1_require_number);
	
	// ==================================================
	// NPC 2
	// ==================================================
	
	// (1 byte) NPC 1 ID
	fread(&levelstate.npc2, 1, 1, map_file);
	
	// NPC 1 spawn condition (min 2 bytes, possibly 7+)
	fread(&levelstate.npc2_eval_type, 1, 1, map_file);
	fread(&levelstate.npc2_require_number, 1, 1, map_file);
	//memset(levelstate.npc2_require, '\0', MAX_REQUIREMENTS * REQUIREMENT_BYTES);
	if (levelstate.npc2_require_number != 0){
		fread(&levelstate.npc2_require, COND_LENGTH, levelstate.npc2_require_number, map_file);
	}
	
	// (2 byte) NPC 1 text ID
	fread(&levelstate.npc2_text, 1, sizeof(unsigned short), map_file);
	
	//printf("NPC 2 ID: %2d, Text: %2d, Eval: %2d, Length %2d\n", levelstate.npc2, levelstate.npc2_text, levelstate.npc2_eval_type, levelstate.npc2_require_number);

	fclose(index_file);
	
	return DATA_LOAD_OK;
}

unsigned char data_LoadStory(unsigned short id){
	return DATA_LOAD_OK;
}
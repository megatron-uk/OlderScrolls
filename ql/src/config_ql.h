/* config_ql.h, Filenames and assorted configuration settings specific
 to the Sinclair QL implementation.
 
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

#ifndef _CONFIG_QL_DEFS_H
#define _CONFIG_QL_DEFS_H

//#define ENGINE_TARGET_NAME	"QL"
#define ENGINE_TARGET_NAME	"<g>OlderScrolls<C> <r>QL<C>"

#define FONT_8X8		"font8x8_bmp"	// Note the underscore as present when copied to the QL filesystem
#define FONT_8X8_FANCY	"font8x8b_bmp"	// Locations, NPC names, fancy words in story text etc

// Items
#define WEAPON_DAT		"weapon_dat"
#define ITEM_DAT		"item_dat"
// People and monsters
#define MONSTER_DAT		"monster_dat"	// Fixed size entries
#define NPC_DAT			"npc_dat"		// Fixed size entries, identical to MONSTER_DAT entries
// Locations and story text
#define MAP_DAT			"world_dat"		
#define MAP_IDX			"world_idx"		// Map location is variable size, so read from index
#define STORY_DAT		"story_dat"
#define STORY_IDX		"story_idx"		// Story entry is variable size, so read from index
// Graphic assets
#define SPRITE_DAT		"sprite_dat"	// Fixed size entries, see below
#define PORTRAIT_DAT 	"portrait_dat"	// Fixed size entries, see below
#define BOSS_DAT		"boss_dat"		// Fixed size entries, see below

#define SPRITE_DAT_SIZE		256		// Size of graphics elements are specific to QL bitmap modes only
#define PORTRAIT_DAT_SIZE	256		// Size of graphics elements are specific to QL bitmap modes only
#define BOSS_DAT_SIZE		2304	// Size of graphics elements are specific to QL bitmap modes only
#define MONSTER_ENTRY_SIZE	91		// Size of a single monster/npc datafile entry

#endif
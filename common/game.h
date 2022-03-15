/* game.h, Internal game data structure definitions and common prototypes to
 all engine implementations.
 
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

#ifdef _GAME_H
#error "The module game.h is already included by another module"
#endif

#ifndef _GAME_H
#define _GAME_H

#define GAME_MODE_MAP		1		// General mode, reading text, with movement and talk options
#define GAME_MODE_COMBAT	2		// In combat
#define GAME_MODE_SHOP		3		// In shop
#define GAME_MODE_EXIT		99		// Exit from game

#define MAX_PLAYERS			4
#define MAX_PLAYER_NAME 	18
#define MAX_WEAPON_NAME		18
#define MAX_SHORT_NAME 		8
#define MAX_REWARD_ITEMS 	6		// Number of items that may be rewarded upon visiting a location, or upon defeat of primary monster(s)
#define MAX_ITEMS 			16		// The size of player inventory
#define MAX_LEVEL_NAME_SIZE 32		// How long a level name can be
#define MAX_STORY_TEXT_SIZE 1024 	// The buffer which holds the text to be shown on screen about a location
#define MAX_LOCATIONS 		256		// The maximum number of levels we can track - stories should not have more than this!
#define MAX_CHARACTERS		256		// Number of monsters, npcs or player characters (8 bit ID)
#define MAX_REQUIREMENTS 	8		// Actions can have prerequisites (or even multiple prerequisities) before they happen
#define MAX_MONSTER_TYPES 	6		// The number of monsters in each location
#define MAX_BOSS_TYPES		1
#define MAX_EFFECTS			5		// maximum number of effects a spell or item can have
#define MAX_DAMAGE_TYPES	3
#define REQUIREMENT_BYTES 	5		// 5 bytes per requirement
#define MAX_PLAYER_CLASSES 16
#define MAX_PROFICIENCIES	10
#define MAX_PLAYER_RACES	5

// Characters can be of 3 different types
#define CHARACTER_TYPE_MONSTER 0
#define CHARACTER_TYPE_NPC		1
#define CHARACTER_TYPE_BOSS		2

#define ITEM_TYPE_NONE			0x00
#define ITEM_TYPE_ITEM			0x69
#define ITEM_TYPE_WEAPON		0x77

// Structure representing the data associated with a single weapon
// This is everything we need to know in order to carry out combat with this weapon
// critical range, damage type
typedef struct {
	unsigned char item_id;
	unsigned char weapon_type;		// 1H, 2H, etc
	unsigned char weapon_class;		// Simple, martial, ranged, magical etc
	unsigned char size;				// small, medium, large, huge, etc
	unsigned char rarity;			// Common, uncommon, rare, legendary
	unsigned char name[MAX_WEAPON_NAME];			// Name of weapon, e.g. "Longsword"
	unsigned char proficiency_1	;	//
	unsigned char proficiency_2	;	//
	unsigned char crit_min;			// Minimum roll for critical, e.g 19
	unsigned char crit_max;			// Maximum roll for critical, e.g. 20
	unsigned char crit_dice_qty;	// Number of rolls if critical, e.g. 2x
	unsigned char versatile;
	unsigned char finesse;
	unsigned char silvered;
	unsigned char bonus;
	unsigned short value;
	unsigned char dmg1_type;		// e.g. PHYSICAL
	unsigned char dmg1_dice_qty;	
	unsigned char dmg1_dice_type;	
	unsigned char dmg2_type;		// e.g. SLASHING
	unsigned char dmg2_dice_qty;	
	unsigned char dmg2_dice_type;
	unsigned char dmg3_type;		// e.g. PIERCING
	unsigned char dmg3_dice_qty;	
	unsigned char dmg3_dice_type;
	unsigned short text_id;			// ID of the text which describes this item
} WeaponState_t;

// Data for a single spell
typedef struct {
	unsigned char spell_id;			// 1-255
	unsigned char oneshot;			// Is it a single use spell? i.e. a scroll?
	unsigned char spell_type;		// The effect of the spell
									// SPELL_TYPE_ATTACK	- offensive attack against hostile
									// SPELL_TYPE_SUPPORT	- buff of self, party member or party, debuff of hostile character or party
	unsigned char name[MAX_PLAYER_NAME];			// Name of spell:63 "Lightning Bolt"
	unsigned char cooldown;			// Number of turns until the spell can be used again, decremented each turn
	unsigned char cooldown_reset;	// What cooldown resets to when cast
	unsigned char target;			// Who/what the spell can be cast on:
									// SPELL_TARGET_SELF 				- caster only
									// SPELL_TARGET_FRIENDLY_PARTY 		- cast on any one character in party
									// SPELL_TARGET_FRIENDLY_PARTY_ALL - cast on entire party
									// SPELL_TARGET_HOSTILE_PARTY		- cast on any one character in hostile party
									// SPELL_TARGET_HOSTILE_PARTY_ALL	- cast on entire hostile party
	unsigned char effectlist[MAX_EFFECTS];	// List of effect ID's. Effects are what the spell actually does
} SpellState_t;

// Data for a single item
typedef struct {
	unsigned char item_id;			// 1-255
	unsigned char name[MAX_PLAYER_NAME];			// Name of Item:1 "Potion"
	unsigned char class_limit;		// UNTRAINED, PALADIN, etc
	unsigned char race_limit;		// HUMAN, ORC, ELF, etc
	unsigned char type;				// ITEM_TYPE_ARMOUR, ITEM_TYPE_CONSUMEABLE, etc
	unsigned char slot;				// SLOT_TYPE_BODY, SLOT_TYPE_NONE, etc
	unsigned short value;			// How much it costs, base value
	unsigned char ac;				// Armour class for armour items
	unsigned char ac_type;			// ARMOUR_TYPE_LIGHT, etc
	unsigned char effectlist[MAX_EFFECTS];	// List of effect ID's. Effects are what the item does on use/equip.
	unsigned short text_id;			// ID of the text which describes this item
} ItemState_t;

struct Item_t {
	unsigned char item_type;		// item_type == ascii 'w' (weapon)
	unsigned char item_id;			// item_id == 22 (weapon id 22)
	unsigned char qty;				// qty == 7x (7 of these items)
};

// Structure representing the status of a single NPC or PC
typedef struct {
	
	char name[MAX_PLAYER_NAME + 1];			// Full player character name, e.g. Argus the Dread
	char short_name[MAX_SHORT_NAME + 1];	// Player character name, e.g. Argus

	unsigned short id;					// Character ID
	unsigned char type;					// CHARACTER_TYPE_NPC, CHARACTER_TYPE_MONSTER, CHARACTER_TYPE_BOSS
	unsigned char sprite_type;			// SPRITE_CLASS_NORMAL, SPRITE_CLASS_BOSS
	unsigned char player_class;			// ROGUE, DRUID, GENERIC_MELEE, UNTRAINED etc, see monsters.h
	unsigned char player_race;			// HUMAN, ELF, ORC, BEAST etc
	unsigned char level;				// 1-10
	unsigned short profile;				// Melee, Ranged, Magic Attack, Magic Support behaviour
										// 4 bits each for how aggressive the character is in that area.
										
	// Core stats
	unsigned char str;					// 0-20
	unsigned char dex;					// 0-20
	unsigned char con;					// 0-20
	unsigned char wis;					// 0-20
	unsigned char intl;					// 0-20
	unsigned char chr;					// 0-20
	
	// Hitpoints and status effects
	unsigned short hp;					// Current Hit points
	unsigned short hp_reset;			// Base/original Hit points
	unsigned long status;				// 32bit bitfield of status effects - see status.h
	
	// Equipped items
	ItemState_t *head;					// head
	ItemState_t *body;					// body
	ItemState_t *option;				// ring/pendant etc
	
	// Location in party when combat begins
	unsigned char formation;			// front/middle/rear
										// front gets a bonus to non-ranged attacks on the enemy front row
										// middle gets no bonuses or penalties
										// rear gets bonus to defence, but penalty to non-ranged attacks
	
	// Item store for this player
	struct Item_t items[MAX_ITEMS];		// items
	
	// Individual player stats
	unsigned short kills;				// Record of how many kills this player has made so far
	unsigned short spells_cast;			// Record of how many spells this player has cast
	unsigned short hits_taken;			// Record of how much damage this player has taken so far
	unsigned short hits_caused;			// Record of how much damage this player has inflicted so far
	
	// Equipped weapons/weapon and shield
	WeaponState_t *weapon_r;			// Data for the weapon currently in the right hand
	WeaponState_t *weapon_l;			// Data for the weapon currently in the left hand		
	
	
	
} PlayerState_t;

// List of players in party
typedef struct {
	unsigned char current;					// array entry of current selected player
	PlayerState_t *player[MAX_PLAYERS];		// array of player characters
} PartyState_t;

// This structure is initialised each time we begin combat with 
// one or more enemy
typedef struct {
	unsigned char current;						// array entry of current enemy
	PlayerState_t *enemy[MAX_MONSTER_TYPES];	// array of enemy characters
} EnemyState_t;

// A list of NPCs we encounter, the 
struct NPCList {
	unsigned char id;					// ID of NPC
	unsigned char talked_count;			// Number of times talked
	unsigned short talked_time;			// Last turn number we talked to them
	unsigned short death_time;			// Time of death / turn number
	long double discussions;			// 64bit bitfield of up to 64 unique dialogues that have occurred.
	struct NPCList *next;				// Pointer to next NPC record
};

// Basic game data
typedef struct {
	char text_buffer[MAX_STORY_TEXT_SIZE + 513];				// A single text buffer to composite any text used for display in the main window. This is 1.5x the size of a normal text string
	char buf[MAX_STORY_TEXT_SIZE + 1];							// To load strings into
	unsigned char gamemode;										// What mode we are in - map, combat, shop, etc
	unsigned char name[MAX_LEVEL_NAME_SIZE];					// Name of the current adventure
	unsigned char level;										// ID of the current location
	unsigned char level_previous;								// ID of the immediately previous location
	unsigned char level_looted[MAX_LOCATIONS];					// Each level has a count of how many times it has been looted
	unsigned char level_visits[MAX_LOCATIONS];					// Each level has a count of how many times it has been visited
	unsigned char level_defeated_primary[MAX_LOCATIONS];		// Each level has a flag to indicate whether the primary monster(s) has been defeated
	unsigned char level_defeated_secondary[MAX_LOCATIONS];		// Each level has a flag to indicate whether the secondary monster(s) has been defeated
	unsigned short counter;										// Game turn/timer/counter
	unsigned short gold;										// Record of currency
	PartyState_t *players;								// partystate, which holds the list of players in party
	EnemyState_t *enemies;								// enemystate, which holds the list of enemies in combat
	struct NPCList *npcs;										// Pointer to a linked-list of NPC's we have met
} GameState_t;

// Each level that we visit is loaded from disk into this structure
// This way we can have (effectively) unlimited number of 'rooms' in 
// our adventure and only need to load one at a time.
//
// This takes up approximately 500 bytes.
typedef struct {
	unsigned short id;								// Unique ID of the location
	unsigned char name[MAX_LEVEL_NAME_SIZE + 1];	// Name of the location
	unsigned short text;							// The *default* story text for this level
	
	// Navigation options
	unsigned short north;						// ID of the location to the north
	unsigned short south;						// ID of the location to the south
	unsigned short east;						// ID of the location to the east
	unsigned short west;						// ID of the location to the west
	
	// Navigation UI display names
	unsigned short north_text;					// ID of the text label shown for the north exit
	unsigned short south_text;					// ID of the text label shown for the south exit
	unsigned short east_text;					// ID of the text label shown for the east exit
	unsigned short west_text;					// ID of the text label shown for the west exit
	
	// Conditions to enable navigation options
	unsigned char north_require[MAX_REQUIREMENTS * REQUIREMENT_BYTES];	// Requirements to exit north
	unsigned char south_require[MAX_REQUIREMENTS * REQUIREMENT_BYTES];	// Requirements to exit south
	unsigned char east_require[MAX_REQUIREMENTS * REQUIREMENT_BYTES];		// Requirements to exit east
	unsigned char west_require[MAX_REQUIREMENTS * REQUIREMENT_BYTES];		// Requirements to exit west
	
	// Condition evaluation types
	unsigned char north_eval_type;					// EMPTY, AND, OR, etc
	unsigned char south_eval_type;					// EMPTY, AND, OR, etc
	unsigned char east_eval_type;					// EMPTY, AND, OR, etc
	unsigned char west_eval_type;					// EMPTY, AND, OR, etc
	
	unsigned char north_require_number;					// EMPTY, AND, OR, etc
	unsigned char south_require_number;					// EMPTY, AND, OR, etc
	unsigned char east_require_number;					// EMPTY, AND, OR, etc
	unsigned char west_require_number;					// EMPTY, AND, OR, etc

	
	// Primary set of monsters for this location
	unsigned char spawn_chance;						// 0-100 Chance of monsters spawning on initial location load
	unsigned char spawn_number;						// NUmber of monster IDs in the spawn list
	unsigned char spawn_list[MAX_MONSTER_TYPES];	// A list of the possible monster ID's for this location
	unsigned char spawn_require[MAX_REQUIREMENTS * REQUIREMENT_BYTES];	// Things which need to be true to spawn monsters
	unsigned char spawn_require_number;				// The number of requirements listed
	unsigned char spawn_eval_type;					// EMPTY, AND, OR, etc
	
	unsigned short text_spawn;						// ID of text label shown when monsters spawn
	unsigned short text_after_spawn;				// ID of text label shown after spawned monsters are killed
	
	// Monsters that may respawn in this location
	unsigned char respawn_chance;					// 0-100 Chance of monsters respawning on subsequent location load
	unsigned char respawn_number;					// Number of monsters IDs in the respawn list
	unsigned char respawn_list[MAX_MONSTER_TYPES];	// A list of the possible monster ID's for this location
	unsigned char respawn_require[MAX_REQUIREMENTS * REQUIREMENT_BYTES];// Things which need to be true to respawn monsters
	unsigned char respawn_require_number;			// The number of requirements listed for respawning
	unsigned char respawn_eval_type;				// EMPTY, AND, OR, etc
	
	unsigned short text_respawn;					// ID of text label shown when monsters respawn
	unsigned short text_after_respawn;				// ID of text label shown after respawned monsters are killed

	// Monsters-have-spawned indicator
	unsigned char spawned;							// Flag set once either type of monster spawn
	
	
	// Items may appear after defeating monsters
	unsigned char weapons_list[MAX_REWARD_ITEMS];		// List of items rewarded upon visiting location, or upon defeating primary monsters, if present
	unsigned char items_list[MAX_REWARD_ITEMS];		// List of items rewarded upon visiting location, or upon defeating primary monsters, if present
	unsigned char items_chance;							// Each item has a chance of being present
	unsigned char weapons_number;
	unsigned char items_number;
	unsigned char items_require[MAX_REQUIREMENTS * REQUIREMENT_BYTES];	// To receive the items, these requirements must be met
	unsigned char items_require_number;
	unsigned char items_eval_type;
	
	// Which NPCs may appear
	unsigned char has_npc1;
	unsigned char npc1;									// ID of NPC
	unsigned char npc1_require[MAX_REQUIREMENTS * REQUIREMENT_BYTES];	// To see this NPC, these requirements must be met
	unsigned char npc1_require_number;
	unsigned char npc1_eval_type;						// EMPTY, AND, OR, etc.
	unsigned short npc1_text;							// ID of text shown when talking to this NPC
	unsigned char npc1_text_unique_id;					// Unique id of this conversation
	
	unsigned char has_npc2;
	unsigned char npc2;									// ID of NPC
	unsigned char npc2_require[MAX_REQUIREMENTS * REQUIREMENT_BYTES];	// To see this NPC, these requirements must be met
	unsigned char npc2_require_number;
	unsigned char npc2_eval_type;						// EMPTY, AND, OR, etc.
	unsigned short npc2_text;							// ID of text shown when talking to this NPC
	unsigned char npc2_text_unique_id;					// Unique id of this conversation
	
	unsigned char has_npc3;
	unsigned char npc3;									// ID of NPC
	unsigned char npc3_require[MAX_REQUIREMENTS * REQUIREMENT_BYTES];	// To see this NPC, these requirements must be met
	unsigned char npc3_require_number;
	unsigned char npc3_eval_type;						// EMPTY, AND, OR, etc.
	unsigned short npc3_text;							// ID of text shown when talking to this NPC
	unsigned char npc3_text_unique_id;					// Unique id of this conversation
	
	unsigned char selected_npc;
	
} LevelState_t;

// =====================================================
// *ALL* platforms must implement the following methods
// =====================================================

// ============================================
// Platform specific game function implementations
// ============================================

// Sinclair QL 16bit m68008
#ifdef TARGET_QL
#include "../src/game_ql.h"
#endif

// Commodore PET 8bit 6502
#ifdef TARGET_PET
#include "../src/game_pet.h"
#endif

// IBM PC 8/16/32bit VGA
//#ifdef TARGET_PC_VGA
//#include "../src/game_vga.h"
//#endif

// Atari ST 16bit m68000
//#ifdef TARGET_ATARI_ST
//#include "../src/game_st.h"
//#endif

// MSX2 8bit Z80
//#ifdef TARGET_MSX2
//#include "../src/game_msx2.h"
//#endif

#endif
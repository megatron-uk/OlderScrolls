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

#include "../common/monsters.h"

#define GAME_MODE_MAP		1		// General mode, reading text, with movement and talk options
#define GAME_MODE_COMBAT	2		// In combat
#define GAME_MODE_SHOP		3		// In shop
#define GAME_MODE_EXIT		99		// Exit from game

#define MAX_PLAYER_NAME 	18
#define MAX_SHORT_NAME 		6
#define MAX_REWARD_ITEMS 	6		// Number of items that may be rewarded upon visiting a location, or upon defeat of primary monster(s)
#define MAX_ITEMS 			32		// The size of player inventory
#define MAX_LEVEL_NAME_SIZE 32		// How long a level name can be
#define MAX_STORY_TEXT_SIZE 1024 	// The buffer which holds the text to be shown on screen about a location
#define MAX_LOCATIONS 		256		// The maximum number of levels we can track - stories should not have more than this!
#define MAX_CHARACTERS		256		// Number of monsters, npcs or player characters (8 bit ID)
#define MAX_REQUIREMENTS 	8		// Actions can have prerequisites (or even multiple prerequisities) before they happen
#define MAX_MONSTER_TYPES 	4		// The number of different types of monsters in each location
#define MAX_EFFECTS			5		// maximum number of effects a spell or item can have
#define MAX_DAMAGE_TYPES	3
#define REQUIREMENT_BYTES 	5		// 5 bytes per requirement

#define FORMATION_FRONT		0x00
#define FORMATION_MID		0x01
#define FORMATION_REAR		0x02

// Structure representing the data associated with a single weapon
// This is everything we need to know in order to carry out combat with this weapon
// critical range, damage type
typedef struct {
	unsigned char item_id;
	unsigned char weapon_type;		// 1H, 2H, etc
	unsigned char weapon_class;		// Simple, martial, ranged, magical etc
	unsigned char weapon_size;		// small, medium, large, huge, etc
	unsigned char rarity;			// Common, uncommon, rare, legendary
	unsigned char name[MAX_PLAYER_NAME];			// Name of weapon, e.g. "Longsword"
	unsigned char crit_min;			// Minimum roll for critical, e.g 19
	unsigned char crit_max;			// Maximum roll for critical, e.g. 20
	unsigned char crit_multi;		// Number of rolls if critical, e.g. 2x
	unsigned char damage_types[MAX_DAMAGE_TYPES];	// Up to 3 damage types per weapon
	unsigned char dmg1_type;		// e.g. PHYSICAL
	unsigned char dmg1_min;			// minimum range of damage, e.g. 1
	unsigned char dmg1_max;			// minimum range of damage, e.g. 6
	unsigned char dmg1_rolls;		// number of rolls of this damage type, e.g. 1
	unsigned char dmg2_type;		// e.g. SLASHING
	unsigned char dmg2_min;			// minimum range of damage, e.g. 1
	unsigned char dmg2_max;			// minimum range of damage, e.g. 6
	unsigned char dmg2_rolls;		// number of rolls of this damage type, e.g. 1
	unsigned char dmg3_type;		// e.g. PIERCING
	unsigned char dmg3_min;			// minimum range of damage, e.g. 1
	unsigned char dmg3_max;			// minimum range of damage, e.g. 6
	unsigned char dmg3_rolls;		// number of rolls of this damage type, e.g. 1
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
	unsigned char class_limit;		// HUMAN_UNTRAINED, HUMAN_PALADIN, BEAST_MAGIC, etc
	unsigned char type;				// ITEM_TYPE_ARMOUR, ITEM_TYPE_CONSUMEABLE, etc
	unsigned char slot;				// SLOT_TYPE_BODY, SLOT_TYPE_NONE, etc
	unsigned short value;			// How much it costs, base value
	unsigned char ac;				// Armour class for armour items
	unsigned char ac_type;			// ARMOUR_TYPE_LIGHT, etc
	unsigned char effectlist[MAX_EFFECTS];	// List of effect ID's. Effects are what the item does on use/equip.
} ItemState_t;

// Structure representing the status of a single NPC or PC
typedef struct {
	char name[MAX_PLAYER_NAME];						// Full player character name, e.g. Argus the Dread
	char short_name[MAX_SHORT_NAME];					// Player character name, e.g. Argus
	
	unsigned char player_class;			// HUMAN_ROGUE, HUMAN_UNTRAINED, BEAST_MAGIC etc, see monsters.h
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
	unsigned char hp;					// Hit points
	unsigned long status;				// 32bit bitfield of status effects - see status.h
	
	// Equipped items
	unsigned char head;					// head
	unsigned char body;					// body
	unsigned char option;				// ring/pendant etc
	
	// Location in party when combat begins
	unsigned char formation;			// front/middle/rear
										// front gets a bonus to non-ranged attacks on the enemy front row
										// middle gets no bonuses or penalties
										// rear gets bonus to defence, but penalty to non-ranged attacks
	
	// Item store for this player
	unsigned short items[MAX_ITEMS];	// items
	
	// Individual player stats
	unsigned short kills;				// Record of how many kills this player has made so far
	unsigned short spells_cast;			// Record of how many spells this player has cast
	unsigned short hits_taken;			// Record of how much damage this player has taken so far
	unsigned short hits_caused;			// Record of how much damage this player has inflicted so far
	
	// Equipped weapons/weapon and shield
	WeaponState_t *weapon_r;			// Data for the weapon currently in the right hand
	WeaponState_t *weapon_l;			// Data for the weapon currently in the left hand		
	
	
	
} PlayerState_t;

// This structure is initialised each time we begin combat with 
// one or more enemy
typedef struct {
	unsigned char current;						// array entry of current enemy
	PlayerState_t enemies[MAX_MONSTER_TYPES];	// array of enemy characters
} EnemyState_t;

// A list of NPCs we encounter, the 
struct NPCList {
	unsigned char id;					// ID of NPC
	unsigned char talked_count;			// Number of times talked
	unsigned short talked_time;			// Last turn number we talked to them
	unsigned short death_time;			// Time of death / turn number
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
	unsigned char level_visits[MAX_LOCATIONS];					// Each level has a count of how many times it has been visited
	unsigned char level_defeated_primary[MAX_LOCATIONS];		// Each level has a flag to indicate whether the primary monster(s) has been defeated
	unsigned char level_defeated_secondary[MAX_LOCATIONS];		// Each level has a flag to indicate whether the secondary monster(s) has been defeated
	unsigned short counter;										// Game turn/timer/counter
	unsigned short gold;										// Record of currency
	PlayerState_t *p1;											// Pointer to player 1
	PlayerState_t *p2;											// Pointer to player 2
	PlayerState_t *p3;											// Pointer to player 3
	PlayerState_t *p4;											// Pointer to player 4
	struct NPCList *npcs;										// Pointer to a linked-list of NPC's we have met
	EnemyState_t *enemies;										// Pointer to the enemystate structure
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
	unsigned char npc1;									// ID of NPC
	unsigned char npc1_require[MAX_REQUIREMENTS * REQUIREMENT_BYTES];	// To see this NPC, these requirements must be met
	unsigned char npc1_require_number;
	unsigned char npc1_eval_type;						// EMPTY, AND, OR, etc.
	unsigned short npc1_text;							// ID of text shown when talking to this NPC
	
	unsigned char npc2;									// ID of NPC
	unsigned char npc2_require[MAX_REQUIREMENTS * REQUIREMENT_BYTES];	// To see this NPC, these requirements must be met
	unsigned char npc2_require_number;
	unsigned char npc2_eval_type;						// EMPTY, AND, OR, etc.
	unsigned short npc2_text;							// ID of text shown when talking to this NPC
	
} LevelState_t;

// =====================================================
// *ALL* platforms must implement the following methods
// =====================================================
void game_Init(GameState_t *gamestate, LevelState_t *levelstate); 	// Init game data
void game_Exit(); 														// De-init game data
void game_Splash(GameState_t *gamestate, LevelState_t *levelstate); 	// Show a splash screen on game start
void game_Map(GameState_t *gamestate, LevelState_t *levelstate); 
void game_Combat(GameState_t *gamestate, LevelState_t *levelstate);
void game_Quit(GameState_t *gamestate, LevelState_t *levelstate);

unsigned char game_CheckMovement(GameState_t *gamestate, LevelState_t *levelstate, unsigned char add_inputs, unsigned char add_text);
unsigned char game_CheckMonsterSpawn(GameState_t *gamestate, LevelState_t *levelstate, unsigned char add_inputs, unsigned char add_text);

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
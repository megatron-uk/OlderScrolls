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

#define MAX_REWARD_ITEMS 6						// Number of items that may be rewarded upon visiting a location, or upon defeat of primary monster(s)
#define MAX_ITEMS 32							// The size of player inventory
#define MAX_LEVEL_NAME_SIZE 32					// How long a level name can be
#define MAX_STORY_TEXT_SIZE 512					// The buffer which holds the text to be shown on screen about a location
#define MAX_LOCATIONS 256						// The maximum number of levels we can track - stories should not have more than this!
#define MAX_REQUIREMENTS 8						// Actions can have prerequisites (or even multiple prerequisities) before they happen
#define MAX_MONSTER_TYPES 8						// The number of different types of monsters in each location
#define REQUIREMENT_BYTES 5						// 5 bytes per requirement

//typedef struct {
//	unsigned short size;						// How long the string is
//	unsigned char text[MAX_STORY_TEXT_SIZE];	// maximum amount of text that can be printed in the main screen
//												// Note that this is an array of PETSCII codes, not printable ASCII
//} TextData;

// Basic game data
struct GameState_t {
	unsigned char level;						// ID of the current location
	unsigned char level_previous;				// ID of the immediately previous location
	unsigned char level_visits[MAX_LOCATIONS];	// Each level has a count of how many times it has been visited
	unsigned char level_defeated_primary[MAX_LOCATIONS];// Each level has a flag to indicate whether the primary monster(s) has been defeated
	unsigned char level_defeated_secondary[MAX_LOCATIONS];// Each level has a flag to indicate whether the secondary monster(s) has been defeated
	unsigned short gold;						// Record of currency
	
};
extern struct GameState_t gamestate;

// Each level that we visit is loaded from disk into this structure
// This way we can have (effectively) unlimited number of 'rooms' in 
// our adventure and only need to load one at a time.
//
// This takes up approximately 500 bytes.
struct LevelState_t {
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
	
};
extern struct LevelState_t levelstate;

// Structure representing the data associated with a single weapon
// This is everything we need to know in order to carry out combat with this weapon
// critical range, damage type
typedef struct {
	unsigned char item_id;
	unsigned char weapon_type;		// 1H, 2H, etc
	unsigned char weapon_class;		// Simple, martial, ranged, magical etc
	unsigned char weapon_size;		// small, medium, large, huge, etc
	unsigned char rarity;			// Common, uncommon, rare, legendary
	unsigned char name[18];			// Name of weapon, e.g. "Longsword"
	unsigned char crit_min;			// Minimum roll for critical, e.g 19
	unsigned char crit_max;			// Maximum roll for critical, e.g. 20
	unsigned char crit_multi;		// Number of rolls if critical, e.g. 2x
	unsigned char damage_types[3];	// Up to 3 damage types per weapon
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
} WeaponState;

// Data for a single spell
typedef struct {
	unsigned char spell_id;
} SpellState;

// Structure representing the status of a single NPC or PC
typedef struct {
	char name[32];						// Full player character name, e.g. Argus the Dread
	char short_name[6];					// Player character name, e.g. Argus
	
	// Core stats
	unsigned char str;				// 0-20
	unsigned char dex;				// 0-20
	unsigned char con;				// 0-20
	unsigned char wis;				// 0-20
	unsigned char intl;				// 0-20
	unsigned char chr;				// 0-20
	
	// Hitpoints and status effects
	unsigned short hp;				// Hit points
	unsigned long status;				// 32bit bitfield of status effects - see status.h
	
	// Equipped items
	unsigned char head;					// head
	unsigned char neck;					// neck/pendant
	unsigned char cloak;				// cloak
	unsigned char body;					// body
	unsigned char arms;					// arms
	unsigned char legs;					// legs
	unsigned char feet;					// feet
	unsigned char hand_r;				// right hand
	unsigned char hand_l;				// left hand
	
	// Location in party when combat begins
	unsigned char formation;			// front/middle/rear
										// front gets a bonus to non-ranged attacks on the enemy front row
										// middle gets no bonuses or penalties
										// rear gets bonus to defence, but penalty to non-ranged attacks
	
	// Item store for this player
	unsigned char items[MAX_ITEMS];		// items
	
	// Individual player stats
	unsigned short kills;				// Record of how many kills this player has made so far
	unsigned short spells_cast;			// Record of how many spells this player has cast
	unsigned long damage_taken;			// Record of how much damage this player has taken so far
	unsigned long damage_caused;		// Record of how much damage this player has inflicted so far
	
	WeaponState weapon_right;			// Data for the weapon currently in the right hand
	WeaponState weapon_left;			// Data for the weapon currently in the left hand
	
} PlayerState;

// Holds a small number of recently loaded weapon items so that they
// can be loaded from ram, and not from disk
typedef struct {
	WeaponState wep1;					// Data structure for a weapon
	unsigned char wep1_lasthit;			// Counter of the number of searches since this was last used
	WeaponState wep2;
	unsigned char wep2_lasthit;
	WeaponState wep3;
	unsigned char wep3_lasthit;
	WeaponState wep4;
	unsigned char wep4_lasthit;
	WeaponState wep5;
	unsigned char wep5_lasthit;
	WeaponState wep6;
	unsigned char wep6_lasthit;
} WeaponStateCache;

// Holds a small number of recently loaded armour items so that they
// can be loaded from ram, and not from disk
typedef struct {
	unsigned char armour1_lasthit;		// Count of the number of searches since this was last hit
} ArmourStateCache;

// =====================================================
// *ALL* platforms must implement the following methods
// =====================================================
void game_Init(); 		// Init game data
void game_Exit(); 		// De-init game data
void game_Splash(); 	// Show a splash screen on game start

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
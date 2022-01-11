// ==================================
//
// Game data definitions
//
// ==================================

#define MAX_REWARD_ITEMS 6						// Number of items that may be rewarded upon visiting a location, or upon defeat of primary monster(s)
#define MAX_ITEMS 32							// The size of player inventory
#define MAX_LEVEL_NAME_SIZE 38					// How long a level name can be
#define MAX_STORY_TEXT_SIZE 512					// The buffer which holds the text to be shown on screen about a location
#define MAX_LOCATIONS 99						// The maximum number of levels we can track - stories should not have more than this!
#define MAX_REQUIREMENTS 16						// Actions can have prerequisites (or even multiple prerequisities) before they happen
#define MAX_MONSTER_TYPES 8						// The number of different types of monsters in each location

//typedef struct {
//	unsigned short size;						// How long the string is
//	unsigned char text[MAX_STORY_TEXT_SIZE];	// maximum amount of text that can be printed in the main screen
//												// Note that this is an array of PETSCII codes, not printable ASCII
//} TextData;

// Basic game data
typedef struct {
	unsigned char level;						// ID of the current location
	unsigned char level_previous;				// ID of the immediately previous location
	unsigned char level_visits[MAX_LOCATIONS];	// Each level has a count of how many times it has been visited
	unsigned char level_defeated[MAX_LOCATIONS];// Each level has a flag to indicate whether the primary monster(s) has been defeated
	unsigned short gold;						// Record of currency
} GameState;

// Each level that we visit is loaded from disk into this structure
// This way we can have (effectively) unlimited number of 'rooms' in 
// our adventure and only need to load one at a time.
typedef struct {
	unsigned char level;						// Unique ID of the location
	char name[MAX_LEVEL_NAME_SIZE];				// Name of the location
	char text[MAX_STORY_TEXT_SIZE];				// The *current* story text for this level
	
	// Navigation options
	unsigned char north;						// ID of the location to the north
	unsigned char south;						// ID of the location to the south
	unsigned char east;							// ID of the location to the east
	unsigned char west;							// ID of the location to the west
	unsigned char north_require[MAX_REQUIREMENTS];	// Requirements to exit north
	unsigned char south_require[MAX_REQUIREMENTS];	// Requirements to exit north
	unsigned char east_require[MAX_REQUIREMENTS];	// Requirements to exit north
	unsigned char west_require[MAX_REQUIREMENTS];	// Requirements to exit north
	
	// Primary set of monsters for this location
	unsigned char spawn;							// 0-100 Chance of monsters spawning on initial location load
	unsigned char spawn_list[MAX_MONSTER_TYPES];	// A list of the possible monster ID's for this location
	unsigned char spawn_require[MAX_REQUIREMENTS];	// Things which need to be true to spawn monsters
	
	// Monsters that may respawn in this location
	unsigned char respawn;							// 0-100 Chance of monsters respawning on subsequent location load
	unsigned char respawn_list[MAX_MONSTER_TYPES];	// A list of the possible monster ID's for this location
	unsigned char respawn_require[MAX_REQUIREMENTS];// Things which need to be true to respawn monsters
	
	// Items may appear after defeating monsters
	unsigned char items_list[MAX_REWARD_ITEMS];		// List of items rewarded upon visiting location, or upon defeating primary monsters, if present
	unsigned char items_chance[MAX_REWARD_ITEMS];	// Each item has a chance of being present
	unsigned char items_require[MAX_REQUIREMENTS];	// To receive the items, these requirements must be met

} LevelState;

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
	unsigned char strength;				// 0-20
	unsigned char dexterity;			// 0-20
	unsigned char constitution;			// 0-20
	unsigned char wisdom;				// 0-20
	unsigned char intelligence;			// 0-20
	unsigned char charisma;				// 0-20
	
	// Hitpoints and status effects
	unsigned short health;				// Hit points
	unsigned long status;				// 32bit bitfield of status effects - see status.h
	
	// Equipped items
	unsigned char head;					// head
	unsigned char neck;					// neck/pendant
	unsigned char cloak;				// cloak
	unsigned char body;					// body
	unsigned char arms;					// arms
	unsigned char legs;					// legs
	unsigned char feet;					// feet
	unsigned char hand_right;			// right hand
	unsigned char hand_left;			// left hand
	
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

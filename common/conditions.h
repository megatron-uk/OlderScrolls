/* conditions.h, Rule types which may be enforced on options at a location.
 
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

// How multiple conditions are evaluated
#define COND_EVAL_EMPTY		0x00	// The condition set is EMPTY, do not attempt to evaluate
#define COND_EVAL_AND		0x10	// All conditions must evaluate to true. The default.
#define COND_EVAL_OR		0x20	// At least one condition must evaluate to true.
#define COND_EVAL_NOR		0x30	// All conditions must evaluate to false.
#define COND_EVAL_NAND		0x40	// Zero or one, (but no more than one) condition must evaluate to true.

// Every condition is 5 bytes long
#define COND_LENGTH			5

// These byte values are used to refer to the statistics of a PC or the Player Party
#define COND_TYPE_STR		0x01
#define COND_TYPE_DEX		0x02
#define COND_TYPE_CON		0x03
#define COND_TYPE_WIS		0x04
#define COND_TYPE_INT		0x06
#define COND_TYPE_CHR		0x07
#define COND_TYPE_HP		0x08
#define COND_TYPE_GOLD		0x09
#define COND_TYPE_DMG		0x0A
#define COND_TYPE_INJURY	0x0B

// Status of party members
#define COND_PARTY_PRESENT	0x01	// Present + alive
#define COND_PARTY_DIMISSED	0x02	// Was dismissed
#define COND_PARTY_DEAD		0x03	// Dead

// ==========================================
// Basic conditions
// 		2 bytes for the condition type
//		3 pad bytes
// e.g. 0x01 00 00 00 00 

#define NO_COND					0x0000 // No conditions
#define COND_NO_MONSTERS		0x0001 // No monsters must be currently spawned at this location

// ==========================================
// Tests for the player 
// 		2 bytes for the condition type (0x0100)
//		1 byte for the tested statistic (0x01 == STR)
//		1 byte for the tested value (0x0A = 10)
//		1 pad byte
// e.g 0x01 00 01 0A 00 // Player has STR 10 or above

#define COND_PC_ATR_TYPE			0x01

// ==========================================
// Test for anyone in the player party
// 		2 bytes for the condition type (0x0200)
//		1 byte for the tested statistic (0x06 == INT)
//		1 byte for the tested value (0x0C = 12)
//		1 pad byte
// e.g 0x02 00 06 0C 00 // Someone in party has INT 12 or above

#define COND_PARTY_ATR_TYPE			0x02

// ==========================================
// Party Membership
//		2 bytes for the condition type
//		1 byte for the id of the playable character
//		1 byte for status of the playable character
//		1 pad byte
// e.g. 0x03 00 02 01 00 // Party member 02 is present and alive
// e.g. 0x03 00 02 03 00 // Party member 02 is present, but dead

#define COND_PARTY_MEMBER_TYPE		0x03

// ==========================================
// Locations
//		1 bytes for the condition type
//		1 byte for the test type (visited, min, max)
//		2 bytes for the location ID
//		1 byte for the number (or pad)
// e.g. 0x04 00 00 01 00 // Location 1 has been visited
// e.g. 0x04 01 00 07 01 // Location 7 has been visited NO MORE THAN once
// e.g. 0x04 02 00 07 09 // Location 7 has been visited MORE THAN 9 times

#define COND_MAP_VISIT_TYPE 	0x04
#define COND_MAP_VISIT_MAX		0x01 // A map location requires to have visited a maximum number of times
#define COND_MAP_VISIT_MIN		0x02 // A map location requires to have visited a minimum number of times

// ==========================================
// Monster spawning tests
//		1 byte for the condition type
//		1 byte for the test type (primary, secondary)
//		2 bytes for the location ID of the monsters we are checking
//		1 byte for the number
// e.g. 0x05 01 00 5A 01 // Primary monster at location 90 must have been defeated at least once
// e.g. 0x05 02 00 0F 03 // Secondary monsters at location 16 must have been defeated at least 3 times

#define COND_MONSTER_DEFEAT_TYPE	0x05
#define COND_MONSTER_PRI_DEFEATED	0x01	// Primary monster at a given map location must have been defeated 'x' times
#define COND_MONSTER_SEC_DEFEATED	0x02	// Secondary monster at a given map location must have been defeated 'x' times

// ==========================================
// NPC spawning/talking rules
//		2 byte1 for the condition type
//		1 bytes for the NPC ID the test is for
//		1 byte for the number
//		1 pad byte
// e.g. 0x06 01 1E 03 00 // Must have talked to NPC 30, 3 times
// e.g. 0x06 01 05 01 00 // Must have talked to NPC 5, 1 times
#define COND_NPC_TYPE		0x06
#define COND_NPC_TALK		0x01 // Must have talked to NPC 'id' 'x' times

// ==========================================
// Items
//		2 bytes for the condition type
//		1 byte for the item ID the test is for
//		1 byte for the number
//		1 pad byte
// e.g. 0x07 01 FE 01 00 // Must posess (at least 1 of) item ID 254.
// e.g. 0x07 02 0D 07 00 // Must not have more than 7 copies of item ID 13.

#define COND_ITEM_TYPE		0x07
#define COND_ITEM_OWN		0x01 // Must posess the item 'id'
#define COND_ITEM_NOTOWN	0x02 // Must NOT posess the item 'id'

// ==========================================
// Weapons
//		2 bytes for the condition type
//		1 byte for the weapon ID the test is for
//		1 byte for the number
//		1 pad byte
// e.g. 0x08 01 01 01 00 // Must posess (at least 1 of) weapon ID 1.
// e.g. 0x08 02 05 01 00 // Must not have more than 1 of weapon ID 5.

#define COND_WEAPON_TYPE	0x08
#define COND_WEAPON_OWN		0x01 // Must posess the weapon 'id'
#define COND_WEAPON_NOTOWN	0x02 // Must NOT possess the weapon 'id'

// ===========================================
// Common functions which all targets must
// implement.
// ===========================================

unsigned char check_Cond(GameState_t *gamestate, LevelState_t *levelstate, unsigned char *requires, unsigned char number, unsigned char eval_type);
unsigned char check_NoCond(GameState_t *gamestate, LevelState_t *levelstate, char *cond);
unsigned char check_PlayerAttribute(GameState_t *gamestate, LevelState_t *levelstate, char *cond);
unsigned char check_PartyAttribute(GameState_t *gamestate, LevelState_t *levelstate, char *cond);
unsigned char check_PartyState(GameState_t *gamestate, LevelState_t *levelstate, char *cond);
unsigned char check_Map(GameState_t *gamestate, LevelState_t *levelstate, char *cond);
unsigned char check_Monster(GameState_t *gamestate, LevelState_t *levelstate, char *cond);
unsigned char check_NPC(GameState_t *gamestate, LevelState_t *levelstate, char *cond);
unsigned char check_Item(GameState_t *gamestate, LevelState_t *levelstate, char *cond);
unsigned char check_Weapon(GameState_t *gamestate, LevelState_t *levelstate, char *cond);
/* engine.h, Prototypes and defines for core game mechanics. Somewhat 
 mostly based on the D&D 5th edition style mechanics, but simplified
 where possible to make it feasible to run on small 8 and 16bit micros.
 
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

#ifndef _ENGINE_H
#define _ENGINE_H

#ifndef _GAME_H
#include "../common/game.h"
#endif

// ===================================================================
// Defaults that are common to all game mechanics on all platforms
// ===================================================================

#define DIFFICULTY_CLASS_EASY		10
#define DIFFICULTY_CLASS_MEDIUM		15
#define DIFFICULTY_CLASS_HARD		20

#define ATTACK_DICE_TYPE		20
#define ATTACK_DICE_QTY			1

#define ABILITY_MODIFIER_MIN -5		// 5th edition maximum negative ability modifier 
#define ABILITY_MODIFIER_MAX 10		// 5th edition maximum positive ability modifier
#define PROFICIENCY_BONUS_MIN 2		// Minimum amount that a skill you are proficienct in can modify a roll
#define PROFICIENCY_BONUS_MAX 6		// Maximum amount that a skill you are proficienct in can modify a roll

// Combat stances
#define FORMATION_FRONT		0x00
#define FORMATION_MID		0x01
#define FORMATION_REAR		0x02

// We can have up to 32 simultaneous status effects
#define STATUS_OK 						0x00000000
// Negative effects from 0x00000001 - 0x00010000
#define STATUS_BLINDED 					0x00000001
#define STATUS_FRIGHTENED				0x00000002
#define STATUS_INCAPACITATED			0x00000004
#define STATUS_INVISIBLE				0x00000008
#define STATUS_PARALYZED				0x00000010
#define STATUS_POISONED					0x00000020
#define STATUS_PRONE					0x00000040
#define STATUS_STUNNED					0x00000080
#define STATUS_UNCONSCIOUS				0x00000100
#define STATUS_EXHAUSTED				0x00000200
#define STATUS_BLEEDING					0x00000400
#define STATUS_BURNING					0x00000800
#define STATUS_SILENCED					0x00001000
#define STATUS_WEAKENED					0x00002000
#define STATUS_SLOWED					0x00004000
#define STATUS_CURSED					0x00008000
// Positive effects from 0x00010000 - 0x80000000
#define STATUS_POWERFUL					0x00010000
#define STATUS_BLESSED					0x00020000
#define STATUS_PRECISE					0x00040000
#define STATUS_FRENZY					0x00080000
#define STATUS_UNSTOPPABLE				0x00100000
#define STATUS_REGENERATION				0x00200000
#define STATUS_ELEMENTAL_RESISTANCE	0x00400000
#define STATUS_FIRE_RESISTANCE			0x00800000
#define STATUS_STALWART					0x01000000
#define STATUS_COLD_RESISTANCE			0x02000000
#define STATUS_POISON_RESISTANCE	 	0x04000000
#define STATUS_ACID_RESISTANCE			0x08000000
#define STATUS_THORNS					0x10000000
#define STATUS_LIGHTNING_RESISTANCE	0x20000000
#define STATUS_MAGICAL_RESISTANCE		0x40000000
#define STATUS_PHYSICAL_RESISTANCE		0x80000000

// Proficiency types
#define PROFICIENCY_NONE				0
#define PROFICIENCY_ARMOUR_L			1
#define PROFICIENCY_ARMOUR_M			2
#define PROFICIENCY_ARMOUR_H			3
#define PROFICIENCY_SHIELD				4
#define PROFICIENCY_WEAPON_SIMPLE		5
#define PROFICIENCY_WEAPON_MARTIAL		6
#define PROFICIENCY_WEAPON_CROSSBOW_L	7
#define PROFICIENCY_WEAPON_CROSSBOW_H	8
#define PROFICIENCY_WEAPON_SWORD_S		9
#define PROFICIENCY_WEAPON_SWORD_L		10
#define PROFICIENCY_WEAPON_SCIMITAR	11
#define PROFICIENCY_WEAPON_RAPIER		12
#define PROFICIENCY_WEAPON_DAGGER		13
#define PROFICIENCY_WEAPON_MACE			14
#define PROFICIENCY_WEAPON_STAFF		15
#define PROFICIENCY_WEAPON_RANGED		16
#define PROFICIENCY_WEAPON_MAGIC		17
#define PROFICIENCY_SAVE_STRENGTH		18
#define PROFICIENCY_SAVE_CONSTITUTION	19
#define PROFICIENCY_SAVE_DEXTERITY		20
#define PROFICIENCY_SAVE_CHARISMA		21
#define PROFICIENCY_SAVE_WISDOM			22
#define PROFICIENCY_SAVE_INTELLIGENCE	23


// Player character and monster classes
#define CLASS_UNTRAINED				0
#define CLASS_GENERIC_MELEE			1
#define CLASS_GENERIC_RANGED		2
#define CLASS_GENERIC_MAGIC			3
#define CLASS_BARBARIAN				4				
#define CLASS_BARD					5
#define CLASS_CLERIC				6
#define CLASS_DRUID					7
#define CLASS_FIGHTER				8
#define CLASS_PALADIN				9
#define CLASS_RANGER				10
#define CLASS_ROGUE					11
#define CLASS_SORCERER				12
#define CLASS_WARLOCK				13
#define CLASS_WIZARD				14

// Player character and monster races
#define RACE_UNKNOWN				0
#define RACE_HUMAN					1
#define RACE_BEAST					2
#define RACE_ORC					3
#define RACE_ELF					4

// Weapon damage types
#define WEAPON_DMG_NONE				0
#define WEAPON_DMG_PHYSICAL			1
#define WEAPON_DMG_SLASHING			2
#define WEAPON_DMG_PIERCING			3
#define WEAPON_DMG_BLUNT			4
#define WEAPON_DMG_LIGHTNING		5
#define WEAPON_DMG_ACID				6
#define WEAPON_DMG_FIRE				7
#define WEAPON_DMG_COLD				8
#define WEAPON_DMG_POISON			9

// Weapon sizes
#define WEAPON_SIZE_SMALL			0
#define WEAPON_SIZE_MEDIUM			1
#define WEAPON_SIZE_LARGE			2
#define WEAPON_SIZE_HUGE			3

// Weapon rarity (e.g. availability from vendors)
#define WEAPON_RARITY_COMMON		0
#define WEAPON_RARITY_UNCOMMON		1
#define WEAPON_RARITY_RARE			2
#define WEAPON_RARITY_LEGENDARY		3

// Weapon classes
#define WEAPON_CLASS_SIMPLE			0
#define WEAPON_CLASS_MARTIAL		1
#define WEAPON_CLASS_RANGED			2
#define WEAPON_CLASS_MAGICAL		3

// Weapon grip
#define WEAPON_1HANDED				0
#define WEAPON_2HANDED				1
#define RIGHT_HAND					2
#define LEFT_HAND					3

// Item types
#define ITEM_TYPE_CONSUMEABLE		0
#define ITEM_TYPE_ARMOUR			1
#define ITEM_TYPE_SPELL				2
#define ITEM_TYPE_GENERIC			3
#define ITEM_TYPE_QUEST				4

// Armour types
#define ARMOUR_TYPE_NONE			0
#define ARMOUR_TYPE_LIGHT			1
#define ARMOUR_TYPE_MEDIUM			2
#define ARMOUR_TYPE_HEAVY			3

#define ITEM_SLOT_NONE				0
#define ITEM_SLOT_HEAD				1
#define ITEM_SLOT_BODY				2
#define ITEM_SLOT_OPTION			3

/*
	An effect can be caused by an item, spell or action
	
	- Has a name "Fireball"
	- Has one or more destinations
		- enum ( self, self party, enemy, enemy party)
	- Has (one or more) type(s) of effect
		- hp +20
		- apply bleeding
		- apply poisoned
		- bless
		- smite
		- cause fire damage
		- cause acid damage
		- etc.
	- Has an animation
		- fireball

*/

extern const char *player_classes[];
extern const char *player_races[];
extern const unsigned char player_class_proficiencies[][MAX_PROFICIENCIES];
extern const char *proficiencies[];
extern const char *status_effects[];


// ===================================================================
// Prototypes that are core to all game mechanics on all platforms
// ===================================================================

// Calculate the damage roll of a player equipped weapon
unsigned char player_weapon_DamageModifier(PlayerState_t *pc, WeaponState_t *weapon, unsigned char dmg_type, unsigned char *dmg_dice_type, unsigned char *dmg_dice_qty, unsigned char weapon_bonus, unsigned char versatile_bonus, unsigned char silvered_bonus, unsigned char status_bonus, unsigned char status_penalty);

// Calculate the attack roll of a player equipped weapon
unsigned char player_weapon_AttackModifier(PlayerState_t *pc, WeaponState_t *weapon, unsigned char proficiency_bonus, unsigned char weapon_bonus, unsigned char ability_bonus, unsigned char status_bonus, unsigned char status_penalty);

// Calculate the modifier for an ability
char ability_Modifier(unsigned char ability);

// Return the proficiency modifier (or 0) for a given skill
unsigned char is_proficient(PlayerState_t *pc, unsigned char proficiency_type);

// Set the hit dice type and quantity for a given player character
void hit_Dice(PlayerState_t *pc, unsigned char *dice_quantity, unsigned char *dice_type, char *constitution_modifier);

// Give x1 of an item to a player character
void pc_GiveItem(PlayerState_t *pc, WeaponState_t *weapon, ItemState_t *item);

// Take x1 of an item from a player character
void pc_TakeItem(PlayerState_t *pc, WeaponState_t *weapon, ItemState_t *item);

// Check if a player character has any free slots
char pc_HasSlots(PlayerState_t *pc, WeaponState_t *weapon, ItemState_t *item);

// Check if a player character can equip a given item/weapon
char pc_CanEquip(PlayerState_t *pc, WeaponState_t *weapon, ItemState_t *item);

// Equip an item/weapon
void pc_Equip(PlayerState_t *pc, WeaponState_t *weapon, ItemState_t *item, unsigned char hand);

// Is an item equipped
char pc_IsEquipped(PlayerState_t *pc, WeaponState_t *weapon, ItemState_t *item);

// Is it the last instance of an item in the inventory?
char pc_IsLastItem(PlayerState_t *pc, WeaponState_t *weapon, ItemState_t *item);

// How many characters are in the party at present
unsigned char party_Count(GameState_t *gamestate, unsigned char include_dead);

// Generate a random number
unsigned char random_GenerateRandom(GameState_t *gamestate);

// Generate a random dice roll between dice_from and dice_to
unsigned char random_GenerateRandomDice(GameState_t *gamestate, unsigned char dice_from, unsigned char dice_to);

#endif
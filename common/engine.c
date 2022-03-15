/* engine.c, Core rule mechanics. Somewhat 
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

#include <string.h>

#include "../common/engine.h"

#ifndef _MONSTER_H
#define _MONSTER_H
#include "../common/monsters.h"
#endif

extern const char *player_races[MAX_PLAYER_RACES] = {
	"Unknown",			// #0
	"Human",			// #1
	"Beast",			// #2
	"Orc",				// #3
	"Elf",				// #4
};

extern const char *player_classes[MAX_PLAYER_CLASSES] = { 
	"Untrained",		// #0, 
	"Generic, melee",	// #1, 
	"Generic, ranged",	// #2, 
	"Generic, magic",	// #3, 
	"Barbarian",		// #4, 
	"Bard",				// #5, 
	"Cleric",			// #6, 
	"Druid",			// #7, 
	"Fighter",			// #8, 
	"Paladin",			// #9, 
	"Ranger",			// #10, 
	"Rogue",			// #11, 
	"Sorceror",			// #12, 
	"Warlock",			// #13, 
	"Wizard",			// #14, 
	"Unknown",			// #15, 
};

extern const char *proficiencies[24] = {
	"None",					// #0
	"Armour (Light)",		// #1
	"Armour (Medium)",		// #2
	"Armour (Heavy)",		// #3
	"Shields",				// #4
	"Weapons (Simple)",		// #5
	"Weapons (Martial)",	// #6
	"Crossbow (Light)",		// #7
	"Crossbow (Heavy)",		// #8
	"Sword (Short)",		// #9
	"Longsword",			// #10
	"Scimitar",				// #11
	"Rapier",				// #12
	"Dagger",				// #13
	"Mace",					// #14
	"Staff",				// #15
	"Weapons (Ranged)",		// #16
	"Weapons (Magical)",	// #17
	"Saving Throw (Str.)",	// #18
	"Saving Throw (Con.)",	// #19
	"Saving Throw (Dex.)",	// #20
	"Saving Throw (Chr.)",	// #21
	"Saving Throw (Wis.)",	// #22
	"Saving Throw (Intl.)",	// #23
};

extern const unsigned char player_class_proficiencies[MAX_PLAYER_CLASSES][MAX_PROFICIENCIES] = {
	// #0, Untrained
	{PROFICIENCY_WEAPON_SIMPLE, PROFICIENCY_SAVE_CONSTITUTION, PROFICIENCY_NONE, PROFICIENCY_NONE, 
	PROFICIENCY_NONE, PROFICIENCY_NONE, PROFICIENCY_NONE, PROFICIENCY_NONE, 
	PROFICIENCY_NONE, PROFICIENCY_NONE},
	
	// #1 Generic, melee
	{PROFICIENCY_WEAPON_SIMPLE, PROFICIENCY_ARMOUR_L, PROFICIENCY_SAVE_CONSTITUTION, PROFICIENCY_NONE, 
	PROFICIENCY_NONE, PROFICIENCY_NONE, PROFICIENCY_NONE, PROFICIENCY_NONE, 
	PROFICIENCY_NONE, PROFICIENCY_NONE},
	
	// #2 Generic, ranged
	{PROFICIENCY_WEAPON_SIMPLE, PROFICIENCY_WEAPON_CROSSBOW_L, PROFICIENCY_SAVE_CONSTITUTION, PROFICIENCY_ARMOUR_L, 
	PROFICIENCY_NONE, PROFICIENCY_NONE, PROFICIENCY_NONE, PROFICIENCY_NONE, 
	PROFICIENCY_NONE, PROFICIENCY_NONE},
	
	// #3 Generic, magic
	{PROFICIENCY_WEAPON_SIMPLE, PROFICIENCY_WEAPON_STAFF, PROFICIENCY_SAVE_CONSTITUTION, PROFICIENCY_NONE, 
	PROFICIENCY_NONE, PROFICIENCY_NONE, PROFICIENCY_NONE, PROFICIENCY_NONE, 
	PROFICIENCY_NONE, PROFICIENCY_NONE},
	
	// #4 Barbarian
	{PROFICIENCY_ARMOUR_L, PROFICIENCY_ARMOUR_M, PROFICIENCY_SHIELD, PROFICIENCY_WEAPON_SIMPLE, 
	PROFICIENCY_WEAPON_MARTIAL, PROFICIENCY_SAVE_STRENGTH, PROFICIENCY_SAVE_CONSTITUTION, PROFICIENCY_NONE, 
	PROFICIENCY_NONE, PROFICIENCY_NONE},
	
	// #5 Bard
	{PROFICIENCY_ARMOUR_L, PROFICIENCY_WEAPON_SIMPLE, PROFICIENCY_WEAPON_CROSSBOW_L, PROFICIENCY_WEAPON_SWORD_S,
	PROFICIENCY_WEAPON_SWORD_L, PROFICIENCY_WEAPON_RAPIER, PROFICIENCY_SAVE_DEXTERITY, PROFICIENCY_SAVE_CHARISMA,
	PROFICIENCY_NONE, PROFICIENCY_NONE},
	
	// #6 Cleric
	{PROFICIENCY_ARMOUR_L, PROFICIENCY_ARMOUR_M, PROFICIENCY_SHIELD, PROFICIENCY_WEAPON_SIMPLE,
	PROFICIENCY_SAVE_CHARISMA, PROFICIENCY_SAVE_WISDOM, PROFICIENCY_NONE, PROFICIENCY_NONE,
	PROFICIENCY_NONE, PROFICIENCY_NONE},
	
	// #7 Druid
	{PROFICIENCY_ARMOUR_L, PROFICIENCY_ARMOUR_M, PROFICIENCY_SHIELD, PROFICIENCY_WEAPON_DAGGER,
	PROFICIENCY_WEAPON_MACE, PROFICIENCY_WEAPON_STAFF, PROFICIENCY_WEAPON_SCIMITAR, PROFICIENCY_SAVE_WISDOM,
	PROFICIENCY_SAVE_INTELLIGENCE, PROFICIENCY_NONE},
	
	// #8 Fighter
	{PROFICIENCY_ARMOUR_L, PROFICIENCY_ARMOUR_M, PROFICIENCY_ARMOUR_H, PROFICIENCY_SHIELD,
	PROFICIENCY_WEAPON_SIMPLE, PROFICIENCY_WEAPON_MARTIAL, PROFICIENCY_SAVE_STRENGTH, PROFICIENCY_SAVE_CONSTITUTION,
	PROFICIENCY_NONE, PROFICIENCY_NONE},
	
	// #9 Paladin
	{PROFICIENCY_ARMOUR_L, PROFICIENCY_ARMOUR_M, PROFICIENCY_ARMOUR_H, PROFICIENCY_SHIELD,
	PROFICIENCY_WEAPON_SIMPLE, PROFICIENCY_WEAPON_MARTIAL, PROFICIENCY_SAVE_CHARISMA, PROFICIENCY_SAVE_WISDOM,
	PROFICIENCY_NONE, PROFICIENCY_NONE},
	
	// #10 Ranger
	{PROFICIENCY_ARMOUR_L, PROFICIENCY_ARMOUR_M, PROFICIENCY_SHIELD, PROFICIENCY_WEAPON_SIMPLE, 
	PROFICIENCY_WEAPON_MARTIAL, PROFICIENCY_SAVE_STRENGTH, PROFICIENCY_SAVE_DEXTERITY, PROFICIENCY_WEAPON_RANGED, 
	PROFICIENCY_NONE, PROFICIENCY_NONE},
	
	// #11 ROgue
	{PROFICIENCY_ARMOUR_L, PROFICIENCY_WEAPON_SIMPLE, PROFICIENCY_WEAPON_CROSSBOW_L, PROFICIENCY_WEAPON_SWORD_S,
	PROFICIENCY_WEAPON_SWORD_L, PROFICIENCY_WEAPON_RAPIER, PROFICIENCY_SAVE_DEXTERITY, PROFICIENCY_SAVE_INTELLIGENCE,
	PROFICIENCY_WEAPON_RANGED, PROFICIENCY_NONE},
	
	// #12 Sorcerer
	{PROFICIENCY_WEAPON_DAGGER, PROFICIENCY_WEAPON_STAFF, PROFICIENCY_WEAPON_CROSSBOW_L, PROFICIENCY_SAVE_CHARISMA,
	PROFICIENCY_SAVE_CONSTITUTION, PROFICIENCY_WEAPON_MAGIC, PROFICIENCY_NONE, PROFICIENCY_NONE, 
	PROFICIENCY_NONE, PROFICIENCY_NONE},
	
	// #13 Warlock
	{PROFICIENCY_ARMOUR_L, PROFICIENCY_WEAPON_SIMPLE, PROFICIENCY_SAVE_WISDOM, PROFICIENCY_SAVE_CHARISMA,
	PROFICIENCY_WEAPON_MAGIC, PROFICIENCY_NONE, PROFICIENCY_NONE, PROFICIENCY_NONE,
	PROFICIENCY_NONE, PROFICIENCY_NONE},
	
	// #15 Wizard
	{PROFICIENCY_WEAPON_DAGGER, PROFICIENCY_WEAPON_STAFF, PROFICIENCY_WEAPON_CROSSBOW_L, PROFICIENCY_SAVE_INTELLIGENCE,
	PROFICIENCY_SAVE_WISDOM, PROFICIENCY_NONE, PROFICIENCY_NONE, PROFICIENCY_NONE,
	PROFICIENCY_NONE, PROFICIENCY_NONE},
		
};

extern const char *status_effects[32] = {
	"Blinded",
	"Frightened",
	"Incapacitated",
	"Invisible",
	"Paralyzed",
	"Poisoned",
	"Prone",
	"Stunned",
	"Unconscious",
	"Exhausted",
	"Bleeding",
	"Burning",
	"Silenced",
	"Weakened",
	"Slowed",
	"Cursed",
	"Powerful",
	"Blessed",
	"Precise",
	"Frenzy",
	"Unstoppable",
	"Regeneration",
	"Elemental Res.",
	"Fire Res.",
	"Stalwart",
	"Cold Res.",
	"Poison Res.",
	"Acid Res.",
	"Thorns",
	"Lightning Res.",
	"Magical Res.",
	"Physical Res.",
};

// ==================================================
//
// Functions to do with transferring items to/from
// player characters.
//
// ==================================================

void pc_GiveItem(PlayerState_t *pc, WeaponState_t *weapon, ItemState_t *item){
	// Send an item to a player
	char slot;
	
	// Check character is present
	if (pc->level){
		slot = pc_HasSlots(pc, weapon, item);
		if (slot >= 0){
			if (weapon->item_id){
				if ((pc->items[slot].item_type == ITEM_TYPE_WEAPON) && (pc->items[slot].item_id == weapon->item_id)){
					pc->items[slot].qty++;
				} else {
					pc->items[slot].qty = 1;
					pc->items[slot].item_type = ITEM_TYPE_WEAPON;
					pc->items[slot].item_id = weapon->item_id;
				}
			}
			if (item->item_id){
				if ((pc->items[slot].item_type == ITEM_TYPE_ITEM) && (pc->items[slot].item_id == item->item_id)){
					pc->items[slot].qty++;
				} else {
					pc->items[slot].qty = 1;
					pc->items[slot].item_type = ITEM_TYPE_ITEM;
					pc->items[slot].item_id = item->item_id;
				}
			}
		}
	}
	
}

void pc_TakeItem(PlayerState_t *pc, WeaponState_t *weapon, ItemState_t *item){
	// Remove one copy of an item from a player
	unsigned char i;
	
	// Find the item slot
	for (i = 0; i < MAX_ITEMS; i++){
		
		if (weapon->item_id){
			if (pc->items[i].item_id == weapon->item_id){
				// Remove one item
				if (pc->items[i].qty > 0){
					pc->items[i].qty--;
				}
			}
		}
		if (item->item_id){
			if (pc->items[i].item_id == item->item_id){
				// Remove one item
				if (pc->items[i].qty > 0){
					pc->items[i].qty--;
				}
			}
		}
		
		// If no items left, blank out the item slot
		if (pc->items[i].qty == 0){
			pc->items[i].item_id = 0;
			pc->items[i].item_type = ITEM_TYPE_NONE;
		}
	}
}

char pc_HasSlots(PlayerState_t *pc, WeaponState_t *weapon, ItemState_t *item){
	// CHeck if the player has a free slot.
	// If no free slots, also check if the player has an existing stock of an item/weapon
	// and it currently holds less than 255 of that item, then it can instead just be increased.
	//
	// Returns player inventory slot number, or -1 if no slots free
	
	unsigned char i;
	
	if (pc->level){
		// CHeck for an existing item of this type
		for (i = 0; i < MAX_ITEMS; i++){
			// If the slot is already taken up by the same item (and qty < 255) 
			// just send back the slot number
			if (weapon->item_id != 0){
				if ((pc->items[i].item_type == ITEM_TYPE_WEAPON) && (pc->items[i].item_id == weapon->item_id)){
					if (pc->items[i].qty < 255){
						// Already have this item, and not at maximum capacity
						return i;
					} else {
						// Quantity of this item already at max
						return -1;	
					}
				}	
			}
			if (item->item_id != 0){
				if ((pc->items[i].item_type == ITEM_TYPE_ITEM) && (pc->items[i].item_id == item->item_id)){
					if (pc->items[i].qty < 255){
						// Already have this item, and not at maximum capacity
						return i;
					} else {
						// Quantity of this item already at max
						return -1;	
					}
				}	
			}
		}
		
		// Check for a free/empty slot
		for (i = 0; i < MAX_ITEMS; i++){
			// If the slot is empty, just send back the slot number
			if ((pc->items[i].item_type) == ITEM_TYPE_NONE){
				return i;
			}
		}
	}
	
	// No slots free, or pc not present
	return -1;
}

// ==================================================
//
// Functions associated with equipping or unequipping
// weapons and items.
//
// ==================================================

char pc_CanEquip(PlayerState_t *pc, WeaponState_t *weapon, ItemState_t *item){
	// Check if the race and class of the player character allows them to equip
	// the given weapon or item.
	unsigned char can_equip = 1;
	
	// Items can go on head, body or option slots
	if (item->item_id != 0){
		
		// If the item has a race limitation of anything other than UNKNOWN/ALL
		// check if we satisfy it.
		if (item->race_limit != RACE_UNKNOWN){
			if (pc->player_race != item->race_limit){
				can_equip = 0;
			}
		}
		
		// If the item has a class limitation of anything other than UNKNOWN/ALL/UNTRAINED
		// check if we satisfy it.
		if (item->class_limit != CLASS_UNTRAINED){
			if (pc->player_class != item->class_limit){
				can_equip = 0;
			}
		}
		
		// All other armour is fair game, but will be less effective if the character
		// is not proficient in them... doesn't stop their use however.
		return can_equip;
	}
	
	// Weapons can go on weapon_r and weapon_l slots
	if (weapon->item_id != 0){
		
		// Regardless of proficiency class, about the only type of weapon that is limited
		// to who can use it are MAGIC weapons
		if ((weapon->weapon_class == WEAPON_CLASS_MAGICAL) || (weapon->proficiency_1 == PROFICIENCY_WEAPON_MAGIC) || (weapon->proficiency_2 == PROFICIENCY_WEAPON_MAGIC)){
			if (is_proficient(pc, PROFICIENCY_WEAPON_MAGIC) == 0){
				can_equip = 0;
			}
		}
		
		// All other weapons are fair-game, but will be less effective if the character
		// if not proficient in them... doesn't stop their use however.
		
		return can_equip;
	}
	
	return 0;
}

char pc_IsEquipped(PlayerState_t *pc, WeaponState_t *weapon, ItemState_t *item){
	// Return 0 or 1 if a given item or weapon is currently equipped
	
	if (item->item_id != 0){
		if (item->slot == ITEM_SLOT_HEAD){
			if (pc->head->item_id == item->item_id){
				return 1;
			}
		}
		if (item->slot == ITEM_SLOT_BODY){
			if (pc->body->item_id == item->item_id){
				return 1;
			}
		}
		if (item->slot == ITEM_SLOT_OPTION){
			if (pc->option->item_id == item->item_id){
				return 1;
			}
		}
	}
	
	if (weapon->item_id != 0){
		if ((pc->weapon_r->item_id == weapon->item_id) || (pc->weapon_l->item_id == weapon->item_id)){
			return 1;
		}
	}
	
	return 0;
}

char pc_IsLastItem(PlayerState_t *pc, WeaponState_t *weapon, ItemState_t *item){
	// Return 0 or 1 if the given item is the last instance of that item
	unsigned char i;
	
	for (i = 0; i < MAX_ITEMS; i++){
		if (weapon->item_id != 0){
			if ((pc->items[i].item_type == ITEM_TYPE_WEAPON) && (pc->items[i].item_id == weapon->item_id)){
				if (pc->items[i].qty == 1){
					return 1;
				}
			}
		}
		if (item->item_id != 0){
			if ((pc->items[i].item_type == ITEM_TYPE_ITEM) && (pc->items[i].item_id == item->item_id)){
				if (pc->items[i].qty == 1){
					return 1;
				}
			}
		}
	}
	return 0;
}

void pc_Equip(PlayerState_t *pc, WeaponState_t *weapon, ItemState_t *item, unsigned char hand){
	// Equip an item on a player character.
	// Automatically unequips any existing item on the same slot.
	// 2 handed weapons automatically unequip any weapon held in the left hand.
	 
	if (item->item_id != 0){
		// Assign item to head slot
		if (item->slot == ITEM_SLOT_HEAD){
			memcpy(pc->head, item, sizeof(ItemState_t));
		}
		// Assign item to body slot
		if (item->slot == ITEM_SLOT_BODY){
			memcpy(pc->body, item, sizeof(ItemState_t));
		}
		// Assign item to option slot
		if (item->slot == ITEM_SLOT_OPTION){
			memcpy(pc->option, item, sizeof(ItemState_t));
		}
	}
	
	if (weapon->item_id != 0){
		
		if (weapon->weapon_type == WEAPON_2HANDED){
			// Un-equip any other item held in the left hand
			pc->weapon_l->item_id = 0;
			// Equip the 2-handed weapon
			memcpy(pc->weapon_r, weapon, sizeof(WeaponState_t));
		} else {
			if (hand == RIGHT_HAND){
				// Equip to right hand
				memcpy(pc->weapon_r, weapon, sizeof(WeaponState_t));
			}
			if (hand == LEFT_HAND){
				// Equip to left hand
				memcpy(pc->weapon_l, weapon, sizeof(WeaponState_t));
			}
		}
	}	
}

void pc_Unequip(PlayerState_t *pc, WeaponState_t *weapon, ItemState_t *item, unsigned char hand){
	// Unequip an item on a player character.
	
	if (item->item_id != 0){
		// Clear item from head slot
		if (item->slot == ITEM_SLOT_HEAD){
			pc->head->item_id = 0;
		}
		// Clear item from body slot
		if (item->slot == ITEM_SLOT_BODY){
			pc->body->item_id = 0;
		}
		// Clear item from option slot
		if (item->slot == ITEM_SLOT_OPTION){
			pc->option->item_id = 0;
		}
	}
	
	if (weapon->item_id != 0){
		// Clear weapon from right hand
		if ((hand == RIGHT_HAND) && (pc->weapon_r->item_id == weapon->item_id)){
			pc->weapon_r->item_id = 0;
		}
		// Clear weapon from left hand
		if ((hand == LEFT_HAND) && (pc->weapon_l->item_id == weapon->item_id)){
			pc->weapon_l->item_id = 0;
		}
	}
}

// ==================================================
//
// Functions associated with detecting number of players 
// in the current party
//
// ==================================================

unsigned char party_Count(GameState_t *gamestate, unsigned char include_dead){
	// Returns a count of how many players are currently in the party
	unsigned char i;
	unsigned char total = 0;
	
	for (i = 0; i < MAX_PLAYERS; i++){
		// All player characters are above level 0
		if (gamestate->players->player[i]->level != 0){
			if (include_dead){
				total++;	
			} else {
				if (gamestate->players->player[i]->hp != 0){
					total++;
				}
			}		
		}
	}
	return total;
}

// ==================================================
//
// Functions associated with determining modifiers to 
// core game mechanics - dice rolls, combat, saving throws,
// weapon proficiencies etc.
//
// ==================================================

unsigned char player_weapon_DamageModifier(PlayerState_t *pc, WeaponState_t *weapon, unsigned char dmg_type, unsigned char *dmg_dice_type, unsigned char *dmg_dice_qty, unsigned char weapon_bonus, unsigned char versatile_bonus, unsigned char silvered_bonus, unsigned char status_bonus, unsigned char status_penalty){
	// Return the active damage modifier for a players weapon
	unsigned char bonus = 0;
	unsigned char b = 0;
	
	// STR modifier
	if ((weapon->weapon_class == WEAPON_CLASS_SIMPLE) || (weapon->weapon_class == WEAPON_CLASS_MARTIAL)){
		
		if (weapon->weapon_type == WEAPON_2HANDED){
			// 2H get 1.5x STR bonus
			bonus = bonus + ((ability_Modifier(pc->str) * 150) / 100);
		} else {
			// 1H get 1x STR bonus
			bonus = bonus + ability_Modifier(pc->str);
		}
	}
	
	if (weapon_bonus){
		bonus = bonus + weapon->bonus;	
	}
	
	// If versatile and no other weapon equipped, then a (fixed) bonus to damage dice type is applied
	if (versatile_bonus){
		if (weapon->versatile){
			if ((pc->weapon_l->item_id == 0) || (pc->weapon_r->item_id == 0)){
				*dmg_dice_type = *dmg_dice_type + 2;
			}
		}
	}
	
	if (silvered_bonus){
		// If silvered
		if (weapon->silvered){
			bonus++;
		}
	}
	
	if (status_penalty){
		b = 0;
		
		bonus = bonus - b;
	}
	
	if (status_bonus){
		b = 0;
		
		if (pc->status && STATUS_POWERFUL){
			b++;
		}
		
		bonus = bonus + b;
	}
	
	return bonus;
}

unsigned char player_weapon_AttackModifier(PlayerState_t *pc, WeaponState_t *weapon, unsigned char proficiency_bonus, unsigned char weapon_bonus, unsigned char ability_bonus, unsigned char status_bonus, unsigned char status_penalty){
	// Return the active attack-roll modifier for a players weapon
	unsigned char bonus = 0;
	unsigned char b1, b2, bmax;
	
	b1 = is_proficient(pc, weapon->proficiency_1);
	b2 = is_proficient(pc, weapon->proficiency_2);
	
	if (proficiency_bonus){
		// Add on skill/proficiency bonus
		if (b1 || b2){
			if (b1){
				bonus = bonus + b1;
			} else if (b2){
				bonus = bonus + b2;
			}
		}
	}
	
	if (weapon_bonus){
		// Add on inherent weapon bonus
		bonus = bonus + weapon->bonus;
	}
	
	if (ability_bonus){
		// Add on proficiency bonus
		b1 = ability_Modifier(pc->str);
		b2 = ability_Modifier(pc->dex);
		if (b1 > b2){
			bmax = b1;
		} else {
			bmax = b2;	
		}
		if (weapon->weapon_class == WEAPON_CLASS_RANGED){
			// DEX modifier
			if (weapon->finesse){
				bonus = bonus + bmax;
			} else {
				bonus = bonus + b2;
			}		
		} else if ((weapon->weapon_class == WEAPON_CLASS_SIMPLE) || (weapon->weapon_class == WEAPON_CLASS_MARTIAL)){
			// STR modifier
			if (weapon->finesse){
				bonus = bonus + bmax;
			} else {
				bonus = bonus + b1;
			}
		} else {
			// Magical weapons do not use either STR or DEX
		}
	}
	
	if (status_penalty){
		// Apply status penalties
		b1 = 0;
		
		if (pc->status && STATUS_BLINDED){
			b1++;
		}
		if (pc->status && STATUS_FRIGHTENED){
			b1++;
		}
		if (pc->status && STATUS_POISONED){
			b1++;
		}
		if (pc->status && STATUS_PRONE){
			b1++;
		}
		if (pc->status && STATUS_EXHAUSTED){
			b1++;
		}
		bonus = bonus - b1;
	}
	
	if (status_bonus){
		// Apply status bonuses
		b2 = 0;
		
		if (pc->status && STATUS_BLESSED){
			b2 = b2++;
		}
		bonus = bonus + b2;
	}
	
	return bonus;
}

char ability_Modifier(unsigned char ability){
	// Calculate the modifier for a given ability score
	// Cap modifier to a defined max and min value
	
	unsigned char new_ability = ability;
	char v;
	
	// Check for positive/negative status effects here
	//
	//
	// TO DO
	//
	
	v = (new_ability - 10) / 2;
	
	if (v > ABILITY_MODIFIER_MAX){
		return ABILITY_MODIFIER_MAX;
	}
	
	if (v < ABILITY_MODIFIER_MIN){
		return ABILITY_MODIFIER_MIN;
	}
	
	return v;
}

unsigned char is_proficient(PlayerState_t *pc, unsigned char proficiency_type){
	// Returns 0 if the selected player character is
	// NOT proficient with a given proficiency type, otherwise
	// returns the current proficiency bonus, based on the level of
	// the player character.
	
	unsigned char bonus = 0;
	unsigned char i;
	unsigned char player_class_id = pc->player_class;
	
	// Loop over all proficiencies for this class
	// to find if they have the proficiency for it.
	for (i = 0; i < MAX_PROFICIENCIES; i++){
		if (player_class_proficiencies[player_class_id][i] == proficiency_type){
			bonus = (pc->level / 5) + PROFICIENCY_BONUS_MIN;
			if (bonus > PROFICIENCY_BONUS_MAX){
				return PROFICIENCY_BONUS_MAX;
			}
			return bonus;
		}
	}
	return 0;	
}

// ==================================================
//
// Function associated with dice rolls
//
// ==================================================

unsigned char roll_Dice(unsigned char dice_quantity, unsigned char dice_type){
	// Roll the specified number and type of dice	
}

void hit_Dice(PlayerState_t *pc, unsigned char *dice_quantity, unsigned char *dice_type, char *constitution_modifier){
	// Sets 3 values (dice_quantity, dice_type, consitution_modifier) representing the number and type
	// of hit-dice used when levelling up for the given
	// player character.
	
	// There are no classes known at this time that
	// receive more than 1 hit die per level.
	*dice_quantity = 1;
	
	// Minimum HP increase from modifier is +1.
	if (*constitution_modifier < 1){
		*constitution_modifier = 1;	
	}
	
	switch(pc->player_class){
		case CLASS_UNTRAINED:		// The average peasant in the street
			*dice_type = 4;
			break;
		case CLASS_BARD:			// Our typical D&D classes
		case CLASS_CLERIC:
		case CLASS_DRUID:
		case CLASS_ROGUE:
		case CLASS_SORCERER:
		case CLASS_WARLOCK:
		case CLASS_WIZARD:
		case CLASS_GENERIC_MAGIC:
		case CLASS_GENERIC_RANGED:
			*dice_type = 8;
			break;
		case CLASS_GENERIC_MELEE:	// Our core, combat focussed classes	
		case CLASS_FIGHTER:			
		case CLASS_PALADIN:
		case CLASS_RANGER:
			*dice_type = 10;
			break;
		case CLASS_BARBARIAN:		// Conan
			*dice_type = 12;
			break;
		default:
			*dice_type = 6;			// If we don't know what class you are
	}
	return;
}
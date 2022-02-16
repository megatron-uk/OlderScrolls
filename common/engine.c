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

extern const char *proficiencies[22] = {
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
	"Saving Throw (Str.)",	// #16
	"Saving Throw (Con.)",	// #17
	"Saving Throw (Dex.)",	// #18
	"Saving Throw (Chr.)",	// #19
	"Saving Throw (Wis.)",	// #20
	"Saving Throw (Intl.)",	// #21
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
	PROFICIENCY_WEAPON_MARTIAL, PROFICIENCY_SAVE_STRENGTH, PROFICIENCY_SAVE_DEXTERITY, PROFICIENCY_NONE, 
	PROFICIENCY_NONE, PROFICIENCY_NONE},
	
	// #11 ROgue
	{PROFICIENCY_ARMOUR_L, PROFICIENCY_WEAPON_SIMPLE, PROFICIENCY_WEAPON_CROSSBOW_L, PROFICIENCY_WEAPON_SWORD_S,
	PROFICIENCY_WEAPON_SWORD_L, PROFICIENCY_WEAPON_RAPIER, PROFICIENCY_SAVE_DEXTERITY, PROFICIENCY_SAVE_INTELLIGENCE,
	PROFICIENCY_NONE, PROFICIENCY_NONE},
	
	// #12 Sorcerer
	{PROFICIENCY_WEAPON_DAGGER, PROFICIENCY_WEAPON_STAFF, PROFICIENCY_WEAPON_CROSSBOW_L, PROFICIENCY_SAVE_CHARISMA,
	PROFICIENCY_SAVE_CONSTITUTION, PROFICIENCY_NONE, PROFICIENCY_NONE, PROFICIENCY_NONE, 
	PROFICIENCY_NONE, PROFICIENCY_NONE},
	
	// #13 Warlock
	{PROFICIENCY_ARMOUR_L, PROFICIENCY_WEAPON_SIMPLE, PROFICIENCY_SAVE_WISDOM, PROFICIENCY_SAVE_CHARISMA,
	PROFICIENCY_NONE, PROFICIENCY_NONE, PROFICIENCY_NONE, PROFICIENCY_NONE,
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
// Functions associated with determining modifiers to 
// core game mechanics - dice rolls, combat, saving throws,
// weapon proficiencies etc.
//
// ==================================================

char ability_Modifier(unsigned char ability){
	// Calculate the modifier for a given ability score
	// Cap modifier to a defined max and min value
	
	char v;
	
	v = (ability - 10) / 2;
	
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
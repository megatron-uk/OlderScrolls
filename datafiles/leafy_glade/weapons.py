#!/usr/bin/env python3

""" weapons.py, Weapon definitions for the adventure 'The Sands of Terror'
 for the OlderScrolls RPG game engine.
 
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
"""

WEAPONS = {
	
	1 : {
		"item_id" 	: 1,		# 1 byte. Unique weapon item id 
		"type"		: '1H', 	# 1 byte. 1H or 2H
		"class"		: 'SIMPLE',	# 1 byte. SIMPLE, MARTIAL, RANGED, MAGICAL
		"rarity"	: 'COMMON',	# 1 byte. COMMON, UNCOMMON, RARE, LEGENDARY
		'size' 		: 'MEDIUM',	# 1 byte. SMALL, MEDIUM, LARGE, HUGE	
		'proficiency_1' : 'PROFICIENCY_WEAPON_SIMPLE',	# 1 byte. Primary weapon proficiency skill
		'proficiency_2' : -1,							# 1 byte. Secondary weapon proficiency skill
		"name" 		: "Wooden Stick", 	# 18 bytes. Name of weapon
		"crit" 		: [20,20, 2] ,		# 3 bytes. Critical damage min/max roll range (20-20) and number of dice (1) 
		"dmgtype"	: 					# 9 bytes total. Up to 3 damage types may be listed
		{				
			"PHYSICAL"		: [1,3],	# 3 bytes. First damage type; (PHYSICAL), number of dice (1), dice type (D2)
										# 3 bytes. Second damage type.
										# 3 bytes. Third damage type.
		},
		"versatile"	: 1,		# 1 byte. Not a versatile weapon (1H or 2H)
		"finesse"	: 0,		# 1 byte. Not a finesse weapon (STRENGTH or DEXTERITY bonus to attack)
		"silvered" 	: 0,		# 1 byte. Not a silvered weapon (bonus damage to undead/monsters with physical resistance)
		"bonus"		: 0,		# 1 byte. 0, +1, +2, +3 bonus to attack and damage rolls.
		"value"		: 1,		# 2 bytes. Base value of the weapon.
		"text"		: 14,
	},
	2 : {
		"item_id" 	: 2,		# 1 byte. Unique weapon item id 
		"type"		: '1H', 	# 1 byte. 1H or 2H
		"class"		: 'SIMPLE',	# 1 byte. SIMPLE, MARTIAL, RANGED, MAGICAL
		"rarity"	: 'COMMON',	# 1 byte. COMMON, UNCOMMON, RARE, LEGENDARY
		'size' 		: 'MEDIUM',	# 1 byte. SMALL, MEDIUM, LARGE, HUGE	
		'proficiency_1' : 'PROFICIENCY_WEAPON_SIMPLE',	# 1 byte. Primary weapon proficiency skill
		'proficiency_2' : 'PROFICIENCY_WEAPON_DAGGER',	# 1 byte. Secondary weapon proficiency skill
		"name" 		: "Rusty Knife", 	# 18 bytes. Name of weapon
		"crit" 		: [20,20, 2] ,		# 3 bytes. Critical damage min/max roll range (20-20) and number of dice (1) 
		"dmgtype"	: 					# 9 bytes total. Up to 3 damage types may be listed
		{				
			"PIERCING"		: [1,4],	# 3 bytes. First damage type; (PHYSICAL), number of dice (1), dice type (D2)
										# 3 bytes. Second damage type.
										# 3 bytes. Third damage type.
		},
		"versatile"	: 0,		# 1 byte. Not a versatile weapon (1H or 2H)
		"finesse"	: 0,		# 1 byte. Not a finesse weapon (STRENGTH or DEXTERITY bonus to attack)
		"silvered" 	: 0,		# 1 byte. Not a silvered weapon (bonus damage to undead/monsters with physical resistance)
		"bonus"		: 0,		# 1 byte. 0, +1, +2, +3 bonus to attack and damage rolls.
		"value"		: 5,		# 2 bytes. Base value of the weapon.
		"text"		: 15,
	},
}

################################################################
#
# END OF DATA SECTION
#
################################################################

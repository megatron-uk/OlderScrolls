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

WEAPON_CLASSES = {
	'SIMPLE' 	: 1,
	'MARTIAL'	: 2,
	'EXOTIC' 	: 3,
	'RANGED' 	: 4,
	'MAGICAL'	: 5,
}

WEAPON_TYPES = {
	'1H' 		: 1,
	'2H' 		: 2,
}

WEAPON_RARITY = {
	'COMMON'	: 1,
	'UNCOMMON'	: 2,
	'RARE'		: 3,
	'LEGENDARY'	: 4,
}

WEAPON_DAMAGE = {
	'PHYSICAL' 	: 1,
	'SLASHING' 	: 2,
	'PIERCING' 	: 3,
	'BLUNT'	   	: 4,
	'LIGHTNING' 	: 5,
	'ACID'		: 6,
	'FIRE'		: 7,
	'COLD'		: 8,
	'POISON'	: 9,
}

WEAPON_SIZE = {
	'SMALL'		: 1,
	'MEDIUM'	: 2,
	'LARGE'		: 3,
	'HUGE'		: 4,
}

WEAPONS = {
	
	#############################################
	#
	# 1H Weapons
	#
	
	1 : {
		"item_id" 	: 1, 
		"type"	: '1H', 
		"class"	: 'SIMPLE',
		"rarity": 'COMMON',
		'size' : 'SMALL',
		"name" 	: "Wooden Stick", 
		"crit" 	: [20,20, 2] ,
		"dmgtype" : {
			"PHYSICAL"		:  [1, 2, 1],
		}
	},
}

################################################################
#
# END OF DATA SECTION
#
################################################################

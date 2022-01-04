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
	'PHYSICAL' 		: 1,
	'SLASHING' 		: 2,
	'PIERCING' 		: 3,
	'BLUNT'	   		: 4,
	'LIGHTNING' 	: 5,
	'ACID'			: 6,
	'FIRE'			: 7,
	'COLD'			: 8,
	'POISON'		: 9,
}

WEAPON_SIZE = {
	'SMALL'		: 1,
	'MEDIUM'	: 2,
	'LARGE'		: 3,
	'HUGE'		: 4,
}

WEAPONS = [
	
	#############################################
	#
	# 1H Weapons
	#
	
	{
		"type"	: '1H', 
		"class"	: 'SIMPLE',
		"rarity": 'COMMON',
		'size' : 'SMALL',
		"item_id" 	: 1, 
		"name" 	: "Dagger", 
		"crit" 	: [20,20, 2] ,
		"dmgtype" : {
			"PHYSICAL"		:  [1, 3, 1],
			"PIERCING" 		: [1, 2, 1]
		}
	},
	{
		"type"	: '1H', 
		"class"	: 'MARTIAL',
		"rarity": 'COMMON',
		'size' : 'MEDIUM',
		"item_id" 	: 2, 
		"name" 	: "Longsword", 
		"crit" 	: [20,20, 2] ,
		"dmgtype" : {
			"PHYSICAL"		: [1, 6, 1],
			"SLASHING" 		: [1, 2, 1]
		}
	},
	{
		"type"	: '1H', 
		"class"	: 'MARTIAL',
		"rarity": 'UNCOMMON',
		'size' : 'MEDIUM',
		"item_id" 	: 3, 
		"name" 	: "Rapier", 
		"crit" 	: [19,20, 2], 
		"dmgtype" : {
			"PHYSICAL"		: [1,4,1],
			"PIERCING" 		: [1,5,1]
		},
	},
	{
		"type"	: '1H', 
		"class"	: 'EXOTIC',
		'size' : 'MEDIUM',
		"item_id" 	: 4, 
		"rarity": 'UNCOMMON',
		"name" 	: "Katana", 
		"crit" 	: [19,20, 2], 
		"dmgtype" : {
			"PHYSICAL"		: [1, 6, 1], 
			"SLASHING" 		: [1, 4, 1]
		}
	},
	{
		"type"	: '1H', 
		"class"	: 'EXOTIC',
		"rarity": 'LEGENDARY',
		'size' : 'MEDIUM',
		"item_id" 	: 5, 
		"name" 	: "Katana of Thunder", 
		"crit" 	: [19,20, 2], 
		"dmgtype" : {
			"PHYSICAL"		: [1, 6, 1], 
			"LIGHTNING" 	: [1, 4, 1], 
			"SLASHING" 		: [1, 4, 1]
		}
	},	
	{
		"type"	: '2H', 
		"class"	: 'SIMPLE',
		"rarity": 'COMMON',
		'size' : 'LARGE',
		"item_id" 	: 6, 
		"name" 	: "Quarterstaff", 
		"dmgtype" : { 
			"PHYSICAL" 	: [1, 6, 1] 
		}, 
		"crit" : [19,20, 2] 
	},
	{
		"type"	: '1H', 
		"class"	: 'MARTIAL',
		"rarity": 'COMMON',
		'size' : 'MEDIUM',
		"item_id" 	: 7, 
		"name" 	: "Mace", 
		"dmgtype" : { 
			"PHYSICAL" 	: [1, 4, 1],
			"BLUNT" 	: [1, 6, 1] 
		}, 
		"crit" : [19,20, 2] 
	},
	{
		"type"	: '2H', 
		"class"	: 'MARTIAL',
		"rarity": 'UNCOMMON',
		'size' : 'LARGE',
		"item_id" 	: 8, 
		"name" 	: "Greatsword", 
		"dmgtype" : {
			"PHYSICAL" 	: [1, 12, 1], 
			"SLASHING" 	: [1, 6, 1] 
		}, 
		"crit" : [19,20, 2] 
	},
	{
		"type"	: '2H', 
		"class"	: 'MARTIAL',
		"rarity": 'UNCOMMON',
		'size' : 'LARGE',
		"item_id" 	: 9, 
		"name" 	: "War Hammer", 
		"dmgtype" : {
			"PHYSICAL" 	: [1, 12, 1], 
			"BLUNT" 	: [1, 6, 1] 
		}, 
		"crit" : [19,20, 2] 
	},
	{
		"type"	: '2H', 
		"class"	: 'RANGED',
		"rarity": 'COMMON',
		'size' : 'MEDIUM',
		"item_id" 	: 10, 
		"name" 	: "Hunting Bow", 
		"dmgtype" : { 
			"PIERCING" 	: [1, 6, 1] 
		}, 
		"crit" : [18,20, 2] 
	},
	{
		"type"	: '2H', 
		"class"	: 'MAGICAL',
		"rarity": 'UNCOMMON',
		'size' : 'MEDIUM',
		"item_id" 	: 11, 
		"name" 	: "Magical Staff", 
		"dmgtype" : { 
			"PHYSICAL" 	: [1, 4, 1] 
		}, 
		"crit" : [19,20, 2] 
	},
	{
		"type"	: '1H', 
		"class"	: 'SIMPLE',
		"rarity": 'COMMON',
		'size' : 'SMALL',
		"item_id" 	: 12, 
		"name" 	: "Axe", 
		"crit" 	: [20,20, 2] ,
		"dmgtype" : {
			"PHYSICAL"		: [1, 8, 1],
		} 
	},
	{
		"type"	: '2H', 
		"class"	: 'MARTIAL',
		"rarity": 'UNCOMMON',
		'size' : 'LARGE',
		"item_id" 	: 13, 
		"name" 	: "Battle Axe", 
		"dmgtype" : { 
			"PHYSICAL" 	: [1, 16, 1]
		}, 
		"crit" : [20,20, 2] 
	},
	{
		"type"	: '1H', 
		"class"	: 'RANGED',
		"rarity": 'COMMON',
		'size' : 'SMALL',
		"item_id" 	: 14, 
		"name" 	: "Slingshot", 
		"dmgtype" : { 
			"PHYSICAL" 	: [1, 4, 1],
			"BLUNT" 	: [1, 2, 1] 
		}, 
		"crit" : [19,20, 2] 
	},
	{
		"type"	: '1H', 
		"class"	: 'RANGED',
		"rarity": 'COMMON',
		'size' : 'SMALL',
		"item_id" 	: 15, 
		"name" 	: "Light Crossbow", 
		"dmgtype" : { 
			"PIERCING" 	: [1, 6, 1]  
		}, 
		"crit" : [19,20, 2] 
	},
	{
		"type"	: '1H', 
		"class"	: 'RANGED',
		"rarity": 'RARE',
		'size' : 'MEDIUM',
		"item_id" 	: 16, 
		"name" 	: "Cursed Longsword", 
		"crit" 	: [20,20, 2] ,
		"dmgtype" : {
			"PHYSICAL"		: [1, 6, 1],
			"SLASHING" 		: [1, 2, 1],
			"POISON"		: [1, 3, 1]
		}
	},
]

################################################################
#
# END OF DATA SECTION
#
################################################################

import os

def generate_weapondata():
	""" Generates a datafile of the above weapons, suitable for
	reading in via simple C file open/read commands. """
	
	print("############################################")
	print("Generating PETSCII weapon datafile")
	print("")
	
	header = ""
	header += "id,type,class,rarity,name,"
	header += "critical_min,critical_max,critical_multi,"
	header += "dmg_types,"
	header += "dmg1_type,dmg1_min,dmg1_max,dmg1_rolls,"
	header += "dmg2_type,dmg2_min,dmg2_max,dmg2_rolls,"
	header += "dmg3_type,dmg3_min,dmg3_max,dmg3_rolls,"
	header += "description"
	header += "\n"
	datafile = ""
	for w in WEAPONS:
		# Item ID, item type, item class, item rarity, item name, 
		# critical-min, critical-max, critical-multi, 
		# number-of-damage-types, 
		# dmg1_type, dmg1_min, dmg1_max, dmg1_rolls
		# dmg2_type, dmg2_min, dmg2_max, dmg2_rolls
		# dmg3_type, dmg3_min, dmg3_max, dmg3_rolls
		
		weapon_line = "%03d,%1s,%1s,%1d,%18s,%02d,%02d,%1d,%1d," % (
				w['item_id'], WEAPON_TYPES[w['type']], WEAPON_CLASSES[w['class']], WEAPON_RARITY[w['rarity']], w['name'][0:17], 
				w['crit'][0], w['crit'][1], w['crit'][2], 
				len(w['dmgtype'].keys())				
			)
		if (len(w['dmgtype'].keys()) == 3):
			for k in w['dmgtype'].keys():
				weapon_line += "%1d,%1d,%02d,%1d,"	% (WEAPON_DAMAGE[k], w['dmgtype'][k][0], w['dmgtype'][k][1], w['dmgtype'][k][2])
		elif (len(w['dmgtype'].keys()) == 2):
			for k in w['dmgtype'].keys():
				weapon_line += "%1d,%1d,%02d,%1d,"	% (WEAPON_DAMAGE[k], w['dmgtype'][k][0], w['dmgtype'][k][1], w['dmgtype'][k][2])
			weapon_line += "0,0,00,0,"	#dmg3
		elif (len(w['dmgtype']) == 1):
			for k in w['dmgtype'].keys():
				weapon_line += "%1d,%1d,%02d,%1d,"	% (WEAPON_DAMAGE[k], w['dmgtype'][k][0], w['dmgtype'][k][1], w['dmgtype'][k][2])
			weapon_line += "0,0,00,0,"	#dmg2
			weapon_line += "0,0,00,0,"	#dmg3
		else:
			weapon_line += "0,0,00,0,"	#dmg1
			weapon_line += "0,0,00,0,"	#dmg2
			weapon_line += "0,0,00,0,"	#dmg3
		weapon_line += "N/A"
			
		weapon_line += "\n"
		print("Line length: %s\n" % len(weapon_line))
		datafile += weapon_line
			
	print(header)
	print(datafile)
			
	# Write a plain ASCII text file
	f = open('datafiles/weapons.txt', "w")
	f.write(datafile)
	f.close()
	# Convert text file to PETSCII
	os.system('datafiles/petcom - datafiles/weapons.txt > datafiles/weapons.dat')
	print("")
	print("The PETSCII datafile can be found in datafiles/weapons.dat")
	print("")
	
	
def generate_header():
	""" Generate a C header file from the data above """
	
	print("############################################")
	print("Generating C header file of weapon constants")
	print("")
	
	header =  "// This is an autogenerated header file featuring\n"
	header += "// all of the weapon classes, types, etc which are\n"
	header += "// defined in datafiles/weapons.py - DO NOT EDIT!\n"
	header += "\n"
	header += "// Weapon types\n"
	for wt in WEAPON_TYPES.keys():
		header += "#define WEAPON_TYPE_%s %d\n" % (wt, WEAPON_TYPES[wt])
		
	header += "\n"
	header += "// Weapon classes\n"
	for wc in WEAPON_CLASSES.keys():
		header += "#define WEAPON_CLASS_%s %d\n" % (wc, WEAPON_CLASSES[wc])
		
	header += "\n"
	header += "// Weapon rarity\n"
	for wr in WEAPON_RARITY.keys():
		header += "#define WEAPON_RARITY_%s %d\n" % (wr, WEAPON_RARITY[wr])
	
	print(header)
	print("")
	# Write a plain ASCII text file
	f = open('include/weapons.h', "w")
	f.write(header)
	f.close()
	print("The header can now be found in include/weapons.h")
	print("")
	
generate_weapondata()
generate_header()
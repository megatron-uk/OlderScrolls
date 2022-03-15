#!/usr/bin/env python3

""" monster.py, Monster definitions for the adventure 'Trouble in Leafy Glade'
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

# Party characters that can be recruited
PARTY = {	
}

# NPC characters you can (should?) talk to... 
# NPC's all have the same stats as monsters, so be careful who you pick a fight with!
NPC = {
	0 : {
		'type'			: "CHARACTER_TYPE_NPC", # One of CHARACTER_TYPE_MONSTER, CHARACTER_TYPE_NPC, CHARACTER_TYPE_BOSS
        'name' 			: "Default",   		# Name as displayed to the player
        'sprite_type' 	: "SPRITE_CLASS_NORMAL",	# 32x32 sprites are normal size
        'resting_sprite' 	: ["skull.bmp"],		# Name of bitmaps file used in combat screen - for resting animation
        'attack_sprite'		: [],
        'magic_sprite'		: [],
        'wounded_sprite'	: [],
        'death_sprite'		: [],
        'portrait_name'		: ["skull.bmp"],		# Name of bitmap file for dialogue screens
        'class'		: "UNTRAINED",			# Class
        'race'		: "HUMAN",
        'level'		: 0,					# Level
        'profile'	: 0xA000,				# Attack/behaviour profile, 4 bits each for how aggressive the monster is in melee, ranged, magic attack, magic support
        'str' 		: 1,					# Standard attributes - they may be modified by the level and class of the monster
        'dex' 		: 1,					# - as above
        'con' 		: 1,					# - as above
        'wis' 		: 1,					# - as above					
        'intl' 		: 1,					# - as above
        'chr' 		: 1,					# - as above
        'hp' 		: 1,					# Starting hitpoints
        'status' 	: 0,					# Starting bitfield of status effects
        'head'		: -1,					# Item equipped to head (e.g. helmet, hat)
        'body'		: -1,					# Item equipped to body (e.g. bronze scale armour)
        'option'	: -1,					# Item equipped to ring/amulet/necklace slot
        'weapon_r' : -1,					# ID of weapon in right hand
        'weapon_l' : -1,					# ID of weapon in left hand
        'formation' : 0,					# Front / Middle / Rear alignment in combat
        'spells'	: [0,0,0,0,0],			# List of all spells that this monster has access to
	},
	1 : {
		'type'			: "CHARACTER_TYPE_NPC",
        'name' 		: "Village Elder",     			# Name as displayed to the player
        'sprite_type' : "SPRITE_CLASS_NORMAL",		# 32x32 sprites are normal size
        'resting_sprite' 	: ["elder.bmp"],		# Name of bitmap file for combat screen
        'attack_sprite'		: [],
        'magic_sprite'		: [],
        'wounded_sprite'	: [],
        'death_sprite'		: [],
        'portrait_name' 	: ["elder.bmp"],		# Name of bitmap file for dialogue, always sprite_class_normal
        'class'		: "UNTRAINED",			# Class
        'race'		: "HUMAN",
        'level'		: 1,					# Level
        'profile'	: 0xA000,				# Attack/behaviour profile, 4 bits each for how aggressive 
        									# the monster is in melee, ranged, magic attack, magic support
        'str' 		: 8,					# Standard attributes - they may be modified by the level and class of the monster
        'dex' 		: 6,					# - as above
        'con' 		: 9,					# - as above
        'wis' 		: 8,					# - as above					
        'intl' 		: 9,					# - as above
        'chr' 		: 6,					# - as above
        'hp' 		: 60,					# Starting hitpoints
        'status' 	: 0,					# Starting bitfield of status effects
        'head'		: -1,					# Item equipped to head (e.g. helmet, hat)
        'body'		: -1,					# Item equipped to body (e.g. bronze cuirass)
        'option'	: -1,					# Item equipped to option slot - ring/necklace etc
        'weapon_r' 	: -1,	 			    # ID of weapon in right hand
        'weapon_l' 	: -1,					# ID of weapon in left hand
        'formation' : 0,					# Front / Middle / Rear alignment in combat
        'spells'	: [0,0,0,0,0],			# List of all spells that this monster has access to
	},
}

# The monsters we have in this adventure...
MONSTER = {
	0 : {
		'type'			: "CHARACTER_TYPE_MONSTER", # One of CHARACTER_TYPE_MONSTER, CHARACTER_TYPE_NPC, CHARACTER_TYPE_BOSS
        'name' 			: "Default",   		# Name as displayed to the player
        'sprite_type' 	: "SPRITE_CLASS_NORMAL",	# 32x32 sprites are normal size
        'resting_sprite' 	: ["skull.bmp"],		# Name of bitmaps file used in combat screen - for resting animation
        'attack_sprite'		: [],
        'magic_sprite'		: [],
        'wounded_sprite'	: [],
        'death_sprite'		: [],
        'portrait_name'		: ["skull.bmp"],		# Name of bitmap file for dialogue screens
        'class'		: "UNTRAINED",			# Class
        'race'		: "HUMAN",
        'level'		: 0,					# Level
        'profile'	: 0xA000,				# Attack/behaviour profile, 4 bits each for how aggressive the monster is in melee, ranged, magic attack, magic support
        'str' 		: 1,					# Standard attributes - they may be modified by the level and class of the monster
        'dex' 		: 1,					# - as above
        'con' 		: 1,					# - as above
        'wis' 		: 1,					# - as above					
        'intl' 		: 1,					# - as above
        'chr' 		: 1,					# - as above
        'hp' 		: 1,					# Starting hitpoints
        'status' 	: 0,					# Starting bitfield of status effects
        'head'		: -1,					# Item equipped to head (e.g. helmet, hat)
        'body'		: -1,					# Item equipped to body (e.g. bronze scale armour)
        'option'	: -1,					# Item equipped to ring/amulet/necklace slot
        'weapon_r' : -1,					# ID of weapon in right hand
        'weapon_l' : -1,					# ID of weapon in left hand
        'formation' : 0,					# Front / Middle / Rear alignment in combat
        'spells'	: [0,0,0,0,0],			# List of all spells that this monster has access to
	},
	1 : {
        'type'			: "CHARACTER_TYPE_MONSTER",
        'name' 			: "Ruffian",     		# Name as displayed to the player
        'sprite_type' 	: "SPRITE_CLASS_NORMAL",	# 32x32 sprites are normal size
        'resting_sprite' 	: ["human.bmp",],				# Name of bitmap file used in combat screen - the default 'resting' animation
        'attack_sprite'		: [],
        'magic_sprite'		: [],
        'wounded_sprite'	: [],
        'death_sprite'		: [],
        'portrait_name'		: ["thug.bmp"],		# Name of bitmap file for dialogue screens
        'class'		: "UNTRAINED",			# Class
        'race'		: "HUMAN",
        'level'		: 1,					# Level
        'profile'	: 0xA000,				# Attack/behaviour profile, 4 bits each for how aggressive 
        									# the monster is in melee, ranged, magic attack, magic support
        'str' 		: 14,					# Standard attributes - they may be modified by the level and class of the monster
        'dex' 		: 7,					# - as above
        'con' 		: 11,					# - as above
        'wis' 		: 3,					# - as above					
        'intl' 		: 4,					# - as above
        'chr' 		: 2,					# - as above
        'hp' 		: 99,					# Starting hitpoints
        'status' 	: 0,					# Starting bitfield of status effects
        'head'		: 4,					# Item equipped to head (e.g. helmet, hat)
        'body'		: 3,					# Item equipped to body (e.g. bronze scale armour)
        'option'	: -1,					# Item equipped to ring/amulet/necklace slot
        'weapon_r' : -1,					    # ID of weapon in right hand
        'weapon_l' : -1,					# ID of weapon in left hand
        'formation' : 0,					# Front / Middle / Rear alignment in combat
        'spells'	: [0,0,0,0,0],			# List of all spells that this monster has access to
	},
	2 : {
		'type'			: "CHARACTER_TYPE_MONSTER",
        'name' 			: "Wild Dog",
        'sprite_type' 	: "SPRITE_CLASS_NORMAL",	
        'resting_sprite' 	: ["wolf.bmp"],
        'attack_sprite'		: [],
        'magic_sprite'		: [],
        'wounded_sprite'	: [],
        'death_sprite'		: [],
        'portrait_name'		: ["wolf.bmp"],
        'class'		: "UNTRAINED",			# Class
        'race'		: "BEAST",			 
        'level'		: 1,					
        'profile'	: 0xD000,				 
        'str' 		: 5,					
        'dex' 		: 8,					
        'con' 		: 4,					
        'wis' 		: 1,										
        'intl' 		: 2,					
        'chr' 		: 1,					
        'hp' 		: 15,					
        'status' 	: 0,					
        'head'		: -1,					
        'body'		: -1,					
        'option'	: -1,					
        'weapon_r' : -1,
        'weapon_l' : -1,
        'formation' : 0,
        'spells'	: [0,0,0,0,0],
	},
	3 : {
		'type'			: "CHARACTER_TYPE_BOSS",
        'name' 			: "Orc Giant",
        'sprite_type' 	: "SPRITE_CLASS_LARGE",	
        'resting_sprite' 	: ["orc.bmp"],
        'attack_sprite'		: [],
        'magic_sprite'		: [],
        'wounded_sprite'	: [],
        'death_sprite'		: [],			
        'portrait_name'		: ["skull.bmp"],
        'class'		: "GENERIC_MELEE",			# Class
        'race'		: "ORC",			 
        'level'		: 1,					
        'profile'	: 0xD000,				 
        'str' 		: 5,					
        'dex' 		: 8,					
        'con' 		: 4,					
        'wis' 		: 1,										
        'intl' 		: 2,					
        'chr' 		: 1,					
        'hp' 		: 15,					
        'status' 	: 0,					
        'head'		: -1,					
        'body'		: -1,					
        'option'	: -1,					
        'weapon_r' : -1,
        'weapon_l' : -1,
        'formation' : 0,
        'spells'	: [0,0,0,0,0],
	}
}

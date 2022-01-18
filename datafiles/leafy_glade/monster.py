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
NPC = {
	1 : {
        'name' 		: "Village Elder",     		# Name as displayed to the player
        'sprite_type' : "SPRITE_CLASS_NORMAL",	# 32x32 sprites are normal size
        'sprite_name' : "beetle_b",		# Name of bitmap file for combat screen
        'portrait_name' : "beetle_b",		# Name of bitmap file for dialogue
        'class'		: "HUMAN_UNTRAINED",	# Class 
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
        'neck'		: -1,					# Item equipped to neck (e.g. amulet)
        'body'		: -1,					# Item equipped to body (e.g. bronze cuirass)
        'arms'		: -1,					# Item equipped to arms (e.g. iron pauldrons)
        'legs'		: -1,					# Item equipped to legs (e.g. plate greaves)
        'hand_r'	: -1,					# Item on right hand (e.g. ring)
        'hand_l'	: -1,					# Item on left hand (e.g. ring)
        'weapon_r' 	: -1,	 			    # ID of weapon in right hand
        'weapon_l' 	: -1,					# ID of weapon in left hand
        'formation' : 0,					# Front / Middle / Rear alignment in combat
        'spells'	: [0,0,0,0,0],			# List of all spells that this monster has access to
	},
}

# The monsters we have in this adventure...
MONSTER = {
	1 : {
        'name' 		: "Ruffian",     		# Name as displayed to the player
        'sprite_type' : "SPRITE_CLASS_NORMAL",	# 32x32 sprites are normal size
        'sprite_name' : "beetle_b",		# Name of bitmap file used in combat screen
        'portrait_name' : "beetle_b",		# Name of bitmap file for dialogue screens
        'class'		: "HUMAN_UNTRAINED",	# Class 
        'level'		: 1,					# Level
        'profile'	: 0xA000,				# Attack/behaviour profile, 4 bits each for how aggressive 
        									# the monster is in melee, ranged, magic attack, magic support
        'str' 		: 10,					# Standard attributes - they may be modified by the level and class of the monster
        'dex' 		: 7,					# - as above
        'con' 		: 11,					# - as above
        'wis' 		: 3,					# - as above					
        'intl' 		: 4,					# - as above
        'chr' 		: 2,					# - as above
        'hp' 		: 99,					# Starting hitpoints
        'status' 	: 0,					# Starting bitfield of status effects
        'head'		: -1,					# Item equipped to head (e.g. helmet, hat)
        'neck'		: -1,					# Item equipped to neck (e.g. amulet)
        'body'		: -1,					# Item equipped to body (e.g. bronze cuirass)
        'arms'		: -1,					# Item equipped to arms (e.g. iron pauldrons)
        'legs'		: -1,					# Item equipped to legs (e.g. plate greaves)
        'hand_r'	: -1,					# Item on right hand (e.g. ring)
        'hand_l'	: -1,					# Item on left hand (e.g. ring)
        'weapon_r' : 1,					    # ID of weapon in right hand
        'weapon_l' : -1,					# ID of weapon in left hand
        'formation' : 0,					# Front / Middle / Rear alignment in combat
        'spells'	: [0,0,0,0,0],			# List of all spells that this monster has access to
	},
	2 : {
        'name' 		: "Wild Dog",
        'sprite_type' : "SPRITE_CLASS_NORMAL",	
        'sprite_name' : "beetle_b",
        'portrait_name' : "beetle_b",
        'class'		: "BEAST_WILD",			 
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
        'neck'		: -1,					
        'body'		: -1,					
        'arms'		: -1,					
        'legs'		: -1,					
        'hand_r'	: -1,					
        'hand_l'	: -1,
        'weapon_r' : -1,
        'weapon_l' : -1,
        'formation' : 0,
        'spells'	: [0,0,0,0,0],
	},
}

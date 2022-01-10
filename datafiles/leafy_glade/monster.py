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

# These classes should match the definitions 
# from the OlderScrolls engine 'monsters.h' header.
MONSTER_CLASSES = {
		"HUMAN_UNTRAINED" 		: 0x10,	
		"HUMAN_GENERIC_MELEE"	: 0x11,
		"HUMAN_GENERIC_RANGED"	: 0x12,
		"HUMAN_GENERIC_MAGIC"	: 0x13,
		"HUMAN_BARBARIAN"		: 0x14,			
		"HUMAN_BARD"			: 0x15,
		"HUMAN_CLERIC"			: 0x16,
		"HUMAN_DRUID"			: 0x17,
		"HUMAN_FIGHTER"			: 0x18,
		"HUMAN_PALADIN"			: 0x19,
		"HUMAN_RANGER"			: 0x1A,
		"HUMAN_ROGUE"			: 0x1B,
		"HUMAN_SORCERER"		: 0x1C,
		"HUMAN_WARLOCK"			: 0x1D,
		"HUMAN_WIZARD"			: 0x1E,
		"BEAST_WILD"			: 0x20,
		"BEAST_MELEE"			: 0x21,
		"BEAST_RANGED"			: 0x22,
		"BEAST_MAGIC"			: 0x23,
}

# NPC characters
NPC = {
	1 : "Village Elder",
}

# The monsters we have in this adventure
MONSTER = {
	1 : {
        'name' 		: "Ruffian",     		# Name as displayed to the player
        'class'		: "HUMAN_UNTRAINED",	# Class 
        'level'		: 1,					# Level
        'profile'	: 0xA000,				# Attack/behaviour profile, 4 bits each for how aggressive 
        									# the monster is in melee, ranged, magic attack, magic support
        'str' 		: 10,					# Standard attributes
        'dex' 		: 7,					#
        'con' 		: 11,					#
        'wis' 		: 3,					#					
        'intl' 		: 4,					#
        'chr' 		: 2,					#
        'hp' 		: 99,					# Starting hitpoints
        'status' 	: 0,					# Starting bitfield of status effects
        'head'		: -1,					# Items equipped
        'neck'		: -1,					#
        'body'		: -1,					#
        'arms'		: -1,					#
        'legs'		: -1,					#
        'hand_r'	: 1,					# Weapon held in right hand
        'hand_l'	: -1,					# Weapon held in left hand
	},
	2 : {
        'name' 		: "Wild Dog",     		
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
	},
}

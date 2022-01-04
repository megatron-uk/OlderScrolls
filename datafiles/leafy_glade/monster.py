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
		"HUMAN_UNTRAINED" : 0x10,		
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
        'clock'		: -1,					#
        'body'		: -1,					#
        'arms'		: -1,					#
        'legs'		: -1,					#
        'feet'		: -1,					#
        'hand_r'	: 1,					# Weapon held in right hand
        'hand_l'	: -1,					# Weapon held in left hand
	},
}

""" datasettings.py, Definitions which match those values defined in
 the OlderScrolls engine - mainly from game.h, but also conditions.h,
 monsters.h and similar. 
 
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

##################################################################################
#
# This section contains definitions that MUST match those found in the following
# OlderScrolls header files:
#
# - conditions.h
# - monsters.h
# - game.h
#
# Deviating from the values defined in the OlderScrolls source code will result
# in data files loading incorrectly, if at all.
#
# Do not change these values unless you understand the consequences of the compiled
# game code and the game datafiles getting out of step with each other!
#
##################################################################################

import string

MAX_REQUIREMENTS = 8		# as per game.h
MAX_MONSTER_TYPES = 4		# as per game.h
MAX_REWARD_ITEMS = 6 		# as per game.h
MAX_LEVEL_NAME_SIZE = 32	# as per game.h
MAX_STORY_TEXT_SIZE = 1024	# as per game.h
MAX_LOCATIONS = 256			# as per game.h
MAX_PLAYER_NAME = 18		# as per game.h
MAX_SHORT_NAME = 6			# as per game.h
MAX_CHARACTERS = 256		# as per game.h
MAX_BMP_FILENAME = 8		# Limit filenames to 8 characters and no '.' to be valid on all targets
MAX_SPELLS = 5
ALLOWED_FILENAME_CHARS = string.ascii_lowercase + string.digits + "_"
BMP_SOURCES	= "/bmp/"		# bitmap images for this dataset should be within
							# a sub-directory of the adventure folder, named '/bmp/master'
							# e.g. ./leafy_glade/bmp/master
							#
							# Target-specific bmp datafiles will then be automatically
							# placed in ./leafy_glade/bmp/ql, ./leafy_glade/bmp/atarixl, etc.
							# 
							# Output datafiles for running the game will be placed in
							# ./leafy_glade/out/ql, ./leafy_glade/out/msx, etc.

OUT_DIR	= "/out/"

##################################################################################
#
# A list of the target systems we can build the datafiles for
#
##################################################################################

AVAILABLE_TARGETS = {
	'1' : { 
			'target'	: 'Sinclair QL',
			'res'		: '512x256 4 colour',
			'suffix'	: 'ql',
	},
}

##################################################################################
#
# Condition checks and their available options
# 
##################################################################################
CONDITION_RULES = {
	"COND_EVAL_EMPTY"	: 0x00,	# The condition set is EMPTY, do not attempt to evaluate
	"COND_EVAL_AND"		: 0x10,	# All conditions must evaluate to true. The default.
	"COND_EVAL_OR"		: 0x20,	# At least one condition must evaluate to true.
	"COND_EVAL_NOR"		: 0x30,	# All conditions must evaluate to false.
	"COND_EVAL_NAND"	: 0x40,	# Zero or one, (but no more than one) condition must evaluate to true.
}

CONDITIONS_PC_TYPE = {
	"COND_TYPE_STR" 	:		0x01,	# Check on PC or Party STR attribute
	"COND_TYPE_DEX" 	:		0x02,	# Check on PC or Party DEX attribute
	"COND_TYPE_CON" 	:		0x03,	# Check on PC or Party CON attribute
	"COND_TYPE_WIS" 	:		0x04,	# Check on PC or Party WIS attribute
	"COND_TYPE_INT" 	:		0x06,	# Check on PC or Party INT attribute
	"COND_TYPE_CHR" 	:		0x07,	# Check on PC or Party CHR attribute
	"COND_TYPE_HP" 		:		0x08,	# Check on PC or Party HP attribute
	"COND_TYPE_GOLD" 	:		0x09,	# Check on PC or Party GOLD attribute
	"COND_TYPE_DMG" 	:		0x0A,	# Check on PC or Party DMG attribute
	"COND_TYPE_INJURY" 	:		0x0B,	# Check on PC or Party INJURY attribute
}

PARTY_MEMBER_STATUS = {
	"COND_PARTY_PRESENT" 	: 0x01, # The given party member 'x' is present in the party
	"COND_PARTY_DIMISSED" 	: 0x02, # The given party member 'x' has been recruited, but is not currently in the party 
	"COND_PARTY_DEAD"		: 0x03, # The given party member 'x' has the status 'dead'
}

VISIT_TYPE = {
	"COND_MAP_VISIT_TYPE"		: 0x00,
	"COND_MAP_VISIT_TYPE_MAX"	: 0x01, # The given location has been visited less than or equal to this number of times
	"COND_MAP_VISIT_TYPE_MIN"	: 0x02, # The given location has been visited more than or equal to this number of times
}

MONSTER_LOCATION_TYPE = {
	"MONSTER_TYPE_PRIMARY"		: 0x01, # The primary monster spawn at this location has been encountered/defeated 'x' times
	"MONSTER_TYPE_SECONDARY"	: 0x02, # The secondary monster spawn at this location has been encountered/defeated 'x' times
}

ITEM_OWN_TYPE = {
	"COND_ITEM_OWN" 	: 	0x01, # The given item is present in the player or party inventory
	"COND_ITEM_NOTOWN" 	: 	0x02, # The given item is not present in the player or party inventory
}

WEAPON_OWN_TYPE = {
	"COND_ITEM_OWN" 	: 	0x01, # The given weapon is present in the player or party inventory
	"COND_ITEM_NOTOWN" 	: 	0x02, # The given weapon is present in the player or party inventory
}

NPC_TEST_TYPE = {
	"COND_NPC_TALK"			: 0x01,	# An NPC has been met and talked to (irrespective of times)
	"COND_NPC_ALIVE"		: 0x02,	# An NPC is still alive (if not met, assumed still alive)
	"COND_NPC_DEAD"			: 0x03,	# An NPC is dead (if not met, assumed still alive) 
	"COND_NPC_TIMER_LESS" 	: 0x04, # An NPC has been met/last talked less than or equal to 'x' turns ago
	"COND_NPC_TIMER_MORE" 	: 0x05, # An NPC has been met/last talked more than or equal to 'x' turns ago
}

ITEM_TYPE_IDS = ["w", "i"]

CONDITIONS = {
	"NO_COND" : {								# No conditions applied
		'bitfield'		: [0x00, 0x00, 0x00, 0x00, 0x00],
		'bytes' 		: 5,					# Total size of check is 5 bytes
	},
	"COND_NO_MONSTERS" : {						# No monsters currently spawned
		'bitfield'		: [0x00, 0x01, 0x00, 0x00, 0x00],
		'bytes' 		: 5,					# Total size of check is 5 bytes
	},
	"COND_PC_ATR_TYPE" 	: {						# PC has above 'x' STR/WIS/CON/etc
		'bitfield'		: [0x01, 0x00],			# Initial bitfield, 2 bytes
		'1_type'		: "CONDITIONS_PC_TYPE",	# PC Lookup, type STR
		'1_sz'			: 1,					# Param 1 is 1 bytes
		'2_type'		: "INTEGER",			# Must be an integer
		'2_sz'			: 1,					# Param 2 is 1 bytes
		'pad_sz'		: 1,					# Extra padding byte
		'bytes' 		: 5,					# Total size of check is 5 bytes
	},
	"COND_PARTY_ATR_TYPE": {					# PC has above 'x' STR/WIS/CON/etc
		'bitfield'		: [0x02, 0x00],
		'1_type'		: "CONDITIONS_PC_TYPE",	# PC Lookup, type STR
		'1_sz'			: 1,					# Param 1 is 1 bytes
		'2_type'		: "INTEGER",			# Integer parameter
		'2_sz'			: 1,					# Param 2 is 1 bytes
		'pad_sz'		: 1,					# Extra padding byte 
		'bytes' 		: 5,					# Total size of check is 5 bytes
	},
	"COND_PARTY_MEMBER_TYPE": {					# Party character presence check
		'bitfield'		: [0x03, 0x00],
		'1_type'		: "PARTY_MEMBER_ID",	# Lookup for a player party character ID
		'1_sz'			: 1,					# Param 1 is 1 bytes
		'2_type'		: "PARTY_MEMBER_STATUS",# Status ID
		'2_sz'			: 1,					# Param 2 is 1 bytes
		'pad_sz'		: 1,					# Extra padding byte 
		'bytes' 		: 5,					# Total size of check is 5 bytes
	},
	"COND_MAP_VISIT_TYPE" : {				# A check on whether a location has been visited
		'bitfield'		: [0x04],
		'1_type'		: "VISIT_TYPE",		# VISIT, VISIT_TYPE_MAX, VISIT_TYPE_MIN
		'1_sz'			: 1,				# Param 1 is 1 byte
		'2_type'		: "LOCATION_ID",	# Location ID
		'2_sz'			: 2,				# Param 2 is 2 bytes
		'3_type'		: "INTEGER",		# Number of visits
		'3_sz'			: 1,				# Param 3 is 1 byte
		'bytes'			: 5,				# Total size if 5 bytes
	},
	"COND_MONSTER_DEFEAT_TYPE" : {
		'bitfield'		: [0x05],
		'1_type'		: "MONSTER_LOCATION_TYPE",		# MONSTER_TYPE_PRIMARY, MONSTER_TYPE_SECONDARY
		'1_sz'			: 1,				# Param 1 is 1 byte
		'2_type'		: "LOCATION_ID",	# Location ID
		'2_sz'			: 2,				# Param 2 is 2 bytes
		'3_type'		: "INTEGER",		# Number of monster defeats
		'3_sz'			: 1,				# Param 3 is 1 byte
		'bytes'			: 5,				# Total size if 5 bytes	
	},
	"COND_NPC_TYPE" : {
		'bitfield'		: [0x06],
		'1_type'		: "NPC_TEST_TYPE",
		'1_sz'			: 1,				
		'2_type'		: "NPC_ID",			# NPC ID
		'2_sz'			: 1,				# Param 1 is 1 byte
		'2_type'		: "INTEGER",		# Times talked
		'2_sz'			: 2,				# Param 2 is 2 byte
		'pad_sz'		: 0,				# Pad byte
		'bytes'			: 5,				# Total size if 5 bytes		
	},
	"COND_ITEM_TYPE" : {
		'bitfield'		: [0x07],
		'1_type'		: "ITEM_OWN_TYPE",
		'1_sz'			: 1,
		'2_type'		: "ITEM_ID",
		'2_sz'			: 1,
		'3_type'		: "INTEGER",
		'3_sz'			: 1,
		'pad_sz'		: 1,
		'bytes'			: 5,
	},
	"COND_WEAPON_TYPE" : {
		'bitfield'		: [0x08],
		'1_type'		: "WEAPON_OWN_TYPE",
		'1_sz'			: 1,
		'2_type'		: "WEAPON_ID",
		'2_sz'			: 1,
		'3_type'		: "INTEGER",
		'3_sz'			: 1,
		'pad_sz'		: 1,
		'bytes'			: 5,
	}
	
}

# These character types should match
# the definitions in data.h - and are used to
# determine when a character is instantiated,
# which data set the attributes are loaded 
# from (NPC.dat, MONSTER.dat etc)
CHARACTER_TYPES = {
	"CHARACTER_TYPE_MONSTER" 	: 0,
	"CHARACTER_TYPE_NPC" 		: 1,
	"CHARACTER_TYPE_BOSS" 		: 2,
}

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

# These classes should match the definitions
# of the possible sprite sizes as found in the
# OlderScrolls engine 'draw.h"
SPRITE_CLASSES = {
	"SPRITE_CLASS_NONE"			: 0,
	"SPRITE_CLASS_NORMAL"		: 1,
	"SPRITE_CLASS_LARGE"		: 2,
	"SPRITE_CLASS_PORTRAIT" 	: 3,
}

SPRITE_SIZES = {
	"SPRITE_CLASS_NORMAL" 		: { 'w' : 32, 'h' : 32, 'file' : 'sprite.dat' },
	"SPRITE_CLASS_LARGE" 		: { 'w' : 96, 'h' : 96, 'file' : 'boss.dat' },
	"SPRITE_CLASS_PORTRAIT" 	: { 'w' : 32, 'h' : 32, 'file' : 'portrait.dat' },
}

SPRITE_ANIMATION_FRAMES = 4

MAX_STATS = {
	'str' 	: { 'min' : 1, 'max' : 20 },
	'dex' 	: { 'min' : 1, 'max' : 20 },
	'con' 	: { 'min' : 1, 'max' : 20 },
	'wis' 	: { 'min' : 1, 'max' : 20 },
	'intl' 	: { 'min' : 1, 'max' : 20 },
	'chr' 	: { 'min' : 1, 'max' : 20 },
	'hp' 	: { 'min' : 1, 'max' : 255 },
	'level' : { 'min' : 0, 'max' : 10 },
}

ITEM_TYPES = {
	'ITEM_TYPE_CONSUMEABLE' 	: 1,	# Something that can be used (potion, food, etc)
	'ITEM_TYPE_ARMOUR'			: 2,	# Something that can be equipped to an armour slot
	'ITEM_TYPE_SPELL'			: 4,	# Something that can be used to cast (one or more) spell(s)
	'ITEM_TYPE_GENERIC'			: 8,	# Nothing specific, can be sold for gold at traders
	'ITEM_TYPE_QUEST'			: 16,	# A quest item 
}

ITEM_SLOT_TYPES = {
	'SLOT_TYPE_NONE' : {	'val' : 0, 'slots' : [] },
	'SLOT_TYPE_HEAD' : {	'val' : 1, 'slots' : ['head'] },
	'SLOT_TYPE_BODY' : {	'val' : 4, 'slots' : ['body'] },
	'SLOT_TYPE_OPTION' : {	'val' : 8, 'slots' : ['option'] },
}

ARMOUR_TYPES = {
	'ARMOUR_TYPE_LIGHT' 	: 1,
	'ARMOUR_TYPE_MEDIUM'	: 2,
	'ARMOUR_TYPE_HEAVY'		: 4,
}

SLOT_NAMES = ["head", "body", "option"]
WEAPON_SLOTS = ["weapon_r", "weapon_l"]

EFFECT_TYPES = {
	'EFFECT_TYPE_NONE'		: 0,	
}

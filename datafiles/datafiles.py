#!/usr/bin/env python3

""" datafiles.py, Python functions to generate target system datafiles
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
import os
import copy
import sys
import string
import traceback
from PIL import Image	# For verifying sprite bitmap dimensions and colour depth

DEBUG =1

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
##################################################################################

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
ALLOWED_FILENAME_CHARS = string.ascii_lowercase + string.digits + "_"
BMP_SOURCES	= "/bmp/"		# bitmap images for this dataset should be within
							# a sub-directory of the adventure folder, named '/bmp/'
							# e.g. ./leafy_glade/bmp/
							# Target-specific bmp datafiles will then be automatically
							# placed in ./leafy_glade/out/ql, ./leafy_glade/out/atarixl, etc.
							# alongside the normal datafiles.

OUT_DIR	= "/out/"

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
	"SPRITE_CLASS_NORMAL" 		: { 'w' : 32, 'h' : 32 },
	"SPRITE_CLASS_LARGE" 		: { 'w' : 96, 'h' : 96 },
	"SPRITE_CLASS_PORTRAIT" 	: { 'w' : 32, 'h' : 32 },
}

MAX_STATS = {
	'str' 	: { 'min' : 1, 'max' : 20 },
	'dex' 	: { 'min' : 1, 'max' : 20 },
	'con' 	: { 'min' : 1, 'max' : 20 },
	'wis' 	: { 'min' : 1, 'max' : 20 },
	'intl' 	: { 'min' : 1, 'max' : 20 },
	'chr' 	: { 'min' : 1, 'max' : 20 },
	'hp' 	: { 'min' : 1, 'max' : 255 },
	'level' : { 'min' : 1, 'max' : 10 },
}

ITEM_TYPES = {
	'ITEM_TYPE_CONSUMEABLE' 	: 1,
	'ITEM_TYPE_ARMOUR'			: 2,
	'ITEM_TYPE_SPELL'			: 4,
	'ITEM_TYPE_QUEST'			: 8,
}

ITEM_SLOT_TYPES = {
	'SLOT_TYPE_NONE'			: 0,
	'SLOT_TYPE_HEAD'			: 1,
	'SLOT_TYPE_NECK'			: 2,
	'SLOT_TYPE_BODY'			: 4,
	'SLOT_TYPE_ARMS'			: 8,
	'SLOT_TYPE_LEGS'			: 16,
	'SLOT_TYPE_HAND'			: 32,
}


##################################################################################
#
# Functions to parse and sanity-check an adventure start here...
#
##################################################################################

def filename_is_valid(filename = None):
	""" Check that a filename matches the criteria to work on all targets """
	
	if "." in filename:
		return False
	
	for f in filename:
		if f not in ALLOWED_FILENAME_CHARS:
			return False
			
	return True
	

def generate_characters(import_dir = None):
	""" Generates party, npc and monster datafiles from a monster.py file """
	
	print("")
	print("*** Parsing Character Data ***")
	
	try:
		game_characters = module = __import__(import_dir + ".monster", globals(), locals(), ["MONSTER"])
		game_items = module = __import__(import_dir + ".items", globals(), locals(), ["ITEMS"])
		game_weapons = module = __import__(import_dir + ".weapons", globals(), locals(), ["WEAPON"])
	except Exception as e:
		print("Error: %s" % e)
		traceback.print_exc(file=sys.stdout)
		return False
	
	party_ids = list(game_characters.PARTY.keys())
	party_ids.sort()
	
	npc_ids = list(game_characters.NPC.keys())
	npc_ids.sort()
	
	monster_ids = list(game_characters.MONSTER.keys())
	monster_ids.sort()
	
	item_ids = list(game_items.ITEMS.keys())
	item_ids.sort()
	
	weapon_ids = list(game_weapons.WEAPONS.keys())
	weapon_ids.sort()

	for char_type in ["NPC", "PARTY", "MONSTER"]:
	
		if char_type == "NPC":
			char_ids = npc_ids
			char_entries = game_characters.NPC
			
		if char_type == "PARTY":
			char_ids = party_ids
			char_entries = game_characters.PARTY
			
		if char_type == "MONSTER":
			char_ids = monster_ids
			char_entries = game_characters.MONSTER

		if len(char_ids) > 0:
			
			print("###################################################################################")
			print("#")
			print("# Step 1.")
			print("#")
			print("# Check %s characters [x%s]" % (char_type, len(char_ids)))
			print("#")
			print("###################################################################################")
			
			#
			# Check names are all within allowable length
			#
			
			print("")
			print("Check 1a: Checking %s name string lengths..." % char_type)
		
			valid = True
			for char_id in char_ids:
				l = len(char_entries[char_id]['name']) 
				if l <= MAX_PLAYER_NAME:
					if DEBUG:
						print("%s Character ID: %3d - %s" % (char_type, char_id, char_entries[char_id]['name']))
				else:
					valid = False
					print("%s Character ID: %d" % (char_type, char_id))
					print("- ERROR: Name [%s] is more than [%s] bytes long [%s bytes]" % (char_entries[char_id]['name'], MAX_PLAYER_NAME, l))
			
			if valid:
				print("GOOD: All character names appear valid")
			else:
				return False
				
			#
			# Check we haven't hit any limits in numbers of npc/enemy/player characters
			#
			
			valid = True
			print("")
			print("Check 1b: Checking number of %s characters..." % char_type)
			l = len(char_ids)
			if l <= MAX_CHARACTERS:
				if DEBUG:
					print("%s Characters: Total %3d, OK" % (char_type, l))
				else:
					valid = False
					print("%s Characters: Total %d !!!" % (char_type, l))
					print("- ERROR: More characters defined [%d] than allowable in game logic [%d]" % (l, MAX_CHARACTERS))
			
			if valid:
				print("GOOD: All characters listed within limit")
			else:
				return False
		
			#
			# Check we have valid sprint/bitmap size defines and that bitmap source images are present
			#
		
			valid = True
			print("")
			print("Check 1c: Checking %s sprite/portrait bitmaps..." % char_type)
			
			for char_id in char_ids:
				sprite_type = char_entries[char_id]['sprite_type']
				if sprite_type in SPRITE_CLASSES:
					if DEBUG:
						print("%s Character ID: %3d, sprite class %s, OK" % (char_type, char_id, sprite_type))
				else:
					valid = False
					print("%s Character ID: %3d" % (char_type, char_id))
					print("- ERROR: This character does not have a valid sprite class defined [%s]" % sprite_type)
			
				# Check filename
					
				sprite_file = char_entries[char_id]['sprite_name']
				l = len(sprite_file)
				if l == 0:
					valid = False
					print("%s Character ID: %3d" % (char_type, char_id))
					print("- ERROR: This character does not have a valid sprite filename defined [%s]" % sprite_file)					
				elif (l > MAX_BMP_FILENAME):
					valid = False
					print("%s Character ID: %3d" % (char_type, char_id))
					print("- ERROR: This character has a sprite filename [%s] longer than [%s] bytes" % (sprite_file, l))
				elif (l <= MAX_BMP_FILENAME) and (l > 0):
					if filename_is_valid(sprite_file):
						if DEBUG:
							print("%s Character ID: %3d, sprite filename [%s], OK" % (char_type, char_id, sprite_file))
					else:
						valid = False
						print("%s Character ID: %3d" % (char_type, char_id))
						print("- ERROR: This character has an invalid sprite filename [%s]" % (sprite_file))
						print("- ERROR: Allowed filename bytes are: [%s]" % ALLOWED_FILENAME_CHARS)
						print("- ERROR: Allowed filename length is: [%s]" % MAX_BMP_FILENAME)				
		
				# Check file is present!
				if os.path.exists(import_dir + BMP_SOURCES + sprite_file):
					if DEBUG:
						print("%s Character ID: %3d, sprite filename [%s] found, OK" % (char_type, char_id, sprite_file))
						
					# Check file dimensions match the class stated for this character (e.g. 32x32 or whatever)
					try:
						img = Image.open(import_dir + BMP_SOURCES + sprite_file)
						my_sizes = SPRITE_SIZES[sprite_type]
						if (img.width == my_sizes['w']) and (img.height == my_sizes['h']):
							if DEBUG:
								print("%s Character ID: %3d, sprite dimensions [%sx%s] match class [%s], OK" % (char_type, char_id, img.width, img.height, sprite_type))
						else:
							valid = False
							print("%s Character ID: %d" % (char_type, char_id))
							print("- ERROR: This character has an invalid sprite size of [%sx%s] which does not match class [%s]" % (img.width, img.height, sprite_type))
							print("- ERROR: Allowed dimensions for this sprite class are [%sx%s]" % (my_sizes['w'], my_sizes['h']))
						img.close()
					except Exception as e:
						valid = False
						print("%s Character ID: %3d" % (char_type, char_id))
						print("- ERROR: This character has an sprite filename which could not be opened [%s]" % sprite_file)
				else:
					valid = False
					print("%s Character ID: %3d" % (char_type, char_id))
					print("- ERROR: This character has a sprite filename which was not found [%s]" % sprite_file)
					print("- ERROR: Sprite files for this adventure should be found in: [%s]" % (import_dir + BMP_SOURCES))
		
				print("-")
		
			if valid:
				print("GOOD: All characters have valid sprite classes and bitmaps are present")
			else:
				return False
			
			valid = True
			print("")
			print("Check 1d: Checking %s item numbers and slot acceptance..." % char_type)
			
			valid = True
			print("")
			print("Check 1e: Checking %s weapon numbers and class suitability..." % char_type)
				
			valid = True
			print("")
			print("Check 1f: Checking %s stats..." % char_type)
				
			for char_id in char_ids:
				for stat_type in MAX_STATS.keys():
					stat_min = MAX_STATS[stat_type]['min']
					stat_max = MAX_STATS[stat_type]['max']
					if stat_type in char_entries[char_id].keys():
						stat_value = char_entries[char_id][stat_type]
						if (stat_value >= stat_min) and (stat_value <= stat_max):
							if DEBUG:
								print("%s Character ID: %3d, stat [%5s] value [%3s], OK" % (char_type, char_id, stat_type, stat_value))
						else:
							valid = False
							print("%s Character ID: %3d" % (char_type, char_id))
							print("- ERROR: This character has an out of bounds stat entry for [%s], [%s]" % (stat_type, stat_value))
							print("- ERROR: This stat should be present and in the range [%s - %s]" % (stat_min, stat_max))
					else:
						valid = False
						print("%s Character ID: %3d" % (char_type, char_id))
						print("- ERROR: This character has no stat entry for [%s]" % stat_type)
						print("- ERROR: This stat should be present and in the range [%s - %s]" % (stat_min, stat_max))
				print("-")
			if valid:
				print("GOOD: All characters have valid stat entries")
			else:
				return False
		
				

def generate_story(import_dir = None):
	""" Generates a story/game world location datafile from a map.py file """
	
	print("")
	print("*** Parsing Story text ***")
	
	try:
		game_story = module = __import__(import_dir + ".story", globals(), locals(), ["STORY"])
	except Exception as e:
		print("Error: %s" % e)
		traceback.print_exc(file=sys.stdout)
		return False

	text_ids = list(game_story.STORY.keys())
	text_ids.sort()

	print("###################################################################################")
	print("#")
	print("# Step 1.")
	print("#")
	print("# Verify that we don't have any characters that are non-ASCII or outside < 32, or > 126")
	print("#")
	print("###################################################################################")
	print("")
	print("Check 1: Checking ASCII...")
	
	valid = True
	for i in text_ids:
		pos = 0
		for c in game_story.STORY[i]:
			pos += 1
			if (ord(c) < 32) or (ord(c) > 126):
				if ord(c) == 10:
					# Line feed aka \n is OK
					pass
				else:
					valid = False
					print("Story Text ID: %d" % i)
					print("- ERROR: Character [%c] at pos %d is outside allowable ASCII range" % (c, pos))
				
	if valid:
		print("GOOD: All text appears valid")			
	else:
		return False
		
	print("###################################################################################")
	print("#")
	print("# Step 2.")
	print("#")
	print("# Check string lengths")
	print("#")
	print("###################################################################################")
	print("")
	print("Check 2: Checking string lengths...")
	
	valid = True
	for i in text_ids:
		if len(game_story.STORY[i]) < MAX_STORY_TEXT_SIZE:
			pass
		else:
			print("Story Text ID: %d" % i)
			print("- ERROR: Text is more than [%s] bytes long [%s bytes]" % (MAX_STORY_TEXT_SIZE, len(game_story.STORY[i])))
	if valid:
		print("GOOD: All text appears valid")			
	else:
		return False	
		
	print("")
	print("###################################################################################")
	print("#")
	print("# Step 3.")
	print("#")
	print("# Write game data files")
	print("#")
	print("###################################################################################")
	print("")
	print("Pass 1: Writing data...")
		
	offset = 0
	records = []
	for i in text_ids:
		new_record = {
			'id' : i,
			'data' : [],
			'size' : 0,
			'offset' : 0,
		}
		new_record['data'] = bytes(game_story.STORY[i].encode('ascii'))			
		new_record['size'] = len(new_record['data'])
		new_record['offset'] = offset
		records.append(new_record)
		offset += new_record['size']
		
	try:
		f = open(import_dir + OUT_DIR + "/story.dat", "wb")
		offset = 0
		for record in records:
			print("- ID: %3d at offset %5d" % (record['id'], record['offset']))
			f.write(record['data'])
		f.close()
	except Exception as e:
		print("ERROR! Unable to write datafile")
		print("ERROR! %s" % e)
		return False
	print("...done!")
	
	print("")
	print("Pass 2: Writing Index")
	try:
		f = open(import_dir + OUT_DIR + "/story.idx", "wb")
		for record in records:
			index = []
			index += record['size'].to_bytes(2, byteorder='big')
			index += record['offset'].to_bytes(4, byteorder='big')
			print("- Record at offset %5d is %4s bytes" % (record['offset'], record['size']))
			f.write(bytes(index))
		f.close()
	except Exception as e:
		print("ERROR! Unable to write header file")
		print("ERROR! %s" % e)
		return False
	print("...done!")
		

def generate_world(import_dir = None):
	""" Generates a story/game world location datafile from a map.py file """
	
	print("")
	print("*** Parsing World Data ***")
	
	try:
		game_world = module = __import__(import_dir + ".world", globals(), locals(), ["MAP"])
		game_story = module = __import__(import_dir + ".story", globals(), locals(), ["STORY"])
		game_monsters = module = __import__(import_dir + ".monster", globals(), locals(), ["MONSTER"])
		game_items = module = __import__(import_dir + ".items", globals(), locals(), ["ITEMS"])
		game_weapons = module = __import__(import_dir + ".weapons", globals(), locals(), ["WEAPON"])
	except Exception as e:
		print("Error: %s" % e)
		traceback.print_exc(file=sys.stdout)
		return False
	
	location_ids = list(game_world.MAP.keys())
	location_ids.sort()
	
	text_ids = list(game_story.STORY.keys())
	text_ids.sort()
	
	monster_ids = list(game_monsters.MONSTER.keys())
	monster_ids.sort()
	
	npc_ids = list(game_monsters.NPC.keys())
	npc_ids.sort()
	
	item_ids = list(game_items.ITEMS.keys())
	item_ids.sort()
	
	weapon_ids = list(game_weapons.WEAPONS.keys())
	weapon_ids.sort()
	
	# TO DO!!!
	player_ids = []
	
	print("")
	print("###################################################################################")
	print("#")
	print("# Step 1.")
	print("#")
	print("# Verify that all exits defined in each location actually point to a real location")
	print("#")
	print("###################################################################################")
	print("")
	print("Check 1: Finding routes...")
	
	valid = True
	for i in location_ids:
		#print(game_world.MAP[i]['name'])
		for direction in ["north", "south", "east", "west"]:
			if game_world.MAP[i][direction] > 0:
				destination_id = game_world.MAP[i][direction]
				if destination_id in location_ids:
					#print("- %s: %s" % (direction, game_world.MAP[destination_id]['name']))
					pass
				else:
					print(game_world.MAP[i]['name'])
					print("- %s: INVALID DESTINATION ID %s" % (direction, destination_id))
					valid = False
			else:
				#print("- %s: -" % (direction))
				pass
	if valid:
		print("GOOD: All locations have valid exit destinations.")
	else:
		print("ERROR: At least one location has an invalid exit destination.")
		print("ERROR: Please review the exit destinations.")
		return False
		
	print("")
	print("###################################################################################")
	print("#")
	print("# Step 2.")
	print("#")
	print("# Verify that all text pointers really exist")
	print("#")
	print("###################################################################################")
		
	print("")
	print("Check 2: Verifying strings...")
	
	valid = True
	for i in location_ids:
		#print(game_world.MAP[i]['name'])
		for text_type in ["north_text", "south_text", "east_text", "west_text", "text_spawn", "text_after_spawn", "text_respawn", "text_after_respawn", "npc1_text", "npc2_text"]:
			if game_world.MAP[i][text_type] > 0:
				text_id = game_world.MAP[i][text_type]
				if text_id in text_ids:
					#print("- %s: Text ID %s ok" % (text_type, text_id))
					pass
				else:
					print(game_world.MAP[i]['name'])
					print("- %s: MISSING TEXT ID %s" % (text_type, text_id))
					valid = False
	if valid:
		print("GOOD: All locations have valid text IDs")
	else:
		print("ERROR: At least one location has an invalid text ID.")
		print("ERROR: Please review the story text IDs.")
		return False
	
	print("")
	print("###################################################################################")
	print("#")
	print("# Step 3.")
	print("#")
	print("# Check that all monster spawn chances and monster IDs are valid")
	print("#")
	print("###################################################################################")
		
	print("")
	print("Check 3: Verifying monster encounters...")
	
	valid = True
	for i in location_ids:
		#print(game_world.MAP[i]['name'])
		for encounter in ["spawn", "respawn"]:
			encounter_chance = encounter + "_chance"
			encounter_list = encounter + "_list"
			encounter_chance_pc = game_world.MAP[i][encounter_chance]
			if encounter_chance_pc > 0:
				if (encounter_chance_pc >= 0) and (encounter_chance_pc <= 100):
					#print("- %s: Chance %s%%" % (encounter_chance, encounter_chance_pc))
					pass
				else:
					print(game_world.MAP[i]['name'])
					print("- %s: INVALID ENCOUNTER CHANCE %s%%" % (encounter_chance, encounter_chance_pc))
					valid = False
					
				for monster_id in game_world.MAP[i][encounter_list]:
					if monster_id in monster_ids:
						#print("- %s: %s" % (encounter_list, game_monsters.MONSTER[monster_id]['name']))
						pass
					else:
						print(game_world.MAP[i]['name'])
						print("- %s: INVALID MONSTER %s" % (encounter_list, monster_id))
						valid = False
							
	if valid:
		print("GOOD: All monster encounters are valid")
	else:
		print("ERROR: At least one monster encounter has an invalid spawn rate or monster ID.")
		print("ERROR: Please review the spawn chance and monster IDs.")
		return False
	
	print("")
	print("###################################################################################")
	print("#")
	print("# Step 4.")
	print("#")
	print("# Check that are conditions/requires are valid and (where possible) that the")
	print("# values to be looked up are present.")
	print("#")
	print("###################################################################################")
		
	print("")
	print("Check 4: Verifying requirement conditions...")
	
	valid = True
	for i in location_ids:
		#print(game_world.MAP[i]['name'])
		
		# Check compass points
		for t in ["north", "south", "east", "west", "spawn", "respawn", "items", "npc1", "npc2"]:
			requires = t + "_require"
			if (len(game_world.MAP[i][requires]) > 0):
				n_params = len(game_world.MAP[i][requires]) 
				if n_params >= 3:
					eval_type = game_world.MAP[i][requires][0]
					eval_length = game_world.MAP[i][requires][1]
					if n_params > 3:
						eval_conds = game_world.MAP[i][requires][2:]
					else:
						eval_conds = [game_world.MAP[i][requires][2]]
					#print("- %s: requires %s, with %s condition(s)" % (requires, eval_type, eval_length))
					if eval_length != len(eval_conds):
						print(game_world.MAP[i]['name'])
						print("- %s: requires %s, with %s condition(s)" % (requires, eval_type, eval_length))
						print("- INVALID NUMBER OF CONDITIONS [%s set != %s found]" % (eval_length, len(eval_conds)))
						valid = False
					else:
						for cond in eval_conds:
							#print("-- %s" % (cond))
							bitfield = evaluate_condition(location_ids, text_ids, monster_ids, npc_ids, item_ids, weapon_ids, player_ids, cond)
							if bitfield:
								pass
							else:
								print(game_world.MAP[i]['name'])
								print("- %s: requires %s, with %s condition(s)" % (requires, eval_type, eval_length))
								print("- %s" % (cond))
								print("- INVALID CONDITION, UNABLE TO EVALUATE")
								valid = False
				else:
					print(game_world.MAP[i]['name'])
					print("- %s: INVALID NUMBER OF PARAMETERS [%s]" % (requires, n_params))
					valid = False
			else:
				#print("- %s: -" % (requires))
				pass
			
	if valid:
		print("GOOD: All condition requirements appear valid (note we cannot check PC/Party requirements)")
	else:
		print("ERROR: At least one condition requirement is invalid.")
		print("ERROR: Please check that you have supplied the correct number of parameters")
		print("ERROR: or that the right number of conditions are listed.")
		return False
	
	print("")
	print("###################################################################################")
	print("#")
	print("# Step 5.")
	print("#")
	print("# Check any items referred to are real and exist in the game data.")
	print("#")
	print("###################################################################################")
		
	print("")
	print("Check 5: Checking item and weapon references...")
	
	valid = True
	for i in location_ids:
		#print(game_world.MAP[i]['name'])
		for i_types in game_world.MAP[i]['items_list']:
			
			if len(i_types) != 2:
				print(game_world.MAP[i]['name'])
				print("- %s: INVALID ITEM TYPE LENGTH" % (i_types))
				valid = False
			else:
				i_type = i_types[0]
				i_id = int(i_types[1:])
				if i_type not in ITEM_TYPE_IDS:
					print(game_world.MAP[i]['name'])
					print("- %s: INVALID ITEM TYPE ID [%s]" % (i_types, i_type))
					valid = False
				else:
					if i_type == "w":
						# Weapon ID
						if i_id not in weapon_ids:
							print(game_world.MAP[i]['name'])
							print("- %s: NOT A VALID WEAPON ID [%s]" % (i_types, i_id))
							valid = False
						else:
							pass
						
					elif i_type == "i":
						# Item ID
						if i_id not in item_ids:
							print(game_world.MAP[i]['name'])
							print("- %s: NOT A VALID ITEM ID [%s]" % (i_types, i_id))
							valid = False
						else:
							pass
						
					else:
						pass
	if valid:
		print("GOOD: All item and weapon references appear valid")
	else:
		print("ERROR: At least one weapon or item reference is invalid.")
		print("ERROR: Please check that you have entered the correct ID and prefixed it with the correct type 'w' or 'i'.")
		return False			

	print("")
	print("###################################################################################")
	print("#")
	print("# Step 6.")
	print("#")
	print("# Write out datafile and header.")
	print("#")
	print("###################################################################################")

	print("")
	print("*** Writing World Data ***")
	
	print("")
	print("Pass 1: Writing Records")
	
	records = []
	for i in location_ids:
		
		print("")
		print("- Processing location %s" % i)
		record = location_to_record(location_ids, text_ids, monster_ids, npc_ids, item_ids, weapon_ids, player_ids, i, game_world.MAP[i])
		if record:
			# Convert list of bytes into a single byte array
			new_record = b"".join(record)
			print("- Record requires %s bytes storage" % len(new_record))
			
			data_record = {
				'id' : i,
				'data' : new_record,
				'size' : len(new_record),
				'offset' : 0,
			}
			records.append(data_record)
		else:
			print("ERROR! Call to create location record failed")
			return False
	
	if len(records) != len(location_ids):
		print("ERROR! Not all records are present")
		return False
	
	print("")
	print("Pass 1: Writing data...")
	try:
		f = open(import_dir + OUT_DIR + "/world.dat", "wb")
		offset = 0
		for record in records:
			record['offset'] = offset
			print("- ID: %3d at offset %5d" % (record['id'], offset))
			f.write(record['data'])
			offset += len(record['data'])
		f.close()
	except Exception as e:
		print("ERROR! Unable to write datafile")
		print("ERROR! %s" % e)
		return False
	print("...done!")
	
	print("")
	print("Pass 2: Writing Index")
	try:
		f = open(import_dir + OUT_DIR + "/world.idx", "wb")
		for record in records:
			index = []
			index += record['size'].to_bytes(2, byteorder='big')
			index += record['offset'].to_bytes(4, byteorder='big')
			print("- Record at offset %5d is %4d bytes" % (record['offset'], record['size']))
			f.write(bytes(index))
		f.close()
	except Exception as e:
		print("ERROR! Unable to write header file")
		print("ERROR! %s" % e)
		return False
	print("...done!")

def location_to_record(location_ids, text_ids, monster_ids, npc_ids, item_ids, weapon_ids, player_ids, location_id, location):
	""" Turn a location into a list of bytes that can be written to disk """
	
	total_conditions = 0
	
	record = []
	
	print("-- Location: %s" % location['name'])
	
	######################################################
	# 1. (2 bytes) ID of location
	######################################################
	byte_list = location_id.to_bytes(2, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	print("-- +%2s bytes, ID" % len(byte_list))
	
	######################################################
	# (2 bytes) ID of primary text block
	######################################################
	if location['text'] > 0:
		byte_list = location['text'].to_bytes(2, byteorder='big')
	else:
		byte_list = (0).to_bytes(2, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	print("-- +%2s bytes, primary text ID" % len(byte_list))
	
	
	######################################################
	# 2. (32 bytes) Name
	######################################################
	byte_list = []
	if len(location['name']) > MAX_LEVEL_NAME_SIZE:
		print("ERROR! Level name [%s] for location [%d] is greater than %d bytes!!!" % (location['name'], location_id, MAX_LEVEL_NAME_SIZE))
		return False
	tmp = "%s" % (location['name'])
	for c in tmp:
		byte_list.append(ord(c).to_bytes(1, byteorder='big'))
	for i in range(len(byte_list), MAX_LEVEL_NAME_SIZE):
		byte_list.append(0x00.to_bytes(1, byteorder='big'))
	for b in byte_list:
		record.append(b)
	print("-- +%2s bytes, name" % len(tmp))
	
	######################################################
	# 3. exits	
	######################################################
	for direction in ["north", "south", "east", "west"]:
		
		compass_text = direction + "_text"
		compass_require = direction + "_require"
		
		compass_id = None
		compass_text_id = None
		compass_requires = []
		
		# 3a. (2 bytes) ID of exit
		if location[direction] > 0:
			compass_id = location[direction].to_bytes(2, byteorder='big')
		else:
			compass_id = (0).to_bytes(2, byteorder='big')
		
		# 3b. (2 bytes) ID of text for exit
		if location[compass_text] > 0:
			compass_text_id = location[compass_text].to_bytes(2, byteorder='big')
		else:
			compass_text_id = (0).to_bytes(2, byteorder='big')
		
		# 3c. (2 bytes or variable) requirement for exit
		if len(location[compass_require]) == 0:
			# 1 byte - Empty
			compass_requires.append(CONDITION_RULES["COND_EVAL_EMPTY"].to_bytes(1, byteorder='big'))
			# 1 byte - 0 conditions are to follow
			compass_requires.append((0).to_bytes(1, byteorder='big'))
		else:
			# 1 byte - Evaluation rule
			compass_requires.append(CONDITION_RULES[location[compass_require][0]].to_bytes(1, byteorder='big'))
			# 1 byte - Number of conditions to follow
			compass_requires.append(location[compass_require][1].to_bytes(1, byteorder='big'))
			# 5 bytes - Per condition
			for cond in location[compass_require][2:]:
				total_conditions += 1
				cond_record = evaluate_condition(location_ids, text_ids, monster_ids, npc_ids, item_ids, weapon_ids, player_ids, cond)
				if len(cond_record) != 5:
					print("ERROR! Condition record for %s did not convert to 5 bytes!!!" % compass_require)
					print("ERROR! %s" % cond_record)
					return False
				for c in cond_record:
					compass_requires.append(c.to_bytes(1, byteorder='big'))
					
		# Add the id of the exit
		for b in compass_id:
			record.append(b.to_bytes(1, byteorder='big'))
		print("-- +%2s bytes, %s exit ID"  % (len(compass_id), direction))
		
		# Add the id of the text shown for this exit
		for b in compass_text_id:
			record.append(b.to_bytes(1, byteorder='big')) 
		print("-- +%2s bytes, %s exit text ID"  % (len(compass_text_id), direction))
		
		# Add the list of conditions
		record += compass_requires
		total_condition_bytes = 0
		for c in compass_requires:
			total_condition_bytes += len(c)
		print("-- +%2s bytes, %s condition requirements"  % (total_condition_bytes, direction))
	
	######################################################
	# 4. Spawn categories
	######################################################
	for spawn in ["spawn", "respawn"]:
		
		spawn_chance = spawn + "_chance"
		spawn_list = spawn + "_list"
		spawn_require = spawn + "_require"
		
		spawn_chance_byte = None
		spawn_list_bytes = []
		spawn_require_bytes = []
		
		# 4a. (1 byte) Spawn chance
		if location[spawn_chance] > 0:
			spawn_chance_byte = location[spawn_chance].to_bytes(1, byteorder='big')
		else:
			spawn_chance_byte = (0).to_bytes(1, byteorder='big')
		
		# 4b. (1 byte or 1 + variable bytes)
		total_monsters = len(location[spawn_list])
		if total_monsters == 0:
			# 1 byte - no monsters
			spawn_list_bytes.append((0).to_bytes(1, byteorder='big'))
		else:
			# 1 byte + length of list - more than 0 monsters
			if total_monsters > 255:
				print("ERROR! More monsters defined than available in 8bit value! for '%s'" % spawn_list)
				print("ERROR! %s" % total_monsters)
				return False
			spawn_list_bytes.append(total_monsters.to_bytes(1, byteorder='big'))
			for m in location[spawn_list]:
				spawn_list_bytes.append(m.to_bytes(1, byteorder='big'))
			
		# 4c. Monster spawn requirement
		if len(location[spawn_require]) == 0:
			# 1 byte - Empty
			spawn_require_bytes.append(CONDITION_RULES["COND_EVAL_EMPTY"].to_bytes(1, byteorder='big'))
			# 1 byte - 0 conditions are to follow
			spawn_require_bytes.append((0).to_bytes(1, byteorder='big'))
		else:
			# 1 byte - Evaluation rule
			spawn_require_bytes.append(CONDITION_RULES[location[spawn_require][0]].to_bytes(1, byteorder='big'))
			# 1 byte - Number of conditions to follow
			spawn_require_bytes.append(location[spawn_require][1].to_bytes(1, byteorder='big'))
			# 5 bytes - Per condition
			for cond in location[spawn_require][2:]:
				total_conditions += 1
				cond_record = evaluate_condition(location_ids, text_ids, monster_ids, npc_ids, item_ids, weapon_ids, player_ids, cond)
				if len(cond_record) != 5:
					print("ERROR! Condition record for '%s' did not convert to 5 bytes!!!" % spawn_require)
					print("ERROR! %s" % cond_record)
					return False
				for c in cond_record:
					spawn_require_bytes.append(c.to_bytes(1, byteorder='big'))
					
		# Add the id of the exit
		record.append(spawn_chance_byte)
		print("-- +%2s bytes, %s chance"  % (len(spawn_chance_byte), spawn_chance))
		
		# Add the id of the text shown for this exit
		for b in spawn_list_bytes:
			record.append(b)
		print("-- +%2s bytes, %s list"  % (len(spawn_list_bytes), spawn_list))
		
		# Add the list of conditions
		record += spawn_require_bytes
		total_condition_bytes = 0
		for c in spawn_require_bytes:
			total_condition_bytes += len(c)
		print("-- +%2s bytes, %s condition requirements"  % (total_condition_bytes, spawn_require))

	######################################################
	# 5. Items
	######################################################
	
	######################################################
	# 5a. (1 byte) Item spawning chance
	######################################################
	if location['items_chance'] > 0:
		record.append(location['items_chance'].to_bytes(1, byteorder='big'))
	else:
		record.append((0).to_bytes(1, byteorder='big'))
	print("-- + 1 byte, item chance"  )
	
	######################################################
	# 5b. (1 byte or 1 + variable bytes)
	######################################################
	total_items = len(location['items_list'])
	byte_list = []
	if total_items == 0:
		# 1 byte - no items
		byte_list.append((0).to_bytes(1, byteorder='big'))
	else:
		# 1 byte + length of list - more than 0 items
		if total_items > 255:
			print("ERROR! More items defined than available in 8bit value!")
			print("ERROR! %s" % total_items)
			return False
		byte_list.append(total_items.to_bytes(1, byteorder='big'))
		for m in location['items_list']:
			for c in m:
				byte_list.append(ord(c).to_bytes(1, byteorder='big'))
	for b in byte_list:
		record.append(b)
	print("-- +%2s byte, item list" % (len(byte_list)))

	######################################################
	# 5c. Item spawn requirements
	######################################################
	item_spawn_require_bytes = []
	if len(location['items_require']) == 0:
		# 1 byte - Empty
		item_spawn_require_bytes.append(CONDITION_RULES["COND_EVAL_EMPTY"].to_bytes(1, byteorder='big'))
		# 1 byte - 0 conditions are to follow
		item_spawn_require_bytes.append((0).to_bytes(1, byteorder='big'))
	else:
		# 1 byte - Evaluation rule
		item_spawn_require_bytes.append(CONDITION_RULES[location['items_require'][0]].to_bytes(1, byteorder='big'))
		# 1 byte - Number of conditions to follow
		item_spawn_require_bytes.append(location['items_require'][1].to_bytes(1, byteorder='big'))
		# 5 bytes - Per condition
		for cond in location['items_require'][2:]:
			total_conditions += 1
			cond_record = evaluate_condition(location_ids, text_ids, monster_ids, npc_ids, item_ids, weapon_ids, player_ids, cond)
			if len(cond_record) != 5:
				print("ERROR! Condition record for '%s' did not convert to 5 bytes!!!" % spawn_require)
				print("ERROR! %s" % cond_record)
				return False
			for c in cond_record:
				item_spawn_require_bytes.append(c.to_bytes(1, byteorder='big'))
				
	for b in item_spawn_require_bytes:
		record.append(b)
	print("-- +%2s bytes, item condition requirements" % (len(item_spawn_require_bytes)))
	
	# 6. (2 bytes) Text shown when primary monster spawning
	if location['text_spawn'] > 0:
		byte_list = location['text_spawn'].to_bytes(2, byteorder='big')
	else:
		byte_list = (0).to_bytes(2, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	print("-- +%2s bytes, primary monster spawn text" % len(byte_list))
	
	# 7. (2 bytes) Text shown when primary monsters spawn, and are then defeated
	if location['text_after_spawn'] > 0:	
		byte_list = location['text_after_spawn'].to_bytes(2, byteorder='big')
	else:
		byte_list = (0).to_bytes(2, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	print("-- +%2s bytes, primary monster after spawn text" % len(byte_list))
	
	# 8. (2 bytes) Text shown when secondary monster spawning
	if location['text_respawn'] > 0:
		byte_list = location['text_respawn'].to_bytes(2, byteorder='big')
	else:
		byte_list = (0).to_bytes(2, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	print("-- +%2s bytes, primary monster respawn text" % len(byte_list))
	
	# 9. (2 bytes) Text shown when secondary monsters spawn, and are then defeated
	if location['text_after_respawn'] > 0:	
		byte_list = location['text_after_respawn'].to_bytes(2, byteorder='big')
	else:
		byte_list = (0).to_bytes(2, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	print("-- +%2s bytes, primary monster after respawn text" % len(byte_list))
	
	# 10. (1 byte) NPC 1
	if location['npc1'] > 0:	
		record.append(location['npc1'].to_bytes(1, byteorder='big'))
	else:
		record.append((0).to_bytes(1, byteorder='big'))
	print("-- + 1 byte, NPC 1 ID")
	
	# 11. (2 bytes to variable) Evaluation rule and requirements for NPC 1
	npc1_require_bytes = []
	if len(location['npc1_require']) == 0:
		# 1 byte - Empty
		npc1_require_bytes.append(CONDITION_RULES["COND_EVAL_EMPTY"].to_bytes(1, byteorder='big'))
		# 1 byte - 0 conditions are to follow
		npc1_require_bytes.append((0).to_bytes(1, byteorder='big'))
	else:
		# 1 byte - Evaluation rule
		npc1_require_bytes.append(CONDITION_RULES[location['npc1_require'][0]].to_bytes(1, byteorder='big'))
		# 1 byte - Number of conditions to follow
		npc1_require_bytes.append(location['npc1_require'][1].to_bytes(1, byteorder='big'))
		# 5 bytes - Per condition
		for cond in location['npc1_require'][2:]:
			total_conditions += 1
			cond_record = evaluate_condition(location_ids, text_ids, monster_ids, npc_ids, item_ids, weapon_ids, cond)
			if len(cond_record) != 5:
				print("ERROR! Condition record for 'npc1_require' did not convert to 5 bytes!!!")
				print("ERROR! %s" % cond_record)
				return False
			for c in cond_record:
				npc1_require_bytes.append(c.to_bytes(1, byteorder='big'))
	for b in npc1_require_bytes:
		record.append(b)
	print("-- +%2s bytes, NPC 1 condition requirements" % (len(npc1_require_bytes)))
	
	# 12. (2 bytes) NPC 1 text
	if location['npc1_text'] > 0:	
		byte_list = location['npc1_text'].to_bytes(2, byteorder='big')
	else:
		byte_list = (0).to_bytes(2, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	print("-- +%2s bytes, NPC 1 text" % len(byte_list))
	
	# 13. (1 byte) NPC 2
	if location['npc2'] > 0:	
		record.append(location['npc2'].to_bytes(1, byteorder='big'))
	else:
		record.append((0).to_bytes(1, byteorder='big'))
	print("-- + 1 byte, NPC 2 ID")
	
	# 14. (2 bytes to variable) Evaluation rule and requirements for NPC 2
	npc2_require_bytes = []
	if len(location['npc2_require']) == 0:
		# 1 byte - Empty
		npc2_require_bytes.append(CONDITION_RULES["COND_EVAL_EMPTY"].to_bytes(1, byteorder='big'))
		# 1 byte - 0 conditions are to follow
		npc2_require_bytes.append((0).to_bytes(1, byteorder='big'))
	else:
		# 1 byte - Evaluation rule
		npc2_require_bytes.append(CONDITION_RULES[location['npc2_require'][0]].to_bytes(1, byteorder='big'))
		# 1 byte - Number of conditions to follow
		npc2_require_bytes.append(location['npc2_require'][1].to_bytes(1, byteorder='big'))
		# 5 bytes - Per condition
		for cond in location['npc2_require'][2:]:
			total_conditions += 1
			cond_record = evaluate_condition(location_ids, text_ids, monster_ids, npc_ids, item_ids, weapon_ids, player_ids, cond)
			if len(cond_record) != 5:
				print("ERROR! Condition record for 'npc1_require' did not convert to 5 bytes!!!")
				print("ERROR! %s" % cond_record)
				return False
			for c in cond_record:
				npc2_require_bytes.append(c.to_bytes(1, byteorder='big'))
	for b in npc2_require_bytes:
		record.append(b)
	print("-- +%2s bytes, NPC 2 condition requirements" % (len(npc2_require_bytes)))
	
	# 15. (2 bytes) NPC 2 text
	if location['npc2_text'] > 0:	
		byte_list = location['npc2_text'].to_bytes(2, byteorder='big')
	else:
		byte_list = (0).to_bytes(2, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	print("-- +%2s bytes, NPC 2 text" % len(byte_list))
		
	if total_conditions > 0:
		print("-- Location record contained %s conditional requirements" % total_conditions)
		
	return record
	
	

def evaluate_condition(location_ids, text_ids, monster_ids, npc_ids, item_ids, weapon_ids, player_ids, condition_list_entry):
	""" Attempt to lookup all the elements of a condition requirement """
	
	# Condition type is always the first element of the list
	cond_type = condition_list_entry[0]
				
	if DEBUG:
		print("--- %s" % condition_list_entry)
				
	if cond_type in CONDITIONS.keys():
		
		#######################################################
		# Simple condition requirements
		#######################################################
		if cond_type in ["NO_COND", "COND_NO_MONSTERS"]:
			bitfield = []
			bitfield = CONDITIONS[cond_type]['bitfield']
			if len(bitfield) == CONDITIONS[cond_type]['bytes']:
				if DEBUG:
					print("--- OK bytestream: %s" % bitfield)
				return bitfield
			else:
				print("--- INVALID SIZE [%s != %s]" % (len(bitfield), CONDITIONS[cond_type]['bytes']))
				return False
		
		#######################################################
		# PC or Party attribute conditions
		#######################################################
		valid = True
		if cond_type in ["COND_PC_ATR_TYPE", "COND_PARTY_ATR_TYPE"]:
			# Should have 2 additional params
			n_params = len(condition_list_entry)
			if n_params == 3:
				param1 = condition_list_entry[1]	# Attribute type
				if param1 in CONDITIONS_PC_TYPE.keys():
					pass
				else:
					print("--- INVALID ATTRIBUTE: [%s]" % param1)
					valid = False
				param2 = condition_list_entry[2]	# Value
				if (param2 >= 0) and (param2 <= 255):
					pass 
				else:
					print("--- INVALID INTEGER: [%s]" % param2)
					valid = False
			else:
				valid = False
				print("--- INVALID NUMBER OF PARAMETERS [%s]" % n_params)
				
			if valid:
				bitfield = []
				bitfield += copy.deepcopy(CONDITIONS[cond_type]['bitfield'])
				bitfield += [CONDITIONS_PC_TYPE[param1]]
				bitfield += [param2]
				if 'pad_sz' in CONDITIONS[cond_type].keys():
					for i in range(0, CONDITIONS[cond_type]['pad_sz']):
							bitfield.append(0x00)
				if len(bitfield) == CONDITIONS[cond_type]['bytes']:
					if DEBUG:
						print("--- OK bytestream: %s" % bitfield)
					return bitfield
				else:
					print("--- INVALID SIZE [%s != %s]" % (len(bitfield), CONDITIONS[cond_type]['bytes']))
					return False
			else:
				return False
		
		#######################################################
		# Party membership conditions
		#######################################################
		valid = True
		if cond_type in ["COND_PARTY_MEMBER_TYPE"]:
			# Should have 2 additional params
			n_params = len(condition_list_entry)
			if n_params == 3:
				param1 = condition_list_entry[1]	# id of the playable character
				if param1 in players.keys():
					pass
				else:
					print("--- INVALID PLAYER CHARACTER ID: [%s]" % param1)
					valid = False
				param2 = condition_list_entry[2]	# tested value (present, dead, dismissed, etc)
			else:
				valid = False
				print("--- INVALID NUMBER OF PARAMETERS [%s]" % n_params)
		
		#######################################################
		# Location visit conditions
		#######################################################
		valid = True
		if cond_type in ["COND_MAP_VISIT_TYPE"]:
			# Should have 3 additional params
			n_params = len(condition_list_entry)
			if n_params == 4:
				pass
			else:
				valid = False
				print("--- INVALID NUMBER OF PARAMETERS [%s]" % n_params)
		
		#######################################################
		# Monster defeat conditions
		#######################################################
		valid = True
		if cond_type in ["COND_MONSTER_DEFEAT_TYPE"]:
			# Should have 3 additional params
			n_params = len(condition_list_entry)
			if n_params == 4:
				param1 = condition_list_entry[1]	# Attribute type
				if param1 in MONSTER_LOCATION_TYPE.keys():
					pass
				else:
					print("--- INVALID MONSTER LOCATION TYPE: [%s]" % param1)
					valid = False
				param2 = condition_list_entry[2]	# Primary/Secondary location monster type
				if (param2 in location_ids):
					pass 
				else:
					print("--- INVALID LOCATION ID: [%s]" % param2)
					valid = False
				param3 = condition_list_entry[3]	# Value
				if (param3 >= 0) and (param3 <= 255):
					pass
				else:
					print("--- INVALID INTEGER: [%s]" % param3)
					valid = False
			
			else:
				valid = False
				print("--- INVALID NUMBER OF PARAMETERS [%s]" % n_params)
				
			if valid:
				bitfield = []
				bitfield = copy.deepcopy(CONDITIONS[cond_type]['bitfield'])
				bitfield += [MONSTER_LOCATION_TYPE[param1]]
				if param2 < 256:
					bitfield += [0x00]
					bitfield += [param2]
				else:
					bitfield += (param2 >> 8) & 0xff
					bitfield += param2 & 0xff
				bitfield += [param3]
				if 'pad_sz' in CONDITIONS[cond_type].keys():
					for i in range(0, CONDITIONS[cond_type]['pad_sz']):
							bitfield.append(0x00)
				if len(bitfield) == CONDITIONS[cond_type]['bytes']:
					if DEBUG:
						print("--- OK bytestream: %s" % bitfield)
					return bitfield
				else:
					print("--- INVALID SIZE [%s != %s]" % (len(bitfield), CONDITIONS[cond_type]['bytes']))
					return False
			else:
				print("--- COND_MONSTER_DEFEAT_TYPE Not valid")
				return False
		
		#######################################################
		# NPC dialogue conditions
		#######################################################
		valid = True
		if cond_type in ["COND_NPC_TYPE"]:
			# Should have 3 additional params
			n_params = len(condition_list_entry)
			if n_params == 4:
				param1 = condition_list_entry[1]	# Condition type
				if param1 in NPC_TEST_TYPE.keys():
					param1 = NPC_TEST_TYPE[condition_list_entry[1]]
				else:
					print("--- INVALID NPC TEST TYPE: [%s]" % param1)
					valid = False
				param2 = condition_list_entry[2]	# NPC ID
				if param2 in npc_ids:
					pass
				else:
					print("--- INVALID NPC ID: [%s]" % param2)
					valid = False
				param3 = condition_list_entry[3]	# Value
				if (param3 >= 0) and (param3 <= 65535):
					pass
				else:
					print("--- INVALID INTEGER: [%s]" % param2)
					valid = False
			else:
				valid = False
				print("--- INVALID NUMBER OF PARAMETERS [%s]" % n_params)
				
			if valid:
				bitfield = []
				bitfield = copy.deepcopy(CONDITIONS[cond_type]['bitfield'])
				bitfield += [param1]
				bitfield += [param2]
				if param3 < 256:
					bitfield += [0x00]
					bitfield += [param3]
				else:
					bitfield += (param3 >> 8) & 0xff
					bitfield += param3 & 0xff
				if 'pad_sz' in CONDITIONS[cond_type].keys():
					for i in range(0, CONDITIONS[cond_type]['pad_sz']):
							bitfield.append(0x00)
				if len(bitfield) == CONDITIONS[cond_type]['bytes']:
					if DEBUG:
						print("--- OK bytestream: %s" % bitfield)
					return bitfield
				else:
					print("--- INVALID SIZE [%s != %s]" % (len(bitfield), CONDITIONS[cond_type]['bytes']))
					return False
			else:
				return False
				
		#######################################################
		# Item conditions
		#######################################################
		valid = True
		if cond_type in ["COND_ITEM_TYPE"]:
			# Should have 3 additional params
			n_params = len(condition_list_entry)
			if n_params == 4:
				param1 = condition_list_entry[1]	# Item require/not-require type
				if param1 in ITEM_OWN_TYPE.keys():
					pass
				else:
					print("--- INVALID ITEM REQUIREMENT TYPE: [%s]" % param1)
					valid = False
				param2 = condition_list_entry[2]	# Item ID
				if param2 in item_ids:
					pass
				else:
					print("--- INVALID ITEM ID: [%s]" % param2)
					valid = False
				param3 = condition_list_entry[3]	# Value
				if (param3 >= 0) and (param3 <= 255):
					pass
				else:
					print("--- INVALID INTEGER: [%s]" % param2)
					valid = False
			else:
				valid = False
				print("--- INVALID NUMBER OF PARAMETERS [%s]" % n_params)
				
			if valid:
				bitfield = []
				bitfield = copy.deepcopy(CONDITIONS[cond_type]['bitfield'])
				bitfield += [ITEM_OWN_TYPE[param1]]
				bitfield += [param2]
				bitfield += [param3]
				if 'pad_sz' in CONDITIONS[cond_type].keys():
					for i in range(0, CONDITIONS[cond_type]['pad_sz']):
							bitfield.append(0x00)
				if len(bitfield) == CONDITIONS[cond_type]['bytes']:
					if DEBUG:
						print("--- OK bytestream: %s" % bitfield)
					return bitfield
				else:
					print("--- INVALID SIZE [%s != %s]" % (len(bitfield), CONDITIONS[cond_type]['bytes']))
					return False
			else:
				return False
		
		#######################################################
		# Weapon conditions
		#######################################################
		valid = True
		if cond_type in ["COND_WEAPON_TYPE"]:
			# Should have 3 additional params
			n_params = len(condition_list_entry)
			if n_params == 4:
				param1 = condition_list_entry[1]	# Item require/not-require type
				if param1 in WEAPON_OWN_TYPE.keys():
					pass
				else:
					print("--- INVALID WEAPON REQUIREMENT TYPE: [%s]" % param1)
					valid = False
				param2 = condition_list_entry[2]	# Item ID
				if param2 in weapon_ids:
					pass
				else:
					print("--- INVALID WEAPON ID: [%s]" % param2)
					valid = False
				param3 = condition_list_entry[3]	# Value
				if (param3 >= 0) and (param3 <= 255):
					pass
				else:
					print("--- INVALID INTEGER: [%s]" % param2)
					valid = False
			else:
				valid = False
				print("--- INVALID NUMBER OF PARAMETERS [%s]" % n_params)
				
			if valid:
				bitfield = []
				bitfield = copy.deepcopy(CONDITIONS[cond_type]['bitfield'])
				bitfield += [WEAPON_OWN_TYPE[param1]]
				bitfield += [param2]
				bitfield += [param3]
				if 'pad_sz' in CONDITIONS[cond_type].keys():
					for i in range(0, CONDITIONS[cond_type]['pad_sz']):
							bitfield.append(0x00)
				if len(bitfield) == CONDITIONS[cond_type]['bytes']:
					if DEBUG:
						print("--- OK bytestream: %s" % bitfield)
					return bitfield
				else:
					print("--- INVALID SIZE [%s != %s]" % (len(bitfield), CONDITIONS[cond_type]['bytes']))
					return False
			else:
				return False
		
	else:
		return False

if __name__ == "__main__":
	
	print("OlderScrolls datafile generator")
	print("===============================")
	
	adventure = "leafy_glade"
	sys.path.append(adventure)
	
	#status = generate_world(import_dir = adventure)
	#if status is False:
	#	print("Not continuing. Please fix errors in world map file.")
	#	sys.exit(1)
		
	#status = generate_story(import_dir = adventure)
	#if status is False:
	#	print("Not continuing. Please fix errors in story file.")
	#	sys.exit(1)
		
	status = generate_characters(import_dir = adventure)
	if status is False:
		print("Not continuing. Please fix errors in monster file.")
		sys.exit(1)	
		
	#status = generate_items(import_dir = adventure)
	#if status is False:
	#	print("Not continuing. Please fix errors in item file.")
	#	sys.exit(1)
	
	#status = generate_weapons(import_dir = adventure)
	#if status is False:
	#	print("Not continuing. Please fix errors in weapon file.")
	#	sys.exit(1)
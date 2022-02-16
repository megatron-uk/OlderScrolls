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

from datasettings import *

DEBUG =0

##################################################################################
#
# Functions to parse and sanity-check an adventure start here...
#
##################################################################################

class bmp_to_target:
	""" THis is a class that processes out bitmap images to whatever target
		system we are using at the time. """

	def __init__(self, target = None):
		""" Test """
		
		self.debug = 0
		
		# The raw image
		self.bmp = None
		
		# The targer machine
		self.target = target
		
		# Decoded byte sequence ready to be written to dat file
		self.bytes = []
	
	def setbit(self, value, bit):
		return value | (1<<bit)

	def clearbit(self, value, bit):
		return value & ~(1<<bit)
	
	#################################################################
	
	def load(self, filename = None):
		""" Loads an image from disk """
		try:
			if self.debug:
				print("INFO - load() Loading [%s]" % filename)
			self.bmp = Image.open(filename)
			return True
		except Exception as e:
			print("ERROR - load() Unable to load image bitmap [%s]" % filename)
			print("ERROR - %s" % e)
			return False
	
	#################################################################
	
	def convert(self, force = False):
		""" Convert a loaded bitmap """
		
		if self.bmp is None:
			print("ERROR - convert() Bitmap is not loaded!")
			return False
		
		if target['suffix'] == 'ql':
			self.to_ql()	
			return True
			
		print("ERROR - convert() Not a valid target [%s]" % self.target['suffix'])
		return False
	
	#################################################################
	
	def out(self, force = False):
		""" Output the byte representation of the current image, suitable for
			use in a dat file for the selected target. """
			
		if self.bmp is None:
			print("ERROR - out() Bitmap is not loaded!")
			return False
		
		if target['suffix'] == 'ql':
			self.out_ql()	
			return True
			
		print("ERROR - out() Not a valid target [%s]" % self.target['suffix'])
		return False	
		
	#################################################################
		
	def to_ql(self):
		""" Convert a bitmap from the master folder into 4bpp fixed palette for QL """
		pass
		
	def out_ql(self):
		""" Turn a 4bpp QL bitmap into a string of 16bit, 8pixel words we store in a dat file """
		
		# Each byte in the 4bpp bitmap is a single pixel
		# We need to concatenate 8 pixels into one 16bit word
		# using the 4 colour mapping scheme of the QL
		pixels = self.bmp.tobytes()
		if (len(pixels) % 8) != 0:
			print("ERROR - out_ql() Bitmap is not divisible by 8!!!")
			return False
		
		# Read 8 pixels
		ql_pixel_blocks = []
		i = 7
		pixel_left = 0
		pixel_right = 0
		for px in pixels:
			
			# Colour BLACK
			if px == 0:
				pixel_left = self.clearbit(pixel_left, i)			# Set pixel in left hand byte
				pixel_right = self.clearbit(pixel_right, i)		# Set pixel in right hand byte
				
			# Colour GREEN
			if px == 1:
				pixel_left = self.setbit(pixel_left, i)
				pixel_right = self.clearbit(pixel_right, i)
				
			# Colour RED
			if px == 2:
				pixel_left = self.clearbit(pixel_left, i)
				pixel_right = self.setbit(pixel_right, i)
				
			# Colour WHITE
			if px == 3:
				pixel_left = self.setbit(pixel_left, i)
				pixel_right = self.setbit(pixel_right, i)
				
			
			if i == 0:		
				ql_pixel_blocks.append(pixel_left)
				ql_pixel_blocks.append(pixel_right)
				
				# Reset left and right pixels
				pixel_left = 0
				pixel_right = 0
				i = 7
			else:
				i -= 1
		
		for pixel_block in ql_pixel_blocks:
			self.bytes.append(pixel_block)
			
		if self.debug:
			print("INFO - out_ql() Generated %s bytes of data" % len(self.bytes))
		
		return True
			

def choose_target(adventure = None):
	""" Selects the target machine type and creates any necessary output folders """

	print("")
	print("*** Choose Deployment Target ***")
	print("")
	print("Choose the target platform that you wish to build a set of datafiles for.")
	print("This will build a target-specific set of datafiles for the adventure: %s" % adventure)
	print("")
	if len(AVAILABLE_TARGETS.keys()) < 1:
		print("ERROR - No targets available")
		return False
	else:
		print("ID | %20s | %20s | %8s" % ("Target", "Resolution", "Suffix"))
		for k in AVAILABLE_TARGETS.keys():
			my_target = AVAILABLE_TARGETS[k]
			print("%2s | %20s | %20s | %8s" % (k, my_target['target'], my_target['res'], my_target['suffix']))
			
		i = None
		while (i not in AVAILABLE_TARGETS.keys()):
			i = input()
			
		target = AVAILABLE_TARGETS[i]
		target_out_dir = adventure + OUT_DIR + target['suffix']
		if os.path.exists(target_out_dir):
			pass
		else:
			try:
				if os.path.exists(adventure + OUT_DIR):
					pass
				else:
					if DEBUG:
						print("Creating output directory [%s]" % adventure + OUT_DIR)
					os.mkdir(adventure + OUT_DIR)
				if DEBUG:
					print("Creating target output directory [%s]" % target_out_dir)
				os.mkdir(target_out_dir)
				
			except Exception as e:
				print("ERROR - Creating output directory failed")
				print("ERROR - %s" % e)
				return False
		
		return AVAILABLE_TARGETS[i]
	

def filename_is_valid(filename = None):
	""" Check that a filename matches the criteria to work on all targets """
	
	if "." in filename:
		return False
	
	for f in filename:
		if f not in ALLOWED_FILENAME_CHARS:
			return False
			
	return True
	

def generate_characters(import_dir = None, target = None):
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

	# Empty list of bitmaps that we keep
	portraits = {}
	bitmaps = {}
	for sprite_type in SPRITE_CLASSES:
		bitmaps[sprite_type] = {}

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
			
			print("")
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

				for sprite_file_class in ['resting_sprite', 'attack_sprite', 'magic_sprite', 'wounded_sprite', 'death_sprite', 'portrait_name']:
					
					#sprite_file = char_entries[char_id][sprite_file_class]

					if sprite_file_class == 'portrait_name':
						my_sizes = SPRITE_SIZES['SPRITE_CLASS_PORTRAIT']
						this_type = 'SPRITE_CLASS_PORTRAIT'
					else:
						my_sizes = SPRITE_SIZES[sprite_type]
						this_type = sprite_type

					for sprite_file in char_entries[char_id][sprite_file_class]:

						if sprite_file not in bitmaps[this_type].keys():
		
							# Only check for filenames which are not blank
							if sprite_file == "":
								print("%s Character ID: %3d, !!WARNING!! %s filename is blank, assuming no sprite? !!WARNING!!" % (char_type, char_id, sprite_file_class))
							else:
								if DEBUG:
									print("%s Character ID: %3d, %s filename [%s], OK" % (char_type, char_id, sprite_file_class, sprite_file))
					
								# Check file is present!
								found_sprite = False
								d1 = import_dir + BMP_SOURCES + target['suffix'] + "/" + sprite_file
								d2 = import_dir + BMP_SOURCES + "master/" + sprite_file
								if os.path.exists(d1):
									found_sprite = True
									d = d1
									if DEBUG:
										print("%s Character ID: %3d, %s filename [%s] found for target [%s], OK" % (char_type, char_id, sprite_file_class, sprite_file, target['target']))
								else:
									if os.path.exists(d2):
										found_sprite = True
										d = d2
										if DEBUG:
											print("%s Character ID: %3d, %s filename [%s] found in master image directory, OK" % (char_type, char_id, sprite_file_class, sprite_file))
										
								if found_sprite:
									# Check file dimensions match the class stated for this character (e.g. 32x32, 96x96 or whatever)
									try:
										img = Image.open(d)
																				
										if (img.width == my_sizes['w']) and (img.height == my_sizes['h']):
											if DEBUG:
												print("%s Character ID: %3d, %s dimensions [%sx%s] match class [%s], OK" % (char_type, char_id, sprite_file_class, img.width, img.height, this_type))
												
											b = bmp_to_target()
											b.load(d)
											if b.out():
											
												if DEBUG:
													print("%s Character ID: %3d, Adding processed bitmap to bitmaps[%s][%s]" % (char_type, char_id, this_type, sprite_file))
													print("%s Character ID: %3d, Any future reference to this bitmap will reuse this data" % (char_type, char_id))
												bitmaps[this_type][sprite_file] = { 'file' : d, 'converter' : b }	
											else:
												print("%s Character ID: %d" % (char_type, char_id))
												print("- ERROR: Conversion of [%s] to native [%s] binary data failed" % (sprite_file, target['target']))
												valid = False
											
										else:
											valid = False
											print("%s Character ID: %d" % (char_type, char_id))
											print("- ERROR: This character has an invalid %s size of [%sx%s] which does not match class [%s]" % (sprite_file_class, img.width, img.height, this_type))
											print("- ERROR: Allowed dimensions for this sprite class are [%sx%s]" % (my_sizes['w'], my_sizes['h']))
										img.close()
									except Exception as e:
										valid = False
										print("%s Character ID: %3d" % (char_type, char_id))
										print("- ERROR: This character has an %s filename which could not be opened [%s]" % (sprite_file_class, sprite_file))
										print("- ERROR: %s" % e)
								else:
									valid = False
									print("%s Character ID: %3d" % (char_type, char_id))
									print("- ERROR: This character has a %s filename which was not found [%s]" % (sprite_file_name, sprite_file))
									print("- ERROR: Sprite files for this adventure should be found in: [%s OR %s]" % ((import_dir + BMP_SOURCES + "master/"), (import_dir + BMP_SOURCES +  target['suffix'] + "/")))
								
						else:
							if DEBUG:
								print("%s Character ID: %3d, %s bitmap [%s] has already been processed" % (char_type, char_id, sprite_file_class, sprite_file))
							valid = True
				if DEBUG:
					print("-")
		
			if valid:
				print("GOOD: All characters have valid sprite classes and bitmaps are present")
			else:
				return False
			
			#
			# Check any equipped items have valid item numbers
			# and are appropriate for the class of this character
			#
			
			valid = True
			print("")
			print("Check 1d: Checking %s item numbers and slot acceptance..." % char_type)
			
			for char_id in char_ids:
				for slot_type in SLOT_NAMES:
					slot_item_id = char_entries[char_id][slot_type]
					if slot_item_id > 0:
						if slot_item_id in item_ids:
							
							# Load the item record
							item = game_items.ITEMS[slot_item_id]

							# Look up where that class of item can be slotted
							item_slot = item['slot']
							if item_slot in ITEM_SLOT_TYPES.keys():
								if slot_type in ITEM_SLOT_TYPES[item_slot]['slots']:
									if DEBUG:
										print("%s Character ID: %3d, item slot: [%6s], item id: [%3d] [%18s]" % (char_type, char_id, slot_type, slot_item_id, item['name']))
								else:
									valid = False
									print("%s Character ID: %3d, item slot: [%6s], invalid item id: [%3d]" % (char_type, char_id, slot_type, slot_item_id))
									print("- ERROR: This character has an assigned item that has to go in one of these slots: %s" % ITEM_SLOT_TYPES[item_slot]['slots'])
							else:
								valid = False
								print("%s Character ID: %3d, item slot: [%6s], invalid item id: [%3d]" % (char_type, char_id, slot_type, slot_item_id))
								print("- ERROR: This character has an assigned item [id:%3d, %s] that cannot go in a character slot!" % (slot_item_id, item['name']))
						else:
							valid = False
							print("%s Character ID: %3d, item slot: [%6s], invalid item id: [%3d]" % (char_type, char_id, slot_type, slot_item_id))
							print("- ERROR: This character has an item slot with an invalid item ID, not found in items.py!")
					else:
						if DEBUG:
							print("%s Character ID: %3d, item slot: [%6s], empty" % (char_type, char_id, slot_type))
				if DEBUG:
					print("-")	 
			
			if valid:
				print("GOOD: All characters have valid item ids")
			else:
				return False
			
			#
			# Check any equipped weapons have valid weapon numbers
			# and are appropriate for the class of this character
			#
			
			valid = True
			print("")
			print("Check 1e: Checking %s weapon numbers and class suitability..." % char_type)
			
			for char_id in char_ids:
				for slot_type in WEAPON_SLOTS:
					slot_item_id = char_entries[char_id][slot_type]
					
					if slot_item_id > 0:
						if slot_item_id in weapon_ids:
							
							# Load the item record
							item = game_weapons.WEAPONS[slot_item_id]
							
							if DEBUG:
								print("%s Character ID: %3d, weapon slot: [%6s], weapon id: [%3d] [%18s]" % (char_type, char_id, slot_type, slot_item_id, item['name']))
						else:
							valid = False
							print("%s Character ID: %3d, weapon slot: [%6s], invalid weapon id: [%3d]" % (char_type, char_id, slot_type, slot_item_id))
							print("- ERROR: This character has a weapon slot with an invalid weapon ID, not found in weapons.py!")
					else:
						if DEBUG:
							print("%s Character ID: %3d, weapon slot: [%6s], empty" % (char_type, char_id, slot_type))
				if DEBUG:
					print("-")
			
			if valid:
				print("GOOD: All characters have valid weapon ids")
			else:
				return False
			
			#
			# Check that the character attributes are within bounds
			#
				
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
				if DEBUG:
					print("-")
			if valid:
				print("GOOD: All characters have valid stat entries")
			else:
				return False
	
	print("")
	print("###################################################################################")
	print("#")
	print("# Step 2a.")
	print("#")
	print("# Generating native format sprite assets [%s]" % target['target'])
	print("#")
	print("###################################################################################")
	
	# Generate bitmap data files
	for sprite_type in bitmaps.keys():
		
		if sprite_type == 'SPRITE_CLASS_PORTRAIT':
			i = 0 # Portraits always start from 0
		else:
			i = 1 # Sprites always start from 1, sprite id 0 == no sprite
			
		valid = True
		print("Processing %s" % sprite_type)
		
		data_size = 0
		for sprite in bitmaps[sprite_type].keys():
			if data_size == 0:
				data_size = len(bitmaps[sprite_type][sprite]['converter'].bytes)
			else:
				if data_size != len(bitmaps[sprite_type][sprite]['converter'].bytes):
					for sprite in bitmaps[sprite_type].keys():
						bitmaps[sprite_type][sprite]['id'] = i	
						print("- ID:%3d --> %13s [%s bytes]" % (i, sprite, len(bitmaps[sprite_type][sprite]['converter'].bytes)))
					print("ERROR! - Not all bitmap sizes are identical!")
					print("ERROR! - PLease investigate why all bitmaps are not the same size!")
					valid = False
					return False
			
		print("New ID mapping for %s" % sprite_type)
		for sprite in bitmaps[sprite_type].keys():
			bitmaps[sprite_type][sprite]['id'] = i
				
			print("- ID:%3d --> %13s [%s bytes]" % (i, sprite, len(bitmaps[sprite_type][sprite]['converter'].bytes)))
				
			i += 1
		
		if len(bitmaps[sprite_type].keys()) > 0:
			try:
				sprite_output_filename = SPRITE_SIZES[sprite_type]['file']
				out = import_dir + OUT_DIR + target['suffix'] + "/" + sprite_output_filename
				print("Writing %s data to %s" % (sprite_type, out))
				tmp = out + ".tmp"
				f = open(tmp, "wb", buffering=0)
				offset = 0
				for sprite in bitmaps[sprite_type].keys():
					print("- ID:%3d added at offset %5d" % (bitmaps[sprite_type][sprite]['id'], offset))	
					for px in bitmaps[sprite_type][sprite]['converter'].bytes:
						f.write(px.to_bytes(1, byteorder='big'))
					offset += len(bitmaps[sprite_type][sprite]['converter'].bytes)
				f.flush()
				f.close()
				if os.path.exists(out):
					os.remove(out)
				os.rename(tmp, out)
				print("Done")
				print("")
			except Exception as e:
				print("ERROR! Unable to write sprite dat file [%s]" % sprite_output_filename)
				print("ERROR! %s" % e)
				return False
				
	print("GOOD: All graphics assets have been written")
				
	print("-")
		
	for char_type in ["NPC","MONSTER"]:
		
		if char_type == "MONSTER":
			character_type = "MONSTER"
			character_file = "monster.dat"
			character_ids = monster_ids
			character_entries = game_characters.MONSTER
		
		if char_type == "NPC":
			character_type = "NPC"
			character_file = "npc.dat"
			character_ids = npc_ids
			character_entries = game_characters.NPC
		
		print("")
		print("###################################################################################")
		print("#")
		print("# Step 3.")
		print("#")
		print("# Generating %s (%s)" % (character_type, character_file))
		print("#")
		print("###################################################################################")
			
		character_records = []
			
		for character_id in character_ids:
			
			character = character_entries[character_id]
			data = character_to_record(character_id, character_type, character, item_ids, weapon_ids, bitmaps)
			if data is False:
				print("ERROR! Unable to generate a character data record")
				return False
			else:
				record = {
					'id' : character_id,
					'offset' : 0,
					'data' : data,
					'character' : character,
				}
				character_records.append(record)
				
		print("")
		print("Writing %s data..." % character_type)
		try:
			f = open(import_dir + OUT_DIR + target['suffix'] + "/" + character_file, "wb")
			offset = 0
			for record in character_records:
				record['offset'] = offset
				print("- Character ID: %3d [%18s] at offset %5d" % (record['id'], record['character']['name'], offset))
				for b in record['data']:
					f.write(b)
				offset += len(record['data'])
			f.close()
		except Exception as e:
			print("ERROR! Unable to write datafile")
			print("ERROR! %s" % e)
			return False
		print("...done!")

def character_to_record(character_id, character_type, character, item_ids, weapon_ids, bitmaps):
	""" Generates the string of bytes that represents a character/npc/monster in a datafile """
	
	record = []
	
	print("-- Character ID: %3s [%18s]" % (character_id, character['name']))
	
	######################################################
	# 0. (2 bytes) ID of character
	######################################################
	byte_list = character_id.to_bytes(2, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	if DEBUG:
		print("-- +%2s bytes, ID: %s" % (len(byte_list), character_id))
	
	######################################################
	# 1. (18 bytes) character name (MAX_PLAYER_NAME)
	######################################################
	byte_list = []
	if len(character['name']) > MAX_PLAYER_NAME:
		print("ERROR! Character name [%s] is greater than %d bytes!!!" % (character['name'], MAX_PLAYER_NAME))
		return False
	for c in character['name']:
		byte_list.append(ord(c).to_bytes(1, byteorder='big'))
	for i in range(len(byte_list), MAX_PLAYER_NAME):
		byte_list.append(0x00.to_bytes(1, byteorder='big'))
	for b in byte_list:
		record.append(b)
	if DEBUG:
		print("-- +%2s bytes, name: [%s]" % (len(byte_list), character['name']))
	
	######################################################
	# 2. (1 bytes) TYPE of character (MONSTER/NPC)
	######################################################
	character_type_lookup = False
	
	if character['type'] in CHARACTER_TYPES.keys():
		character_type_lookup = CHARACTER_TYPES[character['type']]
		
	if character_type_lookup is not False:
		byte_list = character_type_lookup.to_bytes(1, byteorder='big')
		for b in byte_list:
			record.append(b.to_bytes(1, byteorder='big'))
		
		if DEBUG:
			print("-- +%2s bytes, CHARACTER TYPE: %s" % (len(byte_list), character_type_lookup))
	else:
		print("ERROR - Unable to determine valid character type for character ID:%s %s" %(character_id, character['type']))
		return False
	
	######################################################
	# 3. (1 bytes) SPRITE TYPE (NORMAL/BOSS)
	######################################################
	sprite_type_lookup = False
	
	if character['sprite_type'] in SPRITE_CLASSES.keys():
		sprite_type_lookup = SPRITE_CLASSES[character['sprite_type']]
	
	if sprite_type_lookup is not False:
		byte_list = sprite_type_lookup.to_bytes(1, byteorder='big')
		for b in byte_list:
			record.append(b.to_bytes(1, byteorder='big'))
		if DEBUG:
			print("-- +%2s bytes, SPRITE TYPE: %s" % (len(byte_list), sprite_type_lookup))
	else:
		print("ERROR - Unable to determine valid sprite type for character ID:%s %s" %(character_id, character['sprite_type']))
		return False
	
	######################################################
	# 4. (2 bytes) per SPRITE filename(ID of the bitmaps processed earlier)
	#    Up to SPRITE_ANIMATION_FRAMES per sprite class
	######################################################
	for sprite_file_class in ["resting_sprite", "attack_sprite", "magic_sprite", "wounded_sprite", "death_sprite"]:
		
		sprite_size = character['sprite_type']
		
		#SPRITE_ANIMATION_FRAMES
		sprite_frames = 0
		for sprite_filename in character[sprite_file_class]:
			
			sprite_id_lookup = False
		
			#sprite_filename = character[sprite]
			
			if sprite_filename == "":
				sprite_id_lookup = 0
			
			else:
				if sprite_filename in bitmaps[sprite_size].keys():
					sprite_id_lookup = bitmaps[sprite_size][sprite_filename]['id']
				else:
					print("ERROR - Unable to find processed sprite for character ID:%s %s %s" %(character_id, sprite_file_class, sprite_filename))
					return False
					
			if sprite_id_lookup is not False:
				byte_list = sprite_id_lookup.to_bytes(2, byteorder='big')
				for b in byte_list:
					record.append(b.to_bytes(1, byteorder='big'))
				if DEBUG:
					print("-- +%2s bytes, %s SPRITE FRAME %s: %s" % (len(byte_list), sprite_file_class, sprite_frames, sprite_id_lookup))
				
			sprite_frames += 1
		
		# Add in null sprites if we don't have enough full frames
		if sprite_frames < SPRITE_ANIMATION_FRAMES:
			
			for i in range(sprite_frames, SPRITE_ANIMATION_FRAMES):
				byte_list = (0).to_bytes(2, byteorder='big')
				for b in byte_list:
					record.append(b.to_bytes(1, byteorder='big'))
				if DEBUG:
					print("-- +%2s bytes, %s NULL SPRITE FRAME %s" % (len(byte_list), sprite_file_class, i))
				
		if sprite_frames > SPRITE_ANIMATION_FRAMES:
			print("ERROR - Too many animation frames [%s] are defined for ID:%s %s" %(sprite_frames, character_id, sprite_file_class))
			print("ERROR - Maximum animation frames is %s" % SPRITE_ANIMATION_FRAMES)
			return False
	
	######################################################
	# 5. (2 bytes) PORTRAIT SPRITE (ID of the bitmaps processed earlier)
	######################################################
	
	sprite_id_lookup = False
	sprite_file_class = "portrait_name"
	sprite_filename = character["portrait_name"][0]
	
	if sprite_filename == "":
		sprite_id_lookup = 0
	
	else:
		
		if sprite_filename in bitmaps['SPRITE_CLASS_PORTRAIT'].keys():
			sprite_id_lookup = bitmaps['SPRITE_CLASS_PORTRAIT'][sprite_filename]['id']
		else:
			print("ERROR - Unable to find processed portrait for character ID:%s %s %s" %(character_id, sprite_file_class, sprite_filename))
			return False
			
	if sprite_id_lookup is not False:
		byte_list = sprite_id_lookup.to_bytes(2, byteorder='big')
		for b in byte_list:
			record.append(b.to_bytes(1, byteorder='big'))
		#if DEBUG:
		print("-- +%2s bytes, %s 0: %s" % (len(byte_list), sprite_file_class, sprite_id_lookup))
	
	######################################################
	# 6. (1 byte) Character CLASS (HUMAN_UNTRAINED, HUMAN_BARD, BEAST, etc)
	######################################################
	
	class_type_lookup = False
	
	if character['class'] in MONSTER_CLASSES.keys():
		class_type_lookup = MONSTER_CLASSES[character['class']]
	
	if class_type_lookup is not False:
		byte_list = class_type_lookup.to_bytes(1, byteorder='big')
		for b in byte_list:
			record.append(b.to_bytes(1, byteorder='big'))
		if DEBUG:
			print("-- +%2s bytes, character class: %s" % (len(byte_list), class_type_lookup))
	else:
		print("ERROR - Unable to determine valid character class for character ID:%s %s" %(character_id, character['class']))
		return False
	
	######################################################
	# 7. (1 byte) Character LEVEL
	######################################################
	
	byte_list = character['level'].to_bytes(1, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	if DEBUG:
		print("-- +%2s bytes, character level: %s" % (len(byte_list), character['level']))
	
	######################################################
	# 8. (2 bytes) Character attack/defense/aggression profile
	######################################################
	
	byte_list = character['profile'].to_bytes(2, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	if DEBUG:
		print("-- +%2s bytes, character profile: %s" % (len(byte_list), character['profile']))
	
	######################################################
	# 9. (1 bytes) Character str
	######################################################
	
	byte_list = character['str'].to_bytes(1, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	if DEBUG:
		print("-- +%2s bytes, character str: %s" % (len(byte_list), character['str']))
	
	######################################################
	# 10. (1 bytes) Character dex
	######################################################
	
	byte_list = character['dex'].to_bytes(1, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	if DEBUG:
		print("-- +%2s bytes, character dex: %s" % (len(byte_list), character['dex']))
	
	######################################################
	# 11. (1 bytes) Character con
	######################################################
	
	byte_list = character['con'].to_bytes(1, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	if DEBUG:
		print("-- +%2s bytes, character con: %s" % (len(byte_list), character['con']))
	
	######################################################
	# 12. (1 bytes) Character wis
	######################################################
	
	byte_list = character['wis'].to_bytes(1, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	if DEBUG:
		print("-- +%2s bytes, character wis: %s" % (len(byte_list), character['wis']))
	
	######################################################
	# 13. (1 bytes) Character intl
	######################################################
	
	byte_list = character['intl'].to_bytes(1, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	if DEBUG:
		print("-- +%2s bytes, character intl: %s" % (len(byte_list), character['intl']))
	
	######################################################
	# 14. (1 bytes) Character chr
	######################################################
	
	byte_list = character['chr'].to_bytes(1, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	if DEBUG:
		print("-- +%2s bytes, character chr: %s" % (len(byte_list), character['chr']))
	
	######################################################
	# 15. (2 bytes) Character hp
	######################################################
	
	byte_list = character['hp'].to_bytes(2, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	if DEBUG:
		print("-- +%2s bytes, character hp: %s" % (len(byte_list), character['hp']))
	
	######################################################
	# 16. (4 bytes) Character status effects
	######################################################
	
	byte_list = character['status'].to_bytes(4, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	if DEBUG:
		print("-- +%2s bytes, character status: %s" % (len(byte_list), character['status']))
	
	######################################################
	# 17. (2 bytes) item equipped to head
	######################################################
	
	if character['head'] < 0:
		character['head'] = 0
	byte_list = character['head'].to_bytes(2, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	if DEBUG:
		print("-- +%2s bytes, head: %s" % (len(byte_list), character['head']))
	
	######################################################
	# 18. (2 bytes) item equipped to body
	######################################################
	
	if character['body'] < 0:
		character['body'] = 0
	byte_list = character['body'].to_bytes(2, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	if DEBUG:
		print("-- +%2s bytes, body: %s" % (len(byte_list), character['body']))
	
	######################################################
	# 19. (2 bytes) item equipped to option slot
	######################################################
	
	if character['option'] < 0:
		character['option'] = 0
	byte_list = character['option'].to_bytes(2, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	if DEBUG:
		print("-- +%2s bytes, option: %s" % (len(byte_list), character['option']))
	
	######################################################
	# 20. (2 bytes) weapon equipped to right hand
	######################################################
	
	if character['weapon_r'] < 0:
		character['weapon_r'] = 0
	byte_list = character['weapon_r'].to_bytes(2, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	if DEBUG:
		print("-- +%2s bytes, weapon_r: %s" % (len(byte_list), character['weapon_r']))
	
	######################################################
	# 21. (2 bytes) weapon equipped to left hand
	######################################################
	
	if character['weapon_l'] < 0:
		character['weapon_l'] = 0
	byte_list = character['weapon_l'].to_bytes(2, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	if DEBUG:
		print("-- +%2s bytes, weapon_l: %s" % (len(byte_list), character['weapon_l']))
	
	######################################################
	# 22. (1 bytes) formation in battle
	######################################################
	
	byte_list = character['formation'].to_bytes(1, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	if DEBUG:
		print("-- +%2s bytes, formation: %s" % (len(byte_list), character['formation']))
	
	######################################################
	# 23. (5 bytes) spells equipped
	######################################################
	
	spells_added = 0
	byte_list = []
	for spell in character['spells']:
		byte_list.append(spell.to_bytes(1, byteorder='big'))
		spells_added += 1
		
	if spells_added < MAX_SPELLS:
		for i in range(spells_added, MAX_SPELLS):
			byte_list.append((0).to_bytes(1, byteorder='big'))
		
	if spells_added > MAX_SPELLS:
		print("ERROR - Too many spells are defined for character ID:%s" %(character_id))
		return False
	if DEBUG:
		print("-- +%2s bytes, spells: %s" % (len(byte_list), character['spells']))
	
	
	
	#################
	# End of record
	#################
	
	print("[%3d] bytes total" % len(record))
	#print(record)
	return record


def generate_story(import_dir = None, target = None):
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
		f = open(import_dir + OUT_DIR + target['suffix'] + "/story.dat", "wb")
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
		f = open(import_dir + OUT_DIR + target['suffix'] + "/story.idx", "wb")
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
		

def generate_world(import_dir = None, target = None):
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
			if text_type in ["npc1_text", "npc2_text", "npc3_text"]:
				if game_world.MAP[i][text_type][1] > 0:
					text_id = game_world.MAP[i][text_type][1]
					if text_id in text_ids:
						#print("- %s: Text ID %s ok" % (text_type, text_id))
						pass
					else:
						print(game_world.MAP[i]['name'])
						print("- %s: MISSING TEXT ID %s" % (text_type, text_id))
						valid = False
			else:
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
		for t in ["north", "south", "east", "west", "spawn", "respawn", "items", "npc1", "npc2", "npc3"]:
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
	print("# Check NPCs have unique dialogue entries.")
	print("#")
	print("###################################################################################")

	print("")
	print("Generating NPC dialogue sequences...")
	valid = True
	npcs = {}
	for i in location_ids:
		npc1 = game_world.MAP[i]['npc1']
		npc2 = game_world.MAP[i]['npc2']
		npc3 = game_world.MAP[i]['npc3']
		if npc1 > 0:
			if npc1 not in npcs.keys():
				npcs[npc1] = {
					'dialogues' : [],
					'strings' : [],
				}
			npcs[npc1]['dialogues'].append(game_world.MAP[i]['npc1_text'][0])
			npcs[npc1]['strings'].append(game_world.MAP[i]['npc1_text'][1])

		if npc2 > 0:
			if npc2 not in npcs.keys():
				npcs[npc2] = {
					'dialogues' : [],
					'strings' : [],
				}
			npcs[npc2]['dialogues'].append(game_world.MAP[i]['npc2_text'][0])
			npcs[npc2]['strings'].append(game_world.MAP[i]['npc2_text'][1])
			
		if npc3 > 0:
			if npc3 not in npcs.keys():
				npcs[npc3] = {
					'dialogues' : [],
					'strings' : [],
				}
			npcs[npc3]['dialogues'].append(game_world.MAP[i]['npc3_text'][0])
			npcs[npc3]['strings'].append(game_world.MAP[i]['npc3_text'][1])
			
	for npc_id in npcs.keys():
		if DEBUG:
			print("NPC ID: %d" % (npc_id))
		dialogue_list = list(npcs[npc_id]['dialogues'])
		strings_list = list(npcs[npc_id]['strings'])
		if DEBUG:
			print(" [%2d] unique dialogues" % (len(dialogue_list)))
			print(" [%2d] unique strings" % (len(strings_list)))
		
		if len(dialogue_list) > MAX_NPC_DIALOGUES:
			print("ERROR: A maximum of %d dialogue sequence per NPC is allowed, with the identifiers 1 - %d." % MAX_NPC_DIALOGUES)
			valid = False
			
		for i in dialogue_list:
			if i > MAX_NPC_DIALOGUES:
				print("ERROR: Dialogue identifiers must be in the range 1 - %d." % MAX_NPC_DIALOGUES)
				valid = False
		
	print("")
		
	if valid:
		print("GOOD: All NPC dialogue sequences appear valid")
	else:
		print("ERROR: At least one NPC dialogue sequence is invalid.")
		return False

	print("")
	print("###################################################################################")
	print("#")
	print("# Step 7.")
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
		f = open(import_dir + OUT_DIR + target['suffix'] + "/world.dat", "wb")
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
		f = open(import_dir + OUT_DIR + target['suffix'] + "/world.idx", "wb")
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
	print("-- +%2s bytes, name [padded to %d]" % (len(tmp), len(byte_list)))
	
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
	
	#####################################################
	# 6. (2 bytes) Text shown when primary monster spawning
	#####################################################
	if location['text_spawn'] > 0:
		byte_list = location['text_spawn'].to_bytes(2, byteorder='big')
	else:
		byte_list = (0).to_bytes(2, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	print("-- +%2s bytes, primary monster spawn text" % len(byte_list))
	
	#####################################################
	# 7. (2 bytes) Text shown when primary monsters spawn, and are then defeated
	#####################################################
	if location['text_after_spawn'] > 0:	
		byte_list = location['text_after_spawn'].to_bytes(2, byteorder='big')
	else:
		byte_list = (0).to_bytes(2, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	print("-- +%2s bytes, primary monster after spawn text" % len(byte_list))
	
	#####################################################
	# 8. (2 bytes) Text shown when secondary monster spawning
	#####################################################
	if location['text_respawn'] > 0:
		byte_list = location['text_respawn'].to_bytes(2, byteorder='big')
	else:
		byte_list = (0).to_bytes(2, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	print("-- +%2s bytes, primary monster respawn text" % len(byte_list))
	
	#####################################################
	# 9. (2 bytes) Text shown when secondary monsters spawn, and are then defeated
	#####################################################
	if location['text_after_respawn'] > 0:	
		byte_list = location['text_after_respawn'].to_bytes(2, byteorder='big')
	else:
		byte_list = (0).to_bytes(2, byteorder='big')
	for b in byte_list:
		record.append(b.to_bytes(1, byteorder='big'))
	print("-- +%2s bytes, primary monster after respawn text" % len(byte_list))
	
	#####################################################
	# 10. NPCs
	#####################################################
	for npc in ['npc1', 'npc2', 'npc3']:
	
		if npc == 'npc1':
			npc_id = location['npc1']
			npc_require = location['npc1_require']
			npc_unique_text_id = location['npc1_text'][0]
			npc_text = location['npc1_text'][1]
			
		if npc == 'npc2':
			npc_id = location['npc2']
			npc_require = location['npc2_require']
			npc_unique_text_id = location['npc2_text'][0]
			npc_text = location['npc2_text'][1]
			
		if npc == 'npc3':
			npc_id = location['npc3']
			npc_require = location['npc3_require']
			npc_unique_text_id = location['npc3_text'][0]
			npc_text = location['npc3_text'][1]
	
		# 10. (1 byte) NPC 1
		if location[npc] > 0:	
			record.append(location[npc].to_bytes(1, byteorder='big'))
		else:
			record.append((0).to_bytes(1, byteorder='big'))
		print("-- + 1 byte, %s ID" % npc)
		
		# 11. (2 bytes to variable) Evaluation rule and requirements for NPC 1
		npc_require_bytes = []
		if len(npc_require) == 0:
			# 1 byte - Empty
			npc_require_bytes.append(CONDITION_RULES["COND_EVAL_EMPTY"].to_bytes(1, byteorder='big'))
			# 1 byte - 0 conditions are to follow
			npc_require_bytes.append((0).to_bytes(1, byteorder='big'))
		else:
			# 1 byte - Evaluation rule
			npc_require_bytes.append(CONDITION_RULES[npc_require[0]].to_bytes(1, byteorder='big'))
			# 1 byte - Number of conditions to follow
			npc_require_bytes.append(npc_require[1].to_bytes(1, byteorder='big'))
			# 5 bytes - Per condition
			for cond in npc_require[2:]:
				print(cond)
				total_conditions += 1
				cond_record = evaluate_condition(location_ids, text_ids, monster_ids, npc_ids, item_ids, weapon_ids, player_ids, cond)
				if len(cond_record) != 5:
					print("ERROR! Condition record for %s require did not convert to 5 bytes!!!" % npc)
					print("ERROR! %s" % cond_record)
					return False
				for c in cond_record:
					npc_require_bytes.append(c.to_bytes(1, byteorder='big'))
		for b in npc_require_bytes:
			record.append(b)
		print("-- +%2s bytes, %s condition requirements" % (len(npc_require_bytes), npc))
		
		# 12. (1 byte) NPC unique text id
		if npc_unique_text_id > 0:	
			byte_list = npc_unique_text_id.to_bytes(1, byteorder='big')
		else:
			byte_list = (0).to_bytes(1, byteorder='big')
		for b in byte_list:
			record.append(b.to_bytes(1, byteorder='big'))
		print("-- +%2s bytes, %s unique text id" % (len(byte_list), npc))
		
		# 13. (2 bytes) NPC text string id
		if npc_text > 0:	
			byte_list = npc_text.to_bytes(2, byteorder='big')
		else:
			byte_list = (0).to_bytes(2, byteorder='big')
		for b in byte_list:
			record.append(b.to_bytes(1, byteorder='big'))
		print("-- +%2s bytes, %s string id" % (len(byte_list), npc))
		
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
	
	target = choose_target(adventure = adventure)
	if target:
	
		status = generate_world(import_dir = adventure, target = target)
		if status is False:
			print("Not continuing. Please fix errors in world map file.")
			sys.exit(1)
			
		status = generate_story(import_dir = adventure, target = target)
		if status is False:
			print("Not continuing. Please fix errors in story file.")
			sys.exit(1)
			
		status = generate_characters(import_dir = adventure, target = target)
		if status is False:
			print("Not continuing. Please fix errors in monster file.")
			sys.exit(1)	
			
		#status = generate_items(import_dir = adventure, target = target)
		#if status is False:
		#	print("Not continuing. Please fix errors in item file.")
		#	sys.exit(1)
		
		#status = generate_weapons(import_dir = adventure, target = target)
		#if status is False:
		#	print("Not continuing. Please fix errors in weapon file.")
		#	sys.exit(1)
#!/usr/bin/env python3

""" items.py, Item/armour definitions for the adventure 'Leafy Glade'
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

ITEMS = {
	##############################################
	1 : {
		'name' 			: "Potion",						# [18 bytes] Display name of item
		'class_limit' 	: "UNTRAINED",					# [1 byte] One of MONSTER_CLASSES, or UNTRAINED to leave as default/all classes
		'race_limit'	: "HUMAN",						# [1 byte] One of MONSTER_RACES, or UNKNOWN to leave as unknown/default/all races
		'type'			: "ITEM_TYPE_CONSUMEABLE",		# [1 byte] One of ITEM_TYPES
		'slot'			: "SLOT_TYPE_NONE",				# [1 byte] One of ITEM_SLOT_TYPES; i.e. where this item can be equipped, SLOT_TYPE_NONE == cannot be equipped in a slot
		'value'			: 10,							# [2 bytes] Cost of the item to buy - some vendors may apply a modifier on buying/selling
		'ac'			: 0,							# [1 byte] Armour Class of the item, if of type ITEM_TYPE_ARMOUR, if equipped
		'ac_type'		: "ARMOUR_TYPE_NONE",			# [1 byte] Armour type - light, medium, heavy
		'effect'		: [								# [5 bytes] List of (up to) 5 effects, from EFFECT_TYPES, that the item has, upon use/equipping
							"EFFECT_TYPE_NONE",
							"EFFECT_TYPE_NONE",
							"EFFECT_TYPE_NONE",
							"EFFECT_TYPE_NONE",
							"EFFECT_TYPE_NONE",
							],	
		'text'			: 10,
	},
	##############################################
	2 : {
		'name' 			: "Padded Clothing",			# [18 bytes] Display name of item
		'class_limit' 	: "UNTRAINED",					# [1 byte] One of MONSTER_CLASSES, or UNTRAINED to leave as default/all classes
		'race_limit'	: "HUMAN",						# [1 byte] One of MONSTER_RACES, or UNKNOWN to leave as unknown/default/all races
		'type'			: "ITEM_TYPE_ARMOUR",			# [1 byte] One of ITEM_TYPES
		'slot'			: "SLOT_TYPE_BODY",				# [1 byte] One of ITEM_SLOT_TYPES; i.e. where this item can be equipped, SLOT_TYPE_NONE == cannot be equipped in a slot
		'value'			: 30,							# [2 bytes] Cost of the item to buy - some vendors may apply a modifier on buying/selling
		'ac'			: 10,							# [1 byte] Armour Class of the item, if of type ITEM_TYPE_ARMOUR, if equipped
		'ac_type'		: "ARMOUR_TYPE_LIGHT",			# [1 byte] One of ARMOUR_TYPES - light, medium, heavy
		'effect'		: [								# [5 bytes] List of (up to) 5 effects, from EFFECT_TYPES, that the item has, upon use/equipping
							"EFFECT_TYPE_NONE",
							"EFFECT_TYPE_NONE",
							"EFFECT_TYPE_NONE",
							"EFFECT_TYPE_NONE",
							"EFFECT_TYPE_NONE",
							],	
		'text'			: 11,		
	},
	##############################################
	3 : {
		'name' 			: "Studded Leather",			# [18 bytes] Display name of item
		'class_limit' 	: "UNTRAINED",					# [1 byte] One of MONSTER_CLASSES, or UNTRAINED to leave as default/all classes
		'race_limit'	: "HUMAN",						# [1 byte] One of MONSTER_RACES, or UNKNOWN to leave as unknown/default/all races
		'type'			: "ITEM_TYPE_ARMOUR",			# [1 byte] One of ITEM_TYPES
		'slot'			: "SLOT_TYPE_BODY",				# [1 byte] One of ITEM_SLOT_TYPES; i.e. where this item can be equipped, SLOT_TYPE_NONE == cannot be equipped in a slot
		'value'			: 55,							# [2 bytes] Cost of the item to buy - some vendors may apply a modifier on buying/selling
		'ac'			: 11,							# [1 byte] Armour Class of the item, if of type ITEM_TYPE_ARMOUR, if equipped
		'ac_type'		: "ARMOUR_TYPE_LIGHT",			# [1 byte] One of ARMOUR_TYPES - light, medium, heavy
		'effect'		: [								# [5 bytes] List of (up to) 5 effects, from EFFECT_TYPES, that the item has, upon use/equipping
							"EFFECT_TYPE_NONE",
							"EFFECT_TYPE_NONE",
							"EFFECT_TYPE_NONE",
							"EFFECT_TYPE_NONE",
							"EFFECT_TYPE_NONE",
							],
		'text'			: 12,
	},
	##############################################
	4 : {
		'name' 			: "Jaunty Hat",					# [18 bytes] Display name of item
		'class_limit' 	: "UNTRAINED",					# [1 byte] One of MONSTER_CLASSES, or UNTRAINED to leave as default/all classes
		'race_limit'	: "HUMAN",						# [1 byte] One of MONSTER_RACES, or UNKNOWN to leave as unknown/default/all races
		'type'			: "ITEM_TYPE_ARMOUR",			# [1 byte] One of ITEM_TYPES
		'slot'			: "SLOT_TYPE_HEAD",				# [1 byte] One of ITEM_SLOT_TYPES; i.e. where this item can be equipped, SLOT_TYPE_NONE == cannot be equipped in a slot
		'value'			: 15,							# [2 bytes] Cost of the item to buy - some vendors may apply a modifier on buying/selling
		'ac'			: 1,							# [1 byte] Armour Class of the item, if of type ITEM_TYPE_ARMOUR, if equipped
		'ac_type'		: "ARMOUR_TYPE_LIGHT",			# [1 byte] One of ARMOUR_TYPES - light, medium, heavy
		'effect'		: [								# [5 bytes] List of (up to) 5 effects, from EFFECT_TYPES, that the item has, upon use/equipping
							"EFFECT_TYPE_NONE",
							"EFFECT_TYPE_NONE",
							"EFFECT_TYPE_NONE",
							"EFFECT_TYPE_NONE",
							"EFFECT_TYPE_NONE",
							],
		'text'			: 13,
	},
}
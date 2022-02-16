#!/usr/bin/env python3

""" map.py, Map/location definitions for the adventure 'Trouble in Leafy Glade'
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

MAP = {
	1 : {
        'name' 				: "Leafy Glade",     	# Name of the location
        'text' 				: 2,                 	# Text block shown initially upon visiting the location
        'north' 			: 2,               		# Exiting North visits this location
        'north_text' 		: 5,           			# Text shown if north is an available exit      
        'north_require' 	: [], 					# Requirement to be met for north to be available
        'south' 			: -1,					# No exit
        'south_text' 		: -1,
        'south_require' 	: [],
        'east' 				: -1,					# No exit
        'east_text' 		: -1,
        'east_require' 		: [],
        'west' 				: -1,					# No exit
        'west_text' 		: -1,
        'west_require' 		: [],
        'spawn_chance' 		: -1,					# No chance of monsters spawning
        'spawn_list' 		: [],         
        'spawn_require' 	: [],      
        'respawn_chance' 	: -1,     
        'respawn_list' 		: [],       
        'respawn_require' 	: [],    
        'items_chance' 		: -1,					# No chance of items spawning as loot
        'items_list' 		: [],
        'items_require' 	: [],
        'text_spawn' 		: -1,           
        'text_after_spawn' : -1,  
        'text_respawn' 		: -1,         
        'text_after_respawn' : -1,
        'npc1' 				: -1,					# No NPC1 at this location
        'npc1_require' 		: [],       
        'npc1_text'  		: [-1, -1],          
        'npc2' 				: 1,                	# NPC2 can appear at this location
        'npc2_require' 		: ["COND_EVAL_AND", 1, 
        							["COND_MONSTER_DEFEAT_TYPE", "MONSTER_TYPE_PRIMARY", 3, 1], # Must have defeated the thief
        						],      
        'npc2_text'  		: [1,9],         		# Unique dialogue #1 for this NPC, uses text string 9
        'npc3' 				: -1,					# No NPC3 at this location
        'npc3_require' 		: [],       
        'npc3_text'  		: [-1, -1],          
	},
	2 : {
		'name' 				: "Trade Route Markers",   
        'text' 				: 3,                		
        'north' 			: 3,                	# Can go north	
        'north_text' 		: -1,          				      
        'north_require' 	: [],       				
        'south' 			: 1,					# Can go south
        'south_text' 		: -1,
        'south_require'		: [],
        'east' 				: -1,
        'east_text' 		: -1,
        'east_require' 		: [],
        'west' 				: -1,
        'west_text' 		: -1,
        'west_require' 		: [],
        'spawn_chance' 		: -1,        
        'spawn_list' 		: [],         
        'spawn_require' 	: [],      
        'respawn_chance' 	: -1,      
        'respawn_list' 		: [],        
        'respawn_require' 	: [],     
        'items_chance' 		: 100,					# Lootable item always spawns if requirements met
        'items_list' 		: ["w1"],
        'items_require' 	: 	["COND_EVAL_AND", 2, 									# Must satisfy both conditions
        							["COND_NPC_TYPE", "COND_NPC_TALK", 1, 0],			# Must have talked to NPC id 1
        							["COND_WEAPON_TYPE", "COND_ITEM_NOTOWN", 1, 0]		# Must not already have the weapon id 1
        						],
        'text_spawn' 		: -1,           
        'text_after_spawn' : -1,    
        'text_respawn' 		: -1,         
        'text_after_respawn' : -1,   
        'npc1' 				: 1,                 
        'npc1_require' 		: ["COND_EVAL_AND", 1, 
        							["COND_MONSTER_DEFEAT_TYPE", "MONSTER_TYPE_PRIMARY", 3, 0] # NPC 1 only appears if we have not already defeated the thief in location id 3
        					  ],       
        'npc1_text'  		: [0,8],	# Unique dialogue reference 0 (1 byte), uses text string 8 (2 bytes)          
        'npc2' 				: -1,                 
        'npc2_require' 		: [] ,       
        'npc2_text'  		: [-1, -1],
        'npc3' 				: -1,					# No NPC3 at this location
        'npc3_require' 		: [],       
        'npc3_text'  		: [-1, -1],          
	},
	3 : {
		'name' 				: "The Bramble Bushes",     # Name of the location
        'text' 				: 4,                 	
        'north' 			: -1,                
        'north_text' 		: -1,               
        'north_require' 	: [],       
        'south' 			: 2,						# Can go south (if not in combat!)
        'south_text' 		: -1,
        'south_require' 	: [],
        'east' 				: -1,
        'east_text' 		: -1,
        'east_require' 		: [],
        'west' 				: -1,
        'west_text' 		: -1,
        'west_require' 		: [],
        'spawn_chance' 		: 100, 						# Monsters always spawn - if requirements met
        'spawn_list' 		: [1],          			# A list of monster ID's which are spawned
        'spawn_require' 	: 	["COND_EVAL_AND", 1, 
        							["COND_MONSTER_DEFEAT_TYPE", "MONSTER_TYPE_PRIMARY", 3, 0]
        						],
        'respawn_chance' 	: -1,
        'respawn_list' 		: [],
        'respawn_require' 	: [],
        'items_chance' 		: -1,
        'items_list' 		: [],
        'items_require' 	: [],
        'text_spawn' 		: 5, 						# This text is shown prior to monsters spawning
        'text_after_spawn' : 6,							# This text is shown after monsters spawn/are defeated
        'text_respawn' 		: -1,
        'text_after_respawn' : -1,
        'npc1' 				: -1, 
        'npc1_require' 		: [] ,
        'npc1_text'  		: [-1, -1],     
        'npc2' 				: -1,           
        'npc2_require' 		: [] ,
        'npc2_text'  		: [-1, -1],	
        'npc3' 				: -1,					# No NPC3 at this location
        'npc3_require' 		: [],       
        'npc3_text'  		: [-1, -1],          
	}
	
	# 59 bytes minimum
	# + 10 require blocks (minimum 1 byte, maximum ??)
	# + 2 monster block (minimum 1 byte, maximum ??)
}

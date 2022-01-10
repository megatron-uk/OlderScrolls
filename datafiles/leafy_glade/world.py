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
        'south' 			: -1,
        'south_text' 		: -1,
        'south_require' 	: [],
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
        'items_chance' 		: -1,
        'items_list' 		: [],
        'items_require' 	: [],
        'text_spawn' 		: -1,           
        'text_after_spawn' : -1,  
        'text_respawn' 		: -1,         
        'text_after_respawn' : -1,
        'npc1' 				: 1,                 
        'npc1_require' 		: [],       
        'npc1_text'  		: 8,          
        'npc2' 				: 1,                
        'npc2_require' 		: ["COND_EVAL_AND", 1, 
        							["COND_MONSTER_DEFEAT_TYPE", "MONSTER_TYPE_PRIMARY", 3, 1]
        						],      
        'npc2_text'  		: 8,         	
	},
	2 : {
		'name' 				: "Trade Route Markers",   # Name of the location
        'text' 				: 3,                		# Text block shown initially upon visiting the location
        'north' 			: 3,                		# Exiting North visits this location
        'north_text' 		: -1,          				# Text shown if north is an available exit      
        'north_require' 	: [],       				# Requirement to be met for north to be available
        'south' 			: 1,
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
        'items_chance' 		: 100,
        'items_list' 		: ["w1"],
        'items_require' 	: 	["COND_EVAL_AND", 1, 
        							["COND_NPC_TYPE", "COND_NPC_TALK", 1, 0],
        						],
        'text_spawn' 		: -1,           
        'text_after_spawn' : -1,    
        'text_respawn' 		: -1,         
        'text_after_respawn' : -1,   
        'npc1' 				: 1,                 
        'npc1_require' 		: [] ,       
        'npc1_text'  		: 7,          
        'npc2' 				: -1,                 
        'npc2_require' 		: [] ,       
        'npc2_text'  		: -1,          	
	},
	3 : {
		'name' 				: "The Bramble Bushes",     # Name of the location
        'text' 				: 4,                 
        'north' 			: -1,                
        'north_text' 		: -1,               
        'north_require' 	: [],       
        'south' 			: 2,
        'south_text' 		: -1,
        'south_require' 	: ["COND_EVAL_AND", 1, 
        							["COND_MONSTER_DEFEAT_TYPE", "MONSTER_TYPE_PRIMARY", 3, 1]
        						],
        'east' 				: -1,
        'east_text' 		: -1,
        'east_require' 		: [],
        'west' 				: -1,
        'west_text' 		: -1,
        'west_require' 		: [],
        'spawn_chance' 		: 100, 
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
        'text_spawn' 		: 5, 
        'text_after_spawn' : 6,
        'text_respawn' 		: -1,
        'text_after_respawn' : -1,
        'npc1' 				: -1, 
        'npc1_require' 		: [] ,
        'npc1_text'  		: -1,     
        'npc2' 				: 1,           
        'npc2_require' 		: [] ,
        'npc2_text'  		: -1,	
	}
	
	# 59 bytes minimum
	# + 10 require blocks (minimum 1 byte, maximum ??)
	# + 2 monster block (minimum 1 byte, maximum ??)
}

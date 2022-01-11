/* conditions_ql.c, Functions to check validity of game logic events and triggers.
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
*/

#include <string.h>
#include <stdio.h>

#ifndef _GAME_H
#include "../common/game.h"
#define _GAME_H
#endif
#ifndef _CONDITIONS_H
#include "../common/conditions.h"
#define _CONDITIONS_H
#endif
#ifndef _DATA_H
#include "../common/data.h"
#define _DATA_H
#endif

unsigned char check_Cond(GameState_t *gamestate, LevelState_t *levelstate, unsigned char *requires, unsigned char number, unsigned char eval_type){

	unsigned char result = 0;
	unsigned char total_false = 0;
	unsigned char total_true = 0;
	unsigned char i;
	unsigned char ii;
	unsigned char cond[COND_LENGTH];
	
	// Test each group of COND_LENGTH bytes in turn, incrementing
	// the total_false or total_true variable based on the whether they
	// return 1 or 0.
	
	for(i = 0; i < number; i++){
		
		// Get next condition 
		memcpy(cond, requires + (i * COND_LENGTH), COND_LENGTH);
		
		// Call correct condition check function
		switch(cond[0]){
			case SIMPLE_COND_TYPE:
				result = check_NoCond(gamestate, levelstate, (char *) cond);
				break;
			case COND_PC_ATR_TYPE:
				result = check_PlayerAttribute(gamestate, levelstate, (char *) cond, 1);
				break;
			case COND_PARTY_ATR_TYPE:
				result = check_PartyAttribute(gamestate, levelstate, (char *) cond);
				break;
			case COND_PARTY_MEMBER_TYPE:
				result = check_PartyState(gamestate, levelstate, (char *) cond);
				break;
			case COND_MAP_VISIT_TYPE:
				result = check_Map(gamestate, levelstate, (char *) cond);
				break;
			case COND_MONSTER_DEFEAT_TYPE:
				result = check_Monster(gamestate, levelstate, (char *) cond);
				break;
			case COND_NPC_TYPE:
				result = check_NPC(gamestate, levelstate, (char *) cond);
				break;
			case COND_ITEM_TYPE:
				result = check_Item(gamestate, levelstate, (char *) cond);
				break;
			case COND_WEAPON_TYPE:
				result = check_Weapon(gamestate, levelstate, (char *) cond);
				break;
			default:
				return 0;
				break;
		}
		
		// Increment result counters
		if (result){
			total_true++;	
		} else {
			total_false++;
		}
	}
	
	// Now work out what we return based on the evaluation rule
	// that these set of conditions are subject to (OR, AND, NOR, NAND)
	switch(eval_type){
		case COND_EVAL_AND:
			// All conditions must be true
			if (total_true == number){
				return 1;
			} else {
				return 0;
			}
			break;
		case COND_EVAL_OR:
			// At least one condition must be true
			if (total_true > 0){
				return 1;
			} else {
				return 0;	
			}
		case COND_EVAL_NOR:
			// All conditions must be false
			if (total_false == number){
				return 1;	
			} else {
				return 0;	
			}
		case COND_EVAL_NAND:
			// 0 or 1 conditions must be true, but no more than 1.
			if ((total_true == 1) || (total_true == 0)){
				return 1;
			} else {
				return 0;	
			}
		default:
			return 0;
	}
}

unsigned char check_NoCond(GameState_t *gamestate, LevelState_t *levelstate, char *cond){
	// Simple condition check
	
	unsigned char check_type = 0;
	
	// 1 byte
	check_type = cond[1];
	
	// No check, return true
	if (check_type == COND_NO_COND){
		return 1;	
	}
	
	// Have monsters spawned
	if (check_type == COND_NO_MONSTERS){
		if (levelstate->spawned == 0){
			return 1;
		} else {
			return 0;	
		}
	}
	
	return 0;
}

unsigned char check_PlayerAttribute(GameState_t *gamestate, LevelState_t *levelstate, char *cond, unsigned char player){
	// Check one aspect of a player attribute
	
	unsigned char check_attribute = 0;
	unsigned char check_value = 0;
	PlayerState_t *pc;
	
	check_attribute = cond[2];
	check_value = cond[3];
	
	// Select the player character
	switch(player){
		case 1:
			pc = gamestate->p1;
		case 2:
			if (gamestate->p2){
				pc = gamestate->p2;
			} else {
				return 0;
			}
			break;
		case 3:
			if (gamestate->p3){
				pc = gamestate->p3;
			} else {
				return 0;
			}
			break;
		case 4:
			if (gamestate->p4){
				pc = gamestate->p4;
			} else {
				return 0;
			}
			break;
		default:
			return 0;
	}
	
	// Check the attribute
	switch(check_attribute){
		
		case COND_TYPE_STR:
			if (pc->str >= check_value) return 1;
			break;
		case COND_TYPE_DEX:
			if (pc->dex >= check_value) return 1;
			break;
		case COND_TYPE_CON:
			if (pc->con >= check_value) return 1;
			break;
		case COND_TYPE_WIS:
			if (pc->wis >= check_value) return 1;
			break;
		case COND_TYPE_INT:
			if (pc->intl >= check_value) return 1;
			break;
		case COND_TYPE_CHR:
			if (pc->chr >= check_value) return 1;
			break;
		case COND_TYPE_HP:
			if (pc->hp >= check_value) return 1;
			break;
		case COND_TYPE_GOLD:
			if (gamestate->gold >= check_value) return 1;
			break;
		case COND_TYPE_DMG:
			if (pc->hits_caused >= check_value) return 1;
			break;
		case COND_TYPE_INJURY:
			if (pc->hits_taken >= check_value) return 1;
			break;
		default:
			break;
	}
	return 0;
}

unsigned char check_PartyAttribute(GameState_t *gamestate, LevelState_t *levelstate, char *cond){
	// Check that at least one party member meets a condition
	
	unsigned char player;
	
	// Check each player in turn
	for (player = 1; player <= 4; player++){
		if (check_PlayerAttribute(gamestate, levelstate, cond, player)){
			return 1;
		}
	}
	return 0;
}

unsigned char check_PartyState(GameState_t *gamestate, LevelState_t *levelstate, char *cond){
}

unsigned char check_Map(GameState_t *gamestate, LevelState_t *levelstate, char *cond){
	// Check visit to a location
	
	unsigned char check_type = 0;
	unsigned short check_attribute = 0;
	unsigned char check_value = 0;
	
	check_type = cond[1];
	check_attribute = (cond[2] << 2) + cond[3];
	check_value = cond[4];
	
	// Check a location has been visited a minimum number of times
	if (check_type == COND_MAP_VISIT_MIN){
		if (gamestate->level_visits[check_attribute] >= check_value){
			return 1;
		}
	}
	
	// Check a location has been visited no more than a maximum number of times
	if (check_type == COND_MAP_VISIT_MAX){
		if (gamestate->level_visits[check_attribute] <= check_value){
			return 1;
		}
	}
	return 0;
}

unsigned char check_Monster(GameState_t *gamestate, LevelState_t *levelstate, char *cond){
	// Check on the monster defeat status of a given location and primary/secondary spawn type
	
	unsigned char defeat_type = 0;
	unsigned short defeat_location_id = 0;
	unsigned char defeat_count = 0;
	
	// 1 byte
	defeat_type = cond[1];
	
	// 2 bytes
	defeat_location_id = (cond[2] << 2) + cond[3];
	
	// 1 byte
	defeat_count = cond[4];
		
	if (gamestate->level_visits[defeat_location_id] > 0){
		// We visited here at least once
		
		if (defeat_type == COND_MONSTER_PRI_DEFEATED){
			// Primary spawned monster beaten at least N times
			if (gamestate->level_defeated_primary[defeat_location_id] >= defeat_count){
				return 1;
			} else {
				return 0;	
			}
		}
		if (defeat_type == COND_MONSTER_SEC_DEFEATED){
			// Secondary spawned monster beaten at least N times
			if (gamestate->level_defeated_secondary[defeat_location_id] >= defeat_count){
				return 1;
			} else {
				return 0;	
			}
		}
	} 
	// Never visited this location, or an invalid check type so all monster checks here are failed
	return 0;
}

unsigned char check_NPC(GameState_t *gamestate, LevelState_t *levelstate, char *cond){
	// Check on the status of an NPC, if we have visited them, if they are alive, etc
	
	unsigned char check_type = 0;
	unsigned char check_npc_id = 0;
	unsigned short check_value = 0;
	struct NPCList *npc;
	
	// 1 byte
	check_type = cond[1];
	
	// 1 byte
	check_npc_id = cond[2];
	
	// 2 bytes
	check_value = (cond[3] << 2) + cond[4];
	
	npc = data_FindNPC(gamestate->npcs, check_npc_id);
	if (npc){
		// Met this NPC
		
		// Are they alive?
		if (check_type == COND_NPC_ALIVE){
			if (npc->death_time == 0){
				return 1;
			} else {
				return 0;
			}
		}
		
		// Are they dead?
		if (check_type == COND_NPC_DEAD){
			if (npc->death_time != 0){
				return 1;
			} else {
				return 0;
			}
		}
		
		// Did we meet them less than 'x' turns ago?
		if (check_type == COND_NPC_TIMER_LESS){
			if ((gamestate->counter - npc->talked_time) <= check_value){
				return 1;
			} else {
				return 0;
			}
		}
		
		// Did we meet them more than 'x' turns ago?
		if (check_type == COND_NPC_TIMER_MORE){
			if ((gamestate->counter - npc->talked_time) >= check_value){
				return 1;
			} else {
				return 0;
			}
		}
	} else {
		// Not yet met this NPC, so they default to being alive
		if (check_type == COND_NPC_ALIVE){
			return 1;	
		}
	}
	// NPC not found
	
	// COND_NPC_TIMER_LESS and COND_NPC_TIMER_MORE
	// both fall through here if we haven't yet met the NPC
	return 0;
}

unsigned char check_ItemWeapon(GameState_t *gamestate, LevelState_t *levelstate, char *cond, char *item_weapon){
	
	unsigned char check_type = 0;
	unsigned char check_item_id = 0;
	unsigned char i;
	unsigned char player;
	char pc_item_type;
	unsigned char pc_item_id;
	PlayerState_t *pc;
	
	check_type = cond[2];
	check_item_id = cond[3];
	
	for (player = 1; player <= 4; player++){
		switch(player){
			case 1:
				pc = gamestate->p1;
			case 2:
				if (gamestate->p2){
					pc = gamestate->p2;
				} else {
					pc = NULL;
				}
				break;
			case 3:
				if (gamestate->p3){
					pc = gamestate->p3;
				} else {
					pc = NULL;;
				}
				break;
			case 4:
				if (gamestate->p4){
					pc = gamestate->p4;
				} else {
					pc = NULL;;
				}
				break;
		}
		if (pc != NULL){
			for (i = 0; i < MAX_ITEMS; i++){
				pc_item_type = (pc->items[i] & 0xff00) >> 2;
				pc_item_id = (unsigned char) (pc->items[i] & 0x00ff);
				
				if ((pc_item_id == check_item_id) && (pc_item_type == *item_weapon)){
					if (check_type == COND_ITEM_OWN) return 1;
					if (check_type == COND_ITEM_NOTOWN) return 0;
				}		
			}
		}
	}
	return 0;	
}

unsigned char check_Item(GameState_t *gamestate, LevelState_t *levelstate, char *cond){
	
	return check_ItemWeapon(gamestate, levelstate, cond, "i");
}

unsigned char check_Weapon(GameState_t *gamestate, LevelState_t *levelstate, char *cond){
	
	return check_ItemWeapon(gamestate, levelstate, cond, "w");
}
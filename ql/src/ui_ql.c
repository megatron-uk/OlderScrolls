/* main_ql.c, Runtime for the Sinclair QL version of OlderScrolls-Engine.
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

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

// Platform agnostic functions
#ifndef _CONFIG_H
#include "../common/config.h"
#endif
#ifndef _GAME_H
#include "../common/game.h"
#endif
#ifndef _UI_H
#include "../common/ui.h"
#endif
#ifndef _DATA_H
#include "../common/data.h"
#endif
#ifndef _DRAW_H
#include "../common/draw.h"
#endif
#ifndef _INPUT_H
#include "../common/input.h"
#endif
#ifndef _UTILS_H
#include "../common/utils.h"
#endif
#ifndef _ERROR_H
#include "../common/error.h"
#endif
#ifndef _MONSTERS_H
#include "../common/monsters.h"
#endif
#ifndef _ENGINE_H
#include "../common/engine.h"
#endif

void ui_Draw(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate){
	// Draws the main user interface - top bar with game name and turn counter
	// Main story/combat window
	// Side bar with party details
	// Bottom bar with status messages/current options
	
	draw_Clear(screen);
	
	// Main outer edges of screen
	draw_Box(screen, UI_OUTER_BORDER_START_X, UI_OUTER_BORDER_START_Y, SCREEN_WIDTH, SCREEN_HEIGHT - 1, UI_OUTER_BORDER_PX, UI_OUTER_BORDER_COLOUR, PIXEL_CLEAR, MODE_PIXEL_SET);
	
	// Game title header bar
	draw_HLine(screen, UI_OUTER_BORDER_START_X, UI_OUTER_BORDER_START_Y + 11, SCREEN_WIDTH, UI_OUTER_BORDER_COLOUR, 0, MODE_PIXEL_SET);
	draw_HLine(screen, UI_OUTER_BORDER_START_X, UI_OUTER_BORDER_START_Y + 12, SCREEN_WIDTH, UI_OUTER_BORDER_COLOUR, 1, MODE_PIXEL_SET);
	
	// Bottom status box
	draw_HLine(screen, UI_STATUSBAR_START_X, UI_STATUSBAR_START_Y, UI_STATUSBAR_LENGTH, UI_OUTER_BORDER_COLOUR, 0, MODE_PIXEL_SET);
	draw_HLine(screen, UI_STATUSBAR_START_X, UI_STATUSBAR_START_Y + 1, UI_STATUSBAR_LENGTH, UI_OUTER_BORDER_COLOUR, 1, MODE_PIXEL_SET);
	
	// Sidebar
	draw_VLine(screen, UI_SIDEBAR_START_X, UI_SIDEBAR_START_Y, UI_SIDEBAR_HEIGHT, UI_OUTER_BORDER_COLOUR, MODE_PIXEL_OR);
	draw_VLine(screen, UI_SIDEBAR_START_X + 1, UI_SIDEBAR_START_Y + 1, UI_SIDEBAR_HEIGHT - 1, UI_OUTER_BORDER_COLOUR, MODE_PIXEL_OR);

	// Game Engine name in title bar
	draw_String(screen, UI_TITLEBAR_TEXT_X, UI_TITLEBAR_TEXT_Y, 24, 1, 0, screen->font_8x8, PIXEL_GREEN, ENGINE_TARGET_NAME, MODE_PIXEL_OR);
	
	screen->dirty = 1;

}

void ui_DrawCombat(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate){
	
	screen->dirty = 1;
}

char ui_DrawCharacterScreen_Overview(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate){
	// Draw the overview character screen
	unsigned char c;
	char new_mode;
	unsigned char e = 0;
	PlayerState_t *pc;
	unsigned char i;
	char pc_id = gamestate->players->current - 1;
	unsigned char dice_quantity = 0;
	unsigned char dice_type = 0;
	unsigned char proficiency = 0;
	char constitution_modifier = 0;
	
	// Load the player character
	pc = gamestate->players->player[pc_id];
	
	// ==========================================
	// Left hand column
	// ==========================================
	
	// Character portrait
	draw_Box(screen, 8, 16, DRAW_PORTRAIT_WIDTH + 1, DRAW_PORTRAIT_HEIGHT + 1, 1, PIXEL_RED, PIXEL_CLEAR, MODE_PIXEL_SET);
	draw_Sprite(screen, 9, 17, screen->players[pc_id], 1);
	
	// Name, Class, Race
	sprintf(gamestate->buf, "<r>Character<C>\nName : %s\nClass: %s\nRace : %s", pc->name, player_classes[pc->player_class], player_races[pc->player_race]);
	draw_String(screen, 6, 18, 24, 4, 0, screen->font_8x8, PIXEL_WHITE, gamestate->buf, MODE_PIXEL_SET);
	
	// Equipment section
	draw_String(screen, 1, 60, 24, 6, 0, screen->font_8x8, PIXEL_WHITE, "<r>Equipment\n<C>Head\nBody\nOption\nR.Hand\nL.Hand", MODE_PIXEL_SET);
	
	// Head
	if (pc->head->item_id == 0){
		sprintf(gamestate->buf, ": -\n");
	} else {
		sprintf(gamestate->buf, ": %s\n", pc->head->name);
	}
	
	// Body
	if (pc->body->item_id == 0){
		sprintf(gamestate->buf + strlen(gamestate->buf), ": -\n");
	} else {
		sprintf(gamestate->buf + strlen(gamestate->buf), ": %s\n", pc->body->name);
	}
	
	// Option
	if (pc->option->item_id == 0){
		sprintf(gamestate->buf + strlen(gamestate->buf), ": -\n");
	} else {
		sprintf(gamestate->buf + strlen(gamestate->buf), ": %s\n", pc->option->name);
	}
	
	// Right hand
	if (pc->weapon_r->item_id == 0){
		sprintf(gamestate->buf + strlen(gamestate->buf), ": -\n");
	} else {
		sprintf(gamestate->buf + strlen(gamestate->buf), ": %s\n", pc->weapon_r->name);
	}
	
	// Left hand
	if (pc->weapon_l->item_id == 0){
		sprintf(gamestate->buf + strlen(gamestate->buf), ": -\n");
	} else {
		sprintf(gamestate->buf + strlen(gamestate->buf), ": %s\n", pc->weapon_l->name);
	}
	
	draw_String(screen, 7, 68, 24, 6, 0, screen->font_8x8, PIXEL_WHITE, gamestate->buf, MODE_PIXEL_SET);
	
	// ==========================================
	// Central divider
	// ==========================================
	draw_VLine(screen, 255, 20, 200, PIXEL_RED, MODE_PIXEL_SET);
	
	// ==========================================
	// Right hand column
	// ==========================================
	
	// Attribute values and their respective modifiers
	sprintf(gamestate->buf, "<r>Attribute<C>\nStrength\nDexterity\nConstitution\nWisdom\nIntelligence\nCharisma");
	draw_String(screen, 33, 18, 12, 13, 0, screen->font_8x8, PIXEL_WHITE, gamestate->buf, MODE_PIXEL_SET);
	sprintf(gamestate->buf, "<r>Value<C>\n%d\n%d\n%d\n%d\n%d\n%d", pc->str, pc->dex, pc->con, pc->wis, pc->intl, pc->chr);
	draw_String(screen, 47, 18, 12, 13, 0, screen->font_8x8, PIXEL_WHITE, gamestate->buf, MODE_PIXEL_SET);
	sprintf(gamestate->buf, "<r>Modifier<C>\n%d\n%d\n%d\n%d\n%d\n%d", ability_Modifier(pc->str), ability_Modifier(pc->dex), ability_Modifier(pc->con), ability_Modifier(pc->wis), ability_Modifier(pc->intl), ability_Modifier(pc->chr));
	draw_String(screen, 54, 18, 12, 13, 0, screen->font_8x8, PIXEL_WHITE, gamestate->buf, MODE_PIXEL_SET);
	
	// Print out the HP increase per level
	constitution_modifier = ability_Modifier(pc->con);
	hit_Dice(pc, &dice_quantity, &dice_type, &constitution_modifier);
	sprintf(gamestate->buf, "HP per level: %dD%d +%d", dice_quantity, dice_type, constitution_modifier);
	draw_String(screen, 33, 100, 30, 1, 0, screen->font_8x8, PIXEL_WHITE, gamestate->buf, MODE_PIXEL_SET);
	
	// Print out skills we are proficient in
	sprintf(gamestate->buf, "<r>Skills<C>\n");
	for (i = 0; i < MAX_PROFICIENCIES; i++){
		proficiency = player_class_proficiencies[pc->player_class][i];
		if (proficiency != 0){
			sprintf(gamestate->buf + strlen(gamestate->buf), "+%d %s\n", is_proficient(pc, proficiency), proficiencies[proficiency]);
		}
	}
	draw_String(screen, 33, 120, 30, 11, 0, screen->font_8x8, PIXEL_WHITE, gamestate->buf, MODE_PIXEL_SET);
	
	while(!e){
		c = input_Get(screen);
		switch(c){
			case INPUT_OVERVIEW:
			case INPUT_OVERVIEW_:
				new_mode = 0;
				e = 1;
				break;
			case INPUT_ITEMS:
			case INPUT_ITEMS_:
				new_mode = 1;
				e = 1;
				break;
			case INPUT_WEAPONS:
			case INPUT_WEAPONS_:
				new_mode = 2;
				e = 1;
				break;
			case INPUT_MAGIC:
			case INPUT_MAGIC_:
				new_mode = 3;
				e = 1;
				break;
			case INPUT_STATUS:
			case INPUT_STATUS_:
				new_mode = 4;
				e = 1;
				break;
			case INPUT_CANCEL:
				new_mode = -1;
				e = 1;
				break;
			default:
				break;
		}
	}	
	return new_mode;
}

char ui_DrawCharacterScreen_InventoryItemDetails(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, WeaponState_t *weapon, ItemState_t *item, char selected_id){
	// Draw text about the current selected item in the bottom of the inventory window

	unsigned char i;
	char pc_id = gamestate->players->current - 1;
	
	if (gamestate->players->player[pc_id]->items[selected_id].item_type == ITEM_TYPE_ITEM){
		// Load item to get name
		data_LoadStory(screen, gamestate, levelstate, item->text_id);
		sprintf(gamestate->text_buffer, gamestate->buf);
	} 
	if (gamestate->players->player[pc_id]->items[selected_id].item_type == ITEM_TYPE_WEAPON){
		// Load weapon to get name
		data_LoadStory(screen, gamestate, levelstate, weapon->text_id);
		sprintf(gamestate->text_buffer, gamestate->buf);
	}
	if (gamestate->players->player[pc_id]->items[selected_id].item_type == ITEM_TYPE_NONE){
		sprintf(gamestate->text_buffer, "");
	}
	
	
	// Draw a green box at bottom of screen to hold selected item details
	draw_Box(screen, 8, 118, 496, 114, 1, PIXEL_GREEN, PIXEL_BLACK, MODE_PIXEL_SET);
	
	// Only draw the buttons and text on initial first pass
	if (selected_id == -1){
		for (i = 0; i < 5; i++){
			// Boxes for each of the buttons
			draw_Box(screen, 8 + (i * 104), 236, 80, 16, 2, PIXEL_RED_STIPPLED, PIXEL_BLACK, MODE_PIXEL_SET);
		}
			
		// Button text
		draw_String(screen, 4, 241, 10, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>U<C>se", MODE_PIXEL_SET);
		draw_String(screen, 16, 241, 10, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>E<C>quip", MODE_PIXEL_SET);
		draw_String(screen, 29, 241, 10, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>R<C>emove", MODE_PIXEL_SET);
		draw_String(screen, 43, 241, 10, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>D<C>rop", MODE_PIXEL_SET);
		draw_String(screen, 54, 241, 16, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>T<C>ransfer", MODE_PIXEL_SET);
	}
	
	// For 
	if ((item->item_id == 0) && (weapon->item_id == 0)){
		if (selected_id >= 0){
			sprintf(gamestate->buf, "<r>Inventory Slot <C>%d: <g>Empty", selected_id);
		} else {
			sprintf(gamestate->buf, "<r>Please select an item");
		}
	} else if (item->item_id != 0){
		sprintf(gamestate->buf, "<r>Inventory Slot <C>%d: <g>Item Information<C>\n\nName: %s\n", selected_id, item->name);
		if (item->class_limit == 0){
			sprintf(gamestate->buf + strlen(gamestate->buf), "Class: All, ");
		} else {
			sprintf(gamestate->buf + strlen(gamestate->buf), "Class: %s, ", player_classes[item->class_limit]);
		}
		if (item->race_limit == 0){
			sprintf(gamestate->buf + strlen(gamestate->buf), "Race: All, ");
		} else {
			sprintf(gamestate->buf + strlen(gamestate->buf), "Race: %s, ", player_races[item->race_limit]);
		}
		switch(item->type){
			case ITEM_TYPE_ARMOUR:
				sprintf(gamestate->buf + strlen(gamestate->buf), "Type: Armour\n");
				switch(item->ac_type){
					case ARMOUR_TYPE_NONE:
						sprintf(gamestate->buf + strlen(gamestate->buf), "AC Class: N/A, ");
						break;
					case ARMOUR_TYPE_LIGHT:
						sprintf(gamestate->buf + strlen(gamestate->buf), "AC Class: Light, ");
						break;
					case ARMOUR_TYPE_MEDIUM:
						sprintf(gamestate->buf + strlen(gamestate->buf), "AC Class: Medium, ");
						break;
					case ARMOUR_TYPE_HEAVY:
						sprintf(gamestate->buf + strlen(gamestate->buf), "AC Class: Heavy, ");
						break;
				}
				sprintf(gamestate->buf + strlen(gamestate->buf), "AC Rating: %d, ", item->ac);
				switch(item->slot){
					case ITEM_SLOT_HEAD:
						sprintf(gamestate->buf + strlen(gamestate->buf), "Equipment Slot: Head\n");
						break;
					case ITEM_SLOT_BODY:
						sprintf(gamestate->buf + strlen(gamestate->buf), "Equipment Slot: Body\n");
						break;
					case ITEM_SLOT_OPTION:
						sprintf(gamestate->buf + strlen(gamestate->buf), "Equipment Slot: Option\n");
						break;
				}
				break;
			case ITEM_TYPE_CONSUMEABLE:
				sprintf(gamestate->buf + strlen(gamestate->buf), "Type: Consumeable\n");
				break;
			case ITEM_TYPE_SPELL:
				sprintf(gamestate->buf + strlen(gamestate->buf), "Type: Spell Scroll\n");
				break;
			case ITEM_TYPE_GENERIC:
				sprintf(gamestate->buf + strlen(gamestate->buf), "Type: -\n");
				break;
			case ITEM_TYPE_QUEST:
				sprintf(gamestate->buf + strlen(gamestate->buf), "Type: Quest Item\n");
				break;
			
		}
		sprintf(gamestate->buf + strlen(gamestate->buf), "Value: %dg\n", item->value);
	} else {
		sprintf(gamestate->buf, "<r>Inventory Slot <C>%d: <g>Weapon Information<C>\n\nName: %s\n", selected_id, weapon->name);
		if (weapon->weapon_type == WEAPON_2HANDED){
			sprintf(gamestate->buf + strlen(gamestate->buf), "Type: 2-Handed, ");
		} else {
			sprintf(gamestate->buf + strlen(gamestate->buf), "Type: 1-Handed, ");
		}
		switch(weapon->weapon_class){
			case WEAPON_CLASS_SIMPLE:
				sprintf(gamestate->buf + strlen(gamestate->buf), "Weapon Class: Simple, ");
				break;
			case WEAPON_CLASS_MARTIAL:
				sprintf(gamestate->buf + strlen(gamestate->buf), "Weapon Class: Martial, ");
				break;
			case WEAPON_CLASS_RANGED:
				sprintf(gamestate->buf + strlen(gamestate->buf), "Weapon Class: Ranged, ");
				break;
			case WEAPON_CLASS_MAGICAL:
				sprintf(gamestate->buf + strlen(gamestate->buf), "Weapon Class: Magical, ");
				break;
		}
		sprintf(gamestate->buf + strlen(gamestate->buf), "Critical: %d-%d x%d\n", weapon->crit_min, weapon->crit_max, weapon->crit_dice_qty);
		ui_DrawCharacterScreen_WeaponDamageToString(screen, gamestate, levelstate, &weapon, weapon->dmg1_type, weapon->dmg1_dice_qty, weapon->dmg1_dice_type);
		sprintf(gamestate->buf + strlen(gamestate->buf), ", ");
		ui_DrawCharacterScreen_WeaponDamageToString(screen, gamestate, levelstate, &weapon, weapon->dmg2_type, weapon->dmg2_dice_qty, weapon->dmg2_dice_type);
		sprintf(gamestate->buf + strlen(gamestate->buf), ", ");
		ui_DrawCharacterScreen_WeaponDamageToString(screen, gamestate, levelstate, &weapon, weapon->dmg3_type, weapon->dmg3_dice_qty, weapon->dmg3_dice_type);
		sprintf(gamestate->buf + strlen(gamestate->buf), "\nValue: %dg\n", weapon->value);
	}
	
	// Draw the constructed text string of the stats of the selected item or weapon
	draw_String(screen, 2, 122, 60, 7, 0, screen->font_8x8, PIXEL_WHITE, gamestate->buf, MODE_PIXEL_SET);
	
	if (selected_id >= 0){
		// Draw the text string loaded for the selected item or weapon
		draw_String(screen, 2, 186, 60, 5, 0, screen->font_8x8, PIXEL_WHITE, gamestate->text_buffer, MODE_PIXEL_SET);
	}	
}

char ui_DrawCharacterScreen_InventoryRedraw(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, WeaponState_t *weapon, ItemState_t *item, char selected_id){
	// Draw the list of items in the inventory, highlighting the currently selected item
	
	WeaponState_t list_weapon;
	ItemState_t list_item;
	unsigned char i;
	char pc_id = gamestate->players->current - 1;
	
	if (selected_id == -1){
		draw_Box(screen, 0, 10, SCREEN_WIDTH, SCREEN_HEIGHT - 11, 1, PIXEL_RED, PIXEL_BLACK, MODE_PIXEL_SET);
	}
	
	// Two columns of inventory items
	for (i = 0; i < 16; i++){
		
		// We only need to load item information the first time around, or if we
		// have selected this item
		
		if (selected_id == -1){
			if (gamestate->players->player[pc_id]->items[i].item_id != 0){
				if (gamestate->players->player[pc_id]->items[i].item_type == ITEM_TYPE_ITEM){
					// Load item to get name
					data_LoadItem(screen, &list_item, gamestate->players->player[pc_id]->items[i].item_id);
					sprintf(gamestate->buf, "  x%02d %s", gamestate->players->player[pc_id]->items[i].qty, list_item.name);	
				}
				if (gamestate->players->player[pc_id]->items[i].item_type == ITEM_TYPE_WEAPON){
					// Load weapon to get name
					data_LoadWeapon(screen, &list_weapon, gamestate->players->player[pc_id]->items[i].item_id);
					sprintf(gamestate->buf, "  x%02d %s", gamestate->players->player[pc_id]->items[i].qty, list_weapon.name);
				}
			} else {
				// The slot is empty
				sprintf(gamestate->buf, "  -                      ");
			}
			if (i < 8){
				draw_String(screen, 6, 20 + (i * 12), 22, 1, 0, screen->font_8x8, PIXEL_WHITE, gamestate->buf, MODE_PIXEL_SET);
			} else {
				draw_String(screen, 38, 20 + ((i - 8) * 12), 22, 1, 0, screen->font_8x8, PIXEL_WHITE, gamestate->buf, MODE_PIXEL_SET);
			}
		}
		
		// Colour in the box of the selected item
		if (i < 8){
			if (selected_id == i){
				draw_Box(screen, 30, 18 + (i * 12), 8, 8, 1, PIXEL_RED, PIXEL_WHITE, MODE_PIXEL_SET);
			} else {
				draw_Box(screen, 30, 18 + (i * 12), 8, 8, 1, PIXEL_RED, PIXEL_BLACK, MODE_PIXEL_SET);
			}
		} else {
			if (selected_id == i){
				draw_Box(screen, 287, 18 + ((i - 8) * 12), 8, 8, 1, PIXEL_RED, PIXEL_WHITE, MODE_PIXEL_SET);
			} else {
				draw_Box(screen, 287, 18 + ((i - 8) * 12), 8, 8, 1, PIXEL_RED, PIXEL_BLACK, MODE_PIXEL_SET);				
			}	
		}
	}
	
	ui_DrawCharacterScreen_InventoryItemDetails(screen, gamestate, levelstate, weapon, item, selected_id);
}
	
void ui_LoadInventoryItem(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, WeaponState_t *weapon, ItemState_t *item, unsigned char selected_id){
	// Load the currently highlighted item
	char pc_id = gamestate->players->current - 1;
	
	if (gamestate->players->player[pc_id]->items[selected_id].item_type == ITEM_TYPE_ITEM){
		// Load item to get name
		weapon->item_id = 0;
		data_LoadItem(screen, item, gamestate->players->player[pc_id]->items[selected_id].item_id);
	} 
	if (gamestate->players->player[pc_id]->items[selected_id].item_type == ITEM_TYPE_WEAPON){
		// Load weapon to get name
		item->item_id = 0;
		data_LoadWeapon(screen, weapon, gamestate->players->player[pc_id]->items[selected_id].item_id);
	}
	if (gamestate->players->player[pc_id]->items[selected_id].item_type == ITEM_TYPE_NONE){
		// If item slot is empty, set the weapon and item objects to empty also.
		item->item_id = 0;
		weapon->item_id = 0;
	}
}

char ui_DrawCharacterScreen_Inventory(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate){
	// Draw the player character inventory screen, allow the player to scroll through items,
	// use, equip, drop or transfer them, etc.
	
	char new_mode = 1;
	char new_selected_id = -1;
	unsigned char pc_id;
	unsigned char c;
	unsigned char e = 0;
	unsigned char refresh = 0;
	WeaponState_t weapon;
	ItemState_t item;
	
	item.item_id = 0;
	weapon.item_id = 0;
	pc_id = gamestate->players->current - 1;
	
	ui_DrawCharacterScreen_InventoryRedraw(screen, gamestate, levelstate, &weapon, &item, new_selected_id);
	
	// Add up/down cursor input to allow selection of items from inventory
	input_Set(INPUT_UP);
	input_Set(INPUT_DOWN);
	// Add keys to represent the inventory button options
	// Use, Equip, Remove, Drop, Transfer
	input_Set(INPUT_U);
	input_Set(INPUT_E);
	input_Set(INPUT_R);
	input_Set(INPUT_D);
	input_Set(INPUT_T);
	input_Set(INPUT_U_);
	input_Set(INPUT_E_);
	input_Set(INPUT_R_);
	input_Set(INPUT_D_);
	input_Set(INPUT_T_);
	
	while(!e){
		c = input_Get(screen);
		switch(c){
			case INPUT_OVERVIEW:
			case INPUT_OVERVIEW_:
				new_mode = 0;
				e = 1;
				break;
			case INPUT_ITEMS:
			case INPUT_ITEMS_:
				new_mode = 1;
				e = 1;
				break;
			case INPUT_WEAPONS:
			case INPUT_WEAPONS_:
				new_mode = 2;
				e = 1;
				break;
			case INPUT_MAGIC:
			case INPUT_MAGIC_:
				new_mode = 3;
				e = 1;
				break;
			case INPUT_STATUS:
			case INPUT_STATUS_:
				new_mode = 4;
				e = 1;
				break;
			case INPUT_DOWN:
				// =====================================
				// Scroll to next item
				// =====================================
				new_mode = 1;
				if (new_selected_id < (MAX_ITEMS - 1)){
					new_selected_id++;
				} else {
					new_selected_id = (MAX_ITEMS - 1);	
				}
				ui_LoadInventoryItem(screen, gamestate, levelstate, &weapon, &item, new_selected_id);
				ui_DrawCharacterScreen_InventoryRedraw(screen, gamestate, levelstate, &weapon, &item, new_selected_id);
				draw_Flip(screen);
				break;
			case INPUT_UP:
				// =====================================
				// Scroll to previous item
				// =====================================
				new_mode = 1;
				if (new_selected_id > 0){
					new_selected_id--;
				} else {
					new_selected_id = 0;	
				}
				ui_LoadInventoryItem(screen, gamestate, levelstate, &weapon, &item, new_selected_id);
				ui_DrawCharacterScreen_InventoryRedraw(screen, gamestate, levelstate, &weapon, &item, new_selected_id);
				draw_Flip(screen);
				break;
				
			case INPUT_U:
			case INPUT_U_:
				// =====================================
				// Use or consume the selected item if it is a consumeable
				// =====================================
				// Check if this is a legit, useable consumeable
				c = ui_DrawBooleanChoice(screen, "Use Item?", "No", "Yes");
				if (c){
					// Use item
				}
				refresh = 1;
				new_mode = 1;
				break;
				
			case INPUT_E:
			case INPUT_E_:
				// =====================================
				// Equip a weapon or armour item
				// =====================================
				// Check if this can be equipped by the current player character
				c = pc_CanEquip(gamestate->players->player[pc_id], &weapon, &item);
				if (c){
					// If armour, just prompt yes/no
					if (item.item_id != 0){
						// Prompt to confirm
						c = ui_DrawBooleanChoice(screen, "Equip Armour?", "No", "Yes");
						if (c){
							// Equip item
							pc_Equip(gamestate->players->player[pc_id], &weapon, &item, 0);
						}
					}
					if (weapon.item_id != 0){
						// Prompt to confirm
						c = ui_DrawBooleanChoice(screen, "Equip Weapon?", "No", "Yes");
						if (c){
							// Weapons prompt for left or right hand
							c = ui_DrawBooleanChoice(screen, "Choose Hand", "Right", "Left");
							if (c == 0){
								// Equip weapon
								pc_Equip(gamestate->players->player[pc_id], &weapon, &item, RIGHT_HAND);
							}
							if (c == 1){
								// Equip weapon
								pc_Equip(gamestate->players->player[pc_id], &weapon, &item, LEFT_HAND);
							}
						}
					}
				} else {
					// Current player cannot equip this item
					ui_DrawEquipError(screen, "This player cannot equip that item.");
				}
				refresh = 1;
				new_mode = 1;
				break;
				
			case INPUT_R:
			case INPUT_R_:
				// =====================================
				// Remove a weapon or armour item
				// =====================================			
				// Check if equipped
				if (pc_IsEquipped(gamestate->players->player[pc_id], &weapon, &item)){
					c = ui_DrawBooleanChoice(screen, "Remove Equipped Item?", "No", "Yes");
					if (c){
						// Auto remove equipped item if it is armour
						if (item.item_id != 0){
							pc_Unequip(gamestate->players->player[pc_id], &weapon, &item);
						}
						if (weapon.item_id != 0){
							// Auto remove equipped weapon if we are not dual wielding the same weapon
							if (gamestate->players->player[pc_id]->weapon_r->item_id != gamestate->players->player[pc_id]->weapon_l->item_id){
								pc_Unequip(gamestate->players->player[pc_id], &weapon, &item);
							} else {
								// If dual wielding the same weapon, prompt for which hand to remove the weapon from
								c = ui_DrawBooleanChoice(screen, "Choose Hand", "Right", "Left");
								if (c == 0){
									// Equip weapon
									pc_Unequip(gamestate->players->player[pc_id], &weapon, &item, RIGHT_HAND);
								}
								if (c == 1){
									// Equip weapon
									pc_Unequip(gamestate->players->player[pc_id], &weapon, &item, LEFT_HAND);
								}
							}
						}
					}
				} else {
					ui_DrawEquipError(screen, "You cannot remove an item that is not equipped.");
				}
				refresh = 1;
				new_mode = 1;
				break;	
				
			case INPUT_D:
			case INPUT_D_:
				// =====================================
				// Drop/destroy an item
				// =====================================
								
				// Is the item currently equipped?
				if ((pc_IsEquipped(gamestate->players->player[pc_id], &weapon, &item)) && (pc_IsLastItem(gamestate->players->player[pc_id], &weapon, &item))){
					// Yes, it is equipped, and is the last item, cannot be dropped
					ui_DrawEquipError(screen, "You cannot drop this item as it is equipped.");
				} else {
					c = ui_DrawBooleanChoice(screen, "Drop Item?", "No", "Yes");
					if (c){
						// Remove one instance of the item
						pc_TakeItem(gamestate->players->player[pc_id], &weapon, &item);
					}
				}
				// Reload the selected item, as the item slot may now be empty
				ui_LoadInventoryItem(screen, gamestate, levelstate, &weapon, &item, new_selected_id);
				refresh = 1;
				new_mode = 1;
				break;
				
			case INPUT_T:
			case INPUT_T_:
				// =====================================
				// Transfer an item to another party character
				// =====================================
				// Is the item currently equipped?
				if ((pc_IsEquipped(gamestate->players->player[pc_id], &weapon, &item)) && (pc_IsLastItem(gamestate->players->player[pc_id], &weapon, &item))){
					// Yes, it is equipped, and is the last item, cannot be dropped
					ui_DrawEquipError(screen, "You cannot transfer this item as it is equipped.");
				} else {
					c = ui_DrawBooleanChoice(screen, "Transfer Item?", "No", "Yes");
					if (c){
						// Show party destination menu
						c = ui_DrawLootDestination(screen, gamestate, levelstate, &weapon, &item);
						if (c){
							// Remove one instance of the item
							pc_TakeItem(gamestate->players->player[pc_id], &weapon, &item);
						}
					}		
				}
				// Reload the selected item, as the item slot may now be empty
				ui_LoadInventoryItem(screen, gamestate, levelstate, &weapon, &item, new_selected_id);
				refresh = 1;
				new_mode = 1;
				break;
				
			case INPUT_CANCEL:
				new_mode = -1;
				e = 1;
				break;
			default:
				if (refresh){
					ui_DrawCharacterScreen_InventoryRedraw(screen, gamestate, levelstate, &weapon, &item, -1);
					ui_DrawCharacterScreen_InventoryRedraw(screen, gamestate, levelstate, &weapon, &item, new_selected_id);
					screen->dirty = 1;
					draw_Flip(screen);
					refresh = 0;
				}
				break;
		}
	}	
	return new_mode;
}

void ui_DrawCharacterScreen_WeaponDamageToString(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, WeaponState_t *weapon, unsigned char dmg_type, unsigned char dice_qty, unsigned char dice_type){
	
	switch(dmg_type){
		case WEAPON_DMG_PHYSICAL:
			sprintf(gamestate->buf + strlen(gamestate->buf), "Physical");
			break;
		case WEAPON_DMG_SLASHING:
			sprintf(gamestate->buf + strlen(gamestate->buf), "Slashing");
			break;
		case WEAPON_DMG_PIERCING:
			sprintf(gamestate->buf + strlen(gamestate->buf), "Piercing");
			break;
		case WEAPON_DMG_BLUNT:
			sprintf(gamestate->buf + strlen(gamestate->buf), "Blunt");
			break;
		case WEAPON_DMG_LIGHTNING:
			sprintf(gamestate->buf + strlen(gamestate->buf), "Lightning");
			break;
		case WEAPON_DMG_ACID:
			sprintf(gamestate->buf + strlen(gamestate->buf), "Acid");
			break;
		case WEAPON_DMG_FIRE:
			sprintf(gamestate->buf + strlen(gamestate->buf), "Fire");
			break;
		case WEAPON_DMG_COLD:
			sprintf(gamestate->buf + strlen(gamestate->buf), "Cold");
			break;
		case WEAPON_DMG_POISON:
			sprintf(gamestate->buf + strlen(gamestate->buf), "Poison");
			break;
		default:
			sprintf(gamestate->buf + strlen(gamestate->buf), "-");
			return;
	}
	
	sprintf(gamestate->buf + strlen(gamestate->buf), " %dD%d", dice_qty, dice_type);

}
	
void ui_DrawCharacterScreen_WeaponsToString(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, WeaponState_t *weapon){
	// Constructs a string in gamestate->buf to print all of the weapon statistics
		
	// Name
	// Handed
	// Class - simple, martial
	// Rarity
	// Proficiency type 1
	// Proficiency type 2
	// Critical damage
	// Damage type 1
	// Damage type 2
	// Damage type 3
	// Versatile
	// Finesse
	// Silvered
	// Bonus
	// Value
	
	sprintf(gamestate->buf, "%s\n", weapon->name);	
	// Handedness
	if (weapon->weapon_type == WEAPON_2HANDED){
		sprintf(gamestate->buf + strlen(gamestate->buf), "2-Handed\n");
	} else {
		sprintf(gamestate->buf + strlen(gamestate->buf), "1-Handed\n");
	}
	// Weapon class
	switch(weapon->weapon_class){
		case WEAPON_CLASS_SIMPLE:
			sprintf(gamestate->buf + strlen(gamestate->buf), "Simple\n");
			break;
		case WEAPON_CLASS_MARTIAL:
			sprintf(gamestate->buf + strlen(gamestate->buf), "Martial\n");
			break;
		case WEAPON_CLASS_RANGED:
			sprintf(gamestate->buf + strlen(gamestate->buf), "Ranged\n");
			break;
		case WEAPON_CLASS_MAGICAL:
			sprintf(gamestate->buf + strlen(gamestate->buf), "Magical\n");
			break;
	}
	// Weapon size
	switch(weapon->size){
		case WEAPON_SIZE_SMALL:
			sprintf(gamestate->buf + strlen(gamestate->buf), "Small\n");
			break;
		case WEAPON_SIZE_MEDIUM:
			sprintf(gamestate->buf + strlen(gamestate->buf), "Medium\n");
			break;
		case WEAPON_SIZE_LARGE:
			sprintf(gamestate->buf + strlen(gamestate->buf), "Large\n");
			break;
		case WEAPON_SIZE_HUGE:
			sprintf(gamestate->buf + strlen(gamestate->buf), "Huge\n");
			break;
	}
	// Weapon rarity
	switch(weapon->rarity){
		case WEAPON_RARITY_COMMON:
			sprintf(gamestate->buf + strlen(gamestate->buf), "Common\n");
			break;
		case WEAPON_RARITY_UNCOMMON:
			sprintf(gamestate->buf + strlen(gamestate->buf), "Uncommon\n");
			break;
		case WEAPON_RARITY_RARE:
			sprintf(gamestate->buf + strlen(gamestate->buf), "Rare\n");
			break;
		case WEAPON_RARITY_LEGENDARY:
			sprintf(gamestate->buf + strlen(gamestate->buf), "Legendary\n");
			break;
	}
	
	// Proficiency #1
	if (weapon->proficiency_1 == 0){
		sprintf(gamestate->buf + strlen(gamestate->buf), "-\n");
	} else {
		sprintf(gamestate->buf + strlen(gamestate->buf), "%s\n", proficiencies[weapon->proficiency_1]);
	}
	// Proficiency #2
	if (weapon->proficiency_2 == 0){
		sprintf(gamestate->buf + strlen(gamestate->buf), "-\n");
	} else {
		sprintf(gamestate->buf + strlen(gamestate->buf), "%s\n", proficiencies[weapon->proficiency_2]);
	}
	
	// Criticals
	sprintf(gamestate->buf + strlen(gamestate->buf), "%d - %d, x%d\n", weapon->crit_min, weapon->crit_max, weapon->crit_dice_qty);
	
	// Dmg 1
	ui_DrawCharacterScreen_WeaponDamageToString(screen, gamestate, levelstate, weapon, weapon->dmg1_type, weapon->dmg1_dice_qty, weapon->dmg1_dice_type);
	sprintf(gamestate->buf + strlen(gamestate->buf), "\n");
	// Dmg 2
	ui_DrawCharacterScreen_WeaponDamageToString(screen, gamestate, levelstate, weapon, weapon->dmg2_type, weapon->dmg2_dice_qty, weapon->dmg2_dice_type);
	sprintf(gamestate->buf + strlen(gamestate->buf), "\n");
	// Dmg 3
	ui_DrawCharacterScreen_WeaponDamageToString(screen, gamestate, levelstate, weapon, weapon->dmg3_type, weapon->dmg3_dice_qty, weapon->dmg3_dice_type);
	sprintf(gamestate->buf + strlen(gamestate->buf), "\n");
	
	// Versatile
	if (weapon->versatile){
		sprintf(gamestate->buf + strlen(gamestate->buf), "Yes\n");	
	} else {
		sprintf(gamestate->buf + strlen(gamestate->buf), "-\n");
	}
	
	// Finesse
	if (weapon->finesse){
		sprintf(gamestate->buf + strlen(gamestate->buf), "Yes\n");	
	} else {
		sprintf(gamestate->buf + strlen(gamestate->buf), "-\n");
	}
	
	// Silvered
	if (weapon->silvered){
		sprintf(gamestate->buf + strlen(gamestate->buf), "Yes\n");	
	} else {
		sprintf(gamestate->buf + strlen(gamestate->buf), "-\n");
	}
	
	// Bonus
	if (weapon->bonus){
		sprintf(gamestate->buf + strlen(gamestate->buf), "+%d\n", weapon->bonus);
	} else {
		sprintf(gamestate->buf + strlen(gamestate->buf), "-\n");
	}
	
	// Value
	sprintf(gamestate->buf + strlen(gamestate->buf), "%ug\n", weapon->value);
}

void ui_DrawCharacterScreen_WeaponsSummary(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned char hand){
	char i;
	unsigned char text_x_column;
	unsigned short gfx_x;
	unsigned char dmg_dice_qty;
	unsigned char dmg_dice_type;
	PlayerState_t *pc;
	char pc_id = gamestate->players->current - 1;
	
	// Load the player character
	pc = gamestate->players->player[pc_id];
	
	if (hand == RIGHT_HAND){
		text_x_column = 1;
		gfx_x = 4;
	} else {
		text_x_column = 33;
		gfx_x = 260;
	}
	
	// Weapon attributes
	sprintf(gamestate->buf, "Name\nHanded\nClass\nSize\nRarity\nSkills\n\nCritical\nDmg.#1\nDmg.#2\nDmg.#3\nVersatile\nFinesse\nSilvered\nBonus\nValue");
	draw_String(screen, text_x_column, 34, 18, 16, 0, screen->font_8x8, PIXEL_WHITE, gamestate->buf, MODE_PIXEL_SET);
	
	// Name of weapon
	ui_DrawCharacterScreen_WeaponsToString(screen, gamestate, levelstate, pc->weapon_r);
	draw_String(screen, text_x_column + 10, 34, 18, 16, 0, screen->font_8x8, PIXEL_WHITE, gamestate->buf, MODE_PIXEL_SET);
	
	// Box to hold calculated attack and damage stats
	draw_Box(screen, gfx_x, 170, 247, 82, 1, PIXEL_GREEN, PIXEL_CLEAR, MODE_PIXEL_OR);
	// Vertical divider in box
	draw_VLine(screen, gfx_x + 120, 170, 82, PIXEL_GREEN, MODE_PIXEL_OR);
	
	// Draw the bonus from weapon proficiencies into left hand divider of box
	sprintf(gamestate->buf, "<r>Attack Roll<C>\n\n%dD%d\n", ATTACK_DICE_QTY, ATTACK_DICE_TYPE);
	i = player_weapon_AttackModifier(pc, pc->weapon_r, 1, 0, 0, 0, 0);
	if (i >= 0){
		sprintf(gamestate->buf + strlen(gamestate->buf), "+%d Skills\n", i);
	} else {
		sprintf(gamestate->buf + strlen(gamestate->buf), "%d Skills\n", i);
	}
	i = player_weapon_AttackModifier(pc, pc->weapon_r, 0, 1, 0, 0, 0);
	if (i >= 0){
		sprintf(gamestate->buf + strlen(gamestate->buf), "+%d Bonus\n", i);
	} else {
		sprintf(gamestate->buf + strlen(gamestate->buf), "%d Bonus\n", i);
	}
	i = player_weapon_AttackModifier(pc, pc->weapon_r, 0, 0, 1, 0, 0);
	if (i >= 0){
		sprintf(gamestate->buf + strlen(gamestate->buf), "+%d Attributes\n", i);
	} else {
		sprintf(gamestate->buf + strlen(gamestate->buf), "%d Attributes\n", i);
	}
	i = player_weapon_AttackModifier(pc, pc->weapon_r, 0, 0, 0, 1, 0);
	if (i >= 0){
		sprintf(gamestate->buf + strlen(gamestate->buf), "+%d Status\n", i);
	} else {
		sprintf(gamestate->buf + strlen(gamestate->buf), "%d Status\n", i);
	}
	i = player_weapon_AttackModifier(pc, pc->weapon_r, 0, 0, 0, 0, 1);
	if (i >= 0){
		sprintf(gamestate->buf + strlen(gamestate->buf), "+%d Status\n", i);
	} else {
		sprintf(gamestate->buf + strlen(gamestate->buf), "%d Status\n", i);
	}
	draw_String(screen, text_x_column, 174, 30, 9, 0, screen->font_8x8, PIXEL_GREEN, gamestate->buf, MODE_PIXEL_SET);

	
	// Damage drawn into the right hand divider of box
	sprintf(gamestate->buf, "<r>Damage Roll<C>\n\n");
	dmg_dice_qty = pc->weapon_r->dmg1_dice_qty;
	dmg_dice_type = pc->weapon_r->dmg1_dice_type;
	i = player_weapon_DamageModifier(pc, pc->weapon_r, pc->weapon_r->dmg1_type, &dmg_dice_type, &dmg_dice_qty, 1, 1, 0, 1, 1);
	if (i >= 0){
		sprintf(gamestate->buf + strlen(gamestate->buf), "Dmg.#1 %dD%d +%d\n", dmg_dice_qty, dmg_dice_type, i);
	} else {
		sprintf(gamestate->buf + strlen(gamestate->buf), "Dmg.#1 %dD%d %d\n", dmg_dice_qty, dmg_dice_type, i);	
	}

	if (pc->weapon_r->dmg2_type){
		dmg_dice_qty = pc->weapon_r->dmg2_dice_qty;
		dmg_dice_type = pc->weapon_r->dmg2_dice_type;
		i = player_weapon_DamageModifier(pc, pc->weapon_r, pc->weapon_r->dmg2_type, &dmg_dice_type, &dmg_dice_qty, 1, 1, 0, 1, 1);
		if (i >= 0){
			sprintf(gamestate->buf + strlen(gamestate->buf), "Dmg.#2 %dD%d +%d\n", dmg_dice_qty, dmg_dice_type, i);
		} else {
			sprintf(gamestate->buf + strlen(gamestate->buf), "Dmg.#2 %dD%d %d\n", dmg_dice_qty, dmg_dice_type, i);
		}	
	}
	
	if (pc->weapon_r->dmg3_type){
		dmg_dice_qty = pc->weapon_r->dmg3_dice_qty;
		dmg_dice_type = pc->weapon_r->dmg3_dice_type;
		i = player_weapon_DamageModifier(pc, pc->weapon_r, pc->weapon_r->dmg2_type, &dmg_dice_type, &dmg_dice_qty, 1, 1, 0, 1, 1);
		if (i >= 0){
			sprintf(gamestate->buf + strlen(gamestate->buf), "Dmg.#3 %dD%d +%d\n", dmg_dice_qty, dmg_dice_type, i);
		} else {
			sprintf(gamestate->buf + strlen(gamestate->buf), "Dmg.#3 %dD%d %d\n", dmg_dice_qty, dmg_dice_type, i);	
		}
	}
	
	if ((pc->weapon_r->item_id == 0) || (pc->weapon_l->item_id == 0)){
		if (pc->weapon_r->versatile){
			sprintf(gamestate->buf + strlen(gamestate->buf), "+ Versatile!\n");
		}
		if (pc->weapon_r->weapon_type == WEAPON_2HANDED){
			sprintf(gamestate->buf + strlen(gamestate->buf), "+ 2-Handed!\n");
		}
	}
	
	draw_String(screen, text_x_column + 16, 174, 30, 9, 0, screen->font_8x8, PIXEL_GREEN, gamestate->buf, MODE_PIXEL_SET);
	
}

char ui_DrawCharacterScreen_Weapons(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate){

	unsigned char c;
	char new_mode;
	unsigned char e = 0;
	PlayerState_t *pc;
	char pc_id = gamestate->players->current - 1;
	
	// Load the player character
	pc = gamestate->players->player[pc_id];
	
	// Titles for each half of the screen
	draw_String(screen, 1, 18, 24, 4, 0, screen->font_8x8, PIXEL_RED, "Right Hand", MODE_PIXEL_SET);
	draw_String(screen, 33, 18, 24, 4, 0, screen->font_8x8, PIXEL_RED, "Left Hand", MODE_PIXEL_SET);
	
	// Central divider
	draw_VLine(screen, 255, 20, 232, PIXEL_RED, MODE_PIXEL_SET);	
	
	// Right hand weapon
	if (pc->weapon_r->item_id == 0){
		draw_String(screen, 1, 34, 24, 1, 0, screen->font_8x8, PIXEL_GREEN, "Nothing equipped", MODE_PIXEL_SET);			
	} else {
		ui_DrawCharacterScreen_WeaponsSummary(screen, gamestate, levelstate, RIGHT_HAND);		
	}
	
	// Left hand weapon
	if (pc->weapon_l->item_id == 0){
		draw_String(screen, 33, 34, 24, 1, 0, screen->font_8x8, PIXEL_GREEN, "Nothing equipped", MODE_PIXEL_SET);
	} else {
		ui_DrawCharacterScreen_WeaponsSummary(screen, gamestate, levelstate, LEFT_HAND);		
	}
	
	while(!e){
		c = input_Get(screen);
		switch(c){
			case INPUT_OVERVIEW:
			case INPUT_OVERVIEW_:
				new_mode = 0;
				e = 1;
				break;
			case INPUT_ITEMS:
			case INPUT_ITEMS_:
				new_mode = 1;
				e = 1;
				break;
			case INPUT_WEAPONS:
			case INPUT_WEAPONS_:
				new_mode = 2;
				e = 1;
				break;
			case INPUT_MAGIC:
			case INPUT_MAGIC_:
				new_mode = 3;
				e = 1;
				break;
			case INPUT_STATUS:
			case INPUT_STATUS_:
				new_mode = 4;
				e = 1;
				break;
			case INPUT_CANCEL:
				new_mode = -1;
				e = 1;
				break;
			default:
				break;
		}
	}	
	return new_mode;
}

char ui_DrawCharacterScreen_Status(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate){
	// Draw the status screen of all active effects on the character
	
	unsigned char c;
	char new_mode;
	unsigned char e = 0;
	PlayerState_t *pc;
	unsigned char i;
	char pc_id = gamestate->players->current - 1;
		
	// Load the player character
	pc = gamestate->players->player[pc_id];
	
	// Column 1 of effects
	for (i = 0; i < 16; i++){
		draw_Box(screen, 30, 18 + (i * 12), 8, 8, 1, PIXEL_RED, PIXEL_CLEAR, MODE_PIXEL_SET);	
		draw_String(screen, 6, 19 + (i * 12), 16, 1, 0, screen->font_8x8, PIXEL_WHITE, status_effects[i], MODE_PIXEL_SET);
		
		// Load item - display name
	}
	
	// Column 2 of effects
	for (i = 0; i < 16; i++){
		draw_Box(screen, 300, 18 + (i * 12), 8, 8, 1, PIXEL_RED, PIXEL_CLEAR, MODE_PIXEL_SET);	
		draw_String(screen, 40, 19 + (i * 12), 16, 1, 0, screen->font_8x8, PIXEL_WHITE, status_effects[(16 + i)], MODE_PIXEL_SET);
		
		// Load item - display name
	}	
	
	while(!e){
		c = input_Get(screen);
		switch(c){
			case INPUT_OVERVIEW:
			case INPUT_OVERVIEW_:
				new_mode = 0;
				e = 1;
				break;
			case INPUT_ITEMS:
			case INPUT_ITEMS_:
				new_mode = 1;
				e = 1;
				break;
			case INPUT_WEAPONS:
			case INPUT_WEAPONS_:
				new_mode = 2;
				e = 1;
				break;
			case INPUT_MAGIC:
			case INPUT_MAGIC_:
				new_mode = 3;
				e = 1;
				break;
			case INPUT_STATUS:
			case INPUT_STATUS_:
				new_mode = 4;
				e = 1;
				break;
			case INPUT_CANCEL:
				new_mode = -1;
				e = 1;
				break;
			default:
				break;
		}
	}	
	return new_mode;
}

char ui_DrawCharacterScreen(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned char tab_id){
	// Draw the player character detail screen to check their status, inventory etc

	char modestring[16];
	char new_mode = 0;
	unsigned char c;
	unsigned char e = 0;
	
	input_Clear();
	input_Set(INPUT_CANCEL);
	input_Set(INPUT_OVERVIEW);
	input_Set(INPUT_OVERVIEW_);
	input_Set(INPUT_ITEMS);
	input_Set(INPUT_ITEMS_);
	input_Set(INPUT_WEAPONS);
	input_Set(INPUT_WEAPONS_);
	input_Set(INPUT_MAGIC);
	input_Set(INPUT_MAGIC_);
	input_Set(INPUT_STATUS);
	input_Set(INPUT_STATUS_);
	
	// Basic screen clear and outline
	draw_Clear(screen);
	draw_Box(screen, 0, 10, SCREEN_WIDTH, SCREEN_HEIGHT - 11, 1, PIXEL_RED, PIXEL_CLEAR, MODE_PIXEL_SET);
	
	// Draw all tabs
	// Overview
	draw_Box(screen, 0, 0, 101, 10, 1, PIXEL_RED, PIXEL_CLEAR, MODE_PIXEL_OR);
	draw_String(screen, 0 + 2, 2, 12, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>O<C>verview", MODE_PIXEL_OR);
	
	// Inventory
	draw_Box(screen, 102, 0, 101, 10, 1, PIXEL_RED, PIXEL_CLEAR, MODE_PIXEL_OR);
	draw_String(screen, 102 / 8 + 2, 2, 12, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>I<C>tems", MODE_PIXEL_OR);
	
	// Equipment
	draw_Box(screen, 204, 0, 101, 10, 1, PIXEL_RED, PIXEL_CLEAR, MODE_PIXEL_OR);
	draw_String(screen, 204 / 8 + 2, 2, 12, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>W<C>eapons", MODE_PIXEL_OR);
	
	// Magic
	draw_Box(screen, 306, 0, 101, 10, 1, PIXEL_RED, PIXEL_CLEAR, MODE_PIXEL_OR);
	draw_String(screen, 306 / 8 + 2, 2, 12, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>M<C>agic", MODE_PIXEL_OR);
	
	// Status
	draw_Box(screen, 408, 0, 104, 10, 1, PIXEL_RED, PIXEL_CLEAR, MODE_PIXEL_OR);
	draw_String(screen, 408 / 8 + 2, 2, 12, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>S<C>tatus", MODE_PIXEL_OR);
	
	// Draw the selected tab
	switch(tab_id){
		case 0:
			strcpy(modestring, "Overview");
			break;
		case 1:
			strcpy(modestring, "Items");
			break;
		case 2:
			strcpy(modestring, "Weapons");
			break;
		case 3:
			strcpy(modestring, "Magic");
			break;
		case 4:
			strcpy(modestring, "Status");
			break;
	}
	draw_Box(screen, tab_id * 102, 0, 101, 10, 1, PIXEL_RED, PIXEL_RED, MODE_PIXEL_OR);
	draw_String(screen, (tab_id * 102) / 8 + 2, 2, strlen(modestring), 1, 0, screen->font_8x8, PIXEL_WHITE, modestring, MODE_PIXEL_OR);
	
	//=======================================
	// Main character overview display 
	//=======================================
	switch(tab_id){
		case 0:
			new_mode = ui_DrawCharacterScreen_Overview(screen, gamestate, levelstate);
			break;
		case 1:
			new_mode = ui_DrawCharacterScreen_Inventory(screen, gamestate, levelstate);
			break;
		case 2:
			new_mode = ui_DrawCharacterScreen_Weapons(screen, gamestate, levelstate);
			break;
		case 4:
			new_mode = ui_DrawCharacterScreen_Status(screen, gamestate, levelstate);
			break;
	}
	
	// Draw overview UI
	// Draw 
	
	//draw_Flip(screen);
	
	return new_mode;
}

void ui_DrawSideBar(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate){
	// Draws the sidebar with the player party portraits, names, basic status, etc
	
	unsigned char i;		// Loop counter
	char buf[20];			// Temporary string buffer
	
	// Draw player stats
	for (i = 0; i <= 3; i++){
		
		// Only draw stats of existing part
		// characters
		if (gamestate->players->player[i]->level){

			// Draw field titles
			draw_String(screen, UI_SIDEBAR_STAT_TEXT_X, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i) + 1, 3, 4, 0, screen->font_8x8, PIXEL_WHITE, "HP:\nLv:\nFm:\nSt:", MODE_PIXEL_SET);
			
			// ===================================================
			// Hitpoints
			// Shows a colour-coded display of the current players
			// health:
			// - Green if HP over 50%
			// - Red if HP under 50%
			// ===================================================
			
			sprintf(buf, "%03d", gamestate->players->player[i]->hp);
			if (((gamestate->players->player[i]->hp * 100) / gamestate->players->player[i]->hp_reset) > UI_HP_WARN_LEVEL){
				draw_String(screen, UI_SIDEBAR_STAT_VALUES_X, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i) + 1, 3, 1, 0, screen->font_8x8, PIXEL_GREEN, buf, MODE_PIXEL_SET);
			} else {
				draw_String(screen, UI_SIDEBAR_STAT_VALUES_X, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i) + 1, 3, 1, 0, screen->font_8x8, PIXEL_RED, buf, MODE_PIXEL_SET);
			}
			
			// ===================================================
			// Level
			// Just shows the level number of the current player
			// ===================================================
			sprintf(buf, "%03d", gamestate->players->player[i]->level);
			draw_String(screen, UI_SIDEBAR_STAT_VALUES_X, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i) + 9, 3, 1, 0, screen->font_8x8, PIXEL_WHITE, buf, MODE_PIXEL_SET);
		
			// ===================================================
			// Combat formation
			// Can display:
			// - Front
			// - Mid
			// - Rear
			// ===================================================
			if (gamestate->players->player[i]->formation == FORMATION_FRONT){
				sprintf(buf, "Front");
			}
			if (gamestate->players->player[i]->formation == FORMATION_MID){
				sprintf(buf, "Mid");
			}
			if (gamestate->players->player[i]->formation == FORMATION_REAR){
				sprintf(buf, "Rear");
			}
			draw_String(screen, UI_SIDEBAR_STAT_VALUES_X, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i) + 17, 5, 1, 0, screen->font_8x8, PIXEL_WHITE, buf, MODE_PIXEL_SET);
			
			// ===================================================
			// Status
			// Green if ok
			// Red if any status effects
			// ===================================================
			if (gamestate->players->player[i]->status == STATUS_OK){
				sprintf(buf, "<g>Good");
			} else {
				sprintf(buf, "<r>Check");
			}
			draw_String(screen, UI_SIDEBAR_STAT_VALUES_X, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i) + 25, 8, 1, 0, screen->font_8x8, PIXEL_WHITE, buf, MODE_PIXEL_SET);
		
			// Name
			sprintf(buf, "%d: %s", (i + 1), gamestate->players->player[i]->short_name);
			draw_String(screen, UI_SIDEBAR_PC_NAME_X, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i) + 36, 12, 1, 0, screen->font_8x8, PIXEL_WHITE, buf, MODE_PIXEL_SET);
		} else {
			// Name
			sprintf(buf, "%d: ----", (i + 1));
			draw_String(screen, UI_SIDEBAR_PC_NAME_X, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i) + 36, 12, 1, 0, screen->font_8x8, PIXEL_WHITE, buf, MODE_PIXEL_SET);
		}
	}
	
	// Draw 4 player head boxes and 4 player portrait sprites
	for (i = 0; i <= 3; i++){
		if (i > 0){
			draw_HLine(screen, UI_SIDEBAR_START_X, UI_SIDEBAR_START_Y + (UI_SIDEBAR_BLOCK * i), SCREEN_WIDTH - UI_SIDEBAR_START_X, PIXEL_GREEN, 0, MODE_PIXEL_OR);	
		}
		
		draw_Box(screen, UI_SIDEBAR_PORTRAIT_X, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i), DRAW_PORTRAIT_WIDTH + 1, DRAW_PORTRAIT_HEIGHT + 1, 1, PIXEL_RED, PIXEL_BLACK, MODE_PIXEL_OR);
		
		if (gamestate->players->player[i]->level > 0){
			// Only draw portraits of existing party characters
			draw_Sprite(screen, UI_SIDEBAR_PORTRAIT_X, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i) + 1, screen->players[i], 1);
		} else {
			// Otherwise draw a placeholder portrait for a default character
			draw_Sprite(screen, UI_SIDEBAR_PORTRAIT_X, UI_SIDEBAR_PORTRAIT_Y + (UI_SIDEBAR_BLOCK * i) + 1, screen->players[i], 1);
		}
	}
	
	screen->dirty = 1;
}

void ui_DrawStatusBar(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned char buttons, unsigned char labels){
	// Draws the available options in the bottom status bar - these are the keys we
	// can press or the options we can take
	
	unsigned char i;
	
	// Clear the status bar
	draw_Box(screen, 8, UI_STATUSBAR_START_Y + 4, SCREEN_WIDTH - 16, (SCREEN_HEIGHT - UI_STATUSBAR_START_Y) - 8, 0, PIXEL_CLEAR, PIXEL_BLACK, MODE_PIXEL_SET);
	
	// Draw blank buttons
	if (buttons){
		draw_Box(screen, 6, 236, 60, 14, 2, PIXEL_RED_STIPPLED, PIXEL_CLEAR, MODE_PIXEL_SET);
		draw_Box(screen, 70, 236, 60, 14, 2, PIXEL_RED_STIPPLED, PIXEL_CLEAR, MODE_PIXEL_SET);
		draw_Box(screen, 134, 236, 60, 14, 2, PIXEL_RED_STIPPLED, PIXEL_CLEAR, MODE_PIXEL_SET);
		draw_Box(screen, 198, 236, 82, 14, 2, PIXEL_RED_STIPPLED, PIXEL_CLEAR, MODE_PIXEL_SET);
		draw_Box(screen, 284, 236, 56, 14, 2, PIXEL_RED_STIPPLED, PIXEL_CLEAR, MODE_PIXEL_SET);
		draw_Box(screen, 344, 236, 62, 14, 2, PIXEL_RED_STIPPLED, PIXEL_CLEAR, MODE_PIXEL_SET);
		draw_Box(screen, 410, 236, 66, 14, 2, PIXEL_RED_STIPPLED, PIXEL_CLEAR, MODE_PIXEL_SET);
		draw_Box(screen, 480, 236, 24, 14, 2, PIXEL_RED_STIPPLED, PIXEL_CLEAR, MODE_PIXEL_SET);
	}
	
	// Draw labels on the buttons
	if (labels){
		for (i = 0; i < MAX_ALLOWED_INPUTS; i++){
			if ((input_allowed[i] == INPUT_MOVE) || (input_allowed[i] == INPUT_MOVE_)){
				draw_String(screen, 2, 240, 8, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>M<C>ove", MODE_PIXEL_SET);
			}
				
			if ((input_allowed[i] == INPUT_TALK) || (input_allowed[i] == INPUT_TALK_)){
				draw_String(screen, 10, 240, 8, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>T<C>alk", MODE_PIXEL_SET);			
			}
			
			// FIGHT and LOOT are mutually exclusive - so they display in the same option box
			if ((input_allowed[i] == INPUT_FIGHT) || (input_allowed[i] == INPUT_FIGHT_)){
				draw_String(screen, 18, 240, 8, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>F<C>ight", MODE_PIXEL_SET);
			}
			if ((input_allowed[i] == INPUT_LOOT) || (input_allowed[i] == INPUT_LOOT_)){
				draw_String(screen, 18, 240, 8, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>L<C>oot", MODE_PIXEL_SET);
			}
			
			if ((input_allowed[i] == INPUT_WITHDRAW) || (input_allowed[i] == INPUT_WITHDRAW_)){
				draw_String(screen, 26, 240, 12, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>W<C>ithdraw", MODE_PIXEL_SET);
			}
			
			if ((input_allowed[i] == INPUT_REST) || (input_allowed[i] == INPUT_REST_)){
				draw_String(screen, 37, 240, 12, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>R<C>est", MODE_PIXEL_SET);
			}
			
			if ((input_allowed[i] == INPUT_BARTER) || (input_allowed[i] == INPUT_BARTER_)){
				draw_String(screen, 44, 240, 12, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>B<C>arter", MODE_PIXEL_SET);
			}
			
			if ((input_allowed[i] == INPUT_N) || (input_allowed[i] == INPUT_N_)){
				draw_String(screen, 52, 240, 12, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>N<C>ext", MODE_PIXEL_SET);
			}
			
			if ((input_allowed[i] == INPUT_QUIT) || (input_allowed[i] == INPUT_QUIT_)){
				draw_String(screen, 61, 240, 12, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>Q", MODE_PIXEL_SET);
			}	
		}
	}
	screen->dirty = 1;
}

char ui_DrawLootDestinationRedraw(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, char selected_id, unsigned char selected){
	// Draw the selection boxes next to player characters to indicate which is currently selected
	// to transfer an item to.
	unsigned short fill_colour;
	unsigned char i;
	char slots;
	
	for (i = 0; i<MAX_PLAYERS; i++){
		if (selected_id == i){
			if (selected){
				fill_colour = PIXEL_RED;
			} else {
				fill_colour = PIXEL_WHITE;
			}
		} else {
			fill_colour = PIXEL_BLACK;
		}
		draw_Box(screen, UI_LOOT_START_X + 204, UI_LOOT_START_Y + 14 + (i * 12), 8, 8, 1, PIXEL_RED, fill_colour, MODE_PIXEL_SET);
	}
}

char ui_DrawLootDestination(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, WeaponState_t *weapon, ItemState_t *item){
	// Select a player character to send the loot item to
	
	char slots;
	unsigned char i;
	unsigned char c;
	unsigned char e = 0;
	char selected_id = -1;
	unsigned char looted = 0;
	
	ui_DrawPopup(screen, UI_LOOT_START_X + 200, UI_LOOT_START_Y, UI_LOOT_WIDTH, 64, "Choose Player", 1);
	
	// Draw all the available players with free slots for this item
	for (i = 0; i<MAX_PLAYERS; i++){
		if (gamestate->players->player[i]->level){
			slots = pc_HasSlots(gamestate->players->player[i], weapon, item);
			if (slots >= 0){
				// This character has slots
				sprintf(gamestate->buf, gamestate->players->player[i]->name);
			} else {
				// This character has no slots
				sprintf(gamestate->buf, "- No Slots");
			}
		} else {
			// This character is not present
			sprintf(gamestate->buf, "- Not Present");
		}
		draw_String(screen, (UI_LOOT_START_X + 220) / 8, UI_LOOT_START_Y + 14 + (i * 12) + 1, 21, 1, 0, screen->font_8x8, PIXEL_WHITE, gamestate->buf, MODE_PIXEL_SET);
	}
	ui_DrawLootDestinationRedraw(screen, gamestate, levelstate, selected_id, 0);
	
	while(!e){
		c = input_Get(screen);
		switch(c){
			case INPUT_UP:
				if (selected_id > 0){
					selected_id--;	
				} else {
					selected_id = 0;
				}
				ui_DrawLootDestinationRedraw(screen, gamestate, levelstate, selected_id, 0);
				draw_Flip(screen);
				break;
			case INPUT_DOWN:
				if (selected_id < (MAX_PLAYERS - 1)){
					selected_id++;	
				} else {
					selected_id = (MAX_PLAYERS - 1);
				}
				ui_DrawLootDestinationRedraw(screen, gamestate, levelstate, selected_id, 0);
				draw_Flip(screen);
				break;
			case INPUT_CONFIRM:
				// Only proceed with the item transfer if the destination is legitimate
				if (selected_id >= 0){
					slots = pc_HasSlots(gamestate->players->player[selected_id], weapon, item);
					if (slots >= 0){
						ui_DrawLootDestinationRedraw(screen, gamestate, levelstate, selected_id, 1);

						// Flash selected player name
						draw_SelectedString(screen, (UI_LOOT_START_X + 220) / 8, UI_LOOT_START_Y + 14 + (selected_id * 12) + 1, strlen(gamestate->players->player[selected_id]->name), PIXEL_WHITE, gamestate->players->player[selected_id]->name);	
						draw_String(screen, (UI_LOOT_START_X + 220) / 8, UI_LOOT_START_Y + 14 + (selected_id * 12) + 1, 21, 1, 0, screen->font_8x8, PIXEL_WHITE, gamestate->players->player[selected_id]->name, MODE_PIXEL_SET);
						draw_Flip(screen);
					
						// Copy item to character
						pc_GiveItem(gamestate->players->player[selected_id], weapon, item);
						looted = 1;
						
						// Redraw window without any character selected
						ui_DrawLootDestinationRedraw(screen, gamestate, levelstate, -1, 0);
						draw_Flip(screen);
						
						// Exit back to loot item selection window
						e = 1;
					}
				}
				break;
			case INPUT_CANCEL:
				// Clear any selection marker on the destination player window
				ui_DrawLootDestinationRedraw(screen, gamestate, levelstate, -1, 0);
				e = 1;
				break;
			default:
				break;
		}
	}
	// Tell the loot item selection window if we transferred anything
	return looted;
}

void ui_DrawLootChoiceRedraw(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned char location_loot, unsigned char enemy_loot, char selected_id, unsigned char selected){
	// Show the highlighted loot item
	WeaponState_t weapon;
	ItemState_t item;
	unsigned char total_loot = 0;
	unsigned char i = 0;
	unsigned char i2 = 0;
	unsigned short fill_colour = PIXEL_BLACK;
		
	// Print out all lootable items for this location
	if (location_loot){
		for (i = 0; i < levelstate->weapons_number; i++){
			// Is this item selected?
			if (selected_id == i){
				// Yes
				
				// Have we pressed enter?
				if (selected){
					// Yes
					fill_colour = PIXEL_RED;
				} else {
					// No
					fill_colour = PIXEL_WHITE;
				}
			} else {
				// No
				fill_colour = PIXEL_BLACK;
			}
			draw_Box(screen, UI_LOOT_START_X + 4, UI_LOOT_START_Y + 14 + (i * 12), 8, 8, 1, PIXEL_RED, fill_colour, MODE_PIXEL_SET);
			i2++;
		}
		for (i = 0; i < levelstate->items_number; i++){
			if (selected_id == i2){
				if (selected){
					fill_colour = PIXEL_RED;
				} else {
					fill_colour = PIXEL_WHITE;
				}
			} else {
				fill_colour = PIXEL_BLACK;
			}
			draw_Box(screen, UI_LOOT_START_X + 4, UI_LOOT_START_Y + 14 + (i2 * 12), 8, 8, 1, PIXEL_RED, fill_colour, MODE_PIXEL_SET);
			i2++;
		}
	}
		
	i2 = 0;
	if (location_loot){
		// Draw all map location loot items
		for (i = 0; i < levelstate->weapons_number; i++){
			if (selected_id == -1){
				if (levelstate->weapons_list[i] != 0){
					data_LoadWeapon(screen, &weapon, levelstate->weapons_list[i]);
					sprintf(gamestate->buf, " %s", weapon.name);
				} else {
					sprintf(gamestate->buf, " - Empty           ");
				}
				draw_String(screen, (UI_LOOT_START_X / 8) + 3, UI_LOOT_START_Y + 14 + (i2 * 12) + 1, 21, 1, 0, screen->font_8x8, PIXEL_WHITE, gamestate->buf, MODE_PIXEL_SET);
			}
			i2++;
		}
		for (i = 0; i < levelstate->items_number; i++){
			if (selected_id == -1){
				if (levelstate->items_list[i] != 0){
					data_LoadItem(screen, &item, levelstate->items_list[i]);
					sprintf(gamestate->buf, " %s", item.name);
				} else {
					sprintf(gamestate->buf, " - Empty           ");
				}
				draw_String(screen, (UI_LOOT_START_X / 8) + 3, UI_LOOT_START_Y + 14 + (i2 * 12) + 1, 21, 1, 0, screen->font_8x8, PIXEL_WHITE, gamestate->buf, MODE_PIXEL_SET);
			}
			i2++;
		}
	}
	
	// Draw footer
	if (selected_id == -1){
		draw_String(screen, (UI_LOOT_START_X / 8) + 10, UI_LOOT_START_Y + 16 + (i2 * 12), 21, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>Ent<C>er/<r>Esc<C>ape", MODE_PIXEL_SET);
	}
	
	// Print out boxes for all enemy loot
	if (enemy_loot){
		
	}
		
	screen->dirty = 1;
}
	
char ui_DrawLootChoice(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned char location_loot, unsigned char enemy_loot){
	// Draw the list of available loot choices
	unsigned char i;
	unsigned char i2 = 0;
	unsigned char c;
	unsigned char e = 0;			// Exit
	unsigned char total_loot = 0;
	char selected_id = -1;
	unsigned char pc_id = 0;
	unsigned char looted = 0;
	WeaponState_t weapon;
	ItemState_t item;
	
	// Total number of items of loot in this location
	if (location_loot){
		total_loot = levelstate->weapons_number + levelstate->items_number;	
	}
	
	// Total number of items on dead enemy characters
	if (enemy_loot){
		total_loot = 0;
	}
	sprintf(gamestate->buf, "Lootable Items (%d)", total_loot);
	ui_DrawPopup(screen, UI_LOOT_START_X, UI_LOOT_START_Y, UI_LOOT_WIDTH, 14 + ((total_loot + 1) * 12), gamestate->buf, 1);
	ui_DrawLootChoiceRedraw(screen, gamestate, levelstate, location_loot, enemy_loot, selected_id, 0);
	
	screen->dirty = 1;
	draw_Flip(screen);
	
	// Add up/down cursor input to allow selection of items from inventory
	input_Set(INPUT_UP);
	input_Set(INPUT_DOWN);
	input_Set(INPUT_CONFIRM);
	
	while(!e){
		c = input_Get(screen);
		switch(c){
			case INPUT_UP:
				if (selected_id > 0){
					selected_id--;	
				} else {
					selected_id = 0;
				}
				ui_DrawLootChoiceRedraw(screen, gamestate, levelstate, location_loot, enemy_loot, selected_id, 0);
				draw_Flip(screen);
				break;
			case INPUT_DOWN:
				if (selected_id < (total_loot - 1)){
					selected_id++;	
				} else {
					selected_id = (total_loot - 1);
				}
				ui_DrawLootChoiceRedraw(screen, gamestate, levelstate, location_loot, enemy_loot, selected_id, 0);
				draw_Flip(screen);
				break;
			case INPUT_CONFIRM:
				if (selected_id >= 0){
					ui_DrawLootChoiceRedraw(screen, gamestate, levelstate, location_loot, enemy_loot, selected_id, 1);
					draw_Flip(screen);
					// Load the selected weapon or item
					if (location_loot){
						if (selected_id < levelstate->weapons_number){
							// Load weapon from location loot
							data_LoadWeapon(screen, &weapon, levelstate->weapons_list[selected_id]);
							item.item_id = 0;
						} else {
							// Load item from location loot
							data_LoadItem(screen, &item, levelstate->items_list[(selected_id - levelstate->weapons_number)]);
							weapon.item_id = 0;
						}
					}
					if (enemy_loot){
						// TO DO !!!!!
					}
					// Bring up the party selection window... the return code 'looted' tells us
					// if the highlighted item was transferred or not
					looted = ui_DrawLootDestination(screen, gamestate, levelstate, &weapon, &item);

					// If we transferred the item - redraw the list of items, minus the transferred item
					if (looted){
						if (location_loot){
							if (selected_id < levelstate->weapons_number){
								levelstate->weapons_list[selected_id] = 0;
							} else {
								levelstate->items_list[(selected_id - levelstate->weapons_number)] = 0;
							}
							
							// Record this location as having been looted
							if (gamestate->level_looted[levelstate->id] < 255){
								gamestate->level_looted[levelstate->id]++;
							}
						}
						if (enemy_loot){
							// TO DO !!!!!
						}
						ui_DrawLootChoiceRedraw(screen, gamestate, levelstate, location_loot, enemy_loot, -1, 0);
					}
					
					// Otherwise just reset the selected box back to a normal button
					ui_DrawLootChoiceRedraw(screen, gamestate, levelstate, location_loot, enemy_loot, selected_id, 0);
				}
				draw_Flip(screen);
				break;
			case INPUT_CANCEL:
				e = 1;
				if (looted && location_loot){
					// Mark this location as looted
					if (gamestate->level_looted[levelstate->id] < 255){
						gamestate->level_looted[levelstate->id]++;
					}
				}
				break;
			default:
				break;
		}
	}
}

void ui_DrawNavigationChoice(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate){
	// Overlays a navigation window with the available exits for a given level
	
	
	unsigned char i;
	unsigned char c;
	unsigned char e = 0;		// Exit
	unsigned char row = 2;		// Start exit labels offset below the window title
	unsigned char have_n = 0;
	unsigned char have_s = 0;
	unsigned char have_e = 0;
	unsigned char have_w = 0;
	
	ui_DrawPopup(screen, UI_NAVBOX_START_X, UI_NAVBOX_START_Y, UI_NAVBOX_WIDTH, UI_NAVBOX_HEIGHT, "Destination", 1);
	
	// Only print out the current allowed navigation options
	for (i = 0; i < MAX_ALLOWED_INPUTS; i++){
		if ((input_allowed[i] == INPUT_N) || (input_allowed[i] == INPUT_N_)){
			if (!have_n){
				draw_String(screen, UI_NAVBOX_TEXT_X, UI_NAVBOX_TEXT_Y + (10), 8, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>N<C>orth", MODE_PIXEL_SET);
				have_n = 1;
			}
		}
		if ((input_allowed[i] == INPUT_S) || (input_allowed[i] == INPUT_S_)){
			if (!have_s){
				draw_String(screen, UI_NAVBOX_TEXT_X, UI_NAVBOX_TEXT_Y + (18), 8, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>S<C>outh", MODE_PIXEL_SET);
				row += 2;
				have_s = 1;
			}
		}
		if ((input_allowed[i] == INPUT_E) || (input_allowed[i] == INPUT_E_)){
			if (!have_e){
				draw_String(screen, UI_NAVBOX_TEXT_X, UI_NAVBOX_TEXT_Y + (26), 8, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>E<C>ast", MODE_PIXEL_SET);
				row += 2;
				have_e = 1;
			}
		}
		if ((input_allowed[i] == INPUT_W) || (input_allowed[i] == INPUT_W_)){
			if (!have_w){
				draw_String(screen, UI_NAVBOX_TEXT_X, UI_NAVBOX_TEXT_Y + (34), 8, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>W<C>est", MODE_PIXEL_SET);
				row += 2;
				have_w = 1;
			}
		}
	}
		
	screen->dirty = 1;
	draw_Flip(screen);
	
	while(!e){
		c = input_Get(screen);
		switch(c){
			case INPUT_N:
			case INPUT_N_:
				// Highlight option
				draw_SelectedString(screen, UI_NAVBOX_TEXT_X, UI_NAVBOX_TEXT_Y + (10), 5, PIXEL_WHITE, "North");				
				gamestate->level_previous = gamestate->level;
				gamestate->level = levelstate->north;
				e = 1;
				break;
			case INPUT_S:
			case INPUT_S_:
				draw_SelectedString(screen, UI_NAVBOX_TEXT_X, UI_NAVBOX_TEXT_Y + (18), 5, PIXEL_WHITE, "South");
				gamestate->level_previous = gamestate->level;
				gamestate->level = levelstate->south;
				e = 1;
				break;
			case INPUT_E:
			case INPUT_E_:
				draw_SelectedString(screen, UI_NAVBOX_TEXT_X, UI_NAVBOX_TEXT_Y + (26), 5, PIXEL_WHITE, "East");
				gamestate->level_previous = gamestate->level;
				gamestate->level = levelstate->east;
				e = 1;
				break;
			case INPUT_W:
			case INPUT_W_:
				draw_SelectedString(screen, UI_NAVBOX_TEXT_X, UI_NAVBOX_TEXT_Y + (34), 5, PIXEL_WHITE, "West");
				gamestate->level_previous = gamestate->level;
				gamestate->level = levelstate->west;
				e = 1;
				break;
			case INPUT_CANCEL:
				e = 1;
				break;
			default:
				break;
		}
	}
	
}

void ui_DrawTalkChoice(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate){
	// Draws a dialogue box into which the available NPC characters we can talk to are placed
	unsigned char row = 2;		// Start character choice labels offset below the window title
	unsigned char i;
	unsigned char e = 0;
	unsigned char c;
	unsigned short remain = 0;
		
	unsigned char input1_y = 0;
	unsigned char input2_y = 0;
	unsigned char input3_y = 0;
	unsigned char flash_y = 0;
	
	ui_DrawPopup(screen, UI_TALKCHOICE_START_X, UI_TALKCHOICE_START_Y, UI_TALKCHOICE_WIDTH, UI_TALKCHOICE_HEIGHT, "Select Character", 1);
	
	if (levelstate->has_npc1){
		sprintf(gamestate->buf, "<r>1<C>. %s", gamestate->enemies->enemy[1]->name);
		input1_y = UI_TALKCHOICE_TEXT_Y + (row * 8);
		draw_String(screen, UI_TALKCHOICE_TEXT_X, input1_y, 28, 1, 0, screen->font_8x8, PIXEL_WHITE, gamestate->buf, MODE_PIXEL_SET);
	} else {
		draw_String(screen, UI_TALKCHOICE_TEXT_X, input1_y, 28, 1, 0, screen->font_8x8, PIXEL_WHITE, "1. ---", MODE_PIXEL_SET);
	}
	if (levelstate->has_npc2){
		sprintf(gamestate->buf, "<r>2<C>- %s", gamestate->enemies->enemy[2]->name);
		input2_y = input1_y + (2 * 8);
		draw_String(screen, UI_TALKCHOICE_TEXT_X, input2_y, 28, 1, 0, screen->font_8x8, PIXEL_WHITE, gamestate->buf, MODE_PIXEL_SET);
	} else {
		draw_String(screen, UI_TALKCHOICE_TEXT_X, input2_y, 28, 1, 0, screen->font_8x8, PIXEL_WHITE, "2. ---", MODE_PIXEL_SET);
	}
	if (levelstate->has_npc3){
		sprintf(gamestate->buf, "<r>3<C>- %s", gamestate->enemies->enemy[3]->name);
		input3_y = input1_y + (3 * 8);
		draw_String(screen, UI_TALKCHOICE_TEXT_X, input3_y, 28, 1, 0, screen->font_8x8, PIXEL_WHITE, gamestate->buf, MODE_PIXEL_SET);
	} else {
		draw_String(screen, UI_TALKCHOICE_TEXT_X, input3_y, 28, 1, 0, screen->font_8x8, PIXEL_WHITE, "3. ---", MODE_PIXEL_SET);
	}
		
	draw_Flip(screen);
	
	levelstate->selected_npc = 0;
	while(!e){
		c = input_Get(screen);
		switch(c){
			case INPUT_1:
				levelstate->selected_npc = 1;;
				flash_y = input1_y;
				break;
			case INPUT_2:
				levelstate->selected_npc = 2;
				flash_y = input2_y;
				break;
			case INPUT_3:
				levelstate->selected_npc = 3;
				flash_y = input3_y;
				break;
			case INPUT_CANCEL:
				e = 1;
				break;
			default:
				if (levelstate->selected_npc){
					// Flash selected option - we backspace 6 characters, as there are 6 control codes in 
					// the string which displays the selected NPC name.
					draw_SelectedString(screen, UI_TALKCHOICE_TEXT_X, flash_y, strlen(gamestate->buf) - 6, PIXEL_WHITE, gamestate->buf);
					remain = ui_NPCDialogue(screen, gamestate, levelstate, 0, 1);
					if (remain > 0){
						// More text than can be shown on one page...
						input_Set(INPUT_N);
						input_Set(INPUT_N_);
					}
					while(!e){
						c = input_Get(screen);
						switch(c){
							case INPUT_N:
							case INPUT_N_:
								// Keep scrolling through dialogue
								remain = ui_NPCDialogue(screen, gamestate, levelstate, remain, 0);
								break;
							case INPUT_CANCEL:
								e = 1;
								break;
							default:
								break;
						}
					}
				}
				break;
		}
	}
	
	screen->dirty = 1;
}

void ui_DrawPopup(Screen_t *screen, unsigned short x, unsigned short y, unsigned short w, unsigned short h, char *title, unsigned char animate){
	// Draws a 'popup' box with a solid title bar and a title string
	// The box is animated by appearing to roll down from the title bar.
	
	unsigned char line = 0;
	unsigned char pc = (h / screen->popup_steps);
	unsigned char new_y = y + 10;
	unsigned char new_h = h - 10;

	// Draw solid title bar and popup title
	draw_Box(screen, x, y, w, 10, 1, PIXEL_RED, PIXEL_RED, MODE_PIXEL_SET);
	draw_String(screen, (x / 8) + 1, y + 2, (w / 8), 1, 0, screen->font_8x8, PIXEL_WHITE, title, MODE_PIXEL_OR);
	
	if (animate){
		// Animate the box getting bigger
		for (line = 0; line < new_h; line++){
			if (line % pc == 0){
				draw_Box(screen, x, new_y, w, line + 1, 1, PIXEL_RED, PIXEL_BLACK, MODE_PIXEL_SET);
				screen_Vsync(screen, 2);
				draw_Flip(screen);
			}
		}
	}
	draw_Box(screen, x, new_y, w, new_h, 1, PIXEL_RED, PIXEL_BLACK, MODE_PIXEL_SET);	
	
}

void ui_DrawLocationName(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate){
	// Draws the location name in the title bar
	
	draw_String(screen, UI_TITLEBAR_MAX_CHARS - (strlen((char *)levelstate->name)), UI_TITLEBAR_TEXT_Y, MAX_LEVEL_NAME_SIZE, 1, 0, screen->font_8x8, PIXEL_WHITE, (char *)levelstate->name, MODE_PIXEL_SET);
	screen->dirty = 1;
}

unsigned short ui_NPCDialogue(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned short remain, unsigned char animate){
	// Draws the dialogue window, places portrait images and shows the text from the selected npc (npc_choice), 
	// which can be either levelstate->npc1 or leveltstate->npc2
	
	unsigned short next_remain = 0;
	unsigned char npc = 0;
	unsigned short npc_id = 0;
	unsigned short npc_text_id = 0;
	unsigned char npc_unique_dialogue_id = 0;
	
	npc = levelstate->selected_npc;
	
	if (npc == 1){
		npc_text_id = levelstate->npc1_text;
		npc_unique_dialogue_id = levelstate->npc1_text_unique_id;
	}
	if (npc == 2){		
		npc_text_id = levelstate->npc2_text;
		npc_unique_dialogue_id = levelstate->npc2_text_unique_id;
	}
	if (npc == 3){		
		npc_text_id = levelstate->npc3_text;
		npc_unique_dialogue_id = levelstate->npc3_text_unique_id;
	} 
	npc_id = gamestate->enemies->enemy[npc]->id;
	
	// We don't need to load data on every page turn of this dialogue box...
	if (remain == 0){
		// Load the dialogue entry for this NPC
		data_LoadStory(screen, gamestate, levelstate, npc_text_id);
		
		// Register this NPC
		data_AddNPC(screen, gamestate, levelstate, npc_id);
		
		// Register this unique dialogue for this NPC
		// ... in case there are any condition checks on
		// this conversation having taken place
		data_IncrementNPCTalk(screen, gamestate, levelstate, npc_id, npc_unique_dialogue_id);
	}
	
	// Animate a popup window to hold the NPC details and dialogue text
	ui_DrawPopup(screen, UI_NPCDIALOGUE_START_X, UI_NPCDIALOGUE_START_Y, UI_NPCDIALOGUE_WIDTH + 4, UI_NPCDIALOGUE_HEIGHT, gamestate->enemies->enemy[npc]->name, animate);
	
	// Display the portrait for this NPC
	draw_Box(screen, UI_NPCDIALOGUE_START_X + 2, UI_NPCDIALOGUE_START_Y + 12, DRAW_PORTRAIT_WIDTH + 1, DRAW_PORTRAIT_HEIGHT + 1, 1, PIXEL_RED, PIXEL_CLEAR, MODE_PIXEL_OR);
	draw_Sprite(screen, UI_NPCDIALOGUE_START_X + 3, UI_NPCDIALOGUE_START_Y + 13, screen->enemies[npc], 1);
	
	next_remain = draw_String(screen, (UI_NPCDIALOGUE_START_X / 8) + 6, UI_NPCDIALOGUE_START_Y + 13, ((UI_NPCDIALOGUE_WIDTH - DRAW_PORTRAIT_WIDTH)/ 8) - 1, 8, remain, screen->font_8x8, UI_MAIN_WINDOW_COLOUR, gamestate->buf, MODE_PIXEL_SET);
	if (next_remain > 0){
		// Right justify the text, minus the 4 displayable characters
		draw_String(screen, ((UI_NPCDIALOGUE_START_X + UI_NPCDIALOGUE_WIDTH) / 8) - 4, UI_NPCDIALOGUE_START_Y + UI_NPCDIALOGUE_HEIGHT - 10, 16, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>N<C>ext", MODE_PIXEL_SET);
	} else {
		// Right justify the text, minus the 6 displayable characters
		draw_String(screen, ((UI_NPCDIALOGUE_START_X + UI_NPCDIALOGUE_WIDTH) / 8) - 6, UI_NPCDIALOGUE_START_Y + UI_NPCDIALOGUE_HEIGHT - 10, 24, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>Esc<C>ape", MODE_PIXEL_SET);
	}
	return next_remain;
}

unsigned short ui_DrawMainWindowText(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate, unsigned short remain, char *c){
	// Draws text into the main window with the correct colours,
	// bounding box sizes and other main-window specific settings.
	//
	// This is ALL it does. If all the text cannot be drawn, it
	// returns the number of characters (of the original string) it
	// has printed, otherwise.
	//
	// A subsequent call with the same string, and a non-zero 'remain'
	// parameter will redraw the text from that offset.
	
	// Clear main text window
	draw_Box(screen, UI_MAIN_WINDOW_X, UI_MAIN_WINDOW_Y, UI_MAIN_WINDOW_WIDTH, UI_MAIN_WINDOW_HEIGHT, 0, PIXEL_CLEAR, PIXEL_BLACK, MODE_PIXEL_SET);
	
	remain = draw_String(screen, UI_MAIN_WINDOW_TEXT_X, UI_MAIN_WINDOW_TEXT_Y, UI_MAIN_WINDOW_MAX_CHARS,	UI_MAIN_WINDOW_MAX_ROWS, remain, screen->font_8x8, UI_MAIN_WINDOW_COLOUR, c, MODE_PIXEL_SET);
	
	// Mark screen as dirty, ready to be repainted
	screen->dirty = 1;
	
	// Return position in buffer
	return remain;
}

void ui_DrawSplashText(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate){
	// Draws the game splash text in the centre of the main UI window
	//
	// Shows:
	// - engine name
	// - engine (git) version
	// - current datafile adventure name (story text ID 0)
	// - current adventure intro (story text ID 1)
	
	// Adventure Name	
	draw_String(screen, UI_MAIN_WINDOW_TEXT_X + 12, UI_MAIN_WINDOW_TEXT_Y + (3 * 8), UI_MAIN_WINDOW_MAX_CHARS, 1, 0, screen->font_8x8, PIXEL_GREEN, (char *) gamestate->name, MODE_PIXEL_SET);
	// Engine Name	
	draw_String(screen, UI_MAIN_WINDOW_TEXT_X + 12, UI_MAIN_WINDOW_TEXT_Y + (5 * 8), UI_MAIN_WINDOW_MAX_CHARS, 1, 0,	screen->font_8x8, PIXEL_GREEN, ENGINE_TARGET_NAME, MODE_PIXEL_SET);
	// Print the introductory text
	draw_String(screen, UI_MAIN_WINDOW_TEXT_X, UI_MAIN_WINDOW_TEXT_Y + (9 * 8), UI_MAIN_WINDOW_MAX_CHARS, 14, 0,	screen->font_8x8, PIXEL_WHITE, (char *) gamestate->text_buffer, MODE_PIXEL_SET);
	// Key to continue message
	draw_String(screen, UI_MAIN_WINDOW_TEXT_X, UI_MAIN_WINDOW_TEXT_Y + (23 * 8), UI_MAIN_WINDOW_MAX_CHARS, 1, 0,	screen->font_8x8, PIXEL_RED, "... Press return to start this adventure!", MODE_PIXEL_SET);
	
	// Mark screen as dirty, ready to be repainted
	screen->dirty = 1;
}

void ui_DrawError(Screen_t *screen, char *title, char *text, short errorcode){
	// Draw 'an error has occurred' box, with a title, body text and an error code
	
	char buf[32];
	
	// Popup, including title
	ui_DrawPopup(screen, UI_ERROR_START_X, UI_ERROR_START_Y, (30 * 8), (7 * 8), (char *)title, 1);
	
	// Main body
	draw_String(screen, UI_ERROR_START_X / 8 + 1, UI_ERROR_START_Y + 12, 28, 3, 0,	screen->font_8x8, PIXEL_RED, (char *)text, MODE_PIXEL_SET);
	
	// Error code in footer
	sprintf(buf, "Error code: <w>[%d]\n", errorcode);
	draw_String(screen, UI_ERROR_START_X / 8 + 1, UI_ERROR_START_Y + 48, 28, 1, 0,	screen->font_8x8, PIXEL_RED, (char *)buf, MODE_PIXEL_SET);
		
	// Flip the screen buffer - all errors are fatal -  don't wait for redraw.
	draw_Flip(screen);
	input_WaitAndReturn(screen);
}

void ui_DebugScreen(Screen_t *screen, GameState_t *gamestate, LevelState_t *levelstate){

	unsigned char c;
	unsigned char e = 0;
	unsigned short i;
	unsigned short locations = 0;
	unsigned short primary = 0;
	unsigned short secondary = 0;
	unsigned char players = 1;
	unsigned char npcs = 0;
	unsigned char *mem;
	unsigned char *mem2;
	unsigned char *mem3;
	unsigned char *mem4;
	unsigned int size_bytes = 0;
	unsigned int total_bytes = 0;
	unsigned int base1 = 2048;
	unsigned int base2 = 1024;
	unsigned int base3 = 256;
	unsigned int base4 = 8;
	
	draw_Clear(screen);
	
	// NPCs encountered
	npcs = data_CountNPC(gamestate->npcs);
	
	// Locations visited
	for (i = 0; i < MAX_LOCATIONS; i++){
		if (gamestate->level_visits[i] > 0){
			locations++;
		}
	}
	
	// Primary spawns defeated
	for (i = 0; i < MAX_LOCATIONS; i++){
		primary += gamestate->level_defeated_primary[i];
	}
	
	// Secondary spawns defeated
	for (i = 0; i < MAX_LOCATIONS; i++){
		secondary += gamestate->level_defeated_secondary[i];
	}
	
	draw_String(screen, UI_TITLEBAR_MAX_CHARS - (strlen("DEBUG SCREEN")), UI_TITLEBAR_TEXT_Y, MAX_LEVEL_NAME_SIZE, 1, 0, screen->font_8x8, PIXEL_RED, "DEBUG SCREEN", MODE_PIXEL_SET);
	
	// Print data structure sizes	
	sprintf((char *)gamestate->text_buffer, "<g>Data Structures<C>\n- <r>%6d<C> BMP buffers\n- <r>%6d<C> Gamestate (inc text buffers)\n- <r>%6d<C> Levelstate\n- <r>%6d<C> per NPC state, (total: <r>%db<C>)\n- <r>%6d<C> Partystate (total: <r>%dx<C>)\n- <r>%6d<C> Enemystate (total: <r>%dx<C>)\n", (sizeof(bmpdata_t) + sizeof(bmpstate_t)), sizeof(GameState_t), sizeof(LevelState_t), sizeof(struct NPCList), (npcs * sizeof(struct NPCList)), (sizeof(PartyState_t) + (MAX_PLAYERS * sizeof(PlayerState_t))), MAX_PLAYERS, (sizeof(EnemyState_t) + (MAX_MONSTER_TYPES * sizeof(PlayerState_t))), MAX_MONSTER_TYPES);
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> per Weapon\n", sizeof(WeaponState_t));
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> per Spell\n", sizeof(SpellState_t));
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> per Item\n", sizeof(ItemState_t));
	draw_String(screen, 1, 15, 48, 11, 0, screen->font_8x8, PIXEL_WHITE, (char *)gamestate->text_buffer, MODE_PIXEL_SET);
	
	sprintf((char *)gamestate->text_buffer, "<g>Graphics Data<C>\n");
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> Double buffering?\n- <r>%6d<C> Screen state\n- <r>%6d<C> Bitmap font\n- <r>%6d<C> PC/Enemy GFX\n- <r>%6d<C> Boss GFX\n- <r>%6d<C> Sprite size\n- <r>%6d<C> Boss size", screen->indirect, (screen->indirect * SCREEN_BYTES) + sizeof(Screen_t), sizeof(fontdata_t), sizeof(ssprite_t) * (MAX_PLAYERS + MAX_MONSTER_TYPES), sizeof(lsprite_t), SPRITE_NORMAL_BYTES, SPRITE_BOSS_BYTES);
	draw_String(screen, 36, 96, 48, 11, 0, screen->font_8x8, PIXEL_WHITE, (char *)gamestate->text_buffer, MODE_PIXEL_SET);
		
	// Calculate largest free blocks of memory that remain
	sprintf((char *)gamestate->text_buffer, "<g>Memory Free<C> wait...\n");
	draw_String(screen, 1, 104, 48, 6, 0, screen->font_8x8, PIXEL_WHITE, (char *)gamestate->text_buffer, MODE_PIXEL_SET);
	sprintf((char *)gamestate->text_buffer, "<g>Memory Free<C>        \n");
	mem = get_FreeBlock(&size_bytes, base1, base1);
	total_bytes += size_bytes;
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> 1st (%db chunks)\n", size_bytes, base1);
	
	mem2 = get_FreeBlock(&size_bytes, base2, base2);
	total_bytes += size_bytes;
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> 2nd (%db)\n", size_bytes, base2);
	
	mem3 = get_FreeBlock(&size_bytes, base3, base3);
	total_bytes += size_bytes;
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> 3rd (%db)\n", size_bytes, base3);
	
	mem4 = get_FreeBlock(&size_bytes, base4, base4);
	total_bytes += size_bytes;
	sprintf((char *)gamestate->text_buffer + strlen((char *)gamestate->text_buffer), "- <r>%6d<C> 4th (%db)\n- <r>%6d<C> Total Bytes Free\n", size_bytes, base4, total_bytes);
	draw_String(screen, 1, 104, 48, 6, 0, screen->font_8x8, PIXEL_WHITE, (char *)gamestate->text_buffer, MODE_PIXEL_SET);
	
	// Free any memory allocated!
	if (mem){
		free(mem);
	}
	if (mem2){
		free(mem2);
	}
	if (mem3){
		free(mem3);
	}
	if (mem4){
		free(mem4);
	}

	// Game progress details
	sprintf((char *)gamestate->text_buffer, "<g>Game Progress\n<C>\n- <r>%6d<C> PC in player party\n- <r>%6d<C> NPCs met\n- <r>%6d<C> Locations discovered\n- <r>%6d<C> Primary spawns\n- <r>%6d<C> Secondary spawns\n", players, npcs, locations, primary, secondary);
	draw_String(screen, 1, 160, 48, 6, 0, screen->font_8x8, PIXEL_WHITE, (char *)gamestate->text_buffer, MODE_PIXEL_SET);
	draw_String(screen, 1, SCREEN_HEIGHT - 10, 32, 1, 0, screen->font_8x8, PIXEL_RED, "Press [ESC] to return to game", MODE_PIXEL_SET);
	
	screen->dirty = 1;
	draw_Flip(screen);
	
	input_Clear();
	input_Set(INPUT_CANCEL);
	while(!e){
		c = input_Get(screen);
		switch(c){
			case INPUT_CANCEL:
				e = 1;
				break;
			default:
				break;
		}
	}
}

void ui_DrawEquipError(Screen_t *screen, char *error){
	// Show a popup explaining the current item cannot be equipped by the current player
	// Pause and wait for any key
	
	ui_DrawPopup(screen, UI_YESNO_START_X, UI_YESNO_START_Y, 200, 50, "Warning!", 1);	
	draw_String(screen, UI_YESNO_START_X / 8 + 1, UI_YESNO_START_Y + 12, 23, 3, 0, screen->font_8x8, PIXEL_WHITE, error, MODE_PIXEL_SET);
	draw_String(screen, UI_YESNO_START_X / 8 + 18, UI_YESNO_START_Y + 42, 23, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>Esc<C>ape", MODE_PIXEL_SET);
	input_WaitAndReturn(screen);
	screen->dirty = 1;
	draw_Flip(screen);
}

char ui_DrawBooleanChoice(Screen_t *screen, char *title, char *choice1, char *choice2){
	// Draw a dialogue box that prompt the player to select:
	// choice1 (returns 0)
	// choice2 (returns 1)
	// escape (returns -1)
	
	unsigned char c;
	unsigned char e = 0;
	unsigned char selected_item = 0;
	
	ui_DrawPopup(screen, UI_YESNO_START_X, UI_YESNO_START_Y, 200, 50, title, 1);
	
	draw_String(screen, UI_YESNO_START_X / 8 + 3, UI_YESNO_START_Y + 14 + 1, 23, 1, 0, screen->font_8x8, PIXEL_WHITE, choice1, MODE_PIXEL_SET);
	draw_Box(screen, UI_YESNO_START_X + 9, UI_YESNO_START_Y + 14, 8, 8, 1, PIXEL_RED, PIXEL_WHITE, MODE_PIXEL_OR);
	
	draw_String(screen, UI_YESNO_START_X / 8 + 3, UI_YESNO_START_Y + 14 + 12 + 1, 23, 1, 0, screen->font_8x8, PIXEL_WHITE, choice2, MODE_PIXEL_SET);
	draw_Box(screen, UI_YESNO_START_X + 9, UI_YESNO_START_Y + 14 + 12, 8, 8, 1, PIXEL_RED, PIXEL_BLACK, MODE_PIXEL_OR);
	
	draw_String(screen, UI_YESNO_START_X / 8 + 18, UI_YESNO_START_Y + 40, 23, 1, 0, screen->font_8x8, PIXEL_WHITE, "<r>Esc<C>ape", MODE_PIXEL_SET);
	
	screen->dirty = 1;
	draw_Flip(screen);
	
	input_Set(INPUT_CONFIRM);
	input_Set(INPUT_CANCEL);
	input_Set(INPUT_UP);
	input_Set(INPUT_DOWN);
	
	while(!e){
		c = input_Get(screen);
		switch(c){
			case INPUT_DOWN:
				// =====================================
				// Scroll to next item
				// =====================================
				
				if (selected_item == 0){
					selected_item = 1;
					draw_Box(screen, UI_YESNO_START_X + 9, UI_YESNO_START_Y + 14, 8, 8, 1, PIXEL_RED, PIXEL_BLACK, MODE_PIXEL_SET);
					draw_Box(screen, UI_YESNO_START_X + 9, UI_YESNO_START_Y + 14 + 12, 8, 8, 1, PIXEL_RED, PIXEL_WHITE, MODE_PIXEL_OR);
					draw_Flip(screen);
				}
				// Highlight selection option button
				break;
				
			case INPUT_UP:
				// =====================================
				// Scroll to previous item
				// =====================================
				
				if (selected_item == 1){
					selected_item = 0;	
					draw_Box(screen, UI_YESNO_START_X + 9, UI_YESNO_START_Y + 14, 8, 8, 1, PIXEL_RED, PIXEL_WHITE, MODE_PIXEL_OR);
					draw_Box(screen, UI_YESNO_START_X + 9, UI_YESNO_START_Y + 14 + 12, 8, 8, 1, PIXEL_RED, PIXEL_BLACK, MODE_PIXEL_SET);
					draw_Flip(screen);
				}
				
				// Highlight selection option button
				break;
			
			case INPUT_CONFIRM:
				// =====================================
				// Select the highlighted option
				// =====================================

				// Flash selected option
				if (selected_item == 0){
					draw_SelectedString(screen, UI_YESNO_START_X / 8 + 3, UI_YESNO_START_Y + 14 + 1, strlen(choice1), MODE_PIXEL_SET, choice1);
					draw_String(screen, UI_YESNO_START_X / 8 + 3, UI_YESNO_START_Y + 14 + 1, 23, 1, 0, screen->font_8x8, PIXEL_WHITE, choice1, MODE_PIXEL_SET);
				} else {
					draw_SelectedString(screen, UI_YESNO_START_X / 8 + 3, UI_YESNO_START_Y + 14 + 12 + 1, strlen(choice2), MODE_PIXEL_SET, choice2);
					draw_String(screen, UI_YESNO_START_X / 8 + 3, UI_YESNO_START_Y + 14 + 12 + 1, 23, 1, 0, screen->font_8x8, PIXEL_WHITE, choice2, MODE_PIXEL_SET);
				}
				draw_Flip(screen);
				
				return selected_item;
				
			case INPUT_CANCEL:
				e = 1;
				break;
			default:
				break;
		}
	}
	return -1;
}
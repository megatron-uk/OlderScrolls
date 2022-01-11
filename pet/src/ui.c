// ====================================
//
// UI-related 'drawing' functions
// for the PET
//
// ====================================

#include <stdio.h>
#include <string.h>

// Local headers
#ifndef HAVE_GAME_H
#include "game.h"
#define HAVE_GAME_H
#endif
#ifndef HAS_SCREEN_H
#include "screen.h"
#define HAS_SCREEN_H
#endif
#ifndef HAS_STRING_H
#include "strings.h"
#define HAS_STRING_H
#endif
#include "ui.h"
#include "utils.h"

void ui_draw(void){
	// Draw the basic UI screen without any content	
	
	unsigned char i;
	unsigned short offset = screen.x;
	
	// ==================================
	// Main outer screen border
	//
	// UL, UR, LL, LR
	POKE(SCREEN_BASE, UI_CORNER_UL);
	POKE(SCREEN_BASE + screen.top_right, UI_CORNER_UR);
	POKE(SCREEN_BASE + screen.bottom_left, UI_CORNER_LL);
	POKE(SCREEN_BASE + screen.bottom_right, UI_CORNER_LR);
	// Left and Right vertical border
	for (i = 1; i < (screen.y - 1); i++){		
		POKE(SCREEN_BASE + offset, UI_EDGE_LEFT);
		POKE(SCREEN_BASE + offset + (screen.x - 1), UI_EDGE_RIGHT);
		offset += screen.x;
	}
	// Upper edge, bottom horizontal border
	memset((unsigned char*) SCREEN_BASE + screen.top_left + 1, UI_BAR, screen.x - 2);
	memset((unsigned char*) SCREEN_BASE + screen.bottom_left + 1, UI_BAR, screen.x - 2);
	
	// ==================================
	// Status bar on the bottom which holds
	// hotkeys, status updates, options etc
	//
	// T marks at left and right hand screen edges
	POKE(SCREEN_BASE + screen.statusbar, UI_LEFT_T);
	POKE(SCREEN_BASE + screen.statusbar + screen.x - 1, UI_RIGHT_T);
	// Horizontal divider
	memset((unsigned char*) SCREEN_BASE + screen.statusbar + 1, UI_BAR, screen.x - 2);
	
	// ==================================
	// Sidebar on the right which holds
	// player character details
	//
	// T mark at the top and bottom of left hand bar
	POKE(SCREEN_BASE + screen.sidebar, UI_LOWER_T);
	POKE(SCREEN_BASE + screen.statusbar + screen.sidebar, UI_UPPER_T);
	// Left hand vertical bar of the sidebar
	offset = screen.x;
	for (i = 0; i < (screen.y - UI_STATUSBAR_HEIGHT - 1); i++){		
		POKE(SCREEN_BASE + offset + screen.sidebar, UI_EDGE_LEFT);
		offset += screen.x;
	}
}

void ui_draw_text(unsigned char *text, unsigned char empty){
	// Draw story text or 'graphics' in the central section of the UI
	
	unsigned char row;
	unsigned char ptr;
	unsigned short size = 0;
	unsigned short offset = screen.x + 1;
	unsigned char col_max = screen.x - UI_SIDEBAR_START - 1;
	unsigned char row_max = screen.y - UI_STATUSBAR_HEIGHT;
	
	// We can fit text into the following columns
	// Col 1 ---> col_max
	// For the following rows:
	// Row 1 ---> row_max
	
	// Determine length of the 'petscii string'
	if (empty != 1){
		size = string_len(text);
	}
	
	ptr = 0;
	// Until we reach the maximum number of printable rows
	for (row = 1; row < row_max; row++){
		// Are we just emptying the screen?
		if (empty){
			// Yes, just blank it
			memset((unsigned char*) SCREEN_BASE + offset, 0x60, col_max);
			offset += screen.x;
		} else {
			// No, print some text
			// If the string currently pointed can
			// fit in one row...
			if ((size - ptr) < col_max){
				// ... copy it in and exit
				memcpy((unsigned char*) SCREEN_BASE + offset, text + ptr, size - ptr);
				break;
			} else {
				// ... copy what can be fitted, increment
				// string pointer and offset, and then
				// go around again.
				memcpy((unsigned char*) SCREEN_BASE + offset, text + ptr, col_max);
				ptr += col_max;
				offset += screen.x;
			}
		}
	}
}

void ui_draw_statusbar_text(unsigned char *text, unsigned char empty){
	// Draw status text am available options in the bottom section of the UI
	
	unsigned char row;
	unsigned char ptr;
	unsigned short size = 0;
	unsigned short offset = screen.statusbar + 1;
	unsigned char col_max = screen.x - 1;
	unsigned char row_max = screen.y - UI_STATUSBAR_HEIGHT;
	
	// We can fit text into the following columns
	// Col 1 ---> col_max
	// For the following rows:
	// Row 1 ---> row_max
	
	// Determine length of the 'petscii string'
	if (empty != 1){
		size = string_len(text);
	}
	
	ptr = 0;
	// Until we reach the maximum number of printable rows
	for (row = 1; row < row_max; row++){
		// Are we just emptying the screen?
		if (empty){
			// Yes, just blank it
			memset((unsigned char*) SCREEN_BASE + offset, 0x60, col_max);
			offset += screen.x;
		} else {
			// No, print some text
			// If the string currently pointed can
			// fit in one row...
			if ((size - ptr) < col_max){
				// ... copy it in and exit
				memcpy((unsigned char*) SCREEN_BASE + offset, text + ptr, size - ptr);
				break;
			} else {
				// ... copy what can be fitted, increment
				// string pointer and offset, and then
				// go around again.
				memcpy((unsigned char*) SCREEN_BASE + offset, text + ptr, col_max);
				ptr += col_max;
				offset += screen.x;
			}
		}
	}
}

void ui_draw_sidebar_text(){
	// Draw the sidebar (character names, stats) in the side section of the UI
}


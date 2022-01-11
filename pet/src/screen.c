// ====================================
//
// Screen-related 'drawing' functions
// for the PET
//
// ====================================

#include <stdlib.h>
#include <string.h>
#include <conio.h>

// Local headers
#include "error.h"
#ifndef HAS_SCREEN_H
#include "screen.h"
#define HAS_SCREEN_H
#endif
#include "ui.h"

unsigned char screen_init(void){
	// Initialise a screen data structure based on the 
	// size of the hardware text mode screen.
	
	unsigned char x = 0;
	unsigned char y = 0;
	
	screensize(&x, &y);
	screen.x = x;
	screen.y = y;
	if (screen.x != 40){
		return ERROR_SCREEN;
	}
	
	// Set UI coordinates
	screen.top_left = 0;
	screen.top_right = (screen.x - 1);
	screen.bottom_left = ((screen.y -1 ) * screen.x);
	screen.bottom_right = ((screen.y * screen.x) - 1);
	screen.statusbar = ((screen.y - UI_STATUSBAR_HEIGHT) * screen.x);
	screen.sidebar = screen.x - UI_SIDEBAR_START;
	
	screen.error_x = 1;
	screen.error_y = screen.y - (UI_STATUSBAR_HEIGHT - 1);
	
	// Calculate size
	screen.bytes = screen.x * screen.y;
	
	// Allocate buffer
	if (screen.bytes == 0){
		return ERROR_SCREEN;
	} else {
		// Clear physical screen and return OK status
		SCREEN_GRAPHIC_MODE;
		return ERROR_NONE;
	}
}
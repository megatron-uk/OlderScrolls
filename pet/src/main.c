#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <conio.h>

// Local headers
#include "error.h"
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

int main(){

	// Initial file IO device
	unsigned char dev;
	
	// Datafile character buffer
	//unsigned char buf[32];
	
	// Weapon data structure
	WeaponState w;
	
	// Screen geometry
	
	
	w.item_id = 255;
	
	screen.x = 0;
	screen.y = 0;
	screen.bytes = 0;
	
	dev = current_device();
	
	if (screen_init() != ERROR_NONE){
		printf(STRING_SIMPLE_SCREEN);
		printf(STRING_SIMPLE_40COL);
		return(ERROR_SCREEN);
	};
	ui_draw();
	ui_draw_text(NULL, 1);
	//cprintf("\r\nbefore screen: .%d.", dev);
	if (open_file(2, "weapons") == ERROR_NONE){
		cputsxy(1, 1, STRING_SIMPLE_OK);
		read_weapon(&w, 1);
	} else {
		// Display error message and exit
		//ui_draw_statusbar_text(&screen, NULL, 1);
		cputsxy(screen.error_x, screen.error_y - 1, STRING_DATAFILE_ERROR);
		cputsxy(screen.error_x, screen.error_y, STRING_ANY_KEY);
		while (kbhit() == 0){
		}
		SCREEN_CLEAR;
		return(ERROR_DATAFILE);
	}
	cputsxy(1, (screen.y -2), STRING_SIMPLE_OK);
	while (kbhit() == 0){
	}
	SCREEN_CLEAR;
	return(ERROR_NONE);
}

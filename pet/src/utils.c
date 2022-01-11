// Utility commands used through the game

#include <cbm.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

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
#include "weapons.h"
#include "utils.h"

unsigned char open_file(unsigned char filehandle, char *filename){
	// Opens a datafile on the current disk device, ready for it to be read
	unsigned char dev = 0;
	unsigned char sa = 0;
	
	// Set device operation, device number and drive number
	cbm_k_setlfs(filehandle, 8, 0);
	if (cbm_k_readst()){
		cputsxy(screen.error_x, screen.error_y, STRING_DEVICE_NOT_FOUND);
		return ERROR_DATAFILE;
	}
	
	// Set name of the file
	cbm_k_setnam(filename);
	if (cbm_k_readst()){
		cputsxy(screen.error_x, screen.error_y, STRING_FILENAME_ERROR);
		return ERROR_DATAFILE;
	}
	
	// Open file
	cbm_k_open();
	if (cbm_k_readst()){
		cputsxy(screen.error_x, screen.error_y, STRING_FILE_NOT_FOUND);
		return ERROR_DATAFILE;
	}
	
	// Assign a file handle number
	cbm_k_chkin(filehandle);
	if (cbm_k_readst()){
		cputsxy(screen.error_x, screen.error_y, STRING_FILEHANDLE_ERROR);
		return ERROR_DATAFILE;	
	}
	// No errors, file opened
	return ERROR_NONE;
}

void close_file(unsigned char filehandle){
	// Close the current open file
	
	cbm_k_close(filehandle);
}

unsigned char read_weapon(WeaponState *wep, unsigned char item_id){
	// Load a weapon from the weapons datafile, indicated by a given weapon item id
	
	unsigned char buf[50];
	unsigned char i;
	unsigned char status = 0;
	
	for(i = 0; i < 3; i++){
		buf[i] = cbm_k_getin();
		status = cbm_k_readst();
		if (status){
			// An error was generated while reading from the file
			cputsxy(screen.error_x, screen.error_y, STRING_DATAFILE_READ_ERROR);
			return ERROR_DATAFILE;
		}
	}
	buf[i+1] = '\0';

	//cprintf("\r\n buf: .%s.\r\n", buf);
	wep->item_id = atoi(buf);
	//cprintf(" atoi: .%d.\r\n", wep->item_id);
	
	if (wep->item_id != item_id){
		// Data read from file does not match intended item id
		cputsxy(screen.error_x, screen.error_y, STRING_DATAFILE_ITEM_ERROR);
		return ERROR_DATAFILE;
	}
	cputsxy(screen.error_x, screen.error_y, STRING_DATAFILE_OK);
	return ERROR_NONE;
}

unsigned char current_device(void){
	return(PEEK(DEVICE_ADDRESS));	
}

unsigned char current_sub_device(void){
	return(PEEK(SUB_DEVICE_ADDRESS));	
}
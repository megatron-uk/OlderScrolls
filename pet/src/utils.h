#ifndef HAVE_GAME_H
#include "game.h"
#define HAVE_GAME_H
#endif

#define DEVICE_ADDRESS	212		// From PET memory map - last device opened
#define SUB_DEVICE_ADDRESS 211		// From PET memory map - last sub-device opened

// Poke a character/value into screen memory
#define POKE(address, value) \
	(*(unsigned char*) (address) = (value))

#define PEEK(addr) \
	(*(unsigned char*) (addr))

	
unsigned char open_file(unsigned char filehandle, char *filename);

void close_file(unsigned char filehandle);

unsigned char read_weapon(WeaponState *wep, unsigned char item_id);

unsigned char current_device(void);

unsigned char current_sub_device(void);
// ====================================
//
// Common static petscii 'strings' used
// within the game. Longer text is stored
// in the on-disk strings.dat file, but
// these are things like on screen labels
// actions, etc.
//
// ====================================

#include <string.h>

// Local files
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

// Global variable that all functions can access about
// screen geometry, window sizes, etc
struct Screen screen;

//void string_put(unsigned char x, unsigned char y, char *text){
	// Print a 0xFF terminated petscii string at a defined on-screen position
	
//	unsigned short size = 0;
//	unsigned short offset = 0;
	
//	size = string_len(text);
//	offset = (screen.x * y) + x;
//	memcpy((unsigned char*) SCREEN_BASE + offset, text, size);
//}

unsigned short string_len(char *text){
	// Return the length of a 0xFF terminated 'petscii string' array
	
	unsigned short i;
	
	for (i = 0; i< MAX_STORY_TEXT_SIZE; i++){
		if (text[i] == 0xFF){
			return i;	
		}
		if (text[i] == '$'){
			return i;	
		}
	}
	return 0;
}
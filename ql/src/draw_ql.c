/* draw_ql.c, Low level pixel drawing routing for the Sinclair QL.
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <qdos.h>

#include "poll.h"
#ifndef _BMP_H
#include "bmp_ql.h"
#endif
#ifndef _CONFIG_H
#include "../common/config.h"
#endif
#ifndef _DRAW_H
#include "../common/draw.h"
#endif
#ifndef _UTILS_H
#include "../common/utils.h"
#endif
#ifndef _BMP_H
#include "bmp_ql.h"
#endif
#ifndef _UI_H
#include "../common/ui.h"
#endif
#ifndef _ERROR_H
#include "../common/error.h"
#endif

int screen_Init(Screen_t *screen){
	// Initialise screen and/or offscreen buffers
	unsigned char i;
	int f;
	
	// ========================================
	//  Initialise offscreen video buffer
	// ========================================
	
	// Where our physical video memory is located
	screen->screen = SCREEN_BASE;
	
	// Offscreen rendering (requires a free region of 32768 bytes)
	// This will composite all UI elements and text, only
	// flipping the buffer to screen once all elements are
	// in place.
	screen->x = SCREEN_WIDTH;
	screen->y = SCREEN_HEIGHT;
	screen->indirect = 1;
	screen->offscreen = NULL;//calloc(SCREEN_BYTES, 1);
	if (screen->offscreen == NULL){
		
		// Couldn't allocate memory for offscreen buffer
		// - we might be on a base model QL with only 128kb 
		// and some background utils running.
		// Direct rendering - write direct to screen memory
		// but this might show flickering as we write the
		// individual UI elements and/or text
		screen->indirect = 0;
		screen->offscreen = NULL;
		screen->buf = (unsigned short*) screen->screen;
	} else {
		//printf("- Offscreen buffer: %d bytes\n", SCREEN_BYTES);
		screen->buf = (unsigned short*) screen->offscreen;
	}
	
	// ==========================================
	// Number of frames of animation when opening new windows
	// ==========================================
	screen->popup_steps = 1;
	
	// ==========================================
	// Initialise vblank timer
	// ==========================================
	poll_init(&screen->vblank_timer);
	
	// ==========================================
	// Open the QDOS input/output channel
	// ==========================================
	
	screen->win = io_open(SCREEN_MODE, 0);
	
	// ==========================================
	// Load font bitmap and initialise fonts
	// ==========================================
	
	// Initialise any fonts
	screen->bmp = (bmpdata_t *) calloc(sizeof(bmpdata_t), 1);
	if (screen->bmp == NULL){
		// Couldn't allocate memory
		return SCREEN_INIT_BMPMEMORY;	
	}
	screen->bmp->pixels = NULL;
	
	// Load and process the 8x8 font
	f = open(FONT_8X8, O_RDONLY);
	if (f < 0){
		// Couldn't open font
		return SCREEN_INIT_FONTFILE;
	}
	screen->font_8x8 = (fontdata_t *) calloc(sizeof(fontdata_t), 1);
	if (screen->font_8x8 == NULL){
		// Couldn't allocate memory
		return SCREEN_INIT_FONTMEMORY;	
	}
	if (bmp_ReadFont(f, screen->bmp, screen->font_8x8, 1, 1, 8, 8) < 0){
		// Couldn't process font bitmap to table
		return SCREEN_INIT_FONTPROCESS;
	}
	screen->font_8x8->unknown_symbol = 37;	// Unknown characters are replaced with the symbol '%' (percent)
	screen->font_8x8->ascii_start = 32;		// First ascii char in our 8x8 font set is the symbol ' ' (space)
	
	// Destroy the pixel memory which was used to load the font bitmap
	free(screen->bmp->pixels);
	
	// At this point we have the bitmap font available, so can use graphical error messages
	
	// ===========================================
	// Set up the progressive bitmap loader
	// We can use this to stream in bitmaps from
	// disk as many times as we want.
	// ===========================================
	
	// Allocate memory for the progressive bmp loader
	screen->bmpstate = (bmpstate_t *) calloc(sizeof(bmpstate_t), 1);
	if (screen->bmpstate == NULL){
		// Couldn't allocate memory
		ui_DrawError(screen, GENERIC_MEMORY_MSG, SCREEN_INIT_MEMORY_MSG, SCREEN_INIT_BMPSTATEMEMORY);
		return SCREEN_INIT_BMPSTATEMEMORY;	
	}
	
	// ===========================================
	// Initialise player/enemy character sprites
	// Only done ONCE per session
	// ===========================================
	
	for (i = 0; i < MAX_PLAYERS; i++){
		screen->players[i] = (ssprite_t *) calloc(sizeof(ssprite_t), 1);
		if (screen->players[i] == NULL){
			// COuld not allocate memory for a player sprite
			ui_DrawError(screen, GENERIC_MEMORY_MSG, SCREEN_INIT_MEMORY_MSG, SCREEN_INIT_PC_SPRITEMEMORY);
			return SCREEN_INIT_PC_SPRITEMEMORY;	
		}
	}
	
	for (i = 0; i < MAX_MONSTER_TYPES; i++){
		screen->enemies[i] = (ssprite_t *) calloc(sizeof(ssprite_t), 1);
		if (screen->enemies[i] == NULL){
			// COuld not allocate memory for a enemy sprite
			ui_DrawError(screen, GENERIC_MEMORY_MSG, SCREEN_INIT_MEMORY_MSG, SCREEN_INIT_E_SPRITEMEMORY);
			return SCREEN_INIT_E_SPRITEMEMORY;	
		}
	}
	
	for (i = 0; i < MAX_BOSS_TYPES; i++){
		screen->boss[i] = (lsprite_t *) calloc(sizeof(lsprite_t), 1);
		if (screen->boss[i] == NULL){
			// COuld not allocate memory for boss sprite
			ui_DrawError(screen, GENERIC_MEMORY_MSG, SCREEN_INIT_MEMORY_MSG, SCREEN_INIT_BOSS_SPRITEMEMORY);
			return SCREEN_INIT_BOSS_SPRITEMEMORY;
		}
	}
	
	// Close file handle used to load font bitmap file
	close(f);
	
	return SCREEN_INIT_OK;
}

void screen_Exit(Screen_t *screen){
	// Free any allocated screen
	if (screen->indirect){
		free(screen->offscreen);
		screen->indirect = 0;
	}
}

void screen_Vsync(Screen_t *screen, unsigned char wait){
	// Return after 'wait' amount of vblank interrupts
	
	screen->vblank_timer = 0;
	while(screen->vblank_timer < wait){	
	}
}

void draw_Clear(Screen_t *screen){
	// Clear screen (or offscreen buffer)
	
	unsigned short i;	// Loop counter
	unsigned short *p;	// Pointer to the current 8 pixel block of screen memory (stored as a pair of bytes / single 16bit word)
	
	// Initialise the pointer to the start of screen memory buffer
	p = (unsigned short*) screen->buf;
	
	// For every 8 pixel block / 16bit word, set those pixels to a given colour
	for (i = 0 ; i < SCREEN_BLOCKS; i++){
		*p = PIXEL_BLACK;
		p++;
	}
}

void draw_Flip(Screen_t *screen){
	// Swap offscreen buffer with video memory,
	// if currently enabled.
	
	if (screen->dirty){
		
		// Copy offscreen buffer
		if (screen->indirect){
			memcpy((unsigned char*) screen->screen, screen->buf, SCREEN_BYTES);
		}
		
		screen->dirty = 0;
	}
}

void draw_SetMask(unsigned short fill, unsigned char vertical, unsigned char *drawing_mask_lo, unsigned char *drawing_mask_hi, unsigned char *pixel_skip, unsigned char *multi_colour){
	// Set drawing mask based on the colour specified
	// Returns 0 if the colour is normal
	// Returns 1 if the colour requires alternating pixels to
	// be drawn with the lo and hi masks - e.g. yellow, which
	// is one green pixel, then one red pixel.
	
	*multi_colour = 0;
	*pixel_skip = 0;
	
	switch(fill){
		case PIXEL_BLACK:
			*drawing_mask_lo = 0x00;
			*drawing_mask_hi = 0x00;
			return;
		case PIXEL_WHITE:
			*drawing_mask_lo = 0xFF;
			*drawing_mask_hi = 0xFF;
			return;
		case PIXEL_WHITE_STIPPLED:
			if (vertical){
				*drawing_mask_lo = 0xFF;
				*drawing_mask_hi = 0xFF;
			} else {
				*drawing_mask_lo = 0xAA;
				*drawing_mask_hi = 0xAA;
			}
			*pixel_skip = 1;
			return;	
		case PIXEL_GREEN:
			*drawing_mask_lo = 0xFF;
			*drawing_mask_hi = 0x00;
			return;
		case PIXEL_GREEN_STIPPLED:
			if (vertical){
				*drawing_mask_lo = 0xFF;
				*drawing_mask_hi = 0x00;
			} else {
				*drawing_mask_lo = 0xAA;
				*drawing_mask_hi = 0x00;
			}
			*pixel_skip = 1;
			return;
		case PIXEL_RED:
			*drawing_mask_lo = 0x00;
			*drawing_mask_hi = 0xFF;
			return;
		case PIXEL_RED_STIPPLED:
			if (vertical){
				*drawing_mask_lo = 0x00;
				*drawing_mask_hi = 0xFF;
			} else {
				*drawing_mask_lo = 0x00;
				*drawing_mask_hi = 0xAA;
			}
			*pixel_skip = 1;
			return;
		case PIXEL_YELLOW:
			if (vertical){
				*drawing_mask_lo = 0xFF;
				*drawing_mask_hi = 0xFF;
			} else {
				*drawing_mask_lo = 0xAA;
				*drawing_mask_hi = 0x55;
			}
			*multi_colour = 1;
			return;
		default:
			*drawing_mask_lo = 0xFF;
			*drawing_mask_hi = 0xFF;
			return;
	}
}

unsigned short draw_IsStippled(unsigned short fill){

	switch(fill){
		case PIXEL_WHITE_STIPPLED:
			return 1;
		case PIXEL_RED_STIPPLED:
			return 1;
		case PIXEL_GREEN_STIPPLED:
			return 1;
		case PIXEL_YELLOW:
			return 1;
	}
	return 0;
}

void draw_GetXY(unsigned short x, unsigned short y, unsigned short *addr, unsigned char *bits){
	// Return offset into screen memory for a given x,y coordinate
	// Write the offset into &addr, with the number of pixels to skip
	// at that address into &bits (i.e. if not on an 8 pixel boundary).
	
	unsigned char r;
		
	// Find the remainder, or the offset number of bits
	r = x % 8;
	if (r == 0){
		*bits = 0;
	} else {
		*bits = r;	
	}
	
	// calculate offset as 'x' position plus 'y' rows * 16bit words per row. 
	*addr = (x / 8) + (y * SCREEN_WORDS_PER_ROW);
	return;
}

void draw_HLine(Screen_t *screen, unsigned short x, unsigned short y, unsigned short length, unsigned short fill, unsigned char pad, unsigned char mode){
	// Draw a horizontal line of pixels
	
	unsigned short *p;
	unsigned short c;
	unsigned short start_p;  // Start position in screen buffer
	unsigned short end_p;    // End position in screen buffer
	unsigned char start_bits;
	unsigned char end_bits;
	unsigned char drawing_mask_lo = 0x00;
	unsigned char drawing_mask_hi = 0x00;
	unsigned short start_mask = 0x0000;
	unsigned short drawing_mask = 0x0000;
	unsigned short end_mask = 0x0000;
	unsigned char pixel_skip = 0;
	unsigned char multi_colour = 0;
	
	draw_GetXY(x + pad, y, &start_p, &start_bits);
	draw_GetXY(x + length - pad, y, &end_p, &end_bits);
	draw_SetMask(fill, 0, &drawing_mask_lo, &drawing_mask_hi, &pixel_skip, &multi_colour);
	
	p = (unsigned short*) screen->buf;
	p += start_p;
	
	// Draw leading pixels if start_bits != 0
	if (start_bits != 0){
		start_mask = 0x0000;
		if (drawing_mask_lo) start_mask = (drawing_mask_lo >> start_bits) << 8;
		if (drawing_mask_hi) start_mask += (unsigned char) drawing_mask_hi >> start_bits;
		if (mode == MODE_PIXEL_OR){
			*p = *p | start_mask;
		} else {
			*p = start_mask;
		}
		p++;
		c = start_p + 1;
		if (start_bits % 2 != 0 ){
			pixel_skip = 1;
		}
	} else {
		c = start_p;	
	}

	if (pad){
		pixel_skip = 1;	
	}
	
	// Draw main section of line in 8 pixel sections
	drawing_mask = 0x0000;
	if (drawing_mask_lo){
		if (pad){
			drawing_mask = (drawing_mask_lo >> 1) << 8;
		} else {
			drawing_mask = drawing_mask_lo << 8;
		}
	}
	if (drawing_mask_hi){
		if (pad){
			drawing_mask += (drawing_mask_hi >> 1);
		} else {
			drawing_mask += drawing_mask_hi;
		}
	}	
	while (c < end_p){
		//if (mode == MODE_PIXEL_OR){
		//	*p = *p | drawing_mask;
		//} else {
			*p = drawing_mask;
		//}
		p++;
		c++;
	}
	
	// Draw trailing pixels if end_bits != 0
	if (end_bits != 0){
		end_mask = 0x0000;
		if (drawing_mask_lo) end_mask = (drawing_mask_lo << (8 - end_bits)) << 8;
		if (drawing_mask_hi) end_mask += (unsigned char) (drawing_mask_hi << (8 - end_bits));
		if (mode == MODE_PIXEL_OR){
			*p = *p | end_mask;
		} else {
			*p = end_mask;
		}
	}
	
	screen->dirty = 1;
}

void draw_VLine(Screen_t *screen, unsigned short x, unsigned short y, unsigned short length, unsigned short fill, unsigned char mode){
	// Draw a vertical line of pixels
	
	unsigned short *p;						// Pointer to a 16bit word in screen buffer with the first and second byte of the 8x1 pixels
	unsigned short c;						// Loop counter
	unsigned short start_p;  				// Start position in screen buffer
	unsigned char start_bits;				// How many pixels into a 8x1 field to skip
	unsigned char skip = 0;					// skips pixels when in stippled colour mode
	unsigned char drawing_mask_lo = 0x00;	// bitmask representing first byte of 8x1 pixels
	unsigned char drawing_mask_hi = 0x00;	// bitmask representing second byte of 8x1 pixels
	unsigned short drawing_mask = 0x0000;	// The combination of the lo and hi masks
	unsigned char pixel_skip = 0;			// Flag indicating the colour used has black pixels inbetween
	unsigned char multi_colour = 0;			// Flag indication the colour used has alternating coloured pixels (red and green)
	unsigned short mask = 0x00;				// Masks off the first and last pixels when not on a 8x1 boundary
	unsigned char next_mask = 0;			// Flip/flops between first and second byte colours in multi colour mode
	
	draw_GetXY(x, y, &start_p, &start_bits);
	draw_SetMask(fill, 1, &drawing_mask_lo, &drawing_mask_hi, &pixel_skip, &multi_colour);
	
	// Are we on a byte boundary?
	if (start_bits != 0){
		// This vertical line is not on a byte boundary, so we
		// need to mask off the first 'start_bits' worth of
		// pixels, leave one, then mask off the remaining
		//if (start_bits > 1){
			mask = (0x01 << (7 - start_bits));
		//} else {
			//mask = (0x01 << 7);
		//}
		if (drawing_mask_lo){
			drawing_mask_lo = drawing_mask_lo & mask;
			drawing_mask = drawing_mask_lo << 8;
		}
		if (drawing_mask_hi){
			drawing_mask_hi = drawing_mask_hi & mask;
			drawing_mask += (unsigned char) drawing_mask_hi;
		}		
	} else {
		// This vertical line is on pixel 0 of a byte boundary, so mask all
		// but the first pixel of each byte
		if (drawing_mask_lo){
			drawing_mask_lo = drawing_mask_lo & 0x80;
			drawing_mask = drawing_mask_lo << 8;
		}
		if (drawing_mask_hi){
			drawing_mask_hi = drawing_mask_hi & 0x80;
			drawing_mask += drawing_mask_hi;
		}
	}
	
	c = 0;
	p = (unsigned short*) screen->buf;
	p += start_p;
	if (pixel_skip){
		skip = 0;
	} else {
		skip = 1;
	}
	while (c < length){
		// Handle stippled lines
		if (pixel_skip){
			skip = !skip;	
		}
		// Only draw if we are not on a black pixel
		if (skip){
			if (multi_colour){
				if (next_mask == 0){
					// Draw the first colour and mask the second byte
					if (mode == MODE_PIXEL_OR){
						*p = *p | (drawing_mask_lo << 8);
					} else {
						*p = (drawing_mask_lo << 8);
					}
					next_mask = 1;
				} else {
					// Draw the second colour and mask the first byte
					if (mode == MODE_PIXEL_OR){
						*p = *p | ((0x0000) + drawing_mask_hi);
					} else {
						*p = ((0x0000) + drawing_mask_hi);
					}
					next_mask = 0;
				}
			} else {
				// Draw a single colour
				if (mode == MODE_PIXEL_OR){
					*p = *p | drawing_mask;
				} else {
					*p = drawing_mask;
				}
			}
		}
		// Skip to address of next row
		p += SCREEN_WORDS_PER_ROW;
		c++;
	}
	
	screen->dirty = 1;
}

void draw_Box(Screen_t *screen, unsigned short x, unsigned short y, 
	unsigned short length, unsigned short height,
	unsigned short borderpx, unsigned short borderfill,
	unsigned short centrefill,
	unsigned char mode){
	// Draws a filled or unfilled box of a given size.
	// Border and fill may be different colours.
	// Stippled fill is offset to give a nice cross-hatching effect.
	// Stippled borders are also offset if > 1px in thickness.

	unsigned short i;		// Loop counter
	
	unsigned short start_p;  // Start position in screen buffer
	unsigned short end_p;    // End position in screen buffer
	unsigned char start_bits;// Number of bits to skip
	unsigned char end_bits;	  // Number of bits to skip
	unsigned char pad = 0;
	unsigned char enable_pad = 0;
	
	draw_GetXY(x, y, &start_p, &start_bits);
	draw_GetXY(x + length, y, &end_p, &end_bits);
	
	// Fill
	if (centrefill != PIXEL_CLEAR){
		// Fill is:
		// Horizontal: (x + borderpx) to ((x + length) - borderpx)
		// Vertical: height - (2 * borderpx)
		if (draw_IsStippled(centrefill)){
			enable_pad = 1;
		} else {
			enable_pad = 0;
		}
		pad = 0;
		for (i = 0; i < height - (2 * borderpx) + 1; i++){
			draw_HLine(screen, x + borderpx, y + borderpx + i, length - (2 * borderpx), centrefill, pad, mode);
			if (enable_pad){
				if (pad == 1){
					pad = 0;
				} else {
					pad = 1;
				}
			}
		}
	}
	
	// Borders
	if (borderfill != PIXEL_CLEAR){
		// Detect if we need to offset rows for cross-hatching effect
		if (borderpx > 1){
			if (draw_IsStippled(borderfill)){
				enable_pad = 1;
			} else {
				enable_pad = 0;
			}
		}
		pad = 0;
		for (i = 0; i < borderpx; i++){
			// Top line
			draw_HLine(screen, x, y + i, length, borderfill, pad, mode);
			// Left
			draw_VLine(screen, x + i, y + borderpx - pad, height - (borderpx * 2) + 1 + pad, borderfill, MODE_PIXEL_OR);
			// Right
			draw_VLine(screen, x + (length - i) - 1, y + borderpx - pad, height - (borderpx * 2) + 1 + pad, borderfill, MODE_PIXEL_OR);
			// Bottom
			draw_HLine(screen, x, y + height - i, length, borderfill, pad, mode);
			if (enable_pad){
				if (pad == 1){
					pad = 0;
				} else {
					pad = 1;
				}
			}
		}
	}
	screen->dirty = 1;
	return;
}

void draw_GetStringXY(unsigned short col, unsigned short y, unsigned short *addr){
	
	// Although there are 512x256 pixels in in the QL, 
	// the odd indirect pixel addressing means that it becomes
	// rather difficult to place text characters at non divisible-by-8
	// locations
	
	// draw_GetStringXY() works on text columns (8px font) and screen rows, rather than 
	// individual pixel addresses. 
	// e.g. x=10, y=2
	// 80 pixels from the left
	// 2 pixels down
	
	*addr = col + ((y - 1)  * SCREEN_WORDS_PER_ROW);
	
	return;
}

void draw_SelectedString(Screen_t *screen, unsigned char col, unsigned char y, unsigned char max_chars, unsigned short fill, char *c){
	// Highlight and flash a selected string of text on screen several times
	
	unsigned short x1;
	unsigned short w;
	unsigned char y1;
	unsigned char h;
	unsigned char i;
	
	x1 = (col * 8);
	w = (8 * max_chars) + 1;
	y1 = y - 1;
	h = 8 + 1;

	draw_StringInvert(screen, col, y, max_chars, screen->font_8x8);
	draw_Flip(screen);
	screen_Vsync(screen, 8);
	for (i = 0; i < 4; i++){
		// Invert text and highlight
		draw_StringInvert(screen, col, y, max_chars, screen->font_8x8);
		draw_Flip(screen);
		screen_Vsync(screen, 2);
		// Normal text
		draw_StringInvert(screen, col, y, max_chars, screen->font_8x8);
		draw_Flip(screen);
		screen_Vsync(screen, 2);
	}
}

void draw_StringInvert(Screen_t *screen, unsigned char col, unsigned char y, unsigned char max_chars, fontdata_t *fontdata){
	// Invert the colours of an already existing string on screen
	unsigned short start_p;
	unsigned short *p;
	unsigned char font_row;
	unsigned char pos;
	unsigned short word;
	
	draw_GetStringXY(col, y, &start_p);
	
	// Reposition write position
	p = (unsigned short*) screen->buf;
	p += start_p;
	
	// For every row of data in a font (default is 8 rows)
	for(font_row = 0; font_row < fontdata->height + 1; font_row++){
		
		// For every character on this line
		for (pos = 0; pos < max_chars; pos++){
			word = *p;
			*p = ~word;
			p++;
		}
		// Increment write position to next row of characters
		p = (unsigned short*) screen->buf;
		p += start_p;
		p += (font_row + 1) * SCREEN_WORDS_PER_ROW;;
	}
}

unsigned short draw_String(Screen_t *screen, unsigned char col, unsigned char y, unsigned char max_chars, unsigned char max_rows, unsigned short offset_chars, fontdata_t *fontdata, unsigned short fill, char *c, unsigned char mode){
	// Put a string of text on the screen, at a set of coordinates (x,y
	// using a specific font.
	// max_chars and max_rows defines a bounding box
	//
	// Embedded newlines (\n) are observed
	// Word-wrapping is performed if the current word would run beyond the end of max_chars
	//
	// Returns 0 on success (all characters have been printed), or number of characters that 
	// have been printed (and can be used as the offset into the string for the next call)
	//
	// Note: This only supports 8x8 fonts.
	
	unsigned short start_p;
	unsigned short *p;
	unsigned short mask;
	unsigned short pos;
	unsigned char i;
	unsigned short row_size = (8 * SCREEN_WORDS_PER_ROW);
	unsigned short string_len = strlen(c);
	
	unsigned char current_rows = 0;
	unsigned char current_chars = 0;
	unsigned char skip = 0;
	unsigned short original_fill = fill;
	
	// Empty string
	if (strlen(c) == 0){
		return 0;
	}
	
	// Calculate starting address
	draw_GetStringXY(col, y, &start_p);
	
	// Reposition write position
	p = (unsigned short*) screen->buf;
	p += start_p;
	
	current_chars = 0;
	current_rows = 1;
	
	for (pos = offset_chars; pos < string_len; pos++){
		// If we started the loop after processing a tag, unset the skip variable
		skip = 0;
		// Next character
		i = (unsigned char) c[pos];
		// Is this the start of a new tag?
		if (i == TEXT_TAG_START){
			// Have we got at least 2 more characters before end of string?
			if ((pos + 2) <= string_len){
				// Is the character +2 from our current position a closing tag?
				if (c[pos + 2] == TEXT_TAG_END){					
					// Figure out the tag
					switch(c[pos + 1]){
						case TEXT_TAG_GREEN:
							fill = PIXEL_GREEN;
							break;
						case TEXT_TAG_RED:
							fill = PIXEL_RED;
							break;
						case TEXT_TAG_WHITE:
							fill = PIXEL_WHITE;
							break;
						case TEXT_TAG_YELLOW:
							// Not supported in 4 colour mode on QL
							break;
						case TEXT_TAG_BLUE:
							// Not supported in 4 colour mode on QL
							break;
						case TEXT_TAG_COLOUR_CLEAR:
							fill = original_fill;
							break;
						default:
							break;
					}
					
					// Increment loop pointer to skip past these 3 characters
					pos += 2;
					// Set skip variable, so we don't try and print anything
					// and instead start the loop at the new character position 
					// from above.
					skip = 1;
				}
			}
		}
		if (!skip){	
			if ((current_chars == 0) && (i == 0x20)){
				// If position 0 and a space, skip it
				current_chars = 0;
				
			} else if ((i == 0x0A) || (i == 0x0D)){
				// Newline characters
				if (current_rows < max_rows){
					// We have at least 1 more row of text left
					p = (unsigned short*) screen->buf;
					start_p += row_size; // Set a new start position to this new line
					p += start_p;
					current_chars = 0;
					current_rows++;	
				} else {
					// We've exceeded number of allowed rows, return
					// with the current position in the string.
					screen->dirty = 1;
					return pos;	
				}
			} else {
			
				if ((word_length(c, pos, string_len) + current_chars) <= max_chars){
					// This character and the rest of word up to the next space or 
					// newline CAN fit on current row
					
					// Find the bitmap for this character in the font table
					draw_FontSymbol(i, fontdata, fill, p, mode);
					p = (unsigned short*) screen->buf;
					current_chars++;
					p += start_p + current_chars;
				} else {
					// This character and the rest of word up to the next space or 
					// newline CANNOT fit on current row
					
					if (current_rows < max_rows){
						// We have at least 1 more row of text left
						p = (unsigned short*) screen->buf;
						start_p += row_size; // Set a new start position to this new line
						p += start_p;
						current_rows++;			
						
						if (i != 0x20){
							// Find the bitmap for this character in the font table
							draw_FontSymbol(i, fontdata, fill, p, mode);
							p = (unsigned short*) screen->buf;
							current_chars = 1;
							p += start_p + current_chars;
						} else {
							// Don't print a leading space if it is the first
							// character on a line. Just skip to next character.
							// This 'left justifies' the text.
							p = (unsigned short*) screen->buf;
							current_chars = 0;
							p += start_p + current_chars;
						}
					} else {
						
						// We've exceeded number of allowed rows, return
						// with the current position in the string.
						screen->dirty = 1;
						return pos;	
					}
				}
			}
		}
	}
	// All characters have been printed
	screen->dirty = 1;
	return 0;	
}

void draw_FontSymbol(unsigned char ascii_num, fontdata_t *fontdata, unsigned short fill, unsigned short *pos, unsigned char mode){
	// Draws a single character in a given colour, at screen position pointed at by p.
	
	unsigned char font_row;
	unsigned char font_symbol;
	unsigned short mask;
	
	if ((ascii_num >= fontdata->ascii_start) && (ascii_num < (fontdata->ascii_start + fontdata->n_symbols))){
		font_symbol = ascii_num - fontdata->ascii_start;
	} else {
		font_symbol = fontdata->unknown_symbol;
	}
	for(font_row = 0; font_row < fontdata->height; font_row++){
		switch(fill){
			case PIXEL_BLACK:
				mask = (fontdata->symbol[font_symbol][font_row] << 8) + fontdata->symbol[font_symbol][font_row];
				break;
			case PIXEL_WHITE:
				mask = (fontdata->symbol[font_symbol][font_row] << 8) + fontdata->symbol[font_symbol][font_row];
				break;
			case PIXEL_RED:
				mask = (unsigned short) fontdata->symbol[font_symbol][font_row];
				break;
			case PIXEL_GREEN:
				mask = (unsigned short) (fontdata->symbol[font_symbol][font_row] << 8);
				break;	
		}
		
		// All other font get or-ed against background
		switch(mode){
			case MODE_PIXEL_SET:
				*pos = mask;
				break;
			case MODE_PIXEL_OR:
				*pos = *pos | mask;
				break;
			case MODE_PIXEL_XOR:
				*pos = *pos ^ mask;
				break;
			case MODE_PIXEL_AND:
				*pos = *pos & mask;
				break;
			default:
				*pos = *pos | mask;
				break;
		}
		pos += SCREEN_WORDS_PER_ROW;
	}
}

/*
int draw_BitmapAsync(Screen_t *screen, int bmpfile){
	// Load from file, decode and display, line by line
	// Every time the function is called, another line is read, decoded and displayed
	
	int	status;					// Status returned from fread/fseek
	unsigned int i = 0;			// Outer loop counter
	unsigned int ii = 0;		// Inner loop counter
	unsigned short start_addr;	// The first pixel
	unsigned short end_addr;	// The last pixel
	unsigned short *p;
	unsigned short mask = 0;
	unsigned short c, row_pos = 0;
	unsigned short i_start = 0;
	unsigned short i_end = 0;
	unsigned short last_word = 0;
	unsigned char bmp_pixel = 0;
	unsigned char pixel_left = 0;	// left byte of a 16bit QL pixel
	unsigned char pixel_right = 0;	// right byte of a 16bit QL pixel
	unsigned char nibble1 = 0;		// Half a byte - a single pixel in 4bpp mode
	unsigned char nibble2 = 0;		// Half a byte - a single pixel in 4bpp mode
	unsigned char pix_pos = 0;
	unsigned char start_bits = 0;
	unsigned char end_bits = 0;
	unsigned char remain_bits = 0;	
	
	// Only supporting 1bpp and 4bpp (first four colours) mode BMP on the QL
	if ((screen->bmp->bpp != BMP_1BPP) && (screen->bmp->bpp != BMP_4BPP)){
		//printf("Async bpp error\n");
		return BMP_ERR_BPP;
	}

	// BMP header has not been read yet
	if (screen->bmp->offset <= 0){
		//printf("Async header not read error\n");
		return BMP_ERR_READ;
	}
	
	if (screen->bmpstate->rows_remaining == screen->bmp->height){
		//printf("Async seek to: %u in %u\n", bmpdata->offset, bmpfile);
		// This is a new image, or we haven't read a row yet			
		// Seek to start of data section in file
		status = lseek(bmpfile, screen->bmp->offset, SEEK_SET);
		if (status != screen->bmp->offset){
			screen->bmpstate->width_bytes = 0;	// not used in 1bpp/4bpp mode
			screen->bmpstate->rows_remaining = 0;
			//printf("Async error seeking data\n");
			return BMP_ERR_READ;
		}
	}	
	
	// Read a row of pixels
	if (screen->bmp->bpp == BMP_4BPP){
		while(i < screen->bmp->row_unpadded){
			pixel_left = 0;
			pixel_right = 0;
			pix_pos = 7;		// Working on bit position 0 (and 1) of the on-screen pixel
			// Read 4 bytes from image == 8x 4bpp pixels == 1x 16bit QL screen location
			for (ii = 0; ii < 4; ii++){
				
				status = read(bmpfile, &bmp_pixel, 1);
				if (status < 1){
					//printf("Async error reading data\n");
					screen->bmpstate->width_bytes = 0; // not used in 1bpp/4bpp mode
					screen->bmpstate->rows_remaining = 0;
					return BMP_ERR_READ;
				}
				nibble1 = bmp_pixel >> 4; 		// Get pixel 1 from the byte
				nibble2 = bmp_pixel & 0x0f;		// Get pixel 2 from the byte
				switch(nibble1){
					case 0:
						// Colour 1 aka Black
						clearbit(pixel_left, pix_pos);
						clearbit(pixel_right, pix_pos);
						break;
					case 1:
						// Colour 2 aka Green
						setbit(pixel_left, pix_pos);
						clearbit(pixel_right, pix_pos);
						break;
					case 2:
						// Colour 3 aka Red
						clearbit(pixel_left, pix_pos);
						setbit(pixel_right, pix_pos);
						break;
					case 3:
						// Colour 3 aka White
						setbit(pixel_left, pix_pos);
						setbit(pixel_right, pix_pos);
						break;
					default:
						clearbit(pixel_left, pix_pos);
						clearbit(pixel_right, pix_pos);
						break;
				}
				switch(nibble2){
					case 0:
						// Colour 1 aka Black
						clearbit(pixel_left, pix_pos - 1);
						clearbit(pixel_right, pix_pos - 1);
						break;
					case 1:
						// Colour 2 aka Green
						setbit(pixel_left, pix_pos - 1);
						clearbit(pixel_right, pix_pos - 1);
						break;
					case 2:
						// Colour 3 aka Red
						clearbit(pixel_left, pix_pos - 1);
						setbit(pixel_right, pix_pos - 1);
						break;
					case 3:
						// Colour 3 aka White
						setbit(pixel_left, pix_pos - 1);
						setbit(pixel_right, pix_pos - 1);
						break;
					default:
						clearbit(pixel_left, pix_pos - 1);
						clearbit(pixel_right, pix_pos - 1);
						break;
				}
				// Next time around, we work on the next two bit positions of the on-screen pixel
				pix_pos = pix_pos - 2;
			}
			// Store this decoded word / block of 8 pixels
			screen->bmpstate->pixels[row_pos] = (pixel_left << 8) + pixel_right;
			row_pos++;
			i = i + 4;
		}
	} else {
		return BMP_ERR_BPP;	
	}

	if (status != screen->bmp->row_unpadded){
		// Seek the number of bytes left in this row
		if ((screen->bmp->row_padded - screen->bmp->row_unpadded) != 0){
			status = lseek(bmpfile, (screen->bmp->row_padded - screen->bmp->row_unpadded), SEEK_CUR);
			if (status != (screen->bmp->row_padded - screen->bmp->row_unpadded)){
				screen->bmpstate->width_bytes = 0;
				screen->bmpstate->rows_remaining = 0;
				//printf("Async error seeking padding bytes (wanted: %u, got %u)\n", (bmpdata->row_padded - bmpdata->row_unpadded), status);
				return BMP_ERR_READ;
			}
		}
	} else {
		// Seek to end of row
		if (screen->bmp->row_padded != screen->bmp->row_unpadded){
			lseek(bmpfile, (screen->bmp->row_padded - screen->bmp->row_unpadded), SEEK_CUR);
		}
	}
	
	// Get coordinates
	draw_GetXY(screen->bmpstate->x, screen->bmpstate->y + screen->bmpstate->rows_remaining - 1, &start_addr, &start_bits);
	draw_GetXY(screen->bmpstate->x + screen->bmp->width, screen->bmpstate->y + screen->bmpstate->rows_remaining, &end_addr, &end_bits);
	remain_bits = 8 - start_bits;
	
	// Reposition screen write position for current x/y position
	p = (unsigned short*) screen->buf;
	p += start_addr;
	last_word = (screen->bmp->width / 8);
	
	// Write any starting pixels if not on a 8 pixel boundary
	if (start_bits != 0){
		
		pixel_left = (unsigned char)((screen->bmpstate->pixels[0] & 0xff00) >> 8) >> start_bits;
		pixel_right = (unsigned char)(screen->bmpstate->pixels[0] & 0x00ff) >> start_bits;
		mask = (pixel_left << 8) + pixel_right;
		
		// OR with any existing leading bits; safest
		*p = *p | mask;
		
		// Move on pointer to next screen word
		p++;
		
		// Save a copy of the current 8 pixels (including those we masked off)
		mask = screen->bmpstate->pixels[0];
		
		// Start 1 word into the pixel array
		i_start = 1;
	}
	// Construct the mask for the last word if it doesn't end on a 8 pixel boundary
	if (end_bits != 0){
		i_end = 1;
	}
	
	// Starting from +1 (if we are not on an 8 pixel boundary)
	// Construct each 8 pixel block in turn, using the masked-off
	// bits of the previous 8 pixels, if necessary.
	// Copy to screen/buffer
	// Increment screen/buffer word pointer
	for(i = i_start; i < (last_word); i++){
		
		if (i_start){
			// This is tricky if we are not on an 8 pixel boundary....
			
			// Use the last pixels we masked off from the previous word
			pixel_left = (unsigned char)((mask & 0xff00) >> 8) << remain_bits;
			pixel_right = (unsigned char)(mask & 0x00ff) << remain_bits;
			
			// Add on the first pixels from the current word
			pixel_left += (unsigned char)((screen->bmpstate->pixels[i] & 0xff00) >> 8) >> start_bits;
			pixel_right += (unsigned char)(screen->bmpstate->pixels[i] & 0x00ff) >> start_bits;
			
			*p = (unsigned short)(pixel_left << 8) + pixel_right;
			
			// Store the current word for the next pass
			mask = screen->bmpstate->pixels[i];
			
		} else {
			// On a 8 pixel boundary, just copy the next block of 8 pixels
			*p = screen->bmpstate->pixels[i];
		}
		p++;
		
		
	}
	
	// If we have any end bits, set the last 8 pixels
	if (i_end){
		
		// Use the last pixels we masked off from the previous word
		pixel_left = (unsigned char)((mask & 0xff00) >> 8) << remain_bits;
		pixel_right = (unsigned char)(mask & 0x00ff) << remain_bits;

		// OR with any existing trailing bits; safest
		*p = *p | (unsigned short)(pixel_left << 8) + pixel_right;
	}
	
	// Decrement number of rows left
	if (screen->bmpstate->rows_remaining == 1){
		screen->bmpstate->rows_remaining = 0;
	} else {
		screen->bmpstate->rows_remaining--;	
	}
	
	return BMP_OK;
	
}

int draw_BitmapAsyncFull(Screen_t *screen, unsigned short x, unsigned short y, char *filename){
	// Display a bitmap using the async call, in its entirety, using no-more than 1 line
	// worth of allocated memory
	
	int f;
	short status;
	
	f = open(filename, O_RDONLY);
	if (f < 0){
		ui_DrawError(screen, GENERIC_FILE_MSG, SCREEN_ASYNCBMP_FILE, 0);
		return DRAW_OPEN_BMPFILE;
	}
	
	// Read image header
	bmp_Init(screen->bmp);
	status = bmp_ReadImage(f, screen->bmp, 1, 0); 
	if (status != 0){
		ui_DrawError(screen, SCREEN_ASYNCBMP_MSG, SCREEN_ASYNCBMP_HEADER, status);
		close(f);
		return status;
	} else {
		//strncpy(screen->bmp->filename, filename, BMP_FILENAME_LEN);
		// Set rows remaining based on height set in header
		bmp_InitState(screen->bmpstate);
		screen->bmpstate->rows_remaining = screen->bmp->height;
		screen->bmpstate->x = x;
		screen->bmpstate->y = y;
		
		//bmp_Print(screen->bmp);
		//bmp_PrintState(screen->bmpstate);
		
		// Loop until all rows processed
		while (screen->bmpstate->rows_remaining > 0){
			status = draw_BitmapAsync(screen, f);
			if (status != BMP_OK){
				ui_DrawError(screen, SCREEN_ASYNCBMP_MSG, SCREEN_ASYNCBMP_DISPLAY, status);
				close(f);
				return status;	
			}
		}
	}
	close(f);
	screen->dirty = 1;
	return status;
}
*/

int draw_Sprite(Screen_t *screen, unsigned short x, unsigned short y, ssprite_t *sprite, unsigned char portrait){
	// Display a normal (non-boss) sprite on screen at the given coordinates
	
	unsigned short *p;				// Pointer to the current position in screen memory
	unsigned short start_addr = 0;	// The screen address of the first (top left) pixel
	unsigned short end_addr = 0;	// The screen address of the last (bottom right) pixel
	unsigned short i = 0; 			// Outer (rows) loop counter
	unsigned short ii = 0; 			// Inner (screen words) loop counter
	unsigned short mask = 0;
	unsigned char pixel_left = 0;	// left byte of a 16bit QL pixel
	unsigned char pixel_right = 0;	// right byte of a 16bit QL pixel
	unsigned char start_bits = 0;	// Number of leading pixels we skip if not on an 8 pixel boundary
	unsigned char end_bits = 0;		// Number of trailing pixels we skip if not on an 8 pixel boundary
	unsigned char remain_bits = 0;	// Number of leading pixels we display, if not on an 8 pixel boundary
	unsigned char last_word = 0;	// The number of screen words we write in any single row of pixels
	unsigned char i_start = 0;
	unsigned char i_end = 0;
	unsigned char i_start_pos = 0;
	unsigned short *pixels;			// Pointer to either the sprite->portrait or sprite->pixels data
	
	if (portrait){
		pixels = (unsigned short*) sprite->portrait;
	} else {
		pixels = (unsigned short*) sprite->pixels;	
	}
	
	//printf("%d x %d @ %d,%d\n", sprite->width, sprite->height, x, y);
	
	// Get coordinates
	draw_GetXY(x, y, &start_addr, &start_bits);
	draw_GetXY(x + sprite->width, y + sprite->height, &end_addr, &end_bits);
	
	// How many pixels we have can display at position x (!=8 if we are not directly on an 8 pixel screen boundary)
	remain_bits = 8 - start_bits;
	
	// Reposition screen write position for current x/y position
	p = (unsigned short*) screen->buf;
	p += start_addr;
	
	// The number of screen words we write in any row
	last_word = sprite->width / 8;
	
	// For every row of pixels in the sprite....
	for(i = 0; i < sprite->height; i++){
				
		// Write any starting pixels if not on a 8 pixel boundary
		if (start_bits != 0){
						
			//pixel_left = (unsigned char)((sprite->pixels[i * last_word] & 0xff00) >> 8) >> start_bits;
			//pixel_right = (unsigned char)(sprite->pixels[i * last_word] & 0x00ff) >> start_bits;
			pixel_left = (unsigned char)((pixels[i * last_word] & 0xff00) >> 8) >> start_bits;
			pixel_right = (unsigned char)(pixels[i * last_word] & 0x00ff) >> start_bits;
			mask = (pixel_left << 8) + pixel_right;
			
			// OR with any existing leading bits; safest
			*p = *p | mask;
			
			// Move on pointer to next screen location
			p++;
			
			// Save a copy of the current 8 pixels (including those we masked off)
			// So that we can use them in the main inner loop
			//mask = sprite->pixels[i * last_word];
			mask = pixels[i * last_word];
			
			// The main inner loop now needs to start 1 word into the pixel array
			i_start = (i * last_word) + 1;
			i_start_pos = 1;
		} else {
			i_start = i * last_word;
			i_start_pos = 0;
		}
		
		// Construct the mask for the last word if it doesn't end on a 8 pixel boundary
		if (end_bits != 0){
			i_end = 1;
		} else {
			i_end = 0;	
		}
				
		// Starting from +1 (if we are not on an 8 pixel boundary)
		// Construct each 8 pixel block in turn, using the masked-off
		// bits of the previous 8 pixels, if necessary.
		// Copy to screen/buffer
		// Increment screen/buffer word pointer
		
		// For every screen word (8 pixels) on this row
		
		for(ii = i_start; ii < (i_start + last_word - 1); ii++){
			if (i_start_pos){
				// This is tricky if we are not on an 8 pixel boundary....
				
				// Use the last pixels we masked off from the previous word
				pixel_left = (unsigned char)((mask & 0xff00) >> 8) << remain_bits;
				pixel_right = (unsigned char)(mask & 0x00ff) << remain_bits;
				
				// Add on the first pixels from the current word
				//pixel_left += (unsigned char)((sprite->pixels[ii] & 0xff00) >> 8) >> start_bits;
				//pixel_right += (unsigned char)(sprite->pixels[ii] & 0x00ff) >> start_bits;
				pixel_left += (unsigned char)((pixels[ii] & 0xff00) >> 8) >> start_bits;
				pixel_right += (unsigned char)(pixels[ii] & 0x00ff) >> start_bits;
				
				*p = (unsigned short)(pixel_left << 8) + pixel_right;
				
				// Store the current word for the next pass
				//mask = sprite->pixels[ii];
				mask = pixels[ii];
				
			} else {
				// On a 8 pixel boundary, just copy the next block of 8 pixels
				//*p = sprite->pixels[ii];
				*p = pixels[ii];
			}
			p++;
		}
		
		// If we have any end bits, set the last 8 pixels
		if (i_end){
			// Use the last pixels we masked off from the previous word
			pixel_left = (unsigned char)((mask & 0xff00) >> 8) << remain_bits;
			pixel_right = (unsigned char)(mask & 0x00ff) << remain_bits;
	
			// OR with any existing trailing bits; safest
			*p = *p | (unsigned short)(pixel_left << 8) + pixel_right;
		}
		
		// Reposition screen memory pointer to next row
		p = (unsigned short*) screen->buf;
		p += start_addr + (i * SCREEN_WORDS_PER_ROW);
		
		// Next row, increment by an image width worth of pixels into the data
		i_start += last_word;
	}
	
	return BMP_OK;
	
}
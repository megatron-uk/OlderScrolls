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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <qdos.h>

#ifndef _CONFIG_H
#include "../common/config.h"
#define _CONFIG_H
#endif
#ifndef _DRAW_H
#include "../common/draw.h"
#define _DRAW_H
#endif
#ifndef _UTILS_H
#include "../common/utils.h"
#define _UTILS_H
#endif
#ifndef _BMP_H
#include "bmp_ql.h"
#define _BMP_H
#endif

unsigned char screen_Init(){
	// Initialise screen and/or offscreen buffers
	unsigned char status;
	FILE *f;
	bmpdata_t *bmp;
	short fontstatus;
	
	// Where our physical video memory is located
	screen.screen = SCREEN_BASE;
	
	// Offscreen rendering (requires a free region of 32kb)
	// This will composite all UI elements and text, only
	// flipping the buffer to screen once all elements are
	// in place.
	screen.indirect = 1;
	screen.offscreen = malloc(SCREEN_BYTES);
	if (screen.offscreen == NULL){
		
		// Couldn't allocate memory for offscreen buffer
		// - we might be on a base model QL with only 128kb
		//printf("- Offscreen buffer: 0 bytes\n");
		
		// Direct rendering - write direct to screen memory
		// but this might show flickering as we write the
		// individual UI elements and/or text
		screen.indirect = 0;
		screen.offscreen = NULL;
		screen.buf = (unsigned short*) screen.screen;
	} else {
		//printf("- Offscreen buffer: %d bytes\n", SCREEN_BYTES);
		screen.buf = (unsigned short*) screen.offscreen;
	}
	
	screen.win = io_open(SCREEN_MODE, 0);
	screen.f = fopen(SCREEN_MODE, "rw");
	
	// Initialise any fonts
	screen.bmp = (bmpdata_t *) malloc(sizeof(bmpdata_t));
	if (screen.bmp == NULL){
		// Couldn't allocate memory for bitmap
		return 2;	
	}
	screen.bmp->pixels = NULL;
	
	// Load and process the 8x8 font
	f = fopen(FONT_8X8, "rb");
	if (f == NULL){
		// Couldn't open font
		return 3;
	}
	screen.font_8x8 = (fontdata_t *) malloc(sizeof(fontdata_t));
	if (screen.font_8x8 == NULL){
		// Couldn't allocate memory for font table
		return 4;	
	}
	fontstatus = bmp_ReadFont(f, screen.bmp, screen.font_8x8, 1, 1, 8, 8);
	if (fontstatus < 0){
		// Couldn't process font bitmap to table
		return 5;
	}
	//printf("- Font data: %d bytes\n", sizeof(fontdata_t));
	screen.font_8x8->unknown_symbol = 37;	// Unknown characters are replaced with '%'
	screen.font_8x8->ascii_start = 32;		// First ascii char in set is ' '
	
	// Destroy the bmp pixel memory
	free(screen.bmp->pixels);
	fclose(f);
	
	return 0;
}

void screen_Exit(){
	// Free any allocated screen
	if (screen.indirect){
		free(screen.offscreen);
		screen.indirect = 0;
	}
	
	// Close any io channel handles
	fclose(screen.f);
}

void draw_Clear(){
	// Clear screen (or offscreen buffer)
	
	unsigned short i;	// Loop counter
	unsigned short *p;	// Pointer to the current 8 pixel block of screen memory (stored as a pair of bytes / single 16bit word)
	
	// Initialise the pointer to the start of screen memory buffer
	p = (unsigned short*) screen.buf;
	
	// For every 8 pixel block / 16bit word, set those pixels to a given colour
	for (i = 0 ; i < SCREEN_BLOCKS; i++){
		*p = PIXEL_BLACK;
		p++;
	}
}

void draw_Flip(unsigned short **buf){
	// Swap offscreen buffer with video memory,
	// if currently enabled.
	
	if (screen.dirty){
	
		// Check for vsync
		// TO DO
		
		// Copy offscreen buffer
		if (screen.indirect){
			memcpy((unsigned char*) screen.screen, screen.buf, SCREEN_BYTES);
		}
		
		screen.dirty = 0;
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
	// Write the offset into &addr, with the number of bits to skip
	// at that address into &bits.
	
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

void draw_HLine(unsigned short x, unsigned short y, unsigned short length, unsigned short fill, unsigned char pad, unsigned char mode){
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
	
	p = (unsigned short*) screen.buf;
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
	
	screen.dirty = 1;
}

void draw_VLine(unsigned short x, unsigned short y, unsigned short length, unsigned short fill, unsigned char mode){
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
		if (start_bits > 1){
			mask = (0x01 << (7 - start_bits));
		} else {
			mask = (0x01 << 6);
		}
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
	p = (unsigned short*) screen.buf;
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
						*p = *p | ((drawing_mask_lo << 8) + 0x00);
					} else {
						*p = ((drawing_mask_lo << 8) + 0x00);
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
	
	screen.dirty = 1;
}

void draw_Box(unsigned short x, unsigned short y, 
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
			draw_HLine(x + borderpx, y + borderpx + i, length - (2 * borderpx), centrefill, pad, mode);
			if (enable_pad){
				if (pad == 1){
					pad = 0;
				} else {
					pad = 1;
				}
			}
		}
	}
	
		// Detect if we need to offset rows for cross-hatching effect
	if (borderpx > 1){
		if (draw_IsStippled(borderfill)){
			enable_pad = 1;
		} else {
			enable_pad = 0;
		}
	}
	pad = 0;
	
	// Borders
	for (i = 0; i < borderpx; i++){
		// Top line
		draw_HLine(x, y + i, length, borderfill, pad, mode);
		// Left
		draw_VLine(x + i, y + borderpx - pad, height - (borderpx * 2) + 1 + pad, borderfill, MODE_PIXEL_OR);
		// Right
		draw_VLine(x + (length - i) - 1, y + borderpx - pad, height - (borderpx * 2) + 1 + pad, borderfill, MODE_PIXEL_OR);
		// Bottom
		draw_HLine(x, y + height - i, length, borderfill, pad, mode);
		if (enable_pad){
			if (pad == 1){
				pad = 0;
			} else {
				pad = 1;
			}
		}
	}
	screen.dirty = 1;
	return;
}

void draw_Pixel(unsigned short x, unsigned short y, unsigned short fill){
	screen.dirty = 1;
}

void draw_GetStringXY(unsigned short col, unsigned short y, unsigned short *addr){
	
	// Although there are 512x256 pixels in in the QL, 
	// the odd indirect pixel addressing means that it becomes
	// rather difficult to place text characters at non divisible-by-8
	// locations
	
	// draw_GetStringXY() works on text columns and rows, rather than 
	// individual pixel addresses. 
	// e.g. x=10, y=2
	// 80 pixels from the left
	// 16 pixels down
	
	*addr = col + ((y - 1)  * SCREEN_WORDS_PER_ROW);
	
	return;
}

unsigned short draw_String(unsigned char col, unsigned char y, unsigned char max_chars, unsigned char max_rows, unsigned short offset_chars, fontdata_t *fontdata, unsigned short fill, char *c){
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
	p = (unsigned short*) screen.buf;
	p += start_p;
	
	current_chars = 0;
	current_rows = 1;
	for (pos = offset_chars; pos < strlen(c); pos++){
		// If we started the loop after processing a tag, unset the skip variable
		skip = 0;
		// Next character
		i = (unsigned char) c[pos];
		// Is this the start of a new tag?
		if (i == TEXT_TAG_START){
			// Have we got at least 2 more characters before end of string?
			if ((pos + 2) <= (strlen(c))){
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
					p = (unsigned short*) screen.buf;
					start_p += (8 * SCREEN_WORDS_PER_ROW); // Set a new start position to this new line
					p += start_p;
					current_chars = 0;
					current_rows++;	
				} else {
					// We've exceeded number of allowed rows, return
					// with the current position in the string.
					screen.dirty = 1;
					return pos;	
				}
			} else {
			
				if ((word_length(c, pos) + current_chars) <= max_chars){
					// This character and the rest of word up to the next space or 
					// newline CAN fit on current row
					
					// Find the bitmap for this character in the font table
					draw_FontSymbol(i, fontdata, fill, p);
					p = (unsigned short*) screen.buf;
					current_chars++;
					p += start_p + current_chars;
				} else {
					// This character and the rest of word up to the next space or 
					// newline CANNOT fit on current row
					
					if (current_rows < max_rows){
						// We have at least 1 more row of text left
						p = (unsigned short*) screen.buf;
						start_p += (8 * SCREEN_WORDS_PER_ROW); // Set a new start position to this new line
						p += start_p;
						current_rows++;			
						
						if (i != 0x20){
							// Find the bitmap for this character in the font table
							draw_FontSymbol(i, fontdata, fill, p);
							p = (unsigned short*) screen.buf;
							current_chars = 1;
							p += start_p + current_chars;
						} else {
							// Don't print a leading space if it is the first
							// character on a line. Just skip to next character.
							// This 'left justifies' the text.
							p = (unsigned short*) screen.buf;
							current_chars = 0;
							p += start_p + current_chars;
						}
					} else {
						
						// We've exceeded number of allowed rows, return
						// with the current position in the string.
						screen.dirty = 1;
						return pos;	
					}
				}
			}
		}
	}
	// All characters have been printed
	screen.dirty = 1;
	return 0;	
}

void draw_FontSymbol(unsigned char ascii_num, fontdata_t *fontdata, unsigned short fill, unsigned short *pos){
	// Draws a single character at screen position pointed at by p.
	
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
		*pos = *pos | mask;
		pos += SCREEN_WORDS_PER_ROW;
	}
}
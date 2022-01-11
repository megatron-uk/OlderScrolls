/* draw_ql.h, Sinclair QL specific drawing functions.
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
#include <qdos.h>

#ifndef _BMP_H
#include "bmp_ql.h"
#define _BMP_H
#endif

// Screen definition
struct Screen_t {
		chanid_t win;				// QDOS virtual screen mode
		FILE 	*f;					// QDOS output channel for fprintf stdio functions
        unsigned short x;			// Width of screen
        unsigned short y;			// Height of screen
        unsigned short *buf;		// Pointer to either screen or offscreen buffer
        unsigned int *offscreen;	// Pointer to possible 'off-screen' video buffer
        unsigned int screen;		// Memory address of real video memory
        unsigned char indirect;		// Flag to indicate use of off-screen or direct video memory writes
        unsigned char dirty;		// Flag to indicate buffer has been changed
        fontdata_t *font_8x8;		// 8x8 font bitmap
        bmpdata_t *bmp;				// Bitmap loader
};
extern struct Screen_t screen;		// Global, all functions have visibility of screen data

// Screen defaults for the QL in 512x256 mode
#define SCREEN_MODE				"CON_512x256a0x0_128"
#define SCREEN_BASE 			0x20000	// Where the QL video memory is located
#define SCREEN_WIDTH 			512	
#define SCREEN_HEIGHT 			256
#define SCREEN_BYTES 			32768 	// (W x H / Pixels per byte)
#define SCREEN_BLOCKS 			16384 	// Screen memory is addressed in 16bit words
#define SCREEN_PIXELS_PER_BYTE 8 		// Each byte has 8 pixels
#define SCREEN_WORDS_PER_ROW 	64 		// 64 x 8 pixels per row
#define SCREEN_BYTES_PER_ROW 	128 	// 64 x 8 pixels per row

// Bitmasks which are used to fill in a 8x1 matrix of pixels in a given colour
#define PIXEL_CLEAR				0x1111	// Flag to indicate 'do not fill' when drawing a box
#define PIXEL_BLACK 			0x0000
#define PIXEL_WHITE 			0xFFFF
#define PIXEL_WHITE_STIPPLED 	0xAAAA
#define PIXEL_GREEN 			0xFF00
#define PIXEL_GREEN_STIPPLED 	0xAA00
#define PIXEL_RED 				0x00FF
#define PIXEL_RED_STIPPLED 		0x00AA
#define PIXEL_YELLOW			0xAA55  // Stipples a 8x1 set of pixels in alternativing 
										// green/red to simulate yellow
#define MODE_PIXEL_OR			0x01
#define MODE_PIXEL_SET			0x00
										
void draw_Clear();
void draw_Flip();
void draw_GetXY(unsigned short x, unsigned short y, unsigned short *addr, unsigned char *bits);
void draw_GetStringXY(unsigned short x, unsigned short y, unsigned short *addr);
void draw_HLine(unsigned short x, unsigned short y, unsigned short length, unsigned short fill, unsigned char pad, unsigned char mode);
void draw_VLine(unsigned short x, unsigned short y, unsigned short length, unsigned short fill, unsigned char mode);
void draw_Box(unsigned short x, unsigned short y, 	unsigned short length, unsigned short height, unsigned short borderpx, unsigned short borderfill, unsigned short centrefill, unsigned char mode);
unsigned short draw_String(unsigned char x, unsigned char y, unsigned char max_chars, unsigned char max_rows, unsigned short offset_chars, fontdata_t *fontdata, unsigned short fill, char *c);
void draw_FontSymbol(unsigned char i, fontdata_t *fontdata, unsigned short fill, unsigned short *p);

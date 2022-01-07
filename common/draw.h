/* draw.h, Prototypes for target specific drawing functions.
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

// Common, and target-specific defines and prototypes
// for low-level OlderScrolls-Engine drawing routines
//
// For UI interface routines that use the low-level
// functions, please see ui.h

// ============================================
// Common defines and data structures
// All targets need to implement these.
// ============================================

#define TEXT_TAG_START			0x3C	// >
#define TEXT_TAG_END			0x3E	// >
#define TEXT_TAG_GREEN			0x67	// g
#define TEXT_TAG_RED			0x72	// r
#define TEXT_TAG_WHITE			0x77	// w
#define TEXT_TAG_YELLOW			0x79	// y
#define TEXT_TAG_BLUE			0x62	// b
#define TEXT_TAG_COLOUR_CLEAR	0x43	// C

unsigned char screen_Init();
void screen_Exit();

// ============================================
// Platform specific drawing implementations
// ============================================

// Sinclair QL 16bit m68008
#ifdef TARGET_QL
#include "../src/draw_ql.h"
#endif

// Commodore PET 8bit 6502
#ifdef TARGET_PET
#include "../src/draw_pet.h"
#endif

// IBM PC 8/16/32bit VGA
//#ifdef TARGET_PC_VGA
//#include "../src/draw_vga.h"
//#endif

// Atari ST 16bit m68000
//#ifdef TARGET_ATARI_ST
//#include "../src/draw_st.h"
//#endif

// MSX2 8bit Z80
//#ifdef TARGET_MSX2
//#include "../src/draw_msx2.h"
//#endif
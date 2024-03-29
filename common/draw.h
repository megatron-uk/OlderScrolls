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

#ifndef _DRAW_H
#define _DRAW_H

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

#define SPRITE_CLASS_NONE			0	// A player/enemy without an on-screen sprite
#define SPRITE_CLASS_NORMAL			1	// Players, normal enemies
#define SPRITE_CLASS_LARGE			2	// Boss enemies
#define SPRITE_CLASS_PORTRAIT		3	// In game dialogue/status bar headshots

// SPRITE_CLASS_PORTRAIT
#define DRAW_PORTRAIT_WIDTH		32		// Player/NPC/Enemy portraits
#define DRAW_PORTRAIT_HEIGHT	32
// SPRITE_CLASS_NORMAL
#define DRAW_PC_WIDTH			32		// Normal player character sprites are these dimensions
#define DRAW_PC_HEIGHT			32
#define DRAW_MONSTER_WIDTH		32		// Normal enemy sprites are these dimensions
#define DRAW_MONSTER_HEIGHT		32
// SPRITE_CLASS_LARGE
#define DRAW_BOSS_WIDTH			96		// 'Boss' enemy sprites are these dimensions
#define DRAW_BOSS_HEIGHT		96

// ============================================
// Platform specific drawing implementations
// ============================================

// Sinclair QL 16bit m68008
#ifdef TARGET_QL
#ifndef _DRAW_QL_DEF_H
#include "../src/draw_ql.h"
#endif
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

#endif
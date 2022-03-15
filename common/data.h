/* data.h, Prototypes for target specific data file search/loading functions.
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

#ifndef _DATA_H
#define _DATA_H

#define DATA_TYPE_STORY		0x01
#define DATA_TYPE_MAP		0x02
#define DATA_TYPE_WEAPON	0x03
#define DATA_TYPE_ITEM		0x04
#define DATA_TYPE_MONSTER	0x05

// Datafile formats
// See ../datafiles/Readme.md for details

#define DATA_HEADER_OFFSET			0
#define DATA_HEADER_ENTRY_SIZE		6
#define DATA_HEADER_RECORD_SIZE		2
#define DATA_HEADER_OFFSET_SIZE		4
#define WEAPON_DAT_SIZE				45
#define ITEM_DAT_SIZE				34

// Not defined here, but on a target level
// SPRITE_DAT_SIZE
// PORTRAIT_DAT_SIZE
// BOSS_DAT_SIZE

// ============================================
// These prototypes are standard for all targets
// ============================================

	
// ============================================
// Platform specific UI implementations
// ============================================

// Sinclair QL 16bit m68008
#ifdef TARGET_QL
#include "../src/data_ql.h"
#endif

// Commodore PET 8bit 6502
#ifdef TARGET_PET
#include "../src/data_pet.h"
#endif

// IBM PC 8/16/32bit VGA
//#ifdef TARGET_PC_VGA
//#include "../src/data_vga.h"
//#endif

// Atari ST 16bit m68000
//#ifdef TARGET_ATARI_ST
//#include "../src/data_st.h"
//#endif

// MSX2 8bit Z80
//#ifdef TARGET_MSX2
//#include "../src/data_msx2.h"
//#endif

#endif
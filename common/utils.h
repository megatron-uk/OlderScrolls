/* utils.h, Prototypes for target specific miscellaneous helper functions.
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

// ============================================
// Platform specific drawing implementations
// ============================================

// Sinclair QL 16bit m68008
#ifdef TARGET_QL
#include "../src/utils_ql.h"
#endif

// Commodore PET 8bit 6502
#ifdef TARGET_PET
#include "../src/utils_pet.h"
#endif

// IBM PC 8/16/32bit VGA
//#ifdef TARGET_PC_VGA
//#include "../src/utils_vga.h"
//#endif

// Atari ST 16bit m68000
//#ifdef TARGET_ATARI_ST
//#include "../src/utils_st.h"
//#endif

// MSX2 8bit Z80
//#ifdef TARGET_MSX2
//#include "../src/utils_msx2.h"
//#endif
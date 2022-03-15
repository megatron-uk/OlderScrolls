/* input_ql.h. Prototypes for the Sinclair QL input routines in OlderScrolls-Engine.
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

#ifndef _INPUT_QL_DEF_H
#define _INPUT_QL_DEF_H

#ifndef _DRAW_H
#include "../common/draw.h"
#endif

// Keys that we listen for
#define INPUT_CANCEL	0x1B		// Escape key
#define INPUT_CONFIRM	0x0A		// Return key
#define INPUT_DEBUG		0x2F		// A 'secret' debug key ('/') that shows system info/status

#define INPUT_MOVE		0x4D		// uppercase 'M'
#define INPUT_MOVE_		0x6D		// lowercase 'm'

#define INPUT_TALK		0x54		// uppercase 'T'
#define INPUT_TALK_		0x74		// lowercase 't'

#define INPUT_FIGHT		0x44		// uppercase 'F'
#define INPUT_FIGHT_	0x66		// lowercase 'f'

#define INPUT_LOOT		0x4c		// uppercase 'L'
#define INPUT_LOOT_		0x6c		// lowercase 'l'

#define INPUT_WITHDRAW		0x57		// uppercase 'W'
#define INPUT_WITHDRAW_		0x77		// lowercase 'w'

#define INPUT_REST		0x52		// uppercase 'R'
#define INPUT_REST_		0x72		// lowercase 'r'

#define INPUT_BARTER	0x42		// uppercase 'B'
#define INPUT_BARTER_	0x62		// lowercase 'b'

#define INPUT_PLAYER	0x50		// uppercase 'P'
#define INPUT_PLAYER_	0x70		// lowercase 'p'

#define INPUT_QUIT		0x51		// uppercase 'Q'
#define INPUT_QUIT_		0x71		// lowercase 'q'

// Character screen tabs
#define INPUT_OVERVIEW		0x4F	// P
#define INPUT_OVERVIEW_		0x6F	// p
#define INPUT_ITEMS			0x49	// I
#define INPUT_ITEMS_		0x69	// i
#define INPUT_WEAPONS		0x47	// W
#define INPUT_WEAPONS_		0x77	// w
#define INPUT_MAGIC			0x4D	// M
#define INPUT_MAGIC_		0x6D	// m
#define INPUT_STATUS		0x53	// S
#define INPUT_STATUS_		0x73	// s

// General keys
#define INPUT_D			0x44
#define INPUT_D_		0x64
#define INPUT_I			0x49		// uppercase 'I'			
#define INPUT_I_		0x69		// lowercase 'i'
#define INPUT_E			0x45		// uppercase 'E'
#define INPUT_E_		0x65		// lowercase 'e'
#define INPUT_N			0x4E		// uppercase 'N'
#define INPUT_N_		0x6E		// lowercase 'n'
#define INPUT_R			0x52
#define INPUT_R_		0x72
#define INPUT_S			0x53		// uppercase 'S'
#define INPUT_S_		0x73		// lowercase 's'
#define INPUT_T			0x54		// uppercase 'T'
#define INPUT_T_		0x74		// lowercase 't'
#define INPUT_U			0x55
#define INPUT_U_		0x75
#define INPUT_W			0x57		// uppercase 'W'
#define INPUT_W_		0x77		// lowercase 'w'
#define INPUT_Y			0x59		// uppercase 'Y'
#define INPUT_Y_		0x79		// lowercase 'y'
#define INPUT_1			0x31		// 1
#define INPUT_2			0x32		// 2
#define INPUT_3			0x33		// 3
#define INPUT_4			0x34		// 4
#define INPUT_5			0x35		// 5
#define INPUT_6			0x36		// 6
#define INPUT_7			0x37		// 7
#define INPUT_8			0x38		// 8
#define INPUT_9			0x39		// 9
#define INPUT_0			0x30		// 0

#define INPUT_UP		0xD0		// Cursor up
#define INPUT_DOWN		0xD8		// Cursor down


#endif

// Protos

#ifndef _INPUT_QL_PROTO_H
#define _INPUT_QL_PROTO_H

unsigned char input_Get(Screen_t *screen);
void input_Set(unsigned char key);
void input_Clear(void);
void input_Wait(Screen_t *screen, unsigned char key);
void input_WaitAndReturn(Screen_t *screen);

#endif
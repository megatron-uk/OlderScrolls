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

// General keys
#define INPUT_E			0x45		// uppercase 'E'
#define INPUT_E_		0x65		// lowercase 'e'
#define INPUT_N			0x4E		// uppercase 'N'
#define INPUT_N_		0x6E		// lowercase 'n'
#define INPUT_S			0x53		// uppercase 'S'
#define INPUT_S_		0x73		// lowercase 's'
#define INPUT_W			0x57		// uppercase 'W'
#define INPUT_W_		0x77		// lowercase 'w'
#define INPUT_Y			0x59		// uppercase 'Y'
#define INPUT_Y_		0x79		// lowercase 'y'

unsigned char input_Get(void);
void input_Set(unsigned char key);
void input_Clear(void);
void input_Wait(unsigned char key);
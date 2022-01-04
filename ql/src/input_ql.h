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
#define INPUT_SPACE		0x32
#define INPUT_RETURN	0x0A

short input_Get(void);
void input_Set(unsigned char key);
void input_Clear(void);
void input_Wait(unsigned char key);
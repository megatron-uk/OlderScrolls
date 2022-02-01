/* monsters.h, Generic monster definitions and classes for the 
 OlderScrolls RPG game engine.
 
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

#ifndef _MONSTERS_H

// Monster classes
// Generic humans who get no special abilities
#define HUMAN_UNTRAINED				0x10
#define HUMAN_GENERIC_MELEE			0x11
#define HUMAN_GENERIC_RANGED		0x12
#define HUMAN_GENERIC_MAGIC			0x13
// More typical human RPG classes
#define HUMAN_BARBARIAN				0x14				
#define HUMAN_BARD					0x15
#define HUMAN_CLERIC				0x16
#define HUMAN_DRUID					0x17
#define HUMAN_FIGHTER				0x18
#define HUMAN_PALADIN				0x19
#define HUMAN_RANGER				0x1A
#define HUMAN_ROGUE					0x1B
#define HUMAN_SORCERER				0x1C
#define HUMAN_WARLOCK				0x1D
#define HUMAN_WIZARD				0x1E

// 
#define BEAST_WILD			0x20
#define BEAST_MELEE			0x21
#define BEAST_RANGED		0x22
#define BEAST_MAGIC			0x23

#define _MONSTERS_H
#endif
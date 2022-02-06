/* error.h, Comprehensive list of runtime error codes in OlderScrolls.
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

#ifndef _ERROR_H

#define OK								0
#define MAIN_SCREEN_FAILURE				-1		// Screen initialisation failed
#define MAIN_DATAFILES_MISSING			-2		// One or more essential datafiles are missing

#define BMP_OK							0 		// BMP loaded and decode okay
#define BMP_ERR_NOFILE					-10 	// Cannot find file
#define BMP_ERR_SIZE					-11 	// Height/Width outside bounds
#define BMP_ERR_MEM						-12 	// Unable to allocate memory
#define BMP_ERR_BPP						-13 	// Unsupported colour depth/BPP
#define BMP_ERR_READ					-14 	// Error reading or seeking within file
#define BMP_ERR_COMPRESSED				-15 	// We dont support comrpessed BMP files
#define BMP_ERR_FONT_WIDTH				-16 	// We dont support fonts of this width
#define BMP_ERR_FONT_HEIGHT				-17 	// We dont support fonts of this height
#define BMP_ERR_NODATA					-18 	// No data received from readImage
#define SCREEN_INIT_OK					0
#define SCREEN_INIT_BMPMEMORY			-20		// Unable to malloc memory for BMP data structure
#define SCREEN_INIT_FONTFILE			-21		// Unable to open font file from disk
#define SCREEN_INIT_FONTMEMORY			-22		// Unable to malloc memory for font structure
#define SCREEN_INIT_FONTPROCESS			-23		// Unable to read and process bmp to font
#define SCREEN_INIT_BMPSTATEMEMORY		-24		// Unable to malloc memory for async BMP display structure
#define SCREEN_INIT_PC_SPRITEMEMORY	-25		// Unable to malloc memory for player character sprites and portraits
#define SCREEN_INIT_E_SPRITEMEMORY		-26		// Unable to malloc memory for enemy character sprites and portraits
#define SCREEN_INIT_BOSS_SPRITEMEMORY	-27		// Unable to malloc memory for boss character sprites and portraits
#define DATA_LOAD_OK					0
#define DATA_LOAD_MAP_INDEXFILE			-30 	// Error opening MAP index file
#define DATA_LOAD_MAP_DATFILE			-31 	// Error opening MAP dat file
#define DATA_LOAD_MAP_MISMATCH			-32 	// Found location does not match query
#define DATA_LOAD_STORY_INDEXFILE		-33 	// Error opening Story index file
#define DATA_LOAD_STORY_DATFILE			-34 	// Error opening Story dat file
#define DATA_LOAD_ITEM_INDEXFILE		-35 	// Error opening item index file
#define DATA_LOAD_ITEM_DATFILE			-36 	// Error opening item dat file
#define DATA_LOAD_WEP_INDEXFILE			-37 	// Error opening weapon index file
#define DATA_LOAD_WEP_DATFILE			-38 	// Error opening weapon dat file
#define DATA_LOAD_NPCMEMORY				-39 	// Unable to malloc memory for new NPC entry
#define DATA_LOAD_SPRITEFILE			-40		// Unable to open sprite datafile
#define DATA_LOAD_PORTRAITFILE			-41		// Unable to open portrait datafile
#define DATA_LOAD_BOSSFILE				-42		// Unable to open boss sprite datafile
#define DATA_LOAD_MONSTERFILE			-43		// Unable to open monster datafile
#define DATA_LOAD_NPCFILE				-44		// Unable to open npc datafile
#define DATA_LOAD_MONSTERFILE_SEEK		-45		// Unable to seek in monster/npc datafile
#define DATA_LOAD_MONSTER_MISMATCH		-46		// The found monster id does not match the search id
#define DRAW_OPEN_BMPFILE				-50		// Unable to open bitmap file for async display


// Generic file error messages
#define GENERIC_FILE_MSG				"File Access Error!"

// Data load error messages
#define DATA_LOAD_ERROR_MSG				"Datafile Error!"															// Used as a title
#define DATA_LOAD_MAP_INDEX_MSG			"Unable to open MAP .idx file."
#define DATA_LOAD_MAP_DAT_MSG			"Unable to open MAP .dat file."
#define DATA_LOAD_MAP_MISMATCH_MSG		"The loaded MAP location does not match. Datafile consistency error!"
#define DATA_LOAD_STORY_INDEX_MSG		"Unable to open STORY .idx file."
#define DATA_LOAD_STORY_DAT_MSG			"Unable to open STORY .dat file."
#define DATA_LOAD_MONSTER_DAT_MSG		"Unable to open MONSTER .dat file."
#define DATA_LOAD_NPC_DAT_MSG			"Unable to open NPC .dat file."
#define DATA_LOAD_SPRITE_DAT_MSG		"Unable to open SPRITE .dat file."
#define DATA_LOAD_PORTRAIT_DAT_MSG		"Unable to open PORTRAIT .dat file."
#define DATA_LOAD_BOSS_DAT_MSG			"Unable to open BOSS .dat file."
#define DATA_LOAD_SPRITE_DAT_READ		"Unable to read sufficient bytes from SPRITE .dat file."
#define DATA_LOAD_PORTRAIT_DAT_READ	"Unable to read sufficient bytes from PORTRAIT .dat file."
#define DATA_LOAD_BOSS_DAT_READ			"Unable to read sufficient bytes from BOSS .dat file."
#define DATA_LOAD_MONSTER_DAT_READ		"Unable to read sufficient bytes from MONSTER/NPC .dat file."
#define DATA_LOAD_MONSTER_DAT_SEEK		"Unable to seek to correct location in MONSTER/NPC .dat file."
#define DATA_LOAD_MONSTER_MISMATCH_MSG	"The loaded MONSTER does not match. Datafile consistency error!"

// Out of memory error messages
#define GENERIC_MEMORY_MSG 				"Memory Error!"																// Used as a title
#define DATA_LOAD_NPCMEMORY_MSG			"Error while adding new NPC. Unable to continue."
#define SCREEN_INIT_MEMORY_MSG			"Error while initialising screen and character image data. Unable to continue."

// Bitmap/sprite error messages
#define SCREEN_ASYNCBMP_MSG				"Async Bitmap Error!"
#define SCREEN_ASYNCBMP_FILE			"Unable to open BMP file for async display."
#define SCREEN_ASYNCBMP_DISPLAY			"Error during call for async bitmap display."
#define SCREEN_ASYNCBMP_HEADER			"Error extracting BMP header for async bitmap display."

#define _ERROR_H
#endif

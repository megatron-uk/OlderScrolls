# OlderScrolls Game Datafiles

The datafiles used in the game adventures are stored in Python dictionaries as this makes them much easier to edit and then turn into the platform-specific containers. This is all done on the build host, not on the 8/16-bit targets themselves.

  * story.py
  * map.py
  * weapon.py
  * item.py
  * monster.py (contains players, npc's and monsters)

**NOTE:**

Integers in the datafiles are *ALWAYS* stored big-endian.

---

## story.py

### Description

  * Holds the main story text of the game (the text shown in the central UI window); passages of text that are shown upon key points in the game upon triggering certain actions (visiting a location, defating a monster, etc), as well as the introductory text displayed on the splash screen starting the game.

### Example

    STORY = {
        1 : "Welcome to Sands of Terror!\n\nA fantasy adventure set in a desert...",
        2 : "You are standing at the gates to the ancient city of...",
    }

### Miscellaneous

The data/text section should contain standard ASCII text. The following rules are applied when displaying the text in the OlderScrolls engine:

  1. Embedded newline characters (*\n*) are obeyed.
  2. Words which would go beyond the printable region are automatically placed on a newline, rather than being split.
  3. Leading spaces on a line are stripped (text is left-justified).
  4. If the text spans more than the viewable lines, the player is prompted to scroll to the next page.
  5. Story ID **1** is *special* in that it is shown on the game start-up splash screen and not within the game itself.

Simple control codes can be wrapped around words/characters to enhance the output, for example:

    <g>This text is green<c> but this text is not.

The following codes are available:

    <g> Following text is in green
    <r> Following text is in red
    <w> Following text is in white (which is the default)
    <y> Following text is in yellow (not available on all platforms)
    <C> Clear current colour setting, return to default 
    <f> Following text is in the alternate font set
    <F> Clear current font setting, return to default

Not all codes are interpreted on all targets. On colour targets the minimum set of codes that work are those for *red*, *green*, *white* and *clear* (minimum of 4 colour display mode required). On targets which do not support colour modes or alternate font sets (e.g. text-mode targets such as the Commodore PET), these codes are ignored.

### Processed Datafile Structure

When the Python script is processed, it generates two files of the following format:

  * 3072 byte **index file** (story.idx)
    * Containing **exactly** 512 records
      * Each **record** has...
        * 1x 16bit (unsigned) size field (minimum 3, maximum 4096)
        * 1x 32bit (unsigned) offset field (from start of file)
      * There must be at least **1** header record with data. Following records *may* be blank.

  * 3 byte to 1049600 byte **data file** (story.dat)
    * Containing **up to** 512 data records
      * Each **record** has...
        * 1x 16bit (unsigned) ID field (mandatory)
        * 1x 1 - 2048 byte data field (ASCII text)

**Commodore PET Targets**

TO DO: Note that the PET (and similar 8bit Commodore machines) do not have an implementation of *fseek()*, so an alternative record searching mechanism for records >256 bytes needs to be implemented. 

---

## world.py

### Description

Despite its name, this datafile really contains the entire game world, logic, actions, events and triggers. It's essentially the main game definition.

### Example

    MAP = {
        1 : { 
            'name' : "Market Town",     # Name of the location, fixed width, 32 characters.
            'text' : 1,                 # Text block shown initially upon visiting the location
            'north' : 2,                # Exiting North visits this location
            'north_text' : 7,           # Text (from story.py) shown if north is an available exit, may be blank, in which case just the compass direction will be listed.      
            'north_require' : [],       # Requirement to be met for north to be available
            'south' :,
            'south_text' : ,
            'south_require' : [],
            'east' :,
            'east_text' : ,
            'east_require' : [],
            'west' :,
            'west_require' : [],
            'west_text' : ,
            'spawn_chance' : 75,        # Chance, out of 100, for primary monsters to spawn
            'spawn_list' : [],          # A list of monster ID's (from monster.py) which are spawned. One ID per monster instance - e.g. for up to 5x Giant Scorpions, add 5 Giant Scorpion ID's. Each will be subject to the 'spawn_chance' rating above.
            'spawn_require' : [],       # Requirement to be met for spawn chance to be triggered
            'respawn_chance' : 50,      # Chance, out of 100, for secondary monsters to spawn
            'respawn_list' : [],        # A list of monster ID's which are spawned
            'respawn_require' : [],     # Requirement to be met for respawn chance to be triggered
            'items_list' : ["w8", "i55"],  # List of weapons and items which can spawn in this location. Weapon id's are prefixed with 'w', items with 'i'.
            'items_chance' : [],        # Chance, out of 100, for items to spawn
            'items_require' : [],       # Requirements to be met in order for item spawn chance to be active
            'text_spawn' : 2,           # Text shown (from story.py) IN ADDITION to main text if the primary monster spawn is triggered
            'text_after_spawn' : 99,    # Text shown IN ADDITION to main text when visiting the location after primary spawned monsters is defeated (instead of 'text').
            'text_respawn' : 3,         # Text shown IN ADDITION to main text if the secondary monster spawn is triggered
            'text_after_respawn' : 98   # Text shown IN ADDITION to main text when visiting the location after respawned monsters are defeated (instead of 'text' or 'text_after_spawn').
            'npc1' : 23                 # NPC id '23' appears and can be talked to
            'npc1_require' : [] ,       # Requirement to be met for npc 1 to appear as an option to talk to.
            'npc1_text'  : 52,          # Text shown when talking to the NPC
            'npc2' : 24                 # NPC id '24' appears and can be talked to
            'npc2_require' : [] ,       # Requirement to be met for npc 2 to appear as an option to talk to.
            'npc2_text'  : 53,          # Text shown when talking to the NPC
            'npc3' : 25                 # NPC id '25' appears and can be talked to
            'npc3_require' : [] ,       # Requirement to be met for npc 3 to appear as an option to talk to.
            'npc3_text'  : 54,          # Text shown when talking to the NPC
        },
    }

### Conditions, Requirements and Triggers 

For the list of all the conditions that can be added to a *_requires* block, see the [conditions](Conditions.md) documentation.


### Processed Datafile Structure

TO BE DOCUMENTED

---

## weapon.py

### Description

Contains all of the weapon definitions used within this specific adventure. The weapon characteristics should be mostly similar to anyone who has played any D&D inspired RPG games before.

### Example

    WEAPONS = {
    	1 : {
    		"type"	: '1H', 
    		"class"	: 'SIMPLE',
    		"rarity": 'COMMON',
    		'size' : 'SMALL',
    		"item_id" 	: 1, 
    		"name" 	: "Dagger", 
    		"crit" 	: [20,20, 2] ,
    		"dmgtype" : {
    			"PHYSICAL"	:  [1, 3, 1],
    			"PIERCING"	: [1, 2, 1]
    		},
            "description" : "A simple, straight bladed dagger"
    	},
    	2 : {
    		"type"	: '1H', 
    		"class"	: 'MARTIAL',
    		"rarity": 'COMMON',
    		'size' : 'MEDIUM',
    		"item_id" 	: 2, 
    		"name" 	: "Longsword", 
    		"crit" 	: [20,20, 2] ,
    		"dmgtype" : {
    			"PHYSICAL"	: [1, 6, 1],
    			"SLASHING"	: [1, 2, 1]
    		}
            "description" : "A typical one handed sword"
    	}

### Processed Datafile Structure

When the Python script is processed, it generates a single file *(no index file is needed as all records are fixed-length)* of the following format:

  * 256 byte to 65536 byte **data file** (weapon.dat).
    * Containing **up to** 256 data records
      * Each **data record** has...
        * 1x *16bit integer*, ID field (e.g. 312)
        * 1x *unsigned char*, Weapon Type field (e.g. 1 == 1 Handed)
        * 1x *unsigned char*, Weapon Class field (e.g. 3 == Exotic)
        * 1x *unsigned char*, Rarity field (e.g. 1 == Common)
        * 1x *unsigned char[20]*, Weapon Name field (e.g. "Longsword")
        * 1x *unsigned char*, Critical Threat *from* field (e.g. 19)
        * 1x *unsigned char*, Critical Threat *to* field (e.g. 20)
        * 1x *unsigned char*, Critical Threat *multiplier* field (e.g. 2)
        * 1x *unsigned char*, Number of *Damage Types* field (min 1, max 3)
        * 1x *unsigned char*, Damage Type 1 *type* field
        * 1x *unsigned char*, Damage Type 1 *minimum* field
        * 1x *unsigned char*, Damage Type 1 *maximum* field
        * 1x *unsigned char*, Damage Type 1 *dice number* field
        * 1x *unsigned char*, Damage Type 2 *type* field
        * 1x *unsigned char*, Damage Type 2 *minimum* field
        * 1x *unsigned char*, Damage Type 2 *maximum* field
        * 1x *unsigned char*, Damage Type 2 *dice number* field
        * 1x *unsigned char*, Damage Type 3 *type* field
        * 1x *unsigned char*, Damage Type 3 *minimum* field
        * 1x *unsigned char*, Damage Type 3 *maximum* field
        * 1x *unsigned char*, Damage Type 3 *dice number* field
        * 1x *unsigned char[215]*, Description

---

## monster.py

### Description

The *monster.py* file is something of a misnomer - the datafile contains the definitions of additional player party characters (i.e. people you can 'recruit'), non-playable characters (i.e. people you can 'talk' to), as well as typical monsters you encounter in the game.

The reason for this is simple; *all three classes share exactly the same data structure*.

### Example

    MONSTER = {
	1 : {
        'name' 		: "Ruffian",     		# Name as displayed to the player
        'sprite_type' : "SPRITE_NORMAL",	# 32x32 sprites are normal size
        'sprite_name' : "human1_b",		# Name of bitmap file used in combat screens
        'portrait_name' : "human1_b",		# Name of bitmap file used in text dialogue
        'class'		: "HUMAN_UNTRAINED",	# Class 
        'level'		: 1,					# Level
        'profile'	: 0xA000,				# Attack/behaviour profile, 4 bits each for how aggressive 
        									# the monster is in melee, ranged, magic attack, magic support
        'str' 		: 10,					# Standard attributes - they may be modified by the level and class of the monster
        'dex' 		: 7,					# - as above
        'con' 		: 11,					# - as above
        'wis' 		: 3,					# - as above					
        'intl' 		: 4,					# - as above
        'chr' 		: 2,					# - as above
        'hp' 		: 99,					# Starting hitpoints
        'status' 	: 0,					# Starting bitfield of status effects
        'head'		: -1,					# Item equipped to head (e.g. helmet, hat) - must be present in items.py
        'neck'		: -1,					# Item equipped to neck (e.g. amulet) - must be present in items.py
        'body'		: -1,					# Item equipped to body (e.g. bronze cuirass) - must be present in items.py
        'arms'		: -1,					# Item equipped to arms (e.g. iron pauldrons) - must be present in items.py
        'legs'		: -1,					# Item equipped to legs (e.g. plate greaves) - must be present in items.py
        'hand_r'	: -1,					# Item on right hand (e.g. ring) - must be present in items.py
        'hand_l'	: -1,					# Item on left hand (e.g. ring) - must be present in items.py
        'weapon_r' : 1,					    # ID of weapon in right hand - must be present in weapons.py
        'weapon_l' : -1,					# ID of weapon in left hand - must be present in weapons.py
        'formation' : 0,					# Front / Middle / Rear alignment in combat
        'spells'	: [0,0,0,0,0],			# List of all spells that this monster has access to - must be present in weapons.py
	},

### Processed Datafile Structure

When the Python script is processed, it generates a three files *(no index file is needed as all records are fixed-length)* of the following format, one for each of NPC's, Party Members and Monsters:

  * TBC **data file** (monster.dat)
  * TBC **data file** (npc.dat)
  * TBC **data file** (party.dat)

---

## item.py

### Description

### Example

### Processed Datafile Structure

When the Python script is processed, it generates files of the following format:

  * TBC **data file** (items.dat)

---
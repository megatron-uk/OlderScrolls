# Conditions, Requirements & Triggers

In the game world definition, any *_requires* entry may have 0, 1 or more conditions applied which affects when that Exit will be available, when an NPC is present to talk, whether an item is dropped or if a Monster is spawned.

These requirements are expressed in a compact, 5-byte sequence with 2 leading bytes (evaluation type and number of conditions listed), thus the minimum length of any *_requires* entry is:

  * 2 bytes if no conditions are listed 
    * 1x evaluation type 'empty' flag of 0x00
    * 1x size '0' byte of 0x00
      * e.g 0x00, 0x00
  * 7 bytes if one condition is listed
    * 1x evaluation type flag of 0x10 to 0x40
    * 1x size byte of 0x01
    * 5x bytes of a single condition
      * e.g. 0x10, 0x01, 0x05, 0x01, 0x00 0x5E, 0x01 (*Primary monster at location 0x5E defeated once*)

## Evaluating Requirements

Every requirement listed against a *_requires* entry is evaluated in the same way and returns a boolean True or False. When multiple requirements are listed for a single *_requires* entry, their results can be interpreted in several ways.

The standard mechanism should be assumed to be **AND**, i.e. all requirement must be true. However, you can use any of the options below:

**Evaluation Type**

    # How multiple conditions are evaluated
    COND_EVAL_EMPTY		0x00	# The condition set is EMPTY, do not attempt to evaluate
    COND_EVAL_AND		0x10	# All conditions must evaluate to true. The default.
    COND_EVAL_OR		0x20	# At least one condition must evaluate to true.
    COND_EVAL_NOR		0x30	# All conditions must evaluate to false.
    COND_EVAL_NAND		0x40	# Zero or one, (but no more than one) condition must evaluate to true.

**Examples**

A simple requirement could be expressed as below:

    'north_requires' : [
        "COND_EVAL_OR",                             # At least one condition must be true
        2,                                          # 2 conditions are listed
        ["COND_NO_MONSTERS"],                       # Condition 1
        ["COND_ITEM_TYPE", "ITEM_TYPE_OWN", 99, 1]  # Condition 2
    ]

The above condition expresses a requirement for the **North** exit to be available when either of two conditions are true: no monsters to be currently spawned in this location OR the player must posess at least one instance of Item 99 (a key, portal stone, mystical necklace or similar).

A more elaborate example could be:

    'respawn_requires' : [
        "COND_EVAL_AND",
        3,
        ["COND_MONSTER_DEFEAT_TYPE", "MONSTER_TYPE_PRIMARY", 13, 1],
        ["COND_NPC_TYPE", 1, 3],
        ["COND_PARTY_MEMBER_TYPE", 2, "COND_PARTY_DEAD"],
    ]

In the example above, the monsters in a locations secondary spawn category ('respawn_list') would be brought into the game if **all** of the following where true:

   1. The primary monster at location ID **13** had been defeated at least **once**.
   2. You have talked to the NPC ID **1** exactly three times...
   2. Your party has recruited the optional party member ID **1**.... but they are dead.

What use is this? Well, the above example could be one way in which a member of your party comes back as a powerful undead monster that you have to then fight!

---

## List of Conditions

Note that COND_PC_ATR_TYPE and COND_PARTY_ATR_TYPE both use option flags which are listed at the bottom of this document.

    // ==========================================
    // Basic conditions
    // 		2 bytes for the condition type
    //		3 pad bytes
    // e.g. 0x01 00 00 00 00 
    NO_COND					0x0000 // No conditions
    COND_NO_MONSTERS		0x0001 // No monsters must be currently spawned at this location

    // ==========================================
    // Tests for the player 
    // 		2 bytes for the condition type (0x0100)
    //		param 1: 1 byte for the tested statistic (0x01 == STR). See the chart of available options.
    //		param 2: 1 byte for the tested value (0x0A = 10)
    //		1 pad byte
    // e.g 0x01 00 01 0A 00 // Player has STR 10 or above
    COND_PC_ATR_TYPE			0x01

    // ==========================================
    // Test for anyone in the player party
    // 		2 bytes for the condition type (0x0200)
    //		param 1: 1 byte for the tested statistic (0x06 == INT). See the chart of available options.
    //		param 2: 1 byte for the tested value (0x0C = 12)
    //		1 pad byte
    // e.g 0x02 00 06 0C 00 // Someone in party has INT 12 or above
    COND_PARTY_ATR_TYPE			0x02

    // ==========================================
    // Party Membership
    //		2 bytes for the condition type
    //		param 1: 1 byte for the id of the playable character
    //		param 2: 1 byte for status of the playable character. See the list of available party status checks.
    //		1 pad byte
    // e.g. 0x03 00 02 01 00 // Party member 02 is present and alive
    // e.g. 0x03 00 02 03 00 // Party member 02 is present, but dead
    COND_PARTY_MEMBER_TYPE		0x03

    // ==========================================
    // Locations
    //		1 bytes for the condition type
    //		param 1: 1 byte for the test type (visited, min, max)
    //		param 2: 2 bytes for the location ID
    //		param 3: 1 byte for the number (or pad)
    // e.g. 0x04 00 00 01 00 // Location 1 has been visited
    // e.g. 0x04 01 00 07 01 // Location 7 has been visited NO MORE THAN once
    // e.g. 0x04 02 00 07 09 // Location 7 has been visited MORE THAN 9 times
    COND_MAP_VISIT_TYPE 	0x04
    COND_MAP_VISIT_MAX		0x01 // A map location requires to have visited a maximum number of   times
    COND_MAP_VISIT_MIN		0x02 // A map location requires to have visited a minimum number of times

    // ==========================================
    // Monster spawning tests
    //		1 byte for the condition type
    //		param 1: 1 byte for the test type (primary, secondary)
    //		param 2: 2 bytes for the location ID of the monsters we are checking
    //		param 3: 1 byte for the number
    // e.g. 0x05 01 00 5A 01 // Primary monster at location 90 must have been defeated at least once
    // e.g. 0x05 02 00 0F 03 // Secondary monsters at location 16 must have been defeated at least 3 times
    COND_MONSTER_DEFEAT_TYPE	0x05
    COND_MONSTER_PRI_DEFEATED	0x01	// Primary monster at a given map location must have been defeated 'x' times
    COND_MONSTER_SEC_DEFEATED	0x02	// Secondary monster at a given map location must have been defeated 'x' times

    // ==========================================
    // NPC spawning/talking rules
    //		1 byte for the condition type
    //      param 1: 1 byte for the test type
    //		param 2: 1 bytes for the NPC ID the test is for
    //		param 3: 1 byte for the number
    //		1 pad byte
    // e.g. 0x06 01 1E 03 00 // Must have talked to NPC 30, 3 times
    // e.g. 0x06 01 05 01 00 // Must have talked to NPC 5, 1 times
    COND_NPC_TYPE		0x06
    COND_NPC_TALK		0x01 // Must have talked to NPC 'id' 'x' times

    // ==========================================
    // Items
    //		1 bytes for the condition type
    //      param 1: 1 byte for the test type
    //		param 2: 1 byte for the item ID the test is for
    //		param 3: 1 byte for the number
    //		1 pad byte
    // e.g. 0x07 01 FE 01 00 // Must posess (at least 1 of) item ID 254.
    // e.g. 0x07 02 0D 07 00 // Must not have more than 7 copies of item ID 13.
    COND_ITEM_TYPE		0x07
    COND_ITEM_OWN		0x01 // Must posess the item 'id'
    COND_ITEM_NOTOWN	0x02 // Must NOT posess the item 'id'

    // ==========================================
    // Weapons
    //		1 bytes for the condition type
    //      param 1: 1 byte for the test type
    //		param 2: 1 byte for the weapon ID the test is for
    //		param 3: 1 byte for the number
    //		1 pad byte
    // e.g. 0x08 01 01 01 00 // Must posess (at least 1 of) weapon ID 1.
    // e.g. 0x08 02 05 01 00 // Must not have more than 1 of weapon ID 5.
    COND_WEAPON_TYPE	0x08
    COND_WEAPON_OWN		0x01 // Must posess the weapon 'id'
    COND_WEAPON_NOTOWN	0x02 // Must NOT possess the weapon 'id'

### List of Conditional Options

These options are used by the conditions expressed above.

    // These byte values are used to refer to the statistics of a PC or the Player Party
    COND_TYPE_STR		0x01
    COND_TYPE_DEX		0x02
    COND_TYPE_CON		0x03
    COND_TYPE_WIS		0x04
    COND_TYPE_INT		0x06
    COND_TYPE_CHR		0x07
    COND_TYPE_HP		0x08
    COND_TYPE_GOLD		0x09    // Check against player gold
    COND_TYPE_DMG		0x0A    // Check against how much damage this PC or Party member has caused
    COND_TYPE_INJURY	0x0B    // Check against how much damage this PC or Party member has taken

    // Status of party members
    COND_PARTY_PRESENT	0x01	// Present + alive
    COND_PARTY_DIMISSED	0x02	// Was dismissed
    COND_PARTY_DEAD		0x03	// Dead
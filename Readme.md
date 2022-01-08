# OlderScrolls

OlderScrolls is a D&D style RPG game engine for various 8bit and 16bit consoles and computers of the 1980's and 1990's.

The intention is to provide a single, complete game engine that can be used to implement a different adventure by simply changing the datafiles that it uses. 

---

# Games

The engine can be built with several game datafiles to produce multiple stand alone games. 

For technical details of the game datafile formats, please look [here](datafiles/Readme.md).

The games included with the engine are:

  * ### Leafy Glade

This is a basic, sample game data set that is used to test basic functionality of the various engine targets and changes to the engine itself.

See more [details](datafiles/leafy_glade/Readme.md)

  * ### Sands of Terror

A more full-featured game set in a typical D&D desert world, with a quest to solve, party members to recruit, loot to find and monsters to kill.

See more [details](datafiles/sand_of_terror/Readme.md)

---

# Engine Features

  * Very loosely based on a D&D type ruleset
  * Screen/Location based movement (compass points + up/down)
  * Turn based combat
  * Randomised monster encounters
  * Hand to hand, ranged and magic abilities
  * Scripted events (actions can be triggered on posession of a specific item, killing a specific enemy, elapsed turns, etc)
  * Status effects, buffs and debuff

---

## Targets

All target systems use the same game datafiles, but certain elements (graphics, colour codes in game text etc) may be ignored depending on the target limitations.

### Commodore PET

The Commodore PET target is developed using CC65 and is a character-mode only implementation. It runs in the common 40 column screen mode of most PET models and uses PETSCII graphics to represent on-screen UI elements. No bitmap graphics support is possible with this target.

View the Commodore PET documentation for [further details](pet/Readme.md).

### MSX 2

View the MSX documentation for [further details](msx/Readme.md).

### Sinclair QL

The first target implementation and basis for more of the other 16bit versions since it is written in a fairly modern C dialect using a modern cross compiler. The Sinclair QL target features a high-resolution screen mode (512x256) and supports bitmap graphics during gameplay (albeit simple 1bpp dithered images).

View the Sinclair QL documentation for [further details](ql/Readme.md).

---

## Target Platform Comparison

A quick reference to the requirements and features in each *planned* target platform:


| Platform                      | RAM (1)| Storage (2) | Screen Mode (3)| Colour (4)| Text Window (5)| Loadable Font (6)| Image Support (7)|
|-------------------------------|--------|-------------|----------------|-----------|----------------|------------------|------------------|
| Amstrad CPC                   | 64KB   | FDD         | 320x200        | 4         | ?              | ?                | ?                |
| Atari XL/XE                   | 64KB   | FDD         | 320x292        | 2         | ?              | ?                | ?                |
| Atari ST/STe                  | 512KB  | FDD, HDD    | 640x200        | 4         | ?              | ?                | ?                |
| BBC Model B / Master          | 64KB   | FDD         | 320x256        | 2         | ?              | ?                | ?                |
| Commodore Amiga               | 512KB  | FDD, HDD    | 640x200        | 16        | ?              | ?                | ?                |
| [Commodore PET](pet/Readme.md)| 32KB   | FDD         | 40x25          | N/A       | 30x19          | N/A              | N/A              |
| [MSX 2](msx/Readme.md)        | 64KB   | FDD, HDD    | 512x212        | 16        | ?              | ?                | ?                |
| NEC PC-8801                   | 64KB   | FDD         | 640x200        | 8         | ?              | ?                | ?                |
| NEC PC-9801 / 9821            | 1024KB | FDD, HDD    | 640x400        | 16        | ?              | 8x8 1bpp         | ?                |
| PC / VGA                      | 640KB  | FDD, HDD    | 640x400        | 256       | ?              | 8x8 1bpp         | ?                |
| [Sinclair QL](ql/Readme.md)   | 128KB+ | FDD, HDD    | 512x256        | 4         | 48x24          | 8x8 1bpp         | ?                |
| Sinclair ZX Spectrum          | 48KB   | FDD         | 32x24          | 8         | ?              | N/A              | N/A              |
| Sharp X68000                  | 1024KB | FDD, HDD    | 512x512        | 65535     | ?              | 8x8 1bpp         | ?                |




### Key

        1. Minimum memory requirement, in kilobytes. Some platforms (marked with **+**), support enhanced features if additional memory is detected.
        2. Reccomended/supported loading mechanism. Most game datafiles are loaded into memory as needed, so a random access storage device such as FDD or HDD (or modern replacement) is reccomended. Tape devices are *not* supported. Support for ROM images is not planned.
        3. Screen resolution that the game engine runs in.
        4. Colour mode that the game engine runs in.
        5. Size of the main text/story window, in characters. Larger text windows show more on-screen text without having the user page through.
        6. Font type in use. Some platforms load a user-definable bitmap font which can be changed at runtime. Others use the font built in to the video/character ROM.
        7. If this target supports bitmap image display (location screens, monster images during combat) during game play.



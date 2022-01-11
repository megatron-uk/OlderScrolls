# OlderScrolls Engine - MSX

This is the MSX target of the OlderScrolls RPG game engine.

## MSX Details

Unlike most home computers, the MSX was a range of machines from various manufacturers all designed to a standard specification. Hardware and software that is MSX compatible can be run on a MSX computer from any manufacturer.

### Hardware

The MSX has 4 main standard specifications:

  * **MSX 1**
    * CPU
        * Z80 @ 3.58MHz
    * TMS9918 Video chip, 16KB VRAM
        * Up to 256x192, 16 colour
    * Minimum 8KB RAM (most are 32-64KB)
    * Audio
        * AY-3-8910 PSG audio
    Storage
        * 3.5" single density floppy drive (optional)

  * **MSX 2**
    * CPU
        * Z80 @ 3.58MHz
    * TMS9938 Video chip, 64-128KB VRAM
        * All of MSX 1, plus:
        * Up to 512x212, 16 colours from 512 RGB palette
        * 256x212, 256 colour
    * Minimum 64KB RAM (some with 128KB)
    * Audio
        * AY-3-8910 PSG audio
        * YM2413 FM audio (optional)
    * Storage
        * 3.5" single or double density floppy drive (optional, DD present on most)

  * **MSX 2+**
    * CPU
        * Z80 @ 3.58MHz
    * TMS9958 Video chip, 128KB VRAM
        * All of MSX 2, plus:
        * Up to 256x212, 12499 YUV colour
        * Up to 256x212, 19268 YJK colour
    * Minimum 64KB RAM (some with 128KB)
    * Audio
        * AY-3-8910 PSG audio
        * YM2413 FM audio (optional)
        * YM3526 FM audio (optional)
    * Storage
        * 3.5" single or double density floppy drive (optional, DD present on most)

  * **MSX Turbo-R**
    * CPU
        * Z80 @ 3.58MHz
        * R800 @ 7.12MHz
    * TMS9958 Video chip, 128KB VRAM
        * All modes as per MSX 2+
    * Audio
        * AY-3-8910 PSG audio
        * YM2413 FM audio (standard)
        * (YM3526 removed from optional specification)
    * Storage
        * 3.5" double density floppy drive(s) (standard on all models)

The early MSX 1 is pretty close to the average Spectrum 128 or similar home computer, albeit with dedicated video processor and video memory (uncommon on most home computers at the time). However, by the time of the MSX 2 and the MSX 2+, it's fairly clear that the MSX was substantially more impressive in terms of hardware features than almost any other 8-bit home computer.

---

# Implementation Specifics

Software

  * Builds using [SDCC/Fusion-C](https://github.com/ericb59/Fusion-C-v1.2), which is a set of libraries aimed at making MSX development easier on the SDCC toolchain

Hardware

  * Uses the high resolution, 512x212 16 colour mode of the **MSX 2** and above
  * Requires minimum of 64KB RAM
  * Requires minimum of 64KB VRAM
  * *(Optionally)* Can use 128KB VRAM to speed up video output (composites screen elements into an off-screen buffer if 128KB VRAM is detected)
  * 3.5 Floppy interface (or modern SD drive) is *strongly reccomended*

---

# Compiling

Prequisities for building:

  * [SDCC](http://sdcc.sourceforge.net/) C cross-compiler
  * [Fusion-C](https://github.com/ericb59/Fusion-C-v1.2) MSX platform libraries
  * [OpenMSX](https://openmsx.org/) Multiplatform MSX emulator (*optional, for testing only*)
  * [mtools](https://www.gnu.org/software/mtools/) Copying files into DOS format floppies/images from Linux

Change to the *msx2* directory and run **make full**. This will do the following:

  * Compile the OlderScrolls engine for MSX 2 from source
  * Create a new empty MSX DOS format floppy disk image
  * Inject the executeable OlderScrolls binary in the floppy
  * Inject any datafiles into the floppy
  * *(Optionally)* Start the **openmsx** MSX emulator with the floppy

---

# Status

NOT YET STARTED
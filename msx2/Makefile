# Sample makefile for MSX/Z88DK projects

#################################
# Temporarily add the cross
# compiler location to the path
#################################
AS = z80asm
CC = zcc
CC_VARIANT = -compiler=sdcc
SHELL := /bin/bash
PREFIX = /opt/toolchains/z88dk
INCLUDEDIR = ${PREFIX}/include/	
LIBDIR = ${PREFIX}/lib/
ZCCCFG := ${PREFIX}/lib/config
export ZCCCFG
PATH := ${PREFIX}/bin:$(PATH)
export PATH

#################################
# Compiler flags
#################################
EXTRA_INCLUDES = -I./src -I./src/msxgl -I./common

#################################
# What our application is named
#################################
TARGET = game
TARGET_MSX2 = game.bin
TARGET_MSX2_COM = game.com
FLOPPY = game.dsk
FLOPPY_NAME = "GameFloppy"
MSXGL_LIB = lib/msxgl.lib
SOUNDLIB_LIB = lib/sound.lib

################################
# Defines only used for Z88DK
################################
ASFLAGS = -m z80
CFLAGS = -m -subtype=msxdos2 -pragma-define:CRT_DISABLELOADER=0 -O3 --opt-code-size -DTARGET_MSX2 -D__SDCC_VERSION_MAJOR=0 -D__SDCC_VERSION_MINOR=0 -D__SDCC_VERSION_PATCH=0
Z88DKTARGET = msx
LDFLAGS = -subtype=msxdos2 -pragma-define:CRT_DISABLELOADER=0 -SO3 --opt-code-size -DTARGET_MSX2
LDFLAGS_SIMPLE = -subtype=msxdos2 -pragma-define:CRT_DISABLELOADER=1 -SO3 --opt-code-size -DTARGET_MSX2

#################################
# Targets to build/run
#################################
all: msxlib $(TARGET) newdisk copydisk viewdisk emulator_msx2
full: clean libclean all
disk: newdisk
run: newdisk copydisk viewdisk emulator_msx2
emu: emulator_msx2

#######################################
# Assembly sections
#######################################

#src/asm.o: src/asm.s src/asm.h
#	$(AS) $(ASFLAGS) $(ZASFLAGS) -o src/asm.o -c src/asm.s

#######################################
# C sections
#######################################

#### Main bank ####
bin/game.o: src/game.c
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -o bin/game.o -c src/game.c
		
#### Bank 1 ####
bin/bank1.o: common/engine.c
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -o bin/bank1.o -c common/engine.c

#### Bank 2 ####
bin/bank2.o: common/conditions.c
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -o bin/bank2.o -c common/conditions.c
	
#################################
# List of all needed game object files
#################################
OBJFILES = bin/game.o
BANKFILES = #bin/bank1.o #bin/bank2.o

###########################################################
# This builds our game binary from the defined OBJFILES
# and packages it in a format that can be run from MSX DOS
# ... we still need to build the disk image and copy the
# files in though. That's handled by 'newdisk' and 'copydisk'.
###########################################################
$(TARGET):  $(OBJFILES) $(BANKFILES)
	@echo ""
	@echo "========================================"
	@echo " -= Linking $(TARGET) =-"
	@echo ""
	@echo Linking main binary....
	zcc $(CC_VARIANT) +$(Z88DKTARGET) $(LDFLAGS) -l$(MSXGL_LIB) -o bin/$(TARGET_MSX2_COM) -create-app $(OBJFILES) $(BANKFILES)

#################################
# List of all needed msxgl lib object files
#################################
LIBFILES = lib/bios.o lib/bios_mainrom.o lib/bios_var.o lib/clock.o lib/compress.o lib/dos.o lib/draw.o lib/game.o \
lib/game_menu.o lib/game_pawn.o lib/input.o lib/math.o lib/memory.o lib/msx-audio.o \
lib/msxgl_string.o lib/msx-music.o lib/print.o lib/psg.o lib/scc.o lib/scroll.o \
lib/system.o lib/v9990.o lib/vdp.o 
SOUNDLIBFILES = lib/ayfx_player.o lib/lvgm_player.o lib/pt3_player.o lib/vgm_player.o

# Core MSXGL functions

lib/bios.o: src/msxgl/bios.c src/msxgl/bios_diskrom.h  src/msxgl/bios.h  src/msxgl/bios_hook.h  src/msxgl/bios_mainrom.h  src/msxgl/bios_subrom.h  src/msxgl/bios_var.h
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -o lib/bios.o -c src/msxgl/bios.c

lib/bios_mainrom.o: src/msxgl/bios_mainrom.h
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -o lib/bios_mainrom.o -c src/msxgl/bios_mainrom.c

lib/bios_var.o: src/msxgl/bios_var.h
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -o lib/bios_var.o -c src/msxgl/bios_var.c
	
lib/clock.o: src/msxgl/clock.c src/msxgl/clock.h
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -o lib/clock.o -c src/msxgl/clock.c

lib/compress.o: src/msxgl/compress.c src/msxgl/compress.h
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -o lib/compress.o -c src/msxgl/compress.c
	
lib/dos.o: src/msxgl/dos.c src/msxgl/dos.h
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -o lib/dos.o -c src/msxgl/dos.c

lib/draw.o: src/msxgl/draw.c src/msxgl/draw.h
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -o lib/draw.o -c src/msxgl/draw.c

lib/game.o: src/msxgl/game.c src/msxgl/game.h  
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -o lib/game.o -c src/msxgl/game.c
	
lib/game_menu.o: src/msxgl/game_menu.c src/msxgl/game_menu.c
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -o lib/game_menu.o -c src/msxgl/game_menu.c
	
lib/game_pawn.o: src/msxgl/game_pawn.c src/msxgl/game_pawn.h
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -o lib/game_pawn.o -c src/msxgl/game_pawn.c

lib/input.o: src/msxgl/input.c src/msxgl/input.h
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -o lib/input.o -c src/msxgl/input.c
	
lib/math.o: src/msxgl/math.c src/msxgl/math.h
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -o lib/math.o -c src/msxgl/math.c
	
lib/memory.o: src/msxgl/memory.c src/msxgl/memory.h
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -o lib/memory.o -c src/msxgl/memory.c

lib/msx-audio.o: src/msxgl/msx-audio.c src/msxgl/msx-audio.h src/msxgl/msx-audio_reg.h
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -o lib/msx-audio.o -c src/msxgl/msx-audio.c
	
lib/msxgl_string.o: src/msxgl/msxgl_string.c src/msxgl/msxgl_string.h
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -o lib/msxgl_string.o -c src/msxgl/msxgl_string.c
	
lib/msx-music.o: src/msxgl/msx-music.c src/msxgl/msx-music.h src/msxgl/msx-music_reg.h
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -o lib/msx-music.o -c src/msxgl/msx-music.c

lib/print.o: src/msxgl/print.c src/msxgl/print.h src/msxgl/print_inl.h
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -o lib/print.o -c src/msxgl/print.c

lib/psg.o: src/msxgl/psg.c src/msxgl/psg.h
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -o lib/psg.o -c src/msxgl/psg.c

lib/scc.o: src/msxgl/scc.c src/msxgl/scc.h
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -o lib/scc.o -c src/msxgl/scc.c

lib/scroll.o: src/msxgl/scroll.c src/msxgl/scroll.h
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -o lib/scroll.o -c src/msxgl/scroll.c

lib/system.o: src/msxgl/system.c src/msxgl/system.h src/msxgl/system_port.h
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -o lib/system.o -c src/msxgl/system.c

lib/v9990.o: src/msxgl/v9990.c src/msxgl/v9990.h
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -o lib/v9990.o -c src/msxgl/v9990.c
	
lib/vdp.o: src/msxgl/vdp.c src/msxgl/vdp.h
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -o lib/vdp.o -c src/msxgl/vdp.c
	
# Sound library addons
	
lib/ayfx_player.o: src/msxgl/ayfx/ayfx_player.c src/msxgl/ayfx/ayfx_player.h
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -I./src/msxgl/ayfx -o lib/ayfx_player.o -c src/msxgl/ayfx/ayfx_player.c

lib/lvgm_player.o: src/msxgl/lvgm/lvgm_player.c src/msxgl/lvgm/lvgm_player.h
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -I./src/msxgl/lvgm -o lib/lvgm_player.o -c src/msxgl/lvgm/lvgm_player.c
	
lib/pt3_player.o: src/msxgl/pt3/pt3_player.c src/msxgl/pt3/pt3_player.h src/msxgl/pt3/pt3_notetable0.h src/msxgl/pt3/pt3_notetable1.h src/msxgl/pt3/pt3_notetable2.h src/msxgl/pt3/pt3_notetable3.h
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -I./src/msxgl/pt3 -o lib/pt3_player.o -c src/msxgl/pt3/pt3_player.c

lib/vgm_player.o: src/msxgl/vgm/vgm_player.c src/msxgl/vgm/vgm_player.h
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -I./src/msxgl/vgm -o lib/vgm_player.o -c src/msxgl/vgm/vgm_player.c
	
lib/wyz_player2.o: src/msxgl/wyz/wyz_player2.c src/msxgl/wyz/wyz_player2.h
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -I./src/msxgl/wyz -o lib/wyz_player2.o -c src/msxgl/wyz/wyz_player2.c
	
lib/wyz_player.o: src/msxgl/wyz/wyz_player.c src/msxgl/wyz/wyz_player.h
	$(CC) $(CC_VARIANT) +$(Z88DKTARGET) $(CFLAGS) $(ZCFLAGS) $(EXTRA_INCLUDES) -I./src/msxgl/wyz -o lib/wyz_player.o -c src/msxgl/wyz/wyz_player.c
	

###########################################################
# This builds the msxgl library.a from the defined LIBFILES
# and packages it in a format that can be used when linking
# the main game binary.
###########################################################
msxlib: $(LIBFILES) $(SOUNDLIBFILES)
	@echo ""
	@echo "========================================"
	@echo " -= Linking MSXgl =-"
	@echo ""
	@echo Creating MSXgl library....
	z80asm -x$(MSXGL_LIB) $(LIBFILES) $(SOUNDLIBFILES)
	
############################################################
# Helper recipes beyond this point, not really related to
# building the application
############################################################
	
###############################
# Makes a new blank FAT12 floppy
###############################
newdisk:
	@echo ""
	@echo "=========================="
	@echo " Creating a new bootable DOS disk"
	@echo ""
	cp -v etc/msxdos2.dsk bin/$(FLOPPY)
	
###############################
# Copy datafiles onto FAT12 floppy
###############################
copydisk:
	@echo ""
	@echo "=========================="
	@echo " Copying data to DOS disk"
	@echo ""
	# Copy the main target binary
	mcopy -v -n -i bin/${FLOPPY} bin/${TARGET_MSX2_COM} ::/${TARGET_MSX2_COM}
	# Every segment must be copied in as well, using the bank name it was defined as
	#mcopy -v -n -i bin/${FLOPPY} bin/game_BANK_01.bin ::/BANK.01
	
###############################
# View FAT12 floppy contents
###############################
viewdisk:
	@echo ""
	@echo "=========================="
	@echo " Viewing data on DOS disk"
	@echo ""
	mdir -i bin/${FLOPPY}
	
###############################
# Run emulator with the floppy
###############################
emulator: emulator_msx2

emulator_msx2:
	@echo ""
	@echo "=========================="
	@echo " Running OpenMSX (MSX2)"
	@echo ""
	openmsx -script etc/openmsx_msx2.cfg

emulator_msx2p:
	@echo ""
	@echo "=========================="
	@echo " Running OpenMSX (MSX2+)"
	@echo ""
	openmsx -script etc/openmsx_msx2p.cfg

emulator_msxturbo:
	@echo ""
	@echo "=========================="
	@echo " Running OpenMSX (Turbo-R)"
	@echo ""
	openmsx -script etc/openmsx_msxturbo.cfg
	
###############################
# Clean up
###############################
clean:
	@echo ""
	@echo "=========================="
	@echo " Cleaning up"
	@echo ""
	@echo "- Old object files..."
	rm -vf bin/*.o bin/*.i bin/*.asm
	@echo ""
	@echo "=========================="
	
libclean:
	@echo ""
	@echo "=========================="
	@echo " Cleaning up MSXgl"
	@echo ""
	@echo "- Old object files..."
	rm -vf lib/*.o lib/*.i lib/*.asm
	@echo ""
	@echo "=========================="
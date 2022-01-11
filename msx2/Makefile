# Sample makefile for MSX/SDCC/Fusion-C projects

#################################
# Temporarily add the cross
# compiler location to the path
#################################
SHELL := /bin/bash
PREFIX = /opt/toolchains/fusion-c
PATH := ${PREFIX}/bin:$(PATH)
INCLUDEDIR = ${PREFIX}/include/	
LIBDIR = ${PREFIX}/lib/

#################################
# Locations and name of compilers 
# and friends
#################################
CC = sdcc
AS = sdasz80
HEX2BIN = hex2bin

#################################
# Compiler flags
#################################
EXTRA_INCLUDES = -I./src
CRT0 = ${PREFIX}/include/crt0_msxdos.rel
ADDR_CODE = 0x106
ADDR_DATA = 0x0
CFLAGS = -I${INCLUDEDIR} ${EXTRA_INCLUDES} -DTARGET_MSX2 -mz80 
LDFLAGS = -I${INCLUDEDIR} ${EXTRA_INCLUDES} -DTARGET_MSX2 --code-loc $(ADDR_CODE) --data-loc $(ADDR_DATA) --disable-warning 196 -mz80 --no-std-crt0 --opt-code-size fusion.lib -L${LIBDIR} ${CRT0}

#################################
# What our application is named
#################################
TARGET = game
TARGET_MSX2 = game.com
FLOPPY = game.dsk
FLOPPY_NAME = "GameFloppy"

#################################
# Targets to build/run
#################################
all: $(TARGET)
full: clean $(TARGET)
disk: newdisk
run: newdisk copydisk viewdisk emulator
		
###############################
# Makes a new blank FAT12 floppy
###############################
newdisk:
	@echo ""
	@echo "=========================="
	@echo " Creating a new DOS disk"
	@echo ""
	cp -v etc/msxdos.dsk bin/$(FLOPPY)
	
###############################
# Copy datafiles onto FAT12 floppy
###############################
copydisk:
	@echo ""
	@echo "=========================="
	@echo " Copying data to DOS disk"
	@echo ""
	mcopy -v -n -i bin/${FLOPPY} bin/${TARGET_MSX2} ::/
	
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
emulator:
	@echo ""
	@echo "=========================="
	@echo " Running OpenMSX"
	@echo ""
	openmsx -script etc/openmsx.cfg
	
###############################
# Clean up
###############################
clean:
	@echo ""
	@echo "=========================="
	@echo " Cleaning up"
	@echo ""
	@echo "- Old object files..."
	rm -f src/*.o src/*.com src/*.asm src/*.lst src/*.sym src/*.bin src/*.ihx src/*.lk src/*.map src/*.noi src/*.rel src/*.mem src/*.rst
	@echo ""
	@echo "- Previous binary..."
	rm -f bin/$(TARGET) bin/$(TARGET_MSX2) bin/*.lk bin/*.map bin/*.mem bin/*.noi
	@echo ""
	@echo "- Floppy images..."
	rm -f bin/$(FLOPPY)
	@echo "=========================="

#################################
# Main application target build recipe
#################################
OBJFILES = src/test_msx2.rel src/test2_msx2.rel
$(TARGET):  $(OBJFILES)
	@echo ""
	@echo "========================================"
	@echo " -= Linking $(TARGET) =-"
	@echo ""
	@echo Linking ....
	$(CC) $(LDFLAGS) $(OBJFILES) -o bin/$(TARGET) 
	@echo ""
	@echo "========================================"
	@echo " -= Creating $(TARGET_MSX2) =-"
	@echo ""
	@echo Calling hexdump ....
	${HEX2BIN} -e com bin/$(TARGET)
	
################################
# Each C source is compiled to
# a .rel file, to be linked
# as a single binary, above.
################################

src/test_msx2.rel:
	$(CC) $(CFLAGS) -c src/test_msx2.c -o src/test_msx2.rel
	
src/test2_msx2.rel:
	$(CC) $(CFLAGS) -c src/test2_msx2.c -o src/test2_msx2.rel
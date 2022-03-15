# Sample makefile for C68 projects

#################################
# Temporarily add the cross
# compiler location to the path
#################################
SHELL := /bin/bash
PREFIX = /opt/toolchains/qdos
PATH := ${PREFIX}/bin:$(PATH)

#################################
# Locations and name of compilers 
# and friends
#################################
CC = qcc
LD = qld
AS = as68

#################################
# Compiler flags
#################################
ASFLAGS = 
LDFLAGS =
INCLUDES = -I./src
CFLAGS_V = ${INCLUDES} -O -Qobsolete=yes -seperate=yes -stackopt=average -warn=5 -DTARGET_QL
CFLAGS = ${INCLUDES} -O -Qobsolete=yes -seperate=yes -stackopt=average -warn=4 -extension=yes -DTARGET_QL
LIBS = 

#################################
# What our application is named
#################################
TARGET = game
TARGET_QL = game_bin
FLOPPY = game.img
FLOPPY_NAME = "GameFloppy"

#################################
# Targets to build/run
#################################
all: $(TARGET)
full: clean $(TARGET) newdisk copydisk rundisk_192
run: rundisk_192
runslow: rundisk_slow
run128: rundisk_128
run192: rundisk_192
run256: rundisk_256
run640: rundisk_640

#################################
# Object files needed to build our 
# main application
#################################

# Standard game engine code which is the same for all targets

src/conditions.o: common/conditions.c common/conditions.h
	$(CC) $(CFLAGS) -c common/conditions.c -o src/conditions.o

src/engine.o: common/engine.c common/engine.h
	$(CC) $(CFLAGS) -c common/engine.c -o src/engine.o

src/monsters.o: common/monsters.c common/monsters.h
	$(CC) $(CFLAGS) -c common/monsters.c -o src/monsters.o
	
# Platform specific
	
src/bmp_ql.o: src/bmp_ql.c src/bmp_ql.h
	$(CC) $(CFLAGS) -c src/bmp_ql.c -o src/bmp_ql.o

src/input_ql.o: src/input_ql.c src/input_ql.h
	$(CC) $(CFLAGS) -c src/input_ql.c -o src/input_ql.o
	
src/main_ql.o: src/main_ql.c src/main_ql.h
	$(CC) $(CFLAGS) -c src/main_ql.c -o src/main_ql.o
	
src/data_ql.o: src/data_ql.c src/data_ql.h
	$(CC) $(CFLAGS) -c src/data_ql.c -o src/data_ql.o
	
src/draw_ql.o: src/draw_ql.c src/draw_ql.h
	$(CC) $(CFLAGS) -c src/draw_ql.c -o src/draw_ql.o
	
src/ui_ql.o: src/ui_ql.c src/ui_ql.h
	$(CC) $(CFLAGS) -c src/ui_ql.c -o src/ui_ql.o
	
src/utils_ql.o: src/utils_ql.c src/utils_ql.h
	$(CC) $(CFLAGS) -c src/utils_ql.c -o src/utils_ql.o
	
src/game_ql.o: src/game_ql.c src/game_ql.h
	$(CC) $(CFLAGS) -c src/game_ql.c -o src/game_ql.o

src/poll.o: src/poll.s src/poll.h
	$(AS) src/poll.s -o src/poll.o
	
#################################
# Main application target build recipe
#################################
$(TARGET): src/engine.o src/monsters.o src/bmp_ql.o src/input_ql.o src/main_ql.o src/conditions.o src/data_ql.o src/draw_ql.o src/ui_ql.o src/utils_ql.o src/game_ql.o src/poll.o
	@echo ""
	@echo "=========================="
	@echo " Linking binary"
	@echo ""
	@echo "- Calling C68 ld..."
	$(LD) $(LDFLAGS) \
		src/engine.o src/monsters.o \
		src/bmp_ql.o src/input_ql.o src/main_ql.o src/conditions.o \
		src/data_ql.o src/draw_ql.o src/ui_ql.o src/utils_ql.o src/game_ql.o \
		src/poll.o \
	$(LIBS) -o bin/$(TARGET)
	
###############################
# Makes a new blank QL floppy
###############################
newdisk:
	@echo ""
	@echo "=========================="
	@echo " Creating a new QL disk"
	@echo ""
	@echo "- Filling image with zeros..."
	dd if=/dev/zero of=bin/${FLOPPY} bs=1024 count=1440
	@echo ""
	@echo "- Formatting with QL filesystem..."
	qltools bin/${FLOPPY} -fhd ${FLOPPY_NAME}

###############################
# Copy datafiles onto QL floppy
###############################
copydisk:
	@echo ""
	@echo "=========================="
	@echo " Copying data to QL disk"
	@echo ""
	@echo "- Copying data..."
	qltools bin/${FLOPPY} -W assets/*.dat
	qltools bin/${FLOPPY} -W assets/*.idx
	qltools bin/${FLOPPY} -W assets/*.bmp
	@echo ""
	@echo "- Copying binary..."
	qltools bin/${FLOPPY} -W bin/${TARGET}
	@echo ""
	@echo "- Setting binary to execute..."
	qltools bin/${FLOPPY} -x ${TARGET} `xtcc bin/${TARGET}`
	@echo ""
	@echo "- Listing disk contents..."
	qltools bin/${FLOPPY} -d && cp bin/${FLOPPY} /tmp
	
###############################
# Run a QL emulator
###############################
rundisk_slow:
	@echo ""
	@echo "=========================="
	@echo " Running QL emulator"
	@echo ""
	@echo "- Calling emulator..."
	sqlux -f etc/sqlux_128kb_slow.ini -w 2x -b 'exec_w flp1_game'

rundisk_128:
	@echo ""
	@echo "=========================="
	@echo " Running QL emulator"
	@echo ""
	@echo "- Calling emulator..."
	sqlux -f etc/sqlux_128kb.ini -w 2x -b 'exec_w flp1_game'
	
rundisk_192:
	@echo ""
	@echo "=========================="
	@echo " Running QL emulator"
	@echo ""
	@echo "- Calling emulator..."
	sqlux -f etc/sqlux_192kb.ini -w 2x -b 'exec_w flp1_game'

rundisk_256:
	@echo ""
	@echo "=========================="
	@echo " Running QL emulator"
	@echo ""
	@echo "- Calling emulator..."
	sqlux -f etc/sqlux_256kb.ini -w 2x -b 'exec_w flp1_game'

rundisk_640:
	@echo ""
	@echo "=========================="
	@echo " Running QL emulator"
	@echo ""
	@echo "- Calling emulator..."
	sqlux -f etc/sqlux_640kb.ini -w 2x -b 'exec_w flp1_game'
	
###############################
# Clean up
###############################
clean:
	@echo ""
	@echo "=========================="
	@echo " Cleaning up"
	@echo ""
	@echo "- Old object files..."
	rm -f src/*.o
	@echo ""
	@echo "- Previous binary..."
	rm -f bin/$(TARGET)
	@echo ""
	@echo "- Floppy images..."
	rm -f bin/$(FLOPPY)
	@echo "=========================="

################################
# Each C source is compiled to
# an object file, to be linked
# as a single binary, above.
################################
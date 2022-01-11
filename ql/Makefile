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
LDFLAGS = -v
INCLUDES = -I./src
CFLAGS = ${INCLUDES} -O -seperate=yes -peep=all -DTARGET_QL
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
full: clean $(TARGET) newdisk copydisk rundisk_128
run: rundisk_128
run128: rundisk_128
run192: rundisk_192
run256: rundisk_256
run640: rundisk_640

#################################
# Object files needed to build our 
# main application
#################################
OBJFILES = src/bmp_ql.o src/input_ql.o src/main_ql.o src/conditions_ql.o src/data_ql.o src/draw_ql.o src/ui_ql.o src/utils_ql.o src/game_ql.o

#################################
# Main application target build recipe
#################################
$(TARGET):  $(OBJFILES)
	@echo ""
	@echo "=========================="
	@echo " Linking binary"
	@echo ""
	@echo "- Calling C68 ld..."
	$(LD) $(LDFLAGS) $(OBJFILES) $(LIBS) -o bin/$(TARGET)
	
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
	qltools bin/${FLOPPY} -W assets/font8x8.bmp
	qltools bin/${FLOPPY} -W assets/*.dat
	qltools bin/${FLOPPY} -W assets/*.idx
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
DEPS = src/%.h

bmp_ql.o: bmp_ql.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@

input_ql.o: input_ql.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@
	
main_ql.o: main_ql.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@
	
conditions_ql.o: conditions_ql.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@
	
data_ql.o: data_ql.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@
	
draw_ql.o: draw_ql.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@

game_ql.o: game_ql.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@
	
ui_ql.o: ui_ql.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@
	
utils_ql.o: utils_ql.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@

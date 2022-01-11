#################################
# A Makefile for GNU Make to
# compile and assemble a simple
# programme from source for
# the Commodore PET using CC65
#
# J.Snowdon, 2021
#
#################################

#################################
# Names of the compiler and friends
#################################
CC	= cc65
AS	= ca65
LD 	= ld65

#################################
# libraries and paths
# Change this to reflect your
# own installation of cc65
#################################
INCLUDES = --include-dir /usr/share/cc65/include --include-dir ./include
LIBS	 = --lib /usr/share/cc65/lib/pet.lib

#################################
# Compiler flags
#################################
ASM_FLAGS= --target pet
LDFLAGS	= --target pet
CFLAGS 	= --target pet -O -Or -Os --static-locals

#################################
# What our application is named
TARGET_NAME = game
TARGET	= game.prg
DISKNAME = game.d64
#################################

# Targets to build/run
all: $(TARGET)
disk: DISK
copy: COPY
test: $(TARGET) COPY TEST

#################################
# Object files needed to build our 
# main application
#################################
OBJFILES = src/main.o src/screen.o src/strings.o src/ui.o src/utils.o

#################################
# Main application target build recipe
#################################
$(TARGET):  $(OBJFILES)
	@echo ""
	@echo "========================================"
	@echo " -= Building $(TARGET) =-"
	@echo ""
	@echo Linking ....
	$(LD) $(LDFLAGS) --obj $(OBJFILES) $(LIBS) -o bin/$(TARGET)
	@echo ""
	ls -l bin/$(TARGET)

################################
# Build a disk image
################################
DISK:
	@echo ""
	@echo "========================================"
	@echo " -= Building $(DISKNAME) =-"
	@echo ""
	@echo Creating new D64 disk image ....
	mkd64 -o bin/$(DISKNAME) -m cbmdos

################################
# Copy data into disk image
################################
COPY:
	@echo ""
	@echo "========================================"
	@echo " -= Copying to $(DISKNAME) =-"
	@echo ""
	@echo Copying prg to D64 disk image ....
	mkd64 -o bin/$(DISKNAME) \
	-m cbmdos
	c1541 -attach bin/$(DISKNAME) \
	-write bin/$(TARGET) $(TARGET_NAME) \
	-write datafiles/weapons.dat weapons
	
		
################################
# Recipe to 'build' the test target 
# (aka run the binary in the VICE emulator)
################################
TEST:
	@echo ""
	@echo "========================================"
	@echo " -= Running $(TARGET) =-"
	xpet +autostart-warp \
	-autostart "bin/$(DISKNAME):$(TARGET_NAME)"
	
################################
# Main code
# Each C source file has a corresponding
# compiler and assembler entry, just the
# way CC65 works, compared to, e.g. gcc.
# Anything named in OBJFILES should have
# an entry here.
################################
src/main.s: src/main.c
	$(CC) $(CFLAGS) $(INCLUDES) src/main.c

src/main.o: src/main.s
	$(AS) $(ASM_FLAGS) src/main.s

src/screen.s: src/screen.c
	$(CC) $(CFLAGS) $(INCLUDES) src/screen.c

src/screen.o: src/screen.s
	$(AS) $(ASM_FLAGS) src/screen.s

src/strings.s: src/strings.c
	$(CC) $(CFLAGS) $(INCLUDES) src/strings.c

src/strings.o: src/strings.s
	$(AS) $(ASM_FLAGS) src/strings.s
	
src/ui.s: src/ui.c
	$(CC) $(CFLAGS) $(INCLUDES) src/ui.c

src/ui.o: src/ui.s
	$(AS) $(ASM_FLAGS) src/ui.s

src/utils.s: src/utils.c
	$(CC) $(CFLAGS) $(INCLUDES) src/utils.c

src/utils.o: src/utils.s
	$(AS) $(ASM_FLAGS) src/utils.s
	
###############################
# Clean up
###############################
clean:
	@echo ""
	@echo "========================================"
	@echo " -= Cleaning object files for $(TARGET) =-"
	rm -f src/*.o
	rm -f src/*.s
	rm -f bin/$(TARGET)

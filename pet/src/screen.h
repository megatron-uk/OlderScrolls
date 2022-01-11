// ====================================
//
// Screen-related definitions and function
// prototypes for the PET
//
// ====================================

// This is only relevant for PET models
// other similar Commodore machines 
// (VIC-20, C64, etc, have a different address)
#define SCREEN_BASE 0x8000

// Holds a local screen buffer and 
// essential physical screen parameters
struct Screen {
	unsigned char x;
	unsigned char y;
	unsigned short top_left;
	unsigned short top_right;
	unsigned short bottom_left;
	unsigned short bottom_right;
	unsigned short statusbar;
	unsigned short sidebar;
	unsigned short bytes;
	unsigned char error_x;
	unsigned char error_y;
};

extern struct Screen screen;

// Macros for setting character display mode
#define SCREEN_GRAPHIC_MODE \
	__asm__("lda #$8E"); \
	__asm__("jsr $ffd2");
	
#define SCREEN_TEXT_MODE \
	__asm__("lda #$0E"); \
	__asm__("jsr $ffd2");
	
// Macro for clearing hardware screen
#define SCREEN_CLEAR \
	__asm__("lda #$93"); \
	__asm__("jsr $ffd2");
	
// Create the local screen buffer based
// on hardware settings
unsigned char screen_init(void);
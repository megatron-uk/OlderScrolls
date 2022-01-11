// ====================================
//
// UI-related definitions and function
// prototypes for the PET
//
// ====================================

#define	UI_CORNER_UL		0x55
#define	UI_CORNER_UR		0x49
#define	UI_CORNER_LL		0x4A
#define	UI_CORNER_LR		0x4B
#define UI_EDGE_LEFT		0x5D
#define UI_EDGE_RIGHT	0x5D
#define UI_BAR			0x40
#define UI_LEFT_T		0x6B
#define UI_RIGHT_T		0x73
#define UI_UPPER_T		0x71
#define UI_LOWER_T		0x72
#define UI_SIDEBAR_START		8 // Start position, in columns, from the RHS screen edge
#define UI_SIDEBAR_WIDTH		8 // Width, in columns, of the text part of the side menu box
#define UI_STATUSBAR_HEIGHT		6 // Height, in rows, of the status bar box

void ui_draw(void);
void ui_draw_text(unsigned char *text, unsigned char empty);
void ui_draw_statusbar_text(unsigned char *text, unsigned char empty);
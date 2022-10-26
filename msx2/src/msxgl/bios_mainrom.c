#include "msxgl.h"
#include "bios_mainrom.h"

const u16 __at(R_CGTABL) g_CGTABL;
const u8 __at(R_VDP_DR) g_VDP_DR;
const u8 __at(R_VDP_DW) g_VDP_DW;
const u8 __at(R_BASRVN) g_BASRVN[2];
const struct BasicROMVersion __at(R_BASRVN) g_ROMVersion;
const u8 __at(R_MSXVER) g_MSXVER;
const u8 __at(R_MSXMID) g_MSXMID;
const u16 __at(R_CHAR_16) g_CHAR_16[2];
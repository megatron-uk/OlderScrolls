#include <stdio.h>
#include <sys/ioctl.h>

#include "msxgl/msxgl.h"
#include "msxgl/bios.h"

/*const u8 __at(R_MSXVER) g_MSXVER;
const u8 __at(M_MODE) g_MODE;
const struct BasicROMVersion __at(R_BASRVN) g_ROMVersion;
*/

int main(){	
	
	u8 g_VDP;
	u8 vram_size;
	
	printf("Hello MSX world\n");
	//VDP_Initialize();
	g_VDP = VDP_GetVersion();
	vram_size = GET_VRAM_SIZE();
	
	printf("Machine: [%u] ", g_MSXVER);
	switch(g_MSXVER){
		case MSXVER_1: printf("MSX 1 \n"); break;
		case MSXVER_2: printf("MSX 2\n"); break;
		case MSXVER_2P: printf("MSX 2+\n"); break;
		case MSXVER_TR: printf("TurboR\n"); break;
		default: printf("Unknown\n"); break;
	}
	
	//printf("ROMVersion.CharacterSet: [%u]\n", g_ROMVersion.CharacterSet);
	//printf("ROMVersion.DateFormat: [%u]\n", g_ROMVersion.DateFormat);
	printf("VDP: [%u] ", g_VDP);
	switch(g_VDP){
		case 0: printf("TMS9918A"); break;
		case 1: printf("V9938"); break;
		case 2: printf("V9958"); break;
		default: printf("???"); break;
	}
	switch(g_ROMVersion.VSF){
		case 0:	printf(" 60Hz\n"); break;
		case 1: printf(" 50Hz\n"); break;
		default: printf(" ??Hz\n"); break;
	}
	printf("VRAM: [%u] ", vram_size);
	switch(vram_size){
		case 0: printf("16 KB\n"); break;
		case 1: printf("64 KB\n"); break;
		case 2: printf("128 KB\n"); break;
		case 3: printf("192 KB\n"); break;
		default: printf("?? KB\n"); break;
	}
	printf("ROMVersion.VSF: [%u]\n", g_ROMVersion.VSF);
	printf("ROMVersion.KeyboardType: [%u]\n", g_ROMVersion.KeyboardType);
	printf("ROMVersion.SSM: [%u]\n", g_ROMVersion.SSM);
	printf("ROMVersion.SLS: [%u]\n", g_ROMVersion.SLS);
	printf("ROMVersion.SRC: [%u]\n", g_ROMVersion.SRC);
	printf("ROMVersion.Cur: [%u]\n", g_ROMVersion.Cur);
	return(0);
}
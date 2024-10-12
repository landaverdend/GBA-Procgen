#include "loadingScreen.h"
#include <stdlib.h>
#include <string.h>
#include "tonc.h"
#include "charactersTransp.h"
#include "characters.h"


void startLoadScreen(int gt, int bar, int stringID) {
	REG_BG1CNT = BG_PRIO(0) | BG_CBB(0) | BG_SBB(4) | BG_8BPP | BG_REG_32x32;
	memset32(&se_mem[4], 50 | (50 << 16), 350);
	loadString(stringID, 4);

	
	REG_DISPCNT = DCNT_BG1;
	REG_BG1HOFS = 0;	
	REG_BG1VOFS = 0;
}

void loadString(int stringID, int block) {
	int length = 8;
	int x = 15 - length / 2;
	int base = x + (9 * 32);

	const char * str = "WORKING";
	for (int i = 0; i < 7; i++) {
		se_mem[block][base + i] = (0 + str[i] - 65) | SE_PALBANK(0);
	}
}

void progressMade() { }

void progressNode() { }

void setProgress(int x) { }

void endLoadScreen() {
	REG_DISPCNT = 0;
}
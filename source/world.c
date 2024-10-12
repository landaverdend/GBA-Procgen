#include <stdlib.h>
#include "world.h"
#include "graphics.h"
#include "tile.h"

int * world = (int *) EWRAM_ADDR;
u16 * world_native = (u16 *) EWRAM_ADDR;

int worldAt(int wtX, int wtY) {
	return *(world + (wtX + wtY * WORLD_HEIGHT));
}

void setWorldAt(int wtX, int wtY, int WT) {
	*(world + (wtX + wtY * WORLD_HEIGHT)) = WT;
}

void setQF(int wtX, int wtY, int xOff, int yOff, int qf) {
	int shift = (6 * (3 - (xOff + yOff * 2)));
	int mask = ~(QF_MASK << shift);
	qf = qf << shift;
	*(world + (wtX + wtY * WORLD_HEIGHT)) =	
		((*(world + (wtX + wtY * WORLD_HEIGHT))) & mask) | qf;
}

void setGT(int wtX, int wtY, int gt) {
	*(world + (wtX + wtY * WORLD_HEIGHT)) =	
		((*(world + (wtX + wtY * WORLD_HEIGHT))) 
		& ~GT_MASK)
		| (gt << 29);
}

void setSF(int wtX, int wtY, int sf) {
	int wt = worldAt(wtX, wtY);
	wt = (wt & GT_MASK) | (sf << 23) | SQ_MASK;
	setWorldAt(wtX, wtY, wt);
}

void clearSF(int wtX, int wtY) {
	int wt = worldAt(wtX, wtY);
	setWorldAt(wtX, wtY, (wt & GT_MASK));
}



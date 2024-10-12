#ifndef WORLD_H
#define WORLD_H

#include "tonc.h"
#include "tile.h"

#define SQ_MASK 0x10000000
#define QF_MASK 0x0000003F
#define GT_MASK 0xE0000000
#define ALL_QF_MASK 0x0FFFFF

#define BLANK_TILE_MASK 0xE0000000

#define WORLD_LENGTH 128
#define WORLD_HEIGHT 128
#define WORLD_SIZE (WORLD_LENGTH * WORLD_HEIGHT)

#define EWRAM_ADDR 0x02020000
extern int * world;

int worldAt(int wtX, int wtY);
void setWorldAt(int wtX, int wtY, int WT);

void setQF(int wtX, int wtY, int xOff, int yOff, int qf);
void setGT(int wtX, int wtY, int gt);
void setSF(int wtX, int wtY, int sf);
void clearSF(int wtX, int wtY);

INLINE int SQ_SET(int wtX, int wtY) {return worldAt(wtX, wtY) & SQ_MASK;}
INLINE int GET_QF(int wtX, int wtY, int xOff, int yOff) {return (worldAt(wtX,wtY) >> (6 * (3 - (xOff + yOff * 2)))) & QF_MASK;}
INLINE int GET_SF(int wtX, int wtY) {return ((worldAt(wtX, wtY) >> 23) & 0x1F);}
INLINE int READ_GT(int wt) {return (wt >> 29) & 0x7;}
INLINE int WRITE_GT(int wt, int gt) { return (wt & 0x1FFFFFFF) | (gt << 29);}
INLINE int WRITE_QF(int wt, int qf, int xOff, int yOff) 
{
	int shift = (6 * (3 - (xOff + yOff * 2)));
	int mask = ~(QF_MASK << shift);
	qf = qf << shift;
	wt = (wt & mask) | qf;
	return wt;
}

INLINE int CONSTRUCT_SF_WT(int gt, int featureID) {return WRITE_GT(0, gt) | SQ_MASK | ((featureID & 0x1F) << 23);}
INLINE int CONSTRUCT_QF_WT(int gt, int qftl, int qftr, int qfbl, int qfbr) 
{
	int wt = WRITE_GT(0, gt);
	return WRITE_QF(wt, qftl, 0, 0) | WRITE_QF(wt, qftr, 0, 1) | WRITE_QF(wt, qfbl, 1, 0) | WRITE_QF(wt, qfbr, 1, 1);
}
INLINE int QF_PRESENT(int wt) {return wt & ALL_QF_MASK;}

#endif
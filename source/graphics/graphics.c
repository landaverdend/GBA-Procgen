#include "graphics.h"

void display() {
	REG_DISPCNT = DCNT_BG0 | DCNT_BG1 | DCNT_BG3 | DCNT_OBJ | DCNT_OBJ_2D;
}

void noDisplay() {
	REG_DISPCNT = 0;
}

void setPropHide(OBJ_ATTR * obj) {
	obj_set_attr(obj, ATTR0_HIDE, 0, 0);
}

uint se_index_fast(uint tx, uint ty, u16 bgcnt)
{
    uint n= tx + ty*32;
    if(tx >= 32)
        n += 0x03E0;
    if(ty >= 32 && (bgcnt&BG_REG_64x64)==BG_REG_64x64)
        n += 0x0400;
    return n;
}
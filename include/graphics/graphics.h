#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "tonc.h"
#include "world.h"

//Inline helpers

INLINE void SET_SPR(OBJ_ATTR * obj, int si, int pal) {BFN_SET(obj->attr2, si, ATTR2_ID);
																		BFN_SET(obj->attr2, pal, ATTR2_PALBANK);}

void display();
void noDisplay();


#endif
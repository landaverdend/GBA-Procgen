#ifndef MISCHELP_H
#define MISCHELP_H

#include "tonc.h"
#include "fixedptc.h"

INLINE int MASK_BOOL(int i) {return (i << 31) >> 31;}
INLINE int NEG_ZERO (int i) {return i & ~(i >> 31);}
INLINE int HIT_HORZ_TRI() {return bit_tribool(key_hit(-1), KI_RIGHT, KI_LEFT);}
INLINE int HIT_VERT_TRI() {return bit_tribool(key_hit(-1), KI_DOWN, KI_UP);}

//for the fixedptc type
typedef struct Vector2D
{
    fp32 x;
    fp32 y;
} Vector2D;

//for the tonc fixed type.
typedef struct VectorFIXED
{
    FIXED x;
    FIXED y;
} VectorFIXED;

typedef void (*FP)();
typedef int (*iFP)();
typedef int (*iFPi)(int);
typedef int (*iFPii)(int, int);
typedef void (*FPi)(int);
typedef void (*FPii)(int);
typedef void (*FPv)(void *);
typedef bool (*bFPi)(int);

#endif
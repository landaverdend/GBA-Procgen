#ifndef TILE_H
#define TILE_H

#include "tonc.h"

//indices for each type of gt converted to WT format
#define WT_DIRT  0x20000000
#define WT_GRASS 0x40000000
#define WT_WATER 0x16000000
#define WT_SAND  0x60000000
#define WT_STONE 0x80000000
#define WT_TREE  0x10000000
#define WT_LAVA  0xE0000000

//Ground feature IDs
#define GT_DUG 0x0
#define GT_DIRT 0x1
#define GT_GRASS 0x2
#define GT_SAND 0x3
#define GT_STONE 0x4
#define GT_WOOD 0x5

//Quad feature IDs
#define QF_AIR 0x0
#define QF_FLOWER 0x1
#define QF_FLOWER_BLUE 0x2
#define QF_BUSH 0x3
#define QF_BOULDER 0x4
#define QF_TALLGRASS 0x5
#define QF_STONE 0x7

//Single feature IDs
#define SF_TREE 0x0
#define SF_CACTUS 0x1
#define SF_ORE 0x5
#define SF_DESC_STAIRS 0x6
#define SF_ASC_STAIRS 0x8
#define SF_PALM_TREE 0xA
#define SF_WATER 0xC

//Single Feature Metadata ID's
#define IRON_ORE 0x0

#endif
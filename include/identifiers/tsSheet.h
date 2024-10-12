//unified header file that contains tile indices and macros related to titleSheet.h in graphics directory

#ifndef TSSHEET_H
#define TSSHEET_H

#define BLANK_TILE 255 //just a blank (black) tile, change value if this is ever written to

#define MENU_NUM_BASE 26
#define MENU_ASCII_BASE 0
#define MENU_BOX_CORNER 36
#define MENU_BOX_EDGE_V 37
#define MENU_BOX_EDGE_H 38
#define MENU_CURSOR 39
#define CURSOR_TILE_INDEX 0

//Ground Tiles
#define MAP_DUG 41
#define MAP_DIRT 42
#define MAP_GRASS 43
#define MAP_SAND 44
#define MAP_STONE 45
#define MAP_WOOD 46
#define MAP_WATER 47
#define MAP_LAVA 48

//single features tile indices
#define MAP_TREE 64 
#define MAP_CACTUS 65
#define MAP_STAIRS 66
#define MAP_ASC_STAIRS 67
#define MAP_PALM_TREE 68

//quad features
#define QUAD_BASE_INDEX 96

#endif
#include <string.h>
#include "tile.h"
#include "worldGeneration.h"
#include "map.h"
#include "world.h"
#include <tonc.h>
#include "titleSheet.h"
#include "tsSheet.h"
#include "quadMap.h"

#define BG_MAP 16
#define BG_QF_MAP 24

BG_AFFINE bgaff;

const int DX = 256;

//these match up to qf id's. change this table to render a qf on
const int qfColorLookup[] = 
{
    0, 0, 0, QF_GREEN, 0, QF_GREEN, 0, QF_GRAY, QF_BROWN,
    0, QF_BROWN
};

const int gtTileLookup[] =  { MAP_DUG, MAP_DIRT, MAP_GRASS, MAP_SAND, MAP_STONE, MAP_WOOD, MAP_WATER, MAP_LAVA }; //just the old mapping of water and lava 

const int sfTileLookup[] = 
{ 
    MAP_TREE, MAP_CACTUS, 0, 0, 0, 0, MAP_STAIRS, 0, MAP_ASC_STAIRS,
    0, MAP_PALM_TREE, 0, MAP_WATER
};

void initMapMaster()
{
    initMapGFX();
    initAffine();
    loadGTMap();
    initMapDisplay(); //this needs to come last otherwise get weird visual errors.
}

void initMapDisplay()
{
	REG_DISPCNT = DCNT_MODE2 | DCNT_BG2 | DCNT_BG3;
	REG_BG2CNT = BG_PRIO(1) | BG_CBB(0) | BG_SBB(BG_MAP) | BG_AFF_128x128 | BG_8BPP; //GT layer
    REG_BG3CNT = BG_PRIO(0) | BG_CBB(1) | BG_SBB(BG_QF_MAP) | BG_AFF_128x128 | BG_8BPP; //QF layer?
}

// load tiles, uses same sheet as menu screen.
void initMapGFX()
{
    memcpy(&tile_mem[0][0], titleSheetTiles, titleSheetTilesLen);
    memcpy(&tile_mem[1][0], quadMapTiles, quadMapTilesLen);
    memcpy(pal_bg_mem, titleSheetPal, titleSheetPalLen);
}

void initAffine()
{    
    bgaff = bg_aff_default;
    REG_BG_AFFINE[2] = bgaff;
    REG_BG_AFFINE[3] = bgaff;
    REG_BG2X = 0;
	REG_BG2Y = 0;
}

int getQFTile(int x, int y)
{
    int wt = worldAt(x, y);

    if (QF_PRESENT(wt)) // if features are set.
    {
        // BL | BR | TR | TL
        int qfTL = qfColorLookup[GET_QF(x, y, 0, 0)];
        int qfTR = qfColorLookup[GET_QF(x, y, 1, 0)] * 4;
        int qfBL = qfColorLookup[GET_QF(x, y, 0, 1)] * 16;
        int qfBR = qfColorLookup[GET_QF(x, y, 1, 1)] * 64; //convert to base 4 number. 

        int ind = qfTL + qfTR + qfBL + qfBR;

        return ind;
    }

    return 0; //should map to all blank
}

int constructTileWord(int t1, int t2, int t3, int t4)
{
	return (t4 << 24) | (t3 << 16) | (t2 << 8) | (t1);
}

int getMapTile(int x, int y)
{
    if (SQ_SET(x, y)) // presumably SF should map to the same ground tile every time. 
    {
        int sf = GET_SF(x, y);
        return sfTileLookup[sf];
    }
    else 
    {
        int gt = READ_GT(worldAt(x, y));
        return gtTileLookup[gt];
    }
}

void loadGTMap()
{
	u32 *gtMap = (u32 *) se_mem[BG_MAP];
    u32 *qfMap = (u32 *) se_mem[BG_QF_MAP];

	int rowLength = MAP_DIM / 4;

	for (int y = 0; y < MAP_DIM - 3; y++)
	{
		for (int x = 0, ind = 0; ind < MAP_DIM; x += 4, ind++)
		{
			int oneD = (y * rowLength) + ind;
            int gt1 = getMapTile(x, y);
            int gt2 = getMapTile(x + 1, y);
            int gt3 = getMapTile(x + 2, y);
            int gt4 = getMapTile(x + 3, y);

            int qf1 = getQFTile(x, y);
            int qf2 = getQFTile(x + 1, y);
            int qf3 = getQFTile(x + 2, y);
            int qf4 = getQFTile(x + 3, y);

			gtMap[oneD] = constructTileWord(gt1, gt2, gt3, gt4);
            qfMap[oneD] = constructTileWord(qf1, qf2, qf3, qf4);
		}
	}
}


void mapLoop()
{
    initMapMaster();

    FIXED ss = float2fx(0.150f); //toncs fixed type is 24.8. dont use fp32 for tonc library functions.

    AFF_SRC_EX asx = {
	    32 << 8, 64 << 8, // Map coords.
	    50, 12,			  // Screen coords.
        0x0100, 0x0100, 0 // Scales and angle.
    };

    while(1)
    {
        VBlankIntrWait();
        key_poll();

        asx.tex_x -= DX * key_tri_horz() * 5;
		asx.tex_y -= DX * key_tri_vert() * 5;

		if (key_is_down(KEY_B))
		{
			ss = ss - 3 > MIN_ZOOM ? ss - 3 : MIN_ZOOM;
		}
		if (key_is_down(KEY_A))
		{
			ss = ss + 3 < MAX_ZOOM ? ss + 3 : MAX_ZOOM;
		}
		if (key_is_down(KEY_START))
		{
			return;
		}

		asx.sx = asx.sy = (1 << 16) / ss; 
        
        bg_rotscale_ex(&bgaff, &asx);
	    REG_BG_AFFINE[2] = bgaff;
        REG_BG_AFFINE[3] = bgaff;
    }
}

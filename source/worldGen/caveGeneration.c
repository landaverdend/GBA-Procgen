#include "caveGeneration.h"
#include <stdlib.h>
#include "loadingScreen.h"
#include "caveConfig.h"
#include "world.h"
#include "tile.h"
//#include "graphics.h"

int currentLevel = 2;

//placement of QUAD STONE. each WT maps to 4 generated noise values.
void placeCaveStone(int x, int y)
{
    int qfx = x * 2;
    int qfy = y * 2;

    fp32 n1 = generateNoise(qfx,     qfy,     *(CAVE_LEVELS[currentLevel].np) );
    fp32 n2 = generateNoise(qfx,     qfy + 1, *(CAVE_LEVELS[currentLevel].np) );
    fp32 n3 = generateNoise(qfx + 1, qfy,     *(CAVE_LEVELS[currentLevel].np) );
    fp32 n4 = generateNoise(qfx + 1, qfy + 1, *(CAVE_LEVELS[currentLevel].np) );
    
    //setWorldAt(x, y, WT_STONE);
    setWorldAt(x, y,  CONSTRUCT_QF_WT(GT_STONE,
        ((n1 <= CAVE_AIR) ? QF_STONE : 0),
        ((n2 <= CAVE_AIR) ? QF_STONE : 0),
        ((n3 <= CAVE_AIR) ? QF_STONE : 0),
        ((n4 <= CAVE_AIR) ? QF_STONE : 0)));
}

// x and y represent top left of the ore vein. Using qran because noise() is way too expensive for every ore type.
// maybe change to spawn in circle shape. Just wanna get it down for now. 
void placeVein(int x, int y, int width, int height, int id)
{
    int area = width * height;
    int miss = area / 2; 

    int yyend = y + height < MAP_DIM ? y + height : MAP_DIM;
    int xxend = x + width < MAP_DIM ? x + width : MAP_DIM;

    for (int yy = y; yy < yyend; yy++)
    {
        for (int xx = x; xx < xxend; xx++)
        {
            if ((worldAt(xx, yy) == WT_WATER) || (worldAt(xx, yy) == WT_LAVA)) break;
            int rng = qran_range(1, area);
            if (rng >= miss) setWorldAt(xx, yy, CONSTRUCT_SF_WT(GT_STONE, SF_ORE | id));
        }
    }
}

bool placeCaveOre()
{
    const Ore *ores = CAVE_LEVELS[currentLevel].oreTable;
    int numOres = CAVE_LEVELS[currentLevel].numOres;

    for (int y = 0; y < MAP_DIM; y++)
    {
        for (int x = 0; x < MAP_DIM; x++)
        {
            for (int o = 0; o < numOres; o++)
            {
                int rng = qran_range(1, 1000);
                if (ores[o].rarity >= rng) 
                {
                    int w = qran_range(3, 5);
                    int h = qran_range(3, 5);
                    placeVein(x, y, w, h, ores[o].id);
                    x += w;
                }
            }
        }
        progressMade();
    }

    return true;
}


bool placeCaveLiquid(int x, int y, int liquidType)
{
    fp32 lnoise = generateNoise(x, y,  CAVE_PROPS[0]);

    int wt = WT_STONE;

    if (lnoise < CAVE_LIQUID)
    {
        wt = liquidType;
    }
    else if (lnoise < DIRT_COAST)
    {
        wt = WT_DIRT;
    }

    bool placed = wt == WT_LAVA || wt == WT_WATER || wt == WT_DIRT;

    if (placed) setWorldAt(x, y, wt);
    return placed;
}


void setCaveTile(int x, int y)
{
    if (CAVE_LEVELS[currentLevel].hasLiquid) 
    {
        if (!placeCaveLiquid(x, y, CAVE_LEVELS[currentLevel].liquidType)) placeCaveStone(x, y);
    }
    else
    {
        placeCaveStone(x, y);
    }
}

void generateCave()
{
    initGlobalCaveProps();
    startLoadScreen(GT_STONE, true, 3);

    for (int y = 0; y < MAP_DIM; y++)
    {
        for (int x = 0; x < MAP_DIM; x++)
        {
            setCaveTile(x, y);
        }
        progressMade();
    }

    placeCaveOre();
    currentLevel = currentLevel == 2 ? 2 : currentLevel + 1; //eventually have more levels. right now ends at 2.

    placeStairs(qran_range(4, 6), SF_DESC_STAIRS);
    placeStairs(qran_range(3, 4), SF_ASC_STAIRS);

    endLoadScreen();
}
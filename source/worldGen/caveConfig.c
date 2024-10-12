#include "caveConfig.h"
#include "worldGeneration.h"
#include "tile.h"
#include <tonc.h>

/**
 * Config file for setting up cave properties
 * 
 */ 


NoiseProperties CAVE_PROPS[TOTAL_PROPS];
CaveLevel CAVE_LEVELS[TOTAL_LEVELS];


const Ore oreTable1[]  =
{
    { .id = IRON_ORE, .rarity = 2}
    
};

void initGlobalCaveProps()
{
    sqran(qran());
    const NoiseProperties liquidProps = 
    {
        .octaves = LIQUID_OCTAVES,
        .scale = LIQUID_SCALE,
        .lacunarity = LIQUID_LACUNARITY,
        .persistence = LIQUID_PERSISTENCE,
        .octaveOffsets = getVectorOffsets(LIQUID_OCTAVES)
    };

    const NoiseProperties setOneProperties =
    {
        .octaves = SET_ONE_OCTAVES,
        .scale = SET_ONE_SCALE,
        .lacunarity = SET_ONE_LACUNARITY,
        .persistence = SET_ONE_PERSISTENCE,
        .octaveOffsets = getVectorOffsets(SET_ONE_OCTAVES)
    };

    const NoiseProperties setTwoProperties =
    {
        .octaves = SET_TWO_OCTAVES,
        .scale = SET_TWO_SCALE,
        .lacunarity = SET_TWO_LACUNARITY,
        .persistence = SET_TWO_PERSISTENCE,
        .octaveOffsets = getVectorOffsets(SET_ONE_OCTAVES)
    };

    const NoiseProperties setThreeProperties =
    {
        .octaves = SET_TWO_OCTAVES,
        .scale = SET_TWO_SCALE,
        .lacunarity = SET_TWO_LACUNARITY,
        .persistence = SET_TWO_PERSISTENCE,
        .octaveOffsets = getVectorOffsets(SET_ONE_OCTAVES)
    };

    CAVE_PROPS[0] = liquidProps;
    CAVE_PROPS[1] = setOneProperties;
    CAVE_PROPS[2] = setTwoProperties;
    CAVE_PROPS[3] = setThreeProperties;

    initCaveLevels();
}


void initCaveLevels()
{
    const CaveLevel lvl_0 = 
    {
        .np = &CAVE_PROPS[1],
        .hasLiquid = false,
        .oreTable = oreTable1,
        .numOres = 1,
    };

    const CaveLevel lvl_1 = 
    {
        .np = &CAVE_PROPS[2],
        .hasLiquid = true,
        .liquidType = WT_WATER,
        .oreTable = oreTable1,
        .numOres = 1
    };

    const CaveLevel lvl_2 = 
    {
        .np = &CAVE_PROPS[2],
        .hasLiquid = true,
        .liquidType = WT_LAVA,
        .oreTable = oreTable1,
        .numOres = 1
    };



    CAVE_LEVELS[0] = lvl_0;
    CAVE_LEVELS[1] = lvl_1;
    CAVE_LEVELS[2] = lvl_2;

}



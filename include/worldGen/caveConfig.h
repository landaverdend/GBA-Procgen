#ifndef CAVECONFIG_H
#define CAVECONFIG_H
#include "fixedptc.h"
#include "worldGeneration.h"

#define TOTAL_PROPS 4
#define TOTAL_LEVELS 3

typedef struct Ore 
{
    int id;
    int rarity;
} Ore;

typedef struct CaveLevel {
    NoiseProperties *np;
    bool hasLiquid;
    int liquidType;
    const Ore *oreTable;
    int numOres;     // length of ore table
} CaveLevel;


extern NoiseProperties CAVE_PROPS[TOTAL_PROPS];
extern CaveLevel CAVE_LEVELS[TOTAL_LEVELS];

void initGlobalCaveProps();
void initCaveLevels();


//Cave cutoff Points...
#define CAVE_AIR fixedpt_rconst(0.55f)
#define CAVE_DIRT fixedpt_rconst(0.9f)  

#define CAVE_LIQUID fixedpt_rconst(0.3f)
#define DIRT_COAST fixedpt_rconst(0.35f)


//liquid will have its own noise properties.
#define LIQUID_SCALE fixedpt_rconst(0.05f)
#define LIQUID_OCTAVES 1
#define LIQUID_LACUNARITY fixedpt_rconst(1.0f)
#define LIQUID_PERSISTENCE fixedpt_rconst(1.0f)

// begin sets....
#define SET_ONE 1 //index in global arr
#define SET_ONE_SCALE fixedpt_rconst(0.03f)
#define SET_ONE_OCTAVES 2
#define SET_ONE_LACUNARITY fixedpt_rconst(3.0f)
#define SET_ONE_PERSISTENCE fixedpt_rconst(1.0f)

#define SET_TWO 2
#define SET_TWO_SCALE fixedpt_rconst(0.04f)
#define SET_TWO_OCTAVES 2
#define SET_TWO_LACUNARITY fixedpt_rconst(3.0f)
#define SET_TWO_PERSISTENCE fixedpt_rconst(1.0f)

#define SET_THREE 3
#define SET_THREE_SCALE fixedpt_rconst(0.15f)
#define SET_THREE_OCTAVES 4
#define SET_THREE_LACUNARITY fixedpt_rconst(4.0f)
#define SET_THREE_PERSISTENCE fixedpt_rconst(1.5f)


#endif
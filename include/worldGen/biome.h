#ifndef BIOME_H
#define BIOME_H
#include "tonc.h"
#include "fixedptc.h"
#include "world.h"
#include "tile.h"

#define SEA_LEVEL fixedpt_rconst(0.225f)
#define COAST_LEVEL fixedpt_rconst(0.275f)
#define LOWLANDS fixedpt_rconst(0.4)
#define MIDLANDS fixedpt_rconst(0.6)
#define HIGHLANDS fixedpt_rconst(1.0)


//temperature just has the same cutoffs as rain but im doing this for semantics IG
#define LOW_TEMP fixedpt_rconst(0.3f)
#define LOW_TEMP_IND 0
#define MED_TEMP fixedpt_rconst(0.4f)
#define MED_TEMP_IND 1
#define HIGH_TEMP fixedpt_rconst(1.0f)
#define HIGH_TEMP_IND 2


//Rainfall level for biomes and associated lookup table index
#define LOW_RAINFALL fixedpt_rconst(0.3f)
#define LOW_RAINFALL_IND 0
#define MED_RAINFALL fixedpt_rconst(0.4f)
#define MED_RAINFALL_IND 1
#define HIGH_RAINFALL fixedpt_rconst(0.65f)
#define HIGH_RAINFALL_IND 2
#define VHIGH_RAINFALL fixedpt_rconst(1.0f)
#define VHIGH_RAINFALL_IND 3




int getTemperature(fp32 temp);
int getRainfall(fp32 RAINFALL);
int getElevation(fp32 elevation);

int getCoastalTile(int x, int y);
int getDesertTile(int x, int y);
int getGrasslandTile(int x, int y);
int getFlowerTile(int x, int y);
int getForest_low(int x, int y);
int getForest_med(int x, int y);
int getForest_high(int x, int y);
int getLake(int x, int y);



int getVegetation(int rng, int max);
int getQFVegetation(int qfid, int chance, int max);
int getLowlandsTile(fp32 RAINFALL);
int getMidlandsTile(fp32 RAINFALL);
int getHighlandsTile(fp32 RAINFALL);
int getTerrainType(int x, int y, fp32 elevationValue, fp32 rainValue, fp32 tempValue);

int GET_QF_FLOWER(int chance, int max);

//wrapper for get QF veg
INLINE int GET_QF_VEG(int qfid, int chance, int max)
{
    return CONSTRUCT_QF_WT(GT_GRASS, 
                    getQFVegetation(qfid, chance, max),
                    getQFVegetation(qfid, chance, max),
                    getQFVegetation(qfid, chance, max),
                    getQFVegetation(qfid, chance, max));
}



#endif
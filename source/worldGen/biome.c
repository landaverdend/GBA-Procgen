#include "biome.h"
#include "tile.h"
#include "miscHelp.h"
#include "overworldGeneration.h"

const int FLWR_LOOKUP[2] = {QF_FLOWER, QF_FLOWER_BLUE};


//this lookup table should be TEMP levels high and RAINFALL levels wide.
const iFPii BIOME_LOOKUP[3][4] =
{
    {getGrasslandTile, getForest_low,    getForest_med,  getForest_med},
    {getDesertTile,    getFlowerTile,    getForest_med,  getForest_high},
    {getDesertTile,    getGrasslandTile, getForest_low,  getForest_high}
};


int getTemperature(fp32 temp)
{
    if (temp <= LOW_TEMP)  return LOW_TEMP_IND;
    if (temp <= MED_TEMP)  return MED_TEMP_IND;
    if (temp <= HIGH_TEMP) return HIGH_TEMP_IND;
    return 0;
}

int getRainfall(fp32 rain)
{
    if (rain <= LOW_RAINFALL)  return LOW_RAINFALL_IND;
    if (rain <= MED_RAINFALL)  return MED_RAINFALL_IND;
    if (rain <= HIGH_RAINFALL) return HIGH_RAINFALL_IND;
    if (rain <= VHIGH_RAINFALL)  return VHIGH_RAINFALL_IND;
    return 0;
}

int getElevation(fp32 elevation)
{
    if (elevation <= SEA_LEVEL)   return SEA_LEVEL;
    if (elevation <= COAST_LEVEL) return COAST_LEVEL;
    return 0;
}

int getQFVegetation(int qfid, int chance, int max)
{
    int rng = qran_range(0, max + 1);
    if (rng <= chance)
    {
        return qfid;
    }
    return 0;
}

// like above except for random flower variation.
int GET_QF_FLOWER(int chance, int max)
{
    return CONSTRUCT_QF_WT(
                GT_GRASS,
                getQFVegetation(FLWR_LOOKUP[qran_range(0, 2)], chance, max),
                getQFVegetation(FLWR_LOOKUP[qran_range(0, 2)], chance, max),
                getQFVegetation(FLWR_LOOKUP[qran_range(0, 2)], chance, max),
                getQFVegetation(FLWR_LOOKUP[qran_range(0, 2)], chance, max) );
}

int getCoastalTile(int x, int y)
{
    int palmrng = qran_range(0, 100);
    return (palmrng <= 3 ? CONSTRUCT_SF_WT(GT_SAND, SF_PALM_TREE) : WT_SAND);
}

int getDesertTile(int x, int y)
{
    int ret = WT_SAND;
    int cactusrng = qran_range(0, 100);
    if (cactusrng <= 3) ret = CONSTRUCT_SF_WT(GT_SAND, SF_CACTUS);
    return ret;
}

//update this to return QF Tall Grass.
int getGrasslandTile(int x, int y)
{
    int ret = WT_GRASS;
    int qfx = x * 2;
    int qfy = y * 2;
    
    fp32 n1 = generateNoise(qfx, qfy, overworld.noiseProps[QGRASS_PROPERTIES]);
    if (n1 > (QGRASS + fixedpt_rconst(0.15f))) return ret; //not even worth calculating. approximate values wont be below threshold
    fp32 n2 = generateNoise(qfx, qfy + 1, overworld.noiseProps[QGRASS_PROPERTIES]);
    fp32 n3 = generateNoise(qfx + 1, qfy, overworld.noiseProps[QGRASS_PROPERTIES]);
    fp32 n4 = generateNoise(qfx + 1, qfy + 1, overworld.noiseProps[QGRASS_PROPERTIES]);

    ret = CONSTRUCT_QF_WT(GT_GRASS,
        ((n1 <= QGRASS) ? QF_TALLGRASS : 0),
        ((n2 <= QGRASS) ? QF_TALLGRASS : 0),
        ((n3 <= QGRASS) ? QF_TALLGRASS : 0),
        ((n4 <= QGRASS) ? QF_TALLGRASS : 0));

    if (!QF_PRESENT(ret)) //no tall grass at noise loc.
    {
        int rng = qran_range(0 , 100);
        if (rng < 1) ret = CONSTRUCT_SF_WT(GT_GRASS, SF_TREE);
        else if (rng < 15) ret = GET_QF_FLOWER(5, 10);
        else if (rng < 30) ret = GET_QF_VEG(QF_BUSH, 6, 10);
    }

    return ret;
}

int getFlowerTile(int x, int y)
{
    int flwrng = qran_range(0, 50);
    int ret = WT_GRASS;

    if (flwrng == 1) 
        ret = CONSTRUCT_SF_WT(GT_GRASS, SF_TREE);
    else if (flwrng <= 35)
    {
        ret = GET_QF_FLOWER(7, 10);
    }
    else if (flwrng <= 40)
    {
        ret = GET_QF_VEG(QF_BUSH, 6, 10);
    }
    return ret;
}

int getForest_low(int x, int y)
{
    int vegrng = qran_range(0, 100);
    int ret = WT_GRASS;
    
    if (vegrng <= 2)
        ret = GET_QF_FLOWER(5, 10);
    else if (vegrng <= 4)
        ret = GET_QF_VEG(QF_BUSH, 6, 10);
    else if (vegrng <= 10)
        ret = CONSTRUCT_SF_WT(GT_GRASS, SF_TREE);
    else if (vegrng <= 15)
        ret = GET_QF_VEG(QF_FLOWER, 4, 10);


    return ret;
}

int getForest_med(int x, int y)
{
    int vegrng = qran_range(0, 100);
    int ret = WT_GRASS;
    
    if (vegrng <= 2)
        ret = GET_QF_FLOWER(4, 10);
    else if (vegrng <= 4) 
        ret = GET_QF_VEG(QF_BUSH, 6, 10);
    else if (vegrng <= 25) //tree
        ret = CONSTRUCT_SF_WT(GT_GRASS, SF_TREE);

    return ret;
}

int getForest_high(int x, int y)
{
    int vegrng = qran_range(0, 100);
    int ret = WT_GRASS;
    
    if (vegrng <= 3)
        ret = GET_QF_FLOWER(6, 10);
    else if (vegrng <= 6) //bush
        ret = GET_QF_VEG(QF_BUSH, 4, 10);
    else if (vegrng <= 50) //tree
        ret = CONSTRUCT_SF_WT(GT_GRASS, SF_TREE);

    return ret;
}

int getLake(int x, int y)
{
    return WT_WATER;
}


//Given temperature and RAINFALL, determine the "biome" in terms of the game.
int getTerrainType(int x, int y, fp32 elevation, fp32 rainfall, fp32 temp)
{
    // normalize all three values.
    int rainLevel = getRainfall(rainfall);
    int tempLevel = getTemperature(temp);
    int elevationLevel = getElevation(elevation);

    int ret = 0;

    switch (elevationLevel)
    {
        case SEA_LEVEL:
            ret = WT_WATER;
            break;
        case COAST_LEVEL:
            //ugly edge case. rethink lakes in the future.
            ret = getCoastalTile(x, y);
            break;
        default:
            ret = BIOME_LOOKUP[tempLevel][rainLevel](x, y);
            break;
    }
    
    return ret;
}

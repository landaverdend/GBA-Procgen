#include "overworldGeneration.h"
#include "worldGeneration.h"
#include "loadingScreen.h"
#include "tile.h"
#include <stdlib.h>
#include "world.h"
#include "biome.h"

Overworld overworld;

//directions are clock wise: T TR R BR D DL L TL 
const int dirs[8][2] = { {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1} };

void initOverworld()
{
    overworld.islandShape = MAP_SHAPE;

    // srand(WORLD_SEED); //stdlib
    sqran(WORLD_SEED); 
    overworld.seed = WORLD_SEED;
    overworld.noiseProps = malloc(NOISE_PROPERTIES_SIZE * sizeof(NoiseProperties));
    overworld.numStairs = qran_range(5, 7);
    
    const NoiseProperties heightprops = {
        .octaves = HEIGHT_OCTAVES,
        .scale = HEIGHT_SCALE,
        .lacunarity = HEIGHT_LACUNARITY,
        .persistence = HEIGHT_PERSISTENCE,
        .octaveOffsets = getVectorOffsets(HEIGHT_OCTAVES) //this should match octaves
    };

    const NoiseProperties rainfallProps = 
    {
        .octaves = RAINFALL_OCTAVES,
        .scale = RAINFALL_SCALE,
        .lacunarity = RAINFALL_LACUNARITY,
        .persistence = RAINFALL_PERSISTENCE,
        .octaveOffsets = getVectorOffsets(RAINFALL_OCTAVES)
    };

    sqran(WORLD_SEED + 1); //want different vector offsets for temp
    const NoiseProperties temperatureProps = 
    {
        .octaves = RAINFALL_OCTAVES,
        .scale = RAINFALL_SCALE,
        .lacunarity = RAINFALL_LACUNARITY,
        .persistence = RAINFALL_PERSISTENCE,
        .octaveOffsets = getVectorOffsets(RAINFALL_OCTAVES)
    };
    sqran(WORLD_SEED); //set it back so that output is always the same.
    

    const NoiseProperties qstoneprops = 
    {
        .octaves = QSTONE_OCTAVES,
        .scale = QSTONE_SCALE,
        .lacunarity = QSTONE_LACUNARITY,
        .persistence = QSTONE_PERSISTENCE,
        .octaveOffsets = getVectorOffsets(QSTONE_OCTAVES)
    };
    
    const NoiseProperties qgrassprops = 
    {
        .octaves = QGRASS_OCTAVES,
        .scale = QGRASS_SCALE,
        .lacunarity = QGRASS_LACUNARITY,
        .persistence = QGRASS_PERSISTENCE,
        .octaveOffsets = getVectorOffsets(QGRASS_OCTAVES)
    };

    overworld.noiseProps[HEIGHT_PROPERTIES] = heightprops;
    overworld.noiseProps[RAINFALL_PROPERTIES] = rainfallProps;
    overworld.noiseProps[TEMPERATURE_PROPERTIES] = temperatureProps;
    overworld.noiseProps[QSTONE_PROPERTIES] = qstoneprops;
    overworld.noiseProps[QGRASS_PROPERTIES] = qgrassprops;
}

fp32 getGradientValue(int x, int y, int islandShape, int mapWidth, int mapHeight)
{
    x = fixedpt_fromint(x);
    y = fixedpt_fromint(y);

    // 64 below represents the following equation: (mapWidth + mapHeight) / 4
    // idea is to get the entire map size, divide it by 2, then do it again.
    fp32 halfisland = fixedpt_fromint(64); 

    fp32 distancex = fixedpt_abs(x - halfisland);
    fp32 distancey = fixedpt_abs(y - halfisland);
    fp32 distance;

    switch (islandShape)
    {
    default:
    case MAP_SQUARE:
        distance = (distancex > distancey) ? distancex : distancey;
        break;
    case MAP_CIRCLE:
        distance = fixedpt_sqrt(fixedpt_mul(distancex, distancex) + fixedpt_mul(distancey, distancey));
        break;
    }

    fp32 maxwidth = halfisland; //radius of the land
    fp32 delta = fixedpt_div(distance, maxwidth);
    fp32 gradient = fixedpt_mul(delta, delta);
    return ((FIXEDPT_ONE - gradient) > 0) ? FIXEDPT_ONE - gradient : 0;
}

//wrapper for getting gradient adjusted value so the world looks like an island.
fp32 getAdjustedNoise(int x, int y)
{
    return fixedpt_mul(generateNoise(x, y, overworld.noiseProps[HEIGHT_PROPERTIES]), getGradientValue(x, y, overworld.islandShape, MAP_DIM, MAP_DIM)); 
}

//given the GT, determine whether or not we can place a QF on this tile.
bool validGT(int gt)
{
    return gt != WT_WATER;
}

void setWorldTile(int x, int y)
{
    fp32 heightValue = getAdjustedNoise(x, y);
    fp32 rainValue = generateNoise(x, y, overworld.noiseProps[RAINFALL_PROPERTIES]);
    fp32 tempValue = generateNoise(x, y, overworld.noiseProps[TEMPERATURE_PROPERTIES]);
    
    int wt = getTerrainType(x, y, heightValue, rainValue, tempValue);
    setWorldAt(x, y, wt);

    if (heightValue > LOWLANDS && wt != WT_WATER) placeQuadStone(x, y);
}

void placeQuadStone(int x, int y)
{
    int qfx = x * 2;
    int qfy = y * 2;
    
    fp32 n1 = generateNoise(qfx, qfy, overworld.noiseProps[QSTONE_PROPERTIES]);
    if (n1 > (QSTONE + fixedpt_rconst(0.15f))) return; //not even worth calculating. approximate values wont be below threshold 
    fp32 n2 = generateNoise(qfx, qfy + 1, overworld.noiseProps[QSTONE_PROPERTIES]);
    fp32 n3 = generateNoise(qfx + 1, qfy, overworld.noiseProps[QSTONE_PROPERTIES]);
    fp32 n4 = generateNoise(qfx + 1, qfy + 1, overworld.noiseProps[QSTONE_PROPERTIES]);

    int wt = CONSTRUCT_QF_WT(READ_GT(worldAt(x, y)),
        ((n1 <= QSTONE) ? QF_STONE : 0),
        ((n2 <= QSTONE) ? QF_STONE : 0),
        ((n3 <= QSTONE) ? QF_STONE : 0),
        ((n4 <= QSTONE) ? QF_STONE : 0));

    if (QF_PRESENT(wt)) setWorldAt(x, y, wt);
}


bool inList(int arr[100][2], int x, int y)
{
    for (int i = 0; i < 100; i++)
    {
        if (arr[i][0] == x && arr[i][1] == y)
        {
            return true;
        }
    }

    return false;
}

// given a coordinate, return the coord index of the neighbor with the smallest heightmap value associated
int getMinimumDirection(int x, int y)
{
    fp32 min = fixedpt_rconst(1000.0f);
    int mindir = 0;

    for (int i = 0; i < 8; i++)
    {
        fp32 height = generateNoise(x + dirs[i][0], y + dirs[i][1], overworld.noiseProps[HEIGHT_PROPERTIES]);
        if (height < min)
        {
            min = height;
            mindir = i;
        }
    }
    return mindir;
}

void placeRiver(int x, int y, int width)
{
    int visited[100][2];
    for (int i = 0; i < 100; i++) {visited[i][0] = 0; visited[i][1] = 0;} //init to zero

    int vind = 0; //visited index.
    int leadDir = getMinimumDirection(x, y); //initial momentum

    int hw = width / 2; 

    while (getAdjustedNoise(x, y) > SEA_LEVEL)
    {
        visited[vind][0] = x;
        visited[vind][1] = y;
        vind = vind + 1 < 100 ? vind + 1 : 0; //just roll over if we get past 100

        int mindir = getMinimumDirection(x, y);

        x += dirs[mindir][0];
        y += dirs[mindir][1];

        //if we already visited a point, just continue from the initial lead direction.
        if (inList(visited, x, y)) 
        {
            x -= dirs[mindir][0];
            y -= dirs[mindir][1]; //return to previous coord
            
            x += dirs[leadDir][0];
            y += dirs[leadDir][1];
        }

        switch (mindir)
        {
            case 0: //top & bottom
            case 4: 
                for (int i = 1; i <= hw; i++)
                {
                    setWorldAt(x + i, y, WT_WATER);
                    setWorldAt(x - i, y, WT_WATER);
                }
                break;
            case 2: // right & left
            case 6: 
                for (int i = 1; i <= hw; i++)
                {
                    setWorldAt(x, y + i, WT_WATER);
                    setWorldAt(x, y - i, WT_WATER);
                }
                break;
            default: // diagonals
                for (int i = 1; i <= hw; i++)
                {
                    // up right down left for hw iters
                    for (int j = 0; j < 8; j += 2)
                    {
                        setWorldAt(x + dirs[j][0] * i, y + dirs[j][1] * i, WT_WATER);
                    }
                }
                //put da water in each diagonal direction.
                for (int i = 1; i < 7; i += 2) setWorldAt(x + dirs[i][0], y + dirs[i][1], WT_WATER);
                break;
        }

        setWorldAt(x, y, WT_WATER);
    }
}

void generateOverworld()
{
    initOverworld();
    startLoadScreen(GT_GRASS, true, 2);
    for (int y = 0; y < MAP_DIM; y++)
    {
        for (int x = 0; x < MAP_DIM; x++)
        {
            setWorldTile(x, y);
        }
        progressMade();
    }

    int numRivers = qran_range(1, 3);

    for (int i = 0; i < numRivers; i++)
    placeRiver(qran_range(40, 75), qran_range(40, 75), qran_range(3, 5));

    placeStairs(overworld.numStairs, SF_DESC_STAIRS);
    endLoadScreen();
}
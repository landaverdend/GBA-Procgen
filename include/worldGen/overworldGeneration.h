#ifndef OVERWORLDGENERATION_H
#define OVERWORLDGENERATION_H
#include "worldGeneration.h"

//indices for noise properties array.
#define NOISE_PROPERTIES_SIZE 5
#define HEIGHT_PROPERTIES 0
#define RAINFALL_PROPERTIES 1
#define TEMPERATURE_PROPERTIES 2
#define QSTONE_PROPERTIES 3
#define QGRASS_PROPERTIES 4

//noise props definitions
#define HEIGHT_OCTAVES 2
#define HEIGHT_SCALE fixedpt_rconst(0.04f)
#define HEIGHT_LACUNARITY fixedpt_rconst(2.0f)
#define HEIGHT_PERSISTENCE fixedpt_rconst(0.2f)

#define RAINFALL_OCTAVES 2
#define RAINFALL_SCALE fixedpt_rconst(0.02f)
#define RAINFALL_LACUNARITY fixedpt_rconst(3.0f)
#define RAINFALL_PERSISTENCE fixedpt_rconst(0.2f)

#define QSTONE_OCTAVES 2
#define QSTONE_SCALE fixedpt_rconst(0.02f)
#define QSTONE_LACUNARITY fixedpt_rconst(3.0f)
#define QSTONE_PERSISTENCE fixedpt_rconst(0.2f)

#define QGRASS_OCTAVES 2
#define QGRASS_SCALE fixedpt_rconst(0.08f)
#define QGRASS_LACUNARITY fixedpt_rconst(4.0f)
#define QGRASS_PERSISTENCE fixedpt_rconst(0.1f)

//QUAD FEATURES
#define QSTONE fixedpt_rconst(0.45f)
#define QGRASS fixedpt_rconst(0.45f)

typedef struct Overworld
{
    NoiseProperties *noiseProps; //ptr to array
    int seed;
    int islandShape;
    int numStairs;
} Overworld;

void setWorldTile(int x, int y);
void placeQuadStone(int x, int y);


// rivers
int getMinimumDirection(int x, int y);
bool inList(int arr[100][2], int x, int y);
void placeRiver(int x, int y, int width);


fp32 getAdjustedNoise(int x, int y);

void initOverworld();
void generateOverworld();

extern Overworld overworld;

#endif
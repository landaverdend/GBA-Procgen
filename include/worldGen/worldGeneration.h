#ifndef WORLDGENERATION_H
#define WORLDGENERATION_H

#include "tonc.h"
#include "fpSimplex.h"
#include "fixedptc.h"
#include "miscHelp.h"

typedef unsigned char WORLDTYPE;


extern int WORLD_SEED;
extern int MAP_SHAPE;

//type of world to generate
#define OVERWORLD 0
#define CAVE 1

#define MAP_SQUARE 0
#define MAP_CIRCLE 1

#define MAP_DIM 128
#define MAP_WIDTH 128
#define MAP_HEIGHT 128
#define MAX_RAND 1000000


//Each aspect of procgen will have an associated noiseproperties struct to use upon noise generation.
typedef struct NoiseProperties
{
    Vector2D *octaveOffsets;
    int seed;
    int octaves;
    fp32 scale;
    fp32 lacunarity;
    fp32 persistence;
} NoiseProperties;

Vector2D *getVectorOffsets(int octaves);
void generateMap(WORLDTYPE wt);
void setWorldSeed(char *str);
void placeStairs(int numStairs, int id);
fp32 generateNoise(int x, int y, NoiseProperties np);
bool sfInArea(int x, int y, int sfID, int range);

#endif
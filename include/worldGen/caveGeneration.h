#ifndef CAVEGENERATION_H
#define CAVEGENERATION_H
#include "worldGeneration.h"
#include "tonc.h"

fp32 getCaveGradientValue(int x, int y);
void setCaveTile(int x, int y);
void generateCave();
bool placeCaveLiquid(int x, int y, int liquidType);
void placeCaveStone(int x, int y);
bool placeOre();

#endif
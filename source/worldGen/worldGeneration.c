#include <stdlib.h>
#include <string.h>
#include "worldGeneration.h"
#include "overworldGeneration.h"
#include "caveGeneration.h"
#include "tile.h"
#include "world.h"

int WORLD_SEED;
int MAP_SHAPE;

void setWorldSeed(char *str)
{

    int len = strlen(str);
    int ind = 0;
    while (str[ind] == ' ') ind++; //traverse until we get to our first none space character.
    int hash = 0;

    //https://docs.oracle.com/javase/6/docs/api/java/lang/String.html#hashCode()
    for (int i = ind; i < len; i++)
    {
        hash = 31 * hash + str[i];
    }

    WORLD_SEED = hash;
}


//Given the number of octaves, generate random offsets and return a pointer to an array
Vector2D *getVectorOffsets(int octaves)
{
    Vector2D *octaveOffsets = malloc(sizeof(Vector2D) * octaves);
    for (int i = 0; i < octaves; i++)
    {
        Vector2D temp = {
            .x = fixedpt_fromint((qran())), //also try a range of -100000 to 100000
            .y = fixedpt_fromint((qran()))};
        octaveOffsets[i] = temp;
    }
    return octaveOffsets;
}

bool sfInArea(int x, int y, int sfID, int range)
{
    int half = range >> 1;
    int xstart = x - half >= 0 ? x - half : 0;
    int ystart = y - half >= 0 ? y - half : 0;
    
    for (int i = ystart; i < ystart + range; i++)
    {
        for (int j = xstart; j < xstart + range; j++)
        {
            if (j > 127 || i > 127) break;
            if (GET_SF(j, i) == sfID) return true;
        }
    }

    return false;
}

void placeStairs(int numStairs, int id)
{
    while (numStairs > 0)
    {
        int x = qran_range(0, 127);
        int y = qran_range(0, 127);

        if (GET_QF(x, y, 1, 1) == QF_STONE && !sfInArea(x, y, id, 10))
        {
            setWorldAt(x, y, CONSTRUCT_SF_WT(GT_STONE, id));
            numStairs--;
        }
    }
}